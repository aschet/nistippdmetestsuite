
// Implementation of I++ DME Version 1.1
// Source Code: IppCmdRes.h
// Created: 08-5-02 Falco

// Changed to release 1.30: 06-19-03 Kramer
// Added allowing multiple arguments for GetProp, GetPropE, SetProp
// Added MAXPROP (upper limit on arguments to GetProp, GetPropE, SetProp)
// Added MAXSCAN (upper limit on number of double in one ScanResponse)
// Added AlignPart, EnumTools, OnScanReport, 10 Scans
// Added ToolA, ToolB, ToolC to:
//   GetCommand, GotoCommand, OnMoveReportCommand, OnPtMeasReport
// Added set methods to all commands
// Added A, B, C, Q, R
// Changed names of error messages 2, 3, and 4. Added 7, 8, and 2506
// Added MaxApproach, MaxRetract, MaxSearch,MinApproach, MinRetract, MinSearch
// Changed AlignToolResponse to AlignResponse
// Changed GetErrorInfoResponse to StringResponse
// Added ScanResponse
// Reformatted and alphabetized

// Changed to release 1.40: 05-11-04 Kramer
// Got rid of upper limit on arguments (MAXARGS), properties (MAXPROP),
//   and scan data entries (MAXSCAN)
// Deleted MaxApproach, MaxRetract, MaxSearch,MinApproach,MinRetract,MinSearch
// Added Commands: CenterPart, GetChangeToolAction,
//   GetDMEVersion, LockAxis, TiltCenterPart, TiltPart
// Added Error message 0515
// Renamed several error messages
// Revised GetProp, GetPropE, SetProp commands to take two to four components
// Revised OnPtMeasReport, OnScanReport commands to take ER and IJK
// Revised various responses to take ER and IJK
// Added response types: DataCenterPart, DataChangeToolE,
//   DataGetChangeToolAction, DataGetDMEVersion, DataGoToE, DataKeyPressE,
//   DataOnMoveReportE, DataPtMeasE, DataTiltCenterPart, DataTiltPart,
//   DataSetPropE
// Changed response class: XYZResponse
// Added response classes: CenterPartResponse, ChangeToolEResponse,
//   GetChangeToolActionResponse, GetDMEVersionResponse, GoToEResponse,
//   KeyPressEResponse, OnMoveReportEResponse, PtMeasOrGetResponse,
//   PtMeasEResponse, SetPropEResponse, TiltCenterPartResponse,
//   TiltPartResponse
// Deleted PropertyResponse
// Added GetPropResponse (for GetProp and GetPropE)
// Added helper classes (OneProperty and 5 OnePropertyXXX) for GetPropResponse
// Added Box class used in OnePropertyCollisionVolume
// Changed GetErrorInfoCommand class
// Added IPPSIZE and uses of it. Removed STRINGSIZE.
// Moved key strings array from IppCmdRes.cc to IppCmdRes.h


#ifndef IPPCMDRES_H
#define IPPCMDRES_H

#include <math.h>
#include <stdio.h>
#include <string.h>

/*

IPPSIZE is given as 65536 on page 35 of the spec. The buffers into
which commands and responses are written are expected to be this size
or smaller. Overflow on writing to a buffer smaller than IPPSIZE is
being checked. Overflow on writing to a buffer larger than IPPSIZE is
not being checked.

*/

#define TAGSTRINGSIZE 6
#define IPPSIZE  65536

typedef enum tagIdType {
  CommandTag = 1,
  EventTag   = 2};


namespace ipp {

typedef enum commandNameType {

  AbortE                 =  2,
  AlignPart              =  3,
  AlignTool              =  4,
  CenterPart             =  5,
  ChangeTool             =  6,
  ClearAllErrors         =  7,
  DisableUser            =  8,
  EnableUser             =  9,
  EndSession             = 10,
  EnumAllProp            = 11,
  EnumProp               = 12,
  EnumTools              = 13,
  FindTool               = 14,
  Get                    = 15,
  GetChangeToolAction    = 16,
  GetCoordSystem         = 17,
  GetCsyTransformation   = 18,
  GetDMEVersion          = 19,
  GetErrorInfo           = 20,
  GetErrStatusE          = 21,
  GetMachineClass        = 22,
  GetProp                = 23,
  GetPropE               = 24,
  GetXtdErrStatus        = 25,
  GoTo                   = 26,
  Home                   = 27,
  IsHomed                = 28,
  IsUserEnabled          = 29,
  LockAxis               = 30,
  OnMoveReportE          = 31,
  OnPtMeasReport         = 32,
  OnScanReport           = 33,
  PtMeas                 = 34,
  ReQualify              = 35,
  ScanInCylEndIsPlane    = 36,
  ScanInCylEndIsSphere   = 37,
  ScanInPlaneEndIsCyl    = 38,
  ScanInPlaneEndIsPlane  = 39,
  ScanInPlaneEndIsSphere = 40,
  ScanOnCircle           = 41,
  ScanOnCircleHint       = 42,
  ScanOnLine             = 43,
  ScanOnLineHint         = 44,
  ScanUnknownHint        = 45,
  SetCoordSystem         = 46,
  SetCsyTransformation   = 47,
  SetProp                = 48,
  SetTool                = 49,
  StartSession           = 50,
  StopAllDaemons         = 51,
  StopDaemon             = 52,
  TiltCenterPart         = 53,
  TiltPart               = 54}; 
} // end of namespace ipp

typedef enum errorNameType {
  BufferFull                = 0,
  IllegalTag                = 1,
  NoSpaceAtPos6             = 2,
  Reserved3                 = 3,
  Reserved4                 = 4,
  Reserved5                 = 5,
  TransactionAborted        = 6,
  IllegalCharacter          = 7,
  ProtocolError             = 8,
  EmergencyStop             = 500,
  UnsupportedCommand        = 501, 
  IncorrectArguments        = 502, 
  ControllerCommFailure     = 503,
  ArgumentOutOfRange        = 504,
  ArgumentNotRecognized     = 505,
  ArgumentNotSupported      = 506,
  IllegalCommand            = 507,
  BadContext                = 508,
  BadArgument               = 509,
  BadProperty               = 510,
  ErrorProcessingMethod     = 511,
  NoDaemonsAreActive        = 512,
  DaemonDoesNotExist        = 513,
  UseClearAllErrorsToCont   = 514,
  DaemonAlreadyExists       = 515,
  MachineInErrorState       = 1000,
  IllegalTouch              = 1001,
  AxisDoesNotExist          = 1002,
  NoTouch                   = 1003,
  NumAnglNotSupOnCurrDev    = 1004,
  ErrorDuringHome           = 1005,
  SurfaceNotFound           = 1006,
  ThetaOutOfRange           = 1007,
  TargetPosOutOfMachVol     = 1008,
  AirPressureOutOfRange     = 1009,
  VectorHasNoNorm           = 1010,
  FailedToReseatHead        = 1500,
  ProbeNotArmed             = 1501,
  ToolNotFound              = 1502,
  ToolNotDefined            = 1503,
  ToolNotCalibrated         = 2000,
  HeadErrorExcessiveForce   = 2001,
  TypeProbeDoesNotAllowOper = 2002,
  MachineLimitEncountered   = 2500,
  AxisNotActive             = 2501,
  AxisPositionError         = 2502,
  ScaleReadHeadFailure      = 2503,
  Collision                 = 2504,
  SpecifiedAngleOutOfRange  = 2505,
  PartNotAligned            = 2506};


typedef enum responseNameType {
  Ack                      =  1,  // 7th char is a &
  Complete                 =  2,  // 7th char is a %
  DataAlign                =  3,  // 7th char is a #
  DataCenterPart           =  4,
  DataChangeToolE          =  5,
  DataEnumProp             =  6,
  DataGetChangeToolAction  =  7,
  DataGetCoordSystem       =  8,
  DataGetCsyTransformation =  9,
  DataGetDMEVersion        = 10,
  DataGetErrStatusE        = 11,
  DataGetMachineClass      = 12,
  DataGetProp              = 13,
  DataGoToE                = 14,
  DataIsHomed              = 15,
  DataIsUserEnabled        = 16,
  DataKeyPressE            = 17,
  DataOnMoveReportE        = 18,
  DataPtMeasOrGet          = 19,
  DataPtMeasE              = 20,
  DataScan                 = 21,
  DataSetPropE             = 22,
  DataString               = 23,
  DataTiltCenterPart       = 24,
  DataTiltPart             = 25,
  ErrorError               = 26}; // 7th char is a !
  
/*

The values in the following four enums are used as indexes into the
ippKeyStrings array below.

*/

typedef enum targetPosType {
  A    = 1,
  B    = 2,
  C    = 3,
  R    = 4,
  X    = 5,
  Y    = 6,
  Z    = 7,
  IJK  = 8};

typedef enum coordSysType {
  MachineCsy          =  9,
  MoveableMachineCsy  = 10,
  MultipleArmCsy      = 11,
  PartCsy             = 12,
  JogDisplayCsy       = 13,
  JogMoveCsy          = 14,
  SensorCsy           = 15};

typedef enum changeToolActionType {
  MoveAuto      = 16,
  MoveMan       = 17,
  Rotate        = 18,
  Switch        = 19};


typedef enum otherKeyType {
  EmptyKey          =  0,
  Accel             = 20,
  Actual            = 21,
  Alignment         = 22,
  Approach          = 23,
  CenterPartKey     = 24, /* for responses only */
  ChangeToolKey     = 25, /* for responses only */
  ChangeToolAction  = 26, /* for responses only */
  CollisionVolume   = 27,
  CoordSystem       = 28, /* for responses only */
  Default           = 29,
  Dis               = 30, /* for commands only  */
  DMEVersion        = 31, /* for responses only */
  ER                = 32,
  ErrorKey          = 33, /* for processing only */
  ErrStatus         = 34, /* for responses only */
  FoundTool         = 35,
  GetCsyTr          = 36, /* for responses only (= GetCsyTransformationKey) */
  GetMachineKey     = 37, /* for responses only */
  GoToKey           = 38, /* for responses only */
  GoToPar           = 39,
  Id                = 40,
  IsHomedKey        = 41, /* for responses only */
  IsUserEnabledKey  = 42, /* for responses only */
  KeyPress          = 43, /* for responses only */
  Max               = 44,
  Min               = 45,
  Name              = 46,
  OBB               = 47, /* for responses only */
  Phi               = 48,
  PtMeasKey         = 49,
  PtMeasPar         = 50,
  Q                 = 51,
  Retract           = 52,
  Search            = 53,
  SetPropKey        = 54, /* for responses only */
  Speed             = 55,
  TagKey            = 56, /* this is needed but is not a keyword */
  TiltPartKey       = 57, /* for responses only */
  TiltCenterPartKey = 58, /* for responses only */
  Time              = 59, /* for commands only  */
  Tool              = 60};

/*

The ippKeyStrings array has indexes that equal the enumeration
constants given in targetPosType, coordSysType, changeToolActionType,
and otherKeyType. Each key string is the string for the corresponding
enumeration constant. These strings are used in generating command
strings and response strings.

*/

const char * const ippKeyStrings[]=
{
  /*  0 */  "",  // empty string
  /*  1 */  "A",
  /*  2 */  "B",
  /*  3 */  "C",
  /*  4 */  "R",
  /*  5 */  "X",
  /*  6 */  "Y",
  /*  7 */  "Z",
  /*  8 */  "IJK",
  /*  9 */  "MachineCsy",
  /* 10 */  "MoveableMachineCsy",
  /* 11 */  "MultipleArmCsy",
  /* 12 */  "PartCsy",
  /* 13 */  "JogDisplayCsy",
  /* 14 */  "JogMoveCsy",
  /* 15 */  "SensorCsy",
  /* 16 */  "MoveAuto",
  /* 17 */  "MoveMan",
  /* 18 */  "Rotate",
  /* 19 */  "Switch",
  /* 20 */  "Accel",
  /* 21 */  "Act",
  /* 22 */  "Alignment",
  /* 23 */  "Approach",
  /* 24 */  "CenterPart",
  /* 25 */  "ChangeTool",
  /* 26 */  "ChangeToolAction",
  /* 27 */  "CollisionVolume",
  /* 28 */  "CoordSystem",
  /* 29 */  "Def",
  /* 30 */  "Dis",
  /* 31 */  "DMEVersion",
  /* 32 */  "ER",
  /* 33 */  "Error",
  /* 34 */  "ErrStatus",
  /* 35 */  "FoundTool",
  /* 36 */  "GetCsyTransformation",
  /* 37 */  "GetMachineClass",
  /* 38 */  "GoTo",
  /* 39 */  "GoToPar",
  /* 40 */  "Id",
  /* 41 */  "IsHomed",
  /* 42 */  "IsUserEnabled",
  /* 43 */  "KeyPress",
  /* 44 */  "Max",
  /* 45 */  "Min",
  /* 46 */  "Name",
  /* 47 */  "OBB",
  /* 48 */  "Phi",
  /* 49 */  "PtMeas",
  /* 50 */  "PtMeasPar",
  /* 51 */  "Q",
  /* 52 */  "Retract",
  /* 53 */  "Search",
  /* 54 */  "SetProp",
  /* 55 */  "Speed",
  /* 56 */  "TagKey", /* never used */
  /* 57 */  "TiltPart",
  /* 58 */  "TiltCenterPart",
  /* 59 */  "Time",
  /* 60 */  "Tool"
};

const char * const ippCommandNameStrings[]=
{
  /*  0 */  "",  // empty string
  /*  1 */  "Bug",
  /*  2 */  "AbortE",
  /*  3 */  "AlignPart",
  /*  4 */  "AlignTool",
  /*  5 */  "CenterPart",
  /*  6 */  "ChangeTool",
  /*  7 */  "ClearAllErrors",
  /*  8 */  "DisableUser",
  /*  9 */  "EnableUser",
  /* 10 */  "EndSession",
  /* 11 */  "EnumAllProp",
  /* 12 */  "EnumProp",
  /* 13 */  "EnumTools",
  /* 14 */  "FindTool",
  /* 15 */  "Get",
  /* 16 */  "GetChangeToolAction",
  /* 17 */  "GetCoordSystem",
  /* 18 */  "GetCsyTransformation",
  /* 19 */  "GetDMEVersion",
  /* 20 */  "GetErrorInfo",
  /* 21 */  "GetErrStatusE",
  /* 22 */  "GetMachineClass",
  /* 23 */  "GetProp",
  /* 24 */  "GetPropE",
  /* 25 */  "GetXtdErrStatus",
  /* 26 */  "GoTo",
  /* 27 */  "Home",
  /* 28 */  "IsHomed",
  /* 29 */  "IsUserEnabled",
  /* 30 */  "LockAxis",
  /* 31 */  "OnMoveReportE",
  /* 32 */  "OnPtMeasReport",
  /* 33 */  "OnScanReport",
  /* 34 */  "PtMeas",
  /* 35 */  "ReQualify",
  /* 36 */  "ScanInCylEndIsPlane",
  /* 37 */  "ScanInCylEndIsSphere",
  /* 38 */  "ScanInPlaneEndIsCyl",
  /* 39 */  "ScanInPlaneEndIsPlane",
  /* 40 */  "ScanInPlaneEndIsSphere",
  /* 41 */  "ScanOnCircle",
  /* 42 */  "ScanOnCircleHint",
  /* 43 */  "ScanOnLine",
  /* 44 */  "ScanOnLineHint",
  /* 45 */  "ScanUnknownHint",
  /* 46 */  "SetCoordSystem",
  /* 47 */  "SetCsyTransformation",
  /* 48 */  "SetProp",
  /* 49 */  "SetTool",
  /* 50 */  "StartSession",
  /* 51 */  "StopAllDaemons",
  /* 52 */  "StopDaemon",
  /* 53 */  "TiltCenterPart",
  /* 54 */  "TiltPart"
};

/* The ippResponseNameStrings are used only to report errors. These are
class names. They correspond to responseNameTypes. */

const char * const ippResponseNameStrings[]=
{
  /*  0 */  "",  // empty string
  /*  1 */  "AckResponse",
  /*  2 */  "CompleteResponse",
  /*  3 */  "AlignResponse",
  /*  4 */  "CenterPartResponse",
  /*  5 */  "ChangeToolEResponse",
  /*  6 */  "EnumPropResponse",
  /*  7 */  "ChangeToolActionResponse",
  /*  8 */  "GetCoordSystemResponse",
  /*  9 */  "GetCsyTransformationResponse",
  /* 10 */  "GetDMEVersionResponse",
  /* 11 */  "GetErrStatusEResponse",
  /* 12 */  "GetMachineClassResponse",
  /* 13 */  "GetPropResponse",
  /* 14 */  "GoToEResponse",
  /* 15 */  "IsHomedResponse",
  /* 16 */  "IsUserEnabledResponse",
  /* 17 */  "KeyPressEResponse",
  /* 18 */  "OnMoveReportEResponse",
  /* 19 */  "PtMeasOrGetResponse",
  /* 20 */  "PtMeasEResponse",
  /* 21 */  "ScanResponse",
  /* 22 */  "SetPropEResponse",
  /* 23 */  "StringResponse",
  /* 24 */  "TiltCenterPartResponse",
  /* 25 */  "TiltPartResponse",
  /* 26 */  "ErrorResponse"
};
 
using namespace ipp;

/* Box class ***********************************************************

A box has 15 attributes, all doubles. They are public here to avoid
writing and using 30 get and set methods.

It would be better if Kx, Ky, Kz were omitted, since they are redundant
information that needs checking, but they are in the spec.

*/

class Box
{  
public:
  double Cx; // center of box in current coordinate system
  double Cy;
  double Cz;
  double Ex; // extent of box in I direction
  double Ey; // extent of box in J direction
  double Ez; // extent of box in K direction
  double Ix; // I direction in current coordinate system
  double Iy;
  double Iz;
  double Jx; // J direction in current coordinate system
  double Jy;
  double Jz;
  double Kx; // K direction in current coordinate system
  double Ky;
  double Kz;
  Box(){}
  Box(double inCx, double inCy, double inCz,
      double inEx, double inEy, double inEz,
      double inIx, double inIy, double inIz,
      double inJx, double inJy, double inJz,
      double inKx, double inKy, double inKz)
    {
      Cx = inCx; Cy = inCy; Cz = inCz;
      Ex = inEx; Ey = inEy; Ez = inEz;
      Ix = inIx; Iy = inIy; Iz = inIz;
      Jx = inJx; Jy = inJy; Jz = inJz;
      Kx = inKx; Ky = inKy; Kz = inKz;
    }
  ~Box(){}
};

/* OneProperty class ***************************************************

This is the parent type for the OnePropertyAlignment,
OnePropertyCollisionVolume, OnePropertyId, OnePropertyName, and
OnePropertyNumber classes.

*/

class OneProperty
{  
public:
  OneProperty(){}
  ~OneProperty(){}
  virtual char * propString(char * buffer, int * k){ return buffer; }
  const char * getKeyString(int key)
    { return (((key < 0) || (key > 60)) ? "bug" : ippKeyStrings[key]); }
};

/* OnePropertyAlignment class ******************************************

Use this for data from GetProp(Tool.Alignment()) or
GetPropE(Tool.Alignment()). Alignment may not be queried for FoundTool.

*/

class OnePropertyAlignment : public OneProperty
{
private:
  double i1;
  double j1;
  double k1;
  double i2;
  double j2;
  double k2;
  bool   has2;
public:
  OnePropertyAlignment(){}
  OnePropertyAlignment(double inI1, double inJ1, double inK1, double inI2,
		       double inJ2, double inK2, bool inHas2)
    {
      i1 = inI1;
      j1 = inJ1;
      k1 = inK1;
      i2 = inI2;
      j2 = inJ2;
      k2 = inK2;
      has2 = inHas2;
    }
  ~OnePropertyAlignment(){}

  void setI1(double inI1){ i1 = inI1; }
  void setJ1(double inJ1){ j1 = inJ1; }
  void setK1(double inK1){ k1 = inK1; }
  void setI2(double inI2){ i2 = inI2; }
  void setJ2(double inJ2){ j2 = inJ2; }
  void setK2(double inK2){ k2 = inK2; }
  void setHas2(bool inHas2){ has2 = inHas2; }

  double getI1(){ return i1; }
  double getJ1(){ return j1; }
  double getK1(){ return k1; }
  double getI2(){ return i2; }
  double getJ2(){ return j2; }
  double getK2(){ return k2; }
  bool getHas2(){ return has2; }

  char * propString(char * buffer, int * k);
};

/* OnePropertyCollisionVolume class ************************************

Use this for data from GetProp(Tool.CollisionVolume()) or
GetPropE(Tool.CollisionVolume()). CollisionVolume may not be queried
for FoundTool.

Note that the destructor deletes boxes.

*/

class OnePropertyCollisionVolume : public OneProperty
{
private:
  int numberBoxes;
  Box * boxes;

public:
  OnePropertyCollisionVolume(){ boxes = NULL; }
  OnePropertyCollisionVolume(int inNumberBoxes, Box * inBoxes)
    {
      numberBoxes = inNumberBoxes;
      boxes = inBoxes;
    }
  ~OnePropertyCollisionVolume(){ delete boxes; }
  
  void setNumberBoxes(int inNumberBoxes){ numberBoxes = inNumberBoxes; }
  void setBoxes(Box * inBoxes){ boxes = inBoxes; };

  int getNumberBoxes(){ return numberBoxes; }
  Box * getBoxes(){ return boxes; }

  char * propString(char * buffer, int * k);
};

/* OnePropertyId class *************************************************

Use this for data from GetProp(Tool.Id()), GetPropE(Tool.Id()), 
GetProp(FoundTool.Id()), or GetPropE(FoundTool.Id()).

*/

class OnePropertyId : public OneProperty
{
private:
  bool isFound; // true=FoundTool, false=Tool
  char * idString;

public:
  OnePropertyId(){ idString = NULL; }
  OnePropertyId(bool inIsFound, char * inIdString)
    {
      isFound = inIsFound;
      idString = inIdString;
    }
  ~OnePropertyId(){ delete idString; }

  void setIsFound(bool inIsFound){ isFound = inIsFound; }
  void setIdString(char * inIdString){ idString = inIdString; }

  bool getIsFound(){ return isFound; }
  char * getIdString(){ return idString; }

  char * propString(char * buffer, int * k);
};

/* OnePropertyName class ************************************************

Use this for data from GetProp(Tool.Name()), GetPropE(Tool.Name()), 
GetProp(FoundTool.Name()), or GetPropE(FoundTool.Name()).

*/

class OnePropertyName : public OneProperty
{
private:
  bool isFound;      // true=FoundTool, false=Tool
  char * nameString;

public:
  OnePropertyName(){ nameString = NULL; }
  OnePropertyName(bool inIsFound, char * inNameString)
    {
      isFound = inIsFound;
      nameString = inNameString;
    }
  ~OnePropertyName(){ delete nameString; }

  void setIsFound(bool inIsFound){ isFound = inIsFound; }
  void setNameString(char * inNameString){ nameString = inNameString; }

  bool getIsFound(){ return isFound; }
  char * getNameString(){ return nameString; }

  char * propString(char * buffer, int * k);
};

/* OnePropertyNumber class ***********************************************

Use this for single number data from
GetProp (key1.key2.key3()),
GetPropE(key1.key2.key3()),
GetProp (key1.key2.key3.key4()), or
GetPropE(key1.key2.key3.key4()).

Two constructors are provided, one with three keys, and one with four.

*/

class OnePropertyNumber : public OneProperty
{
private:
  int numberKeys;
  otherKeyType key1;
  otherKeyType key2;
  otherKeyType key3;
  otherKeyType key4;
  double val;

public:
  OnePropertyNumber(){}
  OnePropertyNumber(otherKeyType inKey1, otherKeyType inKey2,
		    otherKeyType inKey3, otherKeyType inKey4, double inVal)
    {
      numberKeys = 4;
      key1 = inKey1;
      key2 = inKey2;
      key3 = inKey3;
      key4 = inKey4;
      val = inVal;
    }
  OnePropertyNumber(otherKeyType inKey1, otherKeyType inKey2,
		    otherKeyType inKey3, double inVal)
    {
      numberKeys = 3;
      key1 = inKey1;
      key2 = inKey2;
      key3 = inKey3;
      key4 = EmptyKey;
      val = inVal;
    }
  ~OnePropertyNumber(){}

  void setNumberKeys(int inNumberKeys){ numberKeys = inNumberKeys; }
  void setKey1(otherKeyType inKey1){ key1 = inKey1; }
  void setKey2(otherKeyType inKey2){ key2 = inKey2; }
  void setKey3(otherKeyType inKey3){ key3 = inKey3; }
  void setKey4(otherKeyType inKey4){ key4 = inKey4; }
  void setVal(double inVal){ val = inVal; }

  int getNumberKeys(){return numberKeys;}
  otherKeyType getKey1(){ return key1; }
  otherKeyType getKey2(){ return key2; }
  otherKeyType getKey3(){ return key3; }
  otherKeyType getKey4(){ return key4; }
  double getVal(){ return val; }

  char * propString(char * buffer, int * k);
};

/* Tag class **********************************************************/

class Tag
{  
private:
  unsigned int    tagNumber;
  tagIdType       tagType;
  
public:
  Tag(){}
  Tag(unsigned int inTagNumber, commandNameType id)
    {
      tagNumber = inTagNumber;
      setTagType(id);
    }
  Tag(unsigned int inTagNumber, tagIdType inTagType)
    {
      tagNumber = inTagNumber;
      tagType = inTagType;
    }
  ~Tag(){}

  void setTagNumber(unsigned int inTagNumber){ tagNumber = inTagNumber; }
  void setTagType(tagIdType inTagType){ tagType = inTagType; }
  void setTagType(commandNameType commandName);

  unsigned int getTagNumber() { return tagNumber; }
  tagIdType getTagType() { return tagType; }
  char * getTagString(char * buffer);
};

/* TargetVector class **************************************************

The vector (0,0,0) is allowed.

*/

class TargetVector {
  
private:
  double value[3];
  bool isSet; 
  
public:
  TargetVector()
    {
      value[0] = 0.0;
      value[1] = 0.0;
      value[2] = 0.0;
      isSet = false;
    }
  TargetVector(double i, double j, double k)
    {
      value[0] = i;
      value[1] = j;
      value[2] = k;
      isSet = true;
    }
  TargetVector(double ijk[3])
    {
      value[0] = ijk[0];
      value[1] = ijk[1];
      value[2] = ijk[2];
      isSet = true;
    }
  ~TargetVector(){}
  
  void setValues(double i, double j, double k)
    {
      value[0] = i;
      value[1] = j;
      value[2] = k; 
      isSet = true;
    }
  void setValues(double ijk[3])
    {
      value[0] = ijk[0];
      value[1] = ijk[1];
      value[2] = ijk[2];
      isSet = true;
    }
  double getI(){ return value[0]; }
  double getJ(){ return value[1]; }
  double getK(){ return value[2]; }
  double length()
    { return sqrt((value[0] * value[0]) +
		  (value[1] * value[1]) +
		  (value[2] * value[2]));
    }
  bool hasData(){ return isSet; }
  char * getVectorString(char * buffer, int n);
};

/* Error Classes ******************************************************/

/*

The severities in the subclasses of IppError are as in version 1.40.

The naming convention is used that the name ErrorNNNN is given to the
error whose error number is NNNN.

*/
   
/* IppError Class *****************************************************/

/*

This is the immediate parent class of all other error classes. It is
not intended to be instantiated. Virtual functions are used.

The ErrorResponse constructor uses an IppError if the error code is
not one of the defined codes. The severity of an IppError is set to
4, which not one of the defined severities. This allows an illegal
error code to be detected without having to check it against all of
the defined codes.

*/

class IppError {
public:
  virtual errorNameType getName(){ return BufferFull; }
  virtual char getSeverity(){ return '4'; }
  virtual const char * getText(){ return "Generic Error"; }
};

/* Error0000 class ****************************************************/

class Error0000 : public IppError {
public:
  errorNameType getName(){ return BufferFull; }
  char getSeverity(){ return '0'; }
  const char * getText(){ return "Buffer full"; }
};

/* Error0001 class ****************************************************/

class Error0001 : public IppError {
public:
  errorNameType getName(){ return IllegalTag; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Illegal tag"; }
};

/* Error0002 class ****************************************************/

class Error0002 : public IppError {
public:
  errorNameType getName(){ return NoSpaceAtPos6; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "No space at pos. 6"; }
};

/* Error0003 class ****************************************************/

class Error0003 : public IppError {
public:
  errorNameType getName(){ return Reserved3; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Reserved"; }
};

/* Error0004 class ****************************************************/

class Error0004 : public IppError {
public:
  errorNameType getName(){ return Reserved4; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Reserved"; }
};

/* Error0005 class ****************************************************/

class Error0005 : public IppError {
public:
  errorNameType getName(){ return Reserved5; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Reserved"; }
};

/* Error0006 class ****************************************************/

class Error0006 : public IppError {
public:
  errorNameType getName(){ return TransactionAborted; }
  char getSeverity(){ return '2'; }
  const char * getText()
    { return "Transaction aborted (Use ClearAllErrors to Continue)"; }
};

/* Error0007 class ****************************************************/

class Error0007 : public IppError {
public:
  errorNameType getName(){ return IllegalCharacter; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Illegal character"; }
};

/* Error0008 class ****************************************************/

class Error0008 : public IppError {
public:
  errorNameType getName(){ return ProtocolError; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Protocol error"; }
};

/* Error0500 class ****************************************************/

class Error0500 : public IppError {
public:
  errorNameType getName(){ return EmergencyStop; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Emergency stop"; }
};

/* Error0501 class ****************************************************/

class Error0501 : public IppError {
public:
  errorNameType getName(){ return UnsupportedCommand; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Unsupported command"; }
};

/* Error0502 class ****************************************************/

class Error0502 : public IppError {
public:
  errorNameType getName(){ return IncorrectArguments; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Incorrect arguments"; }
};


/* Error0503 class ****************************************************/

class Error0503 : public IppError {
public:
  errorNameType getName(){ return ControllerCommFailure; }
  char getSeverity(){ return '9'; }
  const char * getText(){ return "Controller communications failure"; }
};

/* Error0504 class ****************************************************/

class Error0504 : public IppError {
public:
  errorNameType getName(){ return ArgumentOutOfRange; }
  char getSeverity(){ return '1'; }
  const char * getText(){ return "Argument out of range"; }
};

/* Error0505 class ****************************************************/

class Error0505 : public IppError {
public:
  errorNameType getName(){ return ArgumentNotRecognized; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Argument not recognized"; }
};

/* Error0506 class ****************************************************/

class Error0506 : public IppError {
public:
  errorNameType getName(){ return ArgumentNotSupported; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Argument not supported"; }
};

/* Error0507 class ****************************************************/

class Error0507 : public IppError {
public:
  errorNameType getName(){ return IllegalCommand; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Illegal command"; }
};

/* Error0508 class ****************************************************/

class Error0508 : public IppError {
public:
  errorNameType getName(){ return BadContext; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Bad context"; }
};

/* Error0509 class ****************************************************/

class Error0509 : public IppError {
public:
  errorNameType getName(){ return BadArgument; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Bad argument"; }
};

/* Error0510 class ****************************************************/

class Error0510 : public IppError {
public:
  errorNameType getName(){ return BadProperty; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Bad property"; }
};

/* Error0511 class ****************************************************/

class Error0511 : public IppError {
public:
  errorNameType getName(){ return ErrorProcessingMethod; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Error processing method"; }
};

/* Error0512 class ****************************************************/

class Error0512 : public IppError {
public:
  errorNameType getName(){ return NoDaemonsAreActive; }
  char getSeverity(){ return '1'; }
  const char * getText(){ return "No daemons are active"; }
};

/* Error0513 class ****************************************************/

class Error0513 : public IppError {
public:
  errorNameType getName(){ return DaemonDoesNotExist; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Daemon does not exist"; }
};

/* Error0514 class ****************************************************/

class Error0514 : public IppError {
public:
  errorNameType getName(){ return UseClearAllErrorsToCont; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Use ClearAllErrors to continue"; }
};

/* Error0515 class ****************************************************/

class Error0515 : public IppError {
public:
  errorNameType getName(){ return DaemonAlreadyExists; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Daemon already exists"; }
};

/* Error1000 class ****************************************************/

class Error1000 : public IppError {
public:
  errorNameType getName(){ return MachineInErrorState; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Machine in error state"; }
};

/* Error1001 class ****************************************************/

class Error1001 : public IppError {
public:
  errorNameType getName(){ return IllegalTouch; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Illegal touch"; }
};

/* Error1002 class ****************************************************/

class Error1002 : public IppError {
public:
  errorNameType getName(){ return AxisDoesNotExist; }
  char getSeverity(){ return '9'; }
  const char * getText(){ return "Axis does not exist"; }
};

/* Error1003 class ****************************************************/

class Error1003 : public IppError {
public:
  errorNameType getName(){ return NoTouch; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "No touch"; }
};

/* Error1004 class ****************************************************/

class Error1004 : public IppError {
public:
  errorNameType getName(){ return NumAnglNotSupOnCurrDev; }
  char getSeverity(){ return '9'; }
  const char * getText()
    { return "Number of angles not supported on current device"; }
};

/* Error1005 class ****************************************************/

class Error1005 : public IppError {
public:
  errorNameType getName(){ return ErrorDuringHome; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Error during home"; }
};

/* Error1006 class ****************************************************/

class Error1006 : public IppError {
public:
  errorNameType getName(){ return SurfaceNotFound; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Surface not found"; }
};

/* Error1007 class ****************************************************/

class Error1007 : public IppError {
public:
  errorNameType getName(){ return ThetaOutOfRange; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Theta out of range"; }
};

/* Error1008 class ****************************************************/

class Error1008 : public IppError {
public:
  errorNameType getName(){ return TargetPosOutOfMachVol; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Target position out of machine volume"; }
};

/* Error1009 class ****************************************************/

class Error1009 : public IppError {
public:
  errorNameType getName(){ return AirPressureOutOfRange; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Air pressure out of range"; }
};

/* Error1010 class ****************************************************/

class Error1010 : public IppError {
public:
  errorNameType getName(){ return VectorHasNoNorm; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Vector has no norm"; }
};

/* Error1500 class ****************************************************/

class Error1500 : public IppError {
public:
  errorNameType getName(){ return FailedToReseatHead; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Failed to re-seat head"; }
};

/* Error1501 class ****************************************************/

class Error1501 : public IppError {
public:
  errorNameType getName(){ return ProbeNotArmed; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Probe not armed"; }
};

/* Error1502 class ****************************************************/

class Error1502 : public IppError {
public:
  errorNameType getName(){ return ToolNotFound; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Tool not found"; }
};

/* Error1503 class ****************************************************/

class Error1503 : public IppError {
public:
  errorNameType getName(){ return ToolNotDefined; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Tool not defined"; }
};

/* Error2000 class ****************************************************/

class Error2000 : public IppError {
public:
  errorNameType getName(){ return ToolNotCalibrated; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Tool not calibrated"; }
};

/* Error2001 class ****************************************************/

class Error2001 : public IppError {
public:
  errorNameType getName(){ return HeadErrorExcessiveForce; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Head error excessive force"; }
};

/* Error2002 class ****************************************************/

class Error2002 : public IppError {
public:
  errorNameType getName(){ return TypeProbeDoesNotAllowOper; }
  char getSeverity(){ return '3'; }
  const char * getText()
    { return "Type of probe does not allow this operation"; }
};

/* Error2500 class ****************************************************/

class Error2500 : public IppError {
public:
  errorNameType getName(){ return MachineLimitEncountered; }
  char getSeverity(){ return '3'; }
  const char * getText()
    { return "Machine limit encountered [Move Out Of Limits]"; }
};

/* Error2501 class ****************************************************/

class Error2501 : public IppError {
public:
  errorNameType getName(){ return AxisNotActive; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Axis not active"; }
};

/* Error2502 class ****************************************************/

class Error2502 : public IppError {
public:
  errorNameType getName(){ return AxisPositionError; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Axis position error"; }
};

/* Error2503 class ****************************************************/

class Error2503 : public IppError {
public:
  errorNameType getName(){ return ScaleReadHeadFailure; }
  char getSeverity(){ return '9'; }
  const char * getText(){ return "Scale read head failure"; }
};

/* Error2504 class ****************************************************/

class Error2504 : public IppError {
public:
  errorNameType getName(){ return Collision; }
  char getSeverity(){ return '3'; }
  const char * getText(){ return "Collision"; }
};

/* Error2505 class ****************************************************/

class Error2505 : public IppError {
public:
  errorNameType getName(){ return SpecifiedAngleOutOfRange; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Specified angle out of range"; }
};

/* Error2506 class ****************************************************/

class Error2506 : public IppError {
public:
  errorNameType getName(){ return PartNotAligned; }
  char getSeverity(){ return '2'; }
  const char * getText(){ return "Part not aligned"; }
};

/* Command classes ****************************************************/

/* Command class *******************************************************

 Base class Command is used to construct all commands 
 that have no arguments:

   AbortE()
   ClearAllErrors()
   DisableUser()
   EnableUser()
   EndSession()
   EnumTools()
   GetCoordSystem()
   GetDMEVersion()
   GetErrStatusE()
   GetMachineClass()
   GetXtdErrStatus()
   Home()
   IsHomed()
   IsUserEnabled()
   ReQualify()
   StartSession()
   StopAllDaemons()

*/

class Command {
  
private:
  bool newCommand;
  commandNameType commandName;
  
protected:
  Tag * tag;
  char * printLikeGet(char * buffer, bool R, bool X, bool Y, bool Z,
		      bool A, bool B, bool C);
  
public:
  Command(){}
  Command(unsigned int inTagNumber, commandNameType inCommandName)
    {
      commandName = inCommandName;
      tag = new Tag(inTagNumber, inCommandName);
    }
  Command(unsigned int inTagNumber, tagIdType inTagType,
	  commandNameType inCommandName)
    {
      commandName = inCommandName;      
      tag = new Tag(inTagNumber, inTagType);
    }
  ~Command(){ delete tag; }
  
  void setCommandName(commandNameType inCommandName)
    { commandName = inCommandName; }

  commandNameType getCommandName(){ return commandName; }
  Tag * getTag(){ return tag; }
  const char * getCommandNameString(int key)
    { return (((key < 0) || (key > 54)) ? "bug" : ippCommandNameStrings[key]);}
  const char * getKeyString(int key)
    { return (((key < 0) || (key > 60)) ? "bug" : ippKeyStrings[key]); }
  char * getCommandString(char * buffer, int bufferSize);
};

/* AlignPartCommand class **********************************************

Only one constructor taking arguments is provided. If there is only
one part vector and one machine vector, inHas2 should be false
and the extra arguments can all be zero (or any other number, since
their value is irrelevant).

*/

class AlignPartCommand : public Command
{
private:
  bool has2;
  TargetVector partVector1;
  TargetVector machineVector1;
  TargetVector partVector2;
  TargetVector machineVector2;
  double alpha;
  double beta;

public:
  AlignPartCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, AlignPart){}
  AlignPartCommand(unsigned int tgNum, bool inHas2,
		   double inPv1i, double inPv1j, double inPv1k,
		   double inMv1i, double inMv1j, double inMv1k,
		   double inPv2i, double inPv2j, double inPv2k,
		   double inMv2i, double inMv2j, double inMv2k,
		   double inAlpha, double inBeta):
    Command(tgNum, CommandTag, AlignPart)
    {
      has2 = inHas2;
      partVector1.setValues(inPv1i, inPv1j, inPv1k);
      machineVector1.setValues(inMv1i, inMv1j, inMv1k);
      partVector2.setValues(inPv2i, inPv2j, inPv2k);
      machineVector2.setValues(inMv2i, inMv2j, inMv2k);
      alpha = inAlpha;
      beta = inBeta;
    }
  ~AlignPartCommand(){}

  bool setHas2(bool inHas2){ has2 = inHas2; return has2; }
  void setPartVector1(double inI, double inJ, double inK)
    { partVector1.setValues(inI, inJ, inK); }
  void setMachineVector1(double inI, double inJ, double inK)
    { machineVector1.setValues(inI, inJ, inK); }
  void setPartVector2(double inI, double inJ, double inK)
    { partVector2.setValues(inI, inJ, inK); }
  void setMachineVector2(double inI, double inJ, double inK)
    { machineVector2.setValues(inI, inJ, inK); }
  double setAlpha(double inAlpha){ alpha = inAlpha; return alpha; }
  double setBeta(double inBeta){ beta = inBeta; return beta; }

  bool getHas2(){ return has2; }
  double getPartVector1i(){ return partVector1.getI(); }
  double getPartVector1j(){ return partVector1.getJ(); }
  double getPartVector1k(){ return partVector1.getK(); }
  double getMachineVector1i(){ return machineVector1.getI(); }
  double getMachineVector1j(){ return machineVector1.getJ(); }
  double getMachineVector1k(){ return machineVector1.getK(); }
  double getPartVector2i(){ return partVector2.getI(); }
  double getPartVector2j(){ return partVector2.getJ(); }
  double getPartVector2k(){ return partVector2.getK(); }
  double getMachineVector2i(){ return machineVector2.getI(); }
  double getMachineVector2j(){ return machineVector2.getJ(); }
  double getMachineVector2k(){ return machineVector2.getK(); }
  double getAlpha(){ return alpha; }
  double getBeta(){ return beta; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* AlignToolCommand class **********************************************

Only one constructor taking arguments is provided. If there is only
one vector, inHas2 should be false and the extra arguments can all be
zero (or any other number, since their value is irrelevant).

*/

class AlignToolCommand : public Command
{
private:
  bool has2;
  TargetVector vector1;
  TargetVector vector2;
  double alpha;
  double beta;
  
public:
  AlignToolCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, AlignTool){}
  AlignToolCommand(unsigned int tgNum, bool inHas2,
		   double i1, double j1, double k1,
		   double i2, double j2, double k2,
		   double inAlpha, double inBeta):
    Command(tgNum, CommandTag, AlignTool)
    {
      has2 = inHas2;
      vector1.setValues(i1, j1, k1);

      vector2.setValues(i2, j2, k2);
      alpha = inAlpha;
      beta = inBeta;
    }
  ~AlignToolCommand(){}
  
  void setHas2(bool inHas2){ has2 =  inHas2; }
  void setVector1(double i, double j, double k){ vector1.setValues(i, j, k); }
  void setVector2(double i, double j, double k){ vector2.setValues(i, j, k); }
  void setAlpha(double inAlpha){ alpha = inAlpha; }
  void setBeta(double inBeta){ beta = inBeta; }

  bool getHas2(){ return has2; }
  double getVector1i(){ return vector1.getI(); }
  double getVector1j(){ return vector1.getJ(); }
  double getVector1k(){ return vector1.getK(); }
  double getVector2i(){ return vector2.getI(); }
  double getVector2j(){ return vector2.getJ(); }
  double getVector2k(){ return vector2.getK(); }
  double getAlpha(){ return alpha; }		
  double getBeta(){ return beta; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* CenterPartCommand class ********************************************/

class CenterPartCommand : public Command
{
private:
  double px;
  double py;
  double pz;
  double limit;

public:
  CenterPartCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, CenterPart){}
  CenterPartCommand(unsigned int tgNum,
		    double inPx, double inPy, double inPz, double inLimit):
    Command(tgNum, CommandTag, CenterPart)
    {
      px = inPx;
      py = inPy;
      pz = inPz;
      limit = inLimit;
    }
  ~CenterPartCommand(){}

  void setPx(double inPx){ px = inPx; }
  void setPy(double inPy){ py = inPy; }
  void setPz(double inPz){ pz = inPz; }
  void setLimit(double inLimit){ limit = inLimit; }

  double getPx(){ return px; }
  double getPy(){ return py; }
  double getPz(){ return pz; }
  double getLimit(){ return limit; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ChangeToolCommand class ********************************************/

class ChangeToolCommand : public Command
{
private:
  char * toolName;
  
public:
  ChangeToolCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ChangeTool){ toolName = NULL; }
  ChangeToolCommand(unsigned int tgNum, char * inToolName):
    Command(tgNum, CommandTag, ChangeTool){ toolName = inToolName; }
  ~ChangeToolCommand(){}
  
  void setToolName(char * inToolName){ toolName = inToolName; }

  char * getToolName(){ return toolName; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* EnumAllPropCommand class ********************************************

This allows up to four components in the argument. For example,
EnumAllProp(Tool.PtMeasPar.Speed.Def())

*/

class EnumAllPropCommand : public Command
{
private:
  int numberKeys;
  otherKeyType key1;
  otherKeyType key2;
  otherKeyType key3;
  otherKeyType key4;
  
public:
  EnumAllPropCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, EnumAllProp){}
  EnumAllPropCommand(unsigned int tgNum, int inNumberKeys,
		     otherKeyType inKey1, otherKeyType inKey2,
		     otherKeyType inKey3, otherKeyType inKey4):
    Command(tgNum, CommandTag, EnumAllProp)
    {
      numberKeys = inNumberKeys;
      key1 = inKey1;
      key2 = inKey2;
      key3 = inKey3;
      key4 = inKey4;
    }
  ~EnumAllPropCommand(){}

  void setNumberKeys(int inNumberKeys){ numberKeys = inNumberKeys; }
  void setKey1(otherKeyType inKey1){ key1 = inKey1; }
  void setKey2(otherKeyType inKey2){ key2 = inKey2; }
  void setKey3(otherKeyType inKey3){ key3 = inKey3; }
  void setKey4(otherKeyType inKey4){ key4 = inKey4; }
  
  int getNumberKeys(){return numberKeys;}
  otherKeyType getKey1(){ return key1; }
  otherKeyType getKey2(){ return key2; }
  otherKeyType getKey3(){ return key3; }
  otherKeyType getKey4(){ return key4; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* EnumPropCommand class ***********************************************

This allows up to four components in the argument. For example,
EnumProp(Tool.PtMeasPar.Speed.Def())

*/

class EnumPropCommand : public Command
{
private:
  int numberKeys;
  otherKeyType key1;
  otherKeyType key2;
  otherKeyType key3;
  otherKeyType key4;
  
public:
  EnumPropCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, EnumProp){}
  EnumPropCommand(unsigned int tgNum, int inNumberKeys,
		  otherKeyType inKey1, otherKeyType inKey2,
		  otherKeyType inKey3, otherKeyType inKey4):
    Command(tgNum, CommandTag, EnumProp)
    {
      numberKeys = inNumberKeys;
      key1 = inKey1;
      key2 = inKey2;
      key3 = inKey3;
      key4 = inKey4;
    }
  ~EnumPropCommand(){}
  
  void setNumberKeys(int inNumberKeys){ numberKeys = inNumberKeys; }
  void setKey1(otherKeyType inKey1){ key1 = inKey1; }
  void setKey2(otherKeyType inKey2){ key2 = inKey2; }
  void setKey3(otherKeyType inKey3){ key3 = inKey3; }
  void setKey4(otherKeyType inKey4){ key4 = inKey4; }
  
  int getNumberKeys(){return numberKeys;}
  otherKeyType getKey1(){ return key1; }
  otherKeyType getKey2(){ return key2; }
  otherKeyType getKey3(){ return key3; }
  otherKeyType getKey4(){ return key4; }

  char * getCommandStringT(char * buffer, char * tagString);
};

/* FindToolCommand class **********************************************/

class FindToolCommand : public Command
{
private:
  char * toolName;
  
public:
  FindToolCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, FindTool){ toolName = NULL; }
  FindToolCommand(unsigned int tgNum, char * inToolName):
    Command(tgNum, CommandTag, FindTool){ toolName = inToolName; }
  ~FindToolCommand(){}
  
  void setToolName(char * aToolName){ toolName = aToolName; }

  char * getToolName(){ return toolName; }

  char * getCommandStringT(char * buffer, char * tagString);
};

/* GetCommand class ***************************************************/

class GetCommand : public Command
{
private:
  bool hasR;
  bool hasX;
  bool hasY;
  bool hasZ;
  bool hasToolA;
  bool hasToolB;
  bool hasToolC;
  
public:
  GetCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, Get){}
  GetCommand(unsigned int tgNum, bool inHasR, bool inHasX, bool inHasY,
	     bool inHasZ, bool inHasToolA, bool inHasToolB, bool inHasToolC):
    Command(tgNum, CommandTag, Get)
    {
      hasR = inHasR;
      hasX = inHasX;
      hasY = inHasY;
      hasZ = inHasZ;
      hasToolA = inHasToolA;
      hasToolB = inHasToolB;
      hasToolC = inHasToolC;
    }
  ~GetCommand(){}

  void setHasR(bool inHasR){ hasR = inHasR; }
  void setHasX(bool inHasX){ hasX = inHasX; }
  void setHasY(bool inHasY){ hasY = inHasY; }
  void setHasZ(bool inHasZ){ hasZ = inHasZ; }
  void setHasToolA(bool inHasToolA){ hasToolA = inHasToolA; }
  void setHasToolB(bool inHasToolB){ hasToolB = inHasToolB; }
  void setHasToolC(bool inHasToolC){ hasToolC = inHasToolC; }
  
  bool getHasR(){ return hasR; }
  bool getHasX(){ return hasX; }
  bool getHasY(){ return hasY; }
  bool getHasZ(){ return hasZ; }
  bool getHasToolA(){ return hasToolA; }
  bool getHasToolB(){ return hasToolB; }
  bool getHasToolC(){ return hasToolC; }

  char * getCommandStringT(char * buffer, char * tagString);
};

/* GetChangeToolActionCommand class ***********************************/

class GetChangeToolActionCommand : public Command
{
private:
  char * toolName;
  
public:
  GetChangeToolActionCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, GetChangeToolAction){ toolName = NULL; }
  GetChangeToolActionCommand(unsigned int tgNum, char * inToolName):
    Command(tgNum, CommandTag, GetChangeToolAction){ toolName = inToolName; }
  ~GetChangeToolActionCommand(){}
  
  void setToolName(char * inToolName){ toolName = inToolName; }

  char * getToolName(){ return toolName; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* GetCsyTransformationCommand class **********************************/

class GetCsyTransformationCommand : public Command
{
private:
  coordSysType   coordSys;
  
public:
  GetCsyTransformationCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, GetCsyTransformation){}
  GetCsyTransformationCommand(unsigned int tgNum, coordSysType inCoordSys):
    Command(tgNum, CommandTag, GetCsyTransformation){ coordSys = inCoordSys; }
  ~GetCsyTransformationCommand(){}
  
  void setCoordSys(coordSysType inCoordSys){ coordSys = inCoordSys; }

  coordSysType getCoordSys(){ return coordSys; }

  char * getCommandStringT(char * buffer, char * tagString);
};

/* GetErrorInfoCommand class *******************************************

The errorNumber identifies theError, so the constructor takes an
inErrorNumber argument but not an inTheError argument. Also, there is
only one "set" method, setTheErrorAndErrorNumber, which takes
inErrorNumber as its argument.

If inErrorNumber is not the number of a defined error, the constructor
and the setTheErrorAndErrorNumber method set the error to a generic
IppError and set the errorNumber to 10000.

*/

class GetErrorInfoCommand : public Command
{
private:
  unsigned int errorNumber;
  IppError * theError;
  
public:
  GetErrorInfoCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ipp::GetErrorInfo) {}
  GetErrorInfoCommand(unsigned int tgNum, unsigned int inErrorNumber):
    Command(tgNum, CommandTag, ipp::GetErrorInfo)
    {
      setTheErrorAndErrorNumber(inErrorNumber);
    }
  ~GetErrorInfoCommand(){}
  
  void setTheErrorAndErrorNumber(unsigned int inErrorNumber);
  
  unsigned int getErrorNumber() { return errorNumber; }
  IppError * getTheError() { return theError; }

  char * getCommandStringT(char * buffer, char * tagString);
};

/* GetPropCommand class ************************************************

A GetProp property must have three or four keys. Examples:
GetProp(Tool.GoToPars.Speed())
GetProp(Tool.GoToPars.Speed.Act())

If property n has only three keys, keys4[n] should be set to EmptyKey.

In the second constructor, this is assuming:
1. inNumberProps is positive.
2. the incoming arrays of keys (inKeys1, etc.) all have at least
   inNumberProps entries.

The setNumberProps method does not increase the size of the keys arrays
so setNumberProps should not be used to increase numberProps. It is
unlikely that a situation will arise where it is useful to call
setNumberProps.

To make a usable command, either use the second constructor, or call
setNewProps and then make repeated calls to setKey1, setKey2, etc.

The setXXX functions all return 1 if everything is OK and 0 if the
the index n is out of range.

*/

class GetPropCommand : public Command
{
private:
  int numberProps;
  otherKeyType * keys1;
  otherKeyType * keys2;
  otherKeyType * keys3;
  otherKeyType * keys4;
  
public:
  GetPropCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ipp::GetProp)
    {
      numberProps = 0;
      keys1 = 0;
      keys2 = 0;
      keys3 = 0;
      keys4 = 0;
    }
  GetPropCommand(unsigned int tgNum, int inNumberProps,
		 otherKeyType * inKeys1, otherKeyType * inKeys2,
		 otherKeyType * inKeys3, otherKeyType * inKeys4):
    Command(tgNum, CommandTag, ipp::GetProp)
    {
      int n;
      keys1 = new otherKeyType[inNumberProps];
      keys2 = new otherKeyType[inNumberProps];
      keys3 = new otherKeyType[inNumberProps];
      keys4 = new otherKeyType[inNumberProps];
      for (n = 0; n < inNumberProps; n++)
	{
	  keys1[n] = inKeys1[n];
	  keys2[n] = inKeys2[n];
	  keys3[n] = inKeys3[n];
	  keys4[n] = inKeys4[n];
	}
      numberProps = inNumberProps;
    }
  ~GetPropCommand()
    {
      delete keys1;
      delete keys2;
      delete keys3;
      delete keys4;
    }

  int setNewProps(int inNumberProps)
    {
      if (inNumberProps > 0)
	{
	  delete keys1; // OK if NULL
	  delete keys2;
	  delete keys3;
	  delete keys4;
	  numberProps = inNumberProps;
	  keys1 = new otherKeyType[inNumberProps];
	  keys2 = new otherKeyType[inNumberProps];
	  keys3 = new otherKeyType[inNumberProps];
	  keys4 = new otherKeyType[inNumberProps];
	}
      return (inNumberProps > 0);
    }
  int setNumberProps(int inNumberProps)
    {
      if (inNumberProps > 0)
	numberProps = inNumberProps;
      return (inNumberProps > 0);
    }
  int setKey1(otherKeyType inKey1, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys1[n] = inKey1;
      return ((n < numberProps) && (n > -1));
    }
  int setKey2(otherKeyType inKey2, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys2[n] = inKey2;
      return ((n < numberProps) && (n > -1));
    }
  int setKey3(otherKeyType inKey3, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys3[n] = inKey3;
      return ((n < numberProps) && (n > -1));
    }
  int setKey4(otherKeyType inKey4, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys4[n] = inKey4;
      return ((n < numberProps) && (n > -1));
    }
  
  int getNumberProps() { return numberProps; }
  otherKeyType getKey1(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys1[n] : ErrorKey); }
  otherKeyType getKey2(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys2[n] : ErrorKey); }
  otherKeyType getKey3(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys3[n] : ErrorKey); }
  otherKeyType getKey4(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys4[n] : ErrorKey); }

  char * getCommandStringT(char * buffer, char * tagString);
};

/* GetPropECommand class ************************************************

This is identical to the GetPropCommand except for the E in the name
and the use of EventTag rather than CommandTag.

*/

class GetPropECommand : public Command
{
private:
  int numberProps;
  otherKeyType * keys1;
  otherKeyType * keys2;
  otherKeyType * keys3;
  otherKeyType * keys4;
  
public:
  GetPropECommand(unsigned int tgNum):
    Command(tgNum, EventTag, ipp::GetPropE)
    {
      numberProps = 0;
      keys1 = 0;
      keys2 = 0;
      keys3 = 0;
      keys4 = 0;
    }
  GetPropECommand(unsigned int tgNum, int inNumberProps,
		 otherKeyType * inKeys1, otherKeyType * inKeys2,
		 otherKeyType * inKeys3, otherKeyType * inKeys4):
    Command(tgNum, EventTag, ipp::GetPropE)
    {
      int n;
      keys1 = new otherKeyType[inNumberProps];
      keys2 = new otherKeyType[inNumberProps];
      keys3 = new otherKeyType[inNumberProps];
      keys4 = new otherKeyType[inNumberProps];
      for (n = 0; n < inNumberProps; n++)
	{
	  keys1[n] = inKeys1[n];
	  keys2[n] = inKeys2[n];
	  keys3[n] = inKeys3[n];
	  keys4[n] = inKeys4[n];
	}
      numberProps = inNumberProps;
    }
  ~GetPropECommand()
    {
      delete keys1;
      delete keys2;
      delete keys3;
      delete keys4;
    }

  int setNewProps(int inNumberProps)
    {
      if (inNumberProps > 0)
	{
	  delete keys1; // OK if NULL
	  delete keys2;
	  delete keys3;
	  delete keys4;
	  numberProps = inNumberProps;
	  keys1 = new otherKeyType[inNumberProps];
	  keys2 = new otherKeyType[inNumberProps];
	  keys3 = new otherKeyType[inNumberProps];
	  keys4 = new otherKeyType[inNumberProps];
	}
      return (inNumberProps > 0);
    }
  int setNumberProps(int inNumberProps)
    {
      if (inNumberProps > 0)
	numberProps = inNumberProps;
      return (inNumberProps > 0);
    }
  int setKey1(otherKeyType inKey1, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys1[n] = inKey1;
      return ((n < numberProps) && (n > -1));
    }
  int setKey2(otherKeyType inKey2, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys2[n] = inKey2;
      return ((n < numberProps) && (n > -1));
    }
  int setKey3(otherKeyType inKey3, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys3[n] = inKey3;
      return ((n < numberProps) && (n > -1));
    }
  int setKey4(otherKeyType inKey4, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys4[n] = inKey4;
      return ((n < numberProps) && (n > -1));
    }
  
  int getNumberProps() { return numberProps; }
  otherKeyType getKey1(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys1[n] : ErrorKey); }
  otherKeyType getKey2(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys2[n] : ErrorKey); }
  otherKeyType getKey3(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys3[n] : ErrorKey); }
  otherKeyType getKey4(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys4[n] : ErrorKey); }

  char * getCommandStringT(char * buffer, char * tagString);
};

/* GoToCommand class **************************************************/

class GoToCommand : public Command
{
private:
  bool hasX;
  bool hasY;
  bool hasZ;
  double  x;
  double  y;
  double  z;
  bool hasToolA;
  bool hasToolB;
  bool hasToolC;
  double toolA;
  double toolB;
  double toolC;

public:
  GoToCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, GoTo){}
  GoToCommand(unsigned int tgNum,
	      bool inHasX, bool inHasY, bool inHasZ,
	      double  inX, double  inY, double  inZ,
	      bool inHasToolA, bool inHasToolB, bool inHasToolC, 
	      double  inToolA, double  inToolB, double  inToolC):
    Command(tgNum, CommandTag, GoTo)
    {
      hasX = inHasX;
      hasY = inHasY;
      hasZ = inHasZ;
      x = inX;
      y = inY;
      z = inZ;
      hasToolA = inHasToolA;
      hasToolB = inHasToolB;
      hasToolC = inHasToolC;
      toolA = inToolA;
      toolB = inToolB;
      toolC = inToolC;
    }
  ~GoToCommand(){}
  
  void setHasX(bool inHasX){ hasX = inHasX; }
  void setHasY(bool inHasY){ hasY = inHasY; }
  void setHasZ(bool inHasZ){ hasZ = inHasZ; }
  void setX(double inX){ x = inX; }
  void setY(double inY){ y = inY; }
  void setZ(double inZ){ z = inZ; }
  void setHasToolA(bool inHasToolA){ hasToolA = inHasToolA; }
  void setHasToolB(bool inHasToolB){ hasToolB = inHasToolB; }
  void setHasToolC(bool inHasToolC){ hasToolC = inHasToolC; }
  void setToolA(double inToolA){ toolA = inToolA; }
  void setToolB(double inToolB){ toolB = inToolB; }
  void setToolC(double inToolC){ toolC = inToolC; }

  bool getHasX(){ return hasX; }
  bool getHasY(){ return hasY; }
  bool getHasZ(){ return hasZ; }
  double getX(){ return x; }  
  double getY(){ return y; }  
  double getZ(){ return z; }  
  bool getHasToolA(){ return hasToolA; }
  bool getHasToolB(){ return hasToolB; }
  bool getHasToolC(){ return hasToolC; }
  double getToolA(){ return toolA; }
  double getToolB(){ return toolB; }
  double getToolC(){ return toolC; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* LockAxisCommand class ***************************************************/

class LockAxisCommand : public Command {
  
private:
  bool hasC;
  bool hasPhi;
  bool hasR;
  bool hasX;
  bool hasY;
  bool hasZ;
  
public:
  LockAxisCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, LockAxis){}
  LockAxisCommand(unsigned int tgNum, bool inHasC, bool inHasPhi,
		  bool inHasR, bool inHasX, bool inHasY, bool inHasZ):
    Command(tgNum, CommandTag, LockAxis)
    {
      hasC = inHasC;
      hasPhi = inHasPhi;
      hasR = inHasR;
      hasX = inHasX;
      hasY = inHasY;
      hasZ = inHasZ;
    }
  ~LockAxisCommand(){}

  void setHasC(bool inHasC){ hasC = inHasC; }
  void setHasPhi(bool inHasPhi){ hasPhi = inHasPhi; }
  void setHasR(bool inHasR){ hasR = inHasR; }
  void setHasX(bool inHasX){ hasX = inHasX; }
  void setHasY(bool inHasY){ hasY = inHasY; }
  void setHasZ(bool inHasZ){ hasZ = inHasZ; }
  
  bool getHasC(){ return hasC; }
  bool getHasPhi(){ return hasPhi; }
  bool getHasR(){ return hasR; }
  bool getHasX(){ return hasX; }
  bool getHasY(){ return hasY; }
  bool getHasZ(){ return hasZ; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* OnMoveReportECommand class *****************************************/

class OnMoveReportECommand : public Command {
  
private:
  bool hasR;
  bool hasX;
  bool hasY;
  bool hasZ;
  bool hasToolA;
  bool hasToolB;
  bool hasToolC;
  bool hasDis;
  bool hasTime;
  double theDis;
  double theTime;

public:
  OnMoveReportECommand(unsigned int tgNum):
    Command(tgNum, EventTag, OnMoveReportE){}
  OnMoveReportECommand(unsigned int tgNum,
		       bool inHasR, bool inHasX, bool inHasY, bool inHasZ,
		       bool inHasToolA, bool inHasToolB, bool inHasToolC,
		       bool inHasDis, bool inHasTime,
		       double inTheDis, double inTheTime):
    Command(tgNum, EventTag, OnMoveReportE)
    {
      hasR = inHasR;
      hasX = inHasX;
      hasY = inHasY;
      hasZ = inHasZ;
      hasToolA = inHasToolA;
      hasToolB = inHasToolB;
      hasToolC = inHasToolC;
      hasDis = inHasDis;
      hasTime = inHasTime;
      theDis = inTheDis;
      theTime = inTheTime;
    }
  ~OnMoveReportECommand(){}

  void setHasR(bool inHasR){ hasR = inHasR; }
  void setHasX(bool inHasX){ hasX = inHasX; }
  void setHasY(bool inHasY){ hasY = inHasY; }
  void setHasZ(bool inHasZ){ hasZ = inHasZ; }
  void setHasToolA(bool inHasToolA){ hasToolA = inHasToolA; }
  void setHasToolB(bool inHasToolB){ hasToolB = inHasToolB; }
  void setHasToolC(bool inHasToolC){ hasToolC = inHasToolC; }
  void setHasDis(bool inHasDis){ hasDis = inHasDis; }
  void setHasTime(bool inHasTime){ hasTime = inHasTime; }
  double setTheDis(double inTheDis) {
    theDis = ((inTheDis > 0) ? inTheDis : theDis);
    return theDis; }
  double setTheTime(double inTheTime) {
    theTime = ((inTheTime > 0.1) ? inTheTime : theTime);
    return theTime; }

  bool getHasR(){ return hasR; }
  bool getHasX(){ return hasX; }
  bool getHasY(){ return hasY; }
  bool getHasZ(){ return hasZ; }
  bool getHasToolA(){ return hasToolA; }
  bool getHasToolB(){ return hasToolB; }
  bool getHasToolC(){ return hasToolC; }
  bool getHasDis(){ return hasDis; }
  bool getHasTime(){ return hasTime; }
  double getTheDis(){ return theDis; }
  double getTheTime(){ return theTime; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* OnPtMeasReportCommand class *****************************************/

class OnPtMeasReportCommand : public Command {
  
private:
  bool hasER;
  bool hasIJK;
  bool hasR;
  bool hasX;
  bool hasY;
  bool hasZ;
  bool hasToolA;
  bool hasToolB;
  bool hasToolC;
  
public:
  OnPtMeasReportCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, OnPtMeasReport){}
  OnPtMeasReportCommand(unsigned int tgNum, bool inHasER, bool inHasIJK,
			bool inHasR, bool inHasX, bool inHasY, bool inHasZ,
			bool inHasToolA, bool inHasToolB, bool inHasToolC):
    Command(tgNum, CommandTag, OnPtMeasReport)
    {
      hasER = inHasER;
      hasIJK = inHasIJK;
      hasR = inHasR;
      hasX = inHasX;
      hasY = inHasY;
      hasZ = inHasZ;
      hasToolA = inHasToolA;
      hasToolB = inHasToolB;
      hasToolC = inHasToolC;
    }
  ~OnPtMeasReportCommand(){}

  void setHasER(bool inHasER){ hasER = inHasER; }
  void setHasIJK(bool inHasIJK){ hasIJK = inHasIJK; }
  void setHasR(bool inHasR){ hasR = inHasR; }
  void setHasX(bool inHasX){ hasX = inHasX; }
  void setHasY(bool inHasY){ hasY = inHasY; }
  void setHasZ(bool inHasZ){ hasZ = inHasZ; }
  void setHasToolA(bool inHasToolA){ hasToolA = inHasToolA; }
  void setHasToolB(bool inHasToolB){ hasToolB = inHasToolB; }
  void setHasToolC(bool inHasToolC){ hasToolC = inHasToolC; }
 
  bool getHasER(){ return hasER; }
  bool getHasIJK(){ return hasIJK; }
  bool getHasR(){ return hasR; }
  bool getHasX(){ return hasX; }
  bool getHasY(){ return hasY; }
  bool getHasZ(){ return hasZ; }
  bool getHasToolA(){ return hasToolA; }
  bool getHasToolB(){ return hasToolB; }
  bool getHasToolC(){ return hasToolC; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* OnScanReportCommand class *****************************************/

class OnScanReportCommand : public Command {

private:
  bool hasER;
  bool hasIJK;
  bool hasQ;
  bool hasR;
  bool hasX;
  bool hasY;
  bool hasZ;
  bool hasToolA;
  bool hasToolB;
  bool hasToolC;
   
public:
  OnScanReportCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, OnScanReport){}
  OnScanReportCommand(unsigned int tgNum, bool inHasER, bool inHasIJK,
		      bool inHasQ, bool inHasR, bool inHasX, bool inHasY,
		      bool inHasZ, bool inHasToolA, bool inHasToolB,
		      bool inHasToolC):
    Command(tgNum, CommandTag, OnScanReport)
    {
      hasER = inHasER;
      hasIJK = inHasIJK;
      hasQ = inHasQ;
      hasR = inHasR;
      hasX = inHasX;
      hasY = inHasY;
      hasZ = inHasZ;
      hasToolA = inHasToolA;
      hasToolB = inHasToolB;
      hasToolC = inHasToolC;
    }
  ~OnScanReportCommand(){}
  
  void setHasER(bool inHasER){ hasER = inHasER; }
  void setHasIJK(bool inHasIJK){ hasIJK = inHasIJK; }
  void setHasQ(bool inHasQ){ hasQ = inHasQ; }
  void setHasR(bool inHasR){ hasR = inHasR; }
  void setHasX(bool inHasX){ hasX = inHasX; }
  void setHasY(bool inHasY){ hasY = inHasY; }
  void setHasZ(bool inHasZ){ hasZ = inHasZ; }
  void setHasToolA(bool inHasToolA){ hasToolA = inHasToolA; }
  void setHasToolB(bool inHasToolB){ hasToolB = inHasToolB; }
  void setHasToolC(bool inHasToolC){ hasToolC = inHasToolC; }

  bool getHasER(){ return hasER; }
  bool getHasIJK(){ return hasIJK; }
  bool getHasQ(){ return hasQ; }
  bool getHasR(){ return hasR; }
  bool getHasX(){ return hasX; }
  bool getHasY(){ return hasY; }
  bool getHasZ(){ return hasZ; }
  bool getHasToolA(){ return hasToolA; }
  bool getHasToolB(){ return hasToolB; }
  bool getHasToolC(){ return hasToolC; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* PtMeasCommand class ************************************************/

class PtMeasCommand : public Command {
  
private:
  bool hasX;
  bool hasY;
  bool hasZ;
  bool hasIJK;
  double x;
  double y;
  double z;
  TargetVector theIJK;
  
public:
  PtMeasCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, PtMeas){}
  PtMeasCommand(unsigned int tgNum, bool inHasX, bool inHasY, bool inHasZ,
		bool inHasIJK, double inX, double inY, double inZ,
		double inI, double inJ, double inK):
    Command(tgNum, CommandTag, PtMeas)
    {
      hasX = inHasX;
      hasY = inHasY;
      hasZ = inHasZ;
      hasIJK = inHasIJK;
      x = inX;
      y = inY;
      z = inZ;
      theIJK.setValues(inI, inJ, inK);
    }
  ~PtMeasCommand(){}
  
  void setHasX(bool inHasX){ hasX = inHasX; }
  void setHasY(bool inHasY){ hasY = inHasY; }
  void setHasZ(bool inHasZ){ hasZ = inHasZ; }
  void setX(double inX){ x = inX; }
  void setY(double inY){ y = inY; }
  void setZ(double inZ){ z = inZ; }
  void setIJK(double inI, double inJ, double inK)
    { theIJK.setValues(inI, inJ, inK); }

  bool getHasX(){ return hasX; }
  bool getHasY(){ return hasY; }
  bool getHasZ(){ return hasZ; }
  bool getHasIJK(){ return hasIJK; }
  double getX(){ return x; }
  double getY(){ return y; }
  double getZ(){ return z; }
  double getI(){ return theIJK.getI(); }
  double getJ(){ return theIJK.getJ(); }
  double getK(){ return theIJK.getK(); }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ScanInCylEndIsPlaneCommand class ***********************************/

class ScanInCylEndIsPlaneCommand : public Command {
  
private:
  double cx;
  double cy;
  double cz;
  TargetVector cIJK;
  double sx;
  double sy;
  double sz;
  TargetVector sIJK;
  double dx;
  double dy;
  double dz;
  double stepW;
  double px;
  double py;
  double pz;
  TargetVector pIJK;
  int num;
  TargetVector eIJK;

public:
  ScanInCylEndIsPlaneCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ScanInCylEndIsPlane){}
  ScanInCylEndIsPlaneCommand(unsigned int tgNum,
			     double inCx, double inCy, double inCz,
			     double inCI, double inCJ, double inCK,
			     double inSx, double inSy, double inSz,
			     double inSI, double inSJ, double inSK,
			     double inDx, double inDy, double inDz,
			     double inStepW,
			     double inPx, double inPy, double inPz,
			     double inPI, double inPJ, double inPK,
			     int inNum,
			     double inEI, double inEJ, double inEK):
    Command(tgNum, CommandTag, ScanInCylEndIsPlane)
    {
      cx = inCx;
      cy = inCy;
      cz = inCz;
      cIJK.setValues(inCI, inCJ, inCK);
      sx = inSx;
      sy = inSy;
      sz = inSz;
      sIJK.setValues(inSI, inSJ, inSK);
      dx = inDx;
      dy = inDy;
      dz = inDz;
      stepW = inStepW;
      px = inPx;
      py = inPy;
      pz = inPz;
      pIJK.setValues(inPI, inPJ, inPK);
      num = inNum;
      eIJK.setValues(inEI, inEJ, inEK);
    }
  ~ScanInCylEndIsPlaneCommand(){}

  void setCx(double inCx){ cx = inCx; }
  void setCy(double inCy){ cy = inCy; }
  void setCz(double inCz){ cz = inCz; }
  void setCIJK(double inCI, double inCJ, double inCK)
    { cIJK.setValues(inCI, inCJ, inCK); }
  void setSx(double inSx){ sx = inSx; }
  void setSy(double inSy){ sy = inSy; }
  void setSz(double inSz){ sz = inSz; }
  void setSIJK(double inSI, double inSJ, double inSK)
    { sIJK.setValues(inSI, inSJ, inSK); }
  void setDx(double inDx){ dx = inDx; }
  void setDy(double inDy){ dy = inDy; }
  void setDz(double inDz){ dz = inDz; }
  void setStepW(double inStepW){ stepW = inStepW; }
  void setPx(double inPx){ px = inPx; }
  void setPy(double inPy){ py = inPy; }
  void setPz(double inPz){ pz = inPz; }
  void setPIJK(double inPI, double inPJ, double inPK)
    { pIJK.setValues(inPI, inPJ, inPK); }
  void setNum(int inNum){ num = inNum; }
  void setEIJK(double inEI, double inEJ, double inEK)
    { eIJK.setValues(inEI, inEJ, inEK); }


  double getCx(){ return cx; }
  double getCy(){ return cy; }
  double getCz(){ return cz; }
  double getCI(){ return cIJK.getI(); }
  double getCJ(){ return cIJK.getJ(); }
  double getCK(){ return cIJK.getK(); }
  double getSx(){ return sx; }
  double getSy(){ return sy; }
  double getSz(){ return sz; }
  double getSI(){ return sIJK.getI(); }
  double getSJ(){ return sIJK.getJ(); }
  double getSK(){ return sIJK.getK(); }
  double getDx(){ return dx; }
  double getDy(){ return dy; }
  double getDz(){ return dz; }
  double getStepW(){ return stepW; }
  double getPx(){ return px; }
  double getPy(){ return py; }
  double getPz(){ return pz; }
  double getPI(){ return pIJK.getI(); }
  double getPJ(){ return pIJK.getJ(); }
  double getPK(){ return pIJK.getK(); }
  int getNum(){ return num; }
  double getEI(){ return eIJK.getI(); }
  double getEJ(){ return eIJK.getJ(); }
  double getEK(){ return eIJK.getK(); }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ScanInCylEndIsSphereCommand class **********************************/

class ScanInCylEndIsSphereCommand : public Command {
  
private:
  double cx;
  double cy;
  double cz;
  TargetVector cIJK;
  double sx;
  double sy;
  double sz;
  TargetVector sIJK;
  double dx;
  double dy;
  double dz;
  double stepW;
  double ex;
  double ey;
  double ez;
  double dia;
  TargetVector eIJK;

public:
  ScanInCylEndIsSphereCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ScanInCylEndIsSphere){}
  ScanInCylEndIsSphereCommand(unsigned int tgNum,
			      double inCx, double inCy, double inCz,
			      double inCI, double inCJ, double inCK,
			      double inSx, double inSy, double inSz,
			      double inSI, double inSJ, double inSK,
			      double inDx, double inDy, double inDz,
			      double inStepW,
			      double inEx, double inEy, double inEz,
			      double inDia,
			      double inEI, double inEJ, double inEK):
    Command(tgNum, CommandTag, ScanInCylEndIsSphere)
    {
      cx = inCx;
      cy = inCy;
      cz = inCz;
      cIJK.setValues(inCI, inCJ, inCK);
      sx = inSx;
      sy = inSy;
      sz = inSz;
      sIJK.setValues(inSI, inSJ, inSK);
      dx = inDx;
      dy = inDy;
      dz = inDz;
      stepW = inStepW;
      ex = inEx;
      ey = inEy;
      ez = inEz;
      dia = inDia;
      eIJK.setValues(inEI, inEJ, inEK);
    }
  ~ScanInCylEndIsSphereCommand(){}

  void setCx(double inCx){ cx = inCx; }
  void setCy(double inCy){ cy = inCy; }
  void setCz(double inCz){ cz = inCz; }
  void setCIJK(double inCI, double inCJ, double inCK)
    { cIJK.setValues(inCI, inCJ, inCK); }
  void setSx(double inSx){ sx = inSx; }
  void setSy(double inSy){ sy = inSy; }
  void setSz(double inSz){ sz = inSz; }
  void setSIJK(double inSI, double inSJ, double inSK)
    { sIJK.setValues(inSI, inSJ, inSK); }
  void setDx(double inDx){ dx = inDx; }
  void setDy(double inDy){ dy = inDy; }
  void setDz(double inDz){ dz = inDz; }
  void setStepW(double inStepW){ stepW = inStepW; }
  void setEx(double inEx){ ex = inEx; }
  void setEy(double inEy){ ey = inEy; }
  void setEz(double inEz){ ez = inEz; }
  void setDia(double inDia){ dia = inDia; }
  void setEIJK(double inEI, double inEJ, double inEK)
    { eIJK.setValues(inEI, inEJ, inEK); }

  double getCx(){ return cx; }
  double getCy(){ return cy; }
  double getCz(){ return cz; }
  double getCI(){ return cIJK.getI(); }
  double getCJ(){ return cIJK.getJ(); }
  double getCK(){ return cIJK.getK(); }
  double getSx(){ return sx; }
  double getSy(){ return sy; }
  double getSz(){ return sz; }
  double getSI(){ return sIJK.getI(); }
  double getSJ(){ return sIJK.getJ(); }
  double getSK(){ return sIJK.getK(); }
  double getDx(){ return dx; }
  double getDy(){ return dy; }
  double getDz(){ return dz; }
  double getStepW(){ return stepW; }
  double getEx(){ return ex; }
  double getEy(){ return ey; }
  double getEz(){ return ez; }
  double getDia(){ return dia; }
  double getEI(){ return eIJK.getI(); }
  double getEJ(){ return eIJK.getJ(); }
  double getEK(){ return eIJK.getK(); }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ScanInPlaneEndIsCylCommand class ***********************************/

class ScanInPlaneEndIsCylCommand : public Command {
  
private:
  double sx;
  double sy;
  double sz;
  TargetVector sIJK;
  double dx;
  double dy;
  double dz;
  double stepW;
  double cx;
  double cy;
  double cz;
  TargetVector cIJK;
  double dia;
  int num;
  TargetVector eIJK;

public:
  ScanInPlaneEndIsCylCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ScanInPlaneEndIsCyl){}
  ScanInPlaneEndIsCylCommand(unsigned int tgNum,
			     double inSx, double inSy, double inSz,
			     double inSI, double inSJ, double inSK,
			     double inDx, double inDy, double inDz,
			     double inStepW,
			     double inCx, double inCy, double inCz,
			     double inCI, double inCJ, double inCK,
			     double inDia,
			     int inNum,
			     double inEI, double inEJ, double inEK):
    Command(tgNum, CommandTag, ScanInPlaneEndIsCyl)
    {
      sx = inSx;
      sy = inSy;
      sz = inSz;
      sIJK.setValues(inSI, inSJ, inSK);
      dx = inDx;
      dy = inDy;
      dz = inDz;
      stepW = inStepW;
      cx = inCx;
      cy = inCy;
      cz = inCz;
      cIJK.setValues(inCI, inCJ, inCK);
      dia = inDia;
      num = inNum;
      eIJK.setValues(inEI, inEJ, inEK);
    }
  ~ScanInPlaneEndIsCylCommand(){}

  void setSx(double inSx){ sx = inSx; }
  void setSy(double inSy){ sy = inSy; }
  void setSz(double inSz){ sz = inSz; }
  void setSIJK(double inSI, double inSJ, double inSK)
    { sIJK.setValues(inSI, inSJ, inSK); }
  void setDx(double inDx){ dx = inDx; }
  void setDy(double inDy){ dy = inDy; }
  void setDz(double inDz){ dz = inDz; }
  void setStepW(double inStepW){ stepW = inStepW; }
  void setCx(double inCx){ cx = inCx; }
  void setCy(double inCy){ cy = inCy; }
  void setCz(double inCz){ cz = inCz; }
  void setCIJK(double inCI, double inCJ, double inCK)
    { cIJK.setValues(inCI, inCJ, inCK); }
  void setDia(double inDia){ dia = inDia; }
  void setNum(int inNum){ num = inNum; }
  void setEIJK(double inEI, double inEJ, double inEK)
    { eIJK.setValues(inEI, inEJ, inEK); }

  double getSx(){ return sx; }
  double getSy(){ return sy; }
  double getSz(){ return sz; }
  double getSI(){ return sIJK.getI(); }
  double getSJ(){ return sIJK.getJ(); }
  double getSK(){ return sIJK.getK(); }
  double getDx(){ return dx; }
  double getDy(){ return dy; }
  double getDz(){ return dz; }
  double getStepW(){ return stepW; }
  double getCx(){ return cx; }
  double getCy(){ return cy; }
  double getCz(){ return cz; }
  double getCI(){ return cIJK.getI(); }
  double getCJ(){ return cIJK.getJ(); }
  double getCK(){ return cIJK.getK(); }
  double getDia(){ return dia; }
  int getNum(){ return num; }
  double getEI(){ return eIJK.getI(); }
  double getEJ(){ return eIJK.getJ(); }
  double getEK(){ return eIJK.getK(); }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ScanInPlaneEndIsPlaneCommand class *********************************/

class ScanInPlaneEndIsPlaneCommand : public Command {

private:
  double sx;
  double sy;
  double sz;
  TargetVector sIJK;
  double dx;
  double dy;
  double dz;
  double stepW;
  double px;
  double py;
  double pz;
  TargetVector pIJK;
  int num;
  TargetVector eIJK;

public:
  ScanInPlaneEndIsPlaneCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ScanInPlaneEndIsPlane){}
  ScanInPlaneEndIsPlaneCommand(unsigned int tgNum,
				double inSx, double inSy, double inSz,
				double inSI, double inSJ, double inSK,
				double inDx, double inDy, double inDz,
				double inStepW,
				double inPx, double inPy, double inPz,
				double inPI, double inPJ, double inPK,
				int inNum,
				double inEI, double inEJ, double inEK):
    Command(tgNum, CommandTag, ScanInPlaneEndIsPlane)
    {
      sx = inSx;
      sy = inSy;
      sz = inSz;
      sIJK.setValues(inSI, inSJ, inSK);
      dx = inDx;
      dy = inDy;
      dz = inDz;
      stepW = inStepW;
      px = inPx;
      py = inPy;
      pz = inPz;
      pIJK.setValues(inPI, inPJ, inPK);
      num = inNum;
      eIJK.setValues(inEI, inEJ, inEK);
    }
  ~ScanInPlaneEndIsPlaneCommand(){}

  void setSx(double inSx){ sx = inSx; }
  void setSy(double inSy){ sy = inSy; }
  void setSz(double inSz){ sz = inSz; }
  void setSIJK(double inSI, double inSJ, double inSK)
    { sIJK.setValues(inSI, inSJ, inSK); }
  void setDx(double inDx){ dx = inDx; }
  void setDy(double inDy){ dy = inDy; }
  void setDz(double inDz){ dz = inDz; }
  void setStepW(double inStepW){ stepW = inStepW; }
  void setPx(double inPx){ px = inPx; }
  void setPy(double inPy){ py = inPy; }
  void setPz(double inPz){ pz = inPz; }
  void setPIJK(double inPI, double inPJ, double inPK)
    { pIJK.setValues(inPI, inPJ, inPK); }
  void setNum(int inNum){ num = inNum; }
  void setEIJK(double inEI, double inEJ, double inEK)
    { eIJK.setValues(inEI, inEJ, inEK); }

  double getSx(){ return sx; }
  double getSy(){ return sy; }
  double getSz(){ return sz; }
  double getSI(){ return sIJK.getI(); }
  double getSJ(){ return sIJK.getJ(); }
  double getSK(){ return sIJK.getK(); }
  double getDx(){ return dx; }
  double getDy(){ return dy; }
  double getDz(){ return dz; }
  double getStepW(){ return stepW; }
  double getPx(){ return px; }
  double getPy(){ return py; }
  double getPz(){ return pz; }
  double getPI(){ return pIJK.getI(); }
  double getPJ(){ return pIJK.getJ(); }
  double getPK(){ return pIJK.getK(); }
  int getNum(){ return num; }
  double getEI(){ return eIJK.getI(); }
  double getEJ(){ return eIJK.getJ(); }
  double getEK(){ return eIJK.getK(); }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ScanInPlaneEndIsSphereCommand class ********************************/

class ScanInPlaneEndIsSphereCommand : public Command {
  
private:
  double sx;
  double sy;
  double sz;
  TargetVector sIJK;
  double dx;
  double dy;
  double dz;
  double stepW;
  double ex;
  double ey;
  double ez;
  double dia;
  TargetVector eIJK;

public:
  ScanInPlaneEndIsSphereCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ScanInPlaneEndIsSphere){}
  ScanInPlaneEndIsSphereCommand(unsigned int tgNum,
				double inSx, double inSy, double inSz,
				double inSI, double inSJ, double inSK,
				double inDx, double inDy, double inDz,
				double inStepW,
				double inEx, double inEy, double inEz,
				double inDia,
				double inEI, double inEJ, double inEK):
    Command(tgNum, CommandTag, ScanInPlaneEndIsSphere)
    {
      sx = inSx;
      sy = inSy;
      sz = inSz;
      sIJK.setValues(inSI, inSJ, inSK);
      dx = inDx;
      dy = inDy;
      dz = inDz;
      stepW = inStepW;
      ex = inEx;
      ey = inEy;
      ez = inEz;
      dia = inDia;
      eIJK.setValues(inEI, inEJ, inEK);
    }
  ~ScanInPlaneEndIsSphereCommand(){}

  void setSx(double inSx){ sx = inSx; }
  void setSy(double inSy){ sy = inSy; }
  void setSz(double inSz){ sz = inSz; }
  void setSIJK(double inSI, double inSJ, double inSK)
    { sIJK.setValues(inSI, inSJ, inSK); }
  void setDx(double inDx){ dx = inDx; }
  void setDy(double inDy){ dy = inDy; }
  void setDz(double inDz){ dz = inDz; }
  void setStepW(double inStepW){ stepW = inStepW; }
  void setEx(double inEx){ ex = inEx; }
  void setEy(double inEy){ ey = inEy; }
  void setEz(double inEz){ ez = inEz; }
  void setDia(double inDia){ dia = inDia; }
  void setEIJK(double inEI, double inEJ, double inEK)
    { eIJK.setValues(inEI, inEJ, inEK); }

  double getSx(){ return sx; }
  double getSy(){ return sy; }
  double getSz(){ return sz; }
  double getSI(){ return sIJK.getI(); }
  double getSJ(){ return sIJK.getJ(); }
  double getSK(){ return sIJK.getK(); }
  double getDx(){ return dx; }
  double getDy(){ return dy; }
  double getDz(){ return dz; }
  double getStepW(){ return stepW; }
  double getEx(){ return ex; }
  double getEy(){ return ey; }
  double getEz(){ return ez; }
  double getDia(){ return dia; }
  double getEI(){ return eIJK.getI(); }
  double getEJ(){ return eIJK.getJ(); }
  double getEK(){ return eIJK.getK(); }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ScanOnCircleCommand class ******************************************/

class ScanOnCircleCommand : public Command {
  
private:
  double cx;
  double cy;
  double cz;
  double sx;
  double sy;
  double sz;
  TargetVector theIJK;
  double delta;
  double sfa;
  double stepW;

public:
  ScanOnCircleCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ScanOnCircle){}
  ScanOnCircleCommand(unsigned int tgNum,
		      double inCx, double inCy, double inCz,
		      double inSx, double inSy, double inSz,
		      double inI, double inJ, double inK,
		      double inDelta, double inSfa, double inStepW):
    Command(tgNum, CommandTag, ScanOnCircle)
    {
      cx = inCx;
      cy = inCy;
      cz = inCz;
      sx = inSx;
      sy = inSy;
      sz = inSz;
      theIJK.setValues(inI, inJ, inK);
      delta = inDelta;
      sfa = inSfa;
      stepW = inStepW;
    }
  ~ScanOnCircleCommand(){}

  void setCx(double inCx){ cx = inCx; }
  void setCy(double inCy){ cy = inCy; }
  void setCz(double inCz){ cz = inCz; }
  void setSx(double inSx){ sx = inSx; }
  void setSy(double inSy){ sy = inSy; }
  void setSz(double inSz){ sz = inSz; }
  void setIJK(double inI, double inJ, double inK)
    { theIJK.setValues(inI, inJ, inK); }
  void setDelta(double inDelta){ delta = inDelta; }
  void setSfa(double inSfa){ sfa = inSfa; }
  void setStepW(double inStepW){ stepW = inStepW; }

  double getCx(){ return cx; }
  double getCy(){ return cy; }
  double getCz(){ return cz; }
  double getSx(){ return sx; }
  double getSy(){ return sy; }
  double getSz(){ return sz; }
  double getI(){ return theIJK.getI(); }
  double getJ(){ return theIJK.getJ(); }
  double getK(){ return theIJK.getK(); }
  double getDelta(){ return delta; }
  double getSfa(){ return sfa; }
  double getStepW(){ return stepW; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ScanOnCircleHintCommand class **************************************/

class ScanOnCircleHintCommand : public Command {
  
private:
  double displacement;
  double form;

public:
  ScanOnCircleHintCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ScanOnCircleHint){}
  ScanOnCircleHintCommand(unsigned int tgNum,
			  double inDisplacement, double inForm):
    Command(tgNum, CommandTag, ScanOnCircleHint)
    {
      displacement = inDisplacement;
      form = inForm;
    }
  ~ScanOnCircleHintCommand(){}

  void setDisplacement(double inDisplacement){ displacement = inDisplacement; }
  void setForm(double inForm){ form = inForm; }

  double getDisplacement(){ return displacement; }
  double getForm(){ return form; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ScanOnLineCommand class ********************************************/

class ScanOnLineCommand : public Command {
  
private:
  double sx;
  double sy;
  double sz;
  double ex;
  double ey;
  double ez;
  TargetVector theIJK;
  double stepW;

public:
  ScanOnLineCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ScanOnLine){}
  ScanOnLineCommand(unsigned int tgNum, double inSx, double inSy, double inSz,
		    double inEx, double inEy, double inEz, double inI,
		    double inJ, double inK, double inStepW):
    Command(tgNum, CommandTag, ScanOnLine)
    {
      sx = inSx;
      sy = inSy;
      sz = inSz;
      ex = inEx;
      ey = inEy;
      ez = inEz;
      theIJK.setValues(inI, inJ, inK);
      stepW = inStepW;
    }
  ~ScanOnLineCommand(){}

  void setSx(double inSx){ sx = inSx; }
  void setSy(double inSy){ sy = inSy; }
  void setSz(double inSz){ sz = inSz; }
  void setEx(double inEx){ ex = inEx; }
  void setEy(double inEy){ ey = inEy; }
  void setEz(double inEz){ ez = inEz; }
  void setIJK(double inI, double inJ, double inK)
    { theIJK.setValues(inI, inJ, inK); }
  void setStepW(double inStepW) { stepW = inStepW; }

  double getSx(){ return sx; }
  double getSy(){ return sy; }
  double getSz(){ return sz; }
  double getEx(){ return ex; }
  double getEy(){ return ey; }
  double getEz(){ return ez; }
  double getI(){ return theIJK.getI(); }
  double getJ(){ return theIJK.getJ(); }
  double getK(){ return theIJK.getK(); }
  double getStepW(){ return stepW; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ScanOnLineHintCommand class ****************************************/

class ScanOnLineHintCommand : public Command {
  
private:
  double angle;
  double form;

public:
  ScanOnLineHintCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ScanOnLineHint){}
  ScanOnLineHintCommand(unsigned int tgNum, double inAngle, double inForm):
    Command(tgNum, CommandTag, ScanOnLineHint)
    {
      angle = inAngle;
      form = inForm;
    }
  ~ScanOnLineHintCommand(){}

  void setAngle(double inAngle){ angle = inAngle; }
  void setForm(double inForm){ form = inForm; }

  double getAngle(){ return angle; }
  double getForm(){ return form; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* ScanUnknownHintCommand class ***************************************/

class ScanUnknownHintCommand : public Command {
  
private:
  double minRadius;

public:
  ScanUnknownHintCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ScanUnknownHint){}
  ScanUnknownHintCommand(unsigned int tgNum, double inMinRadius):
    Command(tgNum, CommandTag, ScanUnknownHint){ minRadius = inMinRadius; }
  ~ScanUnknownHintCommand(){}

  void setMinRadius(double inMinRadius){ minRadius = inMinRadius; }

  double getMinRadius(){ return minRadius;}
  char * getCommandStringT(char * buffer, char * tagString);
};

/* SetCoordSystemCommand class ****************************************/

class SetCoordSystemCommand : public Command {
  
private:
  coordSysType coordSys;
  
public:
  SetCoordSystemCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, SetCoordSystem){}
  SetCoordSystemCommand(unsigned int tgNum, coordSysType inCoordSys):
    Command(tgNum, CommandTag, SetCoordSystem){ coordSys = inCoordSys; }
  ~SetCoordSystemCommand(){}
  
  void setCsyType(coordSysType cst){ coordSys = cst; }

  coordSysType getCsyType(){ return coordSys; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* SetCsyTransformationCommand class **********************************/

class SetCsyTransformationCommand : public Command {

private:
  coordSysType coordSys;
  double x0;
  double y0;
  double z0;
  double theta;
  double psi;
  double phi;

public:
  SetCsyTransformationCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, SetCsyTransformation){}
  SetCsyTransformationCommand(unsigned int tgNum, coordSysType inCoordSys,
                              double inX0, double inY0, double inZ0,
                              double inTheta, double inPsi, double inPhi):
    Command(tgNum, CommandTag, SetCsyTransformation)
    {
      coordSys = inCoordSys;
      x0 = inX0;
      y0 = inY0;
      z0 = inZ0;
      theta = inTheta;
      psi = inPsi;
      phi = inPhi;
    }
  ~SetCsyTransformationCommand(){}

  void setCoordSys(coordSysType inCoordSys){ coordSys = inCoordSys; }
  void setX0(double inX0){ x0 = inX0; }
  void setY0(double inY0){ y0 = inY0; }
  void setZ0(double inZ0){ z0 = inZ0; }
  void setTheta(double inTheta){ theta = inTheta; }
  void setPsi(double inPsi){ psi = inPsi; }
  void setPhi(double inPhi){ phi = inPhi; }

  coordSysType getCoordSys(){ return coordSys; }
  double getX0(){ return x0; }
  double getY0(){ return y0; }
  double getZ0(){ return z0; }
  double getTheta(){ return theta; }
  double getPsi(){ return psi; }
  double getPhi(){ return phi; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* SetPropCommand class ************************************************

This is along the same lines as GetPropCommand, except that this
contains values.

The getValue function returns the value if n is in range
(0 <= n < numberProps) and returns 0 if not.

*/

class SetPropCommand : public Command {
  
private:
  int numberProps;
  otherKeyType * keys1;
  otherKeyType * keys2;
  otherKeyType * keys3;
  otherKeyType * keys4;
  double * propertyValues;
  
public:
  SetPropCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, ipp::SetProp)
    {
      numberProps = 0;
      keys1 = 0;
      keys2 = 0;
      keys3 = 0;
      keys4 = 0;
      propertyValues = 0;
    }
  SetPropCommand(unsigned int tgNum, int inNumberProps,
		 otherKeyType * inKeys1, otherKeyType * inKeys2,
		 otherKeyType * inKeys3, otherKeyType * inKeys4,
		 double * inPropertyValues):
    Command(tgNum, CommandTag, ipp::SetProp)
    {
      int n;
      keys1 = new otherKeyType[inNumberProps];
      keys2 = new otherKeyType[inNumberProps];
      keys3 = new otherKeyType[inNumberProps];
      keys4 = new otherKeyType[inNumberProps];
      propertyValues = new double[inNumberProps];
      for (n = 0; n < inNumberProps; n++)
	{
	  keys1[n] = inKeys1[n];
	  keys2[n] = inKeys2[n];
	  keys3[n] = inKeys3[n];
	  keys4[n] = inKeys4[n];
	  propertyValues[n] = inPropertyValues[n];
	}
      numberProps = inNumberProps;
    }
  ~SetPropCommand()
    {
      delete keys1;
      delete keys2;
      delete keys3;
      delete keys4;
      delete propertyValues;
    }

  int setNewProps(int inNumberProps)
    {
      if (inNumberProps > 0)
	{
	  delete keys1; // OK if NULL
	  delete keys2;
	  delete keys3;
	  delete keys4;
	  delete propertyValues;
	  numberProps = inNumberProps;
	  keys1 = new otherKeyType[inNumberProps];
	  keys2 = new otherKeyType[inNumberProps];
	  keys3 = new otherKeyType[inNumberProps];
	  keys4 = new otherKeyType[inNumberProps];
	  propertyValues = new double[inNumberProps];
	}
      return (inNumberProps > 0);
    }
  int setNumberProps(int inNumberProps)
    {
      if (inNumberProps > 0)
	numberProps = inNumberProps;
      return (inNumberProps > 0);
    }
  int setKey1(otherKeyType inKey1, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys1[n] = inKey1;
      return ((n < numberProps) && (n > -1));
    }
  int setKey2(otherKeyType inKey2, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys2[n] = inKey2;
      return ((n < numberProps) && (n > -1));
    }
  int setKey3(otherKeyType inKey3, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys3[n] = inKey3;
      return ((n < numberProps) && (n > -1));
    }
  int setKey4(otherKeyType inKey4, int n)
    {
      if ((n < numberProps) && (n > -1))
	keys4[n] = inKey4;
      return ((n < numberProps) && (n > -1));
    }
  int setValue(double inValue, int n)
    {
      if ((n < numberProps) && (n > -1))
	propertyValues[n] = inValue;
      return ((n < numberProps) && (n > -1));
    }
  
  int getNumberProps() { return numberProps; }
  otherKeyType getKey1(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys1[n] : ErrorKey); }
  otherKeyType getKey2(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys2[n] : ErrorKey); }
  otherKeyType getKey3(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys3[n] : ErrorKey); }
  otherKeyType getKey4(int n){ return (((n < numberProps) && (n > -1)) ?
				       keys4[n] : ErrorKey); }
  double getValue(int n) { return (((n < numberProps) && (n > -1)) ?
				   propertyValues[n] : 0); }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* SetToolCommand class ***********************************************/

class SetToolCommand : public Command {
  
private:
  char * toolName;
  
public:
  SetToolCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, SetTool){}
  SetToolCommand(unsigned int tgNum, char * inToolName):
    Command(tgNum, CommandTag, SetTool){ toolName = inToolName; }
  ~SetToolCommand(){}
  
  void setToolName(char * aToolName){ toolName = aToolName; }

  char * getToolName() { return toolName; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* StopDaemonCommand class *********************************************

The tag type of the daemon stopped by StopDaemon is EventTag.

*/

class StopDaemonCommand : public Command {
  
private:
  Tag daemonTag;
  
public:
  StopDaemonCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, StopDaemon){}
  StopDaemonCommand(unsigned int tgNum, unsigned int inDaemonTagNumber):
    Command(tgNum, CommandTag, StopDaemon)
    {
      daemonTag.setTagNumber(inDaemonTagNumber);
      daemonTag.setTagType(EventTag);
    }
  ~StopDaemonCommand(){}
  
  void setDaemonTagNumber(unsigned int inDaemonTagNumber)
    { daemonTag.setTagNumber(inDaemonTagNumber); }
  void setDaemonTagType() { daemonTag.setTagType(EventTag); }
  
  unsigned int getDaemonTagNumber(){ return daemonTag.getTagNumber(); }
  tagIdType getDaemonTagType(){ return daemonTag.getTagType(); }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* TiltCenterPartCommand class ****************************************/

class TiltCenterPartCommand : public Command {
  
private:
  double px1;
  double py1;
  double pz1;
  double px2;
  double py2;
  double pz2;
  double limit;

public:
  TiltCenterPartCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, TiltCenterPart){}
  TiltCenterPartCommand(unsigned int tgNum, double inPx1, double inPy1,
			double inPz1, double inPx2, double inPy2,
			double inPz2, double inLimit):
    Command(tgNum, CommandTag, TiltCenterPart)
    {
      px1 = inPx1;
      py1 = inPy1;
      pz1 = inPz1;
      px2 = inPx2;
      py2 = inPy2;
      pz2 = inPz2;
      limit = inLimit;
    }
  ~TiltCenterPartCommand(){}

  void setPx1(double inPx1){ px1 = inPx1; }
  void setPy1(double inPy1){ py1 = inPy1; }
  void setPz1(double inPz1){ pz1 = inPz1; }
  void setPx2(double inPx2){ px2 = inPx2; }
  void setPy2(double inPy2){ py2 = inPy2; }
  void setPz2(double inPz2){ pz2 = inPz2; }
  void setLimit(double inLimit){ limit = inLimit; }

  double getPx1(){ return px1; }
  double getPy1(){ return py1; }
  double getPz1(){ return pz1; }
  double getPx2(){ return px2; }
  double getPy2(){ return py2; }
  double getPz2(){ return pz2; }
  double getLimit(){ return limit; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* TiltPartCommand class **********************************************/

class TiltPartCommand : public Command {
  
private:
  double dx;
  double dy;
  double dz;
  double limit;

public:
  TiltPartCommand(unsigned int tgNum):
    Command(tgNum, CommandTag, TiltPart){}
  TiltPartCommand(unsigned int tgNum,
		  double inDx, double inDy, double inDz, double inLimit):
    Command(tgNum, CommandTag, TiltPart)
    {
      dx = inDx;
      dy = inDy;
      dz = inDz;
      limit = inLimit;
    }
  ~TiltPartCommand(){}

  void setDx(double inDx){ dx = inDx; }
  void setDy(double inDy){ dy = inDy; }
  void setDz(double inDz){ dz = inDz; }
  void setLimit(double inLimit){ limit = inLimit; }

  double getDx(){ return dx; }
  double getDy(){ return dy; }
  double getDz(){ return dz; }
  double getLimit(){ return limit; }
  char * getCommandStringT(char * buffer, char * tagString);
};

/* Response classes ***************************************************/

/* Response class ******************************************************

This is the top-level parent type for all response classes. It is not
intended to be instantiated.

The XYZResponse class, which is derived from this Response class, has
derived classes of its own and is also not intended to be instantiated.
All other derived classes of Response are intended to be instantiated.

*/

class Response {

private:
  responseNameType responseName;

protected:
  Tag * tag;

public:
  Response(unsigned int inTagNumber, tagIdType inTagType,
	   responseNameType inResponseName)
    {
      tag = new Tag(inTagNumber, inTagType);
      responseName = inResponseName;
    }
  ~Response(){ delete tag; }
  
  void setResponseName(responseNameType inResponseName)
    { responseName = inResponseName; }

  responseNameType getResponseName(){ return responseName; }
  Tag *	getTag(){ return tag; }
  const char * getKeyString(int key)
        { return (((key < 0) || (key > 60)) ? "bug" : ippKeyStrings[key]); }
  char * getResponseString(char * buffer, int n);
};

/* XYZResponse class ***************************************************

This is the parent class for PtMeasOrGetResponse, PtMeasEResponse,
GoToEResponse, and OnMoveReportEResponse. This class is not intended
to be instantiated.  The first three of the derived classes have the
additional data members ER and IJK.  PtMeasOrGetResponse uses a
CommandTag, while the other three (which have "E" in their names) use
an EventTag. PtMeasEResponse and GoToEResponse always use E0000.

*/

class XYZResponse : public Response {

private:

  bool hasR;
  bool hasToolA;
  bool hasToolB;
  bool hasToolC;
  bool hasX;
  bool hasY;
  bool hasZ;
  
  double r;
  double toolA;
  double toolB;
  double toolC;
  double x;
  double y;
  double z;

public:
  XYZResponse(unsigned int inTagNumber, tagIdType inTagType, 
	      responseNameType inResponseName):
    Response(inTagNumber, inTagType, inResponseName){}
  XYZResponse(unsigned int inTagNumber, tagIdType inTagType,
	      responseNameType inResponseName, bool inHasR,
	      bool inHasToolA, bool inHasToolB, bool inHasToolC,
	      bool inHasX, bool inHasY, bool inHasZ,
	      double inR, double inToolA, double inToolB, double inToolC,
	      double inX, double inY, double inZ):
    Response(inTagNumber, inTagType, inResponseName)
    {
      hasR = inHasR;
      hasToolA = inHasToolA;
      hasToolB = inHasToolB;
      hasToolC = inHasToolC;
      hasX = inHasX;
      hasY = inHasY;
      hasZ = inHasZ;

      r = inR;
      toolA = inToolA;
      toolB = inToolB;
      toolC = inToolC;
      x = inX;
      y = inY;
      z = inZ;
    }
  ~XYZResponse(){}

  void setHasR(bool inHasR){ hasR = inHasR; }
  void setHasToolA(bool inHasToolA){ hasToolA = inHasToolA; }
  void setHasToolB(bool inHasToolB){ hasToolB = inHasToolB; }
  void setHasToolC(bool inHasToolC){ hasToolC = inHasToolC; }
  void setHasX(bool inHasX){ hasX = inHasX; }
  void setHasY(bool inHasY){ hasY = inHasY; }
  void setHasZ(bool inHasZ){ hasZ = inHasZ; }

  void setR(double inR){ r = inR; }
  void setToolA(double inToolA){ toolA = inToolA; }
  void setToolB(double inToolB){ toolB = inToolB; }
  void setToolC(double inToolC){ toolC = inToolC; }
  void setX(double inX){ x = inX; }
  void setY(double inY){ y = inY; }
  void setZ(double inZ){ z = inZ; }

  bool   getHasR()     { return hasR; }
  bool   getHasToolA() { return hasToolA; }
  bool   getHasToolB() { return hasToolB; }
  bool   getHasToolC() { return hasToolC; }
  bool   getHasX()     { return hasX; }
  bool   getHasY()     { return hasY; }
  bool   getHasZ()     { return hasZ; }

  double getR()        { return r; }
  double getToolA()    { return toolA; }
  double getToolB()    { return toolB; }
  double getToolC()    { return toolC; }
  double getX()        { return x; }
  double getY()        { return y; }
  double getZ()        { return z; }

  char * getResponseStringEnd(char * buffer, int k, int closeParen);
};

/* AckResponse class ****************************************************

Use this to acknowledge all commands.

*/

class AckResponse : public Response {

public:
  AckResponse(unsigned int inTagNumber, tagIdType inTagType):
    Response(inTagNumber, inTagType, Ack){}
  ~AckResponse(){}
};

/* AlignResponse class ************************************************

Use this for data from AlignTool or AlignPart.

In the case of AlignPart, the spec is not clear whether both p (part)
and m (machine) vectors should be returned in the data. Here we assume
that only part vectors are returned. 

If an AlignPartCommand has two vectors, the response gives one. In the
command, the first vector is a vector on the part (in part coordinates)
and the second vector is a vector in machine coordinates. Executing
the command is supposed to rotate a rotary table (to which the part is
fastened) so that the vector on the part points in the direction given
by the vector in machine coordinates.  The returned data gives the
direction in machine coordinates in which the part vector is actually
pointing after the rotation is finished.  If the rotation achieves
perfect alignment, the vector in the response will have the same
values as what the command gave for the vector in machine coordinates.

If an AlignPartCommand has four vectors, they are pairs of part vectors and
machine vectors. The combined rotations of two rotary tables are supposed
to align both pairs. The response in this case has two vectors. The first
gives the actual final direction of the first part vector. The second
gives the actual final direction of the second part vector.

*/

class AlignResponse : public Response {

private:
  double i1;
  double j1;
  double k1;
  double i2;
  double j2;
  double k2;
  bool   has2;
public:
  AlignResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataAlign){}
  AlignResponse(unsigned int inTagNumber, double inI1, double inJ1,
		double inK1, double inI2, double inJ2, double inK2,
		bool inHas2):
    Response(inTagNumber, CommandTag, DataAlign)
    {
      i1 = inI1;
      j1 = inJ1;
      k1 = inK1;
      i2 = inI2;
      j2 = inJ2;
      k2 = inK2;
      has2 = inHas2;
    }
  ~AlignResponse(){}

  void setI1(double inI1){ i1 = inI1; }
  void setJ1(double inJ1){ j1 = inJ1; }
  void setK1(double inK1){ k1 = inK1; }
  void setI2(double inI2){ i2 = inI2; }
  void setJ2(double inJ2){ j2 = inJ2; }
  void setK2(double inK2){ k2 = inK2; }
  void setHas2(bool inHas2){ has2 = inHas2; }

  double getI1(){ return i1; }
  double getJ1(){ return j1; }
  double getK1(){ return k1; }
  double getI2(){ return i2; }
  double getJ2(){ return j2; }
  double getK2(){ return k2; }
  bool getHas2(){ return has2; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* CenterPartResponse class **********************************************

Use this for data from CenterPart.

*/

class CenterPartResponse : public Response {

private:
  bool  centered;

public:
  CenterPartResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataCenterPart){}
  CenterPartResponse(unsigned int inTagNumber, bool inCentered):
    Response(inTagNumber, CommandTag, DataCenterPart){ centered = inCentered; }
  ~CenterPartResponse(){}

  void setCentered(bool inCentered){ centered = inCentered; }
  
  bool   getCentered(){ return centered; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* ChangeToolEResponse class **********************************************

Use this for data from a server-initiated ChangeTool.
The tag number must be 0, so the constructors do not take a tag number
argument.

*/

class ChangeToolEResponse : public Response {

private:
  char * toolName;

public:
  ChangeToolEResponse():
    Response(0, EventTag, DataChangeToolE){}
  ChangeToolEResponse(char * inToolName):
    Response(0, EventTag, DataChangeToolE){ toolName = inToolName; }
  ~ChangeToolEResponse(){}

  void setToolName(char * inToolName){ toolName = inToolName; }

  char * getToolName() { return toolName; }

  char * getResponseStringT(char * buffer);
};

/* CompleteResponse class ***********************************************

Use this for completion of all commands.

*/

class CompleteResponse : public Response {

public:
  CompleteResponse(unsigned int inTagNumber, tagIdType inTagType):
    Response(inTagNumber, inTagType, Complete){}
  ~CompleteResponse(){}
};

/* GetDMEVersionResponse class *******************************************

Use this for data from GetDMEVersion.

*/

class GetDMEVersionResponse : public Response {

private:
  char * version;

public:
  GetDMEVersionResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataGetDMEVersion){}
  GetDMEVersionResponse(unsigned int inTagNumber, char * inVersion):
    Response(inTagNumber, CommandTag, DataGetDMEVersion){ version=inVersion; }
  ~GetDMEVersionResponse(){}

  void setVersion(char * inVersion){ version = inVersion; }

  char * getVersion() { return version; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* EnumPropResponse class ***********************************************

Use this (possibly repeatedly) for data from EnumAllProp and EnumProp.

*/

class EnumPropResponse : public Response {

private: 
  char * memberName;
  char * valueTypeName;

public:
  EnumPropResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataEnumProp){}
  EnumPropResponse(unsigned int inTagNumber, char * inMemberName,
		   char * inValueTypeName):
    Response(inTagNumber, CommandTag, DataEnumProp)
    {
      memberName = inMemberName;
      valueTypeName = inValueTypeName;
    }
  ~EnumPropResponse(){}

  void setMemberName(char * inMemberName){ memberName = inMemberName; }
  void setValueTypeName(char * inValueTypeName)
    { valueTypeName = inValueTypeName; }

  char * getMemberName(){ return memberName; }
  char * getValueTypeName(){ return valueTypeName; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* ErrorResponse class **************************************************

Use this for all errors.

The destructor must not (and does not) delete theError.

*/

class ErrorResponse : public Response {

private: 
  IppError * theError;
  char * errorCausingMethod;

public:
  ErrorResponse(unsigned int inTagNumber, tagIdType inTagType):
    Response(inTagNumber, inTagType, ErrorError){}
  ErrorResponse(unsigned int inTagNumber, tagIdType inTagType,
		int inErrorName, char * inErrorCausingMethod);
  ~ErrorResponse(){}

  void setTheError(IppError * inTheError){ theError = inTheError; }
  void setCausingMethod(char * inErrorCausingMethod)
    { errorCausingMethod = inErrorCausingMethod; }
  IppError * getTheError(){ return theError; }
  char * getCausingMethod(){ return errorCausingMethod; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* GetChangeToolActionResponse class ************************************

Use this for data from GetChangeToolAction.

*/

class GetChangeToolActionResponse : public Response {

private:
  changeToolActionType changeType;
  double x;
  double y;
  double z;

public:
  GetChangeToolActionResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataGetChangeToolAction){}
  GetChangeToolActionResponse(unsigned int inTagNumber,
			      changeToolActionType inChangeType,
			      double inX, double inY, double inZ):
    Response(inTagNumber, CommandTag, DataGetChangeToolAction)
    {
      changeType = inChangeType;
      x = inX;
      y = inY;
      z = inZ;
    }
  ~GetChangeToolActionResponse(){}

  void setChangeType(changeToolActionType inChangeType)
    { changeType = inChangeType; }
  void setX(double inX){ x = inX; }
  void setY(double inY){ y = inY; }
  void setZ(double inZ){ z = inZ; }

  double getChangeType(){ return changeType; }
  double getX()         { return x; }
  double getY()         { return y; }
  double getZ()         { return z; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* GetCoordSystemResponse class *****************************************

Use this for data from GetCoordSystem.

*/

class GetCoordSystemResponse : public Response {

private:
  coordSysType coordSys;

public:
  GetCoordSystemResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataGetCoordSystem){}
  GetCoordSystemResponse(unsigned int inTagNumber, coordSysType inCoordSys):
    Response(inTagNumber, CommandTag, DataGetCoordSystem)
    { coordSys = inCoordSys; }
  ~GetCoordSystemResponse(){}

  void setCoordSys(coordSysType inCoordSys){ coordSys = inCoordSys; }

  coordSysType getCoordSys(){ return coordSys; }
  char * getResponseStringT(char * buffer, char * tagString);
};


/* GetCsyTransformationResponse class ************************************

Use this for data from GetCsyTransformation. The response does not identify
the coordinate system. Release 2.1 mistakenly included that.

*/

class GetCsyTransformationResponse : public Response
{  
private:
  double        X0;
  double        Y0;
  double        Z0;
  double        Theta;
  double        Psi;
  double        Phi;
public:
  GetCsyTransformationResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataGetCsyTransformation){}
  GetCsyTransformationResponse(unsigned int inTagNumber,
			       double inX0, double inY0, double inZ0,
			       double inTheta, double inPsi, double inPhi):
    Response(inTagNumber, CommandTag, DataGetCsyTransformation)
    { 
      X0 = inX0;
      Y0 = inY0;
      Z0 = inZ0;
      Theta = inTheta;
      Psi = inPsi;
      Phi = inPhi;
    }
  ~GetCsyTransformationResponse(){}

  void setX0(double inX0){ X0 = inX0; }
  void setY0(double inY0){ Y0 = inY0; }
  void setZ0(double inZ0){ Z0 = inZ0; }
  void setTheta(double inTheta){ Theta = inTheta; }
  void setPsi(double inPsi){ Psi = inPsi; }
  void setPhi(double inPhi){ Phi = inPhi; }
 
  double         getX0() {return X0;}
  double         getY0() {return Y0;}
  double         getZ0() {return Z0;}
  double         getTheta() {return Theta;}
  double         getPsi() {return Psi;}
  double         getPhi() {return Phi;}
  char *         getResponseStringT(char * buffer, char * tagString);
};

/* GetErrStatusEResponse class *****************************************

Use this for data from GetErrStatusE.

*/

class GetErrStatusEResponse : public Response {

private: 
  bool   isErred;

public:
  GetErrStatusEResponse(unsigned int inTagNumber):
    Response(inTagNumber, EventTag, DataGetErrStatusE){}
  GetErrStatusEResponse(unsigned int inTagNumber, bool inIsErred):
    Response(inTagNumber, EventTag, DataGetErrStatusE){ isErred = inIsErred; }
  ~GetErrStatusEResponse(){}

  void setIsErred(bool inIsErred){ isErred = inIsErred; }

  bool   getIsErred(){ return isErred; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* GetMachineClassResponse class ***************************************

Use this for data from GetMachineClass.

*/

class GetMachineClassResponse : public Response {

private:
  char className[24];

public:
  GetMachineClassResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataGetMachineClass)
    { className[0] = 0; }
  GetMachineClassResponse(unsigned int inTagNumber, char * inClassName):
    Response(inTagNumber, CommandTag, DataGetMachineClass)
    {
      if ((strcmp(inClassName, "CartCMM") == 0) ||
	  (strcmp(inClassName, "CartCMMWithRotaryTable") == 0))
	strcpy(className, inClassName);
      else
	className[0] = 0;
    }
  ~GetMachineClassResponse(){}

  char * setClassName(char * inClassName)
    {
      if ((strcmp(inClassName, "CartCMM") == 0) ||
	  (strcmp(inClassName, "CartCMMWithRotaryTable") == 0))
	strcpy(className, inClassName);
      else
	className[0] = 0;
      return className;
    }

  char * getClassName(){ return className; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* GetPropResponse class **********************************************

Use this for data from GetProp, GetPropE

The command format for these two commands allows several properties
to be requested or set in a single command. The spec says nothing explicit
about the number of returned property values per message, but shows
two properties in a single response in the GetProp example on page
73. This seems to imply that the number of properties in the response
should be the same as the number in the command. This response allows
multiple properties to be returned.

See documentation of GetPropCommand.

The format of a property differs according to property. To handle this,
GetPropResponse contains an array of OneProperty's and the OneProperty
class has several derived classes. Each derived class knows how to
print itself.

*/

class GetPropResponse : public Response {

private:
  int            numberProps; // size of oneProps array
  OneProperty ** oneProps;    // array of pointers to OneProperty

public:
  GetPropResponse(unsigned int inTagNumber, tagIdType inTagType):
    Response(inTagNumber, inTagType, DataGetProp)
    {
      numberProps = 0;
      oneProps = NULL;
    }
  GetPropResponse(unsigned int inTagNumber, tagIdType inTagType,
		   int inNumberProps, OneProperty ** inOneProps):
    Response(inTagNumber, inTagType, DataGetProp)
    {
      numberProps = inNumberProps;
      oneProps = inOneProps;
    }
  ~GetPropResponse(){ delete oneProps; }

  void setNumberProps(int inNumberProps)
    {
      if (inNumberProps > 0)
	numberProps = inNumberProps;
    }
  void setOneProp(OneProperty * inOneProp, int n)
    {
      if ((n < numberProps) && (n > -1))
	oneProps[n] = inOneProp;
    }
  OneProperty * getOneProp(int n)
    { return (((n < numberProps) && (n > -1)) ? oneProps[n] : NULL); }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* GoToEResponse class ***************************************************

Use this for server-initiated GoTo data.

The tag number must be 0, so the constructors do not take a tag number
argument.

If hasIJK is true, the value of the IJK vector must be (0,0,0), so the
longer constructor does not take inI, inJ, or inK.

See documentation of XYZResponse.

*/

class GoToEResponse : public XYZResponse {

private:
  bool hasER;
  bool hasIJK;

  double theER;

public:
  GoToEResponse():
    XYZResponse(0, EventTag, DataGoToE){}
  GoToEResponse(bool inHasER, bool inHasIJK, bool inHasR,
		bool inHasToolA, bool inHasToolB, bool inHasToolC,
		bool inHasX, bool inHasY, bool inHasZ, double inTheER,
		double inR, double inToolA, double inToolB, double inToolC,
		double inX, double inY, double inZ):
    XYZResponse(0, EventTag, DataGoToE, inHasR,
		inHasToolA, inHasToolB, inHasToolC, inHasX, inHasY, inHasZ,
		inR, inToolA, inToolB, inToolC, inX, inY, inZ)
    {
      hasER = inHasER;
      hasIJK = inHasIJK;
      
      theER = inTheER;
    }
  ~GoToEResponse(){}

  void setHasER(bool inHasER){ hasER = inHasER; }
  void setHasIJK(bool inHasIJK){ hasIJK = inHasIJK; }
  void setTheER(double inTheER){ theER = inTheER; }

  bool   getHasER() { return hasER; }
  bool   getHasIJK(){ return hasIJK; }
  double getTheER() { return theER; }

  char * getResponseStringT(char * buffer);
};

/* IsHomedResponse class ***********************************************

Use this for data from IsHomed.

*/

class IsHomedResponse : public Response {

private:
  bool  homed;

public:
  IsHomedResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataIsHomed){}
  IsHomedResponse(unsigned int inTagNumber, bool inHomed):
    Response(inTagNumber, CommandTag, DataIsHomed){ homed = inHomed; }
  ~IsHomedResponse(){}

  void setHomed(bool inHomed){ homed = inHomed; }
  
  bool   getHomed(){ return homed; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* IsUserEnabledResponse class *****************************************

Use this for data from IsUserEnabled.

*/

class IsUserEnabledResponse : public Response {

private: 
  bool   enabled;

public:
  IsUserEnabledResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataIsUserEnabled){}
  IsUserEnabledResponse(unsigned int inTagNumber, bool inEnabled):
    Response(inTagNumber, CommandTag, DataIsUserEnabled)
    { enabled = inEnabled; }
  ~IsUserEnabledResponse(){}

  void setEnabled(bool inEnabled){ enabled = inEnabled; }

  bool getEnabled(){ return enabled; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* KeyPressEResponse class **********************************************

Use this for a server-initiated key press response.
The tag number must be 0, so the constructors do not take a tag number
argument.

*/

class KeyPressEResponse : public Response {

private:
  char * keyName;

public:
  KeyPressEResponse():
    Response(0, EventTag, DataKeyPressE){}
  KeyPressEResponse(char * inKeyName):
    Response(0, EventTag, DataKeyPressE){ keyName = inKeyName; }
  ~KeyPressEResponse(){}

  void setKeyName(char * inKeyName){ keyName = inKeyName; }

  char * getKeyName() { return keyName; }
  char * getResponseStringT(char * buffer);
};

/* OnMoveReportEResponse class *******************************************

Use this for OnMoveReportE data.
See documentation of XYZResponse.

*/

class OnMoveReportEResponse : public XYZResponse {

public:
  OnMoveReportEResponse(unsigned int inTagNumber):
    XYZResponse(inTagNumber, EventTag, DataOnMoveReportE){}
  OnMoveReportEResponse(unsigned int inTagNumber,
		  bool inHasR, bool inHasToolA, bool inHasToolB,
		  bool inHasToolC, bool inHasX, bool inHasY, bool inHasZ,
		  double inR, double inToolA, double inToolB, double inToolC,
		  double inX, double inY, double inZ):
    XYZResponse(inTagNumber, EventTag, DataOnMoveReportE, inHasR,
		inHasToolA, inHasToolB, inHasToolC, inHasX, inHasY, inHasZ,
		inR, inToolA, inToolB, inToolC, inX, inY, inZ){}
  ~OnMoveReportEResponse(){}
  char * getResponseStringT(char * buffer, char * tagString);
};

/* PtMeasOrGetResponse class **************************************************

Use this for data resulting from a PtMeas or Get command.
See documentation of XYZResponse.

In parsing a response, a response to a Get command cannot be distinguished
from a response to a PtMeas command.

Of course, if the response contains an ER or an IJK it must be the response
to a PtMeas command. It would be possible to define a PtMeasReponse class as
a derived class of this, but then sometimes a response to a PtMeas command
would be recognized as being in the PtMeasReponse class and sometimes it
could only be determined that it is in the PtMeasOrGetResponse class.
This would be a nuisance, so the PtMeasReponse class has not been defined.

*/

class PtMeasOrGetResponse : public XYZResponse {

private:
  bool hasER;
  bool hasIJK;

  double theER;
  TargetVector theIJK;

public:
  PtMeasOrGetResponse(unsigned int inTagNumber):
    XYZResponse(inTagNumber, CommandTag, DataPtMeasOrGet){}
  PtMeasOrGetResponse(unsigned int inTagNumber, bool inHasER, bool inHasIJK,
		      bool inHasR, bool inHasToolA, bool inHasToolB,
		      bool inHasToolC, bool inHasX, bool inHasY, bool inHasZ,
		      double inTheER, double inI, double inJ, double inK,
		      double inR, double inToolA, double inToolB,
		      double inToolC, double inX, double inY, double inZ):
    XYZResponse(inTagNumber, CommandTag, DataPtMeasOrGet, inHasR,
		inHasToolA, inHasToolB, inHasToolC, inHasX, inHasY, inHasZ,
		inR, inToolA, inToolB, inToolC, inX, inY, inZ)
    {
      hasER = inHasER;
      hasIJK = inHasIJK;
      
      theER = inTheER;
      theIJK.setValues(inI, inJ, inK);
    }
  ~PtMeasOrGetResponse(){}
  
  void setHasER(bool inHasER){ hasER = inHasER; }
  void setHasIJK(bool inHasIJK){ hasIJK = inHasIJK; }
  void setTheER(double inTheER){ theER = inTheER; }
  void setIJK(double inI, double inJ, double inK)
    { theIJK.setValues(inI, inJ, inK); }
  
  bool   getHasER()    { return hasER; }
  bool   getHasIJK()   { return hasIJK; }
  double getTheER()    { return theER; }
  double getI()        { return theIJK.getI(); }
  double getJ()        { return theIJK.getJ(); }
  double getK()        { return theIJK.getK(); }
  
  char * getResponseStringT(char * buffer, char * tagString);
};

/* PtMeasEResponse class *************************************************

Use this for server-initiated PtMeas data.
The tag number must be 0, so the constructors do not take a tag number
argument.

See documentation of XYZResponse.

*/

class PtMeasEResponse : public XYZResponse {

private:
  bool hasER;
  bool hasIJK;

  double theER;
  TargetVector theIJK;

public:
  PtMeasEResponse():
    XYZResponse(0, EventTag, DataPtMeasE){}
  PtMeasEResponse(bool inHasER, bool inHasIJK,
		  bool inHasR, bool inHasToolA, bool inHasToolB,
		  bool inHasToolC, bool inHasX, bool inHasY, bool inHasZ,
		  double inTheER, double inI, double inJ, double inK,
		  double inR, double inToolA, double inToolB, double inToolC,
		  double inX, double inY, double inZ):
    XYZResponse(0, EventTag, DataPtMeasE, inHasR,
		inHasToolA, inHasToolB, inHasToolC, inHasX, inHasY, inHasZ,
		inR, inToolA, inToolB, inToolC, inX, inY, inZ)
    {
      hasER = inHasER;
      hasIJK = inHasIJK;

      theER = inTheER;
      theIJK.setValues(inI, inJ, inK);
    }
  ~PtMeasEResponse(){}

  void setHasER(bool inHasER){ hasER = inHasER; }
  void setHasIJK(bool inHasIJK){ hasIJK = inHasIJK; }
  void setTheER(double inTheER){ theER = inTheER; }
  void setIJK(double inI, double inJ, double inK)
    { theIJK.setValues(inI, inJ, inK); }

  bool   getHasER() { return hasER; }
  bool   getHasIJK(){ return hasIJK; }
  double getTheER() { return theER; }
  double getI()     { return theIJK.getI(); }
  double getJ()     { return theIJK.getJ(); }
  double getK()     { return theIJK.getK(); }

  char * getResponseStringT(char * buffer);
};

/* ScanResponse class **************************************************

Use this for data from scan commands.

The data is stuck in one big array of doubles but may be conceptually
grouped into sets (one set for each scanned point). Each set may have
one to ten entries for ER, IJK, Q, R, X, Y, Z, ToolA, ToolB, and
ToolC. The entries are always in the order just given, but not all of
ER ... ToolC are necessarily used.  Only by knowing the contents of
the last OnScanReport command is it possible to make sense of a
ScanResponse.

*/

class ScanResponse : public Response {

private:
  int dataSize;      /* the total number of doubles in the data array */
  double * data;

public:
  ScanResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataScan)
    {
      dataSize = 0;
      data = NULL;
    }
  ScanResponse(unsigned int inTagNumber, int inDataSize, double * inData);
  ~ScanResponse(){ } /* should probably delete data */
  
  void setDataSize(int inDataSize){ dataSize = inDataSize; }
  void setData(int inDataSize, double * inData);
  int getDataSize(){ return dataSize; }
  double * getData(){ return data;}
  char * getResponseStringT(char * buffer, char * tagString);
};

/* SetPropEResponse class ************************************************

Use this for data from a server-initiated SetProp
The tag number must be 0, so the constructors do not take a tag number
argument.

This allows only one property to be set. The spec does not make it
clear whether the argument list may have more than one property.

This allows making a request to set any property that can be set using
SetProp (this excludes Name, Id, CollisionVolume, and Alignment). All
values are numeric.

Key4 may be EmptyKey.

*/

class SetPropEResponse : public Response {

private:
  otherKeyType  key1;
  otherKeyType  key2;
  otherKeyType  key3;
  otherKeyType  key4;
  double        propertyValue;

public:
  SetPropEResponse():
    Response(0, EventTag, DataSetPropE){}
  SetPropEResponse(otherKeyType inKey1, otherKeyType inKey2,
		   otherKeyType inKey3, otherKeyType inKey4,
		   double inPropertyValue):
    Response(0, EventTag, DataSetPropE)
    {    
      propertyValue = inPropertyValue;
      key1 = inKey1;
      key2 = inKey2;
      key3 = inKey3;
      key4 = inKey4;
    }
  ~SetPropEResponse(){}

  void setKey1(otherKeyType inKey1){ key1 = inKey1; }
  void setKey2(otherKeyType inKey2){ key2 = inKey2; }
  void setKey3(otherKeyType inKey3){ key3 = inKey3; }
  void setKey4(otherKeyType inKey4){ key4 = inKey4; }
  void setPropertyValue(double inPropertyValue)
    { propertyValue = inPropertyValue; }

  otherKeyType  getKey1(){ return key1; }
  otherKeyType  getKey2(){ return key2; }
  otherKeyType  getKey3(){ return key3; }
  otherKeyType  getKey4(){ return key4; }
  double        getPropertyValue(){ return propertyValue; }
  char *        getResponseStringT(char * buffer);
};

/* StringResponse class **************************************************

Use this for data from GetErrorInfo and EnumTools.

This returns one string.

*/

class StringResponse : public Response {

private:
  char * data;

public:
  StringResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataString){}
  StringResponse(unsigned int inTagNumber, char * inData):
    Response(inTagNumber, CommandTag, DataString){ data = inData; }
  ~StringResponse(){}

  void setData(char * inData){ data = inData; }

  char * getData() { return data; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* TiltCenterPartResponse class *****************************************

Use this for data from TiltCenterPart.

*/

class TiltCenterPartResponse : public Response {

private:
  bool trued;

public:
  TiltCenterPartResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataTiltCenterPart){}
  TiltCenterPartResponse(unsigned int inTagNumber, bool inTrued):
    Response(inTagNumber, CommandTag, DataTiltCenterPart){ trued = inTrued; }
  ~TiltCenterPartResponse(){}

  void setTrued(bool inTrued){ trued = inTrued; }
  
  bool   getTrued(){ return trued; }
  char * getResponseStringT(char * buffer, char * tagString);
};

/* TiltPartResponse class ***********************************************

Use this for data from TiltPart.

*/

class TiltPartResponse : public Response {

private:
  bool  tilted;

public:
  TiltPartResponse(unsigned int inTagNumber):
    Response(inTagNumber, CommandTag, DataTiltPart){}
  TiltPartResponse(unsigned int inTagNumber, bool inTilted):
    Response(inTagNumber, CommandTag, DataTiltPart){ tilted = inTilted; }
  ~TiltPartResponse(){}

  void setTilted(bool inTilted){ tilted = inTilted; }
  
  bool   getTilted(){ return tilted; }
  char * getResponseStringT(char * buffer, char * tagString);
};


#endif

/* The end **********************************************************/

