#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define S 1000

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

typedef struct Connection {
    double dist;
    size_t box1;
    size_t box2;
} Connection;

int comp_connections(const void* a, const void* b) {
    Connection *ca = (Connection*)a;
    Connection *cb = (Connection*)b;
    if (ca->dist < cb->dist)
        return -1;
    if (cb->dist < ca->dist)
        return 1;
    return 0;
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

    // Read coords.
    size_t nboxes = 0;
    double coords[S*3];
    while (fscanf(f, "%lf,%lf,%lf\n",
            &coords[nboxes*3],
            &coords[nboxes*3+1],
            &coords[nboxes*3+2]) == 3) {
        ++nboxes;
    }
    fclose(f);

    // Calculate distances.
    size_t nconn = 0;
    Connection *conns = malloc(sizeof(Connection) * S*S);
    for (size_t i = 0; i < nboxes - 1; i++) {
        for (size_t j = i+1; j < nboxes; j++) {
            conns[nconn].dist =
                pow(coords[i*3] - coords[j*3], 2.0) +
                pow(coords[i*3+1] - coords[j*3+1], 2.0) +
                pow(coords[i*3+2] - coords[j*3+2], 2.0);
             conns[nconn].box1 = i;
             conns[nconn].box2 = j;
             ++nconn;
        }
    }

    // Sort the connections.
    qsort(conns, nconn, sizeof(Connection), &comp_connections);

    // Init circuit numbers.
    size_t circuit[nboxes];
    for (size_t i = 0; i < nboxes; i++) {
        circuit[i] = i+1;
    }

    // Make connections.
    for (size_t c = 0; c < nconn; c++) {
        if (circuit[conns[c].box1] != circuit[conns[c].box2]) {
            size_t to_circuit = MIN(
                circuit[conns[c].box1], circuit[conns[c].box2]);
            size_t from_circuit = MAX(
                circuit[conns[c].box1], circuit[conns[c].box2]);
            // We don't know how many boxes are in from_circuit,
            // So got to go through all and verify the connectedness.
            size_t connected_boxes = 0;
            for (size_t i = 0; i < nboxes; i++) {
                if (circuit[i] == from_circuit)
                    circuit[i] = to_circuit;
                if (circuit[i] == to_circuit)
                    ++connected_boxes;
            }
            if (connected_boxes == nboxes) {
                printf("%zd\n", c);
                double x1 = coords[conns[c].box1*3];
                double x2 = coords[conns[c].box2*3];
                printf("Solution: %.0f\n", x1*x2);
                break;
            }
        }
    }

    free(conns);
    return EXIT_SUCCESS;
}
