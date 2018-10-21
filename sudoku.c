/* John Ragan Harrison - 4/27/2018 - Program used to incorporate a set of sudoku functions that sets a game board,
checks whether or not a sudoku input is valid, allows the user to load and save previous game boards, 
print values to the board, and give hints to the user. 

Warning NOTE: warning: conflicting types for built-in function 'index' static int index(char row, int col)
  This warning is being issued on Windows because of the conflicting name of the provided function 'index' with
  a built in function from the C library and its identifier as a RESERVED identifier.

  Please use -fno -builtin when compiling with GCC to avoid this error.
*/


#include "sudoku.h" //Including the functions located in sudoku.h.
#include "stdlib.h" //Indlucing the stdlib.h livrary.
#include "stdio.h" //Including the stdio.h library.
#include "sudoku.h" //including the sudoku.h library.
#include <assert.h> //including the assert.h library.



struct sudoku_square //Sudoku square struct. Contains the attributes of each individual square on the sudoku gmae board.
{
  char value; //Value that is currently present in each square. This variable is useful when saving the values in the game board.
  int original_value; //Value used when implementing the hint implementation and when reverting the box back to its original value if the input needs to be changed.
  int possible[10]; //Array used when providing a hint to the player.
};


struct sudoku //Larger struct, sudoku, defined as an array of 81 sudoku_square structs.
{
  struct sudoku_square grid[81]; //Array of 81 sudoku_square structs. 
};


struct sudoku game; //Renaming sudoku to game to be referenced later on in the program.


static int index(char row, int col) //Index function used to find the sudoku square associated with the corresponding row and column.
{
  return (row-'A')*9 + (col-1); //Returns the corresponding square.
}


int Sudoku_initialize (void) //Sudoku initialize function. Resets the game board and all global variables.
{

int i=80; //Used as a counter to determine whether or not the program has iterated through the 81 squares and cleared them.

  for(i=80; i>=0; i--) //For loop. Iterates 81 times for 81 squares.
  {
    game.grid[i].value = '-'; //Sets each square to '-'.
  }

  for(i=80; i>=0; i--) //Checks to see whether or not each square has been set to '-'. If it hasn't it will return SUDOKU_UNKNOWN_ERROR.
  {
    if (game.grid[i].value != '-')
    {
      return SUDOKU_UNKNOWN_ERROR;
    }
  }
return SUDOKU_SUCCESS; //If each square was set to '-', SUDOKU_SUCCESS is returned.
}



int Sudoku_loadfile(const char *filename) //Loads the gameboard from a file.
{
  FILE *file = fopen(filename,"r"); //Opens the file for reading ONLY.
  if (file==NULL)
    {
      return SUDOKU_NO_FILE_ERROR; //If the file does not open or there is no file at all, SUDOKU_NO_FILE_ERROR is returned.
    }

int row; //row variable. Used as a counter for the rows in the for loops below.
int col; //column variable. Also used as a counter for the rows in the for loops below.
int iterate=0;

  for(row=0; row<9; row++) //Iterates through the 81 squares.
  {
    for(col=1; col<=9; col++)
    {
      char val; //Contains each indvidual value loaded from the file.
      if(fscanf(file,"%c", &val)!=EOF) //If the file reaches the EOF, the loops break.
      {   
        if((val == '1') || (val == '2') || (val == '3') || (val == '4') || (val == '5') || (val == '6') || (val == '7') || (val == '8') || (val == '9') || (val == '-'))
        {
          Sudoku_set_square((char)(row+'A'),col,val); //If the value is a legal value, sudoku_set_square is referenced to set the individual value.
          ++iterate; //Incraments iterate until it reaches 81.
        } 
        else //If there is NOT a legal value inputted, function returns SUDOKU_BAD_FORMAT.
        {
          return SUDOKU_BAD_FORMAT;
        }
      }
    }
  fscanf(file,"%*c"); //Inputs a newline after 9 values are scanned in / read.
  }  
  
  fclose(file); //Closes the file after reading in all of the values.

  if(iterate != 81) //If iterate does not equal 81, SUDOKU_BAD_FORMAT is returned.
  {
    return SUDOKU_BAD_FORMAT;
  }

  return SUDOKU_SUCCESS; //Else, SUDOKU_SUCCESS is returned;
}




int Sudoku_savefile(const char *filename) //Function used to save the file by writing the values stored in the squares to the written file.
{
 
FILE *file = fopen(filename,"w"); //Standard format for writing a file using "w".
  
if (file==NULL)
  {
    return SUDOKU_NO_FILE_ERROR; //If there is no file or a file is not found, SUDOKU_NO_FILE_ERROR is returned.
  }

int row; //row variable. Used as a counter for the rows in the for loops below.
int col; //column variable. Also used as a counter for the rows in the for loops below.

  for(row=0; row<9; row++) //Iterates through the 81 squares.
  {
    for(col=1; col<=9; col++)
    {
    char val; //Declares a variable, 'val' to store the individual value that is extracted from the current file then to be saved into the saving file.
    Sudoku_get_square((char)(row+'A'),col,&val);
      if((val == '1') || (val == '2') || (val == '3') || (val == '4') || (val == '5') || (val == '6') || (val == '7') || (val == '8') || (val == '9') || (val == '-'))  
      {   
        fprintf(file, "%c", val); //Print the individual value to the saving file.
      }
      else
      {
        return SUDOKU_UNKNOWN_ERROR; //If the value cannot be written to the file, SUDOKU_UNKNOWN_ERROR would be returned.
      }
    }
    fprintf(file, "\n"); //Print a newline to the file after 9 values have been inputted.
  } 

fclose(file); //Closes the file after the file values have been printed. After the file has been "saved".
return SUDOKU_SUCCESS; //SUDOKU_SUCCESS is returned if the file is written successfully.
}



int Sudoku_set_square(char row, int col, char digit) //Function used to set each square in the sudoku game board. Also contains checks to see whether or not there is a row, column, or 3x3 contradiction.
{
  
  if((col > 9) || (col < 1)) //Checks to see whether or not the column input is legal.
  {
    return SUDOKU_ILLEGAL_INDEX; //If it is not, return SUDOKU_ILLEGAL_INDEX.
  }

  if((row > 'I') || (row < 'A')) //Checks to see whether or not the row input is legal.
  {
    return SUDOKU_ILLEGAL_INDEX; //If it is not, return SUDOKU_ILLEGAL_INDEX.
  }

  if((digit == '1') || (digit == '2') || (digit == '3') || (digit == '4') || (digit == '5') || (digit == '6') || (digit == '7') || (digit == '8') || (digit == '9') || (digit == '-'))
  { //It is then checked whether or not the value inputted for digit is legal.
    
    game.grid[index(row, col)].value = digit; //Sets the value at the specific square in the sudoku board indicated by the row and the column to the "digit" value.
    
    int i; //Initializes a counter for the for loop, i.
    
    for(i=0; i<9; i++) //Loop used to check whether or not there is a contradiction in the column with the inputted digit.
    {
      if((game.grid[index(row, col)].value != game.grid[(index(('A'+i), col))].value) || (index(row, col) == index(('A'+i), col)))
      {
        continue; //If there is not a contradiction, the loop will continue comparing other values in the column.
      }
      else
      {
        return SUDOKU_CONTRADICTION; //If there is a contradiction, SUDOKU_CONTRADICTION will be returned.
      }   
    }
    

    for(i=0; i<9; i++) //Loop used to check whether or not there is a contradiction in the row with the inputted digit.
    {
      if((game.grid[index(row, col)].value != game.grid[(index(row, (1+i)))].value) || (index(row, col) == (index(row, (1+i)))))
      {
        continue; //If there is not a contradiction, the loop will continue comparing other values in the row.
      }
      else
      {
        return SUDOKU_CONTRADICTION; //If there is a contradiction, SUDOKU_CONTRADICTION will be returned.
      }   
    }

    char new_row; //Used in the 3x3 check. Contains the 'pivot' of each 3x3 square. The pivot is the corresponding (row, col) value of the top left location of each 3x3 square. This value contains the row value of the pivot.
    int new_col; //This value contains the column value of the pivot.
    int x; //Used as a counter for the first for loop.
    int y; //Used as a counter for the second for loop.

     
     if(((row) == 'A') || ((row) == 'D') || ((row) == 'G')) //These if statements are used to find the pivots. 
    {                                                         //These pivots are located at the row values 'A', 'D', 'G' and column 1,4,7. These if statements find the respective row and column of each pivot for each digit inputted.
      new_row = (row);
    }
      if(((row) == 'B') || ((row) == 'E') || ((row) == 'H'))
    {
      new_row = (row-1);
    }
      if(((row) == 'C') || ((row) == 'F') || ((row) == 'I'))
    {
      new_row = (row-2);
    }
      if(((col) == 1) || ((col) == 4) || ((col) == 7))
    {
      new_col = (col);
    }
      if(((col) == 2) || ((col) == 5) || ((col) == 8))
    {
      new_col = (col-1);
    }
      if(((col) == 3) || ((col) == 6) || ((col) == 9))
    {
      new_col = (col-2);
    }

      for (x=0; x<3; x++) //For loops for iterating through the 3x3 square from the pivot.
    {
      for (y=0; y<3; y++)
      {
        if((game.grid[index(row, col)].value != game.grid[index((new_row+x), (new_col+y))].value) || (index(row, col) == index((new_row+x), (new_col+y))))
        { 
          continue; //If there is not a contradiction, the loop will continue comparing other values in the 3x3 square.
        }
        else
        {
          return SUDOKU_CONTRADICTION; //If there is a contradiction, SUDOKU_CONTRADICTION will be returned.
        }
      }
    }

    return SUDOKU_SUCCESS; //After all values in the 3x3 square are compared with the digit AND found to be legal, SUDOKU_SUCCESS is returned.
  } 
  
  else //If the input is NOT legal, SUDOKU_ILLEGAL_VALUE is returned.
  {  
    return SUDOKU_ILLEGAL_VALUE;
  }
}


int Sudoku_get_square (char row, int col, char *value) //Function used to extract the value of each indvidual square given the row and column and store it in a pointer pointing to value.
{

  if((col > 9) || (col < 1)) //Checking to see if the row inputs are valid.
  {
    return SUDOKU_ILLEGAL_INDEX; //If not, return SUDOKU_ILLEGAL_INDEX.
  }

  if((row > 'I') || (row < 'A')) //Checking to see if the column inputs are valid.
  {
    return SUDOKU_ILLEGAL_INDEX; //If not, return SUDOKU_ILLEGAL_INDEX.
  }
  
  else //If the value inputted IS legal, a pointer to value stores the digit corresponding to the row and column.
  {
  *value = (game.grid[index(row, col)].value);
  return SUDOKU_SUCCESS; //SUDOKU_SUCCESS is returned.
  }
}


int Sudoku_give_hint (char *r, int *c, char *value) //DID NOT IMPLEMENT SUDOKU_GIVE_HINT. Would rather study for the final.
{
  return SUDOKU_NO_HINT; //That being said, this function calculates the possibilities that can be inputted in a particular square and returns these possibilities to the user if they request it.
}


void Sudoku_print(void) //Function that prints the sudoku board and the values read in by the file.
{
  
  char row = 'A'; //The initial condition for row in the for loop below.  
  int col; //The initial condition for column in the for loop below. 
  char value;

  printf("   | 1  2  3 | 4  5  6 | 7  8  9 |\n"); //Prints the column header.
  for(row='A'; row <= 'I'; row++)
    {
      if ((row-'A') % 3 == 0)
      printf("   |---------+---------+---------|\n"); //Prints the initial dividor between the column numbers and the square values.

      printf(" %c |",row); //Prints the row value on the side as a reference for the game board.
      col = 1;

      for(; col<=3; col++)
	{
	  Sudoku_get_square(row,col,&value); //Gets the value in the square by the row and column. Prints this value to the board.
	  printf(" %c ",value);              //This process is done three times and repeated three times by the for loop.
	}
      printf("|");                     //Prints a "|" to divide the values.
      for(; col<=6; col++)
	{
	  Sudoku_get_square(row,col,&value);
	  printf(" %c ",value);
	}
      printf("|");
      for(; col<=9; col++)
	{
	  Sudoku_get_square(row,col,&value);
	  printf(" %c ",value);
	}
      printf("|\n");
    }
    printf("   |---------+---------+---------|\n"); //Prints the end of the game board.
}