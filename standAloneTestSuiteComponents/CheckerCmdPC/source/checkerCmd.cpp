/* checkerCmd.cc

This defines functions in the checkerCmd class and defines a main
function outside the checkerCmd class. The main function uses both the
command parser and the checker so that it can be used with .prg files.

Functions named checkXXX (where XXX is a command name) are used to
check each type of command in context. The checkXXX functions are
called by checkCommand, which does some checks before calling.  The
documentation for each checkXXX function gives the rules that it is
enforcing and gives one or more references to pages of version 1.4 of
the spec. The error code generated if a rule is broken is given after
each rule.

The arguments to each command are assumed to have passed the checks
performed by the command parser. If this checker is used without the
checks performed by the command parser having been made previously, the
checker may crash or give wrong results.

Several semantic checks are made in this checker that could be made in
the command parser (because context is not required). For example, the
check that a direction vector is not (0, 0, 0). These are identified
here as semantic checks.

The reference pages reference both text and examples. Text references
are given in parentheses. Example references are given in brackets.
Other references are not enclosed.

Example: Reference pages: 23 26 (39) [40] 85
This means there is relevant text on page 39 and an example on page 40.

Page 17 is not referenced because the print is too small. Page 25 is not
referenced since everything on it appears identically elsewhere.

*/

#include "IppCmdRes.h"
#include "checkerCmd.h"

/*******************************************************************/

/* remove the // on the next line to build a stand-alone command checker */
//#define CHECKER_MAIN

#ifdef CHECKER_MAIN

#include "parserCmd.h"

/*******************************************************************/

/* main

This main function is compiled only if CHECKER_MAIN is defined.

When this function starts, it asks the user to enter "y" or "n" to
answer the question, "turn stepping on?". If the user enters y
(followed by <Enter>), stepping is turned on. Otherwise, stepping is
kept off. When stepping is on, the function parses and checks one
command at a time, waiting for the user to press <Enter> before going
on to the next command. When stepping is off, the function parses and
checks an entire file of commands without pausing.

The function then asks the user to enter the name of a command file to
check, or to enter q to quit. If the name of a command file is
entered, the function parses and checks it.

The function reads command strings from the command file, parses
them, and checks them. It is is not putting them on queues.  If
parsing and checking succeed, it prints each command the parser has
returned followed by \\ on a separate line. If parsing or checking
fails, the text of the command string is printed followed by the error
message caused by the command string. The world is updated to emulate
the execution of the command (so that the context may change for the
next command).

The world has a probe named "Probe1" in it so that commands requiring
data for a realistic probe will not cause errors. Test files that test
commands related to probes can use the the command
ChangeTool("Probe1"). That command will run without error in this
checker. After that command is executed, commands asking for or requiring
probe data will run without spurious errors.

*/

main()
{
  FILE * infile;
  checkerCmd checker1;
  parserCmd parser1;
  world * world1;
  char buffer[IPPSIZE];
  char commandText[IPPSIZE];
  char errorMessage[100];
  int k;
  int c;
  Command * aCommand;
  int code;
  int step;
  Box * box1;
  OnePropertyCollisionVolume * collisionVolume1;
  
  printf("turn stepping on? y/n\n");
  scanf("%s", buffer);
  step = ((strcmp(buffer, "y") == 0) ? 1 : 0);
  for(world1 = NULL; ;)
    {
      if (world1 != NULL)
	delete world1;
      world1 = new world; // need a new world for each file
      box1 = new Box(20, 20, 20, 10, 10, 10, 1, 0, 0, 0, 1, 0, 0, 0, 1);
      collisionVolume1 = new OnePropertyCollisionVolume(1, box1);
      world1->getTools()->addTool(new KTool("Probe1", "Probe1Id",
					    new GoToPars, new PtMeasPars,
					    false, collisionVolume1, 100, 4));
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
	  checker1.checkCommand(aCommand, world1);
	  code = checker1.getCheckerErr();
	  if (code != CHECKER_OK)
	    {
	      fprintf(stderr, "%s", buffer);
	      checker1.getErrorMessageString(errorMessage);
	      fprintf(stderr, "%s\n", errorMessage);
	      fprintf(stderr, "%04d\n\n", checker1.getIppErr(code));
	    }
	  else
	    {
	      if (aCommand->getCommandString(commandText, (IPPSIZE - 1)))
		{
		  printf("%s", commandText);
		  world1->update(aCommand);
		  if (world1->getOutOfRangeFlag() == true)
		    fprintf(stderr, "WARNING: PARAMETER OUT OF RANGE%c%c",
			    13, 10);
		  printf("%c%c%c%c", 92, 92, 13, 10);
		}
	      else
		fprintf(stderr, "command did not print\n");
	    }
	  if (step)
	    getchar();
	}
      fclose(infile);
      printf("%c%c%c%c%c%c", 58, 13, 10, 58, 13, 10);
    }
  return 0;
}

#endif /* #ifdef CHECKER_MAIN */

/*******************************************************************/

/* checkerCmd::getIppErr

Returned Value: int (the number of the I++ error corresponding to the
  "code" argument)

Called By:
  external functions
  main (in stand-alone command checker)

Rules: None.

For attempting to do anything with UNDEFTOOL, the IppErr is always
ToolNotDefined.

For attempting to do illegal things with NOTOOL, the IppErr is always
IllegalCommand.

*/

const int checkerCmd::getIppErr(int code)
{
  static const int ippErrors[] =
  {
    IllegalCommand,          // dummy to go with OK
    IllegalCommand,          // ALREADY_IN_SESSION
    VectorHasNoNorm,         // AXIS_DIRECTION_VECTOR_HAS_ZERO_LENGTH
    ArgumentOutOfRange,      // BAD_ERROR_NUMBER
    ToolNotDefined,          // CAN_NOT_CHANGE_TOOL_TO_UNDEFTOOL
    IllegalCommand,          // CAN_NOT_ENUMALLPROP_NOTOOL_PTMEASPAR
    ToolNotDefined,          // CAN_NOT_ENUMALLPROP_UNDEFTOOL_GOTOPAR
    ToolNotDefined,          // CAN_NOT_ENUMALLPROP_UNDEFTOOL_PTMEASPAR
    IllegalCommand,          // CAN_NOT_ENUMPROP_NOTOOL_PTMEASPAR
    ToolNotDefined,          // CAN_NOT_ENUMPROP_UNDEFTOOL_GOTOPAR
    ToolNotDefined,          // CAN_NOT_ENUMPROP_UNDEFTOOL_PTMEASPAR
    IllegalCommand,          // CAN_NOT_GETPROP_PTMEASPARS_OF_NOTOOL
    ToolNotDefined,          // CAN_NOT_GETPROP_UNDEFTOOL
    IllegalCommand,          // CAN_NOT_GETPROPE_PTMEASPARS_OF_NOTOOL
    ToolNotDefined,          // CAN_NOT_GETPROPE_UNDEFTOOL
    ToolNotDefined,          // CAN_NOT_GOTO_USING_UNDEFTOOL
    IllegalCommand,          // CAN_NOT_PTMEAS_USING_NOTOOL
    ToolNotDefined,          // CAN_NOT_PTMEAS_USING_UNDEFTOOL
    IllegalCommand,          // CAN_NOT_REQUALIFY_NOTOOL,
    ToolNotDefined,          // CAN_NOT_REQUALIFY_UNDEFTOOL,
    IllegalCommand,          // CAN_NOT_SCAN_USING_NOTOOL
    ToolNotDefined,          // CAN_NOT_SCAN_USING_UNDEFTOOL
    ToolNotDefined,          // CAN_NOT_SET_TOOL_TO_UNDEFTOOL
    IllegalCommand,          // CAN_NOT_SETPROP_PTMEASPARS_OF_NOTOOL
    ToolNotDefined,          // CAN_NOT_SETPROP_UNDEFTOOL
    DaemonDoesNotExist,      // DAEMON_TO_STOP_DOES_NOT_EXIST
    VectorHasNoNorm,         // DIRECTION_VECTOR_HAS_ZERO_LENGTH
    BadProperty,             // DIS_ARGUMENT_MUST_BE_POSITIVE
    IncorrectArguments,      // FIRST_VECTOR_NOT_NORMALIZED
    DaemonAlreadyExists, // MUST_STOP_EXISTING_DAEMON_BEFORE_STARTING_ANOTHER
    IllegalCommand,          // NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
    IllegalCommand,          // NOT_IN_SESSION_CAN_NOT_EXECUTE_COMMAND
    IncorrectArguments,      // RADIUS_MUST_BE_PERPENDICULAR_TO_NORMAL
    IncorrectArguments,      // RADIUS_MUST_HAVE_POSITIVE_LENGTH
    UseClearAllErrorsToCont, // READINESS_ABORTED_CAN_NOT_EXECUTE_COMMAND
    UseClearAllErrorsToCont, // READINESS_ERRED_CAN_NOT_EXECUTE_COMMAND
    ArgumentOutOfRange,      // SCAN_POINT_MUST_BE_IN_WORK_VOLUME,
    IncorrectArguments,      // SECOND_VECTOR_NOT_NORMALIZED
    IncorrectArguments,      // START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT
    IncorrectArguments,      // START_POINT_MUST_DIFFER_FROM_END_POINT
    IncorrectArguments,      // START_POINT_MUST_NOT_BE_ON_CYLINDER_AXIS
    TargetPosOutOfMachVol,   // TARGET_POINT_OUT_OF_WORK_VOLUME
    ThetaOutOfRange,         // THETA_MUST_NOT_BE_NEGATIVE_OR_MORE_THAN_180
    BadProperty,             // TIME_ARGUMENT_MUST_NOT_BE_LESS_THAN_A_TENTH
    ToolNotFound             // UNKNOWN_TOOL
  };
  return ippErrors[code];
}

/*******************************************************************/

/* checkerCmd::getErrorMessageString

Returned Value: char *
  If there is a problem with providing a string, this returns NULL.
  Otherwise, it returns the buffer argument.

Called By:
  external functions
  main (in stand-alone command checker)

Rules: None.

This copies an error message into the buffer, preceded by the name
of the command being checked when the error was found.

The symbol for each error code is the same as the text of the error
message, except that the symbol has underscores. The symbol serves as
an index into the array of strings defined here. The symbols are
defined in checkerCmd.h.

*/

char * checkerCmd::getErrorMessageString(char * buffer)
{
  static const char * const errorStrings[] = {
    "OK",
    "ALREADY IN SESSION",
    "AXIS DIRECTION VECTOR HAS ZERO LENGTH",
    "BAD ERROR NUMBER",
    "CAN NOT CHANGE TOOL TO UNDEFTOOL",
    "CAN NOT ENUMALLPROP NOTOOL PTMEASPAR",
    "CAN NOT ENUMALLPROP UNDEFTOOL GOTOPAR",
    "CAN NOT ENUMALLPROP UNDEFTOOL PTMEASPAR",
    "CAN NOT ENUMPROP NOTOOL PTMEASPAR",
    "CAN NOT ENUMPROP UNDEFTOOL GOTOPAR",
    "CAN NOT ENUMPROP UNDEFTOOL PTMEASPAR",
    "CAN NOT GETPROP PTMEASPARS OF NOTOOL",
    "CAN NOT GETPROP UNDEFTOOL",
    "CAN NOT GETPROPE PTMEASPARS OF NOTOOL",
    "CAN NOT GETPROPE UNDEFTOOL",
    "CAN NOT GOTO USING UNDEFTOOL",
    "CAN NOT PTMEAS USING NOTOOL",
    "CAN NOT PTMEAS USING UNDEFTOOL",
    "CAN_NOT_REQUALIFY_NOTOOL",
    "CAN_NOT_REQUALIFY_UNDEFTOOL",
    "CAN NOT SCAN USING NOTOOL",
    "CAN NOT SCAN USING UNDEFTOOL",
    "CAN NOT SET TOOL TO UNDEFTOOL",
    "CAN NOT SETPROP PTMEASPARS OF NOTOOL",
    "CAN NOT SETPROP UNDEFTOOL",
    "DAEMON TO STOP DOES NOT EXIST",
    "DIRECTION VECTOR HAS ZERO LENGTH",
    "DIS ARGUMENT MUST BE POSITIVE",
    "FIRST VECTOR NOT NORMALIZED",
    "MUST STOP EXISTING DAEMON BEFORE STARTING ANOTHER",
    "NOT HOMED CAN NOT EXECUTE COMMAND",
    "NOT IN SESSION CAN NOT EXECUTE COMMAND",
    "RADIUS MUST BE PERPENDICULAR TO NORMAL",
    "RADIUS MUST HAVE POSITIVE LENGTH",
    "READINESS ABORTED CAN NOT EXECUTE COMMAND",
    "READINESS ERRED CAN NOT EXECUTE COMMAND",
    "SCAN POINT MUST BE IN WORK VOLUME",
    "SECOND VECTOR NOT NORMALIZED",
    "START POINT MUST DIFFER FROM DIRECTION POINT",
    "START POINT MUST DIFFER FROM END POINT",
    "START POINT MUST NOT BE ON CYLINDER AXIS",
    "TARGET POINT OUT OF WORK VOLUME",
    "THETA MUST NOT BE NEGATIVE OR MORE THAN 180",
    "TIME ARGUMENT MUST NOT BE LESS THAN A TENTH",
    "UNKNOWN TOOL",
  };
  if ((commandName > -1) && (commandName < 55))
    sprintf(buffer, "%s: %s",
	    ippCommandNameStrings[commandName], errorStrings[errorCode]);
  else
    sprintf(buffer, "%s", errorStrings[errorCode]);
  return buffer;
}

/*******************************************************************/

/* checkerCmd::checkAbortE 1
   
Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 9 24 26 (42) [43] 99

*/

void checkerCmd::checkAbortE(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkAlignPart 2
   
Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The system must be homed (see p. 48)
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND

Reference pages: 24 28 63 (95) 105

*/

void checkerCmd::checkAlignPart(Command * aCommand, world * aWorld)
{
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
}

/*******************************************************************/

/* checkerCmd::checkAlignTool 3

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. Vector1 must be a unit vector.
   FIRST_VECTOR_NOT_NORMALIZED
2. If vector2 is set, it must be a unit vector.
   SECOND_VECTOR_NOT_NORMALIZED
3. The system must be homed (see p. 48)
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND

Reference pages: 24 27 29 (55) 66 70 102 107

The first two rules are semantic checks.

Might add not allowing AlignTool using UnDefTool or NoTool.

*/

void checkerCmd::checkAlignTool(Command * aCommand, world * aWorld)
{
  AlignToolCommand * theAlign;
  double i;
  double j;
  double k;

  theAlign = (AlignToolCommand *)aCommand;
  i = theAlign->getVector1i();
  j = theAlign->getVector1j();
  k = theAlign->getVector1k();
  if (fabs(sqrt((i*i)+(j*j)+(k*k)) - 1.0) > IPP_NORMAL_ERROR)
    errorCode = FIRST_VECTOR_NOT_NORMALIZED;
  if ((errorCode == CHECKER_OK) && (theAlign->getHas2()))
    {
      i = theAlign->getVector2i();
      j = theAlign->getVector2j();
      k = theAlign->getVector2k();
      if (fabs(sqrt((i*i)+(j*j)+(k*k)) - 1.0) > IPP_NORMAL_ERROR)
	errorCode = SECOND_VECTOR_NOT_NORMALIZED;
    }
  if ((errorCode == CHECKER_OK) && (!(aWorld->getIsHomed())))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
}

/*******************************************************************/

/* checkerCmd:: checkCenterPart 4

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1.The system must be homed (see p. 48)
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND

Reference pages: 24 27 (96) 102

*/

void checkerCmd::checkCenterPart(Command * aCommand, world * aWorld)
{
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
}

/*******************************************************************/

/* checkerCmd::checkChangeTool 5

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The name of the tool must not be "UnDefTool".
   CAN_NOT_CHANGE_TOOL_TO_UNDEFTOOL
2. The tool must be one of the tools known to the toolchanger.
   UNKNOWN_TOOL

Reference pages: 24 27 29 (54) (57) (58) (78) 98 102 106

Rule 2 above is not explicit in the spec, but seems implicit. It also
seems implicit that it should be possible to change to "NoTool",
"BaseTool" or "RefTool". Rule 2 is a semantic check.

This is not currently requiring that the machine be homed, since it may
be possible to change tools without moving the machine on some machines.

*/

void checkerCmd::checkChangeTool(Command * aCommand, world * aWorld)
{
  ChangeToolCommand * theChange;

  theChange = (ChangeToolCommand *)aCommand;
  if (strcmp("UnDefTool", theChange->getToolName()) == 0)
    errorCode = CAN_NOT_CHANGE_TOOL_TO_UNDEFTOOL;
  else if (aWorld->getTools()->findTool(theChange->getToolName()) == NULL)
    errorCode = UNKNOWN_TOOL;
}

/*******************************************************************/

/* checkerCmd::checkClearAllErrors 6

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 26 [38] (40) (41) (42) (43) [44] (47) (75) 76 99

*/

void checkerCmd::checkClearAllErrors(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkDisableUser 7

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 (48) (51) (53) 102

*/

void checkerCmd::checkDisableUser(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkEnableUser 8

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 [39] (48) (51) 102

*/

void checkerCmd::checkEnableUser(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkEndSession 9

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 18 20 24 26 (41) (77) 99

*/

void checkerCmd::checkEndSession(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkEnumAllProp 10

Returned Value: None.

Called By: checkerCmd::checkCommand

The parser requires that the argument have one of the following patterns.
   key1()
   key1.key2()
   key1.key2.key3()
   key1.key2.key3.key4()
where key1 must be FoundTool or Tool and key2 must be GoToPar or PtMeasPar.

Rules:
1. If the value found by key1 is UnDefTool, there must be no key2.
   CAN_NOT_ENUMALLPROP_UNDEFTOOL_PTMEASPAR
   CAN_NOT_ENUMALLPROP_UNDEFTOOL_GOTOPAR
2. If the value found by key1 is NoTool, key2 must not be PtMeasPar.
   CAN_NOT_ENUMALLPROP_NOTOOL_PTMEASPAR

Reference pages: 24 26 45 (46) [74] 99

These rules are not explicit in the spec, but seem implicit. The rules
are consistent with the rules for EnumProp, GetProp, and GetPropE.

*/

void checkerCmd::checkEnumAllProp(Command * aCommand, world * aWorld)
{
  EnumAllPropCommand * theEnum;
  KTool * theTool;

  theEnum = (EnumAllPropCommand *)aCommand;
  if (theEnum->getKey1() == Tool)
    theTool = aWorld->getTools()->getActiveTool();
  else if (theEnum->getKey1() == FoundTool)
    theTool = aWorld->getTools()->getFoundTool();
  else // should never happen
    theTool = NULL;
  if (theTool && (theTool == aWorld->getTools()->getUnDefTool()))
    {
      if (theEnum->getNumberKeys() > 1)
	{
	  if (theEnum->getKey2() == PtMeasPar)
	    errorCode = CAN_NOT_ENUMALLPROP_UNDEFTOOL_PTMEASPAR;
	  else // if (theEnum->getKey2() == GoToPar)
	    errorCode = CAN_NOT_ENUMALLPROP_UNDEFTOOL_GOTOPAR;
	}
    }
  else if (theTool && (theTool == aWorld->getTools()->getNoTool()))
    {
      if ((theEnum->getNumberKeys() > 1) && (theEnum->getKey2() == PtMeasPar))
	errorCode = CAN_NOT_ENUMALLPROP_NOTOOL_PTMEASPAR;
    }
}

/*******************************************************************/

/* checkerCmd::checkEnumProp 11

Returned Value: None.

Called By: checkerCmd::checkCommand

The parser requires that the argument have one of the following patterns.
   key1()
   key1.key2()
   key1.key2.key3()
   key1.key2.key3.key4()
where key1 must be FoundTool or Tool and key2 must be GoToPar or PtMeasPar.

Rules:
1. If the value found by key1 is UnDefTool, there must be no key2.
   CAN_NOT_ENUMPROP_UNDEFTOOL_PTMEASPAR
   CAN_NOT_ENUMPROP_UNDEFTOOL_GOTOPAR
2. If the value found by key1 is NoTool, key2 must not be PtMeasPar.
   CAN_NOT_ENUMPROP_NOTOOL_PTMEASPAR

Reference pages: 24 26 29 (45) [73] 99 108 109 110 111

These rules are not explicit in the spec, but seem implicit. The rules
are consistent with the rules for EnumAllProp, GetProp, and GetPropE.

*/

void checkerCmd::checkEnumProp(Command * aCommand, world * aWorld)
{
  EnumPropCommand * theEnum;
  KTool * theTool;

  theEnum = (EnumPropCommand *)aCommand;
  if (theEnum->getKey1() == Tool)
    theTool = aWorld->getTools()->getActiveTool();
  else if (theEnum->getKey1() == FoundTool)
    theTool = aWorld->getTools()->getFoundTool();
  else
    theTool = NULL;
  if (theTool && (theTool == aWorld->getTools()->getUnDefTool()))
    {
      if (theEnum->getNumberKeys() > 1)
	{
	  if (theEnum->getKey2() == PtMeasPar)
	    errorCode = CAN_NOT_ENUMPROP_UNDEFTOOL_PTMEASPAR;
	  else // if (theEnum->getKey2() == GoToPar)
	    errorCode = CAN_NOT_ENUMPROP_UNDEFTOOL_GOTOPAR;
	}
    }
  else if (theTool && (theTool == aWorld->getTools()->getNoTool()))
    {
      if ((theEnum->getNumberKeys() > 1) && (theEnum->getKey2() == PtMeasPar))
	errorCode = CAN_NOT_ENUMPROP_NOTOOL_PTMEASPAR;
    }
}

/*******************************************************************/

/* checkerCmd::checkEnumTools 12

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 29 (53) (54) (56) 106

*/

void checkerCmd::checkEnumTools(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkFindTool 13

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 29 (54) [74] 102 106

This (unlike checkChangeTool) is allowing "UnDefTool". Thus, FoundTool
can be UnDefTool. It might be better to make this illegal.

*/

void checkerCmd::checkFindTool(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkGet 14

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None

Reference pages: 24 27 [38] [44] (49) (51) 57 (61) (63) (66) (81) 102

*/

void checkerCmd::checkGet(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd:: checkGetChangeToolAction 15

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The name of the tool must not be "UnDefTool".
   CAN_NOT_CHANGE_TOOL_TO_UNDEFTOOL
2. The tool must be one of the tools known to the toolchanger.
   UNKNOWN_TOOL

Reference pages: 24 27 29 (57-58) 102 106

*/

void checkerCmd::checkGetChangeToolAction(Command * aCommand, world * aWorld)
{
  GetChangeToolActionCommand * theChangeAction;

  theChangeAction = (GetChangeToolActionCommand *)aCommand;
  if (strcmp("UnDefTool", theChangeAction->getToolName()) == 0)
    errorCode = CAN_NOT_CHANGE_TOOL_TO_UNDEFTOOL;
  else if (aWorld->getTools()->findTool(theChangeAction->getToolName())
	   == NULL)
    errorCode = UNKNOWN_TOOL;
}

/*******************************************************************/

/* checkerCmd::checkGetCoordSystem 16

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 28 (59) 103

*/

void checkerCmd::checkGetCoordSystem(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkGetCsyTransformation 17

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 28 (60) (80) 103

*/

void checkerCmd::checkGetCsyTransformation(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd:: checkGetDMEVersion 18

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 26 (46) [47] 99

*/

void checkerCmd::checkGetDMEVersion(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkGetErrorInfo 19

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The error number must be an allowed error number.
   BAD_ERROR_NUMBER

Reference pages:  19 24 26 (43) 76 99
Page 76 does not mention the command but gives allowable error numbers.

The range 8000-9999 is provided for server-defined and client-defined
errors, as given on page 75, but it is not clear how such error numbers
are to be defined.

Rule 1 is a semantic check.

*/

void checkerCmd::checkGetErrorInfo(Command * aCommand, world * aWorld)
{
  GetErrorInfoCommand * theGetErrorInfo;
  unsigned int error_number;

  theGetErrorInfo = (GetErrorInfoCommand *)aCommand;
  error_number = theGetErrorInfo->getErrorNumber();
  if (((error_number >=    0) && (error_number <=    8)) ||
      ((error_number >=  500) && (error_number <=  515)) ||
      ((error_number >= 1000) && (error_number <= 1010)) ||
      ((error_number >= 1500) && (error_number <= 1503)) ||
      ((error_number >= 2000) && (error_number <= 2002)) ||
      ((error_number >= 2500) && (error_number <= 2506)) ||
      ((error_number >= 8000) && (error_number <= 9999)));
  else
    errorCode = BAD_ERROR_NUMBER;
}

/*******************************************************************/

/* checkerCmd::checkGetErrStatusE 20

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 (50) 102

*/

void checkerCmd::checkGetErrStatusE(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkGetMachineClass 21

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 18 24 27 (50) 102

*/

void checkerCmd::checkGetMachineClass(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkGetProp 22

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: 
1. Properties of UnDefTool cannot be gotten by GetProp.
   CAN_NOT_GETPROP_UNDEFTOOL
2. PtMeas parameters of NoTool cannot be gotten by GetProp.
   CAN_NOT_GETPROP_PTMEASPARS_OF_NOTOOL

Reference pages: 24 26 29 (45) 66 67 [68] [69] 70 [73] [74] 77 79 99 108

These rules are not explicit in the spec, but seem implicit. The rules
are consistent with the rules for EnumAllProp, EnumProp, and GetPropE.

*/

void checkerCmd::checkGetProp(Command * aCommand, world * aWorld)
{
  GetPropCommand * theGetProp;
  KTool * theTool;
  int n;

  theGetProp = (GetPropCommand *)aCommand;
  for (n = 0;
       ((errorCode == CHECKER_OK) && (n < theGetProp->getNumberProps()));
       n++)
    {
      if (theGetProp->getKey1(n) == Tool)
	theTool = aWorld->getTools()->getActiveTool();
      else if (theGetProp->getKey1(n) == FoundTool)
	theTool = aWorld->getTools()->getFoundTool();
      else
	theTool = NULL;
      if (theTool && (theTool == aWorld->getTools()->getUnDefTool()))
	errorCode = CAN_NOT_GETPROP_UNDEFTOOL;
      else if (theTool && (theGetProp->getKey2(n) == PtMeasPar) &&
	       (theTool == aWorld->getTools()->getNoTool()))
	errorCode = CAN_NOT_GETPROP_PTMEASPARS_OF_NOTOOL;
    }
}

/*******************************************************************/

/* checkerCmd::checkGetPropE 23

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. Properties of UnDefTool cannot be gotten by GetPropE.
   CAN_NOT_GETPROPE_UNDEFTOOL
2. PtMeas parameters of NoTool cannot be gotten by GetPropE.
   CAN_NOT_GETPROPE_PTMEASPARS_OF_NOTOOL

Reference pages: 24 26 29 (45) 99 108

These rules are not explicit in the spec, but seem implicit. The rules
are consistent with the rules for EnumAllProp, EnumProp, and GetProp.

*/

void checkerCmd::checkGetPropE(Command * aCommand, world * aWorld)
{
  GetPropECommand * theGetPropE;
  KTool * theTool;
  int n;

  theGetPropE = (GetPropECommand *)aCommand;
  for (n = 0;
       ((errorCode == CHECKER_OK) && (n < theGetPropE->getNumberProps()));
       n++)
    {
      if (theGetPropE->getKey1(n) == Tool)
	theTool = aWorld->getTools()->getActiveTool();
      else if (theGetPropE->getKey1(n) == FoundTool)
	theTool = aWorld->getTools()->getFoundTool();
      else
	theTool = NULL;
      if (theTool && (theTool == aWorld->getTools()->getUnDefTool()))
	errorCode = CAN_NOT_GETPROPE_UNDEFTOOL;
      else if (theTool && (theGetPropE->getKey2(n) == PtMeasPar) &&
	   (theTool == aWorld->getTools()->getNoTool()))
	errorCode = CAN_NOT_GETPROPE_PTMEASPARS_OF_NOTOOL;
    }
}

/*******************************************************************/

/* checkerCmd::checkGetXtdErrStatus 24

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 (50) 102

*/

void checkerCmd::checkGetXtdErrStatus(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkGoTo 25

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The system must be homed (see page 48).
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
2. The active tool must not be UnDefTool.
   CAN_NOT_GOTO_USING_UNDEFTOOL
3. The target point must be inside the work volume of the machine.
   TARGET_POINT_OUT_OF_WORK_VOLUME

Reference pages: 24 27 [38] [43] (51 - 53) (58) (62) [71] [72] (78) (98) 102

*/

void checkerCmd::checkGoTo(Command * aCommand, world * aWorld)
{
  GoToCommand * theGoTo;

  theGoTo = (GoToCommand *)aCommand;
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getUnDefTool())
    errorCode = CAN_NOT_GOTO_USING_UNDEFTOOL;
  else if (!(aWorld->inWorkVolume
	     ((theGoTo->getHasX() ? theGoTo->getX() : aWorld->getCurrentX()),
	      (theGoTo->getHasY() ? theGoTo->getY() : aWorld->getCurrentY()),
	      (theGoTo->getHasZ() ? theGoTo->getZ() : aWorld->getCurrentZ()))))
    errorCode = TARGET_POINT_OUT_OF_WORK_VOLUME;
}

/*******************************************************************/

/* checkerCmd::checkHome 26

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 [38] (48) 102

*/

void checkerCmd::checkHome(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkIsHomed 27

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 (48) 102 111

*/

void checkerCmd::checkIsHomed(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkIsUserEnabled 28

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 (49) 102

*/

void checkerCmd::checkIsUserEnabled(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd:: checkLockAxis 29

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 (97-98)

*/

void checkerCmd::checkLockAxis(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkOnMoveReportE 30

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. If Time is included, the interval must not be less than 0.1.
   TIME_ARGUMENT_MUST_NOT_BE_LESS_THAN_A_TENTH
2. If Dis is included, the interval must be positive.
   DIS_ARGUMENT_MUST_BE_POSITIVE
3. An OnMoveReportE daemon must not already exist.
   MUST_STOP_EXISTING_DAEMON_BEFORE_STARTING_ANOTHER

Reference pages: 19 24 27 [39] (49 - 50) 102
This is referenced as "OnReport" on pages 56, 57, 61, 63, and 66.

Rules 1 and 2 are semantic checks.

*/

void checkerCmd::checkOnMoveReportE(Command * aCommand, world * aWorld)
{
  OnMoveReportECommand * theOn;

  theOn = (OnMoveReportECommand *)aCommand;
  if (theOn->getHasTime() && (theOn->getTheTime() < 0.1))
    errorCode = TIME_ARGUMENT_MUST_NOT_BE_LESS_THAN_A_TENTH;
  else if (theOn->getHasDis() && (theOn->getTheDis() <= 0))
    errorCode = DIS_ARGUMENT_MUST_BE_POSITIVE;
  else if (aWorld->getOnMoveTag() != 0)
    errorCode = MUST_STOP_EXISTING_DAEMON_BEFORE_STARTING_ANOTHER;
}

/*******************************************************************/

/* checkerCmd::checkOnPtMeasReport 31

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 (41) (49) (52) (53) 61 [71] [72] (78) 102

*/

void checkerCmd::checkOnPtMeasReport(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkOnScanReport 32

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 (41) (81) 82 83 84 86 87 88 90 91 [93] 103

*/

void checkerCmd::checkOnScanReport(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkPtMeas 33

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. If there is an IJK vector, its length must not be zero.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
2. The system must be homed (see p. 48).
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
3. The target point must be inside the work volume of the machine.
   TARGET_POINT_OUT_OF_WORK_VOLUME
4. The active tool must be an actual tool.
   CAN_NOT_PTMEAS_USING_UNDEFTOOL
   CAN_NOT_PTMEAS_USING_NOTOOL

Reference pages: 9 24 27 (41) (49) (52 - 53) (62) [71] [72] (78) 83 84 86
                 87 89 90 91 102

*/

void checkerCmd::checkPtMeas(Command * aCommand, world * aWorld)
{
  PtMeasCommand * thePtMeas;

  thePtMeas = (PtMeasCommand *)aCommand;
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
  else if ((thePtMeas->getHasIJK()) && (thePtMeas->getI() == 0) &&
	   (thePtMeas->getJ() == 0) && (thePtMeas->getK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if (!(aWorld->inWorkVolume(
         (thePtMeas->getHasX() ? thePtMeas->getX() : aWorld->getCurrentX()),
         (thePtMeas->getHasY() ? thePtMeas->getY() : aWorld->getCurrentY()),
         (thePtMeas->getHasZ() ? thePtMeas->getZ() : aWorld->getCurrentZ()))))
    errorCode = TARGET_POINT_OUT_OF_WORK_VOLUME;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getUnDefTool())
    errorCode = CAN_NOT_PTMEAS_USING_UNDEFTOOL;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getNoTool())
    errorCode = CAN_NOT_PTMEAS_USING_NOTOOL;
}

/*******************************************************************/

/* checkerCmd::checkReQualify 34

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The system must be homed (see p. 48).
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
2. The current tool must not be UnDefTool.
   CAN_NOT_REQUALIFY_UNDEFTOOL
3. The current tool must not be NoTool.
   CAN_NOT_REQUALIFY_NOTOOL

Reference pages: 24 29 (64) 106 107
[This is called Qualify on pages 29 106 107]

Rules 2 and 3 are not explicit in the spec, but it seems prudent not
to attempt to qualify a non-existent tool or a tool whose identity and
existence are unknown.

It might be good to add not allowing Requalify when the tool is BaseTool,
since that does not seem to be an actual tool.

*/

void checkerCmd::checkReQualify(Command * aCommand, world * aWorld)
{
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getUnDefTool())
    errorCode = CAN_NOT_REQUALIFY_UNDEFTOOL;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getNoTool())
    errorCode = CAN_NOT_REQUALIFY_NOTOOL;
}

/*******************************************************************/

/* checkerCmd::checkScanInCylEndIsPlane 35

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The system must be homed (see p. 48).
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
2. The active tool must be an actual tool.
   CAN_NOT_SCAN_USING_UNDEFTOOL
   CAN_NOT_SCAN_USING_NOTOOL
3. The direction vector of the axis of the cylinder must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
4. The direction vector at the start point must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
5. The direction vector of the stop plane must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
6. The direction vector at the end point must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
7. The start point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
8. The direction point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
9. The start point must differ from the direction point.
   START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT
10. The start point must not be on the cylinder axis.
   START_POINT_MUST_NOT_BE_ON_CYLINDER_AXIS

Reference pages: 24 27 (90 - 91) 103

Rules 3 - 6 are semantic checks.

Rule 10 is explicit on page 91. The check is made by checking that the
absolute value of the cosine of the angle between [the axis vector] and
[the vector from the axis point to the start point] is less than one
(actually, less than 0.9999, to account for rounding error).

*/

void checkerCmd::checkScanInCylEndIsPlane(Command * aCommand, world * aWorld)
{
  ScanInCylEndIsPlaneCommand * theScan;
  double ci;
  double cj;
  double ck;
  double vi;
  double vj;
  double vk;
  double lengthc;
  double lengthv;

  theScan = (ScanInCylEndIsPlaneCommand *)aCommand;
  ci = theScan->getCI();
  cj = theScan->getCJ();
  ck = theScan->getCK();
  vi = (theScan->getSx() - theScan->getCx());
  vj = (theScan->getSy() - theScan->getCy());
  vk = (theScan->getSz() - theScan->getCz());
  lengthc = sqrt((ci * ci) + (cj * cj) + (ck * ck));
  lengthv = sqrt((vi * vi) + (vj * vj) + (vk * vk));
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getUnDefTool())
    errorCode = CAN_NOT_SCAN_USING_UNDEFTOOL;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getNoTool())
    errorCode = CAN_NOT_SCAN_USING_NOTOOL;
  else if (lengthc == 0)
    errorCode = AXIS_DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if ((theScan->getSI() == 0) &&
	   (theScan->getSJ() == 0) &&
	   (theScan->getSK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if ((theScan->getPI() == 0) &&
	   (theScan->getPJ() == 0) &&
	   (theScan->getPK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if ((theScan->getEI() == 0) &&
	   (theScan->getEJ() == 0) &&
	   (theScan->getEK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if (!(aWorld->inWorkVolume
	     (theScan->getSx(), theScan->getSy(), theScan->getSz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if (!(aWorld->inWorkVolume
	     (theScan->getDx(), theScan->getDy(), theScan->getDz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if ((theScan->getSx() == theScan->getDx()) &&
	   (theScan->getSy() == theScan->getDy()) &&
	   (theScan->getSz() == theScan->getDz()))
    errorCode = START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT;
  else if ((lengthv == 0) ||
	   (fabs((((ci * vi) + (cj * vj) + (ck * vk)) / lengthc) / lengthv)
	    > 0.9999))
    errorCode = START_POINT_MUST_NOT_BE_ON_CYLINDER_AXIS;
}

/*******************************************************************/

/* checkerCmd::checkScanInCylEndIsSphere 36

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: 
1. The system must be homed (see p. 48).
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
2. The active tool must be an actual tool.
   CAN_NOT_SCAN_USING_UNDEFTOOL
   CAN_NOT_SCAN_USING_NOTOOL
3. The direction vector of the axis of the cylinder must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
4. The direction vector at the start point must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
5. The direction vector at the end point must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
6. The start point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
7. The direction point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
8. The start point must differ from the direction point.
   START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT
9. The start point must not be on the cylinder axis.
   START_POINT_MUST_NOT_BE_ON_CYLINDER_AXIS

Reference pages: 24 27 (89 - 90) 103

Rules 3 - 5 are semantic checks.

Rule 9 is explicit on page 90. The check is made by checking that the
absolute value of the cosine of the angle between [the axis vector] and
[the vector from the axis point to the start point] is less than one
(actually, less than 0.9999, to account for rounding error).

*/

void checkerCmd::checkScanInCylEndIsSphere(Command * aCommand, world * aWorld)
{
  ScanInCylEndIsSphereCommand * theScan;
  double ci;
  double cj;
  double ck;
  double vi;
  double vj;
  double vk;
  double lengthc;
  double lengthv;

  theScan = (ScanInCylEndIsSphereCommand *)aCommand;
  ci = theScan->getCI();
  cj = theScan->getCJ();
  ck = theScan->getCK();
  vi = (theScan->getSx() - theScan->getCx());
  vj = (theScan->getSy() - theScan->getCy());
  vk = (theScan->getSz() - theScan->getCz());
  lengthc = sqrt((ci * ci) + (cj * cj) + (ck * ck));
  lengthv = sqrt((vi * vi) + (vj * vj) + (vk * vk));
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getUnDefTool())
    errorCode = CAN_NOT_SCAN_USING_UNDEFTOOL;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getNoTool())
    errorCode = CAN_NOT_SCAN_USING_NOTOOL;
  else if (lengthc == 0)
    errorCode = AXIS_DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if ((theScan->getSI() == 0) &&
	   (theScan->getSJ() == 0) &&
	   (theScan->getSK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if ((theScan->getEI() == 0) &&
	   (theScan->getEJ() == 0) &&
	   (theScan->getEK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if (!(aWorld->inWorkVolume
	     (theScan->getSx(), theScan->getSy(), theScan->getSz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if (!(aWorld->inWorkVolume
	     (theScan->getDx(), theScan->getDy(), theScan->getDz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if ((theScan->getSx() == theScan->getDx()) &&
	   (theScan->getSy() == theScan->getDy()) &&
	   (theScan->getSz() == theScan->getDz()))
    errorCode = START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT;
  else if ((lengthv == 0) ||
	   (fabs((((ci * vi) + (cj * vj) + (ck * vk)) / lengthc) / lengthv)
	    > 0.9999))
    errorCode = START_POINT_MUST_NOT_BE_ON_CYLINDER_AXIS;
}

/*******************************************************************/

/* checkerCmd::checkScanInPlaneEndIsCyl 37

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The system must be homed (see p. 48).
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
2. The active tool must be an actual tool.
   CAN_NOT_SCAN_USING_UNDEFTOOL
   CAN_NOT_SCAN_USING_NOTOOL
3. The direction vector of the axis of the cylinder must not have length 0.
   AXIS_DIRECTION_VECTOR_HAS_ZERO_LENGTH
4. The direction vector at the start point must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
5. The direction vector at the end point must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
6. The start point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
7. The direction point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
8. The start point must differ from the direction point.
   START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT

Reference pages: 24 27 (88 - 89) 103

*/

void checkerCmd::checkScanInPlaneEndIsCyl(Command * aCommand, world * aWorld)
{
  ScanInPlaneEndIsCylCommand * theScan;

  theScan = (ScanInPlaneEndIsCylCommand *)aCommand;
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getUnDefTool())
    errorCode = CAN_NOT_SCAN_USING_UNDEFTOOL;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getNoTool())
    errorCode = CAN_NOT_SCAN_USING_NOTOOL;
  else if ((theScan->getCI() == 0) &&
	   (theScan->getCJ() == 0) &&
	   (theScan->getCK() == 0))
    errorCode = AXIS_DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if ((theScan->getSI() == 0) &&
	   (theScan->getSJ() == 0) &&
	   (theScan->getSK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if ((theScan->getEI() == 0) &&
	   (theScan->getEJ() == 0) &&
	   (theScan->getEK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if (!(aWorld->inWorkVolume
	     (theScan->getSx(), theScan->getSy(), theScan->getSz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if (!(aWorld->inWorkVolume
	     (theScan->getDx(), theScan->getDy(), theScan->getDz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if ((theScan->getSx() == theScan->getDx()) &&
	   (theScan->getSy() == theScan->getDy()) &&
	   (theScan->getSz() == theScan->getDz()))
    errorCode = START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT;
}

/*******************************************************************/

/* checkerCmd::checkScanInPlaneEndIsPlane 38

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The system must be homed (see p. 48).
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
2. The active tool must be an actual tool.
   CAN_NOT_SCAN_USING_UNDEFTOOL
   CAN_NOT_SCAN_USING_NOTOOL
3. The direction vector at the start point must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
4. The direction vector of the stop plane must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
5. The direction vector at the end point must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
6. The start point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
7. The direction point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
8. The start point must differ from the direction point.
   START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT

Reference pages: 24 27 (86 - 87) 103

*/

void checkerCmd::checkScanInPlaneEndIsPlane(Command * aCommand, world * aWorld)
{
  ScanInPlaneEndIsPlaneCommand * theScan;

  theScan = (ScanInPlaneEndIsPlaneCommand *)aCommand;
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getUnDefTool())
    errorCode = CAN_NOT_SCAN_USING_UNDEFTOOL;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getNoTool())
    errorCode = CAN_NOT_SCAN_USING_NOTOOL;
  else if ((theScan->getSI() == 0) &&
	   (theScan->getSJ() == 0) &&
	   (theScan->getSK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if ((theScan->getPI() == 0) &&
	   (theScan->getPJ() == 0) &&
	   (theScan->getPK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if ((theScan->getEI() == 0) &&
	   (theScan->getEJ() == 0) &&
	   (theScan->getEK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if (!(aWorld->inWorkVolume
	     (theScan->getSx(), theScan->getSy(), theScan->getSz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if (!(aWorld->inWorkVolume
	     (theScan->getDx(), theScan->getDy(), theScan->getDz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if ((theScan->getSx() == theScan->getDx()) &&
	   (theScan->getSy() == theScan->getDy()) &&
	   (theScan->getSz() == theScan->getDz()))
    errorCode = START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT;
}

/*******************************************************************/

/* checkerCmd::checkScanInPlaneEndIsSphere 39

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The system must be homed (see p. 48).
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
2. The active tool must be an actual tool.
   CAN_NOT_SCAN_USING_UNDEFTOOL
   CAN_NOT_SCAN_USING_NOTOOL
3. The direction vector at the start point must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
4.The direction vector at the end point must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
5. The start point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
6. The end point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
7. The direction point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
8. The start point must differ from the direction point
   START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT

Reference pages: 24 27 (85 - 86) [94] 103

*/

void checkerCmd::checkScanInPlaneEndIsSphere(Command * aCommand, world * aWorld)
{
  ScanInPlaneEndIsSphereCommand * theScan;

  theScan = (ScanInPlaneEndIsSphereCommand *)aCommand;
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getUnDefTool())
    errorCode = CAN_NOT_SCAN_USING_UNDEFTOOL;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getNoTool())
    errorCode = CAN_NOT_SCAN_USING_NOTOOL;
  else if ((theScan->getSI() == 0) &&
	   (theScan->getSJ() == 0) &&
	   (theScan->getSK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if ((theScan->getEI() == 0) &&
	   (theScan->getEJ() == 0) &&
	   (theScan->getEK() == 0))
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if (!(aWorld->inWorkVolume
	     (theScan->getSx(), theScan->getSy(), theScan->getSz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if (!(aWorld->inWorkVolume
	     (theScan->getDx(), theScan->getDy(), theScan->getDz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if (!(aWorld->inWorkVolume
	     (theScan->getEx(), theScan->getEy(), theScan->getEz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if ((theScan->getSx() == theScan->getDx()) &&
	   (theScan->getSy() == theScan->getDy()) &&
	   (theScan->getSz() == theScan->getDz()))
    errorCode = START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT;
}

/*******************************************************************/

/* checkerCmd::checkScanOnCircle 40

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The system must be homed (see p. 48).
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
2. The active tool must be an actual tool.
   CAN_NOT_SCAN_USING_UNDEFTOOL
   CAN_NOT_SCAN_USING_NOTOOL
3. The direction vector of the axis of the circle must not have length 0.
   DIRECTION_VECTOR_HAS_ZERO_LENGTH
4. The radius of the circle must not have length 0.
   RADIUS_MUST_HAVE_POSITIVE_LENGTH
5. The radius of the circle must be perpendicular to its normal
   RADIUS_MUST_BE_PERPENDICULAR_TO_NORMAL
6. The start point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME

Reference pages: 24 27 (82 - 83) [93] 103

It would be useful to add:
5. end point in work volume

*/

void checkerCmd::checkScanOnCircle(Command * aCommand, world * aWorld)
{
  ScanOnCircleCommand * theScan;
  double i;
  double j;
  double k;
  double radx;
  double rady;
  double radz;
  double radl;
  double norml;

  theScan = (ScanOnCircleCommand *)aCommand;
  i = theScan->getI();
  j = theScan->getJ();
  k = theScan->getK();
  norml = sqrt((i * i) + (j * j) + (k * k));
  radx = (theScan->getSx() - theScan->getCx());
  rady = (theScan->getSy() - theScan->getCy());
  radz = (theScan->getSz() - theScan->getCz());
  radl = sqrt((radx * radx) + (rady * rady) + (radz * radz));
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getUnDefTool())
    errorCode = CAN_NOT_SCAN_USING_UNDEFTOOL;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getNoTool())
    errorCode = CAN_NOT_SCAN_USING_NOTOOL;
  else if (norml == 0)
    errorCode = DIRECTION_VECTOR_HAS_ZERO_LENGTH;
  else if (radl == 0)
    errorCode = RADIUS_MUST_HAVE_POSITIVE_LENGTH;
  else if (fabs(((i * radx) + (j * rady) + (k * radz)) / (norml * radl))
	   > IPP_NORMAL_ERROR)
    errorCode = RADIUS_MUST_BE_PERPENDICULAR_TO_NORMAL;
  else if (!(aWorld->inWorkVolume
	     (theScan->getSx(), theScan->getSy(), theScan->getSz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
}

/*******************************************************************/

/* checkerCmd::checkScanOnCircleHint 41

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None

Reference pages: 24 27 (82) [93] 103

*/

void checkerCmd::checkScanOnCircleHint(Command * aCommand, world * aWorld)
{
  
}

/*******************************************************************/

/* checkerCmd::checkScanOnLine 42

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The system must be homed (see p. 48).
   NOT_HOMED_CAN_NOT_EXECUTE_COMMAND
2. The active tool must be an actual tool.
   CAN_NOT_SCAN_USING_UNDEFTOOL
   CAN_NOT_SCAN_USING_NOTOOL
3. The start point must differ from the end point.
   START_POINT_MUST_DIFFER_FROM_END_POINT
4. The start point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME
5. The end point must be in the work volume.
   SCAN_POINT_MUST_BE_IN_WORK_VOLUME

Reference pages: 24 27 (83 - 84) 103

*/

void checkerCmd::checkScanOnLine(Command * aCommand, world * aWorld)
{
  ScanOnLineCommand * theScan;

  theScan = (ScanOnLineCommand *)aCommand;
  if (!(aWorld->getIsHomed()))
    errorCode = NOT_HOMED_CAN_NOT_EXECUTE_COMMAND;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getUnDefTool())
    errorCode = CAN_NOT_SCAN_USING_UNDEFTOOL;
  else if (aWorld->getTools()->getActiveTool() ==
	   aWorld->getTools()->getNoTool())
    errorCode = CAN_NOT_SCAN_USING_NOTOOL;
  else if ((theScan->getSx() == theScan->getEx()) &&
	   (theScan->getSy() == theScan->getEy()) &&
	   (theScan->getSz() == theScan->getEz()))
    errorCode = START_POINT_MUST_DIFFER_FROM_END_POINT;
  else if (!(aWorld->inWorkVolume
	     (theScan->getSx(), theScan->getSy(), theScan->getSz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
  else if (!(aWorld->inWorkVolume
	     (theScan->getEx(), theScan->getEy(), theScan->getEz())))
    errorCode = SCAN_POINT_MUST_BE_IN_WORK_VOLUME;
}

/*******************************************************************/

/* checkerCmd::checkScanOnLineHint 43

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 (83) 103

*/

void checkerCmd::checkScanOnLineHint(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkScanUnknownHint 44

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 27 (85) [93] 103

*/

void checkerCmd::checkScanUnknownHint(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkSetCoordSystem 45

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 28 58 (59) (67) [71] [72] 103

*/

void checkerCmd::checkSetCoordSystem(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkSetCsyTransformation 46

Returned Value: None.

Called By:
 checkerCmd::checkCommand
Rules:
1. Theta must not be negative or more than 180 degrees.
   THETA_MUST_NOT_BE_NEGATIVE_OR_MORE_THAN_180

Reference pages: 24 28 59 (60) 67 [71] [72] (80) 103

Rule 1 is a semantic check and is currently also done in the command
parser.

*/

void checkerCmd::checkSetCsyTransformation(Command * aCommand, world * aWorld)
{
  SetCsyTransformationCommand * theSetCsyTransformation;

  theSetCsyTransformation = (SetCsyTransformationCommand *)aCommand;
  if ((theSetCsyTransformation->getTheta() < 0) ||
      (theSetCsyTransformation->getTheta() > 180))
    errorCode = THETA_MUST_NOT_BE_NEGATIVE_OR_MORE_THAN_180;
}

/*******************************************************************/

/* checkerCmd::checkSetProp 47

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: 
1. Properties of UnDefTool cannot be set by SetProp.
   CAN_NOT_SETPROP_UNDEFTOOL
2. PtMeas parameters of NoTool cannot be set by SetProp.
   CAN_NOT_SETPROP_PTMEASPARS_OF_NOTOOL

Reference pages: 24 26 29 (45) 65 [73] 77 (78) 99 108
Also see the reference pages for GetProp, since several of those imply
that certain properties cannot be set directly.

These rules are not explicit in the spec, but seem implicit.

This is not checking for attempting to cause an out-of-range condition
or attempting to set an unsettable parameter. These checks might be
added. The behavior of updateSetProp in world.cc, combined with the
behavior of the Param class in tools.h handles these cases without error.

*/

void checkerCmd::checkSetProp(Command * aCommand, world * aWorld)
{
  SetPropCommand * theSetProp;
  KTool * theTool;
  int n;

  theSetProp = (SetPropCommand *)aCommand;
  for (n = 0;
       ((errorCode == CHECKER_OK) && (n < theSetProp->getNumberProps()));
       n++)
    {
      if (theSetProp->getKey1(n) == Tool)
	theTool = aWorld->getTools()->getActiveTool();
      else if (theSetProp->getKey1(n) == FoundTool)
	theTool = aWorld->getTools()->getFoundTool();
      else
	theTool = NULL;
      if (theTool && (theTool == aWorld->getTools()->getUnDefTool()))
	errorCode = CAN_NOT_SETPROP_UNDEFTOOL;
      else if (theTool && (theSetProp->getKey2(n) == PtMeasPar) &&
	       (theTool == aWorld->getTools()->getNoTool()))
	errorCode = CAN_NOT_SETPROP_PTMEASPARS_OF_NOTOOL;
    }
}

/*******************************************************************/

/* checkerCmd::checkSetTool 48

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: 
1. The tool must be one of the tools known to the toolchanger.
   UNKNOWN_TOOL
2. The name of the tool must not be "UnDefTool".
   CAN_NOT_SET_TOOL_TO_UNDEFTOOL

Reference pages: 24 27 29 (54 - 55) 102 106

Rule 2 above is not explicit in the spec, but seems implicit. It also
seems implicit that it should be possible to set the tool to "NoTool",
"BaseTool" or "RefTool". These are the same rules as used for ChangeTool.

*/

void checkerCmd::checkSetTool(Command * aCommand, world * aWorld)
{
  SetToolCommand * theSet;

  theSet = (SetToolCommand *)aCommand;
  if (strcmp("UnDefTool", theSet->getToolName()) == 0)
    errorCode = CAN_NOT_SET_TOOL_TO_UNDEFTOOL;
  else if (aWorld->getTools()->findTool(theSet->getToolName()) == NULL)
    errorCode = UNKNOWN_TOOL;
}

/*******************************************************************/

/* checkerCmd::checkStartSession 49

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. A session must not already be in progress.
   ALREADY_IN_SESSION

Reference pages: 18 20 24 26 (38) (41) 49 [71] (77) 81 99

*/

void checkerCmd::checkStartSession(Command * aCommand, world * aWorld)
{
  if (aWorld->getInSession())
    errorCode = ALREADY_IN_SESSION;
}

/*******************************************************************/

/* checkerCmd::checkStopAllDaemons 50

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules: None.

Reference pages: 24 26 (42) (50) 99

The spec lists no errors. It is not an error to call this when no
daemons exist.

*/

void checkerCmd::checkStopAllDaemons(Command * aCommand, world * aWorld)
{

}

/*******************************************************************/

/* checkerCmd::checkStopDaemon 51

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The daemon identified in the command must exist.
   DAEMON_TO_STOP_DOES_NOT_EXIST

Reference pages: 19 (22) 24 26 [40] (42) (50) 99

*/

void checkerCmd::checkStopDaemon(Command * aCommand, world * aWorld)
{
  StopDaemonCommand * theStop;
  int tagNumber;
  
  theStop = (StopDaemonCommand *)aCommand;
  tagNumber = theStop->getDaemonTagNumber();
  if (!(aWorld->daemonExists(tagNumber)))
    errorCode = DAEMON_TO_STOP_DOES_NOT_EXIST;
}

/*******************************************************************/

/* checkerCmd:: checkTiltCenterPart 52

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The two circle centers must not be in the same place.
   AXIS_DIRECTION_VECTOR_HAS_ZERO_LENGTH

Reference pages: 24 27 (97) 102

If the circle centers are in the same place, the vector between them
does not have a direction. This rule is not explicit in the spec, but
if the direction does not exist, the distance to the axis (which is
needed for determining whether the centers are within the limit) does
not exist.

Rule 1 is a semantic check.

*/

void checkerCmd::checkTiltCenterPart(Command * aCommand, world * aWorld)
{
  TiltCenterPartCommand * theTiltCenter;
  double vecX;
  double vecY;
  double vecZ;
  double length;

  theTiltCenter = (TiltCenterPartCommand *)aCommand;
  vecX = (theTiltCenter->getPx1() - theTiltCenter->getPx2());
  vecY = (theTiltCenter->getPy1() - theTiltCenter->getPy2());
  vecZ = (theTiltCenter->getPz1() - theTiltCenter->getPz2());
  length = sqrt((vecX * vecX) + (vecY * vecY) + (vecZ * vecZ));
  if (length == 0)
    errorCode = AXIS_DIRECTION_VECTOR_HAS_ZERO_LENGTH;
}

/*******************************************************************/

/* checkerCmd:: checkTiltPart 53

Returned Value: None.

Called By: checkerCmd::checkCommand

Rules:
1. The length of the axis direction vector must not be 0.
   AXIS_DIRECTION_VECTOR_HAS_ZERO_LENGTH

Reference pages: 24 27 (97) 102

Rule 1 is not explicit in the spec, but if the direction vector has no
length, it has no direction and cannot be used for the required
comparison with the measured direction.

Rule 1 is a semantic check.

*/

void checkerCmd::checkTiltPart(Command * aCommand, world * aWorld)
{
  TiltPartCommand * theTilt;
  double vecX;
  double vecY;
  double vecZ;
  double length;

  theTilt = (TiltPartCommand *)aCommand;
  vecX = theTilt->getDx();
  vecY = theTilt->getDy();
  vecZ = theTilt->getDz();
  length = sqrt((vecX * vecX) + (vecY * vecY) + (vecZ * vecZ));
  if (length == 0)
    errorCode = AXIS_DIRECTION_VECTOR_HAS_ZERO_LENGTH;
}

/*******************************************************************/

/* checkerCmd::checkCommand 54

Returned Value: None.

Called By:
  external functions
  main (in stand-alone command checker)

Rules:
1. If a session is not in progress, only StartSession and EndSession
   may be executed.
   NOT_IN_SESSION_CAN_NOT_EXECUTE_COMMAND
2. If in session and readiness is erred, only GetErrStatusE,
   GetXtdErrStatus, or ClearAllErrors may be executed.
   READINESS_ERRED_CAN_NOT_EXECUTE_COMMAND
3. If readiness is aborted, only GetErrStatusE,
   GetXtdErrStatus, or ClearAllErrors may be executed.
   READINESS_ABORTED_CAN_NOT_EXECUTE_COMMAND

This is allowing AbortE if readiness is aborted or erred, but not if no
session is in progress.

StartSession has the effect of ClearAllErrors (in case an error occurs
during the execution of StartSession).

References:
Rule 1 above: (41)
AbortE: 9 24 26 (42) [43] 99
ClearAllErrors: 24 26 [38] (40) (41) (42) (43) [44] (47) (75) 76 99
StartSession: 18 20 24 26 (38) (41) 49 [71] (77) 81 99

After checking the rules described above, the function calls a checker
specific to the command.

There is currently no command that can call for an abort while a
StartSession command is being executed.

Allowing GetErrStatusE and GetXtdErrStatus when in an error state or
an aborted state (as done by Rules 2 and 3 above) is not currently
supported by the spec, but it should be because otherwise
GetErrStatusE and GetXtdErrStatus have no useful function. The spec
actually says (page 43) that after AbortE, only ClearAllErrors is
allowed, and (page 40) that after an error of severity 2 or greater,
only ClearAllErrors is allowed.

It seems prudent to allow AbortE at any time, even though the spec
does not support that. It was allowed in previous versions of this
command checker (1.0 to 2.1) but is not allowed in this version.

The positions of functions in the checkers array correspond to the
names in the commandNameType enumeration in IppCmdRes.h. The
enumeration has no name for indexes 0 and 1. checkAbortE is
used here in those positions. These indexes will never be used, so
any of the checkXXX functions could have been used there.

*/

void checkerCmd::checkCommand(Command * aCommand, world * aWorld)
{
  static void (checkerCmd::*checkers[])(Command *, world *) = {
  &checkerCmd::checkAbortE,
  &checkerCmd::checkAbortE,
  &checkerCmd::checkAbortE,
  &checkerCmd::checkAlignPart,
  &checkerCmd::checkAlignTool,
  &checkerCmd::checkCenterPart,
  &checkerCmd::checkChangeTool,
  &checkerCmd::checkClearAllErrors,
  &checkerCmd::checkDisableUser,
  &checkerCmd::checkEnableUser,
  &checkerCmd::checkEndSession,
  &checkerCmd::checkEnumAllProp,
  &checkerCmd::checkEnumProp,
  &checkerCmd::checkEnumTools,
  &checkerCmd::checkFindTool,
  &checkerCmd::checkGet,
  &checkerCmd::checkGetChangeToolAction,
  &checkerCmd::checkGetCoordSystem,
  &checkerCmd::checkGetCsyTransformation,
  &checkerCmd::checkGetDMEVersion,
  &checkerCmd::checkGetErrorInfo,
  &checkerCmd::checkGetErrStatusE,
  &checkerCmd::checkGetMachineClass,
  &checkerCmd::checkGetProp,
  &checkerCmd::checkGetPropE,
  &checkerCmd::checkGetXtdErrStatus,
  &checkerCmd::checkGoTo,
  &checkerCmd::checkHome,
  &checkerCmd::checkIsHomed,
  &checkerCmd::checkIsUserEnabled,
  &checkerCmd::checkLockAxis,
  &checkerCmd::checkOnMoveReportE,
  &checkerCmd::checkOnPtMeasReport,
  &checkerCmd::checkOnScanReport,
  &checkerCmd::checkPtMeas,
  &checkerCmd::checkReQualify,
  &checkerCmd::checkScanInCylEndIsPlane,
  &checkerCmd::checkScanInCylEndIsSphere,
  &checkerCmd::checkScanInPlaneEndIsCyl,
  &checkerCmd::checkScanInPlaneEndIsPlane,
  &checkerCmd::checkScanInPlaneEndIsSphere,
  &checkerCmd::checkScanOnCircle,
  &checkerCmd::checkScanOnCircleHint,
  &checkerCmd::checkScanOnLine,
  &checkerCmd::checkScanOnLineHint,
  &checkerCmd::checkScanUnknownHint,
  &checkerCmd::checkSetCoordSystem,
  &checkerCmd::checkSetCsyTransformation,
  &checkerCmd::checkSetProp,
  &checkerCmd::checkSetTool,
  &checkerCmd::checkStartSession,
  &checkerCmd::checkStopAllDaemons,
  &checkerCmd::checkStopDaemon,
  &checkerCmd::checkTiltCenterPart,
  &checkerCmd::checkTiltPart
  };

  errorCode = CHECKER_OK;
  commandName = aCommand->getCommandName();
  if (!(aWorld->getInSession()))
    {
      if ((commandName != StartSession) && (commandName != EndSession))
	errorCode = NOT_IN_SESSION_CAN_NOT_EXECUTE_COMMAND;
    }
  if (errorCode == CHECKER_OK)
    {
      if ((aWorld->getReadiness() == erred) &&
	  (commandName != GetErrStatusE) &&
	  (commandName != GetXtdErrStatus) &&
	  (commandName != ClearAllErrors))
	errorCode = READINESS_ERRED_CAN_NOT_EXECUTE_COMMAND;
      else if ((aWorld->getReadiness() == aborted) &&
	       (commandName != GetErrStatusE) &&
	       (commandName != GetXtdErrStatus) &&
	       (commandName != ClearAllErrors))
	errorCode = READINESS_ABORTED_CAN_NOT_EXECUTE_COMMAND;
    }
  if (errorCode == CHECKER_OK)
    (*this.*checkers[commandName])(aCommand, aWorld);
}

/*******************************************************************/

