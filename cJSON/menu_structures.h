/**
 * @file
 * This file contains the header for the menu structures
 *
 * @author Paul Vogel
 * @date January 16, 2024
 * @copyright Long Range Systems, LLC
 */

#if !defined(MENU_STRUCTURES_H_)
#define MENU_STRUCTURES_H_

#include <stdint.h>
#include <stdbool.h>


#define _NUM_FONTS_ 16

/** Definition of menu entry types */
typedef enum
{
    MENU_ENTRY_none = 0,                /*!< unused entry type */
    MENU_ENTRY_menu = 1,                /*!< Entry is a menu type */
    MENU_ENTRY_numeric = 2,             /*!< Entry is a numeric type */
    MENU_ENTRY_percent = 3,             /*!< Entry is a percent type */
    MENU_ENTRY_boolean = 4,             /*!< Entry is a boolean type */
    MENU_ENTRY_r5g6b5 = 5,              /*!< Entry is R5:G6:B5 color */
    MENU_ENTRY_r8g8b8 = 6,              /*!< Entry is R8:G8:B8 color */
    MENU_ENTRY_font = 7,                /*!< Entry is font size */
    MENU_ENTRY_date = 8,                /*!< Entry is date value */
    MENU_ENTRY_time = 9,                /*!< Entry is time value */
    MENU_ENTRY_alert = 10,              /*!< Entry is alert control */
} MenuEntryType_t;

/** Definition for menu access permission */
typedef enum
{
    MENU_PERMS_none = 0x00,             /*!< not readable or writable */
    MENU_PERMS_read_only = 0x01,        /*!< read only mode */
    MENU_PERMS_read_write = 0x02,       /*!< read/write mode */
} MenuEntryPermissions_t;

/** Definition of the color target */
typedef enum
{
    MENU_COLOR_TARGET_any = 0,             /*!< any use */
    MENU_COLOR_TARGET_foreground = 1,      /*!< used for text foreground */
    MENU_COLOR_TARGET_background = 2,      /*!< used for text background */
} MenuColorEntryTarget_t;

/** Definition of a menu type */
typedef struct {
    const struct menu_entry_s *head;    /*!< Pointer to first entry in menu */
} MenuEntryMenu_t;

/** Definition of a numeric menu entry */
typedef struct
{
    uint32_t min;                       /*!< minimum value allowed */
    uint32_t max;                       /*!< maximum value allowed */
    uint32_t scale;                     /*!< scale factor */
    union
    {
        uint8_t (*get8)(void);          /*!< Pointer to function to get uint8_t
                                         * value */
        uint16_t (*get16)(void);        /*!< Pointer to function to get uint16_t
                                         * value */
        uint32_t (*get32)(void);        /*!< Pointer to function to get uint32_t
                                         * value */
    };
    union
    {
        void (*set8)(uint8_t val);      /*!< Pointer to function to set uint8_t
                                         * value */
        void (*set16)(uint16_t val);    /*!< Pointer to function to set uint16_t
                                         * value */
        void (*set32)(uint32_t val);    /*!< Pointer to function to set uint32_t
                                         * value */
    };
    uint8_t size;                       /*!< size in bits (8, 16, 32) */
} MenuEntryNumeric_t;

/** Definition of a percent menu entry */
typedef struct
{
    uint8_t (*get_percent)(void);       /*!< Pointer to function to get
                                         * percent */
    void (*set_percent)(uint8_t val);   /*!< Pointer to function to set
                                          percent */
} MenuEntryPercent_t;

/** Definition of boolean menu entry */
typedef struct
{
    bool (*get_boolean)(void);          /*!< Pointer to function to get
                                         * boolean */
    void (*set_boolean)(bool val);      /*!< Pointer to function to set
                                         * boolean */
} MenuEntryBoolean_t;

/** Definition of r5g6b5 color menu entry */
typedef struct
{
    MenuColorEntryTarget_t target;      /*!< what the color is used for */
    uint16_t (*get_color)(void);        /*!< Pointer to function to get
                                         * color */
    void (*set_color)(uint16_t val);    /*!< Pointer to function to set
                                         * color */
    uint16_t (*get_linked)(void);       /*!< Pointer to function to get
                                         * linked color, may be NULL */
} MenuEntryR5G6B5_t;

/** Definition of r8g8b8 color menu entry */
typedef struct
{
    uint32_t (*get_color)(void);        /*!< Pointer to function to get
                                         * color */
    void (*set_color)(uint32_t val);    /*!< Pointer to function to set
                                         * color */
} MenuEntryR8G8B8_t;

typedef struct FontTableIndex_s
{
    uint8_t font_index;                       /*!< font index */
    char * font_name;                      /*!< font name */
} FontTableIndex_t;
/** Definition of font menu entry */
typedef struct
{
    bool allowed[_NUM_FONTS_];                  /*!< which fonts are allowed */
    FontTableIndex_t (*get_font)(void);         /*!< Pointer to function to get
                                                 * font size */
    void (*set_font)(FontTableIndex_t val);     /*!< Pointer to function to set
                                                 * font size */
} MenuEntryFont_t;

/** Definition of a date menu entry */
typedef struct
{

    /** Pointer to function to get date value */
    void (*get_date)(char *format, uint16_t *year, uint8_t *month,
                     uint8_t *day);

    /** Pointer to function to set date value */
    void (*set_date)(uint16_t year, uint8_t month, uint8_t day);
} MenuEntryDate_t;


/** Definition of a time menu entry */
typedef struct
{
    /** Pointer to function to get time value */
    void (*get_time)(char *format, uint8_t *hour, uint8_t *minute,
                     uint8_t *second);

    /** Pointer to function to set date value */
    void (*set_time)(uint8_t hour, uint8_t minute, uint8_t second);
} MenuEntryTime_t;

typedef struct {
    uint8_t alert_id;                     /*!< alert id */
    uint8_t alert_type;                   /*!< alert type */
} AlertControl_t;

/** Definition of an alert control menu entry */
typedef struct
{
    /** Pointer to function to get alert control value */
    AlertControl_t (*get_alert)(void);

    /** Pointer to function to set alert control value */
    void (*set_alert)(AlertControl_t value);
} MenuEntryAlert_t;

/** Definition of menu control types */
typedef struct
{
    MenuEntryType_t type;                  /*!< type of menu entry control */
    char *key;                             /*!< key for entry */
    char *text;                            /*!< text for entry */
    union
    {
        const MenuEntryMenu_t *menu;       /*!< pointer to menu entry type */
        const MenuEntryNumeric_t *numeric; /*!< pointer to numeric entry type */
        const MenuEntryPercent_t *percent; /*!< pointer to percent entry type */
        const MenuEntryBoolean_t *boolean; /*!< pointer to boolean entry type */
        const MenuEntryR5G6B5_t *r5g6b5;   /*!< pointer to r5g6b6 color type */
        const MenuEntryR8G8B8_t *r8g8b8;   /*!< pointer to r8g8b8 color type */
        const MenuEntryFont_t *font;       /*!< pointer to font entry type */
        const MenuEntryDate_t *date;       /*!< pointer to date entry type */
        const MenuEntryTime_t *time;       /*!< pointer to time entry type */
        const MenuEntryAlert_t *alert;     /*!< pointer to alert entry type */
    };
} MenuControl_t;

/** Definition of a menu entry */
typedef struct menu_entry_s
{
    MenuControl_t control;              /*!< the menu control entry */
    MenuEntryPermissions_t perms;       /*!< read/write permissions */

    const struct menu_entry_s *parent;     /*!< pointer to parent menu entry */
    const struct menu_entry_s *next;       /*!< pointer to next entry */
    const struct menu_entry_s *prev;       /*!< pointer to previous entry */
} MenuEntry_t;

extern const MenuEntry_t *menu_get_root_entry(void);

extern const MenuControl_t *menu_get_controls_table(size_t *count);

#endif
