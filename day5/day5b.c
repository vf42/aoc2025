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

    size_t range_start[200];
    size_t range_end[200];
    size_t nranges = 0;
    char in_start[100], in_end[100];
    while (fscanf(f, "%[0-9]-%[0-9]\n", in_start, in_end) == 2) {
        size_t rstart = strtoul(in_start, NULL, 10);
        size_t rend = strtoul(in_end, NULL, 10);
        for (size_t i = 0; i < nranges; i++) {
            if (rstart < range_start[i]) {
                size_t tmp = range_start[i];
                range_start[i] = rstart;
                rstart = tmp;
                tmp = range_end[i];
                range_end[i] = rend;
                rend = tmp;
            }
        }
        range_start[nranges] = rstart;
        range_end[nranges] = rend;
        nranges++;
    }

    // Can skip reading the rest for this part.
    fclose(f);

    // Merge ranges until there's no more to merge.
    size_t overlap, start_pos = 1;
    do {
        overlap = 0;
        for (size_t i = start_pos; i < nranges; i++) {
            if (!overlap && range_start[i] <= range_end[i-1] + 1) {
                if (range_end[i] > range_end[i-1])
                    range_end[i-1] = range_end[i];
                overlap = 1;
                start_pos = i;
            }
            if (overlap && i < nranges - 1) {
                range_start[i] = range_start[i+1];
                range_end[i] = range_end[i+1];
            }
        }
        if (overlap) {
            --nranges;
        }
    } while (overlap > 0);

    // Count numbers in ranges.
    size_t nnumbers = 0;
    for (size_t i = 0; i < nranges; i++) {
        if (range_end[i] > range_start[i])
            nnumbers += range_end[i] - range_start[i] + 1;
    }

    printf("Solution: %zd\n", nnumbers);
    return EXIT_SUCCESS;
}
