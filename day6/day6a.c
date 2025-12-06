#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    char* fname = "test.txt";
    if (argc > 1) {
        fname = argv[1];
    }
    FILE* f = fopen(fname, "r");
    if (!f) {
        printf("No such file!\n");
        return EXIT_FAILURE;
    }

    unsigned long long nums[10000];
    char ops[1000];
    size_t ncols = 0;
    size_t nrows = 0, row = 0;
    char inbuff[100], valbuff[100];
    size_t nread;
    size_t col = 0;
    valbuff[0] = 0;
    size_t valsize = 0;
    while ((nread = fread(inbuff, sizeof(char), 100, f)) != 0) {
        for (size_t i = 0; i < nread; i++) {
            if ((inbuff[i] >= '0' && inbuff[i] <= '9')
                || inbuff[i] == '+' || inbuff[i] == '*') {
                valbuff[valsize++] = inbuff[i];
                valbuff[valsize] = 0;
            } else if ((inbuff[i] == ' ' || inbuff[i] == '\n')
                    && valsize > 0) {
                if (valbuff[0] == '*' || valbuff[0] == '+') {
                    ops[col] = valbuff[0];
                } else {
                    nums[row*ncols + col] = strtoull(valbuff, NULL, 10);
                }
                valbuff[0] = 0;
                valsize = 0;
                ++col;
            }
            if (inbuff[i] == '\n') {
                if (!ncols) ncols = col;
                col = 0;
                ++row;
                ++nrows;
            }
        }
    }
    fclose(f);

    unsigned long long total = 0;
    for (size_t c = 0; c < ncols; c++) {
        unsigned long long curr = ops[c] == '*' ? 1 : 0;
        for(size_t r = 0; r < nrows; r++) {
            unsigned long long val = nums[r*ncols + c];
            if (ops[c] == '*')
                curr *= val;
            else
                curr += val;
        }
        printf("%c %llu\n", ops[c], curr);
        total += curr;
    }

    printf("Solution: %llu\n", total);
    return EXIT_SUCCESS;
}
