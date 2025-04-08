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

// Convert a string ("AAA", "BBB", ... "ZZZ") to a number index (0 for "AAA")
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

// Read instructions and build the Node tree
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

#define START "AAA"
// Execute navigation instructions and return the number of steps
uint32_t execute_instructions(const char* instructions, const Node_t* arr) {
    uint32_t numInstr = strlen(instructions);
    uint32_t index = str_val(START);
    uint32_t steps = 0;

    while (index >= 0 && index < MAX_NODES) {
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

int main(uint32_t argc, char ** argv) {
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
        printf("bad read!");
        free(arr);
        fclose(file);
        return 1;
    }

    uint32_t result = execute_instructions(instructions, arr);
    printf("%d\n", result);

    free(arr);
    fclose(file);
    return 0;
}