#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define WIDTH 500
#define HEIGHT 500

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

int main(int argc, char * argv[]) {
    // define grid input
    char input[HEIGHT][WIDTH];
    int width = 0, height = 0;

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }

    while (fgets(input[height], sizeof(input[0]), file)) {
        height++;
        width = strlen(input[height - 1]);
    }

    fclose(file);

    int grid[WIDTH][HEIGHT];

    // Initialize grid by flipping rows and columns
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            grid[x][y] = input[x][y];
        }
    }

    int sum = 0;
    int num = 0;
    int symbol = 0;

    // Process the grid
    for (int x = 0; x < height; x++) {
        // Reset the number checker for each row
        for (int y = 0; y < width; y++) {
            char character = grid[x][y];

            // Check if we are reading a number
            if (isdigit(character)) {
                int value = character - '0';  // Convert char to int

                // Construct the current number (handling multi-digit numbers)
                num = num*10 + value;

                // Check the neighboring cells
                for (int d = 0; d < 8; d++) {
                    int nx = x + Directions[d].x;
                    int ny = y + Directions[d].y;

                    // make sure it's in grid bounds
                    if (nx < 0, nx >= height, ny < 0 || ny >= width) {
                        continue;
                    }

                    char nc = grid[nx][ny];

                    // if not digit and not "." set flag true
                    if (!isdigit(nc) && nc != '.') {
                        symbol = 1;
                    }
                }
            } else {
                // End the current number if it was valid
                if (num != 0 && symbol) {
                    // printf("%d\n", num);
                    sum += num;
                }
                num = 0;
                symbol = 0;
            }
        }

        // check for the current number at the end of the row
        if (num != 0 && symbol) {
            // printf("%d\n", num);
            sum += num;
        }
        num = 0;
        symbol = 0;
    }
    printf("Total sum: %d\n", sum);
}