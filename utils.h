#ifndef __UTILS_H
#define __UTILS_H

#include <stdbool.h>

static const char switch_char = '-';
static const char unknown_char = '?';

extern int opt_index; /* first option should be argv[1] */
extern char *opt_arg; /* global option argument pointer */

/*
* options() parses option letters and option arguments from the argv list.
* Succesive calls return succesive option letters which match one of
* those in the legal list. Option letters may require option arguments
* as indicated by a ':' following the letter in the legal list.
* for example, a legal list of "ab:c" implies that a, b and c are
* all valid options and that b takes an option argument. The option
* argument is passed back to the calling function in the value
* of the global OptArg pointer. The opt_index gives the next string
* in the argv[] array that has not already been processed by options().
*
* options() returns -1 if there are no more option letters or if
* double SwitchChar is found. Double SwitchChar forces options()
* to finish processing options.
*
* options() returns '?' if an option not in the legal set is
* encountered or an option needing an argument is found without an
* argument following it.
*
*/
int options(int argc, char *argv[], const char *legal);

/* Prints program usage */
void usage();

/* Gets arguments from the terminal and sets global variables */
void get_arguments(int argc, char** argv);

/* Prints Error message and exits with EINVAL */
void invalid_arg_exit();

/* Function to check if provided target is in range between min and max */
bool is_in_range(int target, int min, int max);

/* Function to reset terminal setting using TermIOS*/
void reset_input_mode(void);

/* Function to set terminal setting using TermIOS*/
void set_input_mode(void);

/*Signal Handler is responsible for*/
/*handling SIGINT signal */
void signal_handler(int signal);

/* Function to clear terminal */
void clearScreen();

#endif  // __UTILS_H