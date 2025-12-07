#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define S 150

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

    char *currl, *prevl;
    currl = calloc(S, sizeof(char));
    prevl = calloc(S, sizeof(char));

    size_t *currt, *prevt;
    prevt = calloc(S, sizeof(size_t));
    currt = prevt;

    size_t line = 0, splits = 0;
    while (fscanf(f, "%[.|^S]\n", currl) == 1) {
        currt = calloc(S, sizeof(size_t));
        for (size_t i = 0; line > 0 && i < S; i++) {
            switch (currl[i]) {
                case 'S':
                case '|':
                    currt[i] += prevt[i];
                    break;
                case '.':
                    if (prevl[i] == 'S') {
                        currt[i] = 1;
                        currl[i] = '|';
                    } else if (prevl[i] == '|') {
                        currt[i] += prevt[i];
                        currl[i] = '|';
                    }
                    break;
                case '^':
                    if (prevl[i] == 'S' || prevl[i] == '|') {
                        if (i > 0) {
                            currt[i-1] += prevt[i];
                            currl[i-1] = '|';
                        }
                        if (currl[i+1] != 0) {
                            currt[i+1] += prevt[i];
                            currl[i+1] = '|';
                        }
                        ++splits;
                    }
                    break;
                default:
                    i = S;
                    break;
            }
        }

        char* tmp = prevl;
        prevl = currl;
        currl = tmp;
        if (line > 0) {
            free(prevt);
            prevt = currt;
        }
        line++;
    }

    fclose(f);
    free(currl);
    free(prevl);

    printf("Solution 1: %zd\n", splits);

    size_t total_timelines = 0;
    for (size_t i = 0; i < S; i++) {
        total_timelines += currt[i];
    }
    free(currt);
    printf("Solution 2: %zd\n", total_timelines);

    return EXIT_SUCCESS;
}
