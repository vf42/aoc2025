#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define S 30

void incr(char input[S]);

inline void incr(char input[S]) {
    size_t len = strlen(input);
    size_t i = len - 1;
    bool carry = true;
    while (carry) {
        // printf("%s\n", input);
        if (i == __SIZE_MAX__) {
            // We need to extend the string length.
            memmove(input+1, input, len+1);
            ++len;
            input[0] = '1';
            carry = false;
        } else if (input[i] == '9') {
            input[i] = '0';
            carry = true;
        } else {
            input[i] += 1;
            carry = false;
        }
        --i;
    }
}

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

    char lower[S], upper[S], delim[1];
    size_t invalids = 0;
    unsigned long long invsum = 0;

    while (fscanf(f, "%[0-9]-%[0-9]%c\n", lower, upper, delim) != EOF) {
        // printf("%s - %s\n", lower, upper);
        while (true) {
            size_t len = strlen(lower);
            if (!(len % 2) && !strncmp(lower, lower+(len/2), len/2)) {
                ++invalids;
                unsigned long long value = strtoull(lower, NULL, 10);
                invsum += value;
                // printf("Found %lld, current %lld\n", value, invsum);
            }
            if (!strcmp(lower, upper))
                break;
            incr(lower);
        }
    }
    fclose(f);
    printf("Result: %lld, %zd\n", invsum, invalids);

    return EXIT_SUCCESS;
}
