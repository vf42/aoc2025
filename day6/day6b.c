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

    size_t ncols = 0, nrows = 0, col = 0;
    char inbuff[1024];
    size_t nread, totread = 0;
    char data[20000];
    while ((nread = fread(inbuff, sizeof(char), 100, f)) != 0) {
        memcpy(data+totread, inbuff, sizeof(char)*nread);
        for (size_t i = 0; i < nread; i++) {
            ++col;
            if (inbuff[i] == '\n') {
                if (!ncols) ncols = col;
                col = 0;
                ++nrows;
            }
        }
        totread += nread;
    }
    fclose(f);

    unsigned long long total = 0;
    unsigned long long nums[100];
    unsigned long long nnums = 0;
    for (int c = ncols-1; c >= 0; c--) {
        unsigned long long cnum = 0;
        for (size_t r = 0; r < nrows; r++) {
            char sym = data[r*ncols + c];
            if (sym == '\n' || sym == 0) {
                break;
            } else if (sym >= '0' && sym <= '9') {
                cnum = cnum*10 + sym - '0';
            } else if (sym == '+' || sym == '*') {
                nums[nnums++] = cnum;

                if (sym == '+') {
                    unsigned long long sum = 0;
                    for (size_t i = 0; i < nnums; i++)
                        sum += nums[i];
                    total += sum;
                } else {
                    unsigned long long prod = 1;
                    for (size_t i = 0; i < nnums; i++)
                        prod *= nums[i];
                    total += prod;
                }

                nnums = 0;
                cnum = 0;
            }
        }
        if (cnum > 0) {
            nums[nnums++] = cnum;
        }
    }

    printf("Solution: %llu\n", total);
    return EXIT_SUCCESS;
}
