#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    char id[4];
    uint8_t value;
} Register;

#define MAX_REGISTERS 1000
Register registers[MAX_REGISTERS];
size_t register_cnt = 0;

size_t new_register(char id[4], uint8_t value) {
    size_t addr = register_cnt++;
    strncpy(registers[addr].id, id, 4);
    registers[addr].value = value;
    return addr;
}

typedef struct {
    char in1[4];
    char in2[4];
    char op[4];
    char out[4];
    uint8_t ready;
} Gate;

#define MAX_GATES 1000
Gate gates[MAX_GATES];
size_t gate_count;

size_t new_gate(char in1[4], char op[4], char in2[4], char out[4]) {
    size_t addr = gate_count++;
    strncpy(gates[addr].in1, in1, 4);
    strncpy(gates[addr].in2, in2, 4);
    strncpy(gates[addr].op, op, 4);
    strncpy(gates[addr].out, out, 4);
    gates[addr].ready = 0;
    return addr;
}

void parse_input(FILE* f) {
    // Read initial gate positions.
    char id[4], value[2];
    id[3] = 0;
    while (fscanf(f, "%3c: %1c\n", id, value) == 2) {
        printf("%s %c\n", id, value[0]);
        new_register(id, value[0] - '0');
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
        printf("%s %s %s %s\n", in1, op, in2, id);
        new_gate(in1, op, in2, id);
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

    parse_input(f);

    fclose(f);
    // printf("Solution: %llu\n", total_max);
    return EXIT_SUCCESS;
}
