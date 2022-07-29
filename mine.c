#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "mine.h"

/* Defaults */
size_t ROWS = 20;       
size_t COLS = 20;       
size_t PERCENTAGE = 20; 

size_t FIELD_MIN_LIMIT = 20;
size_t FIELD_MAX_LIMIT = 128;
size_t PERCENTAGE_MIN_LIMIT = 1;
size_t PERCENTAGE_MAX_LIMIT = 60;

/* Cell State Enum */
enum cell_state {
    closed,
    opened,
    flagged
};

/* Cell Content */
enum cell_value {
    empty,
    bomb
};

/* Cell structure */
struct cell {
    enum cell_state state;
    enum cell_value value;
};

/* Main playground struct */
struct field {
    size_t rows, cols, percentage;
    struct cell* cells;
};

int main2(void) {
    return 0;
}

void field_init();

int main(int argc, char** argv) {
    struct field main;

    get_arguments(argc, argv);
    //field_init(&main, ROWS, COLS, PERCENTAGE);

#ifdef DEBUG
    printf("ROWS: %zu\n", ROWS);
    printf("COLS: %zu\n", COLS);
    printf("PERCENTAGE: %zu\n", PERCENTAGE);
#endif
    return 0;
}