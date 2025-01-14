#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// function to remove leading substring from a string up to a delimiter
static void trimToDelimiter(char *line, const char *delim)
{
    char *pos = strstr(line, delim);
    if (pos)
    {
        // Move pos to the end of the delimiter
        pos += strlen(delim);
        // Shift everything over in 'line' so that we start at correct 'pos'
        memmove(line, pos, strlen(pos) + 1);
    }
}

// function to replace double space occurences with 0
static void replaceDoubleSpaceWithZero(char *line)
{
    int length = (int)strlen(line);
    for (int i = 0; i < length - 1; i++)
    {
        if (line[i] == ' ' && line[i + 1] == ' ')
        {
            line[i + 1] = '0';  // Replace second space with 0
        }
    }
}

/* Splits a string on spaces into an array of strings (tokens).
   Returns the number of tokens found, and `*outArray` will point to
   a dynamically allocated array of token C-strings.
   Good to free both the token and *outArray.
*/
static int splitBySpace(const char *input, char ***outArray)
{
    // Copy input to a temp buffer because strtok will modify it
    char *buffer = (char *)malloc(strlen(input) + 1);
    strcpy(buffer, input);

    // First pass to count number of space-delimited tokens 
    int count = 0;
    char *token = strtok(buffer, " ");
    while (token)
    {
        count++;
        token = strtok(NULL, " ");
    }

    // Allocate array of char* to hold the tokens
    *outArray = (char **)malloc(count * sizeof(char *));
    if (*outArray == NULL)
    {
        free(buffer);
        return 0;
    }

    // Second pass: to actually store the tokens
    strcpy(buffer, input);
    int idx = 0;
    token = strtok(buffer, " ");
    while (token)
    {
        // Allocate space for current token and copy it
        (*outArray)[idx] = (char *)malloc(strlen(token) + 1);
        strcpy((*outArray)[idx], token);
        idx++;
        token = strtok(NULL, " ");
    }

    free(buffer);
    return count;
}

int partOne(char **lines, int numLines)
{
    int solution = 0;

    // Delimiters
    const char *startDelimiter  = ": ";
    const char *cardDelimiter   = " | ";
    const char *numbersDelimiter= " ";

    for (int i = 0; i < numLines; i++)
    {
        // Make a temp copy of the line to manipulate
        char *tempLine = (char *)malloc(strlen(lines[i]) + 1);
        strcpy(tempLine, lines[i]);

        // Replace "  " with " 0"
        replaceDoubleSpaceWithZero(tempLine);

        // Remove everything up to and ": "
        trimToDelimiter(tempLine, startDelimiter);

        /* Extract winningNumbers = substring before " | "
           and cardNumbers = substring after " | " */
        char *cardPos = strstr(tempLine, cardDelimiter);
        if (!cardPos)
        {
            // Skip if no delim
            free(tempLine);
            continue;
        }

        // Split into two pieces
        int delimIndex = (int)(cardPos - tempLine);

        // winningNumbers = tempLine[0..delimIndex-1], terminate null
        char *winningNumbers = (char *)malloc(delimIndex + 1);
        strncpy(winningNumbers, tempLine, delimIndex);
        winningNumbers[delimIndex] = '\0';

        // cardNumbers after the " | "
        cardPos += strlen(cardDelimiter);
        char *cardNumbers = (char *)malloc(strlen(cardPos) + 1);
        strcpy(cardNumbers, cardPos);

        // Split winningNumbers by space
        char **winningArr = NULL;
        int winningCount = splitBySpace(winningNumbers, &winningArr);

        // Split cardNumbers by space */
        char **cardArr = NULL;
        int cardCount = splitBySpace(cardNumbers, &cardArr);

        // Count matches
        int matches = 0;
        for (int j = 0; j < winningCount; j++)
        {
            for (int k = 0; k < cardCount; k++)
            {
                if (strcmp(winningArr[j], cardArr[k]) == 0)
                {
                    matches++;
                }
            }
        }

        // Calculate solution
        if (matches >= 1)
        {
            solution += (int)pow(2.0, (double)(matches - 1));
        }
        free(winningArr);
        free(cardArr);
        free(winningNumbers);
        free(cardNumbers);
        free(tempLine);
    }
    return solution;
}

int partTwo(char **lines, int numLines)
{
    int solution = 0;

    // Delimiters
    const char *startDelimiter  = ": ";
    const char *cardDelimiter   = " | ";
    const char *numbersDelimiter= " ";

    int *cards = (int *)malloc(numLines * sizeof(int));
    for (int i = 0; i < numLines; i++)
    {
        cards[i] = 1;
    }

    for (int i = 0; i < numLines; i++)
    {
        // Make a temp copy of the line to manipulate
        char *tempLine = (char *)malloc(strlen(lines[i]) + 1);
        strcpy(tempLine, lines[i]);

        // Replace "  " with " 0"
        replaceDoubleSpaceWithZero(tempLine);

        // Remove everything up to and ": "
        trimToDelimiter(tempLine, startDelimiter);

        /* Extract winningNumbers = substring before " | "
           and cardNumbers = substring after " | " */
        char *cardPos = strstr(tempLine, cardDelimiter);
        if (!cardPos)
        {
            // Skip if no delim
            free(tempLine);
            continue;
        }

        // Split into two pieces
        int delimIndex = (int)(cardPos - tempLine);

        // winningNumbers = tempLine[0..delimIndex-1], terminate null
        char *winningNumbers = (char *)malloc(delimIndex + 1);
        strncpy(winningNumbers, tempLine, delimIndex);
        winningNumbers[delimIndex] = '\0';

        // cardNumbers after the " | "
        cardPos += strlen(cardDelimiter);
        char *cardNumbers = (char *)malloc(strlen(cardPos) + 1);
        strcpy(cardNumbers, cardPos);

        // Split winningNumbers by space
        char **winningArr = NULL;
        int winningCount = splitBySpace(winningNumbers, &winningArr);

        // Split cardNumbers by space */
        char **cardArr = NULL;
        int cardCount = splitBySpace(cardNumbers, &cardArr);

        // count matches
        int matches = 0;
        for (int j = 0; j < winningCount; j++)
        {
            for (int k = 0; k < cardCount; k++)
            {
                if (strcmp(winningArr[j], cardArr[k]) == 0)
                {
                    matches++;
                }
            }
        }

        // for each match update card count
        for (int j = 0; j < matches; j++)
        {
            if (i + j + 1 < numLines)
            {
                cards[i + j + 1] += cards[i];
            }
        }
        free(winningArr);
        free(cardArr);
        free(winningNumbers);
        free(cardNumbers);
        free(tempLine);
    }

    // calculate solution
    for (int i = 0; i < numLines; i++)
    {
        solution += cards[i];
    }

    free(cards);
    return solution;
}

int main(int argc, char* argv[])
{
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Unable to open file\n");
        return 1;
    }

    // Dynamically read lines from the file
    char buffer[1024];
    char *lines[1000];
    int numLines = 0;

    while (fgets(buffer, sizeof(buffer), file))
    {
        // remove trailing newlines
        char *newlinePos = strchr(buffer, '\n');
        if (newlinePos)
            *newlinePos = '\0';

        // break if empty line
        if (buffer[0] == '\0')
            break;

        // allocate space to store current line in lines array
        lines[numLines] = (char *)malloc(strlen(buffer) + 1);
        strcpy(lines[numLines], buffer);

        numLines++;
    }
    fclose(file);

    int solution1 = partOne(lines, numLines);
    printf("%d\n", solution1);

    int solution2 = partTwo(lines, numLines);
    printf("%d\n", solution2);

    for(int i = 0; i < numLines; i++)
    {
        free(lines[i]);
    }

    return 0;
}
