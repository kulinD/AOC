#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "cJSON.h"
#include "menu_structures.h"

// Define font table
FontTableIndex_t font_table[_NUM_FONTS_] = 
{
    {0, "small"},
    {1, "medium"},
    {2, "large"},
    {3, "large_digits"}
};

// Function to read JSON file into a string
char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Could not open file '%s'\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int32_t length = ftell(file);
    if (length < 0)
    {
        printf("Error: Could not determine file size\n");
        fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (!buffer)
    {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, length, file);
    if (read_size != length)
    {
        printf("Error: Could not read file contents\n");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

// Function to parse and print controls
void parse_controls(cJSON *controls)
{
    if (!controls)
    {
        printf("Error: No controls found in JSON\n");
        return;
    }

    cJSON *control = NULL;
    printf("Parsed controls:\n");
    cJSON_ArrayForEach(control, controls)
    {
        MenuControl_t menu_control = {0};

        cJSON *numeric = cJSON_GetObjectItem(control, "numeric");
        cJSON *boolean = cJSON_GetObjectItem(control, "boolean");
        cJSON *r5g6b5 = cJSON_GetObjectItem(control, "r5g6b5");
        cJSON *percent = cJSON_GetObjectItem(control, "percent");
        cJSON *font = cJSON_GetObjectItem(control, "font");
        cJSON *date = cJSON_GetObjectItem(control, "date");
        cJSON *time = cJSON_GetObjectItem(control, "time");
        cJSON *alert = cJSON_GetObjectItem(control, "alert");

        if (numeric)
        {
            menu_control.type = MENU_ENTRY_numeric;
            menu_control.key = strdup(cJSON_GetObjectItem(numeric, "name")->valuestring);
            menu_control.text = strdup(cJSON_GetObjectItem(numeric, "text")->valuestring);

            MenuEntryNumeric_t *numeric_data = malloc(sizeof(MenuEntryNumeric_t));
            if (!numeric_data)
            {
                printf("Error: Memory allocation failed for numeric_data\n");
                free(menu_control.key);
                free(menu_control.text);
                continue;
            }

            numeric_data->min = cJSON_GetObjectItem(numeric, "min")->valueint;
            numeric_data->max = cJSON_GetObjectItem(numeric, "max")->valueint;
            numeric_data->size = cJSON_GetObjectItem(numeric, "size")->valueint;

            cJSON *scale_item = cJSON_GetObjectItem(numeric, "scale");
            if (scale_item)
            {
                numeric_data->scale = scale_item->valueint;
                printf("Control: numeric, Name: %s, Text: %s, Min: %u, Max: %u, Size: %u, Scale: %u\n",
                       menu_control.key, menu_control.text, numeric_data->min, numeric_data->max, numeric_data->size, numeric_data->scale);
            }
            else
            {
                printf("Control: numeric, Name: %s, Text: %s, Min: %u, Max: %u, Size: %u\n",
                       menu_control.key, menu_control.text, numeric_data->min, numeric_data->max, numeric_data->size);
            }

            menu_control.numeric = numeric_data;
        }

        if (boolean)
        {
            menu_control.type = MENU_ENTRY_boolean;
            menu_control.key = strdup(cJSON_GetObjectItem(boolean, "name")->valuestring);
            menu_control.text = strdup(cJSON_GetObjectItem(boolean, "text")->valuestring);

            if (!menu_control.key || !menu_control.text)
            {
                printf("Error: Memory allocation failed for boolean control\n");
                free(menu_control.key);
                free(menu_control.text);
                continue;
            }

            printf("Control: boolean, Name: %s, Text: %s\n", menu_control.key, menu_control.text);
        }

        if (r5g6b5)
        {
            menu_control.type = MENU_ENTRY_r5g6b5;
            menu_control.key = strdup(cJSON_GetObjectItem(r5g6b5, "name")->valuestring);
            menu_control.text = strdup(cJSON_GetObjectItem(r5g6b5, "text")->valuestring);

            MenuEntryR5G6B5_t *r5g6b5_data = malloc(sizeof(MenuEntryR5G6B5_t));
            if (!r5g6b5_data)
            {
                printf("Error: Memory allocation failed for r5g6b5_data\n");
                free(menu_control.key);
                free(menu_control.text);
                continue;
            }

            const char *target_str = cJSON_GetObjectItem(r5g6b5, "target")->valuestring;
            if (strcmp(target_str, "foreground") == 0)
            {
                r5g6b5_data->target = MENU_COLOR_TARGET_foreground;
            } 
            else if (strcmp(target_str, "background") == 0)
            {
                r5g6b5_data->target = MENU_COLOR_TARGET_background;
            }
            else
            {
                r5g6b5_data->target = MENU_COLOR_TARGET_any;
            }

            const char *linked_str;
            if (cJSON_GetObjectItem(r5g6b5, "linked"))
            {
                linked_str = cJSON_GetObjectItem(r5g6b5, "linked")->valuestring;
            }
            else
            {
                linked_str = "none";
            }
            printf("Control: r5g6b5, Name: %s, Text: %s, Target: %s, Linked: %s\n",
                   menu_control.key, menu_control.text, target_str, linked_str);

            menu_control.r5g6b5 = r5g6b5_data;
        }

        if (percent)
        {
            menu_control.type = MENU_ENTRY_percent;
            menu_control.key = strdup(cJSON_GetObjectItem(percent, "name")->valuestring);
            menu_control.text = strdup(cJSON_GetObjectItem(percent, "text")->valuestring);

            if (!menu_control.key || !menu_control.text)
            {
                printf("Error: Memory allocation failed for percent control\n");
                free(menu_control.key);
                free(menu_control.text);
                continue;
            }

            printf("Control: percent, Name: %s, Text: %s\n", menu_control.key, menu_control.text);
        }

        if (font)
        {
            menu_control.type = MENU_ENTRY_font;
            menu_control.key = strdup(cJSON_GetObjectItem(font, "name")->valuestring);
            menu_control.text = strdup(cJSON_GetObjectItem(font, "text")->valuestring);

            MenuEntryFont_t *font_data = malloc(sizeof(MenuEntryFont_t));
            if (!font_data)
            {
                printf("Error: Memory allocation failed for font_data\n");
                free(menu_control.key);
                free(menu_control.text);
                continue;
            }
            memset(font_data->allowed, 0, sizeof(font_data->allowed));

            cJSON *allowed = cJSON_GetObjectItem(font, "allowed");
            cJSON *allowed_item = NULL;
            printf("Control: font, Name: %s, Text: %s, Allowed: ", menu_control.key, menu_control.text);
            cJSON_ArrayForEach(allowed_item, allowed)
            {
                const char *font_name = allowed_item->valuestring;
                printf("%s ", font_name);
                for (int i = 0; i < _NUM_FONTS_; i++)
                {
                    if (strcmp(font_name, font_table[i].font_name) == 0)
                    {
                        font_data->allowed[i] = true;
                        break;
                    }
                }
            }
            printf("\n");
            menu_control.font = font_data;
        }
        
        if (date)
        {
            menu_control.type = MENU_ENTRY_date;
            menu_control.key = strdup(cJSON_GetObjectItem(date, "name")->valuestring);
            menu_control.text = strdup(cJSON_GetObjectItem(date, "text")->valuestring);

            if (!menu_control.key || !menu_control.text)
            {
                printf("Error: Memory allocation failed for date control\n");
                free(menu_control.key);
                free(menu_control.text);
                continue;
            }

            printf("Control: date, Name: %s, Text: %s\n", menu_control.key, menu_control.text);
        }

        if (time)
        {
            menu_control.type = MENU_ENTRY_time;
            menu_control.key = strdup(cJSON_GetObjectItem(time, "name")->valuestring);
            menu_control.text = strdup(cJSON_GetObjectItem(time, "text")->valuestring);

            if (!menu_control.key || !menu_control.text)
            {
                printf("Error: Memory allocation failed for time control\n");
                free(menu_control.key);
                free(menu_control.text);
                continue;
            }

            printf("Control: time, Name: %s, Text: %s\n", menu_control.key, menu_control.text);
        }

        if (alert)
        {
            menu_control.type = MENU_ENTRY_alert;
            menu_control.key = strdup(cJSON_GetObjectItem(alert, "name")->valuestring);
            menu_control.text = strdup(cJSON_GetObjectItem(alert, "text")->valuestring);

            if (!menu_control.key || !menu_control.text)
            {
                printf("Error: Memory allocation failed for alert control\n");
                free(menu_control.key);
                free(menu_control.text);
                continue;
            }

            printf("Control: alert, Name: %s, Text: %s\n", menu_control.key, menu_control.text);
        }
    }
}

// Function to parse and print menus
void parse_menus(cJSON *menus)
{
    if (!menus)
    {
        printf("Error: No menus found in JSON\n");
        return;
    }

    cJSON *menu = NULL;
    printf("\nParsed menus:\n");
    cJSON_ArrayForEach(menu, menus)
    {
        const char *name = cJSON_GetObjectItem(menu, "name")->valuestring;
        const char *text = cJSON_GetObjectItem(menu, "text")->valuestring;
        printf("Menu: Name: %s, Text: %s\n", name, text);

        cJSON *submenus = cJSON_GetObjectItem(menu, "submenus");
        cJSON *submenu = NULL;
        cJSON_ArrayForEach(submenu, submenus)
        {
            const char *submenu_name = cJSON_GetObjectItem(submenu, "name")->valuestring;
            const char *perms = cJSON_GetObjectItem(submenu, "perms")->valuestring;
            printf("  Submenu: Name: %s, Perms: %s\n", submenu_name, perms);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Expected: %s <menu.json>\n", argv[0]);
        return 1;
    }

    // Read JSON file
    char *json_data = read_file(argv[1]);
    if (!json_data)
    {
        return 1;
    }

    // Parse JSON data using cJSON
    cJSON *json = cJSON_Parse(json_data);
    if (!json)
    {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(json_data);
        return 1;
    }

    // Get controls and menus
    cJSON *controls = cJSON_GetObjectItem(json, "controls");
    cJSON *menus = cJSON_GetObjectItem(json, "menus");

    // Parse and print controls
    if (controls)
    {
        parse_controls(controls);
    }

    // Parse and print menus
    if (menus)
    {
        parse_menus(menus);
    }

    // Free allocated memory
    cJSON_Delete(json);
    free(json_data);
    return 0;
}