/* parserRes.cc

This is a response parser for version 1.40 of the I++ DME Spec.

This defines functions in the parserRes class and defines a main function
outside the parserRes class.

Functions named makeXXX (where XXX is a response name) are used to
check the components of each type of response and to return an instance
of the response.

References to the responses are given here. The reference pages
reference both text and examples. Text references are given in
parentheses. Example references are given in brackets.  Other
references are not enclosed.

Example: Reference pages: 23 26 (39) [40] 85
This means there is relevant text on page 39 and an example on page 40.

*/

#include "IppCmdRes.h"
#include "parserRes.h"

/*******************************************************************/

/* remove the // on the next line to build a stand-alone response parser */
//#define PARSER_MAIN

#ifdef PARSER_MAIN

/*******************************************************************/

/* main

This main function is compiled only if PARSER_MAIN is defined.

When this function starts, it asks the user to enter "y" or "n" to
answer the question, "turn stepping on?". If the user enters y
(followed by <Enter>), stepping is turned on. Otherwise, stepping is
kept off. When stepping is on, the function parses one response at a
time, waiting for the user to press <Enter> before going on to the
next response. When stepping is off, the function parses an entire
file of responses without pausing.

The function has an outer loop that asks the user to enter the name of
a response file to parse, or to enter q to quit. If the name of a
response file is entered, the function parses it in the inner loop.

The inner loop then repeatedly does the following:
1. reads a response string from the response file,
2. calls the parser's parseResponse method. If parsing succeeds,
   parseResponse makes an instance of a Response, sets the parser's
   errorCode to OK and returns a pointer to the response. If parsing
   fails, the parser's errorCode is set to some code that is not OK and
   a NULL pointer is returned.
3. calls the parser's getParserError method (which returns the parser's
   errorCode).
   a. If this returns OK, the main function then prints the Response
      (from the Response class instance, not from the input string)
      followed by \\ on a separate line.
   b. If this does not return OK, the main function prints the text
      of the input string, followed by the error message caused by
      the input string.
When the inner loop is finished with the response file, the outer loop
asks for another response file name or q to quit.

*/

main()
{
  FILE * infile;
  parserRes parser1;
  char buffer[IPPSIZE];
  char responseText[IPPSIZE];
  char errorMessage[100];
  int k;
  int c;
  Response * aResponse;
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
      infile = fopen(buffer, "rb"); // need "rb" for PC
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
	      fprintf(stderr, "response too long\n");
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
	  aResponse = parser1.parseResponse();
	  code = parser1.getParserErr();
	  if (code != OK)
	    {
	      fprintf(stderr, "%s", buffer);
	      parser1.getErrorMessageString(errorMessage);
	      fprintf(stderr, "%s\n", errorMessage);
	      if (step)
		getchar();
	      continue;
	    }
	  else if (aResponse == NULL)
	    { // if aResponse is NULL, code should not be OK
	      fprintf(stderr, "bug in main\n");
	      exit(1);
	    }
	  else 
	    {
	      if (aResponse->getResponseString(responseText, (IPPSIZE - 1)))
		printf("%s%c%c%c%c", responseText, 92, 92, 13, 10);
	      else
		fprintf(stderr, "response did not print\n");
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

/* parserRes::getErrorMessage

Returned Value : char *
  If there is a problem with providing a string, this returns NULL.
  Otherwise, it returns the buffer argument.

Called By:
  external functions
  main (in stand-alone response parser)

Errors: None.

This copies an error message into the buffer.

The symbol for each error code is the same as the text of the error
message, except that the symbol has underscores. The symbol serves as
an index into the array of strings defined here. The symbols are
defined in parserRes.h.

*/

char * parserRes::getErrorMessageString(char * buffer)
{
  static const char * const errorStrings[] = {
    "OK",
    "BAD CHARACTER AFTER KEYWORD",
    "BAD CHARACTER AFTER RESPONSE END",
    "BAD COORDINATE SYSTEM",
    "BAD E NUMBER EXPONENT MUST HAVE ONE TWO OR THREE DIGITS",
    "BAD ERROR CODE",
    "BAD GOTOPAR PROPERTY",
    "BAD KEYWORD",
    "BAD MACHINE CLASS",
    "BAD NUMBER MORE THAN 16 DIGITS",
    "BAD NUMBER NO DIGITS",
    "BAD NUMBER TWO DECIMAL POINTS",
    "BAD PTMEASPAR PROPERTY",
    "BAD RESPONSE ITEMS",
    "BAD SECOND KEYWORD",
    "BAD SEVENTH CHARACTER IN RESPONSE",
    "BAD SEVERITY",
    "BAD STRING",
    "BAD TAG CHARACTER",
    "BAD TEXT",
    "BAD THIRD KEYWORD",
    "BUG",
    "ER USED TWICE",
    "ERROR CODE MUST HAVE FOUR DIGITS",
    "FIRST ARGUMENT MUST BE SWITCH ROTATE MOVEAUTO OR MOVEMAN",
    "IJK USED TWICE",
    "IJK VALUES MUST BE ZERO",
    "ILLEGAL SPACE",
    "R USED TWICE",
    "RESPONSE NAME MUST BE ERROR",
    "SEVERITY MUST HAVE ONE DIGIT",
    "SPACE MISSING AFTER TAG",
    "SPACE MISSING AT EIGHTH CHARACTER OF RESPONSE",
    "TAG NUMBER OUT OF RANGE FOR COMMAND TAG",
    "TOOL A USED TWICE",
    "TOOL B USED TWICE",
    "TOOL C USED TWICE",
    "VALUE MUST BE 0 OR 1",
    "X USED TWICE",
    "Y USED TWICE",
    "Z USED TWICE",
    "ZERO TAG MUST BE SERVER INITIATED RESPONSE",
  };
  if ((responseName > -1) && (responseName < 27))
    sprintf(buffer, "%s: %s",
            ippResponseNameStrings[responseName], errorStrings[errorCode]);
  else
    sprintf(buffer, "%s", errorStrings[errorCode]);
  return buffer;
}

/*******************************************************************/

/* parserCmd::makeArraysBigger

Returned Value: None.

Called By: parserRes::parseArguments

Rules: None.

This doubles the sizes of the four arrays in which response items are stored.
It also doubles resSize.

This makes new arrays twice the size of the old arrays and copies the
contents of the old arrays into the first half of the new arrays.
The old arrays are deleted.

*/

void parserRes::makeArraysBigger()
{
  double * oldResDoubles;
  int * oldResKeywords;
  int * oldResTypes;
  char ** oldResStrings;
  int n;
    
  oldResDoubles = resDoubles;
  oldResKeywords = resKeywords;
  oldResStrings = resStrings;
  oldResTypes = resTypes;
  resDoubles = new double[2 * resSize];
  resKeywords = new int[2 * resSize];
  resStrings = new char * [2 * resSize];
  resTypes = new int[2 * resSize];
  for (n = 0; n < resSize; n++)
    {
      resDoubles[n] = oldResDoubles[n];
      resKeywords[n] = oldResKeywords[n];
      resStrings[n] = oldResStrings[n];
      resTypes[n] = oldResTypes[n];
    }
  delete oldResDoubles;
  delete oldResKeywords;
  delete oldResStrings;
  delete oldResTypes;
  resSize = (2 * resSize);
}

/*******************************************************************/

/* parserRes::makeAlignResponse

Returned Value: An AlignResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. If the response does not match either of the patterns
   (double, double, double)
   (double, double, double, double, double, double)   :
   BAD_RESPONSE_ITEMS.

This is called when the first data character of a data response is
a left parenthesis, because the only valid response which has a left
parenthesis in that position is an AlignResponse.

An AlignResponse responds to either an AlignTool command or an AlignPart
command.

Example 1. 00010 # (0.6, 0.0, 0.8)
Example 2. 00010 # (1, 0, 0, 0, 0, 1)

AlignPart Reference pages: 24 28 63 (95) 105
AlignTool Reference pages: 24 27 29 (55) 66 70 102 107

*/

Response * parserRes::makeAlignResponse()
{
  responseName = DataAlign;
  if (((itemCount != 7) && (itemCount != 13)) ||
      (resTypes[0] != RESPARENLEFT) ||
      (resTypes[1] != RESDOUBLE) ||
      (resTypes[2] != RESCOMMA)  ||
      (resTypes[3] != RESDOUBLE) ||
      (resTypes[4] != RESCOMMA)  ||
      (resTypes[5] != RESDOUBLE) ||
      ((itemCount == 7) &&
       (resTypes[6] != RESPARENRIGHT)) ||
      ((itemCount == 13) &&
       ((resTypes[6]  != RESCOMMA)  ||
	(resTypes[7]  != RESDOUBLE) ||
	(resTypes[8]  != RESCOMMA)  ||
	(resTypes[9]  != RESDOUBLE) ||
	(resTypes[10] != RESCOMMA)  ||
	(resTypes[11] != RESDOUBLE) ||
	(resTypes[12] != RESPARENRIGHT))))
    errorCode = BAD_RESPONSE_ITEMS;
  return ((errorCode != OK) ? NULL :
	  new AlignResponse(tag, resDoubles[1], resDoubles[3],
			    resDoubles[5], resDoubles[7], 
			    resDoubles[9], resDoubles[11],
			    (itemCount == 13)));
}

/*******************************************************************/

/* parserRes::makeCenterPartResponse

Returned Value: A CenterPartResponse as described by the response text,
   or NULL if the response text is bad.

Called By: parserRes::makeDataResponse

Errors:
1. If the response items do not match the pattern
   CenterPart(double)
   or the double is not a si ngle character: BAD_RESPONSE_ITEMS.
2. If the double is not 0 or 1: VALUE_MUST_BE_0_OR_1;

This function is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == CenterPart), so those conditions are not checked here.

The check on inputArray[20] ensures there is only one character between
parentheses.

A CenterPartResponse responds to a CenterPart command.

Example 1. 00010 # CenterPart(1)

Reference pages: 24 27 (96) 102

*/

Response * parserRes::makeCenterPartResponse()
{
  responseName = DataCenterPart;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESDOUBLE)    ||
      (resTypes[3] != RESPARENRIGHT)||
      (inputArray[20] != ')'))
    errorCode = BAD_RESPONSE_ITEMS;
  else if ((resDoubles[2] != 0) && (resDoubles[2] != 1))
    errorCode = VALUE_MUST_BE_0_OR_1;
  return ((errorCode != OK) ? NULL :
	  new CenterPartResponse(tag, ((resDoubles[2] == 1) ? true : false)));
}

/*******************************************************************/

/* parserRes::makeChangeToolEResponse

Returned Value: A ChangeToolEResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::makeServerInitiatedResponse

Errors:
  1. If the response does not have the pattern
     ChangeTool(string)
     BAD_RESPONSE_ITEMS

This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == ChangeToolKey), so those conditions are not checked here.

A ChangeToolE response is server-initiated and is not a response to a command.

Example 1. E0000 # ChangeTool("Probe1")

Reference pages: (78)

*/

Response * parserRes::makeChangeToolEResponse()
{
  responseName = DataChangeToolE;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESSTRING)    ||
      (resTypes[3] != RESPARENRIGHT))
    errorCode = BAD_RESPONSE_ITEMS;
  return ((errorCode != OK) ? NULL :
	  new ChangeToolEResponse(resStrings[2]));
}

/*******************************************************************/

/* parserRes::makeDataResponse

Returned Value: A data Response of the appropriate type as described
   by the response text, or NULL if the response text is bad.

Called By:  parserRes::parseResponse

Errors:
1. The response items do not conform to any defined pattern of response
   items: BAD_RESPONSE_ITEMS.

This examines the type of the first response item (and possibly other
information about response items), decides what type of response it must
be, and calls a subordinate function to make a response of that type.

This is called if the seventh character of the response is # and the
tag is OK but is not E0000.

*/

Response * parserRes::makeDataResponse()
{
  Response * aResponse;

  if (resTypes[0] == RESPARENLEFT)
    aResponse = makeAlignResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == CenterPartKey))
    aResponse = makeCenterPartResponse();
  else if ((resTypes[0] == RESSTRING) && (itemCount == 3))
    aResponse = makeEnumPropResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == ChangeToolAction))
    aResponse = makeGetChangeToolActionResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == CoordSystem))
    aResponse = makeGetCoordSystemResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == GetCsyTr))
    aResponse = makeGetCsyTransformationResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == DMEVersion))
    aResponse = makeGetDMEVersionResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == ErrStatus))
    aResponse = makeGetErrStatusEResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == GetMachineKey))
    aResponse = makeGetMachineClassResponse();
  else if ((resTypes[0] == RESKEYWORD) &&
	   ((resKeywords[0] == FoundTool) ||
	    ((resKeywords[0] == Tool) &&
	     ((itemCount < 3) || (resTypes[2] != RESKEYWORD) ||
	      ((resKeywords[2] != A) &&
	       (resKeywords[2] != B) &&
	       (resKeywords[2] != C))))))
    aResponse = makeGetPropResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == IsHomedKey))
    aResponse = makeIsHomedResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == IsUserEnabledKey))
    aResponse = makeIsUserEnabledResponse();
  else if ((resTypes[0] == RESKEYWORD) &&
	   ((resKeywords[0] == ER)  ||
	    (resKeywords[0] == IJK) ||
	    (resKeywords[0] == R)   ||
	    (resKeywords[0] == X)   ||
	    (resKeywords[0] == Y)   ||
	    (resKeywords[0] == Z)   ||
	    (resKeywords[0] == Tool)))
    aResponse = makePtMeasOrGetResponse();
  else if (resTypes[0] == RESDOUBLE)
    aResponse = makeScanResponse();
  else if ((resTypes[0] == RESSTRING) && (itemCount == 1))
    aResponse = makeStringResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == TiltCenterPartKey))
    aResponse = makeTiltCenterPartResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == TiltPartKey))
    aResponse = makeTiltPartResponse();
  else
    {
      aResponse = NULL;
      errorCode = BAD_RESPONSE_ITEMS;
    }
  return aResponse;
}

/*******************************************************************/

/* parserRes::makeEnumPropResponse

Returned Value: An EnumPropResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. If the response items do not match the pattern
   string, string
   BAD_RESPONSE_ITEMS

This is called when (itemCount == 3) and (resTypes[0] == RESSTRING),
so those conditions are not checked here.

An EnumPropResponse responds to an EnumProp command or an EnumAllProp command.

Example 1. 00020 # "GoToPar", "Property"

EnumAllProp Reference pages: 24 26 45 (46) [74] 99
EnumProp Reference pages: 24 26 29 (45) [73] 99 108 109 110 111

*/

Response * parserRes::makeEnumPropResponse()
{
  responseName = DataEnumProp;
  if ((resTypes[1] != RESCOMMA) ||
      (resTypes[2] != RESSTRING))
    errorCode = BAD_RESPONSE_ITEMS;
  return ((errorCode != OK) ? NULL :
	  new EnumPropResponse(tag, resStrings[0], resStrings[2]));
}

/*******************************************************************/

/* parserRes::makeErrorResponse

Returned Value: An ErrorResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::parseResponse

Errors:
1. If the response does not match the pattern
   keyword(double, double, string, string):
   BAD_RESPONSE_ITEMS.
2. If the keyword is not Error: RESPONSE_NAME_MUST_BE_ERROR.
3. If the first double (the severity) has more than one digit:
   SEVERITY_MUST_HAVE_ONE_DIGIT.
4. If the second double (the error code) does not consist of four digits:
   ERROR_CODE_IS_MUST_HAVE_FOUR_DIGITS.
5. If the severity is 4 (which happens only if the error code is not a
   defined error code): BAD_ERROR_CODE.
6. If the error text is not the correct error text for the given error
   code: BAD_TEXT.
7. If the severity is not the correct severity for the given error code:
   BAD_SEVERITY.

This is called when an exclamation point follows the tag.

An ErrorResponse responds to any command whose execution causes an error.
An ErrorResponse may also be server-initiated.

Example 1. 00010 ! Error(3,1008, "Do", "Target position out of machine volume")
Example 2. E0000 ! Error(3, 1009, "pressureCheck", "Air pressure out of range")

Reference pages: 20 26 30 31 33-35 [36] 37 38 [38] 39 [39]
                 40 43 [43] [44] (75-76) 100 101 113
These reference pages cover only the format and use of error messages.
They do not cover the specific errors that can occur for each command.
They do not cover ClearAllErrors, GetErrStatusE, or GetXtdErrStatus.

*/

Response * parserRes::makeErrorResponse()
{
  char severity;
  int  code;
  ErrorResponse * aResponse;

  responseName = ErrorError;
  if ((itemCount != 10) ||
      (resTypes[0] != RESKEYWORD)   ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESDOUBLE) ||
      (resTypes[3] != RESCOMMA)  ||
      (resTypes[4] != RESDOUBLE) ||
      (resTypes[5] != RESCOMMA)  ||
      (resTypes[6] != RESSTRING) ||
      (resTypes[7] != RESCOMMA)  ||
      (resTypes[8] != RESSTRING) ||
      (resTypes[9] != RESPARENRIGHT))
    errorCode = BAD_RESPONSE_ITEMS;
  else if (resKeywords[0] != ErrorKey)
    errorCode = RESPONSE_NAME_MUST_BE_ERROR;
  else if (inputArray[15] != ',')
    errorCode = SEVERITY_MUST_HAVE_ONE_DIGIT;
  else if ((inputArray[16] < 48) || (inputArray[16] > 57) ||
	   (inputArray[17] < 48) || (inputArray[17] > 57) ||
	   (inputArray[18] < 48) || (inputArray[18] > 57) ||
	   (inputArray[19] < 48) || (inputArray[19] > 57) ||
	   (inputArray[20] != ','))
    errorCode = ERROR_CODE_MUST_HAVE_FOUR_DIGITS;
  else
    {
      severity = ((resDoubles[2] == 0) ? '0' :
		  (resDoubles[2] == 1) ? '1' :
		  (resDoubles[2] == 2) ? '2' :
		  (resDoubles[2] == 3) ? '3' :
		  (resDoubles[2] == 9) ? '9' : '4');
      code = (int)resDoubles[4];
      aResponse = new ErrorResponse(tag, tagType, code, resStrings[6]);
      if (aResponse->getTheError()->getSeverity() == '4')
	errorCode = BAD_ERROR_CODE;
      else if (strcmp(aResponse->getTheError()->getText(), resStrings[8]))
	errorCode = BAD_TEXT;
      else if (severity != aResponse->getTheError()->getSeverity())
	errorCode = BAD_SEVERITY;
      if (errorCode != OK)
	delete aResponse;
    }
  return ((errorCode != OK) ? NULL : aResponse);
}

/*******************************************************************/

/* parserRes::makeGetChangeToolActionResponse

Returned Value: A GetChangeToolActionResponse as described by the response
   text, or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
  1. The response does not have the pattern
     ChangeToolAction(Arg, X(<number>), Y(<number>), Z(<number>))
     BAD_RESPONSE_ITEMS
  2. The arg is not SWITCH, ROTATE, MOVEAUTO, MOVEMAN
     FIRST_ARGUMENT_MUST_BE_SWITCH_ROTATE_MOVEAUTO_OR_MOVEMAN
  
This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == ChangeToolAction), so those conditions are not checked here.

A ChangeToolActionResponse responds to a GetChangeToolAction command.

Example 1. 00010 # ChangeToolAction(Switch, X(1), Y(2), Z(3))

Reference pages: 24 27 29 (57-58) 102 106

*/

Response * parserRes::makeGetChangeToolActionResponse()
{
  responseName = DataGetChangeToolAction;
  if ((itemCount != 19) ||
      (resTypes[1]  != RESPARENLEFT)  ||
      (resTypes[2]  != RESKEYWORD)    ||
      (resTypes[3]  != RESCOMMA)      ||
      (resTypes[4]  != RESKEYWORD)    || (resKeywords[4] != X) ||
      (resTypes[5]  != RESPARENLEFT)  ||
      (resTypes[6]  != RESDOUBLE)     ||
      (resTypes[7]  != RESPARENRIGHT) ||
      (resTypes[8]  != RESCOMMA)      ||
      (resTypes[9]  != RESKEYWORD)    || (resKeywords[9] != Y) ||
      (resTypes[10] != RESPARENLEFT)  ||
      (resTypes[11] != RESDOUBLE)     ||
      (resTypes[12] != RESPARENRIGHT) ||
      (resTypes[13] != RESCOMMA)      ||
      (resTypes[14] != RESKEYWORD)    || (resKeywords[14] != Z) ||
      (resTypes[15] != RESPARENLEFT)  ||
      (resTypes[16] != RESDOUBLE)     ||
      (resTypes[17] != RESPARENRIGHT) ||
      (resTypes[18] != RESPARENRIGHT))
    errorCode = BAD_RESPONSE_ITEMS;
  else if
    ((resKeywords[2] != Switch) && 
     (resKeywords[2] != Rotate) && 
     (resKeywords[2] != MoveAuto) && 
     (resKeywords[2] != MoveMan))
    errorCode = FIRST_ARGUMENT_MUST_BE_SWITCH_ROTATE_MOVEAUTO_OR_MOVEMAN;
  return ((errorCode != OK) ? NULL :
	  new GetChangeToolActionResponse(tag,
					  (changeToolActionType)resKeywords[2],
					  resDoubles[6],
					  resDoubles[11],
					  resDoubles[16]));
}

/*******************************************************************/

/* parserRes::makeGetCoordSystemResponse

Returned Value: A GetCoordSystemResponse as described by the response
   text, or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. If the response items do not match the pattern
   CoordSystem(keyword)
   BAD_RESPONSE_ITEMS.
2. If the keyword is not MachineCsy, MoveableMachineCsy, MultipleArmCsy
   or PartCsy:
   BAD_COORDINATE_SYSTEM.

This function is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == CoordSystem), so those conditions are not checked here.

A GetCoordSystemResponse responds to a GetCoordSystem command.

Example 1. 00010 # CoordSystem(PartCsy)

Reference pages: 24 28 (59) 103

*/

Response * parserRes::makeGetCoordSystemResponse()
{
  responseName = DataGetCoordSystem;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESKEYWORD)   ||
      (resTypes[3] != RESPARENRIGHT))
    errorCode = BAD_RESPONSE_ITEMS;
  else if ((resKeywords[2] < MachineCsy) || (resKeywords[2] > PartCsy))
    errorCode = BAD_COORDINATE_SYSTEM;
  return ((errorCode != OK) ? NULL :
	  new GetCoordSystemResponse(tag, (coordSysType)resKeywords[2]));
}

/*******************************************************************/

/* parserRes::makeGetCsyTransformationResponse

Returned Value: A GetCsyTransformationResponse as described by the
   response text, or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. If the response items do not match the pattern:
   GetCsyTransformation(double, double, double, double, double, double)
   BAD_RESPONSE_ITEMS.

This function is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == GetCsyTr), so those conditions are not checked here.

A GetCsyTransformationResponse responds to a GetCsyTransformation command.

Example 1. 00040 # GetCsyTransformation(1,2,3,4,5,6)

Reference pages: 24 28 (60) (80) 103

Note: The response does not identify the coordinate system. Release 2.1
mistakenly included that.

*/

Response * parserRes::makeGetCsyTransformationResponse()
{
  responseName = DataGetCsyTransformation;
  if ((itemCount != 14) ||
      (resTypes[1]  != RESPARENLEFT) ||
      (resTypes[2]  != RESDOUBLE)    ||
      (resTypes[3]  != RESCOMMA)     ||
      (resTypes[4]  != RESDOUBLE)    ||
      (resTypes[5]  != RESCOMMA)     ||
      (resTypes[6]  != RESDOUBLE)    ||
      (resTypes[7]  != RESCOMMA)     ||
      (resTypes[8]  != RESDOUBLE)    ||
      (resTypes[9]  != RESCOMMA)     ||
      (resTypes[10] != RESDOUBLE)    ||
      (resTypes[11] != RESCOMMA)     ||
      (resTypes[12] != RESDOUBLE)    ||
      (resTypes[13] != RESPARENRIGHT))
    errorCode = BAD_RESPONSE_ITEMS;
  return ((errorCode != OK) ? NULL :
	  new GetCsyTransformationResponse(tag, resDoubles[2], resDoubles[4],
					   resDoubles[6], resDoubles[8],
					   resDoubles[10], resDoubles[12]));
}

/*******************************************************************/

/* parserRes::makeGetDMEVersionResponse

Returned Value: A GetDMEVersionResponse as described by the response
   text, or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. If the response items do not match the pattern
   DMEVersion(string)
   BAD_RESPONSE_ITEMS.

This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == DMEVersion), so those conditions are not checked here.

A GetDMEVersionResponse responds to a GetDMEVersion command.

Example 1. 00010 # DMEVersion("1.3.9")

Reference pages: 24 26 (46) [47] 99

*/

Response * parserRes::makeGetDMEVersionResponse()
{
  responseName = DataGetDMEVersion;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESSTRING)    ||
      (resTypes[3] != RESPARENRIGHT))
    errorCode = BAD_RESPONSE_ITEMS;
  return ((errorCode != OK) ? NULL :
	  new GetDMEVersionResponse(tag, resStrings[2]));
}

/*******************************************************************/

/* parserRes::makeGetErrStatusEResponse

Returned Value: A GetErrStatusResponse as described by the response
   text, or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. If the response items do not match the pattern
   ErrStatus(double)
   or the double does not consist of a single character:
   BAD_RESPONSE_ITEMS.
2. If the double is not 0 or 1: VALUE_MUST_BE_0_OR_1.

This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == ErrStatus), so those conditions are not checked here.

The check on inputArray[19] ensures there is only one character between
parentheses.

A GetErrStatusEResponse responds to a GetErrStatusE command.

Example 1. E0010 # ErrStatus(1)

Reference pages: 24 27 (50) 102

*/

Response * parserRes::makeGetErrStatusEResponse()
{
  responseName = DataGetErrStatusE;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESDOUBLE)    ||
      (resTypes[3] != RESPARENRIGHT)||
      (inputArray[19] != ')'))
    errorCode = BAD_RESPONSE_ITEMS;
  else if ((resDoubles[2] != 0) && (resDoubles[2] != 1))
    errorCode = VALUE_MUST_BE_0_OR_1;
  return ((errorCode != OK) ? NULL :
	  new GetErrStatusEResponse(tag, ((resDoubles[2]==1) ? true : false)));
}

/*******************************************************************/

/* parserRes::makeGetMachineClassResponse

Returned Value: A GetMachineClassResponse as described by the response
   text, or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. If the response items do not match the pattern
   MachineClass(string)
   BAD_RESPONSE_ITEMS.
2. If the string is not "CartCMM" or "CartCMMWithRotaryTable":
   BAD_MACHINE_CLASS.

This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == GetMachineKey), so those conditions are not checked here.

A GetMachineClassResponse responds to a GetMachineClass command.

Example 1. 00010 # GetMachineClass("CartCMM")

Reference pages: 18 24 27 (50) 102

*/

Response * parserRes::makeGetMachineClassResponse()
{
  responseName = DataGetMachineClass;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESSTRING)    ||
      (resTypes[3] != RESPARENRIGHT))
    errorCode = BAD_RESPONSE_ITEMS;
  else if ((strcmp(resStrings[2], "CartCMM") != 0) &&
	   (strcmp(resStrings[2], "CartCMMWithRotaryTable") != 0))
    errorCode = BAD_MACHINE_CLASS;
  return ((errorCode != OK) ? NULL :
	  new GetMachineClassResponse(tag, resStrings[2]));
}

/*******************************************************************/

/* parserRes::makeGetPropResponse

Returned Value: A GetPropResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. There must be at least one argument, and each argument must have at
   least six components and begin with the pattern:
   key1.key2 ...
   in which key1 is Tool or FoundTool.
   BAD_RESPONSE_ITEMS
2. If key1 is Tool, key2 must be Alignment, CollisionVolume, Name,
   Id, GoToPar, or PtMeasPar.
   If key1 is FoundTool, key2 must be Name, Id, GoToPar, or PtMeasPar.
   BAD_SECOND_KEYWORD

This is called when (resTypes[0] == RESKEYWORD) and
((resKeywords[0] == Tool) or (resKeywords[0] == FoundTool))
[except when resKeywords[2] is A, B, or C].

A GetPropResponse responds to a GetProp or GetPropE command.

Example 1. 00010 # Tool.GoToPar.Speed(30), Tool.GoToPar.Speed.Act(30)

Reference pages: 24 26 29 (45) 66 67 [68] [69] 70 [73] [74] 77 79 99 108

Notes:

oneProps1 may be larger than necessary, so it is copied to
oneProps2 for making the GetPropResponse.

Each of the makeOnePropertyXXX functions resets n to be the index of
the next item to be parsed. If there is no next item, the "for" loop
terminates. 

*/

Response * parserRes::makeGetPropResponse()
{
  int n; // index for all components of command arguments
  int m; // index for command arguments
  int j; // index for oneProps
  OneProperty ** oneProps1;
  OneProperty ** oneProps2;

  responseName = DataGetProp;
  m = 0;
  oneProps1 = new OneProperty * [(itemCount + 1) / 6];
  for (n = 0; ((n < itemCount) && (errorCode == OK)); )
    {
      if ((itemCount < (n + 5))            ||
	  (resTypes[n] != RESKEYWORD)      ||
	  ((resKeywords[n] != Tool) &&
	   (resKeywords[n] != FoundTool))  ||
	  (resTypes[n + 1] != RESDOT)      ||
	  (resTypes[n + 2] != RESKEYWORD))
	errorCode = BAD_RESPONSE_ITEMS;
      else if ((resKeywords[n + 2] == GoToPar) ||
	       (resKeywords[n + 2] == PtMeasPar))
	oneProps1[m] = makeOnePropertyNumber(&n);
      else if (resKeywords[n + 2] == Alignment)
	{
	  if (resKeywords[n] == FoundTool)
	    errorCode = BAD_SECOND_KEYWORD;
	  else
	    oneProps1[m] = makeOnePropertyAlignment(&n);
	}
      else if (resKeywords[n + 2] == CollisionVolume)
	{
	  if (resKeywords[n] == FoundTool)
	    errorCode = BAD_SECOND_KEYWORD;
	  else
	    oneProps1[m] = makeOnePropertyCollisionVolume(&n);
	}
      else if (resKeywords[n + 2] == Name)
	oneProps1[m] = makeOnePropertyName(&n);
      else if (resKeywords[n + 2] == Id)
	oneProps1[m] = makeOnePropertyId(&n);
      else
	errorCode = BAD_SECOND_KEYWORD;
      m++;
    }
  if (errorCode == OK)
    {
      oneProps2 = new OneProperty * [m];
      for (j = 0; j < m; j++)
	oneProps2[j] = oneProps1[j];
    }
  delete oneProps1;
  return ((errorCode != OK) ? NULL : 
	  new GetPropResponse(tag, tagType, m, oneProps2));
}

/*******************************************************************/

/* parserRes::makeGoToEResponse

Returned Value: A GoToEResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::makeServerInitiatedResponse

Errors:
1. The response does not have the pattern
   GoTo(<data items>)
   BAD_RESPONSE_ITEMS
2. There are not one to nine data items.
   BAD_RESPONSE_ITEMS
3. A data item is not one of: ER(double), IJK(double, double, double),
   R(double), X(double), Y(double), Z(double), Tool.A(double),
   Tool.B(double), or Tool.C(double).
   BAD_RESPONSE_ITEMS
4. An allowed data item appears more than once.
   ER_ARGUMENT_USED_TWICE
   IJK_ARGUMENT_USED_TWICE
   R_ARGUMENT_USED_TWICE
   X_ARGUMENT_USED_TWICE
   Y_ARGUMENT_USED_TWICE
   Z_ARGUMENT_USED_TWICE
   TOOL_A_ARGUMENT_USED_TWICE
   TOOL_B_ARGUMENT_USED_TWICE
   TOOL_C_ARGUMENT_USED_TWICE
5. IJK is used but the not all three doubles are zero.
   IJK_VALUES_MUST_BE_ZERO
6. Commas do not separate the response items if there are two or more.
   BAD_RESPONSE_ITEMS

This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == GoToKey), so those conditions are not checked here.

A GoToE response is server-initiated and is not a response to a command.

Example 1. E0000 # GoTo(X(15), Z(-0.09))

server-initiated GoTo Reference pages: (78)
other GoTo Ref. pages : 24 27 [38] [43] (51 - 53) (58) (62) [71] [72] (98) 102
OnPtMeas Report Ref. pages: 24 27 (41) (49) (52) (53) 61 [71] [72] (78) 102

Notes:

The spec provides that a server-initiated GoTo response should use the
same format inside the GoTo(..) as a response to an OnPtMeasReport command,
so that is done here. The body of this function is very similar to the body
of makePtMeasOrGetResponse.

*/

Response * parserRes::makeGoToEResponse()
{
  int n; // index for components of response items
  bool hasER = false;
  bool hasIJK = false;
  bool hasR = false;
  bool hasToolA = false;
  bool hasToolB = false;
  bool hasToolC = false;
  bool hasX = false;
  bool hasY = false;
  bool hasZ = false;
  double ERvalue = 0;
  double Rvalue = 0;
  double toolAvalue = 0;
  double toolBvalue = 0;
  double toolCvalue = 0;
  double Xvalue = 0;
  double Yvalue = 0;
  double Zvalue = 0;

  responseName = DataGoToE;
  if (resTypes[1] != RESPARENLEFT)
    errorCode = BAD_RESPONSE_ITEMS;
  for (n = 2; ((n < itemCount) && (errorCode == OK)); )
    {
      if ((itemCount < (n + 5)) ||
	  (resTypes[n] != RESKEYWORD))
	errorCode = BAD_RESPONSE_ITEMS;
      else if ((resKeywords[n] == ER) ||
	       (resKeywords[n] == R)  ||
	       (resKeywords[n] == X)  ||
	       (resKeywords[n] == Y)  ||
	       (resKeywords[n] == Z))
	{
	  if ((resTypes[n + 1] != RESPARENLEFT)  ||
              (resTypes[n + 2] != RESDOUBLE)     ||
	      (resTypes[n + 3] != RESPARENRIGHT) ||
	      ((itemCount == (n + 5)) && (resTypes[n + 4] != RESPARENRIGHT)) ||
	      ((itemCount  > (n + 5)) && (resTypes[n + 4] != RESCOMMA)))
	    errorCode = BAD_RESPONSE_ITEMS;
	  else if (resKeywords[n] == ER)
	    {
	      if (hasER)
		errorCode = ER_USED_TWICE;
	      else
		{
		  hasER = true;
		  ERvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == R)
	    {
	      if (hasR)
		errorCode = R_USED_TWICE;
	      else
		{
		  hasR = true;
		  Rvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == X)
	    {
	      if (hasX)
		errorCode = X_USED_TWICE;
	      else
		{
		  hasX = true;
		  Xvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == Y)
	    {
	      if (hasY)
		errorCode = Y_USED_TWICE;
	      else
		{
		  hasY = true;
		  Yvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == Z)
	    {
	      if (hasZ)
		errorCode = Z_USED_TWICE;
	      else
		{
		  hasZ = true;
		  Zvalue = resDoubles[n + 2];
		}
	    }
	  n = (n + 5);
	}
      else if (resKeywords[n] == Tool)
	{
	  if ((itemCount < (n + 7)) ||
	      (resTypes[n + 1] != RESDOT) ||
	      (resTypes[n + 2] != RESKEYWORD) ||
	      (resTypes[n + 3] != RESPARENLEFT) ||
	      (resTypes[n + 4] != RESDOUBLE) ||
	      (resTypes[n + 5] != RESPARENRIGHT) ||
	      ((itemCount == (n + 7)) && (resTypes[n + 6] != RESPARENRIGHT)) ||
	      ((itemCount  > (n + 7)) && (resTypes[n + 6] != RESCOMMA)))
	    errorCode = BAD_RESPONSE_ITEMS;
	  else if (resKeywords[n + 2] == A)
	    {
	      if (hasToolA)
		errorCode = TOOL_A_USED_TWICE;
	      else
		{
		  hasToolA = true;
		  toolAvalue = resDoubles[n + 4];
		}
	    }
	  else if (resKeywords[n + 2] == B)
	    {
	      if (hasToolB)
		errorCode = TOOL_B_USED_TWICE;
	      else
		{
		  hasToolB = true;
		  toolBvalue = resDoubles[n + 4];
		}
	    }
	  else if (resKeywords[n + 2] == C)
	    {
	      if (hasToolC)
		errorCode = TOOL_C_USED_TWICE;
	      else
		{
		  hasToolC = true;
		  toolCvalue = resDoubles[n + 4];
		}
	    }
	  else
	    errorCode = BAD_RESPONSE_ITEMS;
	  n = (n + 7);
	}
      else if (resKeywords[n] == IJK)
	{
	  if ((itemCount < (n + 9)) ||
	      (resTypes[n + 1] != RESPARENLEFT)  ||
	      (resTypes[n + 2] != RESDOUBLE)     ||
	      (resTypes[n + 3] != RESCOMMA)      ||
	      (resTypes[n + 4] != RESDOUBLE)     ||
	      (resTypes[n + 5] != RESCOMMA)      ||
	      (resTypes[n + 6] != RESDOUBLE)     ||
	      (resTypes[n + 7] != RESPARENRIGHT) ||
	      ((itemCount == (n + 9)) && (resTypes[n + 8] != RESPARENRIGHT)) ||
	      ((itemCount  > (n + 9)) && (resTypes[n + 8] != RESCOMMA)))
	    errorCode = BAD_RESPONSE_ITEMS;
	  else if (hasIJK)
	    errorCode = IJK_USED_TWICE;
	  else if ((resDoubles[n + 2] != 0) ||
		   (resDoubles[n + 4] != 0) ||
		   (resDoubles[n + 6] != 0))
	    errorCode = IJK_VALUES_MUST_BE_ZERO;
	  else
	    {
	      hasIJK = true;
	      n = (n + 9);
	    }
	}
      else
	errorCode = BAD_RESPONSE_ITEMS;
    }
  return ((errorCode != OK) ? NULL:
	  new GoToEResponse(hasER, hasIJK, hasR, hasToolA, hasToolB, hasToolC,
			    hasX, hasY, hasZ, ERvalue, Rvalue, toolAvalue,
			    toolBvalue, toolCvalue, Xvalue, Yvalue, Zvalue));
}

/*******************************************************************/

/* parserRes::makeIsHomedResponse

Returned Value: An IsHomedResponse as described by the response
   text, or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. If the response items do not match the pattern
   IsHomed(double)
   or the double is not a single character: BAD_RESPONSE_ITEMS.
2. If the double is not 0 or 1: VALUE_MUST_BE_0_OR_1.

This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == IsHomedKey), so those conditions are not checked here.

An IsHomedResponse responds to an IsHomed command.

Example 1. 00010 # IsHomed(1)

Reference pages: 24 27 (48) 102 111

Notes:

The check on inputArray[17] ensures there is only one character between
parentheses.

*/

Response * parserRes::makeIsHomedResponse()
{
  responseName = DataIsHomed;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESDOUBLE)    ||
      (resTypes[3] != RESPARENRIGHT)||
      (inputArray[17] != ')'))
    errorCode = BAD_RESPONSE_ITEMS;
  else if ((resDoubles[2] != 0) && (resDoubles[2] != 1))
    errorCode = VALUE_MUST_BE_0_OR_1;
  return ((errorCode != OK) ? NULL :
	  new IsHomedResponse(tag, ((resDoubles[2] == 1) ? true : false)));
}

/*******************************************************************/

/* parserRes::makeIsUserEnabledResponse

Returned Value: An IsUserEnabledResponse as described by the response
   text, or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. If the response items do not match the pattern
   IsUserEnabled(double)
   or the double is not a single character: BAD_RESPONSE_ITEMS.
2. If the double is not 0 or 1: VALUE_MUST_BE_0_OR_1.

This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == IsUserEnabledKey), so those conditions are not checked
here.

Example 1. 00010 # IsUserEnabled(1)

Reference pages: 24 27 (49) 102

Notes:

The check on inputArray[23] ensures there is only one character between
parentheses.

*/

Response * parserRes::makeIsUserEnabledResponse()
{
  responseName = DataIsUserEnabled;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESDOUBLE)    ||
      (resTypes[3] != RESPARENRIGHT)||
      (inputArray[23] != ')'))
    errorCode = BAD_RESPONSE_ITEMS;
  else if ((resDoubles[2] != 0) && (resDoubles[2] != 1))
    errorCode = VALUE_MUST_BE_0_OR_1;
  return ((errorCode != OK) ? NULL :
	  new IsUserEnabledResponse(tag, ((resDoubles[2]==1) ? true : false)));
}

/*******************************************************************/

/* parserRes::makeKeyPressEResponse

Returned Value: A KeyPressEResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::makeServerInitiatedResponse

Errors:
1. The response does not have the pattern
   KeyPress(string)
   BAD_RESPONSE_ITEMS

This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == KeyPress), so those conditions are not checked here.

A KeyPressEResponse is server-initiated and is not a response to a command.

Example 1. E0000 # KeyPress("F1")

Reference pages: (78-79)

*/

Response * parserRes::makeKeyPressEResponse()
{
  responseName = DataKeyPressE;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESSTRING)    ||
      (resTypes[3] != RESPARENRIGHT))
    errorCode = BAD_RESPONSE_ITEMS;
  return ((errorCode != OK) ? NULL :
	  new KeyPressEResponse(resStrings[2]));
}

/*******************************************************************/

/* parserRes::makeOnePropertyAlignment

Returned Value: A OnePropertyAlignment as described by the response
  items, or NULL if the response items are bad.

Called By:  parserRes::makeGetPropResponse

Errors:
  1. The argument does not have one of the two patterns
     Tool.Alignment(double, double, double)
     Tool.Alignment(double, double, double, double, double, double)
     BAD_RESPONSE_ITEMS
  2. There are more response items after this argument, but the next
     response item is not a comma.
     BAD_RESPONSE_ITEMS

This is called by makeGetPropResponse when an argument starts with
Tool.Alignment, so the first three response items are not checked here.

Example 1. Tool.Alignment(0.0, 0.6, 0.8)

Reference pages: 24 29 (70) 107

Notes:

This resets *n to (*n + 11) or (*n + 17), since, including a comma,
this function handles 11 or 17 response items. These settings are OK
when the end of the response items has been reached and there is no
comma, since the loop in makeGetPropResponse will stop in that case.

It is known that there are at least five more response items to be parsed
when this is called, the first of which (Tool or FoundTool) has index *n.

*/

OnePropertyAlignment * parserRes::makeOnePropertyAlignment(
  int * n)
{
  int m;

  m = *n;
  if ((itemCount < (m + 10))            ||
      (resTypes[m + 3] != RESPARENLEFT) ||
      (resTypes[m + 4] != RESDOUBLE)    ||
      (resTypes[m + 5] != RESCOMMA)     ||
      (resTypes[m + 6] != RESDOUBLE)    ||
      (resTypes[m + 7] != RESCOMMA)     ||
      (resTypes[m + 8] != RESDOUBLE))
    errorCode = BAD_RESPONSE_ITEMS;
  else if ((itemCount == (m + 10)) ||
	   (itemCount > (m + 10)) && (resTypes[m + 10] == RESCOMMA))
    {
      *n = (m + 11);
      if (resTypes[m + 9] != RESPARENRIGHT)
	errorCode = BAD_RESPONSE_ITEMS;
    }
  else if ((itemCount == (m + 16)) ||
	   (itemCount > (m + 16)) && (resTypes[m + 16] == RESCOMMA))
    {
      *n = (m + 17);
      if ((resTypes[m +  9] != RESCOMMA)      ||
	  (resTypes[m + 10] != RESDOUBLE)     ||
	  (resTypes[m + 11] != RESCOMMA)      ||
	  (resTypes[m + 12] != RESDOUBLE)     ||
	  (resTypes[m + 13] != RESCOMMA)      ||
	  (resTypes[m + 14] != RESDOUBLE)     ||
	  (resTypes[m + 15] != RESPARENRIGHT))
	errorCode = BAD_RESPONSE_ITEMS;
    }
  else
    errorCode = BAD_RESPONSE_ITEMS;
  return ((errorCode != OK) ? NULL : 
	  (*n == (m + 11)) ?
	  new OnePropertyAlignment(resDoubles[m + 4], resDoubles[m + 6],
				   resDoubles[m + 8], 0, 0, 0, false) :
	  new OnePropertyAlignment(resDoubles[m + 4], resDoubles[m + 6],
				   resDoubles[m + 8], resDoubles[m + 10],
				   resDoubles[m + 12], resDoubles[m + 14],
				   true));
}

/*******************************************************************/

/* parserRes::makeOnePropertyCollisionVolume

Returned Value: A OnePropertyCollisionVolume as described by the response
  items, or NULL if the response items are bad.

Called By:  parserRes::makeGetPropResponse

Errors:
  1. The argument being parsed does not have the pattern
     Tool.CollisionVolume(OBB,double<15 times>, ... OBB,double<15 times>)
     or the pattern Tool.CollisionVolume(), indicating no boxes
     BAD_RESPONSE_ITEMS
  2. There are more response items after this argument, but the next
     response item is not a comma.
     BAD_RESPONSE_ITEMS

This is called by makeGetPropResponse when an argument starts with
Tool.CollisionVolume, so the first three response items are not checked here.

Example 1. Tool.CollisionVolume(OBB,150,10,530,50,50,50,1,0,0,0,1,0,0,0,1),

Example 2. Tool.CollisionVolume(),

Reference pages: 24 29 (67-70) [68] [69] 107

Notes:

This resets *n to (*n + 6) if there are no boxes and to (*n + 5 + (m *
32)) where m > 0 is the number of boxes in the CollisionVolume, since,
including a comma at the end, this function handles 6 or (5 + (m * 32)
response items. This setting is OK when the end of the response items
has been reached and there is no comma, since the loop in
makeGetPropResponse will stop in that case.

It is known that there are at least five more response items to be
parsed when this is called, the first of which (Tool) has index *n.

*/

OnePropertyCollisionVolume * parserRes::makeOnePropertyCollisionVolume(
 int * n)
{
  int k;       // index for response items
  int m;       // number of boxes
  int j;       // utility counter
  Box * boxes; // array of boxes
  
  k = (*n + 3);
  if (resTypes[k] != RESPARENLEFT)
    errorCode = BAD_RESPONSE_ITEMS;
  else if (resTypes[k + 1] == RESPARENRIGHT)
    {
      *n = (*n + 6);
      m = 0;
      boxes = NULL;
      if ((itemCount != (k + 2)) && (resTypes[k + 2] != RESCOMMA))
	errorCode = BAD_RESPONSE_ITEMS;
    }
  else
    {
      for (m = 1; (errorCode == OK); m++)
	{
	  if ((itemCount < (k + 32)) ||
	      (resTypes[k + 1] != RESKEYWORD) ||
	      (resKeywords[k + 1] != OBB))
	    errorCode = BAD_RESPONSE_ITEMS;
	  if (errorCode == OK)
	    {
	      k = (k + 2);
	      for (j = 0; j < 30; j = (j + 2))
		{
		  if ((resTypes[k + j] != RESCOMMA) ||
		      (resTypes[k + j + 1] != RESDOUBLE))
		    errorCode = BAD_RESPONSE_ITEMS;
		}
	    }
	 if (errorCode == OK)
	   { 
	     if (resTypes[k + 30] == RESPARENRIGHT)
	       break;
	     else if (resTypes[k + 30] != RESCOMMA)
	       errorCode = BAD_RESPONSE_ITEMS;
	     else
	       k = (k + 30);
	   }
	}
      if (errorCode == OK)
	{
	  if ((itemCount != (k + 31)) && (resTypes[k + 31] != RESCOMMA))
	    errorCode = BAD_RESPONSE_ITEMS;
	}
      if (errorCode == OK)
	{
	  k = (*n + 6);
	  boxes = new Box[m];
	  for (j = 0; j < m; j++)
	    {
	      boxes[j].Cx = resDoubles[k + 0];
	      boxes[j].Cy = resDoubles[k + 2];
	      boxes[j].Cz = resDoubles[k + 4];
	      boxes[j].Ex = resDoubles[k + 6];
	      boxes[j].Ey = resDoubles[k + 8];
	      boxes[j].Ez = resDoubles[k + 10];
	      boxes[j].Ix = resDoubles[k + 12];
	      boxes[j].Iy = resDoubles[k + 14];
	      boxes[j].Iz = resDoubles[k + 16];
	      boxes[j].Jx = resDoubles[k + 18];
	      boxes[j].Jy = resDoubles[k + 20];
	      boxes[j].Jz = resDoubles[k + 22];
	      boxes[j].Kx = resDoubles[k + 24];
	      boxes[j].Ky = resDoubles[k + 26];
	      boxes[j].Kz = resDoubles[k + 28];
	      k = (k + 32);
	    }
	}
      *n = (*n + 5 + (m * 32));
    }
  return ((errorCode != OK) ? NULL :
	  new OnePropertyCollisionVolume(m, boxes));
}

/*******************************************************************/

/* parserRes::makeOnePropertyId

Returned Value: A OnePropertyId as described by the response
  items, or NULL if the response items are bad.

Called By:  parserRes::makeGetPropResponse

Errors:
  1. The argument being parsed does not have either pattern
     Tool.Id(string) or FoundTool.Id(string)
     BAD_RESPONSE_ITEMS
  2. There are more response items after this argument, but the next
     response item is not a comma.
     BAD_RESPONSE_ITEMS


This is called by makeGetPropResponse when an argument starts with Tool.Id
or FoundTool.Id, so the first three response items are not checked here.

Example 1. Tool.Id("123")

Reference pages: 24 29 (66) 107

Notes:

This resets *n to (*n + 7), since, including a comma, this function
handles seven response items. This setting is OK when the end of the
response items has been reached and there is no comma, since the loop
in makeGetPropResponse will stop in that case.

It is known that there are at least five more response items to be
parsed when this is called, the first of which (Tool or FoundTool)
has index *n.

*/

OnePropertyId * parserRes::makeOnePropertyId(
 int * n)
{
  int m;
  m = *n;
  if ((itemCount < (m + 6))              ||
      (resTypes[m + 3] != RESPARENLEFT)  ||
      (resTypes[m + 4] != RESSTRING)     ||
      (resTypes[m + 5] != RESPARENRIGHT) ||
      ((itemCount != (m + 6)) && (resTypes[m + 6] != RESCOMMA)))
    errorCode = BAD_RESPONSE_ITEMS;
  *n = (m + 7);
  return ((errorCode != OK) ? NULL :
	  new OnePropertyId((resKeywords[m] == FoundTool), resStrings[m + 4]));
}

/*******************************************************************/

/* parserRes::makeOnePropertyName

Returned Value: A OnePropertyName as described by the response
  items, or NULL if the response items are bad.

Called By:  parserRes::makeGetPropResponse

Errors:
  1. The argument being parsed does not have either pattern
     Tool.Name(string) or FoundTool.Name(string)
     BAD_RESPONSE_ITEMS
  2. There are more response items after this argument, but the next
     response item is not a comma.
     BAD_RESPONSE_ITEMS

This is called by makeGetPropResponse when an argument starts with Tool.Name
or FoundTool.Name, so the first three response items are not checked here.

Example 1. Tool.Name("probe1")

Reference pages: 24 29 (66) 107

Notes:

This resets *n to (*n + 7), since, including a comma, this function
handles seven response items. This setting is OK when the end of the
response items has been reached and there is no comma, since the loop
in makeGetPropResponse will stop in that case.

It is known that there are at least six more response items to be
parsed when this is called, the first of which (Tool or FoundTool)
has index *n.

*/

OnePropertyName * parserRes::makeOnePropertyName(
 int * n)
{
  int m;
  m = *n;
  if ((itemCount < (m + 6))              ||
      (resTypes[m + 3] != RESPARENLEFT)  ||
      (resTypes[m + 4] != RESSTRING)     ||
      (resTypes[m + 5] != RESPARENRIGHT) ||
      ((itemCount != (m + 6)) && (resTypes[m + 6] != RESCOMMA)))
    errorCode = BAD_RESPONSE_ITEMS;
  *n = (m + 7);
  return ((errorCode != OK) ? NULL :
	  new OnePropertyName((resKeywords[m] == FoundTool), resStrings[m+4]));
}

/*******************************************************************/

/* parserRes::makeOnePropertyNumber

Returned Value: A OnePropertyNumber as described by the response
  items, or NULL if the response items are bad.

Called By:  parserRes::makeGetPropResponse

Errors:
  1. The argument does not match one of the patterns
     [Found]Tool.PtMeasPar.{PtMeasParItem}(value)
     [Found]Tool.PtMeasPar.{PtMeasParItem}.{ParDetail}(value)
     [Found]Tool.PtMeasPar.{GoToParItem}(value)
     [Found]Tool.PtMeasPar.{GoToParItem}.{ParDetail}(value)
     where {ParDetail} is one of Act, Def, Max, or Min
     BAD_RESPONSE_ITEMS
  2. {GoToParItem} is not Speed or Accel.
     BAD_GOTOPAR_PROPERTY
  3. {PtMeasParItem} is not one of Speed, Accel, Approach, Search, or Retract.
     BAD_PTMEASPAR_PROPERTY
  4. There are more response items after this argument, but the next
     response item is not a comma.
     BAD_RESPONSE_ITEMS

This is called by makeGetPropResponse when an argument starts with one of
  Tool.GoToPar
  Tool.PtMeasPar
  FoundToolTool.GoToPar
  FoundToolTool.PtMeasPar
so the first three response items are not checked here.

Example 1. Tool.GoToPar.Accel(809.0)
Example 2. Tool.GoToPar.Accel.Act(809.0)

Reference pages: See makeGetPropResponse

Notes:

This resets *n to (*n + 9) or (*n + 11), since, including a comma,
this function handles 9 or 11 response items. These settings are OK
when the end of the response items has been reached and there is no
comma, since the loop in makeGetPropResponse will stop in that case.

It is known that there are at least five more response items to be parsed
when this is called, the first of which (Tool or FoundTool) has index *n.

*/

OnePropertyNumber * parserRes::makeOnePropertyNumber(
 int * n)
{
  int m;
  m = *n;
  if (resTypes[m + 3] != RESDOT)
    errorCode = BAD_RESPONSE_ITEMS;
  else if ((itemCount == (m + 10)) ||
	   ((itemCount > (m + 10)) && (resTypes[m + 10] == RESCOMMA)))
    {
      *n = (m + 11);
      if ((resTypes[m + 5] != RESDOT)       ||
	  (resTypes[m + 6] != RESKEYWORD)   ||
	  ((resKeywords[m + 6] != Actual)  &&
	   (resKeywords[m + 6] != Default) &&
	   (resKeywords[m + 6] != Max)     &&
	   (resKeywords[m + 6] != Min))     ||
	  (resTypes[m + 7] != RESPARENLEFT) ||
	  (resTypes[m + 8] != RESDOUBLE)    ||
	  (resTypes[m + 9] != RESPARENRIGHT))
	errorCode = BAD_RESPONSE_ITEMS;
    }
  else if ((itemCount == (m + 8)) ||
	   ((itemCount > (m + 8)) && (resTypes[m + 8] == RESCOMMA)))
    {
      *n = (m + 9);
      if ((resTypes[m + 5] != RESPARENLEFT) ||
	  (resTypes[m + 6] != RESDOUBLE)    ||
	  (resTypes[m + 7] != RESPARENRIGHT))
	errorCode = BAD_RESPONSE_ITEMS;
    }
  else
    errorCode = BAD_RESPONSE_ITEMS;
  if (errorCode == OK)
    {
      if (resKeywords[m + 2] == GoToPar)
	{
	  if ((resTypes[m + 4] != RESKEYWORD) ||
	      ((resKeywords[m + 4] != Speed) &&
	       (resKeywords[m + 4] != Accel)))
	    errorCode = BAD_GOTOPAR_PROPERTY;
	}
      else if (resKeywords[m + 2] == PtMeasPar)
	{
	  if ((resTypes[m + 4] != RESKEYWORD) ||
	      ((resKeywords[m + 4] != Speed)    &&
	       (resKeywords[m + 4] != Accel)    &&
	       (resKeywords[m + 4] != Approach) &&
	       (resKeywords[m + 4] != Retract)  &&
	       (resKeywords[m + 4] != Search)))
	    errorCode = BAD_PTMEASPAR_PROPERTY;
	}
      else
	errorCode = BUG; // should never happen
    }
  return ((errorCode != OK) ? NULL :
	  (*n == (m + 9)) ?
	  new OnePropertyNumber((otherKeyType)resKeywords[m],
				(otherKeyType)resKeywords[m + 2],
				(otherKeyType)resKeywords[m + 4],
				resDoubles[m + 6]) :
	  new OnePropertyNumber((otherKeyType)resKeywords[m],
				(otherKeyType)resKeywords[m + 2],
				(otherKeyType)resKeywords[m + 4],
				(otherKeyType)resKeywords[m + 6],
				resDoubles[m + 8]));
}

/*******************************************************************/

/* parserRes::makePtMeasEResponse

Returned Value: A PtMeasOrGetResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. The response items do not have the pattern
   PtMeas(<data items>)
   BAD_RESPONSE_ITEMS
2. There are not one to nine data items.
   BAD_RESPONSE_ITEMS
3. A data item is not one of: ER(double), IJK(double, double, double),
   R(double), X(double), Y(double), Z(double), Tool.A(double),
   Tool.B(double), or Tool.C(double).
   BAD_RESPONSE_ITEMS
4. An allowed data item appears more than once.
   ER_USED_TWICE
   IJK_USED_TWICE
   R_USED_TWICE
   X_USED_TWICE
   Y_USED_TWICE
   Z_USED_TWICE
   TOOL_A_USED_TWICE
   TOOL_B_USED_TWICE
   TOOL_C_USED_TWICE
5. Commas do not separate the response items if there are two or more.
   BAD_RESPONSE_ITEM

This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == PtMeasKey), so those conditions are not checked here.

A PtMeasEResponse is server-initiated and is not a response to a command.

Example 1. E0000 # PtMeas(X(15), Z(-0.09))

server-initiated PtMeas Reference pages: (78)
other PtMeas Reference pages: 9 24 27 (41) (49) (52 - 53) (62) [71] [72]
                              83 84 86 87 89 90 91 102

*/

Response * parserRes::makePtMeasEResponse()
{
  int n; // index for components of response items
  bool hasER = false;
  bool hasIJK = false;
  bool hasR = false;
  bool hasToolA = false;
  bool hasToolB = false;
  bool hasToolC = false;
  bool hasX = false;
  bool hasY = false;
  bool hasZ = false;
  double ERvalue = 0;
  double Ivalue = 0;
  double Jvalue = 0;
  double Kvalue = 0;
  double Rvalue = 0;
  double toolAvalue = 0;
  double toolBvalue = 0;
  double toolCvalue = 0;
  double Xvalue = 0;
  double Yvalue = 0;
  double Zvalue = 0;

  responseName = DataPtMeasE;
  if (resTypes[1] != RESPARENLEFT)
    errorCode = BAD_RESPONSE_ITEMS;
  for (n = 2; ((n < itemCount) && (errorCode == OK)); )
    {
      if ((itemCount < (n + 5)) ||
	  (resTypes[n] != RESKEYWORD))
	errorCode = BAD_RESPONSE_ITEMS;
      else if ((resKeywords[n] == ER) ||
	       (resKeywords[n] == R)  ||
	       (resKeywords[n] == X)  ||
	       (resKeywords[n] == Y)  ||
	       (resKeywords[n] == Z))
	{
	  if ((resTypes[n + 1] != RESPARENLEFT)  ||
              (resTypes[n + 2] != RESDOUBLE)     ||
	      (resTypes[n + 3] != RESPARENRIGHT) ||
	      ((itemCount == (n + 5)) && (resTypes[n + 4] != RESPARENRIGHT)) ||
	      ((itemCount  > (n + 5)) && (resTypes[n + 4] != RESCOMMA)))
	    errorCode = BAD_RESPONSE_ITEMS;
	  else if (resKeywords[n] == ER)
	    {
	      if (hasER)
		errorCode = ER_USED_TWICE;
	      else
		{
		  hasER = true;
		  ERvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == R)
	    {
	      if (hasR)
		errorCode = R_USED_TWICE;
	      else
		{
		  hasR = true;
		  Rvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == X)
	    {
	      if (hasX)
		errorCode = X_USED_TWICE;
	      else
		{
		  hasX = true;
		  Xvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == Y)
	    {
	      if (hasY)
		errorCode = Y_USED_TWICE;
	      else
		{
		  hasY = true;
		  Yvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == Z)
	    {
	      if (hasZ)
		errorCode = Z_USED_TWICE;
	      else
		{
		  hasZ = true;
		  Zvalue = resDoubles[n + 2];
		}
	    }
	  n = (n + 5);
	}
      else if (resKeywords[n] == Tool)
	{
	  if ((itemCount < (n + 7)) ||
	      (resTypes[n + 1] != RESDOT) ||
	      (resTypes[n + 2] != RESKEYWORD) ||
	      (resTypes[n + 3] != RESPARENLEFT) ||
	      (resTypes[n + 4] != RESDOUBLE) ||
	      (resTypes[n + 5] != RESPARENRIGHT) ||
	      ((itemCount == (n + 7)) && (resTypes[n + 6] != RESPARENRIGHT)) ||
	      ((itemCount  > (n + 7)) && (resTypes[n + 6] != RESCOMMA)))
	    errorCode = BAD_RESPONSE_ITEMS;
	  else if (resKeywords[n + 2] == A)
	    {
	      if (hasToolA)
		errorCode = TOOL_A_USED_TWICE;
	      else
		{
		  hasToolA = true;
		  toolAvalue = resDoubles[n + 4];
		}
	    }
	  else if (resKeywords[n + 2] == B)
	    {
	      if (hasToolB)
		errorCode = TOOL_B_USED_TWICE;
	      else
		{
		  hasToolB = true;
		  toolBvalue = resDoubles[n + 4];
		}
	    }
	  else if (resKeywords[n + 2] == C)
	    {
	      if (hasToolC)
		errorCode = TOOL_C_USED_TWICE;
	      else
		{
		  hasToolC = true;
		  toolCvalue = resDoubles[n + 4];
		}
	    }
	  else
	    errorCode = BAD_RESPONSE_ITEMS;
	  n = (n + 7);
	}
      else if (resKeywords[n] == IJK)
	{
	  if ((itemCount < (n + 9)) ||
	      (resTypes[n + 1] != RESPARENLEFT)  ||
	      (resTypes[n + 2] != RESDOUBLE)     ||
	      (resTypes[n + 3] != RESCOMMA)      ||
	      (resTypes[n + 4] != RESDOUBLE)     ||
	      (resTypes[n + 5] != RESCOMMA)      ||
	      (resTypes[n + 6] != RESDOUBLE)     ||
	      (resTypes[n + 7] != RESPARENRIGHT) ||
	      ((itemCount == (n + 9)) && (resTypes[n + 8] != RESPARENRIGHT)) ||
	      ((itemCount  > (n + 9)) && (resTypes[n + 8] != RESCOMMA)))
	    errorCode = BAD_RESPONSE_ITEMS;
	  else if (hasIJK)
	    errorCode = IJK_USED_TWICE;
	  else
	    {
	      hasIJK = true;
	      Ivalue = resDoubles[n + 2];
	      Jvalue = resDoubles[n + 4];
	      Kvalue = resDoubles[n + 6];
	      n = (n + 9);
	    }
	}
      else
	errorCode = BAD_RESPONSE_ITEMS;
    }
  return ((errorCode != OK) ? NULL :
	  new PtMeasEResponse(hasER, hasIJK, hasR, hasToolA, hasToolB,
			      hasToolC, hasX, hasY, hasZ, ERvalue, Ivalue,
			      Jvalue, Kvalue, Rvalue, toolAvalue, toolBvalue,
			      toolCvalue, Xvalue, Yvalue, Zvalue));
}

/*******************************************************************/

/* parserRes::makePtMeasOrGetResponse

Returned Value: A PtMeasOrGetResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. There must be one to nine data items.
   BAD_RESPONSE_ITEMS
2. A data item is not one of: ER(double), IJK(double, double, double),
   R(double), X(double), Y(double), Z(double), Tool.A(double),
   Tool.B(double), or Tool.C(double).
   BAD_RESPONSE_ITEMS
3. An allowed data item appears more than once.
   ER_USED_TWICE
   IJK_USED_TWICE
   R_USED_TWICE
   X_USED_TWICE
   Y_USED_TWICE
   Z_USED_TWICE
   TOOL_A_USED_TWICE
   TOOL_B_USED_TWICE
   TOOL_C_USED_TWICE
4. Commas do not separate the response items if there are two or more.
   BAD_RESPONSE_ITEMS

This is called when (resTypes[0] == RESKEYWORD) and the keyword is ER, IJK, R,
X, Y, Z, or [the keyword is Tool and the keyword after that is A, B, or C].
Other responses where (resTypes[0] == RESKEYWORD) and the keyword is Tool
are handled by makeGetPropResponse.

A PtMeasOrGetResponse responds to a PtMeas command or a Get command.

Example 1. X(3), Y(4)

PtMeas Reference pages: 9 24 27 (41) (49) (52 - 53) (62) [71] [72] (78)
                        83 84 86 87 89 90 91 102
Get Reference pages: 24 27 [38] [44] (49) (51) 57 (61) (63) (66) (81) 102

Note: A PtMeasOrGetResponse responding to a Get command should never
have ER or IJK. When the response does not have ER or IJK, it is not
possible to tell solely by examining the response whether the command
was a PtMeas or a Get. The caller must keep a record of commands and
check the tag to determine what the command was.

Which items should be included in a PtMeasOrGetResponse made in
response to a PtMeas command is determined by the most recent
OnPtMeasReport command. This function does not try to determine if
the appropriate items have been reported.

*/

Response * parserRes::makePtMeasOrGetResponse()
{
  int n; // index for components of response items
  bool hasER = false;
  bool hasIJK = false;
  bool hasR = false;
  bool hasToolA = false;
  bool hasToolB = false;
  bool hasToolC = false;
  bool hasX = false;
  bool hasY = false;
  bool hasZ = false;
  double ERvalue = 0;
  double Ivalue = 0;
  double Jvalue = 0;
  double Kvalue = 0;
  double Rvalue = 0;
  double toolAvalue = 0;
  double toolBvalue = 0;
  double toolCvalue = 0;
  double Xvalue = 0;
  double Yvalue = 0;
  double Zvalue = 0;

  responseName = DataPtMeasOrGet;
  for (n = 0; ((n < itemCount) && (errorCode == OK)); )
    {
      if ((itemCount < (n + 4)) ||
	  (resTypes[n] != RESKEYWORD))
	errorCode = BAD_RESPONSE_ITEMS;
      else if ((resKeywords[n] == ER) ||
	       (resKeywords[n] == R)  ||
	       (resKeywords[n] == X)  ||
	       (resKeywords[n] == Y)  ||
	       (resKeywords[n] == Z))
	{
	  if ((resTypes[n + 1] != RESPARENLEFT) ||
              (resTypes[n + 2] != RESDOUBLE) ||
	      (resTypes[n + 3] != RESPARENRIGHT) ||
	      ((itemCount != (n + 4)) && (resTypes[n + 4] != RESCOMMA)))
	    errorCode = BAD_RESPONSE_ITEMS;
	  else if (resKeywords[n] == ER)
	    {
	      if (hasER)
		errorCode = ER_USED_TWICE;
	      else
		{
		  hasER = true;
		  ERvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == R)
	    {
	      if (hasR)
		errorCode = R_USED_TWICE;
	      else
		{
		  hasR = true;
		  Rvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == X)
	    {
	      if (hasX)
		errorCode = X_USED_TWICE;
	      else
		{
		  hasX = true;
		  Xvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == Y)
	    {
	      if (hasY)
		errorCode = Y_USED_TWICE;
	      else
		{
		  hasY = true;
		  Yvalue = resDoubles[n + 2];
		}
	    }
	  else if (resKeywords[n] == Z)
	    {
	      if (hasZ)
		errorCode = Z_USED_TWICE;
	      else
		{
		  hasZ = true;
		  Zvalue = resDoubles[n + 2];
		}
	    }
	  n = (n + 5);
	}
      else if (resKeywords[n] == Tool)
	{
	  if ((itemCount < (n + 6)) ||
	      (resTypes[n + 1] != RESDOT) ||
	      (resTypes[n + 2] != RESKEYWORD) ||
	      (resTypes[n + 3] != RESPARENLEFT) ||
	      (resTypes[n + 4] != RESDOUBLE) ||
	      (resTypes[n + 5] != RESPARENRIGHT) ||
	      ((itemCount != (n + 6)) && (resTypes[n + 6] != RESCOMMA)))
	    errorCode = BAD_RESPONSE_ITEMS;
	  else if (resKeywords[n + 2] == A)
	    {
	      if (hasToolA)
		errorCode = TOOL_A_USED_TWICE;
	      else
		{
		  hasToolA = true;
		  toolAvalue = resDoubles[n + 4];
		}
	    }
	  else if (resKeywords[n + 2] == B)
	    {
	      if (hasToolB)
		errorCode = TOOL_B_USED_TWICE;
	      else
		{
		  hasToolB = true;
		  toolBvalue = resDoubles[n + 4];
		}
	    }
	  else if (resKeywords[n + 2] == C)
	    {
	      if (hasToolC)
		errorCode = TOOL_C_USED_TWICE;
	      else
		{
		  hasToolC = true;
		  toolCvalue = resDoubles[n + 4];
		}
	    }
	  else
	    errorCode = BAD_RESPONSE_ITEMS;
	  n = (n + 7);
	}
      else if (resKeywords[n] == IJK)
	{
	  if ((itemCount < (n + 8)) ||
	      (resTypes[n + 1] != RESPARENLEFT)  ||
	      (resTypes[n + 2] != RESDOUBLE)     ||
	      (resTypes[n + 3] != RESCOMMA)      ||
	      (resTypes[n + 4] != RESDOUBLE)     ||
	      (resTypes[n + 5] != RESCOMMA)      ||
	      (resTypes[n + 6] != RESDOUBLE)     ||
	      (resTypes[n + 7] != RESPARENRIGHT) ||
	      ((itemCount != (n + 8)) && (resTypes[n + 8] != RESCOMMA)))
	    errorCode = BAD_RESPONSE_ITEMS;
	  else if (hasIJK)
	    errorCode = IJK_USED_TWICE;
	  else
	    {
	      hasIJK = true;
	      Ivalue = resDoubles[n + 2];
	      Jvalue = resDoubles[n + 4];
	      Kvalue = resDoubles[n + 6];
	      n = (n + 9);
	    }
	}
      else
	errorCode = BAD_RESPONSE_ITEMS;
    }
  return ((errorCode != OK) ? NULL:
	  new PtMeasOrGetResponse (tag, hasER, hasIJK, hasR, hasToolA,
				   hasToolB, hasToolC, hasX, hasY, hasZ,
				   ERvalue, Ivalue, Jvalue, Kvalue, Rvalue,
				   toolAvalue, toolBvalue, toolCvalue,
				   Xvalue, Yvalue, Zvalue));
}

/*******************************************************************/

/* parserRes::makeScanResponse

Returned Value: A ScanResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors:
1. If the number of response items is not odd:
   BAD_RESPONSE_ITEMS.
2. If the first of a pair of response items is not a double:
   BAD_RESPONSE_ITEMS
3. If the second of a pair of response items (followed by more response
   items) is not a comma: BAD_RESPONSE_ITEMS.

This is called when (resTypes[0] == RESDOUBLE). Scan data is just
doubles separated by commas.

Example 1. 00010 # 1, 2, 3, 4, 5, 6

Reference pages: 24 27 (41) (81) 82 83 84 86 87 88 90 91 [93] 103

*/

Response * parserRes::makeScanResponse()
{
  int n;
  double * scanData;
  
  responseName = DataScan;
  if ((itemCount % 2) != 1)
    errorCode = BAD_RESPONSE_ITEMS;
  scanData = new double[(itemCount + 1) / 2];
  for (n = 0; (errorCode == OK); n++)
    {
      if (resTypes[n] == RESDOUBLE)
	scanData[n / 2] = resDoubles[n];
      else
	errorCode = BAD_RESPONSE_ITEMS;
      n++;
      if (n >= itemCount)
	break;
      if (resTypes[n] != RESCOMMA)
	errorCode = BAD_RESPONSE_ITEMS;
    }
  return ((errorCode != OK) ? NULL :
	  new ScanResponse(tag, ((itemCount + 1) / 2), scanData));
}

/*******************************************************************/

/* parserRes::makeServerInitiatedResponse

Returned Value: A data Response of the appropriate type as described
   by the response text, or NULL if the response text is bad.

Called By:  parserRes::parseResponse

Errors:
1. The response items do not conform to any defined pattern of response
   items: BAD_RESPONSE_ITEMS.

This examines the type of the first response item (and possibly other
information about response items), decides what type of response it must
be, and calls a subordinate function to make a response of that type.

This is called only if the tag is known to be E0000.

*/

Response * parserRes::makeServerInitiatedResponse()
{
  Response * aResponse;

  if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == ChangeToolKey))
    aResponse = makeChangeToolEResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == GoToKey))
    aResponse = makeGoToEResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == KeyPress))
    aResponse = makeKeyPressEResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == PtMeasKey))
    aResponse = makePtMeasEResponse();
  else if ((resTypes[0] == RESKEYWORD) && (resKeywords[0] == SetPropKey))
    aResponse = makeSetPropEResponse();
  else
    {
      aResponse = NULL;
      errorCode = BAD_RESPONSE_ITEMS;
    }
  return aResponse;
}

/*******************************************************************/

/* parserRes::makeSetPropEResponse

Returned Value: A SetPropEResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  makeServerInitiatedResponse

Errors:
1. If the response items do not match one of the two patterns:
   SetProp(key1.key2.key3(val)) -- 11 components in itemCount
   SetProp(key1.key2.key3.key4(val)) -- 13 components in argCount
   BAD_RESPONSE_ITEMS
2. key1 must be FoundTool or Tool.
   BAD_KEYWORD
3. key2 must be GoToPar or PtMeasPar.
   BAD_KEYWORD
4. If key2 is GoToPar, key3 must be either Speed or Accel. If key2 is
   PtMeasPar, key3 must be one of Speed, Accel, Approach, Retract, or Search.
   BAD_KEYWORD
5. If there is a key4, it must be Actual.
   BAD_KEYWORD

This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] == SetPropKey), so those conditions are not checked here.

A SetPropE response is server-initiated and is not a response to a command.

Example 1. E0000 # SetProp(Tool.GoToPar.Speed(55))

server-initiated SetProp Reference pages: (78)
other SetProp Reference pages: 24 26 29 (45) 65 [73] 77 99 108
Also see the reference pages for GetProp, since several of those imply
that certain properties cannot be set directly.

*/

Response * parserRes::makeSetPropEResponse()
{
  otherKeyType key4;
  double propertyValue;

  responseName = DataSetPropE;
  if ((itemCount < 11) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESKEYWORD)   ||
      (resTypes[3] != RESDOT)       ||
      (resTypes[4] != RESKEYWORD)   ||
      (resTypes[5] != RESDOT)       ||
      (resTypes[6] != RESKEYWORD))
    errorCode = BAD_RESPONSE_ITEMS;
  else if (((resKeywords[2] != Tool   ) && (resKeywords[2] != FoundTool)) ||
	   ((resKeywords[4] != GoToPar) && (resKeywords[4] != PtMeasPar)) ||
	   ((resKeywords[6] != Speed) && 
	    (resKeywords[6] != Accel) &&
	    ((resKeywords[4] == GoToPar) ||
	     ((resKeywords[6] != Approach) &&
	      (resKeywords[6] != Retract) &&
	      (resKeywords[6] != Search)))))
    errorCode = BAD_KEYWORD;
  else if (itemCount == 11)
    {
      if ((resTypes[7]  != RESPARENLEFT)  ||
	  (resTypes[8]  != RESDOUBLE)     ||
	  (resTypes[9]  != RESPARENRIGHT) ||
	  (resTypes[10] != RESPARENRIGHT))
	errorCode = BAD_RESPONSE_ITEMS;
      else
	{
	  key4 = EmptyKey;
	  propertyValue = resDoubles[8];
	}
    }
  else if (itemCount == 13)
    {
      if ((resTypes[7]  != RESDOT)        ||
	  (resTypes[8]  != RESKEYWORD)    ||
	  (resTypes[9]  != RESPARENLEFT)  ||
	  (resTypes[10] != RESDOUBLE)     ||
	  (resTypes[11] != RESPARENRIGHT) ||
	  (resTypes[12] != RESPARENRIGHT))
	errorCode = BAD_RESPONSE_ITEMS;
      else if (resKeywords[8] != Actual)
	errorCode = BAD_KEYWORD;
      else
	{
	  key4 = (otherKeyType)resKeywords[8];
	  propertyValue = resDoubles[10];
	}
    }
  else
    errorCode = BAD_RESPONSE_ITEMS;
  return ((errorCode != OK) ? NULL :
	  new SetPropEResponse((otherKeyType)resKeywords[2],
			       (otherKeyType)resKeywords[4],
			       (otherKeyType)resKeywords[6],
			       key4, propertyValue));
}

/*******************************************************************/

/* parserRes::makeStringResponse

Returned Value: A StringResponse as described by the response text,
   or NULL if the response text is bad.

Called By:  parserRes::makeDataResponse

Errors: None.

This is called when (itemCount == 1) and (resTypes[0] == RESSTRING),
so those conditions are not checked here.

StringResponse responds to a GetErrorInfo command or an EnumTools command.

Example 1. "No clue"
Example 2. "Probe6"

GetErrorInfo Reference pages: 19 24 26 (43) 76 99
Page 76 does not mention the command but gives allowable error numbers.
EnumTools Reference pages: 24 29 (53) (54) (56) 106

*/

Response * parserRes::makeStringResponse()
{
  responseName = DataString;
  return new StringResponse(tag, resStrings[0]);
}

/*******************************************************************/

/* parserRes::makeTiltCenterPartResponse

Called By:  parserRes::makeDataResponse

Errors: 
1. If the response items do not match the pattern
   TiltCenterPart(double)
   or the double is not a single character: BAD_RESPONSE_ITEMS.
2. If the double is not 0 or 1: VALUE_MUST_BE_0_OR_1.


This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] = TiltCenterPartKey), so those conditions are not checked here.

A TiltCenterPartResponse responds to a TiltCenterPart command.

Example 1. 00010 # TiltCenterPart(1)

Reference pages: 24 27 (97) 102

*/

Response * parserRes::makeTiltCenterPartResponse()
{
  responseName = DataTiltCenterPart;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESDOUBLE)    ||
      (resTypes[3] != RESPARENRIGHT)||
      (inputArray[24] != ')'))
    errorCode = BAD_RESPONSE_ITEMS;
  else if ((resDoubles[2] != 0) && (resDoubles[2] != 1))
    errorCode = VALUE_MUST_BE_0_OR_1;
  return ((errorCode != OK) ? NULL :
	  new TiltCenterPartResponse(tag,((resDoubles[2] == 1) ? true :false)));
}

/*******************************************************************/

/* parserRes::makeTiltPartResponse

Called By:  parserRes::makeDataResponse

Errors: 
1. If the response items do not match the pattern
   TiltPart(double)
   or the double is not a single character: BAD_RESPONSE_ITEMS.
2. If the double is not 0 or 1: VALUE_MUST_BE_0_OR_1.


This is called when (resTypes[0] == RESKEYWORD) and
(resKeywords[0] = TiltPartKey), so those conditions are not checked here.

A TiltPartResponse responds to a TiltPart command.

Example 1. 00010 # TiltPart(1)

Reference pages: 24 27 (97) 102

*/

Response * parserRes::makeTiltPartResponse()
{
  responseName = DataTiltPart;
  if ((itemCount != 4) ||
      (resTypes[1] != RESPARENLEFT) ||
      (resTypes[2] != RESDOUBLE)    ||
      (resTypes[3] != RESPARENRIGHT)||
      (inputArray[18] != ')'))
    errorCode = BAD_RESPONSE_ITEMS;
  else if ((resDoubles[2] != 0) && (resDoubles[2] != 1))
    errorCode = VALUE_MUST_BE_0_OR_1;
  return ((errorCode != OK) ? NULL :
	  new TiltPartResponse(tag, ((resDoubles[2] == 1) ? true : false)));
}

/*******************************************************************/

/* parserRes::parseKeyword

Returned Value: None.

Called By: parserRes::parseResItems

Errors:
1. If the letters starting with inputArray[arrayIndex] do not make
   any defined keyword: BAD_KEYWORD.
2. If the keyword is not followed by a dot, comma, left parenthesis,
   or right parenthesis: BAD_CHARACTER_AFTER_KEYWORD.

This is called only if the character at inputArray[arrayIndex] is an
upper case letter.

If there are extra letters after a valid keyword, that is detected.

If one keyword is the same as the beginning of another keyword, a check
for the longer keyword must be made first. There are several cases of that.

When the keyword is a single letter, it is not necessary to use a
check like [if (strncmp(keyword, "A", 1) == 0)] since that is already
known from the case statement to be true, but those checks are
included just to keep the code looking consistent.

If the keyword is OK, it is stored in the parser's resKeywords[itemCount].

*/


void parserRes::parseKeyword()
{
  char * keyword;
  int length;

  resKeywords[itemCount] = -1;
  errorCode = BAD_KEYWORD;
  keyword = (inputArray + arrayIndex);
  switch (keyword[0])
    {
    case 'A':
      if (strncmp(keyword, "Accel", 5) == 0)
	{
	  resKeywords[itemCount] = Accel;
	  length = 4;
	}
      else if (strncmp(keyword, "Act", 3) == 0)
	{
	  resKeywords[itemCount] = Actual;
	  length = 2;
	}
      else if (strncmp(keyword, "Alignment", 9) == 0)
	{
	  resKeywords[itemCount] = Alignment;
	  length = 8;
	}
      else if (strncmp(keyword, "Approach", 8) == 0)
	{
	  resKeywords[itemCount] = Approach;
	  length = 7;
	}
      else if (strncmp(keyword, "A", 1) == 0)
	{
	  resKeywords[itemCount] = A;
	  length = 0;
	}
      break;
    case 'B':
      if (strncmp(keyword, "B", 1) == 0)
	{
	  resKeywords[itemCount] = B;
	  length = 0;
	}
      break;
    case 'C':
      if (strncmp(keyword, "CenterPart", 10) == 0)
	{
	  resKeywords[itemCount] = CenterPartKey;
	  length = 9;
	}
      else if (strncmp(keyword, "ChangeToolAction", 16) == 0)
	{
	  resKeywords[itemCount] = ChangeToolAction;
	  length = 15;
	}
      else if (strncmp(keyword, "ChangeTool", 10) == 0)
	{
	  resKeywords[itemCount] = ChangeToolKey;
	  length = 9;
	}
      else if (strncmp(keyword, "CollisionVolume", 15) == 0)
	{
	  resKeywords[itemCount] = CollisionVolume;
	  length = 14;
	}
      else if (strncmp(keyword, "CoordSystem", 11) == 0)
	{
	  resKeywords[itemCount] = CoordSystem;
	  length = 10;
	}
      else if (strncmp(keyword, "C", 1) == 0)
	{
	  resKeywords[itemCount] = C;
	  length = 0;
	}
      break;
    case 'D':
      if (strncmp(keyword, "Def", 3) == 0)
	{
	  resKeywords[itemCount] = Default;
	  length = 2;
	}
      else if (strncmp(keyword, "DMEVersion", 10) == 0)
	{
	  resKeywords[itemCount] = DMEVersion;
	  length = 9;
	}
      break;
    case 'E':
      if (strncmp(keyword, "ER", 2) == 0)
	{
	  resKeywords[itemCount] = ER;
	  length = 1;
	}
      else if (strncmp(keyword, "Error", 5) == 0)
	{
	  resKeywords[itemCount] = ErrorKey;
	  length = 4;
	}
      else if (strncmp(keyword, "ErrStatus", 9) == 0)
	{
	  resKeywords[itemCount] = ErrStatus;
	  length = 8;
	}
      break;
    case 'F':
      if (strncmp(keyword, "FoundTool", 9) == 0)
	{
	  resKeywords[itemCount] = FoundTool;
	  length = 8;
	}
      break;
    case 'G':
      if (strncmp(keyword, "GetCsyTransformation", 20) == 0)
	{
	  resKeywords[itemCount] = GetCsyTr;
	  length = 19;
	}
      else if (strncmp(keyword, "GetMachineClass", 15) == 0)
	{
	  resKeywords[itemCount] = GetMachineKey;
	  length = 14;
	}
      else if (strncmp(keyword, "GoToPar", 7) == 0)
	{
	  resKeywords[itemCount] = GoToPar;
	  length = 6;
	}
      else if (strncmp(keyword, "GoTo", 4) == 0)
	{
	  resKeywords[itemCount] = GoToKey;
	  length = 3;
	}
      break;
    case 'I':
      if(strncmp(keyword, "Id", 2) == 0)
	{
	  resKeywords[itemCount] = Id;
	  length = 1;
	}
      else if(strncmp(keyword, "IJK", 3) == 0)
	{
	  resKeywords[itemCount] = IJK;
	  length = 2;
	}
      else if(strncmp(keyword, "IsHomed", 7) == 0)
	{
	  resKeywords[itemCount] = IsHomedKey;
	  length = 6;
	}
      else if(strncmp(keyword, "IsUserEnabled", 13) == 0)
	{
	  resKeywords[itemCount] = IsUserEnabledKey;
	  length = 12;
	}
      break;
    case 'J':
      if (strncmp(keyword, "JogDisplayCsy", 13) == 0)
	{
	  resKeywords[itemCount] = JogDisplayCsy;
	  length = 12;
	}
      else if (strncmp(keyword, "JogMoveCsy", 10) == 0)
	{
	  resKeywords[itemCount] = JogMoveCsy;
	  length = 9;
	}
      break;
    case 'K':
      if (strncmp(keyword, "KeyPress", 8) == 0)
	{
	  resKeywords[itemCount] = KeyPress;
	  length = 7;
	}
      break;
    case 'M':
      if (strncmp(keyword, "MachineCsy", 10) == 0)
	{
	  resKeywords[itemCount] = MachineCsy;
	  length = 9;
	}
      else if (strncmp(keyword, "Max", 3) == 0)
	{
	  resKeywords[itemCount] = Max;
	  length = 2;
	}
      else if (strncmp(keyword, "Min", 3) == 0)
	{
	  resKeywords[itemCount] = Min;
	  length = 2;
	}
      else if (strncmp(keyword, "MoveableMachineCsy", 18) == 0)
	{
	  resKeywords[itemCount] = MoveableMachineCsy;
	  length = 17;
	}
      else if (strncmp(keyword, "MoveAuto", 8) == 0)
	{
	  resKeywords[itemCount] = MoveAuto;
	  length = 7;
	}
      else if (strncmp(keyword, "MoveMan", 7) == 0)
	{
	  resKeywords[itemCount] = MoveMan;
	  length = 6;
	}
      else if (strncmp(keyword, "MultipleArmCsy", 14) == 0)
	{
	  resKeywords[itemCount] = MultipleArmCsy;
	  length = 13;
	}
      break;
    case 'N':
      if (strncmp(keyword, "Name", 4) == 0)
	{
	  resKeywords[itemCount] = Name;
	  length = 3;
	}
      break;
    case 'O':
      if (strncmp(keyword, "OBB", 3) == 0)
	{
	  resKeywords[itemCount] = OBB;
	  length = 2;
	}
      break;
    case 'P':
      if (strncmp(keyword, "PartCsy", 7) == 0)
	{
	  resKeywords[itemCount] = PartCsy;
	  length = 6;
	}
      else  if (strncmp(keyword, "Phi", 3) == 0)
	{
	  resKeywords[itemCount] = Phi;
	  length = 2;
	}
      else  if (strncmp(keyword, "PtMeasPar", 9) == 0)
	{
	  resKeywords[itemCount] = PtMeasPar;
	  length = 8;
	}
      else if (strncmp(keyword, "PtMeas", 6) == 0)
	{
	  resKeywords[itemCount] = PtMeasKey;
	  length = 5;
	}
      break;
    case 'Q':
      if (strncmp(keyword, "Q", 1) == 0)
	{
	  resKeywords[itemCount] = Q;
	  length = 0;
	}
      break;
    case 'R':
      if (strncmp(keyword, "Retract", 7) == 0)
	{
	  resKeywords[itemCount] = Retract;
	  length = 6;
	}
      else if (strncmp(keyword, "Rotate", 6) == 0)
	{
	  resKeywords[itemCount] = Rotate;
	  length = 5;
	}
      else if (strncmp(keyword, "R", 1) == 0)
	{
	  resKeywords[itemCount] = R;
	  length = 0;
	}
      break;
    case 'S':
      if (strncmp(keyword, "Search", 6) == 0)
	{
	  resKeywords[itemCount] = Search;
	  length = 5;
	}
      else if (strncmp(keyword, "SensorCsy", 9) == 0)
	{
	  resKeywords[itemCount] = SensorCsy;
	  length = 8;
	}
      else if (strncmp(keyword, "SetProp", 7) == 0)
	{
	  resKeywords[itemCount] = SetPropKey;
	  length = 6;
	}
      else if (strncmp(keyword, "Speed", 5) == 0)
	{
	  resKeywords[itemCount] = Speed;
	  length = 4;
	}
      else if (strncmp(keyword, "Switch", 6) == 0)
	{
	  resKeywords[itemCount] = Switch;
	  length = 5;
	}
      break;
    case 'T':
      if (strncmp(keyword, "TiltCenterPart", 14) == 0)
	{
	  resKeywords[itemCount] = TiltCenterPartKey;
	  length = 13;
	}
      else if (strncmp(keyword, "TiltPart", 8) == 0)
	{
	  resKeywords[itemCount] = TiltPartKey;
	  length = 7;
	}
      else if (strncmp(keyword, "Tool", 4) == 0)
	{
	  resKeywords[itemCount] = Tool;
	  length = 3;
	}
      break;
    case 'X':
      if (strncmp(keyword, "X", 1) == 0)
	{
	  resKeywords[itemCount] = X;
	  length = 0;
	}
      break;
    case 'Y':
      if (strncmp(keyword, "Y", 1) == 0)
	{
	  resKeywords[itemCount] = Y;
	  length = 0;
	}
      break;
    case 'Z':
      if (strncmp(keyword, "Z", 1) == 0)
	{
	  resKeywords[itemCount] = Z;
	  length = 0;
	}
      break;
    default:
      break;
    }
  if (resKeywords[itemCount] != -1)
    {
      arrayIndex = (arrayIndex + length);
      if ((inputArray[arrayIndex + 1] == '.') ||
	  (inputArray[arrayIndex + 1] == ',') ||
	  (inputArray[arrayIndex + 1] == '(') ||
	  (inputArray[arrayIndex + 1] == ')'))
	errorCode = OK;
      else
	errorCode = BAD_CHARACTER_AFTER_KEYWORD;
    }
}

/*******************************************************************/

/* parserRes::parseNumber

Returned Value: None.

Called By: parserRes::parseResItems

Errors:
1. If the characters that might form a number include two dots:
   BAD_NUMBER_TWO_DECIMAL_POINTS.
2. If the characters that might form a number include no digits:
   BAD_NUMBER_NO_DIGITS.
3. If the characters that might form a number include more than
   sixteen digits: BAD_NUMBER_MORE_THAN_16_DIGITS.
4. If the number has an exponent and the exponent does not have
   one, two, or three digits:
   BAD_E_NUMBER_EXPONENT_MUST_HAVE_ONE_TWO_OR_THREE_DIGITS.

This is called only if the response item being parsed starts with a
plus sign, minus sign, digit, or [decimal point not followed by an
upper case letter].

If the number is OK, it is stored in the parser's resDoubles[itemCount].

Reference pages: (33 - 34)

*/

void parserRes::parseNumber()
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
      sscanf((inputArray + arrayIndex), "%lf", (resDoubles + itemCount));
      inputArray[k] = save; // put char back
      arrayIndex = (k - 1); // set arrayIndex to end of number
    }
}

/*******************************************************************/

/* parserRes::parseResItems

Returned Value: None.

Called By: parserRes::parseResponse

Errors:
1. If the beginning of the response items does not match any allowed
   pattern: BAD_RESPONSE_ITEMS.

A dot may appear either before a capital letter (as in Tool.PtMeasPar)
or at the beginning of a number (as in .123). In the first case it is
a response item. In the second case it is part of the number.

*/

void parserRes::parseResItems()
{
  char c;
  char d;

  arrayIndex = 8;
  for(itemCount = 0; ((c = inputArray[arrayIndex]) != 13); itemCount++)
    {
      if (itemCount == resSize)
	makeArraysBigger();
      d = inputArray[arrayIndex + 1];
      if (c == '(')
	resTypes[itemCount] = RESPARENLEFT;
      else if (c == ')')
	resTypes[itemCount] = RESPARENRIGHT;
      else if (c == 34) /* open quote */
	{
	  parseString();
	  resTypes[itemCount] = RESSTRING;
	}
      else if (c == ',')
	resTypes[itemCount] = RESCOMMA;
      else if ((c == '.') && (d > 64) && (d < 91)) /* dot before upper letter */
	resTypes[itemCount] = RESDOT;
      else if ((c == '-') ||
	       (c == '+') ||
	       (c == '.') ||
	       ((c > 47) && (c < 58)))
	{
	  parseNumber();
	  resTypes[itemCount] = RESDOUBLE;
	}
      else if ((c > 64) && (c < 91))
	{
	  parseKeyword();
	  resTypes[itemCount] = RESKEYWORD;
	}
      else
	errorCode = BAD_RESPONSE_ITEMS;
      if (errorCode != OK)
	break;
      arrayIndex++; /* move to character after end of item just parsed */
    }
}

/*******************************************************************/

/* parserRes::parseResponse

Returned Value: A Response (if the inputArray holds a valid response)
  of NULL (if not).

Called By:
  external functions
  main (in stand-alone response parser)

Errors:
1. If the response items form an allowed response but are not followed
   in the inputArray by  ASCII 13   ASCII 10  ASCII 0  :
   BAD_CHARACTER_AFTER_RESPONSE_END.
2. If the seventh character of the response is not one of  & % ! #  :
   BAD_SEVENTH_CHARACTER_IN_RESPONSE.
3. if the response is an error response or a data response, and the eighth
   character of the response is not a space:
   SPACE_MISSING_AT_EIGHTH_CHARACTER_OF_RESPONSE.

The setInput function copies a response string into the parser's
inputArray, stopping at the first ASCII 0, so the check for ASCII 0 is
reasonable.

parseTag sets the errorCode to OK if the tag is OK syntactically.

*/

Response * parserRes::parseResponse()
{
  Response * aResponse;

  responseName = -1;
  parseTag();
  if (errorCode == OK)
    {
      if (inputArray[6] == '&')
	{
	  if ((inputArray[7] != 13) ||
	      (inputArray[8] != 10) ||
	      (inputArray[9] != 0))
	    errorCode = BAD_CHARACTER_AFTER_RESPONSE_END;
	  else
	    aResponse = new AckResponse(tag, tagType);
	}
      else if (inputArray[6] == '%')
	{
	  if ((inputArray[7] != 13) ||
	      (inputArray[8] != 10) ||
	      (inputArray[9] != 0))
	    errorCode = BAD_CHARACTER_AFTER_RESPONSE_END;
	  else
	    aResponse = new CompleteResponse(tag, tagType);
	}
      else if ((inputArray[6] != '!') &&(inputArray[6] != '#'))
	errorCode = BAD_SEVENTH_CHARACTER_IN_RESPONSE;
      else if (inputArray[7] != ' ')
	errorCode = SPACE_MISSING_AT_EIGHTH_CHARACTER_OF_RESPONSE;
      else
	{
	  unSpaceInputArray();
	  if (errorCode == OK)
	    parseResItems();
	  if ((errorCode == OK) &&
	      ((inputArray[arrayIndex] != 13) ||
	       (inputArray[arrayIndex + 1] != 10) ||
	       (inputArray[arrayIndex + 2] != 0)))
	    errorCode = BAD_CHARACTER_AFTER_RESPONSE_END;
	  if (errorCode == OK)
	    {
	      if (inputArray[6] == '!')
		aResponse = makeErrorResponse();
	      else if (tag == 0) // tag is E0000 and (inputArray[6] == '#')
		aResponse = makeServerInitiatedResponse();
	      else // (inputArray[6] == '#')
		aResponse = makeDataResponse();
	    }
	}
    }
  return ((errorCode == OK) ? aResponse : NULL);
}

/*******************************************************************/

/* parserRes::parseString

Returned Value: None.

Called By: parserRes::parseResItems

Errors:
1. The string being read has an illegal character:
   BAD_STRING.

This is called whenever the first character of a response item is a
double-quote (ASCII 34). Reading the string stops at the second
double-quote.

If the string is OK it is stored in the parser's resStrings[itemCount].

Reference pages: 10 (33) 34 35 43 46 66 73 99 102 106 107 108 109
These references are only those where string is the data type of a
component of a command or a response. The word string is used in other
places to refer to an entire command or response.

A String starts with a double quote (ASCII 34) and ends at the
next double quote. A double quote cannot be inside a string.
When this function starts, arrayIndex is indicating the opening
double quote.

*/


void parserRes::parseString()
{
  int start;

  arrayIndex++; // skip starting quote
  start = arrayIndex;
  for (; (inputArray[arrayIndex] != 34); arrayIndex++)
    {
      if ((inputArray[arrayIndex] < 32) || (inputArray[arrayIndex] > 126)) 
	{
	  errorCode = BAD_STRING;
	  return;
	}
    }
  inputArray[arrayIndex] = 0; // set temporary null terminator
  resStrings[itemCount] = strdup(inputArray + start); // hook copy in
  inputArray[arrayIndex] = 34; // restore quote at end
}

/*******************************************************************/

/* parserRes::parseTag

Returned Value: None.

Called By: parserRes::parseResponse

Errors:
1. If the first character of the response is not E or a digit, or any
   of the next four characters is not a digit: BAD_TAG_CHARACTER.
2. If the sixth character of the response is not a space:
   SPACE_MISSING_AFTER_TAG.
2. If the first character is a digit and the tag number is zero:
   TAG_NUMBER_OUT_OF_RANGE_FOR_COMMAND_TAG.
4. If the tag is E0000 and the seventh character is not ! or #
   ZERO_TAG_MUST_BE_SERVER_INITIATED_RESPONSE.

If the tag is OK, its type (as indicated by the first character) is
stored in the "tagType" data member of the parser, and the value of
the tag is stored in the "tag" data member.

Reference pages: (21) (22) 26-30 (34-42) (47) (49) (50) (75) (77) 99-113

There should be a tag at the beginning of the inputArray. This function
reads that tag.

*/

void parserRes::parseTag()
{
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
      sscanf((inputArray + 1), "%d", &tag);
      if ((tag == 0) && (inputArray[6] != '!') && (inputArray[6] != '#'))
	errorCode = ZERO_TAG_MUST_BE_SERVER_INITIATED_RESPONSE;
      else
	{
	  tagType = EventTag;
	  errorCode = OK;
	}
    }
  else if ((inputArray[0] > 47) && (inputArray[0] < 58))
    {
      sscanf(inputArray, "%d", &tag);
      if (tag == 0)
	errorCode = TAG_NUMBER_OUT_OF_RANGE_FOR_COMMAND_TAG;
      else
	{
	  tagType = CommandTag;
	  errorCode = OK;
	}
    }
  else
    errorCode = BAD_TAG_CHARACTER;
}

/*******************************************************************/

/* parserRes::setInput

Returned Value: None.

Called By:
  external functions
  main (in stand-alone response parser)

Errors: None.

This copies the input string into the parser's inputArray. If the input
string is too long, it is truncated.

*/

void parserRes::setInput(
 char * input)
{
  strncpy(inputArray, input, (IPPSIZE - 1));
}


/*******************************************************************/

/* parserRes::unSpaceInputArray

Returned Value: None.

Called By: parserRes::parseResponse

Errors:
1. Starting with the ninth position of a response string, if a space is
   found that is not before or after a comma, before or after a left or
   right parenthesis, or in a string:
   ILLEGAL_SPACE.

Reference pages: (33 - 34)

Notes:

Starting at the ninth position (index 8), this removes from the
inputArray all spaces outside of strings. It does this by moving
characters from right to left as necessary to fill in any spaces.

The n-1 in the termination condition of the "for" loop allows the NULL
to be copied also.

*/

void parserRes::unSpaceInputArray()
{
  char c;
  int n;
  int gap = 0;
  bool stringing = false;
  bool spaceBefore = false;
  bool puncBefore = false;

  for (n = 8; inputArray[n-1] != NULL; n++)
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

