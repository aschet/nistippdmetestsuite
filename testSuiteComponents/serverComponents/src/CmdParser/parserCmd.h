/* parserCmd.h

*/

#ifndef PARSERCMD_HH
#define PARSERCMD_HH

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INITARGS 64

/* error codes

*/

typedef enum parserCmdErrorId {
//A
OK = 0,
ARGUMENT_FIRST_PART_MUST_BE_TOOL_OR_FOUNDTOOL,
ARGUMENT_FOURTH_PART_MUST_BE_ACT,
ARGUMENT_FOURTH_PART_BAD,
ARGUMENT_MUST_BE_A_NUMBER,
//B
ARGUMENT_MUST_BE_EVENT_TAG,
ARGUMENT_MUST_BE_INTEGER,
ARGUMENT_MUST_BE_POSITIVE,
ARGUMENT_MUST_BE_STRING,
ARGUMENT_MUST_END_WITH_EMPTY_PARENTHESES,
//C
ARGUMENT_MUST_END_WITH_NUMBER_IN_PARENTHESES,
ARGUMENT_SECOND_PART_BAD,
ARGUMENT_SECOND_PART_MUST_BE_GOTOPAR_OR_PTMEASPAR,
ARGUMENT_THIRD_PART_BAD,
BAD_ARGUMENTS,
//D
BAD_CHARACTER_AFTER_COMMAND_END,
BAD_CHARACTER_AFTER_KEYWORD,
BAD_COMMAND_ARGUMENTS,
BAD_COMMAND_NAME,
BAD_DOTS,
//E
BAD_E_NUMBER_EXPONENT_MUST_HAVE_ONE_TWO_OR_THREE_DIGITS,
BAD_FIRST_ARGUMENT,
BAD_IJK_NUMBERS,
BAD_KEYWORD,
BAD_NUMBER_MORE_THAN_16_DIGITS,
//F
BAD_NUMBER_NO_DIGITS,
BAD_NUMBER_OF_THROUGHS,
BAD_NUMBER_TWO_DECIMAL_POINTS,
BAD_STRING,
BAD_TAG_CHARACTER,
//G
C_ARGUMENT_USED_TWICE,
DIAMETER_MUST_BE_POSITIVE,
DIS_ARGUMENT_USED_TWICE,
DIS_MUST_BE_FOLLOWED_BY_NUMBER_IN_PARENS,
ER_ARGUMENT_USED_TWICE,
//H
EVENT_COMMAND_MUST_HAVE_EVENT_TAG,
IJK_ARGUMENT_USED_TWICE,
IJK_NOT_FOLLOWED_BY_ARGIJK,
ILLEGAL_SPACE,
MUST_HAVE_NO_ARGUMENTS,
//I
MUST_HAVE_ONE_ARGUMENT,
MUST_HAVE_4_ARGUMENTS,
MUST_HAVE_4_OR_8_ARGUMENTS,
MUST_HAVE_7_ARGUMENTS,
MUST_HAVE_7_OR_14_ARGUMENTS,
//J
NON_EVENT_COMMAND_MUST_NOT_HAVE_EVENT_TAG,
NUMBER_THROUGH_MUST_BE_INTEGER,
NUMBER_THROUGH_MUST_BE_POSITIVE,
PHI_ARGUMENT_USED_TWICE,
Q_ARGUMENT_USED_TWICE,
//K
R_ARGUMENT_USED_TWICE,
SPACE_MISSING_AFTER_TAG,
STEPW_MUST_BE_POSITIVE,
TAG_NUMBER_OUT_OF_RANGE_FOR_EVENT_TAG,
TAG_NUMBER_OUT_OF_RANGE_FOR_COMMAND_TAG,
//L
THETA_OUT_OF_RANGE,
TIME_ARGUMENT_USED_TWICE,
TIME_MUST_BE_FOLLOWED_BY_NUMBER_IN_PARENS,
TOOL_A_ARGUMENT_USED_TWICE,
TOOL_B_ARGUMENT_USED_TWICE,
//M
TOOL_C_ARGUMENT_USED_TWICE,
X_ARGUMENT_USED_TWICE,
X_NOT_FOLLOWED_BY_NUMBER_IN_PARENS,
X_Y_Z_ALL_MISSING,
Y_ARGUMENT_USED_TWICE,
//N
Y_NOT_FOLLOWED_BY_NUMBER_IN_PARENS,
Z_ARGUMENT_USED_TWICE,
Z_NOT_FOLLOWED_BY_NUMBER_IN_PARENS
};

/* argItemType

argItemType is an enumeration of the types of component that may be in
an argument list.  Periods, commas, empty parentheses, and parentheses
with doubles in them are included, as well as etags, keywords,
doubles, ints, and strings.  

Example 1. For the command GoTo(X(20), Y(15)), there are five argument
components, and the types are:
ARGKEYWORD, ARGPARENDOUBLE, ARGCOMMA, ARGKEYWORD, ARGPARENDOUBLE.

Example 2. For the command OnPtMeasReport(X(), Y(), Z()) there are eight
argument components, and the types are: ARGKEYWORD, ARGPARENEMPTY, ARGCOMMA,
ARGKEYWORD, ARGPARENEMPTY, ARGCOMMA, ARGKEYWORD, ARGPARENEMPTY.

Example 3. For the command GetProp(Tool.PtMeasPar.Accel()), there are six
argument components, and the types are:
ARGKEYWORD, ARGDOT, ARGKEYWORD, ARGDOT, ARGKEYWORD, ARGPARENEMPTY.

The ARGETAG type appears correctly only as the argument to StopDaemon.

The ARGIJK type appears correctly only as an argument to PtMeas.

The ARGINT type is not defined. Where a number must be an integer (as
in several scan commands), it is read as a double and later checked
for being an integer.

*/

typedef enum argItemType {ARGCOMMA, ARGDOT, ARGDOUBLE, ARGETAG, ARGIJK,
			 ARGKEYWORD, ARGPARENEMPTY, ARGPARENDOUBLE, ARGSTRING};

/*******************************************************************/

/* parserCmd

To use the parser, the setInput() method must first be called to copy an
input string into the parser's inputArray. An alternative would be to
have a string as an argument to the parseXXX methods. Copying to
inputArray takes time, but it prevents the parser from messing up the
original string.

parseTag() parses the tag, and sets the parser's isEvent, tag,
tagType, and errorCode.  It is intended that if getParserErr() is OK
(no error in the tag) following the call to parseTag(), the caller
will:

1. check that the tag returned by getTag() is not in use.
2. call getIsEvent() to determine if the tag is an event tag.
3. put the command string on fast queue or the slow queue of the world.

When it is time to execute a command string (because it is at the head
of a queue), parseCommand() should be called to parse the command
string and build an instance of a command.

After calling parseCommand(), the caller should call getParserErr()
to see if there was an error. If the returned error code is not OK,
the caller can call getErrorMessageString(buffer) to get an error message
that describes the error. The buffer should be at least 100 chars long.
If no error occurs until after the name of the command is parsed, the
name of the command is given at the beginning of the error message.

If the error code is OK, the caller can start processing the Command
instance.

*/

class parserCmd
{
public:
  parserCmd()
    {
      argCount = 0;
      argSize = INITARGS;
      argDoubles = new double[argSize];
      argKeywords = new int[argSize];
      argStrings = new char * [argSize]; // might set all to 0
      argTypes = new int[argSize];
      arrayIndex = 0;
    }
  ~parserCmd()
    {
      delete argDoubles;
      delete argKeywords;
      delete argStrings;  // might delete the strings, too
      delete argTypes;
    }
  char * getErrorMessageString(char * buffer); // might add length argument
  static const int getIppErr(int code);
  int getIsEvent() {return isEvent;}
  int getParserErr(){return errorCode;}
  int getTag() {return tag;}
  Command * parseCommand();
  void parseTag();
  void setInput(char * input){strcpy(inputArray, input);} /* strncpy better */
  
private:
  int argCount;              // number of argument components
  double * argDoubles;       // array of doubles for arguments
  unsigned int argInt;       // used only for tag numbers
  int * argKeywords;         // array of keywords for arguments
  int argSize;               // size of argDoubles, argKeywords, etc. 
  char ** argStrings;        // array of pointers to strings for arguments
  int * argTypes;            // array of argument types
  double argIJK[3];          // place to save one IJK argument
  int arrayIndex;            // current index for inputArray
  int commandName;           // enumeration value of command
  int errorCode;             // parser error code
  char inputArray[IPPSIZE];  // array to put command in for parsing
  int isEvent;               // flag indicating event command
  int tag;                   // tag number of command being parsed

  void makeArraysBigger();
  Command * makeCommand();
  Command * makeAbortE();
  Command * makeAlignPart();
  Command * makeAlignTool();
  Command * makeCenterPart();
  Command * makeChangeTool();
  Command * makeClearAllErrors();
  Command * makeDisableUser();
  Command * makeEnableUser();
  Command * makeEndSession();
  Command * makeEnumAllProp();
  Command * makeEnumProp();
  Command * makeEnumTools();
  Command * makeFindTool();
  Command * makeGet();
  Command * makeGetChangeToolAction();
  Command * makeGetCoordSystem();
  Command * makeGetCsyTransformation();
  Command * makeGetDMEVersion();
  Command * makeGetErrorInfo();
  Command * makeGetErrStatusE();
  Command * makeGetMachineClass();
  Command * makeGetProp();
  Command * makeGetPropE();
  Command * makeGetXtdErrStatus();
  Command * makeGoTo();
  Command * makeHome();
  Command * makeIsHomed();
  Command * makeIsUserEnabled();
  Command * makeLockAxis();
  Command * makeOnMoveReportE();
  Command * makeOnPtMeasReport();
  Command * makeOnScanReport();
  Command * makePtMeas();
  Command * makeReQualify();
  Command * makeScanInCylEndIsPlane();
  Command * makeScanInCylEndIsSphere();
  Command * makeScanInPlaneEndIsCyl();
  Command * makeScanInPlaneEndIsPlane();
  Command * makeScanInPlaneEndIsSphere();
  Command * makeScanOnCircle();
  Command * makeScanOnCircleHint();
  Command * makeScanOnLine();
  Command * makeScanOnLineHint();
  Command * makeScanUnknownHint();
  Command * makeSetCoordSystem();
  Command * makeSetCsyTransformation();
  Command * makeSetProp();
  Command * makeSetTool();
  Command * makeStartSession();
  Command * makeStopAllDaemons();
  Command * makeStopDaemon();
  Command * makeTiltCenterPart();
  Command * makeTiltPart();
  void parseCmdArguments();
  void parseCmdName();
  void parseIJK();
  void parseKeyword();
  void parseNumber(double * store);
  void parseString();
  void unSpaceInputArray();
};
#endif /* ifndef PARSERCMD_HH */
