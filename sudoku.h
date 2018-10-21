#ifndef SUDOKU_H
#define SUDOKU_H

#define SUDOKU_SUCCESS         0
#define SUDOKU_NO_FILE_ERROR   1
#define SUDOKU_UNKNOWN_ERROR   2
#define SUDOKU_CONTRADICTION   3
#define SUDOKU_ILLEGAL         4
#define SUDOKU_NO_HINT         5
#define SUDOKU_BAD_FORMAT      6
#define SUDOKU_SOLVED          7
#define SUDOKU_ILLEGAL_INDEX   8
#define SUDOKU_NO_SOLUTION     9
#define SUDOKU_ILLEGAL_VALUE     10
#define SUDOKU_ERROR           99
#define SUDOKU_MAX_ERROR       100

/*
  This file describes the interface for a Sudoku Abstract
  Data Type.  
 */

/* 
   Sudoku_initialize: Initialize/reset any global variables or other
      data that you need.  This function should be called first,
      before any other function.
   
   Return values:
      SUDOKU_SUCCESS: initialize succeeded.
      SUDOKU_UNKNOWN_ERROR: there was a problem.
 */
int Sudoku_initialize (void);

/*
  Sudoku_loadfile: Initialize the game to the board held in filename. The
  current game and all of its settings, if any, is discarded.

  The structure of the file must be as follows:
    - A digit represents a value from 1-9 and a - represents an empty game position.
    - There will be 9 lines in the file. Each line corresponds to one
      row of the Sudoku puzzle
    - Each line contains 9 digits/dashes. Each digit/dash falls into one of the 9 columns. 
    - Read in the file and populate your structure.

  Return the following error codes:
     SUDOKU_SUCCESS : file read successfully
     SUDOKU_NO_FILE_ERROR : no file found or cannot open file
     SUDOKU_BAD_FORMAT : file contents in invalid format:
           - fewer than 9 digits/dashes on a line
           - fewer than 9 lines in the file
	   - unknown character
     SUDOKU_UNKNOWN_ERROR : any other error you think of
 */
int Sudoku_loadfile (const char *filename);

/*
  Sudoku_savefile: Save the current game to filename. 

  The structure of the file must be compatible with Sudoku_loadfile.

  Return the following error codes:
     SUDOKU_SUCCESS : file written successfully
     SUDOKU_NO_FILE_ERROR : cannot create file
     SUDOKU_UNKNOWN_ERROR : any other error
 */

int Sudoku_savefile (const char *filename);

/*
  Sudoku_set_square: set the square at position (col, row) to value
  digit. 

  Return values:
     SUDOKU_SUCCESS : square set successfully
     SUDOKU_CONTRADICTION : square is update, but it leads to a contradiction.
        - You do not need to solve the puzzle, just look at current values
	  and see if a contradiction already exists.
     SUDOKU_ILLEGAL_VALUE : digit is not legal
     SUDOKU_ILLEGAL_INDEX : col and row do not forma legal index
 */
int Sudoku_set_square (char row, int col, char digit);

/*
  Sudoku_get_square: return the square at position (col, row) by reference in *value.

  Return values:
     SUDOKU_SUCCESS : value is returned successfully
     SUDOKU_ILLEGAL_INDEX : col and row do not forma legal index
 */
int Sudoku_get_square (char row, int col, char *value);

/*
  Sudoku_give_hint: return a hint by value.  The col, row, and value are returned
     by reference in *col, *row, and *value.

  Return value:
     SUDOKU_SUCCESS : hint supplied
     SUDOKU_NO_HINT : no hint because solver could not find one.
*/
int Sudoku_give_hint (char *row, int *col, char *value);

/*
  Print the Sudoku game to the screen.  Already implemented. Do not modify.
 */
void Sudoku_print (void);

#endif /* SUDOKU_H */
