/* parserRes.h

*/

#ifndef PARSERRES_HH
#define PARSERRES_HH

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INITSIZE 64

/* error codes

*/

enum parserResErrorId {
  OK = 0,
  BAD_CHARACTER_AFTER_KEYWORD,
  BAD_CHARACTER_AFTER_RESPONSE_END,
  BAD_COORDINATE_SYSTEM,
  BAD_E_NUMBER_EXPONENT_MUST_HAVE_ONE_TWO_OR_THREE_DIGITS,
  BAD_ERROR_CODE,
  BAD_GOTOPAR_PROPERTY,
  BAD_KEYWORD,
  BAD_MACHINE_CLASS,
  BAD_NUMBER_MORE_THAN_16_DIGITS,
  BAD_NUMBER_NO_DIGITS,
  BAD_NUMBER_TWO_DECIMAL_POINTS,
  BAD_PTMEASPAR_PROPERTY,
  BAD_RESPONSE_ITEMS,
  BAD_SECOND_KEYWORD,
  BAD_SEVENTH_CHARACTER_IN_RESPONSE,
  BAD_SEVERITY,
  BAD_STRING,
  BAD_TAG_CHARACTER,
  BAD_TEXT,
  BAD_THIRD_KEYWORD,
  BUG,
  ER_USED_TWICE,
  ERROR_CODE_MUST_HAVE_FOUR_DIGITS,
  FIRST_ARGUMENT_MUST_BE_SWITCH_ROTATE_MOVEAUTO_OR_MOVEMAN,
  IJK_USED_TWICE,
  IJK_VALUES_MUST_BE_ZERO,
  ILLEGAL_SPACE,
  R_USED_TWICE,
  RESPONSE_NAME_MUST_BE_ERROR,
  SEVERITY_MUST_HAVE_ONE_DIGIT,
  SPACE_MISSING_AFTER_TAG,
  SPACE_MISSING_AT_EIGHTH_CHARACTER_OF_RESPONSE,
  TAG_NUMBER_OUT_OF_RANGE_FOR_COMMAND_TAG,
  TOOL_A_USED_TWICE,
  TOOL_B_USED_TWICE,
  TOOL_C_USED_TWICE,
  VALUE_MUST_BE_0_OR_1,
  X_USED_TWICE,
  Y_USED_TWICE,
  Z_USED_TWICE,
  ZERO_TAG_MUST_BE_SERVER_INITIATED_RESPONSE
};

enum resItemType {RESCOMMA, RESDOT, RESDOUBLE, RESKEYWORD,
			  RESPARENLEFT, RESPARENRIGHT, RESSTRING};

/* parserRes

To use the parser, the setInput() method must first be called to copy an
input string into the parser's inputArray. An alternative would be to
have a string as an argument to the parseXXX methods. Copying to
inputArray takes time, but it prevents the parser from messing up the
original string.

The caller should then call parseResponse(), which makes an instance of
the Response from the input string (if there is no error in the string).
If there is an error in the string, parseResponse returns NULL.

After calling parseResponse, the caller should call getParserErr()
to see if there was an error. If the returned error code is not OK,
the caller can call getErrorMessageString(buffer) to get an error message
that describes the error. The buffer should be at least 100 chars long.
If no error occurs until after the responseNameType is identified, the
responseNameType is given at the beginning of the error message.

If the error code is OK, the caller can start processing the Response
instance.

The caller may call parseTag() before calling parseResponse(), but that
is not required.

*/

class parserRes
{
public:
  parserRes()
    {
      itemCount = 0;
      resSize = INITSIZE;
      resDoubles = new double[resSize];
      resKeywords = new int[resSize];
      resStrings = new char * [resSize];
      resTypes = new int[resSize];
      arrayIndex = 0;
    }
  ~parserRes()
    {
      delete resDoubles;
      delete resKeywords;
      delete resStrings;  // might delete the strings, too
      delete resTypes;
    }
  char * getErrorMessageString(char * buffer); // might add length argument
  int getParserErr(){return errorCode;}
  tagIdType getTagType() {return tagType;}
  int getTag() {return tag;}
  Response * parseResponse();
  void parseTag();
  void setInput(char * input);

private:
  int arrayIndex;           // current index for inputArray
  int errorCode;            // parser error code
  char inputArray[IPPSIZE]; // array to put response in for parsing
  int itemCount;            // number of response components
  double * resDoubles;      // array of doubles for response components
  int * resKeywords;        // array of keywords for response components
  int resSize;              // size of resDoubles, resKeywords, etc. 
  char ** resStrings;       // array of pointers to strings for res components
  int * resTypes;           // array of response component types
  int responseName;         // enumeration value of response
  tagIdType tagType;
  int tag;                  // tag number of command being parsed

  void makeArraysBigger();
  OnePropertyAlignment * makeOnePropertyAlignment(int * n);
  OnePropertyCollisionVolume * makeOnePropertyCollisionVolume(int * n);
  OnePropertyId * makeOnePropertyId(int * n);
  OnePropertyName * makeOnePropertyName(int * n);
  OnePropertyNumber * makeOnePropertyNumber(int * n);
  Response * makeAlignResponse();
  Response * makeCenterPartResponse();
  Response * makeChangeToolEResponse();
  Response * makeDataResponse();
  Response * makeEnumPropResponse();
  Response * makeErrorResponse();
  Response * makeGetChangeToolActionResponse();
  Response * makeGetCoordSystemResponse();
  Response * makeGetCsyTransformationResponse();
  Response * makeGetDMEVersionResponse();
  Response * makeGetErrStatusEResponse();
  Response * makeGetMachineClassResponse();
  Response * makeGetPropResponse();
  Response * makeGoToEResponse();
  Response * makeIsHomedResponse();
  Response * makeIsUserEnabledResponse();
  Response * makeKeyPressEResponse();
  Response * makePtMeasEResponse();
  Response * makePtMeasOrGetResponse();
  Response * makeScanResponse();
  Response * makeServerInitiatedResponse();
  Response * makeSetPropEResponse();
  Response * makeStringResponse();
  Response * makeTiltCenterPartResponse();
  Response * makeTiltPartResponse();
  void parseResItems();
  void parseKeyword();
  void parseNumber();
  void parseString();
  void unSpaceInputArray();
};

#endif /* #ifndef PARSERRES_HH */





