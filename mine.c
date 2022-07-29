#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "mine.h"
#include "utils.h"

/* Defaults */
size_t ROWS = 20;
size_t COLS = 20;
size_t PERCENTAGE = 20;

size_t FIELD_MIN_LIMIT = 20;
size_t FIELD_MAX_LIMIT = 128;
size_t PERCENTAGE_MIN_LIMIT = 1;
size_t PERCENTAGE_MAX_LIMIT = 60;

void field_init(struct Field* field, size_t rows, size_t cols, size_t perc) {
    field->cols = cols;
    field->rows = rows;
    field->percentage = perc;

    field->cursor.col = 0;
    field->cursor.row = 0;

    field->cells = (struct Cell*)calloc(rows * cols, sizeof(struct Cell));
    if (field->cells == NULL) {
        printf("ERROR: memory was not allocated\n");
        exit(EXIT_FAILURE);
    }

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            field->cells[row * col + col].value = empty;
            field->cells[row * col + col].state = closed;
        }
    }
}

void field_redisplay(struct Field* field) {
    putchar(27);
    putchar('[');
    putchar(field->rows);
    putchar('A');    
    putchar(27);
    putchar('[');
    putchar(field->cols*3);
    putchar('D');
     
    field_display(field);
}

void field_display(struct Field* field) {
    for (size_t row = 0; row < field->rows; row++) {
        for (size_t col = 0; col < field->cols; col++) {
            bool at_cursor = field_at_cursor(field, row, col);
            printf("%c", at_cursor ? '[' : ' ');
            switch (field->cells[row * col + col].state) {
                case closed:
                    printf(".");
                    break;

                case opened:
                    break;

                case flagged:
                    break;

                default:
                    break;
            }
            printf("%c", at_cursor ? ']' : ' ');
        }
        printf("\n");
    }
}

bool field_at_cursor(struct Field* field, size_t row, size_t col) {
    return field->cursor.row == row && field->cursor.col == col;
}

void field_free(struct Field* field) {
    free(field->cells);
}

int main(int argc, char** argv) {
    struct Field main;
    bool exited = false;
    char c;

    set_input_mode();
    get_arguments(argc, argv);

#ifdef DEBUG
    printf("ROWS: %zu\n", ROWS);
    printf("COLS: %zu\n", COLS);
    printf("PERCENTAGE: %zu\n", PERCENTAGE);
#endif

    field_init(&main, ROWS, COLS, PERCENTAGE);
    field_display(&main);

    while (!exited){
        field_redisplay(&main);
    }
    

    return EXIT_SUCCESS;
}