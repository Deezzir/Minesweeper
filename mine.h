#ifndef __MINE_H
#define __MINE_H

#include <stdbool.h>

typedef unsigned int uint;

/* Defaults */
extern uint ROWS;           /* Number of rows */
extern uint COLS;           /* Number of cols */
extern uint PERCENTAGE;     /* Percentage of bombs */
 
extern uint FIELD_MIN_LIMIT;       /* Min Number of rows and cols */
extern uint FIELD_MAX_LIMIT;       /* Max Number of rows and cols */
extern uint PERCENTAGE_MIN_LIMIT;  /* Min percentage of mines */
extern uint PERCENTAGE_MAX_LIMIT;  /* Max percentage of mines */

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
    uint neighbor_count;
};

/* Cursor struct */
struct Cursor {
    uint row, col;
};

/* Main playground Struct */
struct Field {
    uint rows, cols, percentage;
    struct Cell* cells;
    struct Cursor cursor;
    bool generated;
#if DEBUG
    bool peeked;
#endif
};


/* Function to initialize Field struct instance */
void field_init(struct Field* field, uint rows, uint cols, uint perc);

/* Function to display Field struct instance */
void field_display(struct Field* field);

/* Function to redisplay Field struct instance overwriting previos output content */
void field_redisplay(struct Field* field);

/* Function to free memory allocated for cells array inside Field struct */
void field_free(struct Field* field);

/* Function returns true if specified row and col is the position of the game cursor, false otherwise  */
bool field_at_cursor(struct Field* field, uint row, uint col);

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
struct Cell* field_get_cell_ref(struct Field* field, uint row, uint col);

/* Function to get a copy of the cell at the provided row and col */
struct Cell field_get_cell(struct Field* field, uint row, uint col);

/* Function to generate field */
void field_generate(struct Field* field);

/* Function to check if provided row and col is out of bounds */
bool field_out_of_bounds(struct Field* field, int row, int col);

#endif // __MINE_H