#pragma once
#include "Global.h"

word Rcon[8] = { 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
                 0x20000000, 0x40000000, 0x80000000};

word Word(byte k1, byte k2, byte k3, byte k4)
{
    //{k1 k2 k3 k4}
    word result = 0x00000000;
    word temp;

    temp = k1;  //K1
    temp <<= 24;
    result |= temp;

    temp = k2;  // K2
    temp <<= 16;
    result |= temp;

    temp = k3;  // K3
    temp <<= 8;
    result |= temp;

    temp = k4;  // K4
    result |= temp;
    return result;
};

word RotWord(word rw)  
{
    word high = rw << 8;
    word low = rw >> 24;
    return high | low;
}

byte SubByte(byte b)
{
    unsigned int col  = b & 15;
    unsigned int temp = b & 240;
    unsigned int row = temp >> 4;
    return SBox[row][col];
}

word SubWord(word sw)
{
    word temp = sw;
    byte b1 = temp;
    temp >>= 8;
    byte b2 = temp;
    temp >>= 8;
    byte b3 = temp;
    temp >>= 8;
    byte b4 = temp;
    return Word(SubByte(b4), SubByte(b3), SubByte(b2), SubByte(b1));
}

void KeyExpansion(byte key[4 * NK], word w[4 * (NR + 1)])
{
    word temp;
    int i = 0;
    while (i < NK)
    {
        w[i] = Word(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
        i++;
    }

    i = NK;

    while (i < NB * (NR + 1))
    {
        temp = w[i - 1]; 
        if (i % NK == 0)
            temp = SubWord(RotWord(temp)) ^ Rcon[i / NK - 1];
        else if (i % NK == 4)
            temp = SubWord(temp);
        w[i] = w[i - NK] ^ temp;
        i++;
    }
}