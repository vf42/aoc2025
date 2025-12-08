#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define S 1000
#define CONNECT 1000

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

typedef struct Connection {
    float dist;
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

int comp_circuit(const void *a, const void *b) {
    size_t *ca = (size_t*)a;
    size_t *cb = (size_t*)b;
    if (*ca < *cb) return -1;
    if (*ca > *cb) return 1;
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
    float coords[S*3];
    while (fscanf(f, "%f,%f,%f\n",
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
                powf(coords[i*3] - coords[j*3], 2.0) +
                powf(coords[i*3+1] - coords[j*3+1], 2.0) +
                powf(coords[i*3+2] - coords[j*3+2], 2.0);
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

    // Make[S*S] connections.
    for (size_t c = 0; c < CONNECT; c++) {
        if (circuit[conns[c].box1] != circuit[conns[c].box2]) {
            size_t to_circuit = MIN(
                circuit[conns[c].box1], circuit[conns[c].box2]);
            size_t from_circuit = MAX(
                circuit[conns[c].box1], circuit[conns[c].box2]);
            // We don't know how many boxes are int from_circuit.
            for (size_t i = 0; i < nboxes; i++) {
                if (circuit[i] == from_circuit)
                    circuit[i] = to_circuit;
            }
        }
    }

    // Count the circuits.
    qsort(circuit, nboxes, sizeof(size_t), &comp_circuit);
    size_t csizes[nboxes];
    size_t ncircuits = 1;
    size_t s = 1;
    for (size_t i = 1; i < nboxes; i++) {
        if (circuit[i] != circuit[i-1]) {
            csizes[ncircuits-1] = s;
            s = 1;
            ++ncircuits;
        } else {
            ++s;
        }
    }
    csizes[ncircuits-1] = s;

    // Get the product of 3 largest.
    qsort(csizes, ncircuits, sizeof(size_t), &comp_circuit);
    size_t product = 1;
    for (size_t i = ncircuits - 1; i >= ncircuits - 3; i--) {
        product *= csizes[i];
    }

    printf("Solution: %zd\n", product);
    free(conns);
    return EXIT_SUCCESS;
}
