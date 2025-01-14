#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINES 500
#define MAX_LINE_LENGTH 500


typedef struct {
    int x;
    int y;
} Direction;

Direction Directions[] = {
    { 0, 1 },
    { 1, 0 },
    { 0, -1 },
    { -1, 0 },
    { -1, -1 },
    { 1, 1 },
    { -1, 1 },
    { 1, -1 }
};

typedef struct {
    int numbers[100];
    int count;
} AdjacentNumbers;

// Checks if a cell at (i, j) has a non-numeric and non-period character in any adjacent cell
int findAdjacentSymbol(char **lines, int numLines, int i, int j) {
    for (int k = 0; k < 8; k++) {
        int y = i + Directions[k].y;
        int x = j + Directions[k].x;

        // Boundary check
        if (x < 0 || x >= strlen(lines[i]) || y < 0 || y >= numLines) {
            continue;
        }

        // Check for a non-period and non-digit character
        if (lines[y][x] != '.' && !isdigit(lines[y][x])) {
            return 1;
        }
    }

    return 0;  // No adjacent symbols found
}

// Extracts the full number starting at position (i, j) by combining digits to the left and right
int getFullNumber(char **lines, int i, int j) {
    char currentNumber[MAX_LINE_LENGTH] = "";
    int index = 0;

    // Append current digit
    currentNumber[index++] = lines[i][j];

    // Check digits to the right
    for (int k = j + 1; k < strlen(lines[i]); k++) {
        if (!isdigit(lines[i][k])) {
            break;
        }
        currentNumber[index++] = lines[i][k];
    }

    // Check digits to the left
    for (int k = j - 1; k >= 0; k--) {
        if (!isdigit(lines[i][k])) {
            break;
        }

        // Shift existing digits right to append to beginning of a digit
        memmove(&currentNumber[1], &currentNumber[0], index);
        currentNumber[0] = lines[i][k];
        index++;
    }

    currentNumber[index] = '\0';  // Null-terminate the string
    return atoi(currentNumber);  // Convert to integer
}

// Finds all full numbers in cells adjacent to (i, j), returns count in adjacentCount
AdjacentNumbers findAdjacentNumbers(char **lines, int numLines, int i, int j) {
    AdjacentNumbers result;
    result.count = 0;

    for (int k = 0; k < 8; k++) {
        int x = i + Directions[k].y;
        int y = j + Directions[k].x;

        if (x < 0 || x >= numLines || y < 0 || y >= strlen(lines[i])) {
            continue;
        }

        if (isdigit(lines[x][y])) {
            result.numbers[result.count++] = getFullNumber(lines, x, y);
        }
    }

    return result;
}

// Removes duplicate numbers from the array
void removeDuplicates(int *numbers, int *size) {
    for (int i = 0; i < *size; i++) {
        for (int j = i + 1; j < *size; j++) {
            if (numbers[i] == numbers[j]) {
                // Shift elements to remove the duplicate
                for (int k = j; k < *size - 1; k++) {
                    numbers[k] = numbers[k + 1];
                }
                (*size)--;  // Decrease size
                j--;        // Adjust index
            }
        }
    }
}

void partTwo(char **lines, int numLines) {
    int solution = 0;

    // find asterisks
    for (int i = 0; i < numLines; i++) {
        for (int j = 0; j < strlen(lines[i]); j++) {
            if (lines[i][j] != '*') {
                continue;
            }

            // Find all adjacent numbers
            AdjacentNumbers adjacent = findAdjacentNumbers(lines, numLines, i, j);
            removeDuplicates(adjacent.numbers, &adjacent.count);

            // If exactly two unique numbers are found, add their product to the solution
            if (adjacent.count == 2) {
                solution += adjacent.numbers[0] * adjacent.numbers[1];
            }
        }
    }

    printf("%d\n", solution);  // Output the result
}

int main(int argc, char* argv[]) {
    FILE *file = fopen(argv[1], "r");

    if (!file) {
        printf("Unable to open file\n");
        return 1;
    }

    char *lines[MAX_LINES];
    int numLines = 0;

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, file)) {
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            break;
        }

        // Remove trailing newline and store the line
        buffer[strcspn(buffer, "\n")] = '\0';
        lines[numLines] = strdup(buffer);
        numLines++;
    }

    fclose(file);

    partTwo(lines, numLines);

    return 0;
}
