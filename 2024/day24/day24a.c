#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
Based on the input data, we may have up to 26*36^2 registers:
Each register starts with a letter and contains two more numbers or letters.
So we can store the values as byte array of size 26*36^2.
The register may take 3 values: -1 (unset), 0 and 1.
*/
#define REGISTER_CNT 33696
int8_t registers[REGISTER_CNT];

static inline
void init_registers() {
    memset(registers, -1, REGISTER_CNT);
}

static inline
size_t register_addr(char *id) {
    // Convert the register id to address.
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
void set_register(char *id, int8_t value) {
    size_t addr = register_addr(id);
    registers[addr] = value;
}

static inline
int8_t get_register(char *id) {
    size_t addr = register_addr(id);
    return registers[addr];
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
    gates[addr].in1 = register_addr(in1);
    gates[addr].in2 = register_addr(in2);
    gates[addr].out = register_addr(out);
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
        int8_t in1 = registers[g.in1];
        int8_t in2 = registers[g.in2];
        if (in1 < 0 || in2 < 0) {
            all_ready = false;
            continue;
        }
        switch (g.op) {
            case AND:
                registers[g.out] = in1 & in2;
                break;
            case OR:
                registers[g.out] = in1 | in2;
                break;
            case XOR:
                registers[g.out] = in1 ^ in2;
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
        size_t rval = get_register(id);
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
        set_register(id, value[0] - '0');
    }

    // The previous fscanf read the first register value
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

    init_registers();
    parse_input(f);
    fclose(f);

    while (!cycle_gates()) {}
    size_t result = get_result();
    printf("Solution: %zd\n", result);

    return EXIT_SUCCESS;
}
