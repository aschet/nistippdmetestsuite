#include <afx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cmdStringParse.h"

/* Pass in the delimiter and find the word before the delimiter */
void find_delimiter(char delimiter, char *buffer, char *word, char *out_buffer)
{
          int exit = 0;
          int exit2 = 0;
		  int end_line = 0;
          char *ptr1, *ptr2, *ptr3;

          ptr1 = buffer;
          ptr2 = word;
          ptr3 = out_buffer;

          word[0] = buffer[0];
                
          while(exit == 0)
          {
              *ptr2 = *ptr1;
              
              if ((*ptr1 == delimiter) || (*ptr1 == '\0') || (*ptr1 == '\n'))
			  {
				  // Remove the delimiter from the word
				  *ptr2 = '\0';
				  exit = -1;
			  }
              if ((*ptr1 == '\0') || (*ptr1 == '\n'))
              {
                  end_line = 1;
              }
              
              else
              {
                  end_line = 0;
              }
              

              ptr2++;
              ptr1++;
          }


          //ptr1++;  /* Increment by 2 to get rid of the previous delimiter and white space */
          
          while (exit2 == 0)
          {
              *ptr3 = *ptr1;
              ptr3++;
              ptr1++;

              if ((*ptr1 == '\0') || (*ptr1 == '\n'))
                  exit2 = -1;
          }
}

// Parsing real numbers of the the string
double readRealNum(char *line)
{
  
int kk = 0;
  char *ptr1;
  char buf[100] = "";
  char num[10] = "";
  ptr1 = line;

  for (;;) 
  {
      
	  if ((*ptr1 == ')') || (*ptr1 ==',') )
		{
	  // End of number information
	  break;
	}

	  // Integer value
	 else if (((*ptr1 >= '0') && (*ptr1 <= '9')) || (*ptr1 == '.') || (*ptr1 == '-'))
	{
	  num[kk] = *ptr1;
	  kk++;
      ptr1++;
	}

      else
	{
	  // Spurious character, therefore just skip over it 

	  ptr1++;
	  
	}

  }
  
  return atof(num);

}



	
// Parsing integers of the string
int readWholeNum(char *line)
{
  int kk = 0;
  char *ptr1;
  char buf[100] = "";
  char num[10] = "";
  ptr1 = line;

  for (;;) 
  {
      
	  if ((*ptr1 == ')') || (*ptr1 ==',') )
		{
	  // End of number information
	  break;
	}

	  // Integer value
	 else if (((*ptr1 >= '0') && (*ptr1 <= '9')) || (*ptr1 == '-'))
	{
	  num[kk] = *ptr1;
	  kk++;
      ptr1++;
	}

      else
	{
	  // Spurious character, therefore just skip over it
	  ptr1++;
	  
	}

  }
  return atoi(num);

}

void writeLogFile(char *buffer, fstream Logfile)
{

	char timestr[20] = "";
	time_t t;
	
	// Add a timestamp for the logfile
	t = time(NULL);
	strftime(timestr, sizeof(timestr), "%y:%m:%d_%H:%M:%S ", localtime(&t));
		
	// Write buffer to logfile
	Logfile.write(timestr, strlen(timestr));
	Logfile.write(buffer, strlen(buffer)).put('\n');
}



bool check_message(char *buffer, char *word, char *out_buffer)
{
		  int end_line = 0;
		  int x = 0;
          int exit = 0;
          int exit2 = 0;
          char *ptr1, *ptr2, *ptr3;
		  bool complete_message = false;

          ptr1 = buffer;
          ptr2 = word;
          ptr3 = out_buffer;

          word[0] = buffer[0];      

          while(exit == 0)
          {
              *ptr2 = *ptr1;

			 ptr2++;
             ptr1++;
			 x++;

              if (*ptr1 == '\0')
			  {
				  *ptr2 = '\0';
				  end_line = 1;
				  exit = -1;
				  exit2 = -1;
			  }
			  if ((*ptr1 == '\n') && (buffer[x-1] == '\r'))
              {
                  complete_message = true;
				  exit = -1;
              }
              
              else
              {
                  end_line = 0;
              }
              
          }


          ptr1++;  /* Increment by 2 to get rid of the previous delimiter and white space */
          
          while (exit2 == 0)
          {
			  if (*ptr1 == '\0')
                  
			  {		
				  *ptr3 = '\0';
				  exit2 = -1;
			  }

			  else
			  {
				*ptr3 = *ptr1;
				ptr3++;
				ptr1++;
			  }

          }

		  if (complete_message)
			  return true;
		  else
			  return false;
}


