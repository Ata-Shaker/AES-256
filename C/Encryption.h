#pragma once
#include "Global.h"

void SubBytes(byte state[16])
{
    for (int i = 0; i < 16; i++)
    {
        unsigned int col = state[i] & 15;
        unsigned int temp = state[i] & 240;
        unsigned int row = temp >> 4;
        state[i] = SBox[row][col];
    }
}

void ShiftRows(byte state[16])
{
     
    byte temp = state[4];
    for (int i = 0; i < 3; i++)
        state[i + 4] = state[i + 5];
    state[7] = temp;

     
    for (int i = 0; i < 2; i++)
    {
        temp = state[i + 8];
        state[i + 8] = state[i + 10];
        state[i + 10] = temp;
    }

     
    temp = state[15];
    for (int i = 3; i > 0; i--)
        state[i + 12] = state[i + 11];
    state[12] = temp;
}

void MixColumns(byte state[16])
{
    byte arr[4];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            arr[j] = state[i + j * 4];

        state[i] = GFMul(0x02, arr[0]) ^ GFMul(0x03, arr[1]) ^ arr[2] ^ arr[3];
        state[i + 4] = arr[0] ^ GFMul(0x02, arr[1]) ^ GFMul(0x03, arr[2]) ^ arr[3];
        state[i + 8] = arr[0] ^ arr[1] ^ GFMul(0x02, arr[2]) ^ GFMul(0x03, arr[3]);
        state[i + 12] = GFMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ GFMul(0x02, arr[3]);
    }
}

void enc(byte input[4 * NB], word w[4 * (NR+ 1)], byte IV[])
{
    for (int i = 0; i < 16; i++)
        input[i] = input[i] ^ IV[i];

    word key[NK];
    for (int i = 0; i < NK; ++i)
        key[i] = w[i];

    AddRoundKey(input, key);

    for (int round = 1; round < NR; round++)
    {
        SubBytes(input);
        ShiftRows(input);
        MixColumns(input);
        for (int i = 0; i < NK; ++i)
            key[i] = w[4 * round + i];
        AddRoundKey(input, key);
    }

    SubBytes(input);
    ShiftRows(input);
    for (int i = 0; i < NK; ++i)
        key[i] = w[4 * NR+ i];
    AddRoundKey(input, key);
}

void encrypt(byte plain[], byte IV[], word w[4 * (NR + 1)], int size)
{
    int s = size;
    int len = s;

    s /= 16;
    byte tempArr1[16];

    int i = 0;
    for (i = 0; i < s; i++) {
        if (i==s-1)
        {
            printf("%d percent", ((int)((float)i + 1) * 100 / s));
        }
        else
        {
            printf("%d percent ->", ((int)((float)i + 1) * 100 / s));
        }
        arrcopy(plain, 16 * i, 16, tempArr1, 0);
        enc(tempArr1, w, IV);
        arrcopy(tempArr1, 0, 16, IV, 0);
        arrcopy(tempArr1, 0, 16, plain, 16 * i);
    }
}