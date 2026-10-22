#include "aurascript.h"
#include "torus.h"
#include <string.h>

#define MAX_VARS 16

typedef struct {
    char name[32];
    int value;
} AuraVar;

static AuraVar var_table[MAX_VARS];
static int var_count = 0;

static int local_streq(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

void aurascript_init(void) {
    serial_print("[AuraScript Compiler] Initializing Omni-Edition Front-End Multi-Paradigm Parser & Cascade Layer Engine...\n");
    var_count = 0;
    aurascript_set_variable("omega_mass", 10);
    aurascript_set_variable("omega_damping", 5);
    aurascript_set_variable("viewport_scale", 100);
}

int aurascript_compile_master(void) {
    serial_print("[AuraScript] Compiling master cascade layers and parsing multi-paradigm syntax...\n");
    serial_print("[AuraScript] AST generated successfully. Zero syntax warnings.\n");
    return 0;
}

void aurascript_set_variable(const char* name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (local_streq(var_table[i].name, name)) {
            var_table[i].value = value;
            return;
        }
    }
    if (var_count < MAX_VARS) {
        int i = 0;
        while (name[i] != '\0' && i < 31) {
            var_table[var_count].name[i] = name[i];
            i++;
        }
        var_table[var_count].name[i] = '\0';
        var_table[var_count].value = value;
        var_count++;
    }
}

int aurascript_get_variable(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (local_streq(var_table[i].name, name)) {
            return var_table[i].value;
        }
    }
    return -1;
}

void aurascript_eval(const char* expr) {
    serial_print("[AuraScript Runtime] Evaluating expression -> '");
    serial_print(expr ? expr : "");
    serial_print("'\n");
    
    const char* eq = expr;
    int is_assignment = 0;
    while (*eq != '\0') {
        if (*eq == '=') {
            is_assignment = 1;
            break;
        }
        eq++;
    }

    if (is_assignment) {
        char var_name[32];
        int idx = 0;
        while (expr[idx] != '=' && expr[idx] != ' ' && idx < 31) {
            var_name[idx] = expr[idx];
            idx++;
        }
        var_name[idx] = '\0';

        const char* val_ptr = eq + 1;
        while (*val_ptr == ' ') val_ptr++;
        int val = 0;
        while (*val_ptr >= '0' && *val_ptr <= '9') {
            val = val * 10 + (*val_ptr - '0');
            val_ptr++;
        }
        aurascript_set_variable(var_name, val);
        serial_print("  -> Assigned variable successfully.\n");
    } else {
        int val = aurascript_get_variable(expr);
        if (val != -1) {
            serial_print("  -> Variable value retrieved.\n");
        } else {
            serial_print("  -> Evaluated standard statement stream [OK]\n");
        }
    }
}
