#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NODES  17575  // 26^3 - 1
#define MAX_STR    10
#define MAX_INSTR  512

typedef struct Node {
    uint32_t left;
    uint32_t right;
} Node_t;

// Convert a string ("AAA", "BBB", ... "ZZZ") to a base-26 index (0 for "AAA")
uint32_t str_val(const char* str) {
    uint32_t result = 0;

    while (*str) {
        if (isupper((unsigned char)*str)) {
            int letterValue = *str - 'A';
            result = result * 26 + letterValue;
        }
        str++;
    }

    return result;
}

void str_from_val(uint32_t val, char* str) {
    str[3] = '\0';
    str[2] = 'A' + val % 26;
    val /= 26;
    str[1] = 'A' + val % 26;
    val /= 26;
    str[0] = 'A' + val % 26;
}

// Read instructions and build the Node_t tree
uint32_t read_input(FILE* file, char* instructions, Node_t* arr) {
    if (fscanf(file, "%s", instructions) != 1) {
        printf("Failed to read instructions\n");
        return 0;
    }

    char name[MAX_STR], left[MAX_STR], right[MAX_STR];
    while (fscanf(file, "%s = %s %s", name, left, right) == 3) {
        uint32_t idx = str_val(name);
        arr[idx].left = str_val(left);
        arr[idx].right = str_val(right);
    }

    return 1;
}

int ends_in_Z(uint32_t index) {
    char str[4];
    str_from_val(index, str);
    return str[2] == 'Z';
}

int ends_in_A(uint32_t index) {
    char str[4];
    str_from_val(index, str);
    return str[2] == 'A';
}

uint64_t gcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

uint64_t lcm(uint64_t a, uint64_t b) {
    return (a / gcd(a, b)) * b;
}

#define START "AAA"
// Execute navigation instructions and return the number of steps
uint32_t execute_instructions(const char* instructions, const Node_t* arr) {
    uint32_t numInstr = strlen(instructions);
    uint32_t index = str_val(START);
    uint32_t steps = 0;

    while (index < MAX_NODES) {
        char instr = instructions[steps % numInstr];
        if (instr == 'R') {
            index = arr[index].right;
        } else {
            index = arr[index].left;
        }
        steps++;
    }

    return steps;
}

uint64_t steps_to_Z(uint32_t start, const char* instructions, const Node_t* arr) {
    uint32_t numInstr = strlen(instructions);
    uint32_t index = start;
    uint64_t steps = 0;

    while (!ends_in_Z(index)) {
        char instr = instructions[steps % numInstr];
        index = (instr == 'R') ? arr[index].right : arr[index].left;
        steps++;
    }

    return steps;
}

uint32_t main(uint32_t argc, char ** argv) {
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Could not open input file.");
        return 1;
    }
   
    Node_t* arr = calloc(MAX_NODES, sizeof(*arr));
    if (!arr) {
        printf("calloc error!");
        fclose(file);
        return 1;
    }

    char instructions[MAX_INSTR];
    if (!read_input(file, instructions, arr)) {
        free(arr);
        fclose(file);
        return 1;
    }

    // part 1
    uint32_t part1_result = execute_instructions(instructions, arr);
    printf("Part 1: %u\n", part1_result);

    // part 2
    uint64_t part2_result = 1;
    for (uint32_t i = 0; i < MAX_NODES; i++) {
        if (arr[i].left == 0 && arr[i].right == 0) continue;
        if (ends_in_A(i)) {
            uint64_t steps = steps_to_Z(i, instructions, arr);
            part2_result = lcm(part2_result, steps);
        }
    }
    printf("Part 2: %llu\n", part2_result);

    free(arr);
    fclose(file);
    return 0;
}