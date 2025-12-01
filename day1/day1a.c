#include <stdio.h>
#include <stdlib.h>

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

    char dir[1]; // Rotation direction.
    int cnt, dial = 50; // Click count, curr pos.
    size_t zero_cnt = 0;
    while (fscanf(f, "%1c%d\n", dir, &cnt) != EOF) {
        if (dir[0] == 'L') cnt = -cnt;
        if ((dial += cnt) < 0) dial += 100;
        dial = dial % 100;
        if (!dial) zero_cnt += 1;
    }
    fclose(f);

    printf("Solution: %zd\n", zero_cnt);
    return EXIT_SUCCESS;
}
