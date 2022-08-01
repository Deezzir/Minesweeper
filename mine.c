#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "mine.h"
#include "utils.h"

/* Defaults */
uint ROWS = 20;
uint COLS = 20;
uint PERCENTAGE = 20;

uint FIELD_MIN_LIMIT = 10;
uint FIELD_MAX_LIMIT = 128;
uint PERCENTAGE_MIN_LIMIT = 1;
uint PERCENTAGE_MAX_LIMIT = 60;

bool is_running;

void field_init(struct Field* field, uint rows, uint cols, uint perc) {
    field->cols = cols;
    field->rows = rows;
    field->percentage = perc;
    field->generated = false;
#if DEBUG
    field->peeked = false;
#endif

    field->cursor.col = 0;
    field->cursor.row = 0;

    if (field->cells != NULL) field_free(field);
    field->cells = (struct Cell*)calloc(rows * cols, sizeof(struct Cell));
    if (field->cells == NULL) {
        printf("ERROR: memory was not allocated\n");
        exit(EXIT_FAILURE);
    }

    for (uint row = 0; row < rows; row++) {
        for (uint col = 0; col < cols; col++) {
            uint index = row * cols + col;
            field->cells[index].state = closed;
            field->cells[index].value = empty;
            field->cells[index].neighbor_count = 0;
        }
    }
}

void field_redisplay(struct Field* field) {
#if DEBUG
    printf("\033[1A");
    printf("\033[24D");
#endif
    printf("\033[%dA", (int)field->rows);
    printf("\033[%dD", (int)field->cols * 3);
    field_display(field);
}

void field_display(struct Field* field) {
#if DEBUG
    printf("CURSOR-> ROW: %02u COL: %02u\n", field->cursor.row, field->cursor.col);
#endif
    for (uint row = 0; row < field->rows; row++) {
        for (uint col = 0; col < field->cols; col++) {
            bool at_cursor = field_at_cursor(field, row, col);
            struct Cell cell = field_get_cell(field, row, col);
            printf("%c", at_cursor ? '[' : ' ');
#if DEBUG
            if (field->peeked)
                switch (cell.value) {
                    case empty:
                        cell.neighbor_count > 0 ? printf("%u", cell.neighbor_count) : printf(" ");
                        break;

                    case bomb:
                        printf("@");
                        break;

                    default:
                        break;
                }
            else
#endif
                switch (cell.state) {
                    case closed:
                        printf("*");
                        break;

                    case opened:
                        switch (cell.value) {
                        case empty:
                            cell.neighbor_count > 0 ? printf("%u", cell.neighbor_count) : printf(" ");
                            break;
                        
                        case bomb:
                            printf("@");
                            break;
                        
                        default:
                            break;
                        }
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

struct Cell* field_get_cell_ref(struct Field* field, uint row, uint col) {
    return &(field->cells[row * field->cols + col]);
}

struct Cell field_get_cell(struct Field* field, uint row, uint col) {
    return field->cells[row * field->cols + col];
}

bool field_at_cursor(struct Field* field, uint row, uint col) {
    return field->cursor.row == row && field->cursor.col == col;
}

void field_free(struct Field* field) {
    free(field->cells);
}

void field_cursor_move_right(struct Field* field) {
    if (field->cursor.col < field->cols - 1) field->cursor.col += 1;
}

void field_cursor_move_up(struct Field* field) {
    if (field->cursor.row > 0) field->cursor.row -= 1;
}

void field_cursor_move_down(struct Field* field) {
    if (field->cursor.row < field->rows - 1) field->cursor.row += 1;
}

void field_cursor_move_left(struct Field* field) {
    if (field->cursor.col > 0) field->cursor.col -= 1;
}

bool field_out_of_bounds(struct Field* field, int row, int col) {
    return row < 0 || row >= field->rows || col < 0 || col >= field->cols;
}

void field_flag_cell(struct Field* field) {
    struct Cell* cell = field_get_cell_ref(field, field->cursor.row, field->cursor.col);
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

void field_generate(struct Field* field) {
    // TODO: Improve field generaion
    uint bombs_count = (field->rows * field->cols * field->percentage) / 100;

    while (bombs_count > 0) {
        uint col = rand() % field->cols;
        uint row = rand() % field->rows;

        if (field->cursor.row != row || field->cursor.col != col) {
            struct Cell* cell = field_get_cell_ref(field, row, col);
            if (cell->value != bomb) {
                for (int x = -1; x <= 1; x++)
                    for (int y = -1; y <= 1; y++)
                        if ((x != 0 || y != 0) && !field_out_of_bounds(field, row + x, col + y)) 
                            field_get_cell_ref(field, row + x, col + y)->neighbor_count++;

                cell->value = bomb;
                bombs_count--;
            }
        }
    }

    field->generated = true;
}

bool field_cell_open_at(struct Field* field, int row, int col) {
    struct Cell* cell = field_get_cell_ref(field, row, col);
    cell->state = opened; 

    if (cell->neighbor_count == 0) 
        for (int x = -1; x <= 1; x++)
            for (int y = -1; y <= 1; y++)
                if (!field_out_of_bounds(field, row + x, col + y))
                    if (field_get_cell(field, row + x, col + y).state == closed)
                        if (field_cell_open_at(field, row + x, col + y))
                            return true;

    return cell->value == bomb;
}

bool field_cell_open(struct Field* field) {
    return field_cell_open_at(field, field->cursor.row, field->cursor.col);
}

void field_mark_all_bombs(struct Field* field, int state) {
    for (uint row = 0; row < field->rows; row++)
        for (uint col = 0; col < field->cols; col++) {
            struct Cell* cell = field_get_cell_ref(field, row, col);
            if (cell->value == bomb) cell->state = state;
        }   
}

void field_finale(struct Field* field, int exodus) {
    field_mark_all_bombs(field, exodus == win ? flagged : opened);
    field_redisplay(field);

    exodus == win ? printf("You won, try again? Y/y N/n: ") 
        : printf("You lost, try again? Y/y N/n: ");

    if (yes()) {
        field_init(field, ROWS, COLS, PERCENTAGE);
        field_redisplay(field);
    } else is_running = false; 
}

void field_defeat(struct Field* field) {
    field_finale(field, defeat);
}

void field_win(struct Field* field) {
    field_finale(field, win);
}

bool field_is_win(struct Field* field) {
    for (uint row = 0; row < field->rows; row++)
        for (uint col = 0; col < field->cols; col++) {
            struct Cell cell = field_get_cell(field, row, col);
            switch (cell.state) {
            case closed:
            case flagged:
                if(cell.value != bomb) return false;
                break;
            
            case opened:
                if (cell.value != empty) return false;
                break;

            default:
                break;
            }
        }

    return true;
}

int main(int argc, char** argv) {
    struct Field main = {
        .cells = NULL
    };
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
        printf("ERROR: failed to set signal action");
        exit(EXIT_FAILURE);
    }

    // Use current time as seed for random generator
    srand(time(0));

#ifdef DEBUG
    printf("ROWS: %u ", ROWS);
    printf("COLS: %u ", COLS);
    printf("PERCENTAGE: %u\n", PERCENTAGE);
#endif

    field_init(&main, ROWS, COLS, PERCENTAGE);
    field_display(&main);

    while (is_running) {
        field_redisplay(&main);
        input = getchar();

        switch (input) {
            case 'q':
                printf("Exit the game? Y/y N/n: ");
                is_running = !yes();
                break;

            case 'w':
            case '\101':  // arrow up
                field_cursor_move_up(&main);
                break;

            case 'a':
            case '\104':  // arrow left
                field_cursor_move_left(&main);
                break;

            case 's':
            case '\102':  // arrow down
                field_cursor_move_down(&main);
                break;

            case 'd':
            case '\103':  // arrow right
                field_cursor_move_right(&main);
                break;

            case 'f':
                field_flag_cell(&main);
                break;

            case ' ': // space
                if (!main.generated)
                    field_generate(&main); 

                if (field_cell_open(&main))
                    field_defeat(&main);
                else 
                    if(field_is_win(&main)) field_win(&main);
                break;

            case 'r':
                printf("Restart the game? Y/y N/n: ");
                if (yes())
                    field_init(&main, ROWS, COLS, PERCENTAGE);
                break;
#if DEBUG
            case 'p':
                main.peeked = !main.peeked;
                break;
#endif
            default:
                break;
        }
    }

    field_free(&main);
    clear_screen();
    printf("Goodbye, stranger!\n");

    return EXIT_SUCCESS;
}
