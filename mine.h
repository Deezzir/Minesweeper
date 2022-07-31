#ifndef __MINE_H
#define __MINE_H

#include <stdbool.h>

/* Defaults */
extern size_t ROWS;           /* Number of rows */
extern size_t COLS;           /* Number of cols */
extern size_t PERCENTAGE;     /* Percentage of bombs */
 
extern size_t FIELD_MIN_LIMIT;       /* Min Number of rows and cols */
extern size_t FIELD_MAX_LIMIT;       /* Max Number of rows and cols */
extern size_t PERCENTAGE_MIN_LIMIT;  /* Min percentage of mines */
extern size_t PERCENTAGE_MAX_LIMIT;  /* Max percentage of mines */

extern bool is_running;              /* program status */

/* Cell State enum */
enum CellState {
    closed,
    opened,
    flagged
};

/* Cell Value enum */
enum CellValue {
    empty,
    bomb
};

/* Cell struct */
struct Cell {
    enum CellState state;
    enum CellValue value;
};

/* Cursor struct */
struct Cursor {
    size_t row, col;
};

/* Main playground Struct */
struct Field {
    size_t rows, cols, percentage;
    struct Cell* cells;
    struct Cursor cursor;
};


/* Function to initialize Field struct instance */
void field_init(struct Field* field, size_t rows, size_t cols, size_t perc);

/* Function to display Field struct instance */
void field_display(struct Field* field);

/* Function to redisplay Field struct instance overwriting previos output content */
void field_redisplay(struct Field* field);

/* Function to free memory allocated for cells array inside Field struct */
void field_free(struct Field* field);

/* Function returns true if specified row and col is the position of the game cursor, false otherwise  */
bool field_at_cursor(struct Field* field, size_t row, size_t col);

/* Function to move the cursor right */
void field_cursor_move_right(struct Field* field);

/* Function to move the cursor up */
void field_cursor_move_up(struct Field* field);

/* Function to move the cursor down */
void field_cursor_move_down(struct Field* field);

/* Function to move the cursor left */
void field_cursor_move_left(struct Field* field);

/* Function to flag the cell at the cursor */
void field_flag_cell(struct Field* field);

/* Function to get a reference of the cell at the provided row and col */
struct Cell* field_get_cell(struct Field* field, size_t row, size_t col);

#endif // __MINE_H