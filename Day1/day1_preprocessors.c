#include <stdio.h>
#include <string.h>
#include <ctype.h>

int calculate_sum(const char *filename) {
    char text[256];
    int sum = 0;

    // Open and read the file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No file to open: %s\n", filename);
        return -1;  // Return -1 to indicate an error
    }

    char *words[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    
    // Read each line from the file
    while (fgets(text, sizeof(text), file)) {
        int firstNum = -1;
        int lastNum = -1;

        // Process the input line
        for (int i = 0; text[i] != '\0' && text[i] != '\n'; i++) {
            // Check for actual numbers
            if (isdigit(text[i])) {
                if (firstNum == -1) {
                    firstNum = text[i] - '0'; // Set firstNum to the first digit found
                }
                lastNum = text[i] - '0'; // Update lastNum to the last digit found
            }
            // Check for word numbers
            else {
                for (int j = 0; j < 10; j++) {
                    if (strncmp(&text[i], words[j], strlen(words[j])) == 0) {
                        if (firstNum == -1) {
                            firstNum = j;
                        }
                        lastNum = j; 
                        break; 
                    }
                }
            }
        }

        // Combine firstNum and lastNum
        if (firstNum != -1 && lastNum != -1) {
            int calValue = firstNum * 10 + lastNum;
            printf("combined number: %d\n", calValue);
            sum += calValue;
        }
    }

    fclose(file);
    return sum; 
}

int main() {
    char filename[256];

    printf("Enter a file name: ");
    scanf("%s", filename);

    int sum = calculate_sum(filename);

    if (sum >= 0) {
        printf("Sum of Calibration Values: %d\n", sum);
    } else {
        printf("Invalid file name or could not read the file.\n");
    }

    return 0;
}
