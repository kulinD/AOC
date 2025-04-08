#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_LEVELS 20
#define MAX_NUMBERS 128
#define MAX_LINE_LENGTH 512

typedef struct {
    int64_t data[MAX_LEVELS][MAX_NUMBERS];
    int count; // Number of numbers in the original sequence
    int depth; // Number of levels in the triangle
} DifferenceTriangle_t;

// Create a base level of the triangle from each line in file
void parse_numbers(char* line, DifferenceTriangle_t* triangle) {
    triangle->count = 0;
    char* pos = line;
    char* end;

    while (true) {
        int64_t num = strtoll(pos, &end, 10);
        if (end == pos) {
            break;
        }
        else {
            triangle->data[0][triangle->count++] = num;
            pos = end;
        }
    }
}

// Build the difference triangle from the base level
void build_difference_triangle(DifferenceTriangle_t* triangle) {
    for (int level = 1; level < MAX_LEVELS; ++level) {
        bool all_zero = true;
        for (int i = 1; i < triangle->count - level + 1; ++i) {
            triangle->data[level][i - 1] =
                triangle->data[level - 1][i] - triangle->data[level - 1][i - 1];
            if (triangle->data[level][i - 1] != 0) {
                all_zero = false;
            }
        }
        if (all_zero) {
            triangle->depth = level;
            return;
        }
    }
    triangle->depth = MAX_LEVELS;
}

// End
int64_t extrapolate_end(DifferenceTriangle_t* triangle) {
    int64_t value = 0;
    for (int level = triangle->depth - 1; level >= 0; --level) {
        value += triangle->data[level][triangle->count - level - 1];
    }
    return value;
}

// Front
int64_t extrapolate_front(DifferenceTriangle_t* triangle) {
    int64_t value = 0;
    for (int level = triangle->depth - 1; level >= 0; --level) {
        value = triangle->data[level][0] - value;
    }
    return value;
}

bool process_file(char* filename, int64_t* part1_sum, int64_t* part2_sum) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open input file!\n");
        return false;
    }

    char line[MAX_LINE_LENGTH];
    *part1_sum = 0;
    *part2_sum = 0;

    while (fgets(line, sizeof(line), file)) {
        DifferenceTriangle_t triangle = {0};
        parse_numbers(line, &triangle);
        if (triangle.count == 0) continue;

        build_difference_triangle(&triangle);
        *part1_sum += extrapolate_end(&triangle);
        *part2_sum += extrapolate_front(&triangle);
    }

    fclose(file);
    return true;
}

int main(int argc, char ** argv) {
    int64_t part1_sum = 0;
    int64_t part2_sum = 0;

    if(process_file(argv[1], &part1_sum, &part2_sum)) {
        printf("Part1: %d\n", part1_sum);
        printf("Part2: %d\n", part2_sum);
    }
    else {
        return 1;
    }

    return 0;
}