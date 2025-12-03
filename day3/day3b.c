#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t K = 12;

int main(int argc, char *argv[]) {
    char* fname = "test.txt";
    if (argc > 1) {
        fname = argv[1];
    }
    FILE* f = fopen(fname, "r");
    if (!f) {
        perror("No such file!\n");
        return EXIT_FAILURE;
    }

    char bank[101];
    uint64_t total_max = 0;
    while (fscanf(f, "%[0-9]\n", bank) == 1) {
        size_t bank_size = strlen(bank);
        for (size_t i = 0; i < bank_size; i++) {
            bank[i] -= '0';
        }

        uint8_t enabled_batt[K];
        for (size_t k = 0; k < K; k++) {
            size_t starting_point = k == 0 ? 0 : enabled_batt[k-1] + 1;
            enabled_batt[k] = starting_point;
            for (size_t i = starting_point + 1; i < bank_size - K + k + 1; i++) {
                if (bank[i] > bank[enabled_batt[k]]) {
                    enabled_batt[k] = i;
                }
            }
        }

        uint64_t bank_max = 0;
        for (size_t k = 0; k < K; k++) {
            // printf("%d ", bank[enabled_batt[k]]);
            bank_max = bank_max*10 + bank[enabled_batt[k]];
            // printf(" -> %llu\n", bank_max);
        }
        total_max += bank_max;
    }
    fclose(f);

    printf("Solution: %llu\n", total_max);
    return EXIT_SUCCESS;
}
