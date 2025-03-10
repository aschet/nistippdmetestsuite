#include "../Server/StdAfx.h"
/* parserCmd.cc

This is a command parser for version 1.40 of the I++ DME Spec.

This defines functions in the parserCmd class and defines a main
function outside the parserCmd class.

Functions named makeXXX (where XXX is a command name) are used to
check the arguments to each type of command and to return an instance
of the command. The documentation for each of these gives the rules
that it is enforcing and gives one or more references to pages of
version 1.40 of the spec. The error code generated if a rule is
broken is given after each rule.

The rules concern allowable arguments only. Context rules are not
handled by the parserCmd.

The reference pages reference both text and examples. Text references
are given in parentheses. Example references are given in brackets.
Other references are not enclosed.

Example: Reference pages: 23 26 (39) [40] 85
This means there is relevant text on page 39 and an example on page 40.

Page 17 is not referenced because the print is too small. Page 25 is not
referenced since everything on it appears identically elsewhere.

*/

#include "../CmdResClasses/IppCmdRes.h"
#include "parserCmd.h"

/*******************************************************************/

/* remove the // on the next line to build a stand-alone command parser */
//#define PARSER_MAIN

#ifdef PARSER_MAIN

/*******************************************************************/

/* main

This main function is compiled only if PARSER_MAIN is defined.

When this function starts, it asks the user to enter "y" or "n" to
answer the question, "turn stepping on?". If the user enters y
(followed by <Enter>), stepping is turned on. Otherwise, stepping is
kept off. When stepping is on, the function parses one command at a
time, waiting for the user to press <Enter> before going on to the
next command. When stepping is off, the function parses an entire file
of commands without pausing.

The function then asks the user to enter the name of a command file to
parse, or to enter q to quit. If the name of a command file is
entered, the function parses it.

The function repeatedly reads a command string from the command file
and calls the parser's parseCommand method.  If parsing succeeds,
parseCommand makes an instance of a Command and returns a pointer to
it. The main function then prints the Command followed by \\ on a
separate line. If parsing fails, parseCommand returns a NULL
pointer. The main function then prints (1) the text of the command string,
followed by (2) the error message caused by the command string, followed
by (3) the I++ error number of the error.

*/

int main()
{
  FILE * infile;
  parserCmd parser1;
  char buffer[IPPSIZE];
  char commandText[IPPSIZE];
  char errorMessage[100];
  int k;
  int c;
  Command * aCommand;
  int code;
  int step;

  printf("turn stepping on? y/n\n");
  scanf("%s", buffer);
  step = ((strcmp(buffer, "y") == 0) ? 1 : 0);
  for(; ;)
    {
      printf("enter input file name or q to quit\n");
      scanf("%s", buffer);
      if (strcmp(buffer, "q") == 0)
	exit(0);
      infile = fopen(buffer, "rb");
      if (infile == NULL)
	{
	  fprintf(stderr, "unable to open file %s\n", buffer);
	  continue;
	}
      for (c = 32; c != EOF; )
	{
	  for (k=0; (((c = fgetc(infile)) != EOF) && (k < IPPSIZE)); k++)
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
	      fprintf(stderr, "command too long\n");
	      continue;
	    }
	  else if (c == EOF) // should exit before EOF
	    {
	      fprintf(stderr, "error in last line of file\n");
	      break;
	    }
	  else if ((k ==2) && (buffer[0] == ':'))
	    break; // stop if line starting w. colon
	  
	  buffer[k - 3] = 0;
	  parser1.setInput(buffer);
	  aCommand = parser1.parseCommand();
	  code = parser1.getParserErr();
	  if (code != OK)
	    {
	      fprintf(stderr, "%s", buffer);
	      parser1.getErrorMessageString(errorMessage);
	      fprintf(stderr, "%s\n", errorMessage);
	      fprintf(stderr, "%04d\n\n", parser1.getIppErr(code));
	      if (step)
		getchar();
	      continue;
	    }
	  else if (aCommand == NULL)
	    { // if aCommand is NULL, code should not be OK
	      fprintf(stderr, "bug in main\n");
	      exit(1);
	    }
	  else 
	    {
	      if (aCommand->getCommandString(commandText, (IPPSIZE - 1)))
		printf("%s%c%c%c%c", commandText, 92, 92, 13, 10);
	      else
		fprintf(stderr, "command did not print\n");
	    }
	  if (step)
	    {
	      fflush(stdin);
	      getchar();
	    }
	}
      fclose(infile);
      printf("%c%c%c%c%c%c", 58, 13, 10, 58, 13, 10);
    }
  return 0;
}

#endif /* #ifdef PARSER_MAIN */

/*******************************************************************/

/* parserCmd::getIppErr

Returned Value : int (the integer value of the I++ error code corresponding
  to the error code assigned by the parser)

Called By:
   external functions
   main (in stand-alone command parser and stand-alone command checker)

*/

const int parserCmd::getIppErr(int code)
{
  static const int ippErrors[] = {
//A
    -1,
    BadArgument,
    BadArgument,
    BadArgument,
    BadArgument,
//B
    BadArgument,
    BadArgument,
    BadArgument,
    BadArgument,
    BadArgument,
//C
    BadArgument,
    BadArgument,
    BadArgument,
    BadArgument,
    IncorrectArguments,
//D
    IllegalCommand,
    IncorrectArguments,
    IncorrectArguments,
    IllegalCommand,
    IncorrectArguments,
//E
    BadArgument,
    BadArgument,
    IncorrectArguments,
    IncorrectArguments,
    BadArgument,
//F
    BadArgument,
    BadArgument,
    BadArgument,
    BadArgument,
    IllegalTag,
//G
    IncorrectArguments,
    BadArgument,
    IncorrectArguments,
    BadArgument,
    IncorrectArguments,
//H
    IllegalTag,
    IncorrectArguments,
    IncorrectArguments,
    IncorrectArguments,
    IncorrectArguments,
//I
    IncorrectArguments,
    IncorrectArguments,
    IncorrectArguments,
    IncorrectArguments,
    IncorrectArguments,
//J
    IllegalTag,
    BadArgument,
    BadArgument,
    IncorrectArguments,
    IncorrectArguments,
//K
    IncorrectArguments,
    NoSpaceAtPos6,
    BadArgument,
    IllegalTag,
    IllegalTag,
//L
    ThetaOutOfRange,
    IncorrectArguments,
    BadArgument,
    IncorrectArguments,
    IncorrectArguments,
//M
    IncorrectArguments,
    IncorrectArguments,
    BadArgument,
    IncorrectArguments,
    IncorrectArguments,
//N
    BadArgument,
    IncorrectArguments,
    BadArgument
  };
  return ippErrors[code];
}

/*******************************************************************/

/* parserCmd::getErrorMessageString

Returned Value : char *
  If there is a problem with providing a string, this returns NULL.
  Otherwise, it returns the buffer argument.

Called By:
   external functions
   main (in stand-alone command parser and stand-alone command checker)

This copies an error message into the buffer. If the error was found
after a command name was recognized (i.e. the error occurred while
parsing the argument list of a valid command), the name of the command
is written into the buffer just before the error message.  For example,
if the command is 05502 GoTo(X(#)), which is OK up to the #, the error
message in the buffer will be "GoTo: BAD COMMAND ARGUMENTS".  If the
command is A2345 GoTo(X(3.1)), which has an error that is detected
before the command name is read, the error message in the buffer will
be "BAD TAG CHARACTER"

The symbol for each error code is the same as the text of the error
message, except that the symbol has underscores. The symbol serves as
an index into the array of strings defined here. The symbols are
defined in parserCmd.h.

*/

char * parserCmd::getErrorMessageString(char * buffer)
{
  static const char * const errorStrings[] = {
//A
    "OK",
    "ARGUMENT FIRST PART MUST BE TOOL OR FOUNDTOOL",
    "ARGUMENT FOURTH PART MUST BE ACT",
    "ARGUMENT FOURTH PART BAD",
    "ARGUMENT MUST BE A NUMBER",
//B
    "ARGUMENT MUST BE EVENT TAG",
    "ARGUMENT MUST BE INTEGER",
    "ARGUMENT MUST BE POSITIVE",
    "ARGUMENT MUST BE STRING",
    "ARGUMENT MUST END WITH EMPTY PARENTHESES",
//C
    "ARGUMENT MUST END WITH NUMBER IN PARENTHESES",
    "ARGUMENT SECOND PART BAD",
    "ARGUMENT SECOND PART MUST BE GOTOPAR OR PTMEASPAR",
    "ARGUMENT THIRD PART BAD",
    "BAD ARGUMENTS",
//D
    "BAD CHARACTER AFTER COMMAND END",
    "BAD CHARACTER AFTER KEYWORD",
    "BAD COMMAND ARGUMENTS",
    "BAD COMMAND NAME",
    "BAD DOTS",
//E
    "BAD E NUMBER EXPONENT MUST HAVE ONE TWO OR THREE DIGITS",
    "BAD FIRST ARGUMENT",
    "BAD IJK NUMBERS",
    "BAD KEYWORD",
    "BAD NUMBER MORE THAN 16 DIGITS",
//F
    "BAD NUMBER NO DIGITS",
    "BAD NUMBER OF THROUGHS",
    "BAD NUMBER TWO DECIMAL POINTS",
    "BAD STRING",
    "BAD TAG CHARACTER",
//G
    "C ARGUMENT USED TWICE",
    "DIAMETER MUST BE POSITIVE",
    "DIS ARGUMENT USED TWICE",
    "DIS MUST BE FOLLOWED BY NUMBER IN PARENS",
    "ER ARGUMENT USED TWICE",
//H
    "EVENT COMMAND MUST HAVE EVENT TAG",
    "IJK ARGUMENT USED TWICE",
    "IJK NOT FOLLOWED BY ARGIJK",
    "ILLEGAL SPACE",
    "MUST HAVE NO ARGUMENTS",
//I
    "MUST HAVE ONE ARGUMENT",
    "MUST HAVE 4 ARGUMENTS",
    "MUST HAVE 4 OR 8 ARGUMENTS",
    "MUST HAVE 7 ARGUMENTS",
    "MUST HAVE 7 OR 14 ARGUMENTS",
//J
    "NON EVENT COMMAND MUST NOT HAVE EVENT TAG",
    "NUMBER THROUGH MUST BE INTEGER",
    "NUMBER THROUGH MUST BE POSITIVE",
    "PHI ARGUMENT USED TWICE",
    "Q ARGUMENT USED TWICE",
//K
    "R ARGUMENT USED TWICE", 
    "SPACE MISSING AFTER TAG",
    "STEPW MUST BE POSITIVE",
    "TAG NUMBER OUT OF RANGE FOR EVENT TAG",
    "TAG NUMBER OUT OF RANGE FOR COMMAND TAG",
//L
    "THETA OUT OF RANGE",
    "TIME ARGUMENT USED TWICE",
    "TIME MUST BE FOLLOWED BY NUMBER IN PARENS",
    "TOOL A ARGUMENT USED TWICE",
    "TOOL B ARGUMENT USED TWICE",
//M
    "TOOL C ARGUMENT USED TWICE",
    "X ARGUMENT USED TWICE", 
    "X NOT FOLLOWED BY NUMBER IN PARENS",
    "X Y Z ALL MISSING",
    "Y ARGUMENT USED TWICE",
//N
    "Y NOT FOLLOWED BY NUMBER IN PARENS",
    "Z ARGUMENT USED TWICE",
    "Z NOT FOLLOWED BY NUMBER IN PARENS"
  };
  if ((commandName > -1) && (commandName < 55))
    sprintf(buffer, "%s: %s",
	    ippCommandNameStrings[commandName], errorStrings[errorCode]);
  else
    sprintf(buffer, "%s", errorStrings[errorCode]);
  return buffer;
}

/*******************************************************************/

/* parserCmd::makeArraysBigger

Returned Value: None.

Called By: parserCmd::parseArguments

Rules: None.

This doubles the sizes of the four arrays in which arguments are stored.
It also doubles argSize.

This makes new arrays twice the size of the old arrays and copies the
contents of the old arrays into the first half of the new arrays.
The old arrays are deleted.

*/

void parserCmd::makeArraysBigger()
{
  double * oldArgDoubles;
  int * oldArgKeywords;
  int * oldArgTypes;
  char ** oldArgStrings;
  int n;
    
  oldArgDoubles = argDoubles;
  oldArgKeywords = argKeywords;
  oldArgStrings = argStrings;
  oldArgTypes = argTypes;
  argDoubles = new double[2 * argSize];
  argKeywords = new int[2 * argSize];
  argStrings = new char * [2 * argSize];
  argTypes = new int[2 * argSize];
  for (n = 0; n < argSize; n++)
    {
      argDoubles[n] = oldArgDoubles[n];
      argKeywords[n] = oldArgKeywords[n];
      argStrings[n] = oldArgStrings[n];
      argTypes[n] = oldArgTypes[n];
    }
  delete oldArgDoubles;
  delete oldArgKeywords;
  delete oldArgStrings;
  delete oldArgTypes;
  argSize = (2 * argSize);
}

/*******************************************************************/

/* parserCmd::makeAbortE

Returned Value: A Command of commandNameType AbortE as described by the
  command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 9 24 26 (42) [43] 99

*/

Command * parserCmd::makeAbortE()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, AbortE)));
}

/*******************************************************************/

/* parserCmd::makeAlignPart

Returned Value: An AlignPartCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 7 or 14 arguments.
   MUST_HAVE_7_OR_14_ARGUMENTS
2. The arguments must all be numbers.
   BAD_ARGUMENTS
3. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 28 63 (95) 105

*/

Command * parserCmd::makeAlignPart()
{
  if ((argCount != 13) && (argCount != 27))
    errorCode = MUST_HAVE_7_OR_14_ARGUMENTS;
  else if ((argTypes[0]  != ARGDOUBLE) ||
	   (argTypes[1]  != ARGCOMMA)  ||
	   (argTypes[2]  != ARGDOUBLE) ||
	   (argTypes[3]  != ARGCOMMA)  ||
	   (argTypes[4]  != ARGDOUBLE) ||
	   (argTypes[5]  != ARGCOMMA)  ||
	   (argTypes[6]  != ARGDOUBLE) ||
	   (argTypes[7]  != ARGCOMMA)  ||
	   (argTypes[8]  != ARGDOUBLE) ||
	   (argTypes[9]  != ARGCOMMA)  ||
	   (argTypes[10] != ARGDOUBLE) ||
	   (argTypes[11] != ARGCOMMA)  ||
	   (argTypes[12] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  else if ((argCount == 27) &&
	   ((argTypes[13] != ARGCOMMA)  ||
	    (argTypes[14] != ARGDOUBLE) ||
	    (argTypes[15] != ARGCOMMA)  ||
	    (argTypes[16] != ARGDOUBLE) ||
	    (argTypes[17] != ARGCOMMA)  ||
	    (argTypes[18] != ARGDOUBLE) ||
	    (argTypes[19] != ARGCOMMA)  ||
	    (argTypes[20] != ARGDOUBLE) ||
	    (argTypes[21] != ARGCOMMA)  ||
	    (argTypes[22] != ARGDOUBLE) ||
	    (argTypes[23] != ARGCOMMA)  ||
	    (argTypes[24] != ARGDOUBLE) ||
	    (argTypes[25] != ARGCOMMA)  ||
	    (argTypes[26] != ARGDOUBLE)))
    errorCode = BAD_ARGUMENTS;
  return ((errorCode != OK) ? NULL:
	  (argCount == 13) ?
	  new AlignPartCommand(tag, false,
			       argDoubles[0], argDoubles[2], argDoubles[4],
			       argDoubles[6], argDoubles[8], argDoubles[10],
                               0, 0, 0, 0, 0, 0, argDoubles[12], 0) :
	  new AlignPartCommand(tag, true,
			       argDoubles[0],  argDoubles[2],  argDoubles[4],
			       argDoubles[6],  argDoubles[8],  argDoubles[10],
			       argDoubles[12], argDoubles[14], argDoubles[16],
			       argDoubles[18], argDoubles[20], argDoubles[22],
			       argDoubles[24], argDoubles[26]));
}

/*******************************************************************/

/* parserCmd::makeAlignTool

Returned Value: An AlignToolCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 4 or 8 arguments.
   MUST_HAVE_4_OR_8_ARGUMENTS
2. The arguments must all be numbers.
   BAD_ARGUMENTS
3. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 29 (55) 66 70 102 107

*/

Command * parserCmd::makeAlignTool()
{
  if ((argCount != 7) && (argCount != 15))
    errorCode = MUST_HAVE_4_OR_8_ARGUMENTS;
  else if ((argTypes[0] != ARGDOUBLE) ||
	   (argTypes[1] != ARGCOMMA)  ||
	   (argTypes[2] != ARGDOUBLE) ||
	   (argTypes[3] != ARGCOMMA)  ||
	   (argTypes[4] != ARGDOUBLE) ||
	   (argTypes[5] != ARGCOMMA)  ||
	   (argTypes[6] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  else if ((argCount == 15) &&
	   ((argTypes[7]  != ARGCOMMA)  ||
	    (argTypes[8]  != ARGDOUBLE) ||
	    (argTypes[9]  != ARGCOMMA)  ||
	    (argTypes[10] != ARGDOUBLE) ||
	    (argTypes[11] != ARGCOMMA)  ||
	    (argTypes[12] != ARGDOUBLE) ||
	    (argTypes[13] != ARGCOMMA)  ||
	    (argTypes[14] != ARGDOUBLE)))
    errorCode = BAD_ARGUMENTS;
  return ((errorCode != OK) ? NULL:
	  (argCount == 7) ?
	  new AlignToolCommand(tag, false, argDoubles[0], argDoubles[2],
			       argDoubles[4], 0, 0, 0, argDoubles[6], 0) :
	  new AlignToolCommand(tag, true, argDoubles[0], argDoubles[2],
			       argDoubles[4], argDoubles[6],
			       argDoubles[8], argDoubles[10],
			       argDoubles[12], argDoubles[14]));
}

/*******************************************************************/

/* parserCmd::makeCenterPart

Returned Value: A CenterPartCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 4 arguments.
   MUST_HAVE_4_ARGUMENTS
2. The arguments must all be numbers.
   BAD_ARGUMENTS
3. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (96) 102

*/

Command * parserCmd::makeCenterPart()
{
  if (argCount != 7)
    errorCode = MUST_HAVE_4_ARGUMENTS;
  else if ((argTypes[0] != ARGDOUBLE) ||
	   (argTypes[1] != ARGCOMMA)  ||
	   (argTypes[2] != ARGDOUBLE) ||
	   (argTypes[3] != ARGCOMMA)  ||
	   (argTypes[4] != ARGDOUBLE) ||
	   (argTypes[5] != ARGCOMMA)  ||
	   (argTypes[6] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  return ((errorCode != OK) ? NULL:
	  new CenterPartCommand(tag, argDoubles[0], argDoubles[2],
				argDoubles[4], argDoubles[6]));
}

/*******************************************************************/

/* parserCmd::makeChangeTool

Returned Value: A ChangeToolCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   MUST_HAVE_ONE_ARGUMENT
2. The argument must be a string.
   ARGUMENT_MUST_BE_STRING

Reference pages: 24 27 29 (54) (57) (58) (78) 98 102 106

*/

Command * parserCmd::makeChangeTool()
{
  if (argCount != 1)
    errorCode = MUST_HAVE_ONE_ARGUMENT;
  else if (argTypes[0] != ARGSTRING)
    errorCode = ARGUMENT_MUST_BE_STRING;
  return ((errorCode != OK) ? NULL :
	  (new ChangeToolCommand(tag, argStrings[0])));
}

/*******************************************************************/

/* parserCmd::makeClearAllErrors

Returned Value: A Command of commandNameType ClearAllErrors as described
  by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 26 [38] (40) (41) (42) (43) [44] (47) (75) 76 99

*/

Command * parserCmd::makeClearAllErrors()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, ClearAllErrors)));
}

/*******************************************************************/

/* parserCmd::makeDisableUser

Returned Value: A Command of commandNameType DisableUser as described
  by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 27 (48) (51) (53) 102

*/

Command * parserCmd::makeDisableUser()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, DisableUser)));
}

/*******************************************************************/

/* parserCmd::makeEnableUser

Returned Value: A Command of commandNameType EnableUser as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 27 [39] (48) (51) 102

*/

Command * parserCmd::makeEnableUser()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, EnableUser)));
}

/*******************************************************************/

/* parserCmd::makeEndSession

Returned Value: A Command of commandNameType EndSession as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 18 20 24 26 (41) (77) 99

*/

Command * parserCmd::makeEndSession()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, EndSession)));
}

/*******************************************************************/

/* parserCmd::makeEnumAllProp

Returned Value: An EnumAllPropCommand as described by the command text,
  or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   BAD_ARGUMENTS
2. The argument must have one of the following patterns.
   key1()
   key1.key2()
   key1.key2.key3()
   key1.key2.key3.key4()
3. key1 must be FoundTool or Tool.
   ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL
4. key2 must be GoToPar or PtMeasPar.
   ARGUMENT_SECOND_PART_MUST_BE_GOTOPAR_OR_PTMEASPAR
5. If key2 is GoToPar, key3 must be Speed or Accel.
   If key2 is PtMeasPar, key3 must be one of
   Speed, Accel, Approach, Retract, or Search.
   ARGUMENT_THIRD_PART_BAD
6. If there is a key4, it must be Act, Def, Max, or Min.
   ARGUMENT_FOURTH_PART_BAD
7. The dots must be placed as shown in rule 2.
   BAD_DOTS
8. The empty parentheses must be placed as shown in rule 2.
   ARGUMENT_MUST_END_WITH_EMPTY_PARENTHESES

Reference pages: 24 26 45 (46) [74] 99

See the documentation of parserCmd::makeGetProp.

This is not allowing Alignment, CollisionVolume, Name, or Id, since
the spec allows them only with GetProp.

*/

Command * parserCmd::makeEnumAllProp()
{
  if ((argCount != 2) && (argCount != 4) && (argCount != 6) && (argCount != 8))
    errorCode = BAD_ARGUMENTS;
  else if ((argTypes[0] != ARGKEYWORD) ||
	   ((argKeywords[0] != Tool) && (argKeywords[0] != FoundTool)))
    errorCode = ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL;
  else if (((argCount == 2) && (argTypes[1] != ARGPARENEMPTY)) ||
	   ((argCount == 4) && (argTypes[3] != ARGPARENEMPTY)) ||
	   ((argCount == 6) && (argTypes[5] != ARGPARENEMPTY)) ||
	   ((argCount == 8) && (argTypes[7] != ARGPARENEMPTY)))
    errorCode = ARGUMENT_MUST_END_WITH_EMPTY_PARENTHESES;
  else if (((argCount > 2) && (argTypes[1] != ARGDOT)) ||
	   ((argCount > 4) && (argTypes[3] != ARGDOT)) ||
	   ((argCount > 6) && (argTypes[5] != ARGDOT)))
    errorCode = BAD_DOTS;
  else if ((argCount > 2) &&
	   ((argTypes[2] != ARGKEYWORD) ||
	    ((argKeywords[2] != GoToPar) && (argKeywords[2] != PtMeasPar))))
    errorCode = ARGUMENT_SECOND_PART_MUST_BE_GOTOPAR_OR_PTMEASPAR;
  else if ((argCount > 4) && 
	   ((argTypes[4] != ARGKEYWORD) ||
	    ((argKeywords[4] != Speed) &&
	     (argKeywords[4] != Accel) &&
	     ((argKeywords[2] == GoToPar) || 
	      ((argKeywords[4] != Approach) &&
	       (argKeywords[4] != Retract) &&
	       (argKeywords[4] != Search))))))
    errorCode = ARGUMENT_THIRD_PART_BAD;
  else if ((argCount > 6) && 
	   ((argTypes[6] != ARGKEYWORD) ||
	    ((argKeywords[6] != Actual) &&
	     (argKeywords[6] != Default) &&
	     (argKeywords[6] != Max) &&
	     (argKeywords[6] != Min))))
    errorCode = ARGUMENT_FOURTH_PART_BAD;
  return ((errorCode != OK) ? NULL:
	  (new EnumAllPropCommand
	   (tag,
	    ((argCount == 2)? 1 : (argCount == 4)? 2 : (argCount == 6)? 3 : 4),
	    (otherKeyType)argKeywords[0],
	    (otherKeyType)((argCount > 2) ? argKeywords[2] : argKeywords[0]),
	    (otherKeyType)((argCount > 4) ? argKeywords[4] : argKeywords[0]),
	    (otherKeyType)((argCount > 6) ? argKeywords[6] : argKeywords[0]))));
}

/*******************************************************************/

/* parserCmd::makeEnumProp

Returned Value: An EnumPropCommand as described by the command text,
  or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   BAD_ARGUMENTS
2. The argument must have one of the following patterns.
   key1()
   key1.key2()
   key1.key2.key3()
   key1.key2.key3.key4()
3. key1 must be FoundTool or Tool.
   ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL
4. If there is a key2, it must be GoToPar or PtMeasPar.
   ARGUMENT_SECOND_PART_MUST_BE_GOTOPAR_OR_PTMEASPAR
5. If key2 is GoToPar and there is a key3, key3 must be Speed or MaxAccel.
   If key2 is PtMeasPar and there is a key3, key3 must be one of
   Speed, Accel, Approach, Retract, or Search.
   ARGUMENT_THIRD_PART_BAD
6. If there is a key4, it must be Act, Def, Max, or Min.
   ARGUMENT_FOURTH_PART_BAD
7. The dots must be placed as shown in rule 1.
   BAD_DOTS
8. The empty parentheses must be placed as shown in rule 1.
   ARGUMENT_MUST_END_WITH_EMPTY_PARENTHESES

Reference pages: 24 26 29 (45) [73] 99 108 109 110 111 

See the documentation of parserCmd::makeGetProp.

This is not allowing Alignment, CollisionVolume, Name, or Id, since
the spec allows them only with GetProp.

*/

Command * parserCmd::makeEnumProp()
{
  if ((argCount != 2) && (argCount != 4) && (argCount != 6) && (argCount != 8))
    errorCode = BAD_ARGUMENTS;
  else if ((argTypes[0] != ARGKEYWORD) ||
	   ((argKeywords[0] != Tool) && (argKeywords[0] != FoundTool)))
    errorCode = ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL;
  else if (((argCount == 2) && (argTypes[1] != ARGPARENEMPTY)) ||
	   ((argCount == 4) && (argTypes[3] != ARGPARENEMPTY)) ||
	   ((argCount == 6) && (argTypes[5] != ARGPARENEMPTY)) ||
	   ((argCount == 8) && (argTypes[7] != ARGPARENEMPTY)))
    errorCode = ARGUMENT_MUST_END_WITH_EMPTY_PARENTHESES;
  else if (((argCount > 2) && (argTypes[1] != ARGDOT)) ||
	   ((argCount > 4) && (argTypes[3] != ARGDOT)) ||
	   ((argCount > 6) && (argTypes[5] != ARGDOT)))
    errorCode = BAD_DOTS;
  else if ((argCount > 2) &&
	   ((argTypes[2] != ARGKEYWORD) ||
	    ((argKeywords[2] != GoToPar) && (argKeywords[2] != PtMeasPar))))
    errorCode = ARGUMENT_SECOND_PART_MUST_BE_GOTOPAR_OR_PTMEASPAR;
  else if ((argCount > 4) && 
	   ((argTypes[4] != ARGKEYWORD) ||
	    ((argKeywords[4] != Speed) &&
	     (argKeywords[4] != Accel) &&
	     ((argKeywords[2] == GoToPar) || 
	      ((argKeywords[4] != Approach) &&
	       (argKeywords[4] != Retract) &&
	       (argKeywords[4] != Search))))))
    errorCode = ARGUMENT_THIRD_PART_BAD;
  else if ((argCount > 6) && 
	   ((argTypes[6] != ARGKEYWORD) ||
	    ((argKeywords[6] != Actual) &&
	     (argKeywords[6] != Default) &&
	     (argKeywords[6] != Max) &&
	     (argKeywords[6] != Min))))
    errorCode = ARGUMENT_FOURTH_PART_BAD;
  return ((errorCode != OK) ? NULL:
	  (new EnumPropCommand
	   (tag,
	    ((argCount == 2)? 1 : (argCount == 4)? 2 : (argCount == 6)? 3 : 4),
	    (otherKeyType)argKeywords[0],
	    (otherKeyType)((argCount > 2) ? argKeywords[2] : argKeywords[0]),
	    (otherKeyType)((argCount > 4) ? argKeywords[4] : argKeywords[0]),
	    (otherKeyType)((argCount > 6) ? argKeywords[6] : argKeywords[0]))));
}

/*******************************************************************/

/* parserCmd::makeEnumTools

Returned Value: A Command of commandNameType EnumTools as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 29 (53) (54) (56) 106

*/

Command * parserCmd::makeEnumTools()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, EnumTools)));
}

/*******************************************************************/

/* parserCmd::makeFindTool

Returned Value: A FindToolCommand as described by the command text,
  or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   MUST_HAVE_ONE_ARGUMENT
2. The argument must be a string.
   ARGUMENT_MUST_BE_STRING

Reference pages: 24 27 29 (54) [74] 102 106

*/

Command * parserCmd::makeFindTool()
{
  if (argCount != 1)
    errorCode = MUST_HAVE_ONE_ARGUMENT;
  else if (argTypes[0] != ARGSTRING)
    errorCode = ARGUMENT_MUST_BE_STRING;
  return((errorCode != OK) ? NULL: (new FindToolCommand(tag, argStrings[0])));
}

/*******************************************************************/

/* parserCmd::makeGet

Returned Value: A GetCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one to seven arguments.
   BAD_ARGUMENTS
2. Each argument must be R(), X(), Y(), Z(), Tool.A(), Tool.B() or Tool.C(),
   which may appear in any order.
   BAD_ARGUMENTS
3. Each allowed argument must appear at most once.
   R_ARGUMENT_USED_TWICE
   X_ARGUMENT_USED_TWICE
   Y_ARGUMENT_USED_TWICE
   Z_ARGUMENT_USED_TWICE
   TOOL_A_ARGUMENT_USED_TWICE
   TOOL_B_ARGUMENT_USED_TWICE
   TOOL_C_ARGUMENT_USED_TWICE
4. Commas must separate the arguments if there are two or more.
   BAD_ARGUMENTS

Reference pages: 24 27 [38] [44] (49) (51) 57 (61) (63) (66) (81) 102

Tool properties that can be set directly are intended to be obtained
by GetProp, not by Get.

This function is similar to makeGoTo.

This function is messy because it takes a variable number of arguments
and the arguments may have different numbers of components, e.g., "X()"
has 2 components while "Tool.A()" has 4.

The check (((n + 2) < argCount) && ((n + 3) == argCount)) is to detect
the case of a comma at the end of the argument list, which will otherwise
slip by. The (((n + 4) < argCount) && ((n + 5) == argCount)) does the
same thing.

*/

Command * parserCmd::makeGet()
{
  int n; // index for components of command arguments
  bool hasR = false;
  bool hasX = false;
  bool hasY = false;
  bool hasZ = false;
  bool hasToolA = false;
  bool hasToolB = false;
  bool hasToolC = false;

  if (argCount < 2)
    errorCode = BAD_ARGUMENTS;
  for (n = 0; ((n < argCount) && (errorCode == OK)); )
    {
      if ((argTypes[n] != ARGKEYWORD) || ((n + 1) == argCount))
	errorCode = BAD_ARGUMENTS;
      else if ((argKeywords[n] == R) ||
	       (argKeywords[n] == X) ||
	       (argKeywords[n] == Y) ||
	       (argKeywords[n] == Z))
	{
	  if ((argTypes[n + 1] != ARGPARENEMPTY) ||
	      (((n + 2) < argCount) && (argTypes[n + 2] != ARGCOMMA)) ||
	      (((n + 2) < argCount) && ((n + 3) == argCount)))
	    errorCode = BAD_ARGUMENTS;
	  else if (argKeywords[n] == R)
	    {
	      if (hasR)
		errorCode = R_ARGUMENT_USED_TWICE;
	      else
		hasR = true;
	    }
	  else if (argKeywords[n] == X)
	    {
	      if (hasX)
		errorCode = X_ARGUMENT_USED_TWICE;
	      else
		hasX = true;
	    }
	  else if (argKeywords[n] == Y)
	    {
	      if (hasY)
		errorCode = Y_ARGUMENT_USED_TWICE;
	      else
		hasY = true;
	    }
	  else if (argKeywords[n] == Z)
	    {
	      if (hasZ)
		errorCode = Z_ARGUMENT_USED_TWICE;
	      else
		hasZ = true;
	    }
	  n = (n + 3);
	}
      else if (argKeywords[n] == Tool)
	{
	  if (((n + 3) >= argCount) ||
	      (argTypes[n + 1] != ARGDOT) ||
	      (argTypes[n + 2] != ARGKEYWORD) ||
	      (argTypes[n + 3] != ARGPARENEMPTY) ||
	      (((n + 4) < argCount) && (argTypes[n + 4] != ARGCOMMA)) ||
	      (((n + 4) < argCount) && ((n + 5) == argCount)))
	    errorCode = BAD_ARGUMENTS;
	  else if (argKeywords[n + 2] == A)
	    {
	      if (hasToolA)
		errorCode = TOOL_A_ARGUMENT_USED_TWICE;
	      else
		hasToolA = true;
	    }
	  else if (argKeywords[n + 2] == B)
	    {
	      if (hasToolB)
		errorCode = TOOL_B_ARGUMENT_USED_TWICE;
	      else
		hasToolB = true;
	    }
	  else if (argKeywords[n + 2] == C)
	    {
	      if (hasToolC)
		errorCode = TOOL_C_ARGUMENT_USED_TWICE;
	      else
		hasToolC = true;
	    }
	  else
	    errorCode = BAD_ARGUMENTS;
	  n = (n + 5);
	}
      else
	errorCode = BAD_ARGUMENTS;
    }
  return ((errorCode != OK) ? NULL:
	  (new GetCommand(tag, hasR, hasX, hasY, hasZ,
			  hasToolA, hasToolB, hasToolC)));
}

/*******************************************************************/

/* parserCmd::makeGetChangeToolAction

Returned Value: A GetChangeToolActionCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   MUST_HAVE_ONE_ARGUMENT
2. The argument must be a string.
   ARGUMENT_MUST_BE_STRING

Reference pages: 24 27 29 (57-58) 102 106

*/

Command * parserCmd::makeGetChangeToolAction()
{
  if (argCount != 1)
    errorCode = MUST_HAVE_ONE_ARGUMENT;
  else if (argTypes[0] != ARGSTRING)
    errorCode = ARGUMENT_MUST_BE_STRING;
  return ((errorCode != OK) ? NULL :
	  (new GetChangeToolActionCommand(tag, argStrings[0])));
}

/*******************************************************************/

/* parserCmd::makeGetCoordSystem

Returned Value: A Command of commandNameType GetCoordSystem as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 28 (59) 103

*/

Command * parserCmd::makeGetCoordSystem()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, GetCoordSystem)));
}

/*******************************************************************/

/* parserCmd::makeGetCsyTransformation

Returned Value: A GetCsyTransformationCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   MUST_HAVE_ONE_ARGUMENT
2. The argument must be one of: JogDisplayCsy, JogMoveCsy, MoveableMachineCsy,
   MultipleArmCsy, PartCsy, SensorCsy.
   BAD_ARGUMENTS

Reference pages: 24 28 (60) (80) 103

*/

Command * parserCmd::makeGetCsyTransformation()
{
  if (argCount != 1)
    errorCode = MUST_HAVE_ONE_ARGUMENT;
  else if ((argTypes[0] != ARGKEYWORD) ||
	   ((argKeywords[0] != JogDisplayCsy) &&
	    (argKeywords[0] != JogMoveCsy) &&
	    (argKeywords[0] != MoveableMachineCsy) &&
	    (argKeywords[0] != MultipleArmCsy) &&
	    (argKeywords[0] != PartCsy) &&
	    (argKeywords[0] != SensorCsy)))
    errorCode = BAD_ARGUMENTS;
  return ((errorCode != OK) ? NULL :
	  (new GetCsyTransformationCommand(tag, (coordSysType)argKeywords[0])));
}

/*******************************************************************/

/* parserCmd::makeGetDMEVersion

Returned Value: A Command of commandNameType GetDMEVersion as
described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 26 (46) [47] 99

*/

Command * parserCmd::makeGetDMEVersion()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, GetDMEVersion)));
}

/*******************************************************************/

/* parserCmd::makeGetErrorInfo

Returned Value: A GetErrorInfoCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   MUST_HAVE_ONE_ARGUMENT
2. The argument must be an integer.
   ARGUMENT_MUST_BE_INTEGER

Reference pages: 19 24 26 (43) 76 99
Page 76 does not mention the command but gives allowable error numbers.

For GetErrorInfo, the value of the argument is stored as a double
even though it must be an integer. This is so that argument reading
does not require the context of knowing the command. Here it is
checked that the double has an integer value.

*/

Command * parserCmd::makeGetErrorInfo()
{
  if (argCount != 1)
    errorCode = MUST_HAVE_ONE_ARGUMENT;
  else if ((argTypes[0] != ARGDOUBLE) || (argDoubles[0] != (int)argDoubles[0]))
    errorCode = ARGUMENT_MUST_BE_INTEGER;
  return ((errorCode != OK) ? NULL :
	  (new  GetErrorInfoCommand(tag, (unsigned int)argDoubles[0])));
}

/*******************************************************************/

/* parserCmd::makeGetErrStatusE

Returned Value: A Command of commandNameType GetErrStatusE as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 27 (50) 102

*/

Command * parserCmd::makeGetErrStatusE()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, GetErrStatusE)));
}

/*******************************************************************/

/* parserCmd::makeGetMachineClass

Returned Value: A Command of commandNameType GetMachineClass as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 18 24 27 (50) 102

*/

Command * parserCmd::makeGetMachineClass()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, GetMachineClass)));
}

/*******************************************************************/

/* parserCmd::makeGetProp

Returned Value: A GetPropCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be at least one argument, and each argument must have one of
   the three patterns:
   key1.key2() -- four components in argCount
   key1.key2.key3() -- six components in argCount
   key1.key2.key3.key4() -- eight components in argCount
   and all arguments except the last must be followed by a comma.
   BAD_ARGUMENTS
   ARGUMENT_SECOND_PART_BAD
2. key1 must be FoundTool or Tool.
   ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL
3. If the pattern is key1.key2():
   If key1 is Tool, key2 must be Alignment, CollisionVolume, Name, or Id.
   If key1 is FoundTool, key2 must be Name, or Id.
   ARGUMENT_SECOND_PART_BAD
4. If key2 is GoToPar, key3 must be Speed or Accel.
   If key2 is PtMeasPar, key3 must be one of
   Speed, Accel, Approach, Retract, or Search.
   ARGUMENT_THIRD_PART_BAD
5. If there is a key4, it must be Act, Def, Max, or Min.
   ARGUMENT_FOURTH_PART_BAD
6. The dots must be placed where shown in rule 1.
   BAD_DOTS
7. The empty parentheses must be placed where shown in rule 1.
   ARGUMENT_MUST_END_WITH_EMPTY_PARENTHESES

Reference pages: 24 26 29 (45) 66 67 [68] [69] 70 [73] [74] 77 79 99 108

The spec is not clear about what should be allowed as arguments.  Here
we assume that only primitive types (string, bool, number) can be
obtained, so the arguments must refer to something whose type is
primitive. For example, GetProp(Tool.PtMeasPar.Speed()) is OK, but
GetProp(Tool.PtMeasPar()) is not OK since the value of PtMeasPar is an
object, not a primitive type.

The spec implies, without being explicit, that key3() and key3.Act()
are both allowed and mean the same thing.

The spec states that GetProp may be used to query settable properties.
It says explicitly that Speed.Max cannot be queried using GetProp.
NIST has raised an issue against only allowing settable properties.
This function allows querying Speed.Max.

The spec is also not clear about whether CanChange, CanChangeSpeed, and
CanChangeAccel are allowable keywords. These are included on pages 29,
109, and 110 for GoToPars, PtMeasPars, and Param. None of the examples
of GetProp uses either of them. We are assuming they are not allowed.

We are assuming here that the properties (other than CanChange) of
"param" objects defined on pages 29, 109 and 110 are intended to be
accessible. These properties are not mentioned in section 6 and are
not included in any examples of GetProp. In a telephone conversation
with Josef Resch, however, he said Speed.Max should be used rather
than MaxSpeed.

The forms MaxSpeed, MinSpeed, MaxAccel, and MinAccel appear on pages
9, 29, 109, and 110, but not in section 6. They are not allowed here.

The spec on page 66 does not allow FoundTool.Name() or FoundTool.Id(),
but this appears to be an oversight and NIST has submitted a comment
suggesting FoundTool should be allowed. This function allows FoundTool.

The argCount is a count of components of arguments, not a count of
arguments.

The [(argCount + 1) / 5] size for the keys arrays is used because this
is the smallest number that is sure to be at least as large as the
number of arguments. Each argument must have at least four components,
and the arguments are separated by commas (which are added into argCount).

*/

Command * parserCmd::makeGetProp()
{
  otherKeyType * keys1;
  otherKeyType * keys2;
  otherKeyType * keys3;
  otherKeyType * keys4;
  int n;                // index for all components of command arguments
  int m;                // index for command arguments
  int components;       // number of components of an argument (4, 6, or 8)
  GetPropCommand * returnMe;

  keys1 = new otherKeyType[(argCount + 1) / 5];
  keys2 = new otherKeyType[(argCount + 1) / 5];
  keys3 = new otherKeyType[(argCount + 1) / 5];
  keys4 = new otherKeyType[(argCount + 1) / 5];
  m = 0;
  for (n = 0; ((n < argCount) && (errorCode == OK)); n++)
    {
      if (argCount < (n + 4))
	errorCode = BAD_ARGUMENTS;
      else if ((argTypes[n] != ARGKEYWORD) ||
	       ((argKeywords[n] != Tool) && (argKeywords[n] != FoundTool)))
	errorCode = ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL;
      else if (argTypes[n + 1] != ARGDOT)
	errorCode = BAD_DOTS;
      else if (argTypes[n + 2] != ARGKEYWORD)
	errorCode = ARGUMENT_SECOND_PART_BAD;
      else if (argTypes[n + 3] == ARGPARENEMPTY)
	{
	  if ((argKeywords[n] == FoundTool) &&
	      ((argKeywords[n + 2] == Alignment) || 
	       (argKeywords[n + 2] == CollisionVolume)))
	    errorCode = BAD_ARGUMENTS;
	  else if ((argKeywords[n + 2] != Alignment) &&
		   (argKeywords[n + 2] != CollisionVolume) &&
		   (argKeywords[n + 2] != Name) &&
		   (argKeywords[n + 2] != Id))
	    errorCode = ARGUMENT_SECOND_PART_BAD;
	  else
	    components = 4;
	}
      else if ((argKeywords[n + 2] != GoToPar) &&
	       (argKeywords[n + 2] != PtMeasPar))
	errorCode = ARGUMENT_SECOND_PART_BAD;
      else if (argTypes[n + 3] != ARGDOT)
	errorCode = BAD_DOTS;
      else if (argCount < (n + 6))
	errorCode = BAD_ARGUMENTS;
      else if ((argTypes[n + 4] != ARGKEYWORD) ||
	       ((argKeywords[n + 4] != Speed) && 
		(argKeywords[n + 4] != Accel) &&
		((argKeywords[n + 2] == GoToPar) ||
		 ((argKeywords[n + 4] != Approach) &&
		  (argKeywords[n + 4] != Retract) &&
		  (argKeywords[n + 4] != Search)))))
	errorCode = ARGUMENT_THIRD_PART_BAD;
      else if (argTypes[n + 5] ==  ARGPARENEMPTY)
	components = 6;
      else if (argTypes[n + 5] != ARGDOT)
	errorCode = BAD_DOTS;
      else if (argCount < (n + 8))
	errorCode = BAD_ARGUMENTS;
      else if (argTypes[n + 7] == ARGPARENEMPTY)
	{
	  if ((argTypes[n + 6] != ARGKEYWORD) ||
	      ((argKeywords[n + 6] != Actual) &&
	       (argKeywords[n + 6] != Default) &&
	       (argKeywords[n + 6] != Max) &&
	       (argKeywords[n + 6] != Min)))
	    errorCode = ARGUMENT_FOURTH_PART_BAD;
	  else
	    components = 8;
	}
      else
	errorCode = ARGUMENT_MUST_END_WITH_EMPTY_PARENTHESES;
      if (errorCode == OK)
	{
	  keys1[m] = (otherKeyType)argKeywords[n];
	  keys2[m] = (otherKeyType)argKeywords[n + 2];
	  if (components > 4)
	    {
	      keys3[m] = (otherKeyType)argKeywords[n + 4];
	      if (components > 6)
		{
		  keys4[m] = (otherKeyType)argKeywords[n + 6];
		  n = (n + 8);
		}
	      else
		{
		  keys4[m] = (otherKeyType)0;
		  n = (n + 6);
		}
	    }
	  else
	    {
	      keys3[m] = (otherKeyType)0;
	      keys4[m] = (otherKeyType)0;
	      n = (n + 4);
	    }
	  m++;
	  if ((n != argCount) && (argTypes[n] != ARGCOMMA))
	    errorCode = BAD_ARGUMENTS;
	}
    }
  returnMe = ((errorCode != OK) ? NULL :
	      (new GetPropCommand(tag, m, keys1, keys2, keys3, keys4)));
  delete keys1;
  delete keys2;
  delete keys3;
  delete keys4;
  return returnMe;
}

/*******************************************************************/

/* parserCmd::makeGetPropE

Returned Value: A GetPropECommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be at least one argument, and each argument must have one of
   the three patterns:
   key1.key2() -- four components in argCount
   key1.key2.key3() -- six components in argCount
   key1.key2.key3.key4() -- eight components in argCount
   and all arguments except the last must be followed by a comma.
   BAD_ARGUMENTS
   ARGUMENT_SECOND_PART_BAD
2. key1 must be FoundTool or Tool.
   ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL
3. If the pattern is key1.key2():
   If key1 is Tool, key2 must be Alignment, CollisionVolume, Name, or Id.
   If key1 is FoundTool, key2 must be Name, or Id.
   ARGUMENT_SECOND_PART_BAD
4. If key2 is GoToPar, key3 must be Speed or Accel.
   If key2 is PtMeasPar, key3 must be one of
   Speed, Accel, Approach, Retract, or Search.
   ARGUMENT_THIRD_PART_BAD
5. If there is a key4, it must be Act, Def, Max, or Min.
   ARGUMENT_FOURTH_PART_BAD
6. The dots must be placed where shown in rule 1.
   BAD_DOTS
7. The empty parentheses must be placed where shown in rule 1.
   ARGUMENT_MUST_END_WITH_EMPTY_PARENTHESES


Reference pages: 24 26 29 (45) 99 108
Also see reference pages for GetProp.

This is identical to parserCmd::makeGetProp except that GetProp is changed
to GetPropE (and GETPROP to GETPROPE).
See the documentation there.

*/

Command * parserCmd::makeGetPropE()
{
  otherKeyType * keys1;
  otherKeyType * keys2;
  otherKeyType * keys3;
  otherKeyType * keys4;
  int n;                // index for all components of command arguments
  int m;                // index for command arguments
  int components;       // number of components of an argument (4, 6, or 8)
  GetPropECommand * returnMe;

  keys1 = new otherKeyType[(argCount + 1) / 5];
  keys2 = new otherKeyType[(argCount + 1) / 5];
  keys3 = new otherKeyType[(argCount + 1) / 5];
  keys4 = new otherKeyType[(argCount + 1) / 5];
  m = 0;
  for (n = 0; ((n < argCount) && (errorCode == OK)); n++)
    {
      if (argCount < (n + 4))
	errorCode = BAD_ARGUMENTS;
      else if ((argTypes[n] != ARGKEYWORD) ||
	       ((argKeywords[n] != Tool) && (argKeywords[n] != FoundTool)))
	errorCode = ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL;
      else if (argTypes[n + 1] != ARGDOT)
	errorCode = BAD_DOTS;
      else if (argTypes[n + 2] != ARGKEYWORD)
	errorCode = ARGUMENT_SECOND_PART_BAD;
      else if (argTypes[n + 3] == ARGPARENEMPTY)
	{
	  if ((argKeywords[n] == FoundTool) &&
	      ((argKeywords[n + 2] == Alignment) || 
	       (argKeywords[n + 2] == CollisionVolume)))
	    errorCode = BAD_ARGUMENTS;
	  else if ((argKeywords[n + 2] != Alignment) &&
		   (argKeywords[n + 2] != CollisionVolume) &&
		   (argKeywords[n + 2] != Name) &&
		   (argKeywords[n + 2] != Id))
	    errorCode = ARGUMENT_SECOND_PART_BAD;
	  else
	    components = 4;
	}
      else if ((argKeywords[n + 2] != GoToPar) &&
	       (argKeywords[n + 2] != PtMeasPar))
	errorCode = ARGUMENT_SECOND_PART_BAD;
      else if (argTypes[n + 3] != ARGDOT)
	errorCode = BAD_DOTS;
      else if (argCount < (n + 6))
	errorCode = BAD_ARGUMENTS;
      else if ((argTypes[n + 4] != ARGKEYWORD) ||
	       ((argKeywords[n + 4] != Speed) && 
		(argKeywords[n + 4] != Accel) &&
		((argKeywords[n + 2] == GoToPar) ||
		 ((argKeywords[n + 4] != Approach) &&
		  (argKeywords[n + 4] != Retract) &&
		  (argKeywords[n + 4] != Search)))))
	errorCode = ARGUMENT_THIRD_PART_BAD;
      else if (argTypes[n + 5] ==  ARGPARENEMPTY)
	components = 6;
      else if (argTypes[n + 5] != ARGDOT)
	errorCode = BAD_DOTS;
      else if (argCount < (n + 8))
	errorCode = BAD_ARGUMENTS;
      else if (argTypes[n + 7] == ARGPARENEMPTY)
	{
	  if ((argTypes[n + 6] != ARGKEYWORD) ||
	      ((argKeywords[n + 6] != Actual) &&
	       (argKeywords[n + 6] != Default) &&
	       (argKeywords[n + 6] != Max) &&
	       (argKeywords[n + 6] != Min)))
	    errorCode = ARGUMENT_FOURTH_PART_BAD;
	  else
	    components = 8;
	}
      else
	errorCode = ARGUMENT_MUST_END_WITH_EMPTY_PARENTHESES;
      if (errorCode == OK)
	{
	  keys1[m] = (otherKeyType)argKeywords[n];
	  keys2[m] = (otherKeyType)argKeywords[n + 2];
	  if (components > 4)
	    {
	      keys3[m] = (otherKeyType)argKeywords[n + 4];
	      if (components > 6)
		{
		  keys4[m] = (otherKeyType)argKeywords[n + 6];
		  n = (n + 8);
		}
	      else
		{
		  keys4[m] = (otherKeyType)0;
		  n = (n + 6);
		}
	    }
	  else
	    {
	      keys3[m] = (otherKeyType)0;
	      keys4[m] = (otherKeyType)0;
	      n = (n + 4);
	    }
	  m++;
	  if ((n != argCount) && (argTypes[n] != ARGCOMMA))
	    errorCode = BAD_ARGUMENTS;
	}
    }
  returnMe = ((errorCode != OK) ? NULL :
	      (new GetPropECommand(tag, m, keys1, keys2, keys3, keys4)));
  delete keys1;
  delete keys2;
  delete keys3;
  delete keys4;
  return returnMe;
}

/*******************************************************************/

/* parserCmd::makeGetXtdErrStatus

Returned Value: A Command of commandNameType GetXtdErrStatus as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 27 (50) 102

*/

Command * parserCmd::makeGetXtdErrStatus()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, GetXtdErrStatus)));
}

/*******************************************************************/

/* parserCmd::makeGoTo

Returned Value: A GoToCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one to six arguments.
   BAD_ARGUMENTS
2. Each argument must be X(<number>), Y(<number>), Z(<number>),
   Tool.A(<number>), Tool.B(<number>) or Tool.C(<number>),
   which may appear in any order.
   BAD_ARGUMENTS
3. Each allowed argument must appear at most once.
   X_ARGUMENT_USED_TWICE
   Y_ARGUMENT_USED_TWICE
   Z_ARGUMENT_USED_TWICE
   TOOL_A_ARGUMENT_USED_TWICE
   TOOL_B_ARGUMENT_USED_TWICE
   TOOL_C_ARGUMENT_USED_TWICE
4. Commas must separate the arguments if there are two or more.
   BAD_ARGUMENTS

Reference pages: 24 27 [38] [43] (51 - 53) (58) (62) [71] [72] (78) (98) 102

This function is similar to makeGet.

*/

Command * parserCmd::makeGoTo()
{
  int n;
  bool hasX = false;
  bool hasY = false;
  bool hasZ = false;
  double x = 0;
  double y = 0;
  double z = 0;
  bool hasToolA = false;
  bool hasToolB = false;
  bool hasToolC = false;
  double toolA = 0;
  double toolB = 0;
  double toolC = 0;

  if (argCount < 2)
    errorCode = BAD_ARGUMENTS;
  for (n = 0; ((n < argCount) && (errorCode == OK)); )
    {
      if ((argTypes[n] != ARGKEYWORD) || ((n + 1) == argCount))
	errorCode = BAD_ARGUMENTS;
      else if ((argKeywords[n] == X) ||
	       (argKeywords[n] == Y) ||
	       (argKeywords[n] == Z))
	{
	  if ((argTypes[n + 1] != ARGPARENDOUBLE) ||
	      (((n + 2) < argCount) && (argTypes[n + 2] != ARGCOMMA)) ||
	      (((n + 2) < argCount) && ((n + 3) == argCount)))
	    errorCode = BAD_ARGUMENTS;
	  else if (argKeywords[n] == X)
	    {
	      if (hasX)
		errorCode = X_ARGUMENT_USED_TWICE;
	      else
		{
		  x = argDoubles[n + 1];
		  hasX = true;
		}
	    }
	  else if (argKeywords[n] == Y)
	    {
	      if (hasY)
		errorCode = Y_ARGUMENT_USED_TWICE;
	      else
		{
		  y = argDoubles[n + 1];
		  hasY = true;
		}
	    }
	  else if (argKeywords[n] == Z)
	    {
	      if (hasZ)
		errorCode = Z_ARGUMENT_USED_TWICE;
	      else
		{
		  z = argDoubles[n + 1];
		  hasZ = true;
		}
	    }
	  n = (n + 3);
	}
      else if (argKeywords[n] == Tool)
	{
	  if (((n + 3) >= argCount) ||
	      (argTypes[n + 1] != ARGDOT) ||
	      (argTypes[n + 2] != ARGKEYWORD) ||
	      (argTypes[n + 3] != ARGPARENDOUBLE) ||
	      (((n + 4) < argCount) && (argTypes[n + 4] != ARGCOMMA)) ||
	      (((n + 4) < argCount) && ((n + 5) == argCount)))
	    errorCode = BAD_ARGUMENTS;
	  else if (argKeywords[n + 2] == A)
	    {
	      if (hasToolA)
		errorCode = TOOL_A_ARGUMENT_USED_TWICE;
	      else
		{
		  toolA = argDoubles[n + 3];
		  hasToolA = true;
		}
	    }
	  else if (argKeywords[n + 2] == B)
	    {
	      if (hasToolB)
		errorCode = TOOL_B_ARGUMENT_USED_TWICE;
	      else
		{
		  toolB = argDoubles[n + 3];
		  hasToolB = true;
		}
	    }
	  else if (argKeywords[n + 2] == C)
	    {
	      if (hasToolC)
		errorCode = TOOL_C_ARGUMENT_USED_TWICE;
	      else
		{
		  toolC = argDoubles[n + 3];
		  hasToolC = true;
		}
	    }
	  n = (n + 5);
	}
      else
	errorCode = BAD_ARGUMENTS;
    }
  return ((errorCode != OK) ? NULL:
	  (new GoToCommand(tag, hasX, hasY, hasZ, x, y, z, hasToolA,
			   hasToolB, hasToolC, toolA, toolB, toolC)));
}

/*******************************************************************/

/* parserCmd::makeHome

Returned Value: A Command of commandNameType Home as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 27 [38] (48) 102

*/

Command * parserCmd::makeHome()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, Home)));
}

/*******************************************************************/

/* parserCmd::makeIsHomed

Returned Value: A Command of commandNameType IsHomed as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 27 (48) 102 111

*/

Command * parserCmd::makeIsHomed()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, IsHomed)));
}

/*******************************************************************/

/* parserCmd::makeIsUserEnabled

Returned Value: A Command of commandNameType IsUserEnabled as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 27 (49) 102

*/

Command * parserCmd::makeIsUserEnabled()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, IsUserEnabled)));
}

/*******************************************************************/

/* parserCmd::makeLockAxis

Returned Value: A LockAxisCommand as described by the command text,
  or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one to nine arguments.
   BAD_ARGUMENTS
2. Each argument must be C(), Phi(), R(), X(), Y(), Z(),
   which may appear in any order.
   BAD_ARGUMENTS
3. Each allowed argument must appear at most once.
   C_ARGUMENT_USED_TWICE
   PHI_ARGUMENT_USED_TWICE
   R_ARGUMENT_USED_TWICE
   X_ARGUMENT_USED_TWICE
   Y_ARGUMENT_USED_TWICE
   Z_ARGUMENT_USED_TWICE
4. Commas must separate the arguments if there are two or more.
   BAD_ARGUMENTS

Reference pages: 24 (97-98)

The spec does not forbid having no arguments, but here there must be
at least one argument.

It is not necessary to check for too many arguments, because if there
were too many, at least one would be duplicated, and a check for
duplicates is made.

*/

Command * parserCmd::makeLockAxis()
{
  int n; // index for components of command arguments
  bool hasC = false;
  bool hasPhi = false;
  bool hasR = false;
  bool hasX = false;
  bool hasY = false;
  bool hasZ = false;

  if (argCount < 2)
    errorCode = BAD_ARGUMENTS;
  for (n = 0; ((n < argCount) && (errorCode == OK)); n = (n + 3))
    {
      if ((argTypes[n] != ARGKEYWORD) ||
	  ((n + 1) == argCount) ||
	  (argTypes[n + 1] != ARGPARENEMPTY) ||
	  (((n + 2) < argCount) && (argTypes[n + 2] != ARGCOMMA)) ||
	  (((n + 2) < argCount) && ((n + 3) == argCount)))
	errorCode = BAD_ARGUMENTS;
      else if (argKeywords[n] == C)
	{
	  if (hasC)
	    errorCode = C_ARGUMENT_USED_TWICE;
	  else
	    hasC = true;
	}
      else if (argKeywords[n] == Phi)
	{
	  if (hasPhi)
	    errorCode = PHI_ARGUMENT_USED_TWICE;
	  else
	    hasPhi = true;
	}
      else if (argKeywords[n] == R)
	{
	  if (hasR)
	    errorCode = R_ARGUMENT_USED_TWICE;
	  else
	    hasR = true;
	}
      else if (argKeywords[n] == X)
	{
	  if (hasX)
	    errorCode = X_ARGUMENT_USED_TWICE;
	  else
	    hasX = true;
	}
      else if (argKeywords[n] == Y)
	{
	  if (hasY)
	    errorCode = Y_ARGUMENT_USED_TWICE;
	  else
	    hasY = true;
	}
      else if (argKeywords[n] == Z)
	{
	  if (hasZ)
	    errorCode = Z_ARGUMENT_USED_TWICE;
	  else
	    hasZ = true;
	}
      else
	errorCode = BAD_ARGUMENTS;
    }
  return ((errorCode != OK) ? NULL :
	  (new LockAxisCommand(tag, hasC, hasPhi, hasR, hasX, hasY, hasZ)));
}

/*******************************************************************/

/* parserCmd::makeOnMoveReportE

Returned Value: An OnMoveReportECommand as described by the command text,
  or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be zero to nine arguments.
   BAD_ARGUMENTS
2. Each argument must be one of R(), X(), Y(), Z(), Tool.A(), Tool.B(),
   Tool.C(), Dis(<number>), or Time(<number>), which may appear in any order.
   DIS_MUST_BE_FOLLOWED_BY_NUMBER_IN_PARENS
   TIME_MUST_BE_FOLLOWED_BY_NUMBER_IN_PARENS
3. Each allowed argument must appear at most once.
   R_ARGUMENT_USED_TWICE
   X_ARGUMENT_USED_TWICE
   Y_ARGUMENT_USED_TWICE
   Z_ARGUMENT_USED_TWICE
   TOOL_A_ARGUMENT_USED_TWICE
   TOOL_B_ARGUMENT_USED_TWICE
   TOOL_C_ARGUMENT_USED_TWICE
   DIS_ARGUMENT_USED_TWICE
   TIME_ARGUMENT_USED_TWICE
4. Commas must separate the arguments if there are two or more.
   BAD_ARGUMENTS

Reference pages: 19 24 27 [39] (49 - 50) 102
This is referenced as "OnReport" on pages 56, 57, 61, 63, and 66.

Much of this function is identical to the makeGet function. The differences
are that in this function it is OK to have no arguments, and in this
function Dis(<number>) and Time(<number>) are allowed as arguments.

It is not necessary to check for too many arguments, because if there
were too many, at least one would be duplicated, and a check for
duplicates is made.

*/

Command * parserCmd::makeOnMoveReportE()
{
  int n;
  bool hasR = false;
  bool hasX = false;
  bool hasY = false;
  bool hasZ = false;
  bool hasToolA = false;
  bool hasToolB = false;
  bool hasToolC = false;
  bool hasDis = false;
  bool hasTime = false;
  double theDis = 100.0;
  double theTime = 100.0;
  
  for (n = 0; ((n < argCount) && (errorCode == OK)); )
    {
      if ((argTypes[n] != ARGKEYWORD) || ((n + 1) == argCount))
	errorCode = BAD_ARGUMENTS;
      else if ((argKeywords[n] == R) ||
	       (argKeywords[n] == X) ||
	       (argKeywords[n] == Y) ||
	       (argKeywords[n] == Z))
	{
	  if ((argTypes[n + 1] != ARGPARENEMPTY) ||
	      (((n + 2) < argCount) && (argTypes[n + 2] != ARGCOMMA)) ||
	      (((n + 2) < argCount) && ((n + 3) == argCount)))
	    errorCode = BAD_ARGUMENTS;
	  else if (argKeywords[n] == R)
	    {
	      if (hasR)
		errorCode = R_ARGUMENT_USED_TWICE;
	      else
		hasR = true;
	    }
	  else if (argKeywords[n] == X)
	    {
	      if (hasX)
		errorCode = X_ARGUMENT_USED_TWICE;
	      else
		hasX = true;
	    }
	  else if (argKeywords[n] == Y)
	    {
	      if (hasY)
		errorCode = Y_ARGUMENT_USED_TWICE;
	      else
		hasY = true;
	    }
	  else if (argKeywords[n] == Z)
	    {
	      if (hasZ)
		errorCode = Z_ARGUMENT_USED_TWICE;
	      else
		hasZ = true;
	    }
	  n = (n + 3);
	}
      else if (argKeywords[n] == Tool)
	{
	  if (((n + 3) >= argCount) ||
	      (argTypes[n + 1] != ARGDOT) ||
	      (argTypes[n + 2] != ARGKEYWORD) ||
	      (argTypes[n + 3] != ARGPARENEMPTY) ||
	      (((n + 4) < argCount) && (argTypes[n + 4] != ARGCOMMA)) ||
	      (((n + 4) < argCount) && ((n + 5) == argCount)))
	    errorCode = BAD_ARGUMENTS;
	  else if (argKeywords[n + 2] == A)
	    {
	      if (hasToolA)
		errorCode = TOOL_A_ARGUMENT_USED_TWICE;
	      else
		hasToolA = true;
	    }
	  else if (argKeywords[n + 2] == B)
	    {
	      if (hasToolB)
		errorCode = TOOL_B_ARGUMENT_USED_TWICE;
	      else
		hasToolB = true;
	    }
	  else if (argKeywords[n + 2] == C)
	    {
	      if (hasToolC)
		errorCode = TOOL_C_ARGUMENT_USED_TWICE;
	      else
		hasToolC = true;
	    }
	  n = (n + 5);
	}
      else if (argKeywords[n] == Dis)
	{
	  if (argTypes[n + 1] != ARGPARENDOUBLE)
	    errorCode = DIS_MUST_BE_FOLLOWED_BY_NUMBER_IN_PARENS;
	  else if (hasDis)
	    errorCode = DIS_ARGUMENT_USED_TWICE;
	  theDis = argDoubles[n + 1];
	  hasDis = true;
	  n = (n + 3);
	}
      else if (argKeywords[n] == Time)
	{
	  if (argTypes[n + 1] != ARGPARENDOUBLE)
	    errorCode =TIME_MUST_BE_FOLLOWED_BY_NUMBER_IN_PARENS;
	  else if (hasTime)
	    errorCode = TIME_ARGUMENT_USED_TWICE;
	  theTime = argDoubles[n + 1];
	  hasTime = true;
	  n = (n + 3);
	}
      else
	errorCode = BAD_ARGUMENTS;
    }
  return ((errorCode != OK) ? NULL:
	  (new OnMoveReportECommand(tag, hasR, hasX, hasY, hasZ, hasToolA,
		       hasToolB, hasToolC, hasDis, hasTime, theDis, theTime)));
}

/*******************************************************************/

/* parserCmd::makeOnPtMeasReport

Returned Value: An OnPtMeasReportCommand as described by the command text,
  or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one to nine arguments.
   BAD_ARGUMENTS
2. Each argument must be ER(), IJK(), R(), X(), Y(), Z(), Tool.A(),
   Tool.B() or Tool.C(), which may appear in any order.
   BAD_ARGUMENTS
3. Each allowed argument must appear at most once.
   ER_ARGUMENT_USED_TWICE
   IJK_ARGUMENT_USED_TWICE
   R_ARGUMENT_USED_TWICE
   X_ARGUMENT_USED_TWICE
   Y_ARGUMENT_USED_TWICE
   Z_ARGUMENT_USED_TWICE
   TOOL_A_ARGUMENT_USED_TWICE
   TOOL_B_ARGUMENT_USED_TWICE
   TOOL_C_ARGUMENT_USED_TWICE
4. Commas must separate the arguments if there are two or more.
   BAD_ARGUMENTS

Reference pages: 24 27 (41) (49) (52) (53) 61 [71] [72] (78) 102
This is referenced as "OnReport" on pages 56, 57, 61, 63, and 66.

The body of the function is similar to the body of makeGet.
See documentation of makeGet.

The spec forbids having no arguments.

It is not necessary to check for too many arguments, because if there
were too many, at least one would be duplicated, and a check for
duplicates is made.

*/

Command * parserCmd::makeOnPtMeasReport()
{
  int n; // index for components of command arguments
  bool hasER = false;
  bool hasIJK = false;
  bool hasR = false;
  bool hasX = false;
  bool hasY = false;
  bool hasZ = false;
  bool hasToolA = false;
  bool hasToolB = false;
  bool hasToolC = false;

  if (argCount < 2)
    errorCode = BAD_ARGUMENTS;
  for (n = 0; ((n < argCount) && (errorCode == OK)); )
    {
      if ((argTypes[n] != ARGKEYWORD) || ((n + 1) == argCount))
	errorCode = BAD_ARGUMENTS;
      else if ((argKeywords[n] == ER)  ||
	       (argKeywords[n] == IJK) ||
	       (argKeywords[n] == R)   ||
	       (argKeywords[n] == X)   ||
	       (argKeywords[n] == Y)   ||
	       (argKeywords[n] == Z))
	{
	  if ((argTypes[n + 1] != ARGPARENEMPTY) ||
	      (((n + 2) < argCount) && (argTypes[n + 2] != ARGCOMMA)) ||
	      (((n + 2) < argCount) && ((n + 3) == argCount)))
	    errorCode = BAD_ARGUMENTS;
	  else if (argKeywords[n] == ER)
	    {
	      if (hasER)
		errorCode = ER_ARGUMENT_USED_TWICE;
	      else
		hasER = true;
	    }
	  else if (argKeywords[n] == IJK)
	    {
	      if (hasIJK)
		errorCode = IJK_ARGUMENT_USED_TWICE;
	      else
		hasIJK = true;
	    }
	  else if (argKeywords[n] == R)
	    {
	      if (hasR)
		errorCode = R_ARGUMENT_USED_TWICE;
	      else
		hasR = true;
	    }
	  else if (argKeywords[n] == X)
	    {
	      if (hasX)
		errorCode = X_ARGUMENT_USED_TWICE;
	      else
		hasX = true;
	    }
	  else if (argKeywords[n] == Y)
	    {
	      if (hasY)
		errorCode = Y_ARGUMENT_USED_TWICE;
	      else
		hasY = true;
	    }
	  else if (argKeywords[n] == Z)
	    {
	      if (hasZ)
		errorCode = Z_ARGUMENT_USED_TWICE;
	      else
		hasZ = true;
	    }
	  n = (n + 3);
	}
      else if (argKeywords[n] == Tool)
	{
	  if (((n + 3) >= argCount) ||
	      (argTypes[n + 1] != ARGDOT) ||
	      (argTypes[n + 2] != ARGKEYWORD) ||
	      (argTypes[n + 3] != ARGPARENEMPTY) ||
	      (((n + 4) < argCount) && (argTypes[n + 4] != ARGCOMMA)) ||
	      (((n + 4) < argCount) && ((n + 5) == argCount)))
	    errorCode = BAD_ARGUMENTS;
	  else if (argKeywords[n + 2] == A)
	    {
	      if (hasToolA)
		errorCode = TOOL_A_ARGUMENT_USED_TWICE;
	      else
		hasToolA = true;
	    }
	  else if (argKeywords[n + 2] == B)
	    {
	      if (hasToolB)
		errorCode = TOOL_B_ARGUMENT_USED_TWICE;
	      else
		hasToolB = true;
	    }
	  else if (argKeywords[n + 2] == C)
	    {
	      if (hasToolC)
		errorCode = TOOL_C_ARGUMENT_USED_TWICE;
	      else
		hasToolC = true;
	    }
	  n = (n + 5);
	}
      else
	errorCode = BAD_ARGUMENTS;
    }
  return ((errorCode != OK) ? NULL :
	  (new OnPtMeasReportCommand(tag, hasER, hasIJK, hasR, hasX, hasY,
				     hasZ,hasToolA, hasToolB, hasToolC)));
}

/*******************************************************************/

/* parserCmd::makeOnScanReport

Returned Value: An OnScanReportCommand as described by the command text,
  or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one to eight arguments.
   BAD_ARGUMENTS
2. Each argument must be Q(), R(), X(), Y(), or Z(), Tool.A(), Tool.B(),
   or Tool.C(), which may appear in any order.
   BAD_ARGUMENTS
3. Each allowed argument must appear at most once.
   ER_ARGUMENT_USED_TWICE
   IJK_ARGUMENT_USED_TWICE
   Q_ARGUMENT_USED_TWICE
   R_ARGUMENT_USED_TWICE
   X_ARGUMENT_USED_TWICE
   Y_ARGUMENT_USED_TWICE
   Z_ARGUMENT_USED_TWICE
   TOOL_A_ARGUMENT_USED_TWICE
   TOOL_B_ARGUMENT_USED_TWICE
   TOOL_C_ARGUMENT_USED_TWICE
4. Commas must separate the arguments if there are two or more.
   BAD_ARGUMENTS

Reference pages: 24 27 (41) (81) 82 83 84 86 87 88 90 91 [93] 103

The body of the function is similar to the body of makeOnPtMeasReport,
except that Q is added and the names of the error codes are
different.

It is not necessary to check for too many arguments, because if there
were too many, at least one would be duplicated, and a check for
duplicates is made.

Although the spec is not specific, this does not allow no arguments.

*/

Command * parserCmd::makeOnScanReport()
{
  int n; // index for components of command arguments
  bool hasER = false;
  bool hasIJK = false;
  bool hasQ = false;
  bool hasR = false;
  bool hasX = false;
  bool hasY = false;
  bool hasZ = false;
  bool hasToolA = false;
  bool hasToolB = false;
  bool hasToolC = false;

  if (argCount < 2)
    errorCode = BAD_ARGUMENTS;
  for (n = 0; ((n < argCount) && (errorCode == OK)); )
    {
      if ((argTypes[n] != ARGKEYWORD) || ((n + 1) == argCount))
	errorCode = BAD_ARGUMENTS;
      else if ((argKeywords[n] == ER)  ||
	       (argKeywords[n] == IJK) ||
	       (argKeywords[n] == Q)   ||
	       (argKeywords[n] == R)   ||
	       (argKeywords[n] == X)   ||
	       (argKeywords[n] == Y)   ||
	       (argKeywords[n] == Z))
	{
	  if ((argTypes[n + 1] != ARGPARENEMPTY) ||
	      (((n + 2) < argCount) && (argTypes[n + 2] != ARGCOMMA)) ||
	      (((n + 2) < argCount) && ((n + 3) == argCount)))
	    errorCode = BAD_ARGUMENTS;
	  else if (argKeywords[n] == ER)
	    {
	      if (hasER)
		errorCode = ER_ARGUMENT_USED_TWICE;
	      else
		hasER = true;
	    }
	  else if (argKeywords[n] == IJK)
	    {
	      if (hasIJK)
		errorCode = IJK_ARGUMENT_USED_TWICE;
	      else
		hasIJK = true;
	    }
	  else if (argKeywords[n] == Q)
	    {
	      if (hasQ)
		errorCode = Q_ARGUMENT_USED_TWICE;
	      else
		hasQ = true;
	    }
	  else if (argKeywords[n] == R)
	    {
	      if (hasR)
		errorCode = R_ARGUMENT_USED_TWICE;
	      else
		hasR = true;
	    }
	  else if (argKeywords[n] == X)
	    {
	      if (hasX)
		errorCode = X_ARGUMENT_USED_TWICE;
	      else
		hasX = true;
	    }
	  else if (argKeywords[n] == Y)
	    {
	      if (hasY)
		errorCode = Y_ARGUMENT_USED_TWICE;
	      else
		hasY = true;
	    }
	  else if (argKeywords[n] == Z)
	    {
	      if (hasZ)
		errorCode = Z_ARGUMENT_USED_TWICE;
	      else
		hasZ = true;
	    }
	  n = (n + 3);
	}
      else if (argKeywords[n] == Tool)
	{
	  if (((n + 3) >= argCount) ||
	      (argTypes[n + 1] != ARGDOT) ||
	      (argTypes[n + 2] != ARGKEYWORD) ||
	      (argTypes[n + 3] != ARGPARENEMPTY) ||
	      (((n + 4) < argCount) && (argTypes[n + 4] != ARGCOMMA)) ||
	      (((n + 4) < argCount) && ((n + 5) == argCount)))
	    errorCode = BAD_ARGUMENTS;
	  else if (argKeywords[n + 2] == A)
	    {
	      if (hasToolA)
		errorCode = TOOL_A_ARGUMENT_USED_TWICE;
	      else
		hasToolA = true;
	    }
	  else if (argKeywords[n + 2] == B)
	    {
	      if (hasToolB)
		errorCode = TOOL_B_ARGUMENT_USED_TWICE;
	      else
		hasToolB = true;
	    }
	  else if (argKeywords[n + 2] == C)
	    {
	      if (hasToolC)
		errorCode = TOOL_C_ARGUMENT_USED_TWICE;
	      else
		hasToolC = true;
	    }
	  n = (n + 5);
	}
      else
	errorCode = BAD_ARGUMENTS;
    }
  return ((errorCode != OK) ? NULL :
	  (new OnScanReportCommand(tag, hasER, hasIJK, hasQ, hasR, hasX, hasY,
				   hasZ, hasToolA, hasToolB, hasToolC)));
}

/*******************************************************************/

/* parserCmd::makePtMeas

Returned Value: A PtMeasCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one, two, three, or four arguments.
   BAD_ARGUMENTS
2. Each argument must be one of X(<number>), Y(<number>), Z(<number>),
   or IJK(<number>,<number>,<number>), which may appear in any order.
   X_NOT_FOLLOWED_BY_NUMBER_IN_PARENS
   Y_NOT_FOLLOWED_BY_NUMBER_IN_PARENS
   Z_NOT_FOLLOWED_BY_NUMBER_IN_PARENS
   IJK_NOT_FOLLOWED_BY_ARGIJK
3. At least one of X(), Y(), or Z() must appear.
   X_Y_Z_ALL_MISSING
4. Each allowed argument must appear at most once.
   X_ARGUMENT_USED_TWICE
   Y_ARGUMENT_USED_TWICE
   Z_ARGUMENT_USED_TWICE
   IJK_ARGUMENT_USED_TWICE
5. Commas must separate the arguments if there are two or more.
   BAD_ARGUMENTS

Reference pages: 9 24 27 (41) (49) (52 - 53) (62) [71] [72] (78) 83 84 86
                 87 89 90 91 102

*/

Command * parserCmd::makePtMeas()
{
  int n;
  bool hasX = false;
  bool hasY = false;
  bool hasZ = false;
  bool hasIJK = false;
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
  
  if ((argCount != 2)  && (argCount != 5) &&
      (argCount != 8) && (argCount != 11))
    errorCode = BAD_ARGUMENTS;
  for (n = 0; ((n < argCount) && (errorCode == OK)); n = (n + 3))
    {
      if ((argTypes[n] != ARGKEYWORD) ||
	  (((n + 2) < argCount) && (argTypes[n + 2] != ARGCOMMA)))
	errorCode = BAD_ARGUMENTS;
      else if (argKeywords[n] == X)
	{
	  if (argTypes[n + 1] != ARGPARENDOUBLE)
	    errorCode = X_NOT_FOLLOWED_BY_NUMBER_IN_PARENS;
	  else if (hasX)
	    errorCode = X_ARGUMENT_USED_TWICE;
	  hasX = true;
	  x = argDoubles[n + 1];
	}
      else if (argKeywords[n] == Y)
	{
	  if (argTypes[n + 1] != ARGPARENDOUBLE)
	    errorCode = Y_NOT_FOLLOWED_BY_NUMBER_IN_PARENS;
	  else if (hasY)
	    errorCode = Y_ARGUMENT_USED_TWICE;
	  hasY = true;
	  y = argDoubles[n + 1];
	}
      else if (argKeywords[n] == Z)
	{
	  if (argTypes[n + 1] != ARGPARENDOUBLE)
	    errorCode = Z_NOT_FOLLOWED_BY_NUMBER_IN_PARENS;
	  else if (hasZ)
	    errorCode = Z_ARGUMENT_USED_TWICE;
	  hasZ = true;
	  z = argDoubles[n + 1];
	}
      else if (argKeywords[n] == IJK)
	{
	  if (argTypes[n + 1] != ARGIJK)
	    errorCode = IJK_NOT_FOLLOWED_BY_ARGIJK;
	  else if (hasIJK)
	    errorCode = IJK_ARGUMENT_USED_TWICE;
	  hasIJK = true;
	}
    }
  if ((errorCode == OK)&&(hasX == false) && (hasY == false) && (hasZ == false))
    errorCode = X_Y_Z_ALL_MISSING;
  return ((errorCode != OK) ? NULL:
	  (new PtMeasCommand(tag, hasX, hasY, hasZ, hasIJK, x, y, z,
			     argIJK[0], argIJK[1], argIJK[2])));
}

/*******************************************************************/

/* parserCmd::makeReQualify

Returned Value: A Command of commandNameType ReQualify as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 29 (64) 106 107
[This is called Qualify on pages 29 106 107]

*/

Command * parserCmd::makeReQualify()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, ReQualify)));
}

/*******************************************************************/

/* parserCmd::makeScanInCylEndIsPlane

Returned Value: A ScanInCylEndIsPlaneCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 26 arguments.
   BAD_ARGUMENTS
2. All of the arguments must be numbers.
   BAD_ARGUMENTS
3. The number of throughs must be a positive integer.
   NUMBER_THROUGH_MUST_BE_POSITIVE
   NUMBER_THROUGH_MUST_BE_INTEGER
4. StepW must be positive.
   STEPW_MUST_BE_POSITIVE
5. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (90 - 91) 103

*/

Command * parserCmd::makeScanInCylEndIsPlane()
{
  int n;
  
  if (argCount != 51)
    errorCode = BAD_ARGUMENTS;
  else if ((argTypes[30] != ARGDOUBLE) ||
	   (argTypes[44] != ARGDOUBLE) ||
	   (argTypes[50] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  else if (argDoubles[30] <= 0)
    errorCode = STEPW_MUST_BE_POSITIVE;
  else if (argDoubles[44] <= 0)
    errorCode = NUMBER_THROUGH_MUST_BE_POSITIVE;
  else if ((argDoubles[44] - (int)argDoubles[44]) != 0)
    errorCode = NUMBER_THROUGH_MUST_BE_INTEGER;
  else
    {
      for (n = 0; ((n < 25) && (errorCode == OK)); n++)
	{
	  if ((argTypes[2*n] != ARGDOUBLE) || (argTypes[(2*n)+1] != ARGCOMMA))
	    errorCode = BAD_ARGUMENTS;
	}
    }
  return ((errorCode != OK) ? NULL:
	  (new ScanInCylEndIsPlaneCommand
	   (tag,
	    argDoubles[0],  argDoubles[2],  argDoubles[4],  argDoubles[6],
	    argDoubles[8],  argDoubles[10], argDoubles[12], argDoubles[14],
	    argDoubles[16], argDoubles[18], argDoubles[20], argDoubles[22],
	    argDoubles[24], argDoubles[26], argDoubles[28], argDoubles[30],
	    argDoubles[32], argDoubles[34], argDoubles[36], argDoubles[38],
	    argDoubles[40], argDoubles[42], (int)argDoubles[44],
	    argDoubles[46], argDoubles[48], argDoubles[50])));
}

/*******************************************************************/

/* parserCmd::makeScanInCylEndIsSphere

Returned Value: A ScanInCylEndIsSphereCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 23 arguments.
   BAD_ARGUMENTS
2. All of the arguments must be numbers.
   BAD_ARGUMENTS
3. The diameter of the sphere must be positive.
   DIAMETER_MUST_BE_POSITIVE
4. StepW must be positive.
   STEPW_MUST_BE_POSITIVE
5. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (89 - 90) 103

*/

Command * parserCmd::makeScanInCylEndIsSphere()
{
  int n;
  
  if (argCount != 45)
    errorCode = BAD_ARGUMENTS;
  else if ((argTypes[30] != ARGDOUBLE) ||
	   (argTypes[38] != ARGDOUBLE) ||
	   (argTypes[44] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  else if (argDoubles[30] <= 0)
    errorCode = STEPW_MUST_BE_POSITIVE;
  else if (argDoubles[38] <= 0)
    errorCode = DIAMETER_MUST_BE_POSITIVE;
  else
    {
      for (n = 0; ((n < 22) && (errorCode == OK)); n++)
	{
	  if ((argTypes[2*n] != ARGDOUBLE) || (argTypes[(2*n)+1] != ARGCOMMA))
	    errorCode = BAD_ARGUMENTS;
	}
    }
  return ((errorCode != OK) ? NULL:
	  (new ScanInCylEndIsSphereCommand
	   (tag,
	    argDoubles[0],  argDoubles[2],  argDoubles[4],  argDoubles[6],
	    argDoubles[8],  argDoubles[10], argDoubles[12], argDoubles[14],
	    argDoubles[16], argDoubles[18], argDoubles[20], argDoubles[22],
	    argDoubles[24], argDoubles[26], argDoubles[28], argDoubles[30],
	    argDoubles[32], argDoubles[34], argDoubles[36], argDoubles[38],
	    argDoubles[40], argDoubles[42], argDoubles[44])));
}

/*******************************************************************/

/* parserCmd::makeScanInPlaneEndIsCyl

Returned Value: A ScanInPlaneEndIsCylCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 21 arguments.
   BAD_ARGUMENTS
2. All of the arguments must be numbers.
   BAD_ARGUMENTS
3. The number of throughs must be a positive integer.
   NUMBER_THROUGH_MUST_BE_POSITIVE
   NUMBER_THROUGH_MUST_BE_INTEGER
4. StepW must be positive.
   STEPW_MUST_BE_POSITIVE
5. The diameter of the cylinder must be positive.
   DIAMETER_MUST_BE_POSITIVE
6. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (88 - 89) 103

*/

Command * parserCmd::makeScanInPlaneEndIsCyl()
{
  int n;
  
  if (argCount != 41)
    errorCode = BAD_ARGUMENTS;
  else if ((argTypes[18] != ARGDOUBLE) ||
	   (argTypes[32] != ARGDOUBLE) ||
	   (argTypes[34] != ARGDOUBLE) ||
	   (argTypes[40] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  else if (argDoubles[18] <= 0)
    errorCode = STEPW_MUST_BE_POSITIVE;
  else if (argDoubles[32] <= 0)
    errorCode = DIAMETER_MUST_BE_POSITIVE;
  else if (argDoubles[34] <= 0)
    errorCode = NUMBER_THROUGH_MUST_BE_POSITIVE;
  else if ((argDoubles[34] - (int)argDoubles[34]) != 0)
    errorCode = NUMBER_THROUGH_MUST_BE_INTEGER;
  else
    {
      for (n = 0; ((n < 20) && (errorCode == OK)); n++)
	{
	  if ((argTypes[2*n] != ARGDOUBLE) || (argTypes[(2*n)+1] != ARGCOMMA))
	    errorCode = BAD_ARGUMENTS;
	}
    }
  return ((errorCode != OK) ? NULL:
	  (new ScanInPlaneEndIsCylCommand
	   (tag,
	    argDoubles[0],  argDoubles[2],  argDoubles[4],  argDoubles[6],
	    argDoubles[8],  argDoubles[10], argDoubles[12], argDoubles[14],
	    argDoubles[16], argDoubles[18], argDoubles[20], argDoubles[22],
	    argDoubles[24], argDoubles[26], argDoubles[28], argDoubles[30],
	    argDoubles[32], (int) argDoubles[34],
	    argDoubles[36], argDoubles[38], argDoubles[40])));
}

/*******************************************************************/

/* parserCmd::makeScanInPlaneEndIsPlane

Returned Value: A ScanInPlaneEndIsPlaneCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 20 arguments.
   BAD_ARGUMENTS
2. All of the arguments must be numbers.
   BAD_ARGUMENTS
3. The number of throughs must be a positive integer.
   NUMBER_THROUGH_MUST_BE_POSITIVE
   NUMBER_THROUGH_MUST_BE_INTEGER
4. StepW must be positive.
   STEPW_MUST_BE_POSITIVE
5. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (86 - 87) 103

*/

Command * parserCmd::makeScanInPlaneEndIsPlane()
{
  int n;
  
  if (argCount != 39)
    errorCode = BAD_ARGUMENTS;
  else if ((argTypes[18] != ARGDOUBLE) ||
	   (argTypes[32] != ARGDOUBLE) ||
	   (argTypes[38] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  else if (argDoubles[18] <= 0)
    errorCode = STEPW_MUST_BE_POSITIVE;
  else if (argDoubles[32] <= 0)
    errorCode = NUMBER_THROUGH_MUST_BE_POSITIVE;
  else if ((argDoubles[32] - (int)argDoubles[32]) != 0)
    errorCode = NUMBER_THROUGH_MUST_BE_INTEGER;
  else
    {
      for (n = 0; ((n < 19) && (errorCode == OK)); n++)
	{
	  if ((argTypes[2*n] != ARGDOUBLE) || (argTypes[(2*n)+1] != ARGCOMMA))
	    errorCode = BAD_ARGUMENTS;
	}
    }
  return ((errorCode != OK) ? NULL:
	  (new ScanInPlaneEndIsPlaneCommand
	   (tag,
	    argDoubles[0],  argDoubles[2],  argDoubles[4],  argDoubles[6],
	    argDoubles[8],  argDoubles[10], argDoubles[12], argDoubles[14],
	    argDoubles[16], argDoubles[18], argDoubles[20], argDoubles[22],
	    argDoubles[24], argDoubles[26], argDoubles[28], argDoubles[30],
	    (int)argDoubles[32],
	    argDoubles[34], argDoubles[36], argDoubles[38])));
}

/*******************************************************************/

/* parserCmd::makeScanInPlaneEndIsSphere

Returned Value: A ScanInPlaneEndIsSphereCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 17 arguments.
   BAD_ARGUMENTS
2. All of the arguments must be numbers.
   BAD_ARGUMENTS
3. StepW must be positive.
   STEPW_MUST_BE_POSITIVE
4. The diameter of the sphere must be positive.
   DIAMETER_MUST_BE_POSITIVE
5. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (85 - 86) [94] 103

*/

Command * parserCmd::makeScanInPlaneEndIsSphere()
{
  int n;
  
  if (argCount != 33)
    errorCode = BAD_ARGUMENTS;
  else if ((argTypes[18] != ARGDOUBLE) ||
	   (argTypes[26] != ARGDOUBLE) ||
	   (argTypes[32] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  else if (argDoubles[18] <= 0)
    errorCode = STEPW_MUST_BE_POSITIVE;
  else if (argDoubles[26] <= 0)
    errorCode = DIAMETER_MUST_BE_POSITIVE;
  else
    {
      for (n = 0; ((n < 16) && (errorCode == OK)); n++)
	{
	  if ((argTypes[2*n] != ARGDOUBLE) || (argTypes[(2*n)+1] != ARGCOMMA))
	    errorCode = BAD_ARGUMENTS;
	}
    }
  return ((errorCode != OK) ? NULL:
	  (new ScanInPlaneEndIsSphereCommand
	   (tag,
	    argDoubles[0],  argDoubles[2],  argDoubles[4],  argDoubles[6],
	    argDoubles[8],  argDoubles[10], argDoubles[12], argDoubles[14],
	    argDoubles[16], argDoubles[18], argDoubles[20], argDoubles[22],
	    argDoubles[24], argDoubles[26], argDoubles[28], argDoubles[30],
	    argDoubles[32])));
}

/*******************************************************************/

/* parserCmd::makeScanOnCircle

Returned Value: A ScanOnCircleCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 12 arguments.
   BAD_ARGUMENTS
2. All of the arguments must be numbers.
   BAD_ARGUMENTS
3. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (82 - 83) [93] 103

It would be good to add a rule requiring StepW to have the same sign as
delta.

*/

Command * parserCmd::makeScanOnCircle()
{
  int n;
  
  if (argCount != 23)
    errorCode = BAD_ARGUMENTS;
  else if (argTypes[22] != ARGDOUBLE)
    errorCode = BAD_ARGUMENTS;
  else
    {
      for (n = 0; ((n < 11) && (errorCode == OK)); n++)
	{
	  if ((argTypes[2*n] != ARGDOUBLE) || (argTypes[(2*n)+1] != ARGCOMMA))
	    errorCode = BAD_ARGUMENTS;
	}
    }
  return ((errorCode != OK) ? NULL:
	  (new ScanOnCircleCommand
	   (tag,
	    argDoubles[0],  argDoubles[2],  argDoubles[4],  argDoubles[6],
	    argDoubles[8],  argDoubles[10], argDoubles[12], argDoubles[14],
	    argDoubles[16], argDoubles[18], argDoubles[20], argDoubles[22])));
}

/*******************************************************************/

/* parserCmd::makeScanOnCircleHint

Returned Value: A ScanOnCircleHintCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be two arguments.
   BAD_ARGUMENTS
2. Both arguments must be non-negative numbers.
   BAD_ARGUMENTS
3. A comma must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (82) [93] 103

*/

Command * parserCmd::makeScanOnCircleHint()
{
  if ((argCount != 3) || (argTypes[0] != ARGDOUBLE) ||
      (argTypes[1] != ARGCOMMA) || (argTypes[2] != ARGDOUBLE) ||
      (argDoubles[0] < 0) || (argDoubles[2] < 0))
    errorCode = BAD_ARGUMENTS;
  return ((errorCode != OK) ? NULL:
	  (new ScanOnCircleHintCommand(tag, argDoubles[0], argDoubles[2])));
}

/*******************************************************************/

/* parserCmd::makeScanOnLine

Returned Value: A ScanOnLineCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 10 arguments.
   BAD_ARGUMENTS
2. All of the arguments must be numbers.
   BAD_ARGUMENTS
3. StepW must be positive.
   STEPW_MUST_BE_POSITIVE
4. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (83 - 84) 103

*/

Command * parserCmd::makeScanOnLine()
{
  int n;
  
  if (argCount != 19)
    errorCode = BAD_ARGUMENTS;
  else if (argTypes[18] != ARGDOUBLE)
    errorCode = BAD_ARGUMENTS;
  else if (argDoubles[18] <= 0)
    errorCode = STEPW_MUST_BE_POSITIVE;
  else
    {
      for (n = 0; ((n < 9) && (errorCode == OK)); n++)
	{
	  if ((argTypes[2*n] != ARGDOUBLE) || (argTypes[(2*n)+1] != ARGCOMMA))
	    errorCode = BAD_ARGUMENTS;
	}
    }
  return ((errorCode != OK) ? NULL:
	  (new ScanOnLineCommand
	   (tag,
	    argDoubles[0],  argDoubles[2],  argDoubles[4],  argDoubles[6],
	    argDoubles[8],  argDoubles[10], argDoubles[12], argDoubles[14],
	    argDoubles[16], argDoubles[18])));
}

/*******************************************************************/

/* parserCmd::makeScanOnLineHint

Returned Value: A ScanOnLineHintCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be two arguments separated by a comma, and both arguments
   must be non-negative numbers.
   BAD_ARGUMENTS

Reference pages: 24 27 (83) 103

*/

Command * parserCmd::makeScanOnLineHint()
{
  if ((argCount != 3) || (argTypes[0] != ARGDOUBLE) ||
      (argTypes[1] != ARGCOMMA) || (argTypes[2] != ARGDOUBLE) ||
      (argDoubles[0] < 0) || (argDoubles[2] < 0))
    errorCode = BAD_ARGUMENTS;
  return ((errorCode != OK) ? NULL:
	  (new ScanOnLineHintCommand(tag, argDoubles[0], argDoubles[2])));
}

/*******************************************************************/

/* parserCmd::makeScanUnknownHint

Returned Value: A ScanUnknownHintCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   MUST_HAVE_ONE_ARGUMENT
2. The argument must be a number.
   ARGUMENT_MUST_BE_A_NUMBER
3. The number must be positive.
   ARGUMENT_MUST_BE_POSITIVE

Reference pages: 24 27 (85) [93] 103

*/

Command * parserCmd::makeScanUnknownHint()
{
  if (argCount != 1)
    errorCode = MUST_HAVE_ONE_ARGUMENT;
  else if (argTypes[0] != ARGDOUBLE)
    errorCode = ARGUMENT_MUST_BE_A_NUMBER;
  else if (argDoubles[0] <= 0)
    errorCode = ARGUMENT_MUST_BE_POSITIVE;
  return ((errorCode != OK) ? NULL:
	  (new ScanUnknownHintCommand(tag, argDoubles[0])));
}

/*******************************************************************/

/* parserCmd::makeSetCoordSystem

Returned Value: A SetCoordSystemCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   MUST_HAVE_ONE_ARGUMENT
2. The argument must be one of:
   MachineCsy, MultipleArmCsy, PartCsy, or MoveableMachineCsy.
   BAD_ARGUMENTS

Reference pages: 24 28 58 (59) (67) [71] [72] 103

These rules are as given in the spec. Note that JogDisplayCsy, JogMoveCsy,
and SensorCsy are not included. It is not clear whether omitting these
is intentional or inadvertent.

*/

Command * parserCmd::makeSetCoordSystem()
{
  if (argCount != 1)
    errorCode = MUST_HAVE_ONE_ARGUMENT;
  else if ((argTypes[0] != ARGKEYWORD) ||
	   ((argKeywords[0] != MachineCsy) &&
	    (argKeywords[0] != MultipleArmCsy) &&
	    (argKeywords[0] != PartCsy) &&
	    (argKeywords[0] != MoveableMachineCsy)))
    errorCode = BAD_ARGUMENTS;
  return ((errorCode != OK) ? NULL :
	  (new SetCoordSystemCommand(tag, (coordSysType)argKeywords[0])));
}

/*******************************************************************/

/* parserCmd::makeSetCsyTransformation

Returned Value: A SetCsyTransformationCommand as described by the command
  text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be seven arguments.
   MUST_HAVE_7_ARGUMENTS
2. The first argument must be JogDisplayCsy, JogMoveCsy, MoveableMachineCsy,
   MultipleArmCsy, PartCsy, or SensorCsy.
   BAD_FIRST_ARGUMENT
3. All the other arguments must be numbers.
   BAD_ARGUMENTS
4. Theta, the fourth numeric argument, must be in the range 0 to 180.
   THETA_OUT_OF_RANGE
5. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 28 59 (60) 67 [71] [72] (80) 103

*/

Command * parserCmd::makeSetCsyTransformation()
{
  if (argCount != 13)
    errorCode = MUST_HAVE_7_ARGUMENTS;
  else if ((argTypes[0]  != ARGKEYWORD) ||
	   (argTypes[1]  != ARGCOMMA) ||
	   (argTypes[2]  != ARGDOUBLE) ||
	   (argTypes[3]  != ARGCOMMA) ||
	   (argTypes[4]  != ARGDOUBLE) ||
	   (argTypes[5]  != ARGCOMMA) ||
	   (argTypes[6]  != ARGDOUBLE) ||
	   (argTypes[7]  != ARGCOMMA) ||
	   (argTypes[8]  != ARGDOUBLE) ||
	   (argTypes[9]  != ARGCOMMA) ||
	   (argTypes[10] != ARGDOUBLE) ||
	   (argTypes[11] != ARGCOMMA) ||
	   (argTypes[12] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  else if ((argKeywords[0] != JogDisplayCsy) &&
	   (argKeywords[0] != JogMoveCsy) &&
	   (argKeywords[0] != MoveableMachineCsy) &&
	   (argKeywords[0] != MultipleArmCsy) &&
	   (argKeywords[0] != PartCsy) &&
	   (argKeywords[0] != SensorCsy))
    errorCode = BAD_FIRST_ARGUMENT;
  else if ((argDoubles[8] < 0) || (argDoubles[8] > 180))
    errorCode = THETA_OUT_OF_RANGE;
  return ((errorCode != OK) ? NULL :
	  (new SetCsyTransformationCommand(tag, (coordSysType)argKeywords[0],
					   argDoubles[2],  argDoubles[4],
					   argDoubles[6],  argDoubles[8],
					   argDoubles[10], argDoubles[12])));
}

/*******************************************************************/

/* parserCmd::makeSetProp

Returned Value: A SetPropCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be at least one argument, and each argument must have one of
   the two patterns:
   key1.key2.key3(val) -- six components in argCount
   key1.key2.key3.key4(val) -- eight components in argCount
   and all arguments except the last must be followed by a comma.
   BAD_ARGUMENTS
2. key1 must be FoundTool or Tool.
   ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL
3. key2 must be GoToPar or PtMeasPar.
   ARGUMENT_SECOND_PART_MUST_BE_GOTOPAR_OR_PTMEASPAR
4. If key2 is GoToPar, key3 must be either Speed or Accel. If key2 is
   PtMeasPar, key3 must be one of Speed, Accel, Approach, Retract, or Search.
   ARGUMENT_THIRD_PART_BAD
5. If there is a key4, it must be Actual.
   ARGUMENT_FOURTH_PART_MUST_BE_ACT
6. The dots must be placed where shown in rule 1.
   BAD_DOTS
7. The number in parentheses must be placed where shown in rule 1.
   ARGUMENT_MUST_END_WITH_NUMBER_IN_PARENTHESES

Reference pages: 24 26 29 (45) 65 [73] 77 (78) 99 108
Also see the reference pages for GetProp, since several of those imply
that certain properties cannot be set directly.

The spec is not explicit about what is allowed to be set using SetProp
and what is not. For the most part, we believe this function does what
the spec intends. For PtMeasPars, the spec (page 65) states that only the
Act value can be set by the client, so here, key4 must be ACT if it is
used. The spec does not explicitly rule out setting DEF for GoToPars, but
it appears by implication not to be allowed, so it is not allowed here.

The [(argCount + 1) / 7] size for the keys arrays and the values array
is used because this is the smallest number that is sure to be at
least as large as the number of arguments. Each argument must have at
least six components, and the arguments are separated by commas (which
are added into argCount).

*/

Command * parserCmd::makeSetProp()
{
  otherKeyType * keys1;
  otherKeyType * keys2;
  otherKeyType * keys3;
  otherKeyType * keys4;
  double * values;
  int n;                // index for all components of command arguments
  int m;                // index for command arguments
  int components;       // number of components of an argument (4, 6, or 8)
  SetPropCommand * returnMe;

  keys1 = new otherKeyType[(argCount + 1) / 7];
  keys2 = new otherKeyType[(argCount + 1) / 7];
  keys3 = new otherKeyType[(argCount + 1) / 7];
  keys4 = new otherKeyType[(argCount + 1) / 7];
  values = new double[(argCount + 1) / 7];
  m = 0;
  for (n = 0; ((n < argCount) && (errorCode == OK)); n++)
    {
      if (argCount < (n + 6))
	errorCode = BAD_ARGUMENTS;
      else if ((argTypes[n] != ARGKEYWORD) ||
	       ((argKeywords[n] != Tool) && (argKeywords[n] != FoundTool)))
	errorCode = ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL;
      else if ((argTypes[n + 1] != ARGDOT) || (argTypes[n + 3] != ARGDOT))
	errorCode = BAD_DOTS;
      else if ((argTypes[n + 2] != ARGKEYWORD) ||
	       ((argKeywords[n + 2] != GoToPar) &&
		(argKeywords[n + 2] != PtMeasPar)))
	errorCode = ARGUMENT_SECOND_PART_MUST_BE_GOTOPAR_OR_PTMEASPAR;
      else if ((argTypes[n + 4] != ARGKEYWORD) ||
	       ((argKeywords[n + 4] != Speed) && 
		(argKeywords[n + 4] != Accel) &&
		((argKeywords[n + 2] == GoToPar) ||
		 ((argKeywords[n + 4] != Approach) &&
		  (argKeywords[n + 4] != Retract) &&
		  (argKeywords[n + 4] != Search)))))
	errorCode = ARGUMENT_THIRD_PART_BAD;
      else if (argTypes[n + 5] ==  ARGPARENDOUBLE)
	components = 6;
      else if (argTypes[n + 5] != ARGDOT)
	errorCode = BAD_DOTS;
      else if (argCount < (n + 8))
	errorCode = BAD_ARGUMENTS;
      else if (argTypes[n + 7] == ARGPARENDOUBLE)
	{
	  if ((argTypes[n + 6] != ARGKEYWORD) ||
	      (argKeywords[n + 6] != Actual))
	    errorCode = ARGUMENT_FOURTH_PART_MUST_BE_ACT;
	  else
	    components = 8;
	}
      else
	errorCode = ARGUMENT_MUST_END_WITH_NUMBER_IN_PARENTHESES;
      if (errorCode == OK)
	{
	  keys1[m] = (otherKeyType)argKeywords[n];
	  keys2[m] = (otherKeyType)argKeywords[n + 2];
	  keys3[m] = (otherKeyType)argKeywords[n + 4];
	  if (components > 6)
	    {
	      keys4[m] = (otherKeyType)argKeywords[n + 6];
	      values[m] = argDoubles[n + 7];
	      n = (n + 8);
	    }
	  else
	    {
	      keys4[m] = (otherKeyType)0;
	      values[m] = argDoubles[n + 5];
	      n = (n + 6);
	    }
	  m++;
	  if ((n != argCount) && (argTypes[n] != ARGCOMMA))
	    errorCode = BAD_ARGUMENTS;
	}
    }
  returnMe = ((errorCode != OK) ? NULL :
	      (new SetPropCommand(tag, m, keys1, keys2, keys3, keys4, values)));
  delete keys1;
  delete keys2;
  delete keys3;
  delete keys4;
  delete values;
  return returnMe;
}

/*******************************************************************/

/* parserCmd::makeSetTool

Returned Value: A SetToolCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   MUST_HAVE_ONE_ARGUMENT
2. The argument must be a string.
   ARGUMENT_MUST_BE_STRING

Reference pages: 24 27 29 (54 - 55) 102 106

*/

Command * parserCmd::makeSetTool()
{
  if (argCount != 1)
    errorCode = MUST_HAVE_ONE_ARGUMENT;
  else if (argTypes[0] != ARGSTRING)
    errorCode = ARGUMENT_MUST_BE_STRING;
  return((errorCode != OK) ? NULL: (new SetToolCommand(tag, argStrings[0])));
}

/*******************************************************************/

/* parserCmd::makeStartSession

Returned Value: A Command of commandNameType StartSession as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 18 20 24 26 (38) (41) 49 [71] (77) 81 99

*/

Command * parserCmd::makeStartSession()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, StartSession)));
}

/*******************************************************************/

/* parserCmd::makeStopAllDaemons

Returned Value: A Command of commandNameType StopAllDaemons as described by
  the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be no arguments.
   MUST_HAVE_NO_ARGUMENTS

Reference pages: 24 26 (42) (50) 99

*/

Command * parserCmd::makeStopAllDaemons()
{
  if (argCount != 0)
    errorCode = MUST_HAVE_NO_ARGUMENTS;
  return ((errorCode != OK) ? NULL : (new Command(tag, StopAllDaemons)));
}

/*******************************************************************/

/* parserCmd::makeStopDaemon

Returned Value: A StopDaemonCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be one argument.
   MUST_HAVE_ONE_ARGUMENT
2. The argument must be an event tag.
   ARGUMENT_MUST_BE_EVENT_TAG

Reference pages: 19 (22) 24 26 [40] (42) (50) 99

*/

Command * parserCmd::makeStopDaemon()
{
  if (argCount != 1)
    errorCode = MUST_HAVE_ONE_ARGUMENT;
  else if (argTypes[0] != ARGETAG)
    errorCode = ARGUMENT_MUST_BE_EVENT_TAG;
  return((errorCode != OK) ? NULL: (new StopDaemonCommand(tag, argInt)));
}

/*******************************************************************/

/* parserCmd::makeTiltCenterPart

Returned Value: A TiltCenterPartCommand as described by the command text,
                or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 7 arguments.
   MUST_HAVE_7_ARGUMENTS
2. The arguments must all be numbers.
   BAD_ARGUMENTS
3. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (97) 102

*/

Command * parserCmd::makeTiltCenterPart()
{
  if (argCount != 13)
    errorCode = MUST_HAVE_7_ARGUMENTS;
  else if ((argTypes[0]  != ARGDOUBLE) ||
	   (argTypes[1]  != ARGCOMMA)  ||
	   (argTypes[2]  != ARGDOUBLE) ||
	   (argTypes[3]  != ARGCOMMA)  ||
	   (argTypes[4]  != ARGDOUBLE) ||
	   (argTypes[5]  != ARGCOMMA)  ||
	   (argTypes[6]  != ARGDOUBLE) ||
	   (argTypes[7]  != ARGCOMMA)  ||
	   (argTypes[8]  != ARGDOUBLE) ||
	   (argTypes[9]  != ARGCOMMA)  ||
	   (argTypes[10] != ARGDOUBLE) ||
	   (argTypes[11] != ARGCOMMA)  ||
	   (argTypes[12] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  return ((errorCode != OK) ? NULL:
	  new TiltCenterPartCommand(tag, argDoubles[0], argDoubles[2],
				    argDoubles[4], argDoubles[6],
				    argDoubles[8], argDoubles[10],
				    argDoubles[12]));
}

/*******************************************************************/

/* parserCmd::makeTiltPart

Returned Value: A TiltPartCommand as described by the command text, or NULL.

Called By: parserCmd::makeCommand

Rules:
1. There must be 4 arguments.
   MUST_HAVE_4_ARGUMENTS
2. The arguments must all be numbers.
   BAD_ARGUMENTS
3. Commas must separate the arguments.
   BAD_ARGUMENTS

Reference pages: 24 27 (97) 102

*/

Command * parserCmd::makeTiltPart()
{
  if (argCount != 7)
    errorCode = MUST_HAVE_4_ARGUMENTS;
  else if ((argTypes[0] != ARGDOUBLE) ||
	   (argTypes[1] != ARGCOMMA)  ||
	   (argTypes[2] != ARGDOUBLE) ||
	   (argTypes[3] != ARGCOMMA)  ||
	   (argTypes[4] != ARGDOUBLE) ||
	   (argTypes[5] != ARGCOMMA)  ||
	   (argTypes[6] != ARGDOUBLE))
    errorCode = BAD_ARGUMENTS;
  return ((errorCode != OK) ? NULL:
	  new TiltPartCommand(tag, argDoubles[0], argDoubles[2],
				argDoubles[4], argDoubles[6]));
}

/*******************************************************************/

/* parserCmd::makeCommand

Returned Value: A Command of the appropriate type or NULL.

Called By: parserCmd::parseCommand

Rules: None.

This just calls the makeXXX function for the command type whose name
is in the commandName data member of the parser.

*/

Command * parserCmd::makeCommand()
{
  static Command * (parserCmd::*makers[])() = {
  /*  0 */ &parserCmd::makeAbortE,
  /*  1 */ &parserCmd::makeAbortE,
  /*  2 */ &parserCmd::makeAbortE,
  /*  3 */ &parserCmd::makeAlignPart,
  /*  4 */ &parserCmd::makeAlignTool,
  /*  5 */ &parserCmd::makeCenterPart,
  /*  6 */ &parserCmd::makeChangeTool,
  /*  7 */ &parserCmd::makeClearAllErrors,
  /*  8 */ &parserCmd::makeDisableUser,
  /*  9 */ &parserCmd::makeEnableUser,
  /* 10 */ &parserCmd::makeEndSession,
  /* 11 */ &parserCmd::makeEnumAllProp,
  /* 12 */ &parserCmd::makeEnumProp,
  /* 13 */ &parserCmd::makeEnumTools,
  /* 14 */ &parserCmd::makeFindTool,
  /* 15 */ &parserCmd::makeGet,
  /* 16 */ &parserCmd::makeGetChangeToolAction,
  /* 17 */ &parserCmd::makeGetCoordSystem,
  /* 18 */ &parserCmd::makeGetCsyTransformation,
  /* 19 */ &parserCmd::makeGetDMEVersion,
  /* 20 */ &parserCmd::makeGetErrorInfo,
  /* 21 */ &parserCmd::makeGetErrStatusE,
  /* 22 */ &parserCmd::makeGetMachineClass,
  /* 23 */ &parserCmd::makeGetProp,
  /* 24 */ &parserCmd::makeGetPropE,
  /* 25 */ &parserCmd::makeGetXtdErrStatus,
  /* 26 */ &parserCmd::makeGoTo,
  /* 27 */ &parserCmd::makeHome,
  /* 28 */ &parserCmd::makeIsHomed,
  /* 29 */ &parserCmd::makeIsUserEnabled,
  /* 30 */ &parserCmd::makeLockAxis,
  /* 31 */ &parserCmd::makeOnMoveReportE,
  /* 32 */ &parserCmd::makeOnPtMeasReport,
  /* 33 */ &parserCmd::makeOnScanReport,
  /* 34 */ &parserCmd::makePtMeas,
  /* 35 */ &parserCmd::makeReQualify,
  /* 36 */ &parserCmd::makeScanInCylEndIsPlane,
  /* 37 */ &parserCmd::makeScanInCylEndIsSphere,
  /* 38 */ &parserCmd::makeScanInPlaneEndIsCyl,
  /* 39 */ &parserCmd::makeScanInPlaneEndIsPlane,
  /* 40 */ &parserCmd::makeScanInPlaneEndIsSphere,
  /* 41 */ &parserCmd::makeScanOnCircle,
  /* 42 */ &parserCmd::makeScanOnCircleHint,
  /* 43 */ &parserCmd::makeScanOnLine,
  /* 44 */ &parserCmd::makeScanOnLineHint,
  /* 45 */ &parserCmd::makeScanUnknownHint,
  /* 46 */ &parserCmd::makeSetCoordSystem,
  /* 47 */ &parserCmd::makeSetCsyTransformation,
  /* 48 */ &parserCmd::makeSetProp,
  /* 49 */ &parserCmd::makeSetTool,
  /* 50 */ &parserCmd::makeStartSession,
  /* 51 */ &parserCmd::makeStopAllDaemons,
  /* 52 */ &parserCmd::makeStopDaemon,
  /* 53 */ &parserCmd::makeTiltCenterPart,
  /* 54 */ &parserCmd::makeTiltPart
  };
  return (*this.*makers[commandName])();
}

/*******************************************************************/

/* parserCmd::parseCommand

Returned Value: A Command of the appropriate type or NULL.

Called By:
   external functions
   main (in stand-alone command parser and stand-alone command checker)

Rules:
1. <13><10><0> must immediately follow the close parentheses at the
   end of the command.
   BAD_CHARACTER_AFTER_COMMAND_END
2. If the command is an event command, the tag must be an event tag.
   EVENT_COMMAND_MUST_HAVE_EVENT_TAG
3. If the command is not an event command, the tag must not be an event tag.
   NON_EVENT_COMMAND_MUST_NOT_HAVE_EVENT_TAG

The setInput function copies a command string into the parser's
inputArray, stopping at the first NULL, so the rule above is reasonable.

parseTag sets the errorCode to OK if the tag is OK syntactically.

*/

Command * parserCmd::parseCommand()
{
  Command * aCommand;

  commandName = -1;
  parseTag();
  if (errorCode == OK)
    unSpaceInputArray();
  if (errorCode == OK)
    parseCmdName();
  if (errorCode == OK)
    parseCmdArguments();
  if ((errorCode == OK) &&
      ((inputArray[arrayIndex] != 13) ||
       (inputArray[arrayIndex + 1] != 10) ||
       (inputArray[arrayIndex + 2] != 0)))
    errorCode = BAD_CHARACTER_AFTER_COMMAND_END;
  if (errorCode == OK)
    {
      if ((commandName == AbortE) ||
	  (commandName == GetErrStatusE) ||
	  (commandName == GetPropE) ||
	  (commandName == OnMoveReportE))
	{
	  if (!isEvent)
	    errorCode = EVENT_COMMAND_MUST_HAVE_EVENT_TAG;
	}
      else
	{
	  if (isEvent)
	    errorCode = NON_EVENT_COMMAND_MUST_NOT_HAVE_EVENT_TAG;
	}
    }
  if (errorCode == OK)
    aCommand = makeCommand();
  return ((errorCode != OK) ? NULL : aCommand);
}

/*******************************************************************/

/* parserCmd::parseCmdArguments

Returned Value: None.

Called By: parserCmd::parseCommand

Rules:
1. If an argument component starts with a left parenthesis and is followed
   by a number, but is not preceded by the IJK keyword, then a right
   parenthesis must follow the number.
   BAD_COMMAND_ARGUMENTS
2. The argument component being parsed does not start the way any known
   argument component starts.
   BAD_COMMAND_ARGUMENTS

Each turn around the "for" loop parses one argument component.

This decides what subordinate function to call to do the parsing
depending on how the argument component starts.

When this is called, arrayIndex points at the first character after the
left parenthesis that starts the argument list. On subsequent turns
around the for loop, arrayIndex points at the first character of an
argument component. Since arrayIndex++ is executed at the end of each turn
around the loop, any function this calls must leave arrayIndex pointing
at the last character it reads.

Spaces outside of strings are removed before this runs, so spaces
are not dealt with outside strings.

A dot may appear either before a capital letter (as in Tool.PtMeasPar)
or at the beginning of a number (as in .123). In the first case it is
an argument list component. In the second case it is part of the number.

If the right parentheses after the arguments is missing, that will be
caught either here (on the second BAD_COMMAND_ARGUMENTS line) or in one
of the functions this calls.

*/

void parserCmd::parseCmdArguments()
{
  char c;
  char d;

  for (argCount = 0; ((c = inputArray[arrayIndex]) != ')' ); argCount++)
    {
      if (argCount == argSize)
	makeArraysBigger();
      d = inputArray[arrayIndex + 1];
      if (c == ',')
	argTypes[argCount] = ARGCOMMA;
      else if ((c == '.') && (d > 64) && (d < 91)) /* dot before upper letter */
	argTypes[argCount] = ARGDOT;
      else if ((c == '(') && (d == ')'))
	    {
	      arrayIndex++;
	      argTypes[argCount] = ARGPARENEMPTY;
	    }
      else if ((c == '(') && 
	       ((d == '-') ||
		(d == '+') ||
		(d == '.') ||
		((d > 47) && (d < 58))))
	{
	  arrayIndex++;
	  if ((argTypes[argCount - 1] == ARGKEYWORD) &&
	      (argKeywords[argCount - 1] == IJK))
	    {
	      parseIJK();
	      argTypes[argCount] = ARGIJK;
	    }
	  else
	    {
	      parseNumber(argDoubles + argCount);
	      if (errorCode != OK);
	      else if (inputArray[arrayIndex + 1] == ')')
		{
		  arrayIndex++;
		  argTypes[argCount] = ARGPARENDOUBLE;
		}
	      else
		errorCode = BAD_COMMAND_ARGUMENTS;
	    }
	}
      else if (c == 34) /* open quote */
	parseString();
      else if ((c == '-') ||
	       (c == '+') ||
	       (c == '.') ||
	       ((c > 47) && (c < 58)))
	{
	  parseNumber(argDoubles + argCount);
	  argTypes[argCount] = ARGDOUBLE;
	}
      else if ((c > 64) && (c < 91))
	parseKeyword();
      else
	errorCode = BAD_COMMAND_ARGUMENTS;
      if (errorCode != OK)
	break;
      arrayIndex++; /* move to character after end of argument just parsed */
    }
  arrayIndex++; /* move to character after the close parenthesis */
}

/*******************************************************************/

/* parserCmd::parseCmdName

Returned Value: None.

Called By: parserCmd::parseCommand

Rules:
1. The command name must be a known command name.
   BAD_COMMAND_NAME

This reads the command name from the inputArray and sets commandName.
If the name is not the name of an I++ command, or if the name is not
followed immediately by a left parenthesis, commandName is set to -1.

The arrayIndex is left set to index the character following the left
parenthesis after the command name.

*/

void parserCmd::parseCmdName()
{
  char * name;

  name = (inputArray + 6);
  switch (name[0])
    {
    case 'A':
      if (strncmp(name, "AbortE(", 7) == 0)
	commandName = AbortE;
      else if (strncmp(name, "AlignPart(", 10) == 0)
	commandName = AlignPart;
      else if (strncmp(name, "AlignTool(", 10) == 0)
	commandName = AlignTool;
      break;
    case 'C':
      if (strncmp(name, "CenterPart(", 11) == 0)
	commandName = CenterPart;
      else if (strncmp(name, "ChangeTool(", 11) == 0)
	commandName = ChangeTool;
      else if (strncmp(name, "ClearAllErrors(", 15) == 0)
	commandName = ClearAllErrors;
      break;
    case 'D':
      if (strncmp(name, "DisableUser(", 12) == 0)
	commandName = DisableUser;
      break;
    case 'E':
      if (strncmp(name, "EnableUser(", 11) == 0)
	commandName = EnableUser;
      else if (strncmp(name, "EndSession(", 11) == 0)
	commandName = EndSession;
      else if (strncmp(name, "EnumAllProp(", 12) == 0)
	commandName = EnumAllProp;
      else if (strncmp(name, "EnumProp(", 9) == 0)
	commandName = EnumProp;
      else if (strncmp(name, "EnumTools(", 10) == 0)
	commandName = EnumTools;
      break;
    case 'F':
      if (strncmp(name, "FindTool(", 9) == 0)
	commandName = FindTool;
      break;
    case 'G':
      if (strncmp(name, "Get", 3) == 0)
	{
	  if (strncmp((name + 3), "(" , 1) == 0)
	    commandName = Get;
	  else if (strncmp((name + 3), "ChangeToolAction(", 17) == 0)
	    commandName = GetChangeToolAction;
	  else if (strncmp((name + 3), "CoordSystem(", 12) == 0)
	    commandName = GetCoordSystem;
	  else if (strncmp((name + 3), "CsyTransformation(", 18) == 0)
	    commandName = GetCsyTransformation;
	  else if (strncmp((name + 3), "ErrorInfo(", 10) == 0)
		  commandName = ipp::GetErrorInfo;
	  else if (strncmp((name + 3), "DMEVersion(", 11) == 0)
	    commandName = GetDMEVersion;
	  else if (strncmp((name + 3), "ErrStatusE(", 11) == 0)
	    commandName = GetErrStatusE;
	  else if (strncmp((name + 3), "MachineClass(", 13) == 0)
	    commandName = GetMachineClass;
	  else if (strncmp((name + 3), "Prop(", 5) == 0)
		  commandName = ipp::GetProp;
	  else if (strncmp((name + 3), "PropE(", 6) == 0)
	    commandName = GetPropE;
	  else if (strncmp((name + 3), "XtdErrStatus(", 13) == 0)
	    commandName = GetXtdErrStatus;
	}
      else if (strncmp(name, "GoTo(", 5) == 0)
	commandName = GoTo;
      break;
    case 'H':
      if (strncmp(name, "Home(", 5) == 0)
	commandName = Home;
      break;
    case 'I':
      if (strncmp(name, "IsHomed(", 8) == 0)
	commandName = IsHomed;
      else if (strncmp(name, "IsUserEnabled(", 14) == 0)
	commandName = IsUserEnabled;
      break;
    case 'L':
      if (strncmp(name, "LockAxis(", 9) == 0)
	commandName = LockAxis;
      break;
    case 'O':
      if (strncmp(name, "OnMoveReportE(", 14) == 0)
	commandName = OnMoveReportE;
      else if (strncmp(name, "OnPtMeasReport(", 15) == 0)
	commandName = OnPtMeasReport;
      else if (strncmp(name, "OnScanReport(", 13) == 0)
	commandName = OnScanReport;
      break;
    case 'P':
      if (strncmp(name, "PtMeas(", 7) == 0)
	commandName = PtMeas;
      break;
    case 'R':
      if (strncmp(name, "ReQualify(", 10) == 0)
	commandName = ReQualify;
      break;
    case 'S':
      if (strncmp(name, "Scan", 4) == 0)
	{
	  if (strncmp((name + 4), "InCylEndIsPlane(", 16) == 0)
	    commandName = ScanInCylEndIsPlane;
	  else if (strncmp((name + 4), "InCylEndIsSphere(", 17) == 0)
	    commandName = ScanInCylEndIsSphere;
	  else if (strncmp((name + 4), "InPlaneEndIsCyl(", 16) == 0)
	    commandName = ScanInPlaneEndIsCyl;
	  else if (strncmp((name + 4), "InPlaneEndIsPlane(", 18) == 0)
	    commandName = ScanInPlaneEndIsPlane;
	  else if (strncmp((name + 4), "InPlaneEndIsSphere(", 19) == 0)
	    commandName = ScanInPlaneEndIsSphere;
	  else if (strncmp((name + 4), "OnCircle(", 9) == 0)
	    commandName = ScanOnCircle;
	  else if (strncmp((name + 4), "OnCircleHint(", 13) == 0)
	    commandName = ScanOnCircleHint;
	  else if (strncmp((name + 4), "OnLine(", 7) == 0)
	    commandName = ScanOnLine;
	  else if (strncmp((name + 4), "OnLineHint(", 11) == 0)
	    commandName = ScanOnLineHint;
	  else if (strncmp((name + 4), "UnknownHint(", 12) == 0)
	    commandName = ScanUnknownHint;
	}
      else if (strncmp(name, "Set", 3) == 0)
	{
	  if (strncmp((name + 3), "CoordSystem(", 12) == 0)
	    commandName = SetCoordSystem;
	  else if (strncmp((name + 3), "CsyTransformation(", 18) == 0)
	    commandName = SetCsyTransformation;
	  else if (strncmp((name + 3), "Prop(", 5) == 0)
		  commandName = ipp::SetProp;
	  else if (strncmp((name + 3), "Tool(", 5) == 0)
	    commandName = SetTool;
	}
      else if (strncmp(name, "StartSession(", 13) == 0)
	commandName = StartSession;
      else if (strncmp(name, "StopAllDaemons(", 15) == 0)
	commandName = StopAllDaemons;
      else if (strncmp(name, "StopDaemon(", 11) == 0)
	commandName = StopDaemon;
      break;
    case 'T':
      if (strncmp(name, "TiltCenterPart(", 15) == 0)
	commandName = TiltCenterPart;
      else if (strncmp(name, "TiltPart(", 9) == 0)
	commandName = TiltPart;
      break;
    default:
      break;
    }
  if (commandName != -1)
    {
      for (arrayIndex = 6; inputArray[arrayIndex] != '('; arrayIndex++);
      arrayIndex++; /* set arrayIndex to point at character after left paren */
    }
  else
    errorCode = BAD_COMMAND_NAME;
}

/*******************************************************************/

/* parserCmd::parseKeyword

Returned Value: None.

Called By: parserCmd::parseCmdArguments

Rules:
1. The character after a keyword must be a period, a comma, or a left or
   right parenthesis
   BAD_CHARACTER_AFTER_KEYWORD
2. The characters being parsed must form either an event tag or a
   known keyword.
   BAD_KEYWORD

This parses event tags as well as keywords.

This is called only if the character at inputArray[arrayIndex] is an
upper case letter.

If there are extra letters after a valid keyword, rule 1 will be violated,
so the extra letters will be detected.

Spaces may be legal in the command itself, but spaces have been
removed before this function is called.

If one keyword is the same as the beginning of another keyword, a check
for the longer keyword must be made first. The only cases of that are
A, C, and R.

When the keyword is a single letter, it is not necessary to use a
check like [if (strncmp(keyword, "A", 1) == 0)] since that is already
known from the case statement to be true, but those checks are
included just to keep the code looking consistent.

This is parsing some keywords (MoveMan, OBB, etc.) that cannot occur
in commands, but there is no harm in that. Inappropriate keywords will
be detected elsewhere.

*/

void parserCmd::parseKeyword()
{
  char * keyword; // pointer to start of word
  int length;     // used to reset arrayIndex to last character parsed

  argKeywords[argCount] = -1;
  keyword = (inputArray + arrayIndex);
  switch (keyword[0])
    {
    case 'A':
      if (strncmp(keyword, "Accel", 5) == 0)
	{
	  argKeywords[argCount] = Accel;
	  length = 4;
	}
      else if (strncmp(keyword, "Act", 3) == 0)
	{
	  argKeywords[argCount] = Actual;
	  length = 2;
	}
      else if (strncmp(keyword, "Alignment", 9) == 0)
	{
	  argKeywords[argCount] = Alignment;
	  length = 8;
	}
      else if (strncmp(keyword, "Approach", 8) == 0)
	{
	  argKeywords[argCount] = Approach;
	  length = 7;
	}
      else if (strncmp(keyword, "A", 1) == 0)
	{
	  argKeywords[argCount] = A;
	  length = 0;
	}
      break;
    case 'B':
      if (strncmp(keyword, "B", 1) == 0)
	{
	  argKeywords[argCount] = B;
	  length = 0;
	}
      break;
    case 'C':
      if (strncmp(keyword, "CollisionVolume", 15) == 0)
	{
	  argKeywords[argCount] = CollisionVolume;
	  length = 14;
	}
      else if (strncmp(keyword, "C", 1) == 0)
	{
	  argKeywords[argCount] = C;
	  length = 0;
	}
      break;
    case 'D':
      if (strncmp(keyword, "Def", 3) == 0)
	{
	  argKeywords[argCount] = Default;
	  length = 2;
	}
      else if (strncmp(keyword, "Dis", 3) == 0)
	{
	  argKeywords[argCount] = Dis;
	  length = 2;
	}
      break;
    case 'E':
      if ((inputArray[arrayIndex + 1] > 47) &&
	  (inputArray[arrayIndex + 1] < 58) &&
	  (inputArray[arrayIndex + 2] > 47) &&
	  (inputArray[arrayIndex + 2] < 58) &&
	  (inputArray[arrayIndex + 3] > 47) &&
	  (inputArray[arrayIndex + 3] < 58) &&
	  (inputArray[arrayIndex + 4] > 47) &&
	  (inputArray[arrayIndex + 4] < 58))
	{
	  argKeywords[argCount] = TagKey;
	  argInt = (unsigned int)((1000 * (inputArray[++arrayIndex] -  48)) +
				  (100  * (inputArray[++arrayIndex] -  48)) +
				  (10   * (inputArray[++arrayIndex] -  48)) +
				  (1    * (inputArray[++arrayIndex] -  48)));
	  length = 0;
	}
      else if (strncmp(keyword, "ER", 2) == 0)
	{
	  argKeywords[argCount] = ER;
	  length = 1;
	}
      break;
    case 'F':
      if (strncmp(keyword, "FoundTool", 9) == 0)
	{
	  argKeywords[argCount] = FoundTool;
	  length = 8;
	}
      break;
    case 'G':
      if (strncmp(keyword, "GoToPar", 7) == 0)
	{
	  argKeywords[argCount] = GoToPar;
	  length = 6;
	}
      break;
    case 'I':
      if (strncmp(keyword, "Id", 2) == 0)
	{
	  argKeywords[argCount] = Id;
	  length = 1;
	}
      else if (strncmp(keyword, "IJK", 3) == 0)
	{
	  argKeywords[argCount] = IJK;
	  length = 2;
	}
      break;
    case 'J':
      if (strncmp(keyword, "JogDisplayCsy", 13) == 0)
	{
	  argKeywords[argCount] = JogDisplayCsy;
	  length = 12;
	}
      else if (strncmp(keyword, "JogMoveCsy", 10) == 0)
	{
	  argKeywords[argCount] = JogMoveCsy;
	  length = 9;
	}
      break;
    case 'M':
      if (strncmp(keyword, "MachineCsy", 10) == 0)
	{
	  argKeywords[argCount] = MachineCsy;
	  length = 9;
	}
      else if (strncmp(keyword, "Max", 3) == 0)
	{
	  argKeywords[argCount] = Max;
	  length = 2;
	}
      else if (strncmp(keyword, "Min", 3) == 0)
	{
	  argKeywords[argCount] = Min;
	  length = 2;
	}
      else if (strncmp(keyword, "MoveableMachineCsy", 18) == 0)
	{
	  argKeywords[argCount] = MoveableMachineCsy;
	  length = 17;
	}
      else if (strncmp(keyword, "MoveAuto", 8) == 0)
	{
	  argKeywords[argCount] = MoveAuto;
	  length = 7;
	}
      else if (strncmp(keyword, "MoveMan", 7) == 0)
	{
	  argKeywords[argCount] = MoveMan;
	  length = 6;
	}
      else if (strncmp(keyword, "MultipleArmCsy", 14) == 0)
	{
	  argKeywords[argCount] = MultipleArmCsy;
	  length = 13;
	}
      break;
    case 'N':
      if (strncmp(keyword, "Name", 4) == 0)
	{
	  argKeywords[argCount] = Name;
	  length = 3;
	}
      break;
    case 'O':
      if (strncmp(keyword, "OBB", 3) == 0)
	{
	  argKeywords[argCount] = OBB;
	  length = 2;
	}
      break;
    case 'P':
      if (strncmp(keyword, "PartCsy", 7) == 0)
	{
	  argKeywords[argCount] = PartCsy;
	  length = 6;
	}
      else if (strncmp(keyword, "Phi", 3) == 0)
	{
	  argKeywords[argCount] = Phi;
	  length = 2;
	}
      else if (strncmp(keyword, "PtMeasPar", 9) == 0)
	{
	  argKeywords[argCount] = PtMeasPar;
	  length = 8;
	}
      break;
    case 'Q':
      if (strncmp(keyword, "Q", 1) == 0)
	{
	  argKeywords[argCount] = Q;
	  length = 0;
	}
      break;
    case 'R':
      if (strncmp(keyword, "Retract", 7) == 0)
	{
	  argKeywords[argCount] = Retract;
	  length = 6;
	}
      else if (strncmp(keyword, "Rotate", 6) == 0)
	{
	  argKeywords[argCount] = Rotate;
	  length = 5;
	}
      else if (strncmp(keyword, "R", 1) == 0)
	{
	  argKeywords[argCount] = R;
	  length = 0;
	}
      break;
    case 'S':
      if (strncmp(keyword, "Search", 6) == 0)
	{
	  argKeywords[argCount] = Search;
	  length = 5;
	}
      else if (strncmp(keyword, "SensorCsy", 9) == 0)
	{
	  argKeywords[argCount] = SensorCsy;
	  length = 8;
	}
      else if (strncmp(keyword, "Speed", 5) == 0)
	{
	  argKeywords[argCount] = Speed;
	  length = 4;
	}
      else if (strncmp(keyword, "Switch", 6) == 0)
	{
	  argKeywords[argCount] = Switch;
	  length = 5;
	}
      break;
    case 'T':
      if (strncmp(keyword, "Time", 4) == 0)
	{
	  argKeywords[argCount] = Time;
	  length = 3;
	}
      else if (strncmp(keyword, "Tool", 4) == 0)
	{
	  argKeywords[argCount] = Tool;
	  length = 3;
	}
      break;
    case 'X':
      if (strncmp(keyword, "X", 1) == 0)
	{
	  argKeywords[argCount] = X;
	  length = 0;
	}
      break;
    case 'Y':
      if (strncmp(keyword, "Y", 1) == 0)
	{
	  argKeywords[argCount] = Y;
	  length = 0;
	}
      break;
    case 'Z':
      if (strncmp(keyword, "Z", 1) == 0)
	{
	  argKeywords[argCount] = Z;
	  length = 0;
	}
      break;
    default:
      break;
    }
  if (argKeywords[argCount] != -1)
    {
      arrayIndex = (arrayIndex + length);
      if ((inputArray[arrayIndex + 1] == '.') ||
	  (inputArray[arrayIndex + 1] == ',') ||
	  (inputArray[arrayIndex + 1] == '(') ||
	  (inputArray[arrayIndex + 1] == ')'))
	argTypes[argCount] =
	  ((argKeywords[argCount] == TagKey) ? ARGETAG : ARGKEYWORD);
      else
	errorCode = BAD_CHARACTER_AFTER_KEYWORD;
    }
  else
    errorCode = BAD_KEYWORD;
}

/*******************************************************************/

/* parserCmd::parseNumber

Returned Value: None.

Called By:
  parserCmd::parseCmdArguments
  parserCmd::parseIJK

Rules:
1. A number must not have two decimal points.
   BAD_NUMBER_TWO_DECIMAL_POINTS
2. A number must have at least one digit.
   BAD_NUMBER_NO_DIGITS
3. A number must not have more than 16 digits.
   BAD_NUMBER_MORE_THAN_16_DIGITS
4. The exponent of a number must have 1, 2, or 3 digits.
   BAD_E_NUMBER_EXPONENT_MUST_HAVE_ONE_TWO_OR_THREE_DIGITS

Reference pages: (33 - 34)

In natural language, the definition of a number is: An optional sign
followed by one to sixteen digits (optionally with a decimal point
before the digits, between two digits, or after the last digit)
followed optionally by an exponent. An exponent is an upper case E or
a lower case e followed by an optional sign, followed by one, two, or
three digits.  This definition is given in the spec on page 33 in
natural language and on page 34 using a production language.

This parses all numbers as doubles. If a number is required to be an
integer by the syntax of command XXX, the makeXXX function checks that
it has the value of an integer.

When this starts up, arrayIndex is set to indicate the first character
of what should be a number (a plus or minus sign, a digit, or a
decimal point). When this finishes, arrayIndex is set to indicate the
last character that could be part of the number.

The parsed number is placed in the store argument.

*/

void parserCmd::parseNumber(double * store)
{
  int sign = 0; // set to 1 if first character is '+' or '-'
  int dot = 0;  // set to 1 if a decimal point is found
  int k;        // index for inputArray
  int j;        // counts digits of exponent
  char save;

  if ((inputArray[arrayIndex] == '-') || (inputArray[arrayIndex] == '+'))
    sign = 1;
  for (k = (arrayIndex + sign);
       ((errorCode == OK) &&
	((inputArray[k] == '.') ||
	 ((inputArray[k] > 47) && (inputArray[k] < 58))));
       k++)
    {
      if (inputArray[k] == '.')
	{
	  if (dot == 0)
	    dot = 1;
	  else
	    errorCode = BAD_NUMBER_TWO_DECIMAL_POINTS;
	}   
    }
  if ((errorCode == OK) && ((arrayIndex + sign + dot) == k))
    errorCode = BAD_NUMBER_NO_DIGITS;
  if ((errorCode == OK) && ((k - (arrayIndex + sign + dot)) > 16))
    errorCode = BAD_NUMBER_MORE_THAN_16_DIGITS;
  if ((errorCode == OK) && ((inputArray[k] == 'E') || (inputArray[k] == 'e')))
    {
      k++;
      if ((inputArray[k] == '+') || (inputArray[k] == '-'))
	k++;
      for (j = 0; ((inputArray[k] > 47) && (inputArray[k] < 58)); j++)
	k++;
      if ((j < 1) || (j > 3))
	errorCode = BAD_E_NUMBER_EXPONENT_MUST_HAVE_ONE_TWO_OR_THREE_DIGITS;
    }
  if (errorCode == OK)
    {
      save = inputArray[k];
      inputArray[k] = 0; //put in temporary null terminator
      sscanf((inputArray + arrayIndex), "%lf", store);
      inputArray[k] = save; // put char back
      arrayIndex = (k - 1); // set arrayIndex to end of number
    }
}

/*******************************************************************/

/* parserCmd::parseString

Returned Value: None.

Called By: parserCmd::parseCmdArguments

Rules:
1. There must be a terminating double-quote (ASCII 34), and all characters
   between the double quotes must be in the range ASCII 32 to 126.
   BAD_STRING
2. A string must have at least one character in it.
   BAD_STRING

Reference pages: 10 (33) 34 35 43 46 66 73 99 102 106 107 108 109
These references are only those where string is the data type of a
component of a command or a response. The word string is used in other
places to refer to an entire command or response.

A String starts with a double quote (ASCII 34) and ends at the
next double quote. A double quote cannot be inside a string.
When this function starts, arrayIndex is indicating the opening
double quote.

*/

void parserCmd::parseString()
{
  int start;
  char c;

  arrayIndex++; // skip starting quote
  start = arrayIndex;
  for (; ((c = inputArray[arrayIndex]) != 34); arrayIndex++)
    {
      if ((c < 32) || (c > 126))
	{
	  errorCode = BAD_STRING;
	  break;
	}
    }
  if (start == arrayIndex)
    errorCode = BAD_STRING;
  if (errorCode == OK)
    {
      inputArray[arrayIndex] = 0; // set temporary null terminator
      argStrings[argCount] = strdup(inputArray + start); // hook copy in
      inputArray[arrayIndex] = 34; // restore quote at end
      argTypes[argCount] = ARGSTRING;
    }
}

/*******************************************************************/

/* parserCmd::parseTag

Returned Value: None.

Called By: parserCmd::parseCommand

Rules:
1. A tag must have five characters, the first of which is a digit or E,
   and the rest of which are digits.
   BAD_TAG_CHARACTER
2. The sixth input character (which should follow the last tag character)
   must be a space.
   SPACE_MISSING_AFTER_TAG
3. If the first character is a digit, the number formed by the five digits
   must be in the range 1 - 99999. Since there are only five digits,
   the upper limit does not need testing, so the only check is for 0.
   TAG_NUMBER_OUT_OF_RANGE_FOR_COMMAND_TAG
4. If the first character is E, the number formed by the four digits must
   be in the range 1 - 9999. Since there are only four digits, the upper
   limit does not need checking, so the only check is for 0.
   TAG_NUMBER_OUT_OF_RANGE_FOR_EVENT_TAG

Reference pages: (21) (22) 26-30 (34-42) (47) (49) (50) (75) (77) 99-113

There should be a tag at the beginning of the inputArray. This function
reads that tag.

In responses, the event tag E0000 is allowed, but this is a command parser,
so it is not allowed here.

*/


void parserCmd::parseTag()
{
  errorCode = OK;
  if ((inputArray[1] < 48) ||
      (inputArray[1] > 57) ||
      (inputArray[2] < 48) ||
      (inputArray[2] > 57) ||
      (inputArray[3] < 48) ||
      (inputArray[3] > 57) ||
      (inputArray[4] < 48) ||
      (inputArray[4] > 57))
    errorCode = BAD_TAG_CHARACTER;
  else if (inputArray[5] != 32)
    errorCode = SPACE_MISSING_AFTER_TAG;
  else if (inputArray[0] == 'E')
    {
      isEvent = 1;
      sscanf((inputArray + 1), "%d", &tag);
      if (tag == 0)
	errorCode = TAG_NUMBER_OUT_OF_RANGE_FOR_EVENT_TAG;
    }
  else if ((inputArray[0] > 47) && (inputArray[0] < 58))
    {
      isEvent = 0;
      sscanf(inputArray, "%d", &tag);
      if (tag == 0)
	errorCode = TAG_NUMBER_OUT_OF_RANGE_FOR_COMMAND_TAG;
    }
  else
    errorCode = BAD_TAG_CHARACTER;
}

/*******************************************************************/

/* parserCmd::parseIJK

Returned Value: None.

Called By: parserCmd::parseCmdArguments

Rules:
1. A comma must follow the first and second numbers.
   BAD_IJK_NUMBERS
2. Each of the three numbers must start with a plus sign, minus
   sign, decimal point, or digit.
   BAD_IJK_NUMBERS
3. The third number must be followed by a right parenthesis.
   BAD_IJK_NUMBERS

Reference pages: 27 28 (49) (52 - 53) (61) (62) (78) (81) 103 104
These reference pages include all uses of IJK, not just the use
that is is parsed by this function.

This is called only if arguments are being parsed, a left parenthesis
has been found, the keyword IJK precedes the left parenthesis, and the
character after the left parenthesis could be part of a number. Three
numbers separated by commas and followed by a right parenthesis are
expected.  The numbers are placed in the argIJK array.

When this starts, arrayIndex is set to indicate the beginning of the
first number. When this finishes, arrayIndex is set to indicate the
closing right parenthesis.

*/

void parserCmd::parseIJK()
{
  char c;
  int n;

  for (n = 0; ((n < 3) && (errorCode == OK)); n++)
    {
      if (n > 0)
	{
	  arrayIndex++;
	  if (inputArray[arrayIndex] != ',')
	    {
	      errorCode = BAD_IJK_NUMBERS;
	      break;
	    }
	  else
	    arrayIndex++;
	}
      c = inputArray[arrayIndex];
      if ((c == '-') ||
	  (c == '+') ||
	  (c == '.') ||
	  ((c > 47) && (c < 58)))
	parseNumber(argIJK + n);
      else
	{
	  errorCode = BAD_IJK_NUMBERS;
	  break;
	}
    }
  if (errorCode == OK)
    {
      arrayIndex++;
      if (inputArray[arrayIndex] != ')')
	errorCode = BAD_IJK_NUMBERS;
    }
}

/*******************************************************************/

/* parserCmd::unSpaceInputArray

Returned Value: None.

Called By: parserCmd::parseCommand

Rules:
1. Starting with the seventh position of a command string, space must
   not appear anywhere except
   a. before or after a left or right parenthesis
   b. before or after a comma
   c. inside a string
   ILLEGAL_SPACE

Reference pages: (33 - 34)

The reference pages are currently not consistent regarding optional space
after a right parenthesis. The text says it is not allowed, but the EBNF
for SRightParen says it is allowed. This affects only the closing right
parentheses of a command, since every other right parenthesis is followed
either by a comma or a right parenthesis, both of which may have space
before them. The I++ response to this issue said that space should be
allowed after a right parenthesis, so that is what this function does.

Notes:

Starting at the seventh position (index 6), this removes from the
inputArray all spaces outside of strings. It does this by moving
characters from right to left as necessary to fill in any spaces.

The n-1 in the termination condition of the "for" loop allows the NULL
to be copied also.

*/

void parserCmd::unSpaceInputArray()
{
  char c;
  int n;
  int gap = 0;
  bool stringing = false;
  bool spaceBefore = false;
  bool puncBefore = false;

  for (n = 6; inputArray[n-1] != NULL; n++)
    {
      c = inputArray[n];
      if (stringing)
	{
	  if (c == 34)
	    stringing = false;
	}
      else if (c == ' ')
	{
	  gap++;
	  if (!puncBefore)
	    spaceBefore = true;
	  continue;
	}
      else if (spaceBefore)
	{
	  if ((c == '(') || (c == ')') || (c == ','))
	    {
	      spaceBefore = false;
	      puncBefore = true;
	    }
	  else
	    {
	      errorCode = ILLEGAL_SPACE;
	      break;
	    }
	}
      else if ((c == '(') || (c == ')') || (c == ','))
	puncBefore = true;
      else
	{
	  puncBefore = false;
	  if (c == 34)
	    stringing = true;
	}
      inputArray[n - gap] = c;
    }
}

/*******************************************************************/

