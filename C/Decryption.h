#pragma once
#include "Global.h"

void InvSubBytes(byte state[16])
{
    for (int i = 0; i < 16; ++i)
    {
        unsigned int col = state[i] & 15;
        unsigned int temp = state[i] & 240;
        unsigned int row = temp >> 4;
        state[i] = rSBox[row][col];
    }
}

void InvShiftRows(byte state[4 * 4])
{
    
    byte temp = state[7];
    for (int i = 3; i > 0; --i)
        state[i + 4] = state[i + 3];
    state[4] = temp;
    
    for (int i = 0; i < 2; ++i)
    {
        temp = state[i + 8];
        state[i + 8] = state[i + 10];
        state[i + 10] = temp;
    }
     
    temp = state[12];
    for (int i = 0; i < 3; ++i)
        state[i + 12] = state[i + 13];
    state[15] = temp;
}

void InvMixColumns(byte state[4 * 4])
{
    byte arr[4];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            arr[j] = state[i + j * 4];

        state[i] = GFMul(0x0e, arr[0]) ^ GFMul(0x0b, arr[1]) ^ GFMul(0x0d, arr[2]) ^ GFMul(0x09, arr[3]);
        state[i + 4] = GFMul(0x09, arr[0]) ^ GFMul(0x0e, arr[1]) ^ GFMul(0x0b, arr[2]) ^ GFMul(0x0d, arr[3]);
        state[i + 8] = GFMul(0x0d, arr[0]) ^ GFMul(0x09, arr[1]) ^ GFMul(0x0e, arr[2]) ^ GFMul(0x0b, arr[3]);
        state[i + 12] = GFMul(0x0b, arr[0]) ^ GFMul(0x0d, arr[1]) ^ GFMul(0x09, arr[2]) ^ GFMul(0x0e, arr[3]);
    }
}

void dec(byte input[16], word w[4 * (NR + 1)], byte IV[])
{
    word key[NK];
    for (int i = 0; i < NK; ++i)
        key[i] = w[4 * NR + i];
    
    AddRoundKey(input, key);

    for (int round = NR - 1; round > 0; --round)
    {
        InvShiftRows(input);
        InvSubBytes(input);
        for (int i = 0; i < NK; ++i)
            key[i] = w[4 * round + i];
        AddRoundKey(input, key);
        InvMixColumns(input);
    }

    InvShiftRows(input);
    InvSubBytes(input);
    for (int i = 0; i < NK; ++i)
        key[i] = w[i];
    
    AddRoundKey(input, key);

    for (int i = 0; i < 16; i++)
        input   [i] = input[i] ^ IV[i];
}

void decrypt(byte cipherText[], byte IV[], word w[4 * (NR + 1)], int size)
{
    int s = size;
    int len = s;
    s /= 16;
    byte tempArr1[16] = { 0 };
    int i = 0;
    for (i = 0; i < s; i++) {
        if (i == s - 1)
        {
            printf("%d percent", ((int)((float)i + 1) * 100 / s));
        }
        else
        {
            printf("%d percent ->", ((int)((float)i + 1) * 100 / s));
        };
        arrcopy(cipherText, 16 * i, 16, tempArr1, 0);
        dec(tempArr1, w, IV);
        arrcopy(cipherText, 16 * i, 16, IV, 0);
        arrcopy(tempArr1, 0, 16, cipherText, 16 * i);
    }
}
