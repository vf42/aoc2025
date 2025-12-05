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

    // The previous fscanf read the first wire value
    // from the next block! Seek back to the start of the line.
    size_t read_result;
    char value[1];
    do {
        fseek(f, -2, SEEK_CUR);
        read_result = fread(value, 1, 1, f);
    } while (read_result > 0 && value[0] != '\n');

    size_t nfresh = 0;
    while (fscanf(f, "%[0-9]\n", in_start) == 1) {
        size_t id = strtol(in_start, NULL, 10);
        for (size_t i = 0; i < nranges; i++) {
            if (id >= range_start[i] && id <= range_end[i]) {
                ++nfresh;
                break;
            }
        }
    }

    fclose(f);

    printf("Solution: %zd\n", nfresh);
    return EXIT_SUCCESS;
}
