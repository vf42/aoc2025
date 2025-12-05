#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
Based on the input data, we may have up to 26*36^2 wires:
Each wire starts with a letter and contains two more numbers or letters.
So we can store the values as byte array of size 26*36^2.
The wire may take 3 values: -1 (unset), 0 and 1.
*/
#define WIRE_CNT 33696
int8_t wires[WIRE_CNT];

static inline
void init_wires() {
    memset(wires, -1, WIRE_CNT);
}

static inline
size_t wire_addr(char *id) {
    // Convert the wire id to address.
    size_t addr = (id[0] - 'a') * 1296; // 1296=36^2
    if (id[1] >= 'a') {
        addr += (id[1] - 'a') * 36;   
    } else {
        addr += (id[1] - '0' + 26) * 36;
    }
    if (id[2] >= 'a') {
        addr += (id[2] - 'a');
    } else {
        addr += (id[2] - '0' + 26);
    }
    return addr;
}

static inline
void set_wire(char *id, int8_t value) {
    size_t addr = wire_addr(id);
    wires[addr] = value;
}

static inline
int8_t get_wire(char *id) {
    size_t addr = wire_addr(id);
    return wires[addr];
}

#define AND 1
#define OR 2
#define XOR 3

typedef struct {
    size_t in1;
    size_t in2;
    size_t out;
    uint8_t op;
    bool ready;
} Gate;

#define MAX_GATES 1000
Gate gates[MAX_GATES];
size_t gate_count;

static inline
size_t add_gate(char in1[4], char op[4], char in2[4], char out[4]) {
    size_t addr = gate_count++;
    gates[addr].in1 = wire_addr(in1);
    gates[addr].in2 = wire_addr(in2);
    gates[addr].out = wire_addr(out);
    switch(op[0]) {
        case 'A':
            gates[addr].op = AND;
            break;
        case 'O':
            gates[addr].op = OR;
            break;
        case 'X':
            gates[addr].op = XOR;
            break;
    }
    gates[addr].ready = false;
    return addr;
}

static inline
bool cycle_gates() {
    // Go through all gates and try to perform the required action.
    // Return true if all gates are ready.
    bool all_ready = true;
    for (size_t i = 0; i < gate_count; i++) {
        Gate g = gates[i];
        if (g.ready)
            continue;
        int8_t in1 = wires[g.in1];
        int8_t in2 = wires[g.in2];
        if (in1 < 0 || in2 < 0) {
            all_ready = false;
            continue;
        }
        switch (g.op) {
            case AND:
                wires[g.out] = in1 & in2;
                break;
            case OR:
                wires[g.out] = in1 | in2;
                break;
            case XOR:
                wires[g.out] = in1 ^ in2;
                break;
        }
        g.ready = true;
    }
    return all_ready;
}

static inline
size_t get_result() {
    size_t result = 0;
    char id[4];
    for (int i = 99; i >= 0; i--) {
        snprintf(id, sizeof(id), "z%02d", i);
        size_t rval = get_wire(id);
        if (rval != -1) {
            result = (result << 1) + rval;
        }
    }
    return result;
}

void parse_input(FILE* f) {
    // Read initial gate positions.
    char id[4], value[2];
    id[3] = 0;
    while (fscanf(f, "%3c: %1c\n", id, value) == 2) {
        // printf("%s %c\n", id, value[0]);
        set_wire(id, value[0] - '0');
    }

    // The previous fscanf read the first wire value
    // from the next block! Seek back to the start of the line.
    size_t read_result;
    do {
        fseek(f, -2, SEEK_CUR);
        read_result = fread(value, 1, 1, f);
    } while (read_result > 0 && value[0] != '\n');

    // Read gates.
    char in1[4], in2[4], op[4];
    in1[3] = in2[3] = op[3] = 0;
    while (fscanf(f, "%3c %[ANDXOR] %3c -> %3c\n", in1, op, in2, id) == 4) {
        // printf("%s %s %s %s\n", in1, op, in2, id);
        add_gate(in1, op, in2, id);
    }
}

/*
Heuristics:
1. Every gate z(N) must be reachable from x(N) and y(N) through 2 XOR gates.
2. Every gate z(N) must be reachable from x(N-1) and y(N-1)
    through AND -> OR -> XOR
*/

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

    init_wires();
    parse_input(f);
    fclose(f);

    while (!cycle_gates()) {}
    size_t result = get_result();
    printf("Solution: %zd\n", result);

    return EXIT_SUCCESS;
}
