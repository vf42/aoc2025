#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <tgmath.h>
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

    uint8_t *grid = NULL;

    char buff[1000];
    size_t row = 0, width = 0;
    while (fscanf(f, "%[.@]\n", buff) == 1) {
        // First line - now we know the grid size, assuming square.
        // Allocate the arrays.
        if (grid == NULL) {
            width = strlen(buff);
            size_t grid_size = width * width;
            grid = calloc(grid_size, sizeof(uint8_t));
        }
        for (size_t i = 0; i <= width; i++) {
            grid[row*width + i] = buff[i] == '@';
        }
        row++;
    }
    fclose(f);
    
    size_t accessible = 0;
    for (row = 0; row < width; row++) {
        for (size_t i = 0; i < width; i++) {
            if (!grid[row*width + i])
                continue;
            uint8_t neigh = 0;
            if (i > 0) {
                neigh += grid[row*width + i - 1];
                if (row > 0) {
                    neigh += grid[(row-1)*width + i - 1];
                }
                if (row < width - 1) {
                    neigh += grid[(row+1)*width + i - 1];
                }
            }
            if (i < width - 1) {
                neigh += grid[row*width + i + 1];
                if (row > 0) {
                    neigh += grid[(row-1)*width + i + 1];
                }
                if (row < width - 1) {
                    neigh += grid[(row+1)*width + i + 1];
                }
            }
            if (row > 0)
                neigh += grid[(row-1)*width + i];
            if (row < width - 1)
                neigh += grid[(row+1)*width + i];
            if (neigh < 4)
                ++accessible;
        }
    }
    printf("Result: %zd\n", accessible);

    return EXIT_SUCCESS;
}
