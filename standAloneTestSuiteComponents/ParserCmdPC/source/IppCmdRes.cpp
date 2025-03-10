
// Implementation of I++ DME Version 1.1
// Source Code: IppCmdRes.cpp
// Created: 08-5-02 Falco

// Changed to release 1.30: 05-12-03 Kramer
// Added AlignPart, EnumTools, OnScanReport, 10 Scans
// Added A, B, C, Q, R
// Changed printing of all doubles to %.5lf (was 2, 4, 6, or unspecified)
// Reformatted and alphabetized

// Changed to release 1.40: 04-16-04 Kramer

/* #include "../Server/StdAfx.h" for windows PC's */
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "IppCmdRes.h"

/* Command::printLikeGet ************************************************

This must always be the last function that prints into the buffer
because this adds the ending. If the command being printed has any
arguments, there will be a comma at position k-2 when this function
finishes printing arguments, and that comma is removed. The
OnMoveReportECommand is not required to have any arguments.

Called By:
  GetCommand::getCommandStringT
  OnMoveReportECommand::getCommandStringT
  OnPtMeasReportCommand::getCommandStringT
  OnScanReportCommand::getCommandStringT

*/

char * Command::printLikeGet(char * buffer, bool R, bool X, bool Y, bool Z,
			     bool A, bool B, bool C)
{
  int k = 0;

  if (R)
    k = k + sprintf((buffer + k), "R(), ");
  if (X)
    k = k + sprintf((buffer + k), "X(), ");
  if (Y)
    k = k + sprintf((buffer + k), "Y(), ");
  if (Z)
    k = k + sprintf((buffer + k), "Z(), ");
  if (A)
    k = k + sprintf((buffer + k), "Tool.A(), ");
  if (B)
    k = k + sprintf((buffer + k), "Tool.B(), ");
  if (C)
    k = k + sprintf((buffer + k), "Tool.C(), ");
  if (buffer[k - 2] == ',')
    k = (k - 2);
  sprintf((buffer + k), ")%c%c", 13, 10);
  return buffer;
}

/* Tag::setTagType *****************************************************/

void Tag::setTagType(commandNameType commandName)
{
  switch(commandName)
    { 
    case AbortE:
      tagType = EventTag;
      break;
    case GetPropE:
      tagType = EventTag;
      break;
    case OnMoveReportE:
      tagType = EventTag;
      break;
    case GetErrStatusE:
      tagType = EventTag;
      break;
    default:
      tagType = CommandTag;
    }
}

/* Tag::getTagString ****************************************************

For command tags, this allows tags in the range [1, 99999]. For
event tags, this allows [E0000, E9999]. These are the ranges provided
in the spec, pages 35 and 36. If a tag is out of range, buffer is
set to NULL.

The buffer must be at least TAGSTRINGSIZE (6) characters long.

Since the tagNumber is an unsigned int, it does not need to be tested
for being positive. 

*/

char * Tag::getTagString(char * buffer)
{
  if (tagType == CommandTag)
    {
      if ((tagNumber > 0) && (tagNumber < 100000))
	sprintf(buffer, "%05d", tagNumber);
      else
	buffer = NULL;
    }
  else if (tagType == EventTag)
    {
      if (tagNumber < 10000)
	sprintf(buffer, "E%04d", tagNumber);
      else
	buffer = NULL;
    }
  else
    buffer = NULL;
  return buffer;
}

/* TargetVector::getVectorString ***************************************/

char * TargetVector::getVectorString(char * buffer, int n)
{
  static char answer[100];
  int k;

  if (isSet == true)
    {
      k = sprintf(answer, "%.5lf,%.5lf,%.5lf", value[0], value[1], value[2]);
      if (n > k)
	strcpy(buffer, answer);
      else
	buffer = NULL;
    }
  else
    buffer = NULL;
  return buffer;
}

/* getCommandString *****************************************************

This prints the command string into the "answer" array first, then
copies into the buffer if the buffer is large enough.

If the commandName is not recognized or if the buffer is too small,
this returns NULL. Otherwise, it returns buffer.

The definitions of getCommandStringT for the various classes follow,
arranged alphabetically by class. None of the getCommandStringT
methods check that the "answer" array is large enough to hold the
command. This check could be added, but adding it would be
tedious. The answer array holds IPPSIZE (65536) characters.

*/

char * Command::getCommandString(char * buffer, int bufferSize)
{
  static char answer[IPPSIZE];
  static char tagString[TAGSTRINGSIZE];
  char * result;

  if (buffer && (tag->getTagString(tagString)))
    {
      switch(commandName)
	{
	case AbortE:
	case ClearAllErrors:
	case DisableUser:
	case EnableUser:
	case EndSession:
	case EnumTools:
 	case GetCoordSystem:
	case GetDMEVersion:
	case GetErrStatusE:
	case GetMachineClass:
	case GetXtdErrStatus:
	case Home:
	case IsHomed:
	case IsUserEnabled:
	case ReQualify:
	case StartSession:
	case StopAllDaemons:
	  sprintf(answer, "%s %s()%c%c",
		  tagString, ippCommandNameStrings[commandName], 13, 10);
	  result = answer;
	  break;
	case AlignPart:
	  result = ((AlignPartCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case AlignTool:
	  result = ((AlignToolCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case CenterPart:
	  result = ((CenterPartCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ChangeTool:
	  result = ((ChangeToolCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case EnumAllProp:
	  result = ((EnumAllPropCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case EnumProp:
	  result = ((EnumPropCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case FindTool:
	  result = ((FindToolCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case Get:
	  result = ((GetCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case GetChangeToolAction:
	  result = ((GetChangeToolActionCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case GetCsyTransformation:
	  result = ((GetCsyTransformationCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ipp::GetErrorInfo:
	  result = ((GetErrorInfoCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ipp::GetProp:
	  result = ((GetPropCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case GetPropE:
	  result = ((GetPropECommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case GoTo:
	  result = ((GoToCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case LockAxis:
	  result = ((LockAxisCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case OnMoveReportE:
	  result = ((OnMoveReportECommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case OnPtMeasReport:
	  result = ((OnPtMeasReportCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case OnScanReport:
	  result = ((OnScanReportCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case PtMeas:
	  result = ((PtMeasCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ScanInCylEndIsPlane:
	  result = ((ScanInCylEndIsPlaneCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ScanInCylEndIsSphere:
	  result = ((ScanInCylEndIsSphereCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ScanInPlaneEndIsCyl:
	  result = ((ScanInPlaneEndIsCylCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ScanInPlaneEndIsPlane:
	  result = ((ScanInPlaneEndIsPlaneCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ScanInPlaneEndIsSphere:
	  result = ((ScanInPlaneEndIsSphereCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ScanOnCircle:
	  result = ((ScanOnCircleCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ScanOnCircleHint:
	  result = ((ScanOnCircleHintCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ScanOnLine:
	  result = ((ScanOnLineCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ScanOnLineHint:
	  result = ((ScanOnLineHintCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ScanUnknownHint:
	  result = ((ScanUnknownHintCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case SetCoordSystem:
	  result = ((SetCoordSystemCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case SetCsyTransformation:
	  result = ((SetCsyTransformationCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case ipp::SetProp:
	  result = ((SetPropCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case SetTool:
	  result = ((SetToolCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case StopDaemon:
	  result = ((StopDaemonCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case TiltCenterPart:
	  result = ((TiltCenterPartCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	case TiltPart:
	  result = ((TiltPartCommand *)this)->
	    getCommandStringT(answer, tagString);
	  break;
	default:
	  result = NULL;
	}
    }
  if (result && (bufferSize > (int)strlen(answer)))
    strcpy(buffer, answer);
  else
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * AlignPartCommand::getCommandStringT(char * buffer, char * tagString)
{
  static char aString[100];
  static char bString[100];
  int k;

  if ((partVector1.getVectorString(aString, 99)) &&
      (machineVector1.getVectorString(bString, 99)))
    {
      k = sprintf(buffer, "%s AlignPart(%s,%s,", tagString, aString, bString);
      if (has2)
	{
	  if ((partVector2.getVectorString(aString, 99)) &&
	      (machineVector2.getVectorString(bString, 99)))
	    sprintf((buffer + k), "%s,%s,%.5lf,%.5lf)%c%c", aString, bString,
		    alpha, beta, 13, 10);
	  else
	    buffer = NULL;
	}
      else
	sprintf((buffer + k), "%.5lf)%c%c", alpha, 13, 10);
    }
  else
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * AlignToolCommand::getCommandStringT(char * buffer, char * tagString)
{
  static char aString[100];
  int k;
  
  if (vector1.getVectorString(aString, 99))
    {
      k = sprintf(buffer, "%s AlignTool(%s,", tagString, aString);
      if (has2)
	{
	  if (vector2.getVectorString(aString, 99))
	    sprintf((buffer + k), "%s,%.5lf,%.5lf)%c%c", aString,
		    alpha, beta, 13, 10);
	  else
	    buffer = NULL;
	}
      else
	sprintf((buffer + k), "%.5lf)%c%c", alpha, 13, 10);
    }
  else
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * CenterPartCommand::getCommandStringT(char * buffer, char * tagString)
{
  sprintf(buffer, "%s CenterPart(%.5lf, %.5lf, %.5lf, %.5lf)%c%c",
	  tagString, px, py, pz, limit, 13, 10);
  return buffer;
}

/*********************************************************************/

char * ChangeToolCommand::getCommandStringT(char * buffer, char * tagString)
{
  if (toolName)
    sprintf(buffer, "%s ChangeTool(\"%s\")%c%c", tagString, toolName, 13, 10);
  else
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * EnumAllPropCommand::getCommandStringT(char * buffer, char * tagString)
{
  int k;
  
  k = sprintf(buffer, "%s EnumAllProp(%s", tagString,
	      getKeyString((int)key1));
  if (numberKeys > 1)
    {
      k = k + sprintf((buffer + k), ".%s",
		      getKeyString((int)key2));
      if (numberKeys > 2)
	{
	  k = k + sprintf((buffer + k), ".%s",
			  getKeyString((int)key3));
	  if (numberKeys > 3)
	    {
	      k = k + sprintf((buffer + k), ".%s",
			      getKeyString((int)key4));
	    }
	}
    }
  sprintf((buffer + k), "())%c%c", 13, 10);
  return buffer;
}

/*********************************************************************/

char * EnumPropCommand::getCommandStringT(char * buffer, char * tagString)
{
  int k;
  
  k = sprintf(buffer, "%s EnumProp(%s", tagString,
	      getKeyString((int)key1));
  if (numberKeys > 1)
    {
      k = k + sprintf((buffer + k), ".%s",
		      getKeyString((int)key2));
      if (numberKeys > 2)
	{
	  k = k + sprintf((buffer + k), ".%s",
			  getKeyString((int)key3));
	  if (numberKeys > 3)
	    {
	      k = k + sprintf((buffer + k), ".%s",
			      getKeyString((int)key4));
	    }
	}
    }
  sprintf((buffer + k), "())%c%c", 13, 10);
  return buffer;
}

/*********************************************************************/

char * FindToolCommand::getCommandStringT(char * buffer, char * tagString)
{
  if (toolName)
    sprintf(buffer, "%s FindTool(\"%s\")%c%c", tagString, toolName, 13, 10);
  else
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * GetCommand::getCommandStringT(char * buffer, char * tagString)
{
  int k;
  
  k = sprintf(buffer, "%s Get(", tagString);
  return printLikeGet((buffer + k), hasR, hasX, hasY, hasZ,
		      hasToolA, hasToolB, hasToolC);
}

/*********************************************************************/

char * GetChangeToolActionCommand::getCommandStringT(char * buffer,
						     char * tagString)
{
  if (toolName)
    sprintf(buffer, "%s GetChangeToolAction(\"%s\")%c%c",
	    tagString, toolName, 13, 10);
  else
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * GetCsyTransformationCommand::getCommandStringT(char * buffer,
						      char * tagString)
{
  sprintf(buffer, "%s GetCsyTransformation(%s)%c%c",
	  tagString, getKeyString((int)coordSys), 13, 10);
  return buffer;
}

/*********************************************************************/

char * GetErrorInfoCommand::getCommandStringT(char * buffer, char * tagString)
{
  sprintf(buffer, "%s GetErrorInfo(%04d)%c%c", tagString, errorNumber, 13, 10);
  return buffer;
}

/***********************************************************************/

char * GetPropCommand::getCommandStringT(char * buffer, char * tagString)
{
  int k;
  int m;

  k = sprintf(buffer, "%s GetProp(", tagString);
  for (m = 0; m < numberProps; m++)
    {
      k = (k + sprintf((buffer + k), "%s.%s",
		       getKeyString((int)keys1[m]),
		       getKeyString((int)keys2[m])));
      if (keys3[m])
	{
	  k = (k + sprintf((buffer + k), ".%s", getKeyString((int)keys3[m])));
	  if (keys4[m])
	    k = k + sprintf((buffer + k), ".%s", getKeyString((int)keys4[m]));
	}
      k = (k + sprintf((buffer + k), "(), "));
    }
  if (buffer[k - 2] == ',')
    sprintf((buffer + (k - 2)), ")%c%c", 13, 10);
  else // no properties
    buffer = NULL;
  return buffer;
}

/***********************************************************************/

char * GetPropECommand::getCommandStringT(char * buffer, char * tagString)
{
  int k;
  int m;

  k = sprintf(buffer, "%s GetPropE(", tagString);
  for (m = 0; m < numberProps; m++)
    {
      k = (k + sprintf((buffer + k), "%s.%s",
		       getKeyString((int)keys1[m]),
		       getKeyString((int)keys2[m])));
      if (keys3[m])
	{
	  k = (k + sprintf((buffer + k), ".%s", getKeyString((int)keys3[m])));
	  if (keys4[m])
	    k = k + sprintf((buffer + k), ".%s", getKeyString((int)keys4[m]));
	}
      k = k + sprintf((buffer + k), "(), ");
    }
  if (buffer[k - 2] == ',')
    sprintf((buffer + (k - 2)), ")%c%c", 13, 10);
  else // no properties
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * GoToCommand::getCommandStringT(char * buffer, char * tagString)
{
  int k;

  k = sprintf(buffer, "%s GoTo(", tagString);
  if (hasX)
    k = k + sprintf((buffer + k), "X(%.5lf), ", x);
  if (hasY)
    k = k + sprintf((buffer + k), "Y(%.5lf), ", y);
  if (hasZ)
    k = k + sprintf((buffer + k), "Z(%.5lf), ", z);
  if (hasToolA)
    k = k + sprintf((buffer + k), "Tool.A(%.5lf), ", toolA);
  if (hasToolB)
    k = k + sprintf((buffer + k), "Tool.B(%.5lf), ", toolB);
  if (hasToolC)
    k = k + sprintf((buffer + k), "Tool.C(%.5lf), ", toolC);
  if (buffer[k - 2] == ',')
    sprintf((buffer + (k - 2)), ")%c%c", 13, 10);
  else // no coordinates
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * LockAxisCommand::getCommandStringT(char * buffer, char * tagString)
{
  int k = 0;

  k = sprintf(buffer, "%s LockAxis(", tagString);
  if (hasC)
    k = k + sprintf((buffer + k), "C(), ");
  if (hasPhi)
    k = k + sprintf((buffer + k), "Phi(), ");
  if (hasR)
    k = k + sprintf((buffer + k), "R(), ");
  if (hasX)
    k = k + sprintf((buffer + k), "X(), ");
  if (hasY)
    k = k + sprintf((buffer + k), "Y(), ");
  if (hasZ)
    k = k + sprintf((buffer + k), "Z(), ");
  if (buffer[k - 2] == ',')
    sprintf((buffer + (k - 2)), ")%c%c", 13, 10);
  else // no axes
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * OnMoveReportECommand::getCommandStringT(char * buffer, char * tagString)
{
  int k;

  k = sprintf(buffer, "%s OnMoveReportE(", tagString);
  if (hasDis)
    k = k + sprintf((buffer + k), "Dis(%.5lf), ", theDis);
  if (hasTime)
    k = k + sprintf((buffer + k), "Time(%.5lf), ", theTime);
  return printLikeGet((buffer + k), hasR, hasX, hasY, hasZ,
		      hasToolA, hasToolB, hasToolC);
}

/*********************************************************************/

char * OnPtMeasReportCommand::getCommandStringT(char * buffer,
						char * tagString)
{
  int k;
  
  k = sprintf(buffer, "%s OnPtMeasReport(", tagString);
  if (hasER)
    k = k + sprintf((buffer + k), "ER(), ");
  if (hasIJK)
    k = k + sprintf((buffer + k), "IJK(), ");
  return printLikeGet((buffer + k), hasR, hasX, hasY, hasZ,
		      hasToolA, hasToolB, hasToolC);
}

/*********************************************************************/

char * OnScanReportCommand::getCommandStringT(char * buffer, char * tagString)
{
  int k;
  
  k = sprintf(buffer, "%s OnScanReport(", tagString);
  if (hasER)
    k = k + sprintf((buffer + k), "ER(), ");
  if (hasIJK)
    k = k + sprintf((buffer + k), "IJK(), ");
  if (hasQ)
    k = k + sprintf((buffer + k), "Q(), ");
  return printLikeGet((buffer + k), hasR, hasX, hasY, hasZ,
		      hasToolA, hasToolB, hasToolC);
}

/*********************************************************************/

char * PtMeasCommand::getCommandStringT(char * buffer, char * tagString)
{
  static char aString[100];
  int k;

  k = sprintf(buffer, "%s PtMeas(", tagString);
  if (hasX)
    k = k + sprintf((buffer + k), "X(%.5lf), ", x);
  if (hasY)
    k = k + sprintf((buffer + k), "Y(%.5lf), ", y);
  if (hasZ)
    k = k + sprintf((buffer + k), "Z(%.5lf), ", z);
  if (hasIJK)
    {
      if (theIJK.getVectorString(aString, 99))
	k = k + sprintf((buffer + k), "IJK(%s), ", aString);
      else
	buffer = NULL;
    }
  if (buffer)
    {
      if (buffer[k - 2] == ',')
	sprintf((buffer + (k - 2)), ")%c%c", 13, 10);
      else // no coordinates
	buffer = NULL;
    }
  return buffer;
}

/*********************************************************************/

char * ScanInCylEndIsPlaneCommand::getCommandStringT(char * buffer,
						     char * tagString)
{
  sprintf(buffer, "%s ScanInCylEndIsPlane(%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%d,%.5lf,%.5lf,%.5lf)%c%c",
	  tagString, cx, cy, cz, cIJK.getI(), cIJK.getJ(), cIJK.getK(),
	  sx, sy, sz, sIJK.getI(), sIJK.getJ(), sIJK.getK(), dx, dy, dz,
	  stepW, px, py, pz, pIJK.getI(), pIJK.getJ(), pIJK.getK(), num,
	  eIJK.getI(), eIJK.getJ(), eIJK.getK(), 13, 10);
  return buffer;
}

/*********************************************************************/

char * ScanInCylEndIsSphereCommand::getCommandStringT(char * buffer,
						      char * tagString)
{
  sprintf(buffer, "%s ScanInCylEndIsSphere(%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf)%c%c", tagString,
	  cx, cy, cz, cIJK.getI(), cIJK.getJ(), cIJK.getK(), sx, sy, sz,
	  sIJK.getI(), sIJK.getJ(), sIJK.getK(), dx, dy, dz, stepW,
	  ex, ey, ez, dia, eIJK.getI(), eIJK.getJ(), eIJK.getK(), 13, 10);
  return buffer;
}

/*********************************************************************/

char * ScanInPlaneEndIsCylCommand::getCommandStringT(char * buffer,
						     char * tagString)
{
  sprintf(buffer, "%s ScanInPlaneEndIsCyl(%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf,%d,%.5lf,%.5lf,%.5lf)%c%c", tagString,
	  sx, sy, sz, sIJK.getI(), sIJK.getJ(), sIJK.getK(), dx, dy, dz,
	  stepW, cx, cy, cz, cIJK.getI(), cIJK.getJ(), cIJK.getK(),
	  dia, num, eIJK.getI(), eIJK.getJ(), eIJK.getK(), 13, 10);
  return buffer;
}

/*********************************************************************/

char * ScanInPlaneEndIsPlaneCommand::getCommandStringT(char * buffer,
						       char * tagString)
{
  sprintf(buffer, "%s ScanInPlaneEndIsPlane(%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%d,%.5lf,%.5lf,%.5lf)%c%c", tagString,
	  sx, sy, sz, sIJK.getI(), sIJK.getJ(), sIJK.getK(), dx, dy, dz,
	  stepW, px, py, pz, pIJK.getI(), pIJK.getJ(), pIJK.getK(),
	  num, eIJK.getI(), eIJK.getJ(), eIJK.getK(), 13, 10);
  return buffer;
}

/*********************************************************************/

char * ScanInPlaneEndIsSphereCommand::getCommandStringT(char * buffer,
							char * tagString)
{
  sprintf(buffer, "%s ScanInPlaneEndIsSphere(%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf)%c%c", tagString, sx, sy, sz, sIJK.getI(),
	  sIJK.getJ(), sIJK.getK(), dx, dy, dz, stepW, ex, ey, ez,
	  dia, eIJK.getI(), eIJK.getJ(), eIJK.getK(), 13, 10);
  return buffer;
}

/*********************************************************************/

char * ScanOnCircleCommand::getCommandStringT(char * buffer, char * tagString)
{

  sprintf(buffer, "%s ScanOnCircle(%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf)%c%c", tagString,
	  cx, cy, cz, sx, sy, sz, theIJK.getI(), theIJK.getJ(), theIJK.getK(),
	  delta, sfa, stepW, 13, 10);
  
  return buffer;
}

/*********************************************************************/

char * ScanOnCircleHintCommand::getCommandStringT(char * buffer,
						  char * tagString)
{
  sprintf(buffer, "%s ScanOnCircleHint(%.5lf,%.5lf)%c%c",
	  tagString, displacement, form, 13, 10);
  return buffer;
}

/*********************************************************************/

char * ScanOnLineCommand::getCommandStringT(char * buffer, char * tagString)
{
  sprintf(buffer, "%s ScanOnLine(%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
	  "%.5lf,%.5lf,%.5lf,%.5lf,%.5lf)%c%c",
	  tagString, sx, sy, sz, ex, ey, ez, theIJK.getI(),
	  theIJK.getJ(), theIJK.getK(), stepW, 13, 10);
  return buffer;
}

/*********************************************************************/

char * ScanOnLineHintCommand::getCommandStringT(char * buffer,
						char * tagString)
{
  sprintf(buffer, "%s ScanOnLineHint(%.5lf, %.5lf)%c%c",
	  tagString, angle, form, 13, 10);
  return buffer;
}

/*********************************************************************/

char * ScanUnknownHintCommand::getCommandStringT(char * buffer,
						 char * tagString)
{
  sprintf(buffer, "%s ScanUnknownHint(%.5lf)%c%c",
	  tagString, minRadius, 13, 10);
  return buffer;
}

/*********************************************************************/

char * SetCoordSystemCommand::getCommandStringT(char * buffer,
						char * tagString)
{
  sprintf(buffer, "%s SetCoordSystem(%s)%c%c",
	  tagString, getKeyString((int)getCsyType()), 13, 10);
  return buffer;
}

/*********************************************************************/

char * SetCsyTransformationCommand::getCommandStringT(char * buffer,
						      char * tagString)
{
  sprintf(buffer,
	"%s SetCsyTransformation(%s,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf)%c%c", 
	  tagString, getKeyString((int)coordSys), x0, y0, z0,
	  theta, psi, phi, 13, 10);
  return buffer;
}

/***********************************************************************

This is the same as GetPropCommand::getCommandStringT,
except that values are included here and the command name is different.

*/

char * SetPropCommand::getCommandStringT(char * buffer, char * tagString)
{
  int k;
  int m;

  k = sprintf(buffer, "%s SetProp(", tagString);
  for (m = 0; m < numberProps; m++)
    {
      k = k + sprintf((buffer + k), "%s.%s.%s",
		      getKeyString((int)keys1[m]),
		      getKeyString((int)keys2[m]),
		      getKeyString((int)keys3[m]));
      if (keys4[m])
	k = k + sprintf((buffer + k), ".%s", getKeyString((int)keys4[m]));
      k = k + sprintf((buffer + k), "(%.5lf), ", propertyValues[m]);
    }
  if (buffer[k - 2] == ',')
    sprintf((buffer + (k - 2)), ")%c%c", 13, 10);
  else // no properties set
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * SetToolCommand::getCommandStringT(char * buffer, char * tagString)
{
  sprintf(buffer, "%s SetTool(\"%s\")%c%c",
	  tagString, getToolName(), 13, 10);
  return buffer;
}

/*********************************************************************/

char * StopDaemonCommand::getCommandStringT(char * buffer, char * tagString)
{
  static char aString[TAGSTRINGSIZE];

  if (daemonTag.getTagString(aString))
    sprintf(buffer, "%s StopDaemon(%s)%c%c", tagString, aString, 13, 10);
  else
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * TiltCenterPartCommand::getCommandStringT(char * buffer,char * tagString)
{
  sprintf(buffer,
    "%s TiltCenterPart(%.5lf, %.5lf, %.5lf, %.5lf, %.5lf, %.5lf, %.5lf)%c%c",
	  tagString, px1, py1, pz1, px2, py2, pz2, limit, 13, 10);
  return buffer;
}

/*********************************************************************/

char * TiltPartCommand::getCommandStringT(char * buffer, char * tagString)
{
  sprintf(buffer, "%s TiltPart(%.5lf, %.5lf, %.5lf, %.5lf)%c%c",
	  tagString, dx, dy, dz, limit, 13, 10);
  return buffer;
}

/* GetResponseString ****************************************************

The answer array must be big enough to hold any possible response since
it is being written into without checking whether there is room or not.
Thus, all the getResponseString functions must be careful not to write
too much. The ScanResponse is the largest.

Note that although the tagString is always written into, it is not
used for writing the answer when the tag number is required to be E0000.

*/

char * Response::getResponseString(char * buffer, int n)
{
  static char answer[1000];
  static char tagString[TAGSTRINGSIZE];
  char * result;

  result = NULL;
  if (buffer && (tag->getTagString(tagString)))
    {
      switch(responseName)
	{
	case Ack:
	  sprintf(answer, "%s &%c%c", tagString, 13, 10);
	  result = answer;
	  break;
	case Complete:
	  sprintf(answer, "%s %%%c%c", tagString, 13, 10);
	  result = answer;
	  break;
	case DataAlign:
	  result = ((AlignResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataCenterPart:
	  result = ((CenterPartResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataChangeToolE:
	  result = ((ChangeToolEResponse *)this)->
	    getResponseStringT(answer);
	  break;
	case DataEnumProp:
	  result = ((EnumPropResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataGetChangeToolAction:
	  result = ((GetChangeToolActionResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataGetCoordSystem:
	  result = ((GetCoordSystemResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataGetCsyTransformation:
	  result = ((GetCsyTransformationResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataGetDMEVersion:
	  result = ((GetDMEVersionResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataGetErrStatusE:
	  result = ((GetErrStatusEResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataGetMachineClass:
	  result = ((GetMachineClassResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataGetProp:
	  result = ((GetPropResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataGoToE:
	  result = ((GoToEResponse *)this)->
	    getResponseStringT(answer);
	  break;
	case DataIsHomed:
	  result = ((IsHomedResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataIsUserEnabled:
	  result = ((IsUserEnabledResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataKeyPressE:
	  result = ((KeyPressEResponse *)this)->
	    getResponseStringT(answer);
	  break;
	case DataOnMoveReportE:
	  result = ((OnMoveReportEResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataPtMeasOrGet:
	  result = ((PtMeasOrGetResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataPtMeasE:
	  result = ((PtMeasEResponse *)this)->
	    getResponseStringT(answer);
	  break;
	case DataScan:
	  result = ((ScanResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataSetPropE:
	  result = ((SetPropEResponse *)this)->
	    getResponseStringT(answer);
	  break;
	case DataString:
	  result = ((StringResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataTiltCenterPart:
	  result = ((TiltCenterPartResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case DataTiltPart:
	  result = ((TiltPartResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	case ErrorError:
	  result = ((ErrorResponse *)this)->
	    getResponseStringT(answer, tagString);
	  break;
	default:
	  result = NULL;
	}
    }
  if (result)
    {
      if (n > (int)strlen(answer))
	strcpy(buffer, answer);
      else
	buffer = NULL;
    }
  else
    buffer = NULL;
  return buffer;
}

/**********************************************************************/

char * AlignResponse::getResponseStringT(char * buffer, char * tagString)
{ 
  if (has2)
    sprintf(buffer, "%s # (%.5lf, %.5lf, %.5lf, %.5lf, %.5lf, %.5lf)%c%c",
	    tagString, i1, j1, k1, i2, j2, k2, 13, 10);
  else
    sprintf(buffer, "%s # (%.5lf, %.5lf, %.5lf)%c%c",
	    tagString, i1, j1, k1, 13, 10);
  return buffer;
}

/***********************************************************************/

char * CenterPartResponse::getResponseStringT(char * buffer,
					      char * tagString)
{  
  sprintf(buffer, "%s # CenterPart(%d)%c%c",
	  tagString, (centered ? 1 : 0), 13, 10);
  return buffer;
}

/*********************************************************************/

char * ChangeToolEResponse::getResponseStringT(char * buffer)
{
  sprintf(buffer, "E0000 # ChangeTool(%c%s%c)%c%c", 34, toolName, 34, 13, 10);
  return buffer;
}

/***********************************************************************

EnumPropResponse examples are on page 73.

*/

char * EnumPropResponse::getResponseStringT(char * buffer, char * tagString)
{  
  sprintf(buffer, "%s # \"%s\", \"%s\"%c%c",
	  tagString, memberName, valueTypeName, 13, 10);
  return buffer;
}


/*********************************************************************/

char * ErrorResponse::getResponseStringT(char * buffer, char * tagString)
{  
  sprintf(buffer, "%s ! Error(%c, %04d, \"%s\", \"%s\")%c%c",
	  tagString, theError->getSeverity(), theError->getName(),
          errorCausingMethod, theError->getText(), 13, 10);
  return buffer;
}

/*********************************************************************/

char * GetChangeToolActionResponse::getResponseStringT(char * buffer,
						       char * tagString)
{  
  sprintf(buffer,"%s # ChangeToolAction(%s, X(%.5lf), Y(%.5lf), Z(%.5lf))%c%c",
	  tagString, getKeyString((int)changeType), x, y, z, 13, 10);
  return buffer;
}

/*********************************************************************/

char * GetCoordSystemResponse::getResponseStringT(char * buffer,
						  char * tagString)
{  
  sprintf(buffer, "%s # CoordSystem(%s)%c%c",
	  tagString, getKeyString((int)coordSys), 13, 10);
  return buffer;
}

/*********************************************************************/

char * GetCsyTransformationResponse::getResponseStringT(char * buffer,
							char * tagString)
{
  sprintf(buffer,
 "%s # GetCsyTransformation(%.5lf, %.5lf, %.5lf, %.5lf, %.5lf, %.5lf)%c%c",
	  tagString, X0, Y0, Z0, Theta, Psi, Phi, 13, 10);
  return buffer;
}

/*********************************************************************/

char * GetDMEVersionResponse::getResponseStringT(char * buffer,
						 char * tagString)
{  
  sprintf(buffer, "%s # DMEVersion(\"%s\")%c%c", tagString, version, 13, 10);
  return buffer;
}

/*********************************************************************/

char * GetErrStatusEResponse::getResponseStringT(char * buffer,
						 char * tagString)
{  
  sprintf(buffer, "%s # ErrStatus(%d)%c%c", tagString, isErred, 13, 10);
  return buffer;
}


/*********************************************************************/

char * GetMachineClassResponse::getResponseStringT(char * buffer,
						   char * tagString)
{  
  sprintf(buffer, "%s # GetMachineClass(\"%s\")%c%c",
	  tagString, className, 13, 10);
  return buffer;
}

/**********************************************************************

For this to work properly, each OnePropertyXXX propString function
must reset k to point to where printing the next item should start.

*/

char * GetPropResponse::getResponseStringT(char * buffer, char * tagString)
{
  int k;
  int m;

  k = sprintf(buffer, "%s # ", tagString);
  for (m = 0; m < numberProps; m++)
    {
      oneProps[m]->propString(buffer, &k);
      sprintf((buffer + k), ", ");
      k = (k + 2);
    }
  if (buffer[k - 2] == ',')
    sprintf((buffer + (k - 2)), "%c%c", 13, 10);
  else // no properties
    buffer = NULL;
  return buffer;
}

/*********************************************************************/

char * GoToEResponse::getResponseStringT(char * buffer)
{
  int k;

  k = sprintf(buffer, "E0000 # GoTo(");
  if (hasER)
    k = k + sprintf((buffer + k), "ER(%.5lf), ", theER);
  if (hasIJK)
    k = k + sprintf((buffer + k), "IJK(0,0,0), ");
  return getResponseStringEnd(buffer, k, 1);
}

/*********************************************************************/

char * IsHomedResponse::getResponseStringT(char * buffer, char * tagString)
{
  sprintf(buffer, "%s # IsHomed(%d)%c%c", tagString, homed, 13, 10);
  return buffer;
}


/*********************************************************************/

char * IsUserEnabledResponse::getResponseStringT(char * buffer,
						 char * tagString)
{  
  sprintf(buffer, "%s # IsUserEnabled(%d)%c%c", tagString, enabled, 13, 10);
  return buffer;
}


/**********************************************************************/

char * KeyPressEResponse::getResponseStringT(char * buffer)
{
  sprintf(buffer, "E0000 # KeyPress(%c%s%c)%c%c", 34, keyName, 34, 13, 10);
  return buffer;
}

/**********************************************************************/

char * OnePropertyAlignment::propString(char * buffer, int * k)
{
  *k = (*k + sprintf((buffer + *k), "Tool.Alignment(%.5lf, %.5lf, %.5lf",
		     i1, j1, k1));
  if (has2)
    *k = (*k + sprintf((buffer + *k), ", %.5lf, %.5lf, %.5lf",
		       i2, j2, k2));
  *k = (*k + sprintf((buffer + *k), ")"));
  return buffer;
}

/*********************************************************************/

char * OnePropertyCollisionVolume::propString(char * buffer, int * k)
{
  int j;
  Box * b;

  *k = (*k + sprintf((buffer + *k), "Tool.CollisionVolume("));
  for (j = 0; j < numberBoxes; j++)
    {
      b = (boxes + j);
      *k = (*k + sprintf((buffer + *k), 
			 "OBB,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,"
			 "%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf",
			 b->Cx, b->Cy, b->Cz, b->Ex, b->Ey, b->Ez,
			 b->Ix, b->Iy, b->Iz, b->Jx, b->Jy, b->Jz,
			 b->Kx, b->Ky, b->Kz));
      if (j != (numberBoxes - 1))
	*k = (*k + sprintf((buffer + *k), ", "));
    }
  *k = (*k + sprintf((buffer + *k), ")"));
  return buffer;
}

/*********************************************************************/

char * OnePropertyId::propString(char * buffer, int * k)
{
  *k = (*k + sprintf((buffer + *k), "%sTool.Id(%c%s%c)",
		     (isFound ? "Found" : ""), 34, idString, 34));
  return buffer;
}

/*********************************************************************/

char * OnePropertyName::propString(char * buffer, int * k)
{
  *k = (*k + sprintf((buffer + *k), "%sTool.Name(%c%s%c)",
		     (isFound ? "Found" : ""), 34, nameString, 34));
  return buffer;
}

/*********************************************************************/

char * OnePropertyNumber::propString(char * buffer, int * k)
{
  *k = (*k + sprintf((buffer + *k), "%s.%s.%s",
		     getKeyString((int)key1),
		     getKeyString((int)key2),
		     getKeyString((int)key3)));
  if (numberKeys == 4)
    *k = (*k + sprintf((buffer + *k), ".%s", getKeyString((int)key4)));
  *k = (*k + sprintf((buffer + *k), "(%.5lf)", val));
  return buffer;
}

/*********************************************************************/

char * OnMoveReportEResponse::getResponseStringT(char * buffer,
						 char * tagString)
{
  int k;

  k = sprintf(buffer, "%s # ", tagString);
  return getResponseStringEnd(buffer, k, 0);
}

/*********************************************************************/

char * PtMeasOrGetResponse::getResponseStringT(char * buffer, char * tagString)
{
  int k;

  k = sprintf(buffer, "%s # ", tagString);
  if (hasER)
    k = k + sprintf((buffer + k), "ER(%.5lf), ", theER);
  if (hasIJK)
    k = k + sprintf((buffer + k), "IJK(%.5lf,%.5lf,%.5lf), ",
		    getI(), getJ(), getK());
  return getResponseStringEnd(buffer, k, 0);
}

/*********************************************************************/

char * PtMeasEResponse::getResponseStringT(char * buffer)
{
  int k;

  k = sprintf(buffer, "E0000 # PtMeas(");
  if (hasER)
    k = k + sprintf((buffer + k), "ER(%.5lf), ", theER);
  if (hasIJK)
    k = k + sprintf((buffer + k), "IJK(%.5lf,%.5lf,%.5lf), ",
		    getI(), getJ(), getK());
  return getResponseStringEnd(buffer, k, 1);
}

/*********************************************************************/

char * ScanResponse::getResponseStringT(char * buffer, char * tagString)
{
  int n;
  int k;

  k = sprintf(buffer, "%s # ", tagString);
  for (n = 0; n < dataSize; n++)
    {
      k = (k + sprintf((buffer + k), "%.5lf,", data[n]));
    }
  if (buffer[k - 1] == ',')
    k--;
  sprintf((buffer + k), "%c%c", 13, 10);
  return buffer;
}

/*********************************************************************/

char * SetPropEResponse::getResponseStringT(char * buffer)
{
  int k;
  
  k = sprintf(buffer, "E0000 # SetProp(%s.%s.%s",
	      getKeyString((int)key1),
	      getKeyString((int)key2),
	      getKeyString((int)key3));
  if (key4)
    k = (k + sprintf((buffer + k), ".%s", getKeyString((int)key4)));
  sprintf((buffer + k), "(%.5lf)%c%c", propertyValue, 13, 10);
  return buffer;
}

/*********************************************************************/

char * StringResponse::getResponseStringT(char * buffer, char * tagString)
{  
  sprintf(buffer, "%s # \"%s\"%c%c", tagString, data, 13, 10);
  return buffer;
}

/**********************************************************************

Version 4.0 of the spec says the name of the data is CenterPart,
but this appears to be unintentional. Here, the name is TiltCenterPart.

*/

char * TiltCenterPartResponse::getResponseStringT(char * buffer,
						  char * tagString)
{  
  sprintf(buffer, "%s # TiltCenterPart(%d)%c%c",
	  tagString, (trued ? 1 : 0), 13, 10);
  return buffer;
}

/**********************************************************************

Version 4.0 of the spec says the name of the data is MoveCenterPart,
but this appears to be unintentional. Here, the name is TiltPart.

*/

char * TiltPartResponse::getResponseStringT(char * buffer,
					    char * tagString)
{  
  sprintf(buffer, "%s # TiltPart(%d)%c%c",
	  tagString, (tilted ? 1 : 0), 13, 10);
  return buffer;
}

/*********************************************************************/

char * XYZResponse::getResponseStringEnd(char * buffer, int k, int closeParen)
{
  if (hasR)
    k = k + sprintf((buffer + k), "R(%.5lf), ", r);
  if (hasX)
    k = k + sprintf((buffer + k), "X(%.5lf), ", x);
  if (hasY)
    k = k + sprintf((buffer + k), "Y(%.5lf), ", y);
  if (hasZ)
    k = k + sprintf((buffer + k), "Z(%.5lf), ", z);
  if (hasToolA)
    k = k + sprintf((buffer + k), "Tool.A(%.5lf), ", toolA);
  if (hasToolB)
    k = k + sprintf((buffer + k), "Tool.B(%.5lf), ", toolB);
  if (hasToolC)
    k = k + sprintf((buffer + k), "Tool.C(%.5lf), ", toolC);
  if (buffer[k - 2] == ',')
    {
      if (closeParen)
	sprintf((buffer + (k - 2)), ")%c%c", 13, 10);
      else
	sprintf((buffer + (k - 2)), "%c%c", 13, 10);
    }
  else // no coordinates
    buffer = NULL;
  return buffer;
}

/* IppError instance declarations **************************************

One instance of each subclass of IppError is declared below for use
in the ErrorResponse constructor and the GetErrorInfoCommand constructor.

In any application, only one error of each type is needed, since all
errors of a given type are identical.

*/

IppError  IppCmdRes_IppError;   // generic error. Use if error number bad.
Error0000 IppCmdRes_Error0000;
Error0001 IppCmdRes_Error0001;
Error0002 IppCmdRes_Error0002;
Error0003 IppCmdRes_Error0003;
Error0004 IppCmdRes_Error0004;
Error0005 IppCmdRes_Error0005;
Error0006 IppCmdRes_Error0006;
Error0007 IppCmdRes_Error0007;
Error0008 IppCmdRes_Error0008;
Error0500 IppCmdRes_Error0500;
Error0501 IppCmdRes_Error0501;
Error0502 IppCmdRes_Error0502;
Error0503 IppCmdRes_Error0503;
Error0504 IppCmdRes_Error0504;
Error0505 IppCmdRes_Error0505;
Error0506 IppCmdRes_Error0506;
Error0507 IppCmdRes_Error0507;
Error0508 IppCmdRes_Error0508;
Error0509 IppCmdRes_Error0509;
Error0510 IppCmdRes_Error0510;
Error0511 IppCmdRes_Error0511;
Error0512 IppCmdRes_Error0512;
Error0513 IppCmdRes_Error0513;
Error0514 IppCmdRes_Error0514;
Error0515 IppCmdRes_Error0515;
Error1000 IppCmdRes_Error1000;
Error1001 IppCmdRes_Error1001;
Error1002 IppCmdRes_Error1002;
Error1003 IppCmdRes_Error1003;
Error1004 IppCmdRes_Error1004;
Error1005 IppCmdRes_Error1005;
Error1006 IppCmdRes_Error1006;
Error1007 IppCmdRes_Error1007;
Error1008 IppCmdRes_Error1008;
Error1009 IppCmdRes_Error1009;
Error1010 IppCmdRes_Error1010;
Error1500 IppCmdRes_Error1500;
Error1501 IppCmdRes_Error1501;
Error1502 IppCmdRes_Error1502;
Error1503 IppCmdRes_Error1503;
Error2000 IppCmdRes_Error2000;
Error2001 IppCmdRes_Error2001;
Error2002 IppCmdRes_Error2002;
Error2500 IppCmdRes_Error2500;
Error2501 IppCmdRes_Error2501;
Error2502 IppCmdRes_Error2502;
Error2503 IppCmdRes_Error2503;
Error2504 IppCmdRes_Error2504;
Error2505 IppCmdRes_Error2505;
Error2506 IppCmdRes_Error2506;
 
/* ErrorResponse::ErrorResponse ***************************************

This uses the inErrorName to calculate an index into the allErrors
array.

*/

ErrorResponse::ErrorResponse(unsigned int inTagNumber, tagIdType inTagType,
		int inErrorName, char * inErrorCausingMethod) :
    Response(inTagNumber, inTagType, ErrorError)
{
  static IppError * const allErrors[]= {
    &IppCmdRes_Error0000, /*  0 */
    &IppCmdRes_Error0001, /*  1 */
    &IppCmdRes_Error0002, /*  2 */
    &IppCmdRes_Error0003, /*  3 */
    &IppCmdRes_Error0004, /*  4 */
    &IppCmdRes_Error0005, /*  5 */
    &IppCmdRes_Error0006, /*  6 */
    &IppCmdRes_Error0007, /*  7 */
    &IppCmdRes_Error0008, /*  8 */
    &IppCmdRes_Error0500, /*  9 */
    &IppCmdRes_Error0501, /* 10 */
    &IppCmdRes_Error0502, /* 11 */
    &IppCmdRes_Error0503, /* 12 */
    &IppCmdRes_Error0504, /* 13 */
    &IppCmdRes_Error0505, /* 14 */
    &IppCmdRes_Error0506, /* 15 */
    &IppCmdRes_Error0507, /* 16 */
    &IppCmdRes_Error0508, /* 17 */
    &IppCmdRes_Error0509, /* 18 */
    &IppCmdRes_Error0510, /* 19 */
    &IppCmdRes_Error0511, /* 20 */
    &IppCmdRes_Error0512, /* 21 */
    &IppCmdRes_Error0513, /* 22 */
    &IppCmdRes_Error0514, /* 23 */
    &IppCmdRes_Error0515, /* 24 */
    &IppCmdRes_Error1000, /* 25 */
    &IppCmdRes_Error1001, /* 26 */
    &IppCmdRes_Error1002, /* 27 */
    &IppCmdRes_Error1003, /* 28 */
    &IppCmdRes_Error1004, /* 29 */
    &IppCmdRes_Error1005, /* 30 */
    &IppCmdRes_Error1006, /* 31 */
    &IppCmdRes_Error1007, /* 32 */
    &IppCmdRes_Error1008, /* 33 */
    &IppCmdRes_Error1009, /* 34 */
    &IppCmdRes_Error1010, /* 35 */
    &IppCmdRes_Error1500, /* 36 */
    &IppCmdRes_Error1501, /* 37 */
    &IppCmdRes_Error1502, /* 38 */
    &IppCmdRes_Error1503, /* 39 */
    &IppCmdRes_Error2000, /* 40 */
    &IppCmdRes_Error2001, /* 41 */
    &IppCmdRes_Error2002, /* 42 */
    &IppCmdRes_Error2500, /* 43 */
    &IppCmdRes_Error2501, /* 44 */
    &IppCmdRes_Error2502, /* 45 */
    &IppCmdRes_Error2503, /* 46 */
    &IppCmdRes_Error2504, /* 47 */
    &IppCmdRes_Error2505, /* 48 */
    &IppCmdRes_Error2506, /* 49 */
    &IppCmdRes_IppError,  /* 50 */
  };
  if (inErrorName < 9)
    theError = allErrors[(int)inErrorName];
  else if (inErrorName < 516)
    {
      if (inErrorName > 499)
	theError = allErrors[(int)inErrorName - 491];
      else
	theError = allErrors[50];
    }
  else if (inErrorName < 1011)
    {
      if (inErrorName > 999)
	theError = allErrors[(int)inErrorName - 975];
      else
	theError = allErrors[50];
    }
  else if (inErrorName < 1504)
    {
      if (inErrorName > 1499)
	theError = allErrors[(int)inErrorName - 1464];
      else
	theError = allErrors[50];
    }
  else if (inErrorName < 2003)
    {
      if (inErrorName > 1999)
	theError = allErrors[(int)inErrorName - 1960];
      else
	theError = allErrors[50];
    }
  else if (inErrorName < 2507)
    {
      if (inErrorName > 2499)
	theError = allErrors[(int)inErrorName - 2457];
      else
	theError = allErrors[50];
    }
  else
    theError = allErrors[50];
  errorCausingMethod = inErrorCausingMethod;
}

/* GetErrorInfoCommand::setTheErrorAndErrorNumber **********************

If inErrorNumber is not the number of a defined error, this sets the
error to a generic IppError and sets the errorNumber to 10000.

*/

void GetErrorInfoCommand::setTheErrorAndErrorNumber(unsigned int inErrorNumber)
{
  static IppError * const allErrors[]= {
    &IppCmdRes_Error0000, /*  0 */
    &IppCmdRes_Error0001, /*  1 */
    &IppCmdRes_Error0002, /*  2 */
    &IppCmdRes_Error0003, /*  3 */
    &IppCmdRes_Error0004, /*  4 */
    &IppCmdRes_Error0005, /*  5 */
    &IppCmdRes_Error0006, /*  6 */
    &IppCmdRes_Error0007, /*  7 */
    &IppCmdRes_Error0008, /*  8 */
    &IppCmdRes_Error0500, /*  9 */
    &IppCmdRes_Error0501, /* 10 */
    &IppCmdRes_Error0502, /* 11 */
    &IppCmdRes_Error0503, /* 12 */
    &IppCmdRes_Error0504, /* 13 */
    &IppCmdRes_Error0505, /* 14 */
    &IppCmdRes_Error0506, /* 15 */
    &IppCmdRes_Error0507, /* 16 */
    &IppCmdRes_Error0508, /* 17 */
    &IppCmdRes_Error0509, /* 18 */
    &IppCmdRes_Error0510, /* 19 */
    &IppCmdRes_Error0511, /* 20 */
    &IppCmdRes_Error0512, /* 21 */
    &IppCmdRes_Error0513, /* 22 */
    &IppCmdRes_Error0514, /* 23 */
    &IppCmdRes_Error0515, /* 24 */
    &IppCmdRes_Error1000, /* 25 */
    &IppCmdRes_Error1001, /* 26 */
    &IppCmdRes_Error1002, /* 27 */
    &IppCmdRes_Error1003, /* 28 */
    &IppCmdRes_Error1004, /* 29 */
    &IppCmdRes_Error1005, /* 30 */
    &IppCmdRes_Error1006, /* 31 */
    &IppCmdRes_Error1007, /* 32 */
    &IppCmdRes_Error1008, /* 33 */
    &IppCmdRes_Error1009, /* 34 */
    &IppCmdRes_Error1010, /* 35 */
    &IppCmdRes_Error1500, /* 36 */
    &IppCmdRes_Error1501, /* 37 */
    &IppCmdRes_Error1502, /* 38 */
    &IppCmdRes_Error1503, /* 39 */
    &IppCmdRes_Error2000, /* 40 */
    &IppCmdRes_Error2001, /* 41 */
    &IppCmdRes_Error2002, /* 42 */
    &IppCmdRes_Error2500, /* 43 */
    &IppCmdRes_Error2501, /* 44 */
    &IppCmdRes_Error2502, /* 45 */
    &IppCmdRes_Error2503, /* 46 */
    &IppCmdRes_Error2504, /* 47 */
    &IppCmdRes_Error2505, /* 48 */
    &IppCmdRes_Error2506, /* 49 */
  };
  if (inErrorNumber < 9)
    theError = allErrors[inErrorNumber];
  else if (inErrorNumber < 516)
    {
      if (inErrorNumber > 499)
	theError = allErrors[inErrorNumber - 491];
      else
	theError = &IppCmdRes_IppError;
    }
  else if (inErrorNumber < 1011)
    {
      if (inErrorNumber > 999)
	theError = allErrors[inErrorNumber - 975];
      else
	theError = &IppCmdRes_IppError;
    }
  else if (inErrorNumber < 1504)
    {
      if (inErrorNumber > 1499)
	theError = allErrors[inErrorNumber - 1464];
      else
	theError = &IppCmdRes_IppError;
    }
  else if (inErrorNumber < 2003)
    {
      if (inErrorNumber > 1999)
	theError = allErrors[inErrorNumber - 1960];
      else
	theError = &IppCmdRes_IppError;
    }
  else if (inErrorNumber < 2507)
    {
      if (inErrorNumber > 2499)
	theError = allErrors[inErrorNumber - 2457];
      else
	theError = &IppCmdRes_IppError;
    }
  else
    theError = &IppCmdRes_IppError;
  errorNumber = ((theError == &IppCmdRes_IppError) ? 10000 : inErrorNumber);
}

/*********************************************************************/

/* ScanResponse::ScanResponse

This makes a copy of inData, so it does not matter what happens to
inData after this runs.

*/

ScanResponse::ScanResponse(unsigned int inTagNumber,
			  int inDataSize, double * inData) :
  Response(inTagNumber, CommandTag, DataScan)
{
  int n;
  
  if ((inData) && (inDataSize > 0))
    {
      dataSize = inDataSize;
      data = new double[inDataSize];
      for (n = 0; n < inDataSize; n++)
	data[n] = inData[n];
    }
  else
    {
      dataSize = 0;
      data = NULL;
    }
}

/*********************************************************************/

/* ScanResponse::setData

*/

void ScanResponse::setData(int inDataSize, double * inData)
{
  int n;
  
  if (data)
    delete data;
  if ((inData) && (inDataSize > 0))
    {
      dataSize = inDataSize;
      data = new double[inDataSize];
      for (n = 0; n < inDataSize; n++)
	data[n] = inData[n];
    }
  else
    {
      dataSize = 0;
      data = NULL;
    }
}

/*********************************************************************/

