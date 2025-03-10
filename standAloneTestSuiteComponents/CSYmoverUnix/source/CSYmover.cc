/*
   File : CSYmover.cc

This file contains the source code for a text-based utility that
reads an I++ command file, and writes an equivalent file with
the part coordinate system translated and/or rotated.

Input command files written by NIST (.prg suffix) that are intended to
be executable assume that when the CMM is viewed from the front, the
X-axis points to the right, the Z-axis points up, and the coordinates
of the point at the middle of the table are (0, 0, 0). Command files
written by NIST are accompanied by text files with the same name but a
.txt suffix. If the <name>.prg file is intended to be executable, near
the beginning of the <name>.txt file, there is the sentence,
"<name>.prg is intended to be executable.".

The work volume assumed by the executable NIST .prg files extends
from -300.0 to +300.0 in X and Y and 0.0 to 300.0 in Z. The coordinate
values in these files are given in machine coordinates.

To use one of these .prg files on a machine whose coordinate system
differs from the one just described, the user tells this utility about
the machine coordinate system of the target machine.  The utility then
prints an output file that is identical to the input file, except that
immediately after the StartSession command two lines are inserted that
define the Part coordinate system and select that system. The output
file may be executed on the target machine -- as long as the height of
its work volume is at least 300 mm, and the width and length of its
work volume are at least 600 mm.

This utility assumes one of the axes of the machine coordinate system
is perpendicular to the table and one of the other two axes points to
the right or the left (when the machine is viewed from the front). If
that is not the case for a machine, this utility will not work for
that machine.

*/

#include <stdio.h>    /* gets, etc. */
#include <stdlib.h>   /* exit       */
#include <string.h>   /* strcpy     */

#define IPPSIZE  65536

typedef enum axisDirection { UP, DOWN, RIGHT, LEFT, FRONT, BACK };

/*********************************************************************/

/* getUserInput

This gives the user seven choices:

enter a number:
1 = write output file
2 = change input file (currently <file name>)
3 = change output file (currently <file name>)
4 = change axis pointing right (currently <axis name>)
5 = change axis pointing up (currently <axis name>)
6 = change coordinates of middle of table (currently (X, Y, Z))
7 = quit
enter choice =>

The choices are given repeatedly until the user chooses 1 or 7.

If the user enters 1:
  a. If the use (i) has not specifed the name of an input file, or
     (ii) has not specifed the name of an output file, or
     (iii) has given the same name for both files,
     the user is asked to fix the problem by changing a file name.
  b. If the axis pointing right is parallel to the axis pointing up,
     the user is asked to change one of the two.
  c. Otherwise, the output file is written.

If the user enters 2, the user is prompted to enter an input file name.

If the user enters 3, the user is prompted to enter an output file name.

If the user enters 4, the user is prompted to enter the name of the
   axis that points right. The axis name must be one of +X, -X, +Y, -Y,
   +Z, or -Z. If some other name is given, the user is prompted to
   enter the axis name again.

If the user enters 5, the user is prompted to enter the name of the
   axis that points up. The axis name must be one of +X, -X, +Y, -Y,
   +Z, or -Z. If some other name is given, the user is prompted to
   enter the axis name again.

If the user enters 6, the user is prompted to enter the coordinates of
   the middle point in the format (x, y, z). If that format is
   not used, the user is prompted to enter the coordinates again.

If the user enters 7, the system exits without writing an output file.

*/

void getUserInput(
 char * inputFileName,
 char * outputFileName,
 char * pointsRight,
 char * pointsUp,
 double * middleX,
 double * middleY,
 double * middleZ,
 int * quit)
{
  char buffer[80];
  char axisName[80];
  int choice;

  for(; ;)
    {
      printf("enter a number:\n");
      printf("1. write output file\n");
      printf("2. change input file name (currently %s)\n",
	     inputFileName);
      printf("3. change output file name: (currently %s)\n",
	     outputFileName);
      printf("4. change axis pointing right (currently %s)\n",
	     pointsRight);
      printf("5. change axis pointing up (currently %s)\n",
	     pointsUp);
      printf("6. change coordinates of middle of table (currently"
	     "(%.3lf, %.3lf, %.3lf))\n", *middleX, *middleY, *middleZ);
      printf("7. quit\n");
      printf("enter choice => ");
      fflush(stdout);
      fgets(buffer, 80, stdin);
      if (sscanf(buffer, "%d", &choice) != 1)
        continue;
      if (choice == 1)
	{
	  if (strcmp(inputFileName, "no input file") == 0)
	    {
	      fprintf(stderr, "must give input file name\n");
	      continue;
	    }
	  else if (strcmp(outputFileName, "no output file") == 0)
	    {
	      fprintf(stderr, "must give output file name\n");
	      continue;
	    }
	  else if (strcmp(inputFileName, outputFileName) == 0)
	    {
	      fprintf(stderr,
		      "input file name must differ from output file name\n");
	      continue;
	    }
	  else if (pointsRight[1] == pointsUp[1])
	    {
	      fprintf(stderr, "axes must be perpendicular to one another\n");
	      continue;
	    }
	  else
	    break;
	}
      else if (choice == 2)
        {
	  printf("enter input file name => ");
	  fflush(stdout);
	  fgets(inputFileName, 80, stdin); /* now zap newline */
	  inputFileName[strlen(inputFileName) - 1] = 0;
        }
      else if (choice == 3)
	{
	  printf("enter output file name => ");
	  fflush(stdout);
	  fgets(outputFileName, 80, stdin); /* now zap newline */
	  outputFileName[strlen(outputFileName) - 1] = 0;
	}
      else if (choice == 4)
	{
	  for (; ;)
	    {
	      printf("enter name of axis pointing right => ");
	      fflush(stdout);
	      fgets(buffer, 80, stdin);
	      if (sscanf(buffer, "%s", axisName) != 1)
		{
		  printf("could not read axis name\n");
		  continue;
		}
	      else if (((axisName[0] != '+') && (axisName[0] != '-')) ||
		       ((axisName[1] != 'X') &&
			(axisName[1] != 'Y') &&
			(axisName[1] != 'Z')) ||
		       (axisName[2] != 0))
		{
		  printf("axis name must be +X, -X, +Y, -Y, +Z, or -Z\n");
		  continue;
		}
	      else
		{
		  strcpy(pointsRight, axisName);
		  break;
		}
	    }
	}
      else if (choice == 5)
	{
	  for (; ;)
	    {
	      printf("enter name of axis pointing up => ");
	      fflush(stdout);
	      fgets(buffer, 80, stdin);
	      if (sscanf(buffer, "%s", axisName) != 1)
		{
		  printf("could not read axis name\n");
		  continue;
		}
	      else if (((axisName[0] != '+') && (axisName[0] != '-')) ||
		       ((axisName[1] != 'X') &&
			(axisName[1] != 'Y') &&
			(axisName[1] != 'Z')) ||
		       (axisName[2] != 0))
		{
		  printf("axis name must be +X, -X, +Y, -Y, +Z, or -Z\n");
		  continue;
		}
	      else
		{
		  strcpy(pointsUp, axisName);
		  break;
		}
	    }
	}
      else if (choice == 6)
	{
	  for(; ;)
	    {
	      printf("enter coordinates of middle of table => ");
	      fflush(stdout);
	      fgets(buffer, 80, stdin);
	      if (sscanf(buffer, " ( %lf , %lf , %lf )",
			 middleX, middleY, middleZ) == 3)
		break;
	      else
		printf("use numbers in parentheses format: (X, Y, Z)\n");
	    } 
	}        
      else if (choice == 7)
	{
	  *quit = 1;
	  break;
	}
    }
}

/***********************************************************************/

void setAngles(
 char * pointsRight,
 char * pointsUp,
 int * theta,
 int * psi,
 int * phi)
{
  if (strcmp(pointsUp, "+Z") == 0)
    {
      *psi = 0;
      *theta = 0;
      *phi = ((strcmp(pointsRight, "+X") == 0) ?   0 :
	      (strcmp(pointsRight, "-Y") == 0) ?  90 :
	      (strcmp(pointsRight, "-X") == 0) ? 180 : 270);
    }
  else if (strcmp(pointsUp, "-Z") == 0)
    {
      *psi = 0;
      *theta = 180;
      *phi = ((strcmp(pointsRight, "+X") == 0) ?   0 :
	      (strcmp(pointsRight, "-Y") == 0) ?  90 :
	      (strcmp(pointsRight, "-X") == 0) ? 180 : 270);
    }
  else if (strcmp(pointsUp, "+Y") == 0)
    {
      *psi = 0;
      *theta = 90;
      *phi = ((strcmp(pointsRight, "+X") == 0) ?   0 :
	      (strcmp(pointsRight, "+Z") == 0) ?  90 :
	      (strcmp(pointsRight, "-X") == 0) ? 180 : 270);
    }
  else if (strcmp(pointsUp, "-Y") == 0)
    {
      *theta = 90;
      *phi = 180;
      *psi = ((strcmp(pointsRight, "-X") == 0) ?   0 :
	      (strcmp(pointsRight, "+Z") == 0) ?  90 :
	      (strcmp(pointsRight, "+X") == 0) ? 180 : 270);
    }
  else if (strcmp(pointsUp, "+X") == 0)
    {
      *theta = 90;
      *phi = 90;
      *psi = ((strcmp(pointsRight, "-Y") == 0) ?   0 :
	      (strcmp(pointsRight, "+Z") == 0) ?  90 :
	      (strcmp(pointsRight, "+Y") == 0) ? 180 : 270);
    }
  else if (strcmp(pointsUp, "-X") == 0)
    {
      *theta = 90;
      *phi = 270;
      *psi = ((strcmp(pointsRight, "+Y") == 0) ?   0 :
	      (strcmp(pointsRight, "+Z") == 0) ?  90 :
	      (strcmp(pointsRight, "-Y") == 0) ? 180 : 270);
    }
  else
    fprintf(stderr, "Bug in CSYmover\n");
}

/***********************************************************************/

/* printFile

Returned Value: int (0 or 1)
   If any of the following errors occur, this returns 1.
   Otherwise, it returns 0.
   1. 

Side Effects:
   

Called By:
   main

The values of state are:
 0 = have not read entry
 1 = have read first entry (2 lines) and checked it
 2 = have printed part coordinate commands

The character reading loop reads characters from an I++ command file
until one of the following conditions occurs:
1. There is a line containing only two backslashes and a line terminator
  (ASCII 13 followed by ASCII 10). If there are no errors in the
  command file, every second line is of this sort until the end of
  the file, so normally the loop reads two lines at a time. The
  rest of the function acts on these two lines and then goes back
  into the character reading loop.
2. There is a line containing only a colon and a line terminator.
  This is normally the next-to-last line of a command file.
  The function prints the end of the outFile when this happens and
  then exits.

*/

void printFile(         /* ARGUMENTS */
 char * pointsRight,
 char * pointsUp,
 double originX,
 double originY,
 double originZ,
 FILE * inFile,
 FILE * outFile)
{
  char buffer[IPPSIZE];
  char c;
  int k;
  int j;
  int state;
  int theta;
  int psi;
  int phi;

  setAngles(pointsRight, pointsUp, &theta, &psi, &phi);
  state = 0;
  for (c = 32; c != EOF; )
    {
      for (k=0; (((c = fgetc(inFile)) != EOF) && (k < IPPSIZE)); k++)
	{
	  buffer[k] = (char)c;
	  if ((k > 1) && (c == 10) && (buffer[k-1] == 13))
	    {
	      if ((k == 2) && (buffer[0] == ':')) 
		break;
	      else if ((k > 2) && (buffer[k-2] == 92) &&
		       (buffer[k-3] == 92))
		break;
	    }
	}
      if (k == IPPSIZE)
	{
	  fprintf(stderr, "line too long\n");
	  break;
	}
      else if (c == EOF) // should exit before EOF
	{
	  fprintf(stderr, "error in last line of file\n");
	  break;
	}
      else if ((k ==2) && (buffer[0] == ':'))
	{
	  fprintf(outFile, "%c%c%c", ':', 13, 10);
	  fprintf(outFile, "%c%c%c", ':', 13, 10);
	  break; // stop if line starting w. colon
	}
      else if (state == 0)
	{
	  if (strncmp((buffer + 6), "StartSession", 12) != 0)
	    {
	      fprintf(stderr, "first line of file must be StartSession\n");
	      break;
	    }
	  else
	    {
	      for (j = 0; j <= k; j++)
		fputc(buffer[j], outFile);
	      state = 1;
	    }
	}
      else if (state == 1)
	{
	  fprintf(outFile, "09997 SetCsyTransformation(PartCsy, "
		  "%.3lf, %.3lf, %.3lf, %d, %d, %d)%c%c",
		  originX, originY, originZ, theta, psi, phi, 13, 10);
	  fprintf(outFile, "%c%c%c%c", 92, 92, 13, 10);
	  fprintf(outFile, "09998 SetCoordSystem(PartCsy)%c%c", 13, 10);
	  fprintf(outFile, "%c%c%c%c", 92, 92, 13, 10);
	  if (strncmp((buffer + 6), "SetCsyTransformation", 20) == 0)
	    fprintf(stderr, "WARNING: file contains SetCsyTransformation\n");
	  else if(strncmp((buffer + 6), "SetCoordSystem", 14) == 0)
	    fprintf(stderr, "WARNING: file contains SetCoordSystem\n");
	  for (j = 0; j <= k; j++)
	    fputc(buffer[j], outFile);
	  state = 2;
	}
      else if (state == 2)
	{
	  if (strncmp((buffer + 6), "SetCsyTransformation", 20) == 0)
	    fprintf(stderr, "WARNING: file contains SetCsyTransformation\n");
	  else if(strncmp((buffer + 6), "SetCoordSystem", 14) == 0)
	      fprintf(stderr, "WARNING: file contains SetCoordSystem\n");
	  for (j = 0; j <= k; j++)
	    fputc(buffer[j], outFile);
	}
      else
	fprintf(stderr, "bug in CSYmover");
    }
}

/************************************************************************/

/* main

The executable should be called with no arguments. If one or more
arguments is given, the interpreter prints:
Usage: CSYmover
and exits with value 1.

This main routine repeatedly asks the user for input and then
reads an input file and write an output file. The files are
.prg I++ command files.

*/

int main(
 int argc,
 char ** argv)
{
  char inputFileName[80];
  char outputFileName[80];
  char pointsRight[80];
  char pointsUp[80];
  double originX;
  double originY;
  double originZ;
  int quit;        /* 1=quit, 0=don't */
  FILE * outFile;
  FILE * inFile;

  if (argc != 1)
    {
      fprintf(stderr, "Usage: %s\n", argv[0]);
      return(1);
    }
  strcpy(inputFileName, "no input file");
  strcpy(outputFileName, "no output file");
  strcpy(pointsRight, "+X");
  strcpy(pointsUp, "+Z");
  originX = 0.0;
  originY = 0.0;
  originZ = 0.0;
  quit = 0;
  for (; ;)
    {
      getUserInput(inputFileName, outputFileName, pointsRight, pointsUp,
		   &originX, &originY, &originZ, &quit);
      if (quit)
	break;
      inFile = fopen(inputFileName, "rb");
      if (inFile == NULL)
	{
	  fprintf(stderr, "could not open input file %s to read\n",
		  inputFileName);
	  continue;
	}
      outFile = fopen(outputFileName, "w");
      if (outFile == NULL)
	{
	  fprintf(stderr, "could not open output file %s to write\n",
		  outputFileName);
	  continue;
	}
      printFile(pointsRight, pointsUp,
		originX, originY, originZ, inFile, outFile);
      fclose(inFile);
      fclose(outFile);
    }
  return(0);
}

/***********************************************************************/
