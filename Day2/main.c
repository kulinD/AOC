#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store the number of cubes for each color
typedef struct {
    int red;
    int green;
    int blue;
} CubeCount;

// Function to parse a line and get cube count
void parseCubes(char* line, CubeCount* cubes) {
    char* token = strtok(line, ",");
    while (token != NULL) {
        int count;
        char color[15];

        // Extract number and color
        sscanf(token, "%d %s", &count, color);

        // Update color count
        if (strcmp(color, "red") == 0) {
            cubes->red = cubes->red > count ? cubes->red : count;
        } else if (strcmp(color, "green") == 0) {
            cubes->green = cubes->green > count ? cubes->green : count;
        } else if (strcmp(color, "blue") == 0) {
            cubes->blue = cubes->blue > count ? cubes->blue : count;
        }

        // Move to the next token
        token = strtok(NULL, ",");
    }
}

// Function to calculate the minimum required cubes for each color
void calculateMinimumCubes(CubeCount* totalCubes, CubeCount* cubes) {
    // Update the minimum cubes
    totalCubes->red = totalCubes->red > cubes->red ? totalCubes->red : cubes->red;
    totalCubes->green = totalCubes->green > cubes->green ? totalCubes->green : cubes->green;
    totalCubes->blue = totalCubes->blue > cubes->blue ? totalCubes->blue : cubes->blue;
}

// calculate the power of the minimum set
int calculatePower(CubeCount* cubes) {
    return cubes->red * cubes->green * cubes->blue;
}

int main() {
    char text[1024];
    int totalPower = 0;

    FILE* file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    // Read each line from the file
    while (fgets(text, sizeof(text), file)) {
        // Get game id
        int gameId;
        if (sscanf(text, "Game %d:", &gameId) == 0) {
            continue;
        }

        CubeCount totalCubes = {0, 0, 0}; // Initialize total cube count

        // continue after game id
        char* token = strchr(text, ':') + 1;

        // Process each subset of games separated by ';'
        while (token != NULL) {
            char* nextToken = strchr(token, ';');
            if (nextToken != NULL) {
                *nextToken = '\0';
                nextToken++;
            }

            CubeCount cubes = {0, 0, 0}; // Initialize cubes count for this set
            parseCubes(token, &cubes);

            // Update the total cubes for the minimum set
            calculateMinimumCubes(&totalCubes, &cubes);

            token = nextToken; // Move to the next set of cubes
        }

        // Calculate the power of min cubes
        int power = calculatePower(&totalCubes);
        totalPower += power;
    }

    printf("Total Power of Minimum Sets: %d\n", totalPower);

    fclose(file);

    return 0;
}
