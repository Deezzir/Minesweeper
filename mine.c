#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

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

bool is_running;

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
            field->cells[row * cols + col].value = empty;
            field->cells[row * cols + col].state = closed;
        }
    }
}

void field_redisplay(struct Field* field) {
    printf("\033[%dA", (int)field->rows); 
    printf("\033[%dD", (int)field->cols*3); 
    field_display(field);
     
}

void field_display(struct Field* field) {
    for (size_t row = 0; row < field->rows; row++) {
        for (size_t col = 0; col < field->cols; col++) {
            bool at_cursor = field_at_cursor(field, row, col);
            printf("%c", at_cursor ? '[' : ' ');
            switch (field->cells[row * field->cols + col].state) {
                case closed:
                    printf("*");
                    break;

                case opened:
                    break;

                case flagged:
                    printf("?");
                    break;

                default:
                    break;
            }
            printf("%c", at_cursor ? ']' : ' ');
        }
        printf("\n");
    }
}

struct Cell* field_get_cell(struct Field* field, size_t row, size_t col) {
    return &(field->cells[row * field->cols + col]);
}

bool field_at_cursor(struct Field* field, size_t row, size_t col) {
    return field->cursor.row == row && field->cursor.col == col;
}

void field_free(struct Field* field) {
    free(field->cells);
}

void field_cursor_move_right(struct Field* field) {
    if (field->cursor.col < COLS - 1) field->cursor.col += 1; 
}

void field_cursor_move_up(struct Field* field) {
    if (field->cursor.row > 0) field->cursor.row -= 1;
}

void field_cursor_move_down(struct Field* field) {
    if (field->cursor.row < ROWS - 1) field->cursor.row += 1;
}

void field_cursor_move_left(struct Field* field) {
    if (field->cursor.col > 0) field->cursor.col -= 1;
}

void field_flag_cell(struct Field* field) {
    struct Cell* cell = field_get_cell(field, field->cursor.row, field->cursor.col);
    switch (cell->state) {
    case closed:
        cell->state = flagged;
        break;

    case flagged:
        cell->state = closed;
        break;
    
    default:
        break;
    }
}

int main(int argc, char** argv) {
    struct Field main;
    is_running = true;
    int input;

    set_input_mode();
    get_arguments(argc, argv);

    // Setup signal handler
    struct sigaction action;
    action.sa_handler = &signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) < 0) {
        printf("ERROR:  failed to set signal action");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    printf("ROWS: %zu\n", ROWS);
    printf("COLS: %zu\n", COLS);
    printf("PERCENTAGE: %zu\n", PERCENTAGE);
#endif
  
    field_init(&main, ROWS, COLS, PERCENTAGE);
    field_display(&main);

    while (is_running){
        field_redisplay(&main);
        input = getchar();

        switch (input) {
        case 'q':
            printf("Exit the game? Y/y N/n: ");
            is_running = !yes();
            break;
        
        case 'w':
        case '\101':
            field_cursor_move_up(&main);
            break;

        case 'a':
        case '\104':
            field_cursor_move_left(&main);
            break;

        case 's':
        case '\102':
            field_cursor_move_down(&main);
            break;

        case 'd':
        case '\103':
            field_cursor_move_right(&main);
            break;

        case 'f':
            field_flag_cell(&main);
            break;

        default:
            break;
        }
    }

    field_free(&main);
    clear_screen();
    printf("Goodbye, stranger!\n");

    return EXIT_SUCCESS;
}