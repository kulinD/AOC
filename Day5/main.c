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

// create MapLineNode with a given dest, source, range
MapLineNode* createMapLineNode(long long dest, long long source, long long range) {
    MapLineNode *node = malloc(sizeof(MapLineNode));
    if (!node) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    (*node).dest = dest;
    node->source = source;
    node->range = range;
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
void appendMapLine(MapGroupNode *group, MapLineNode *line) {
    if (!group->mapLinesHead) {
        group->mapLinesHead = line;
    } else {
        MapLineNode *current = group->mapLinesHead;
        while (current->next)
            current = current->next;
        current->next = line;
    }
}


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
    
    // parse seeds into an array
    long long *seeds = NULL;
    size_t seedsCount = 0, seedsCapacity = 0;
    char *token = strtok(seedStr, " ");
    while (token != NULL) {
        long long value = strtoll(token, NULL, 10);
        if (seedsCount == seedsCapacity) {
            seedsCapacity = (seedsCapacity == 0) ? 4 : seedsCapacity * 2;
            seeds = realloc(seeds, seedsCapacity * sizeof(long long));
            if (!seeds) {
                fprintf(stderr, "Memory allocation error for seeds\n");
                exit(1);
            }
        }
        seeds[seedsCount++] = value;
        token = strtok(NULL, " ");
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
            long long dest = strtoll(lineToken, NULL, 10);
            
            lineToken = strtok(NULL, " ");
            if (lineToken == NULL) continue;
            long long source = strtoll(lineToken, NULL, 10);
            
            lineToken = strtok(NULL, " ");
            if (lineToken == NULL) continue;
            long long range = strtoll(lineToken, NULL, 10);

            MapLineNode *lineNode = createMapLineNode(dest, source, range);

            if (currentGroup == NULL){
                currentGroup = createMapGroupNode();
                appendMapGroup(&groupsHead, currentGroup);
            }
            // append MapLine to group
            appendMapLine(currentGroup, lineNode);
        }
    }
    fclose(f);

    // processing each seed through each group
    // seed >= source && seed < source + range
    // seed = dest + (seed - source)
    long long solution = 1000000000000000LL;
    for (size_t i = 0; i < seedsCount; i++){
        long long seed = seeds[i];

        for (MapGroupNode *group = groupsHead; group != NULL; group = group -> next){
            MapLineNode *num = NULL;

            for (MapLineNode *line = group->mapLinesHead; line != NULL; line = line-> next){
                if (seed >= line->source && seed < line->source + line->range){
                    num = line;
                }
            }
            if (num != NULL){
                seed = num->dest + (seed - num->source);
            }
        }
        if (seed < solution){
            solution = seed;
        }
    }

    printf("Solution: %lld\n", solution);
    free(seeds);

    return 0;
}
