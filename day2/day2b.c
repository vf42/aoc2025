#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define S 30

void incr(char input[S]);
bool test(char input[S]);

inline void incr(char input[S]) {
    size_t len = strlen(input);
    size_t i = len - 1;
    bool carry = true;
    while (carry) {
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

inline bool test(char input[S]) {
    size_t len = strlen(input);
    for (size_t csize = 1; csize <= len / 2; csize++) {
        if (len % csize)
            continue;
        size_t chunks = len / csize;
        bool good = true;
        for (size_t i = 0; good && i < csize; i++) {
            for (size_t c = 1; good && c < chunks; c++) {
                size_t j = i + c*csize;
                if (input[i] != input[j])
                    good = false;
            }
        }
        if (good)
            return true;
    }
    return false;
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
            if (test(lower)) {
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
