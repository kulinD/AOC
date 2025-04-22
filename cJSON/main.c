#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

// structure for controls
typedef struct {
    char name[50];
    char text[100];
    int min;
    int max;
    int size;
    double scale;
    char allowed[5][20];  // For font types like small, medium, large, large_digits
    char target[50]; // For r5g6b5 target
    char linked[50]; // For r5g6b5 linked
} Control_t;

// structure for submenus within each menu
typedef struct {
    char name[50];
    char perms[20];
} Submenu_t;

// structure for menus
typedef struct {
    char name[50];
    char text[100];
    int submenu_count;
    Submenu_t submenus[10]; // Assuming a max of 10 submenus per menu
} Menu_t;

// Function to read JSON file into a string
char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (!buffer) {
        printf("Memory allocation error");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

// Function to parse the controls section and populate the data
void parse_controls(cJSON *controls, Control_t *control_array, int *control_count) {
    cJSON *control = NULL;
    *control_count = 0;

    cJSON_ArrayForEach(control, controls) {
        cJSON *numeric = cJSON_GetObjectItem(control, "numeric");
        cJSON *boolean = cJSON_GetObjectItem(control, "boolean");
        cJSON *r5g6b5 = cJSON_GetObjectItem(control, "r5g6b5");
        cJSON *percent = cJSON_GetObjectItem(control, "percent");
        cJSON *font = cJSON_GetObjectItem(control, "font");
        cJSON *date = cJSON_GetObjectItem(control, "date");
        cJSON *time = cJSON_GetObjectItem(control, "time");
        cJSON *alert = cJSON_GetObjectItem(control, "alert");

        // Handle numeric type
        if (numeric) {
            strcpy(control_array[*control_count].name, cJSON_GetObjectItem(numeric, "name")->valuestring);
            strcpy(control_array[*control_count].text, cJSON_GetObjectItem(numeric, "text")->valuestring);
            control_array[*control_count].min = cJSON_GetObjectItem(numeric, "min") ? cJSON_GetObjectItem(numeric, "min")->valueint : 0;
            control_array[*control_count].max = cJSON_GetObjectItem(numeric, "max") ? cJSON_GetObjectItem(numeric, "max")->valueint : 0;
            control_array[*control_count].size = cJSON_GetObjectItem(numeric, "size") ? cJSON_GetObjectItem(numeric, "size")->valueint : 0;
            control_array[*control_count].scale = cJSON_GetObjectItem(numeric, "scale") ? cJSON_GetObjectItem(numeric, "scale")->valuedouble : 0.0;
            strcpy(control_array[*control_count].target, "");
            strcpy(control_array[*control_count].linked, "");
            for (int i = 0; i < 5; i++) {
                strcpy(control_array[*control_count].allowed[i], "");
            }
            (*control_count)++;
        }
        // Handle r5g6b5 type
        else if (r5g6b5) {
            strcpy(control_array[*control_count].name, cJSON_GetObjectItem(r5g6b5, "name")->valuestring);
            strcpy(control_array[*control_count].text, cJSON_GetObjectItem(r5g6b5, "text")->valuestring);
            strcpy(control_array[*control_count].target, cJSON_GetObjectItem(r5g6b5, "target")->valuestring);
            strcpy(control_array[*control_count].linked, cJSON_GetObjectItem(r5g6b5, "linked")->valuestring);
            for (int i = 0; i < 5; i++) {
                strcpy(control_array[*control_count].allowed[i], "");
            }
            (*control_count)++;
        }
        // Handle boolean type
        else if (boolean) {
            strcpy(control_array[*control_count].name, cJSON_GetObjectItem(boolean, "name")->valuestring);
            strcpy(control_array[*control_count].text, cJSON_GetObjectItem(boolean, "text")->valuestring);
            strcpy(control_array[*control_count].target, "");
            strcpy(control_array[*control_count].linked, "");
            control_array[*control_count].min = 0;
            control_array[*control_count].max = 0;
            control_array[*control_count].size = 0;
            control_array[*control_count].scale = 0;
            for (int i = 0; i < 5; i++) {
                strcpy(control_array[*control_count].allowed[i], "");
            }
            (*control_count)++;
        }
        // Handle percent type
        else if (percent) {
            strcpy(control_array[*control_count].name, cJSON_GetObjectItem(percent, "name")->valuestring);
            strcpy(control_array[*control_count].text, cJSON_GetObjectItem(percent, "text")->valuestring);
            strcpy(control_array[*control_count].target, "");
            strcpy(control_array[*control_count].linked, "");
            control_array[*control_count].min = 0;
            control_array[*control_count].max = 0;
            control_array[*control_count].size = 0;
            control_array[*control_count].scale = 0;
            for (int i = 0; i < 5; i++) {
                strcpy(control_array[*control_count].allowed[i], "");
            }
            (*control_count)++;
        }
        // Handle font type
        else if (font) {
            strcpy(control_array[*control_count].name, cJSON_GetObjectItem(font, "name")->valuestring);
            strcpy(control_array[*control_count].text, cJSON_GetObjectItem(font, "text")->valuestring);
            // for items in allowed field
            cJSON *allowed = cJSON_GetObjectItem(font, "allowed");
            int i = 0;
            cJSON *allowed_item = NULL;
            cJSON_ArrayForEach(allowed_item, allowed) {
                strncpy(control_array[*control_count].allowed[i], allowed_item->valuestring, 
                    sizeof(control_array[*control_count].allowed[i]) - 1);
                i++;
            }
            strcpy(control_array[*control_count].target, "");
            strcpy(control_array[*control_count].linked, "");
            control_array[*control_count].min = 0;
            control_array[*control_count].max = 0;
            control_array[*control_count].size = 0;
            control_array[*control_count].scale = 0;
            (*control_count)++;
        }
        // Handle date type
        else if (date) {
            strcpy(control_array[*control_count].name, cJSON_GetObjectItem(date, "name")->valuestring);
            strcpy(control_array[*control_count].text, cJSON_GetObjectItem(date, "text")->valuestring);
            strcpy(control_array[*control_count].target, "");
            strcpy(control_array[*control_count].linked, "");
            control_array[*control_count].min = 0;
            control_array[*control_count].max = 0;
            control_array[*control_count].size = 0;
            control_array[*control_count].scale = 0;
            for (int i = 0; i < 5; i++) {
                strcpy(control_array[*control_count].allowed[i], "");
            }
            (*control_count)++;
        }
        // Handle time type
        else if (time) {
            strcpy(control_array[*control_count].name, cJSON_GetObjectItem(time, "name")->valuestring);
            strcpy(control_array[*control_count].text, cJSON_GetObjectItem(time, "text")->valuestring);
            strcpy(control_array[*control_count].target, "");
            strcpy(control_array[*control_count].linked, "");
            control_array[*control_count].min = 0;
            control_array[*control_count].max = 0;
            control_array[*control_count].size = 0;
            control_array[*control_count].scale = 0;
            for (int i = 0; i < 5; i++) {
                strcpy(control_array[*control_count].allowed[i], "");
            }
            (*control_count)++;
        }
        // Handle alert type
        else if (alert) {
            strcpy(control_array[*control_count].name, cJSON_GetObjectItem(alert, "name")->valuestring);
            strcpy(control_array[*control_count].text, cJSON_GetObjectItem(alert, "text")->valuestring);
            strcpy(control_array[*control_count].target, "");
            strcpy(control_array[*control_count].linked, "");
            control_array[*control_count].min = 0;
            control_array[*control_count].max = 0;
            control_array[*control_count].size = 0;
            control_array[*control_count].scale = 0;
            for (int i = 0; i < 5; i++) {
                strcpy(control_array[*control_count].allowed[i], "");
            }
            (*control_count)++;
        }
    }
}

// Function to parse json file and populate the menus and submenus
void parse_menus(cJSON *menus, Menu_t *menu_array, int *menu_count) {
    cJSON *menu = NULL;
    *menu_count = 0;

    cJSON_ArrayForEach(menu, menus) {
        strcpy(menu_array[*menu_count].name, cJSON_GetObjectItem(menu, "name")->valuestring);
        strcpy(menu_array[*menu_count].text, cJSON_GetObjectItem(menu, "text")->valuestring);

        cJSON *submenus = cJSON_GetObjectItem(menu, "submenus");
        cJSON *submenu = NULL;
        menu_array[*menu_count].submenu_count = 0;

        cJSON_ArrayForEach(submenu, submenus) {
            strcpy(menu_array[*menu_count].submenus[menu_array[*menu_count].submenu_count].name, 
                cJSON_GetObjectItem(submenu, "name")->valuestring);
            strcpy(menu_array[*menu_count].submenus[menu_array[*menu_count].submenu_count].perms, 
                cJSON_GetObjectItem(submenu, "perms")->valuestring);
            menu_array[*menu_count].submenu_count++;
        }
        (*menu_count)++;
    }
}

int main(int argc, char ** argv) {
    // Read JSON file
    char *json_data = read_file(argv[1]);
    if (!json_data) {
        return 1;
    }

    // Parse JSON data using cJSON
    cJSON *json = cJSON_Parse(json_data);
    if (!json) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(json_data);
        return 1;
    }

    // Get controls and menus
    cJSON *controls = cJSON_GetObjectItem(json, "controls");
    cJSON *menus = cJSON_GetObjectItem(json, "menus");

    // Arrays to hold parsed data
    Control_t control_array[50];  // Maximum 50 controls
    Menu_t menu_array[10];        // Maximum 10 menus
    int control_count = 0;
    int menu_count = 0;

    // Parse and populate the data structures
    parse_controls(controls, control_array, &control_count);
    parse_menus(menus, menu_array, &menu_count);

    // Output the parsed data
    printf("Parsed controls:\n");
    for (int i = 0; i < control_count; i++) {
        printf("Control: %s, Text: %s, Target: %s, Linked: %s Min: %d, Max: %d, Size: %d, Scale: %.2f",
            control_array[i].name, control_array[i].text, control_array[i].target, control_array[i].linked, control_array[i].min, 
            control_array[i].max, control_array[i].size, control_array[i].scale);

        // Print the allowed field content if it exists
        if (strlen(control_array[i].allowed[0]) > 0) {
            printf(", Allowed: ");
            for (int j = 0; j < 5; j++) {
                if (strlen(control_array[i].allowed[j]) > 0) {
                    printf("%s", control_array[i].allowed[j]);
                    if (j < 4 && strlen(control_array[i].allowed[j + 1]) > 0) {
                        printf(", ");
                    }
                }
            }
        }
        printf("\n");
    }

    printf("\nParsed menus:\n");
    for (int i = 0; i < menu_count; i++) {
        printf("Menu: %s, Text: %s\n", menu_array[i].name, menu_array[i].text);
        for (int j = 0; j < menu_array[i].submenu_count; j++) {
            printf("  Submenu: %s, Perms: %s\n", menu_array[i].submenus[j].name, menu_array[i].submenus[j].perms);
        }
    }

    // free
    cJSON_Delete(json);
    free(json_data);
    return 0;
}
