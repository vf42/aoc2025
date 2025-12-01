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
    int cnt, dial = 50, prev_dial; // Click count, curr pos.
    size_t zero_cnt = 0;
    while (fscanf(f, "%1c%d\n", dir, &cnt) != EOF) {
        if (cnt > 100) {
            // Adjust for full rotations.
            zero_cnt += cnt / 100;
            cnt %= 100;
        }

        if (dir[0] == 'L') cnt = -cnt;
        printf("%d : %d ", dial, cnt);

        prev_dial = dial;
        dial += cnt;
        if (dial < 0) {
            dial += 100;
            // Zero crossed going left.
            if (prev_dial) zero_cnt += 1;
        } else if (dial > 100 && prev_dial) {
            // Zero croseed going right.
            zero_cnt += 1;
        }
        dial = dial % 100;
        if (!dial) zero_cnt += 1;
        printf("--> %d, %zd\n", dial, zero_cnt);
    }
    fclose(f);

    printf("Solution: %zd\n", zero_cnt);
    return EXIT_SUCCESS;
}
