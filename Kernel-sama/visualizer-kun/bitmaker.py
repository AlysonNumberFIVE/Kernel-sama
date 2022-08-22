



if __name__ == "__main__":
#    print(bin(4294967295))
#    value = 0b10000000000000000000000000000001
   # print(bin(value))
   # print(bin(900))
   # value = 4294967295
    first = (0b100100000000001100000000 >> 8)
    value = ((0b100100000000001100000000 >> 8) & first) & 0xff
    print(bin(value)) 
    value = 4265
    print(bin(value))
    # mask = 900
    # print(bin(mask))
    # print(bin((value | mask) << 22))
    # print(0b10011100001110000000000000000000000)