RAM_SIZE = 1024


class RAM:
    def __init__(self, size=RAM_SIZE):
        self._minAddr = 0
        self._maxAddr = RAM_SIZE - 1
        self._memory = []   # a list of values.  Could be #s or instructions.
        for i in range(size):
            self._memory.append(0)

    def __getitem__(self, addr):
        return self._memory[addr]

    def __setitem__(self, addr, val):
        self._memory[addr] = val

    def is_legal_addr(self, addr):
        return self._minAddr <= addr <= self._maxAddr

class MMU:
    def __init__(self, ram):
        self._ram = ram
        self.reloc = 0
        self.limit = 0

    def __translate_addr(self, logical_addr):
        if (logical_addr >= self.limit):
            print("BAD ADDRESS: %d too high" % (logical_addr))
        return logical_addr + self.reloc

    def get_value(self, logical_addr):
        physical_addr = self.__translate_addr(logical_addr)
        return self._ram.__getitem__(physical_addr)

    def set_value(self, logical_addr, value):
        physical_addr = self.__translate_addr(logical_addr)
        self._ram.__setitem__(physical_addr, value)

    def set_reloc_register(self, reloc_addr):
        self.reloc = reloc_addr

    def set_limit_register(self, limit):
        self.limit = limit
    