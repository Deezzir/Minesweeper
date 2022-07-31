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
            field->cells[row * col + col].value = empty;
            field->cells[row * col + col].state = closed;
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
        
        default:
            break;
        }
    }

    field_free(&main);
    clear_screen();
    printf("Goodbye, stranger!\n");

    return EXIT_SUCCESS;
}