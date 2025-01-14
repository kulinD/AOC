#include <stdio.h>

int main() {

    char text[256];
    int sum = 0;

    // open and read file
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("No file to open.\n");
        return 1;
    }

    // read each line
    while (fgets(text, sizeof(text), file)) {
        int firstNum = -1;
        int lastNum = -1;

        for (int i = 0; text[i] != '\0' && text[i] != '\n'; i++) {
            if (text[i] >= '0' && text[i] <= '9') {
                if (firstNum == -1) {
                    firstNum = text[i] - '0'; // first num; subtract 0 to turn to int
                }
                lastNum = text[i] - '0'; // last num
            }
        }

        // combines first num and last num
        if (firstNum != -1 && lastNum != -1) {
            int calValue = firstNum * 10 + lastNum;
            printf("combined number: %d\n", calValue);
            sum += calValue;
        }
    }

    fclose(file);
    printf("Sum of Calibration Values: %d\n", sum);
    return 0;
}
