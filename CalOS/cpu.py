'''CPU class for CalOS.

@author Victor Norman
@date 12/26/17

@author (editor) Nikita Sietsema
@date 6 February 2020
'''

import time
import threading   # for CPU

# Time to delay between executing instructions, in seconds.
DELAY_BETWEEN_INSTRUCTIONS = 0.2

class CPU(threading.Thread):
    def __init__(self, ram, os, startAddr, debug, num = 0):
        threading.Thread.__init__(self)

        self._num = num   # unique ID of this cpu
        self._registers = {
            'reg0' : 0,
            'reg1' : 0,
            'reg2' : 0,
            'pc': startAddr
            }

        self._ram = ram
        self._os = os
        self._debug = debug

        # NS - Default batchMode is false.
        # batchMode is updated when provided a valid program_list_addr
        self._isBatchMode = False
        self._program_list_addr = set()

        # TODO: need to protect these next two variables as they are shared
        # between the CPU thread and the device threads.
        self._intr_raised = False
        self._intr_addrs = set()
        
        self._intr_vector = [self._kbrd_isr,
                             self._screen_isr]

        # Dictionary of registers and their values, that is used when
        # an interrupt occurs and the current state of the CPU needs to be
        # stored.
        self._backup_registers = {}

    def set_pc(self, pc):
        # TODO: check if value of pc is good?
        self._registers['pc'] = pc

    def set_interrupt(self, intr_val):
        '''Set the interrupt line to be True if an interrupt is raised, or
        False to indicate the interrupt is cleared.
        '''
        assert isinstance(intr_val, bool)
        self._intr_raised = intr_val

    # NS - Set the addr responsible for storing the list of program starting addresses
    def set_program_list_addr(self, addr):
        self._isBatchMode = True
        self._program_list_addr = addr

    def add_interrupt_addr(self, addr):
        '''Add the device bus address to the set of devices that have
        raised an interrupt.'''
        self._intr_addrs.add(addr)

    def backup_registers(self):
        self._backup_registers = self._registers

    def restore_registers(self):
        self._registers = self._backup_registers

    # NS - Clear registers 0 - 2 to 0
    # PC does not change
    def clear_registers(self):
        self._registers = {
            'reg0' : 0,
            'reg1' : 0,
            'reg2' : 0,
            'pc': self._registers['pc']
            }

    def isregister(self, s):
        return s in ('reg0', 'reg1', 'reg2', 'pc')

    def __str__(self):
        res = '''CPU {}: pc {}, reg0 {}, reg1 {}, reg2 {}'''.format(
            self._num, self._registers['pc'], self._registers['reg0'],
            self._registers['reg1'], self._registers['reg2'])
        return res

    def run(self):
        '''Overrides run() in thread.  Called by calling start().
            NS - Handles both batch and single run modes.'''

        # NS - Batch mode should loop through a list of addresses corresponding to programs to run
        if self._isBatchMode:
            # NS - currentListAddr is our addr (index) in the list of program addresses
            currentListAddr = self._program_list_addr
            counter = 0

            while True:
                # NS - Ensure our currentListAddr is valid
                if not self._ram.is_legal_addr(currentListAddr):
                    print("ERROR: Illegal Address in RAM: {}".format(currentListAddr))
                    break

                # NS - Extract a programAddr from the list of addresses
                programAddr = self._ram[currentListAddr]

                # NS - Ensure our programAddr is also valid
                if not self._ram.is_legal_addr(programAddr):
                    print("ERROR: Illegal Address in RAM: {}".format(programAddr))
                    break

                # NS - 0 indicates the end of our list
                # Return to single mode and stop executing programs
                if programAddr == 0:
                    self._isBatchMode = False
                    print("All programs have been run.\n")
                    break

                # NS - Move pc to program start addr and begin executing program
                else:
                    self.set_pc(programAddr)
                    currentListAddr += 1 # NS - Get next program addr

                    # NS - Begin loader
                    print("\nProcessing [", end="", flush=True)

                    self._run_program()
                    self.clear_registers()

                    # NS - Finish loader
                    print("]\nFinished program %d\n" % counter)
                    counter += 1

        # NS - We only have one program to run
        else:
            self._run_program()
        

    def _run_program(self):
        while True:
            if self._debug:
                print("Executing code at [%d]: %s" % (self._registers['pc'],
                                                      self._ram[self._registers['pc']]))
            if not self.parse_instruction(self._ram[self._registers['pc']]):
                # False means an error occurred or the program ended, so return
                break

            # print CPU state
            if self._debug: print(self)

            # Now, check if an interrupt has been raised.  If it has, run the
            # corresponding handler(s).
            # TODO: critical section below!
            if self._intr_raised:
                if self._debug: print("GOT INTERRUPT")
                self.backup_registers()
                for addr in sorted(self._intr_addrs):
                    # Call the interrupt handler.
                    self._intr_vector[addr]()
                    # Remove the device address from the list of pending interrupts.
                    self._intr_addrs.remove(addr)
                
                # Mark all interrupts handled.
                self.restore_registers()
                self.set_interrupt(False)  # clear the interrupt
            
            time.sleep(DELAY_BETWEEN_INSTRUCTIONS)

    def parse_instruction(self, instr):
        '''return False when program is done'''

        # Make sure it is an instruction.  The PC may have wandered into
        # data territory.
        if isinstance(instr, int):
            print("ERROR: Not an instruction: {}".format(instr))
            return False

        # NS - Show loader progress
        if self._isBatchMode:
            print("---", end="", flush=True)

        instr = instr.replace(",", "")
        words = instr.split()
        instr = words[0]
        if len(words) == 2:
            dst = words[1]    # for jmp and call.
        elif len(words) == 3:
            src = words[1]
            dst = words[2]

        if instr == "call":
            # Call a python function.  Syntax is
            # call fname.  Function fname is a method in 
            # CalOS class and is called with the values in reg0, reg1, and reg2.
            self.handle_call(dst)
            self._registers['pc'] += 1
        elif instr == "mov":
            self.handle_mov(src, dst)
            self._registers['pc'] += 1
        elif instr == 'add':
            self.handle_add(src, dst)
            self._registers['pc'] += 1
        elif instr == 'sub':
            self.handle_sub(src, dst)
            self._registers['pc'] += 1
        elif instr == 'jez':
            self.handle_jez(src, dst)
        elif instr == 'jnz':
            self.handle_jnz(src, dst)
        elif instr == 'jgz':
            self.handle_jgz(src, dst)
        elif instr == 'jlz':
            self.handle_jlz(src, dst)
        elif instr == 'jmp':
            self.handle_jmp(dst)
        elif instr == 'end':
            return False
        return True
        

    # TODO: do error checking in all these.
    # Could check for illegal addresses, etc.
    def handle_jmp(self, dst):
        if self.isregister(dst):
            self._registers['pc'] = self._registers[dst]
        else:
            self._registers['pc'] = eval(dst)
        
    def handle_jez(self, src, dst):
        if not self.isregister(src):
            print("Illegal instruction")
            return
        if self._registers[src] == 0:
            if self.isregister(dst):
                self._registers['pc'] = self._registers[dst]
            else:
                self._registers['pc'] = eval(dst)
        else:
            self._registers['pc'] += 1
            
    def handle_jnz(self, src, dst):
        if not self.isregister(src):
            print("Illegal instruction")
            return
        if self._registers[src] != 0:
            if self.isregister(dst):
                self._registers['pc'] = self._registers[dst]
            else:
                self._registers['pc'] = eval(dst)
        else:
            self._registers['pc'] += 1
            
    def handle_jlz(self, src, dst):
        if not self.isregister(src):
            print("Illegal instruction")
            return
        if self._registers[src] < 0:
            if self.isregister(dst):
                self._registers['pc'] = self._registers[dst]
            else:
                self._registers['pc'] = eval(dst)
        else:
            self._registers['pc'] += 1
            
    def handle_jgz(self, src, dst):
        if not self.isregister(src):
            print("Illegal instruction")
            return
        if self._registers[src] > 0:
            if self.isregister(dst):
                self._registers['pc'] = self._registers[dst]
            else:
                self._registers['pc'] = eval(dst)
        else:
            self._registers['pc'] += 1

    def _get_value_at(self, addr):
        '''addr is "*<someval>".  return the value from
        RAM at the addr, which might be decimal
        or hex.'''
        addr = eval(addr[1:])
        return self._ram[addr]

    def _get_srcval(self, src):
        if self.isregister(src):
            return self._registers[src]
        elif src[0] == '*':
            return self._get_value_at(src)
        else:   # assume src holds a literal value
            return eval(src)    # handles decimal and hex values.
            # TODO: does the above handle putting strings in memory too?  It should
            # allow single characters, perhaps.


    def handle_mov(self, src, dst):
        '''move value from a src to a dst.  src can be one of:
        literal value:          5
        value in memory:        *4
        value in register:      reg2
        dst can be one of:
        memory location:        4
        register name:          reg1
        memory location in reg: *reg1
        You cannot mov a value from RAM into RAM: you must use
        a register.
        '''
        srcval = self._get_srcval(src)

        if self.isregister(dst):
            self._registers[dst] = srcval
        elif dst[0] == '*':    # for *<register>
            if self.isregister(dst[1:]):
                self._ram[self._registers[dst[1:]]] = srcval
            else:
                print("Illegal instruction")
                return
        else:   # assume dst holds a literal value
            self._ram[eval(dst)] = srcval

    def handle_add(self, src, dst):
        srcval = self._get_srcval(src)

        if self.isregister(dst):
            self._registers[dst] += srcval
        elif dst[0] == '*':    # for *<register>
            if self.isregister(dst[1:]):
                self._ram[self._registers[dst[1:]]] += srcval
            else:
                print("Illegal instruction")
                return
        else:   # assume dst holds a literal value
            self._ram[eval(dst)] += srcval

                 
    def handle_sub(self, src, dst):
        srcval = self._get_srcval(src)

        if self.isregister(dst):
            self._registers[dst] -= srcval
        elif dst[0] == '*':    # for *<register>
            if self.isregister(dst[1:]):
                self._ram[self._registers[dst[1:]]] -= srcval
            else:
                print("Illegal instruction")
                return
        else:   # assume dst holds a literal value
            self._ram[eval(dst)] -= srcval

    def handle_call(self, fname):
        self._os.syscall(fname, self._reg0, self._reg1, self._reg2)


    def _kbrd_isr(self):
        # Read the value from the register in ram.
        key = self._ram[999]
        # Clear the data-in register
        self._ram[999] = 0
        print("Keyboard interrupt detected! location 999 holds", key)
        

    def _screen_isr(self):
        print("Screen interrupt detected!")
