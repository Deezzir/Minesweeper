#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/* Defaults */
size_t ROWS = 20;       /* Number of rows */
size_t COLS = 20;       /* Number of cols */
size_t PERCENTAGE = 20; /* Percentage of bombs */

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
struct Playground {
    int rows, cols;
    struct cell* cells;
};

int main2(void) {
    return 0;
}

void get_arguments(int argc, char** argv) {
    int letter = -1;
    int value = 0;
    if (argc > 1) {
        do {
            letter = options(argc, argv, "r:c:p:");
            value = strtoul(opt_arg, NULL, 10);
            if (value == 0) {
                printf("invalid option argument: [-%c num] - must be followed by number\n", letter);
                invalid_arg_exit();
            }

            switch (letter) {
                case 'r':
                    if (value != 0) ROWS = value;
                    break;
                case 'c':
                    if (value != 0) COLS = value;
                    break;
                case 'p':
                    if (value != 0) PERCENTAGE = value;
                    break;
                case '?':
                    printf("invalid option: %s\n", argv[opt_index - 1]);
                    invalid_arg_exit();
                    break;
                default:
                    break;
            }
        } while (opt_index != argc && letter != -1);
    }
}

int main(int argc, char** argv) {
    get_arguments(argc, argv);

    printf("ROWS: %zu\n", ROWS);
    printf("COLS: %zu\n", COLS);
    printf("PERCENTAGE: %zu\n", PERCENTAGE);
    return 0;
}