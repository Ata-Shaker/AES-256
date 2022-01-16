#include <stdio.h>
#include <time.h>
#include "AES.h"

int main() {
    byte IV1[16];
    byte IV2[16];
    arrcopy(IV, 0, 16, IV1, 0);
    arrcopy(IV, 0, 16, IV2, 0);
    word w[4 * (NR + 1)];

    int option;
    char key[32] = {"abcdefghijklmnopqrstuvwxyz123456"}, src[100], dest[100];
    byte key_copy[32];

    printf("Enter 0 to encrypt and 1 to decrypt\n>> ");
    scanf(" %d", &option);

    clock_t begin = clock();

    if(option == 0) {
        printf("Source: ");
        scanf(" %s", &src);

        printf("Destination: ");
        scanf(" %s", &dest);

        for(int k = 0; k < 32; k++) {
            key_copy[k] = key[k];
        }

        KeyExpansion(key_copy, w);

        FILE* fsrc;
        fsrc = fopen(src, "r");
        FILE* fdest;
        fdest = fopen(dest, "w");
        fseek(fsrc, 0, SEEK_END);
        int sz = ftell(fsrc);
        fseek(fsrc, 0, SEEK_SET);
        byte data[10000];
        int i = 0, index = 0;
        for(int c = 0; c < sz; c++) {
            data[i] = (byte) fgetc(fsrc);
            i++;
            index++;
        }

        for(int j = 0; j < i % 16; j++) {
            data[i + j] = 0;
        };

        encrypt(data, IV1, w, index);
        
        for(int z = 0; z < index; z++) {
            fputc(data[z], fdest);
        }
        
        fclose(fdest);
        fclose(fsrc);
        printf("\nDone!\n");
        begin = clock() - begin;
        printf("Finished in %f seconds.\n", ((float)begin/CLOCKS_PER_SEC));
    } else if(option == 1) {
        printf("Source: ");
        scanf(" %s", &src);

        printf("Destination: ");
        scanf(" %s", &dest);

        for(int k = 0; k < 32; k++) {
            key_copy[k] = key[k];
        }

        KeyExpansion(key_copy, w);

        FILE* fsrc;
        fsrc = fopen(src, "r");
        FILE* fdest;
        fdest = fopen(dest, "w");
        fseek(fsrc, 0, SEEK_END);
        int sz = ftell(fsrc);
        fseek(fsrc, 0L, SEEK_SET);
        byte data[10000];
        
        int i = 0, index = 0;
        for(int c = 0; c < sz; c++) {
            data[i] = (byte) fgetc(fsrc);
            i++;
            index++;
        }
        
        for(int j = 0; j < i % 16; j++) {
            data[i + j] = 0;
        };

        decrypt(data, IV1, w, sz);
        
        for(int z = 0; z < index; z++) {
            fputc(data[z], fdest);
        }

        fclose(fdest);
        fclose(fsrc);
        printf("\nDone!\n");
        begin = clock() - begin;
        printf("Finished in %f seconds.\n", ((float)begin/CLOCKS_PER_SEC));
    } else {
        printf("Invalid option. Please try again.\n");
    }

    return 0;
};
    