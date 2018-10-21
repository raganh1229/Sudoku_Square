#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sudoku.h"

#define DEBUG(x) printf("%d: %s\n", __LINE__, x)

static int errors=0;
static int tests=0;

void test_set_square()
{
  int ret;

  tests += 3;

  ret = Sudoku_set_square('X',1,'1');
  if (ret != SUDOKU_ILLEGAL_INDEX)
    {
      errors += 1;
      DEBUG("Sudoku_set_square() didn't return SUDOKU_ILLEGAL_INDEX");
    }

  ret = Sudoku_set_square('A',0,'1');
  if (ret != SUDOKU_ILLEGAL_INDEX)
    {
      errors += 1;
      DEBUG("Sudoku_set_square() didn't return SUDOKU_ILLEGAL_INDEX");
    }

  ret = Sudoku_set_square('A',1,'Q');
  if (ret != SUDOKU_ILLEGAL_VALUE)
    {
      errors += 1;
      DEBUG("Sudoku_set_square() didn't return SUDOKU_ILLEGAL_VALUE");
    }

  ret = Sudoku_set_square('A',1,'1');
  if (ret != SUDOKU_SUCCESS)
    {
      errors += 1;
      DEBUG("Sudoku_set_square() didn't return SUDOKU_SUCCESS");
    }
}

void test_get_square()
{
  int ret;
  char val;

  tests += 4;

  ret = Sudoku_get_square('1',1,&val);
  if (ret != SUDOKU_ILLEGAL_INDEX)
    {
      errors += 1;
      DEBUG("Sudoku_get_square() didn't return SUDOKU_ILLEGAL_INDEX");
    }

  ret = Sudoku_get_square('A',10,&val);
  if (ret != SUDOKU_ILLEGAL_INDEX)
    {
      errors += 1;
      DEBUG("Sudoku_get_square() didn't return SUDOKU_ILLEGAL_INDEX");
    }

  char row;
  int col;
  for(row='A'; row < 'A'+9; row++)
    for(col=1; col <= 9; col++)
      {
	val = 0;
	ret = Sudoku_set_square(row,col,'5');
	if (ret == SUDOKU_SUCCESS)
	  {
	    ret = Sudoku_get_square(row,col,&val);
	    if (ret != SUDOKU_SUCCESS)
	      {
		errors += 2;
		DEBUG("Sudoku_get_square() didn't return SUDOKU_SUCCESS.");
		return;
	      }
	    else
	      {
		if (val != '5')
		  {
		    errors++;
		    DEBUG("Sudoku_get_square() didn't return correct value.");
		    return;
		  }
	      }	
	  }
	else
	  {
	    errors += 2;
	    printf("Ret = %d\n", ret);
	    DEBUG("Sudoku_set_square() didn't return SUDOKU_SUCCESS");
	    return;
	  }
	// reset the square to blank to avoid contradictions
	ret = Sudoku_set_square(row,col,'-');
      }
}

void test_load_file()
{
  int ret;

  tests += 4;

  ret = Sudoku_loadfile("made-up-name.sko");
  if (ret != SUDOKU_NO_FILE_ERROR)
    {
      errors += 1;
      DEBUG("Sudoku_loadfile() didn't return SUDOKU_NO_FILE_ERROR when it should.");
    }

  ret = Sudoku_loadfile("bad.sko");
  if (ret != SUDOKU_BAD_FORMAT)
    {
      errors += 1;
      DEBUG("Sudoku_loadfile() didn't return SUDOKU_BAD_FORMAT when it should.");
    }

  ret = Sudoku_loadfile("bad2.sko");
  if (ret != SUDOKU_BAD_FORMAT)
    {
      errors += 1;
      DEBUG("Sudoku_loadfile() didn't return SUDOKU_BAD_FORMAT when it should.");
    }

  ret = Sudoku_loadfile("easy.sko");
  if (ret != SUDOKU_SUCCESS)
    {
      errors += 1;
      DEBUG("Sudoku_loadfile() didn't return SUDOKU_SUCCESS when it should.");
    }
  else
    {
      printf("Here's what your board looks like after loading easy.sko:\n");
      Sudoku_print();
    }
}

void test_save_file()
{
  int ret;

  tests += 4;

  Sudoku_initialize();
  
  int col;
  for(col=1; col <= 9; col++)
    Sudoku_set_square('A',col,col+'0');

  ret = Sudoku_savefile("snapshot.sko");
  if (ret != SUDOKU_SUCCESS)
    {
      errors += 1;
      DEBUG("Sudoku_savefile() didn't return SUDOKU_SUCCESS when it should.");
    }

  Sudoku_initialize();
  char val;
  Sudoku_get_square('A',1,&val);
  if (val!='-')
    {
      errors += 3;
      DEBUG("Sudoku_initialize() didn't reset the game board.");
      return;
    }

  ret = Sudoku_loadfile("snapshot.sko");
  if (ret != SUDOKU_SUCCESS)
    {
      errors += 1;
      DEBUG("Sudoku_loadfile() didn't return SUDOKU_SUCCESS when it should.");
    }
  
  for(col=1; col <= 9; col++)
    {
      Sudoku_get_square('A',col,&val);
      if (val != col+'0')
	{
	  errors += 1;
	  DEBUG("Sudoku_loadfile() didn't get the values it should have from the file.");
	  return;
	}
    }

}

void test_for_contradiction()
{
  int ret;

  tests += 3;

  ret = Sudoku_set_square('A',1,'1');

  // Test same square
  ret = Sudoku_set_square('B',2,'1'); 
  if (ret != SUDOKU_CONTRADICTION)
    {
      errors += 1;
      DEBUG("Sudoku_set_square() didn't return SUDOKU_CONTRADICTION when it should.");
    }

  // Test same column
  ret = Sudoku_set_square('E',1,'1'); 
  if (ret != SUDOKU_CONTRADICTION)
    {
      errors += 1;
      DEBUG("Sudoku_set_square() didn't return SUDOKU_CONTRADICTION when it should.");
    }

  // Test same row
  ret = Sudoku_set_square('A',9,'1'); 
  if (ret != SUDOKU_CONTRADICTION)
    {
      errors += 1;
      DEBUG("Sudoku_set_square() didn't return SUDOKU_CONTRADICTION when it should.");
    }
}

void test_for_hint()
{
  tests += 1;

  Sudoku_initialize();

  if ( Sudoku_loadfile("simple.sko") != SUDOKU_SUCCESS )
    {
      printf("You appear to be missing simple.sko. Please download it, copy to this directory, and re-run.\n");
      errors += 1;
      return;
    }
  
  char row;
  int col;
  char val;


  if (Sudoku_give_hint(&row, &col, &val) != SUDOKU_SUCCESS)
    {
      errors+=1;
      DEBUG("Sudoku_give_hint() didn't return SUDOKU_SUCCESS when it should.");
    }
  else
    {
      if ( !( (row=='A' && col==9) || (row=='I' && col==1)) )
	{
	  errors += 1;
	  DEBUG("Sudoku_give_hint() didn't return an appriopriate hint.");
	}
    }

 Sudoku_initialize();
 Sudoku_loadfile("easy.sko");
 while (Sudoku_give_hint(&row, &col, &val) == SUDOKU_SUCCESS) 
   {
     if (Sudoku_set_square(row,col,val) != SUDOKU_SUCCESS)
       break;
   }
 printf("Your hints solved this much of easy.sko:\n");
 Sudoku_print(); 
}


int main(int argc, char *argv[])
{
  
  int ret=0;
  ret = Sudoku_initialize();
  tests += 1;
  if (ret != SUDOKU_SUCCESS)
    {
      errors += 1;
      DEBUG("Sudoku_initialize() didn't return SUDOKU_SUCCESS");
    }
  
  
  test_set_square();
  test_get_square();
  test_load_file();

  test_save_file();
  test_for_contradiction();
  test_for_hint();


  printf("%d of %d passed. (%2.1lf%%) \n", tests-errors, tests, (tests-errors)/(double)tests * 100);
  return 0;
}

  
