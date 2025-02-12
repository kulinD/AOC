#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// dest, source, range
typedef struct MapLineNode {
    long long dest;           // dest base value
    long long source;         // lower bound
    long long range;          // range
    struct MapLineNode *next; // pointer to the next map line in the group
} MapLineNode;

// Node to represent a group of map lines
typedef struct MapGroupNode {
    MapLineNode *mapLinesHead; // head of the linked list
    struct MapGroupNode *next; // pointer to the next group
} MapGroupNode;

// create group node
MapGroupNode* createMapGroupNode(void) {
    MapGroupNode *node = malloc(sizeof(MapGroupNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    node->mapLinesHead = NULL;
    node->next = NULL;
    return node;
}

// append group node to linked list
void appendMapGroup(MapGroupNode **head, MapGroupNode *group) {
    if (!*head) {
        *head = group;
    } else {
        MapGroupNode *current = *head;
        while (current->next)
            current = current->next;
        current->next = group;
    }
}

// append MapLineNode to group


int main(void) {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        fprintf(stderr, "Unable to open input.txt\n");
        return 1;
    }
    
    char buffer[1024];
    size_t len;

    // parse seeds and put into array
    if (fgets(buffer, sizeof(buffer), f) == NULL) {
        fprintf(stderr, "Error reading file\n");
        fclose(f);
        return 1;
    }
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
    
    // delim
    const char *seedsPrefix = "seeds: ";
    char *seedStr = buffer;
    char *prefixPos = strstr(buffer, seedsPrefix);
    if (prefixPos != NULL) {
        seedStr = prefixPos + strlen(seedsPrefix);
    }
    
    // read lines and build groups
    MapGroupNode *groupsHead = NULL;
    MapGroupNode *currentGroup = NULL;
    
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
        if (buffer[0] == '\0')
            continue;

        if (!isdigit((unsigned char)buffer[0])) {
            // create a new group and append group node to group
            currentGroup = createMapGroupNode();
            appendMapGroup(&groupsHead, currentGroup);
        } else {
            // parse line into dest, source, range nums
            char *lineToken = strtok(buffer, " ");
            if (lineToken == NULL) continue;
            long long source = strtoll(lineToken, NULL, 10);
            
            lineToken = strtok(NULL, " ");
            if (lineToken == NULL) continue;
            long long dest = strtoll(lineToken, NULL, 10);
            
            lineToken = strtok(NULL, " ");
            if (lineToken == NULL) continue;
            long long range = strtoll(lineToken, NULL, 10);

            if (currentGroup == NULL){
                currentGroup = createMapGroupNode();
                appendMapGroup(&groupsHead, currentGroup);
            }
            // append MapLine to group
        }
    }
    fclose(f);

    // processing each seed through each group
    // seed = dest + (seed - source) ???
    

    return 0;
}
