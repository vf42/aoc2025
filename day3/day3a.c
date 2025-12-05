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

    char bank[100];
    uint32_t total_max = 0;
    while (fscanf(f, "%[0-9]\n", bank) == 1) {
        uint8_t prev_max = bank[0] - '0';
        uint32_t bank_max = 0;
        for (size_t i = 1; i < strlen(bank); i++) {
            uint8_t curr = bank[i] - '0';
            uint8_t bank_curr = prev_max*10 + curr;
            if (bank_curr > bank_max) bank_max = bank_curr;
            if (curr > prev_max) prev_max = curr;
        }
        total_max += bank_max;
    }
    fclose(f);

    printf("Solution: %d\n", total_max);
    return EXIT_SUCCESS;
}
