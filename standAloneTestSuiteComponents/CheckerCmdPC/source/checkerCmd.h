/* checkerCmd.h

*/

#ifndef CHECKERCMD_HH
#define CHECKERCMD_HH

#include <stdio.h>
#include <string.h>
#include "world.h"

enum checkerErrorId {
CHECKER_OK = 0,
ALREADY_IN_SESSION,
AXIS_DIRECTION_VECTOR_HAS_ZERO_LENGTH,
BAD_ERROR_NUMBER,
CAN_NOT_CHANGE_TOOL_TO_UNDEFTOOL,
CAN_NOT_ENUMALLPROP_NOTOOL_PTMEASPAR,
CAN_NOT_ENUMALLPROP_UNDEFTOOL_GOTOPAR,
CAN_NOT_ENUMALLPROP_UNDEFTOOL_PTMEASPAR,
CAN_NOT_ENUMPROP_NOTOOL_PTMEASPAR,
CAN_NOT_ENUMPROP_UNDEFTOOL_GOTOPAR,
CAN_NOT_ENUMPROP_UNDEFTOOL_PTMEASPAR,
CAN_NOT_GETPROP_PTMEASPARS_OF_NOTOOL,  
CAN_NOT_GETPROP_UNDEFTOOL,
CAN_NOT_GETPROPE_PTMEASPARS_OF_NOTOOL,
CAN_NOT_GETPROPE_UNDEFTOOL,
CAN_NOT_GOTO_USING_UNDEFTOOL,
CAN_NOT_PTMEAS_USING_NOTOOL,
CAN_NOT_PTMEAS_USING_UNDEFTOOL,
CAN_NOT_REQUALIFY_NOTOOL,
CAN_NOT_REQUALIFY_UNDEFTOOL,
CAN_NOT_SCAN_USING_NOTOOL,
CAN_NOT_SCAN_USING_UNDEFTOOL,
CAN_NOT_SET_TOOL_TO_UNDEFTOOL,
CAN_NOT_SETPROP_PTMEASPARS_OF_NOTOOL,
CAN_NOT_SETPROP_UNDEFTOOL,
DAEMON_TO_STOP_DOES_NOT_EXIST,
DIRECTION_VECTOR_HAS_ZERO_LENGTH,
DIS_ARGUMENT_MUST_BE_POSITIVE,
FIRST_VECTOR_NOT_NORMALIZED,
MUST_STOP_EXISTING_DAEMON_BEFORE_STARTING_ANOTHER,
NOT_HOMED_CAN_NOT_EXECUTE_COMMAND,
NOT_IN_SESSION_CAN_NOT_EXECUTE_COMMAND,
RADIUS_MUST_BE_PERPENDICULAR_TO_NORMAL,
RADIUS_MUST_HAVE_POSITIVE_LENGTH,
READINESS_ABORTED_CAN_NOT_EXECUTE_COMMAND,
READINESS_ERRED_CAN_NOT_EXECUTE_COMMAND,
SCAN_POINT_MUST_BE_IN_WORK_VOLUME,
SECOND_VECTOR_NOT_NORMALIZED,
START_POINT_MUST_DIFFER_FROM_DIRECTION_POINT,
START_POINT_MUST_DIFFER_FROM_END_POINT,
START_POINT_MUST_NOT_BE_ON_CYLINDER_AXIS,
TARGET_POINT_OUT_OF_WORK_VOLUME,
THETA_MUST_NOT_BE_NEGATIVE_OR_MORE_THAN_180,
TIME_ARGUMENT_MUST_NOT_BE_LESS_THAN_A_TENTH,
UNKNOWN_TOOL
};

class checkerCmd
{
public:
  checkerCmd(){}
  ~checkerCmd(){}
  void checkCommand(Command * aCommand, world * aWorld);
  int getCheckerErr(){return errorCode;}
  static const int getIppErr(int code);
  char * getErrorMessageString(char * buffer);
  
private:
  int errorCode;
  int commandName;
  void checkAbortE(Command * aCommand, world * aWorld);
  void checkAlignPart(Command * aCommand, world * aWorld);
  void checkAlignTool(Command * aCommand, world * aWorld);
  void checkCenterPart(Command * aCommand, world * aWorld);
  void checkChangeTool(Command * aCommand, world * aWorld);
  void checkClearAllErrors(Command * aCommand, world * aWorld);
  void checkDisableUser(Command * aCommand, world * aWorld);
  void checkEnableUser(Command * aCommand, world * aWorld);
  void checkEndSession(Command * aCommand, world * aWorld);
  void checkEnumAllProp(Command * aCommand, world * aWorld);
  void checkEnumProp(Command * aCommand, world * aWorld);
  void checkEnumTools(Command * aCommand, world * aWorld);
  void checkFindTool(Command * aCommand, world * aWorld);
  void checkGet(Command * aCommand, world * aWorld);
  void checkGetChangeToolAction(Command * aCommand, world * aWorld);
  void checkGetCoordSystem(Command * aCommand, world * aWorld);
  void checkGetCsyTransformation(Command * aCommand, world * aWorld);
  void checkGetDMEVersion(Command * aCommand, world * aWorld);
  void checkGetErrorInfo(Command * aCommand, world * aWorld);
  void checkGetErrStatusE(Command * aCommand, world * aWorld);
  void checkGetMachineClass(Command * aCommand, world * aWorld);
  void checkGetProp(Command * aCommand, world * aWorld);
  void checkGetPropE(Command * aCommand, world * aWorld);
  void checkGetXtdErrStatus(Command * aCommand, world * aWorld);
  void checkGoTo(Command * aCommand, world * aWorld);
  void checkHome(Command * aCommand, world * aWorld);
  void checkIsHomed(Command * aCommand, world * aWorld);
  void checkIsUserEnabled(Command * aCommand, world * aWorld);
  void checkLockAxis(Command * aCommand, world * aWorld);
  void checkOnMoveReportE(Command * aCommand, world * aWorld);
  void checkOnPtMeasReport(Command * aCommand, world * aWorld);
  void checkOnScanReport(Command * aCommand, world * aWorld);
  void checkPtMeas(Command * aCommand, world * aWorld);
  void checkReQualify(Command * aCommand, world * aWorld);
  void checkScanInCylEndIsPlane(Command * aCommand, world * aWorld);
  void checkScanInCylEndIsSphere(Command * aCommand, world * aWorld);
  void checkScanInPlaneEndIsCyl(Command * aCommand, world * aWorld);
  void checkScanInPlaneEndIsPlane(Command * aCommand, world * aWorld);
  void checkScanInPlaneEndIsSphere(Command * aCommand, world * aWorld);
  void checkScanOnCircle(Command * aCommand, world * aWorld);
  void checkScanOnCircleHint(Command * aCommand, world * aWorld);
  void checkScanOnLine(Command * aCommand, world * aWorld);
  void checkScanOnLineHint(Command * aCommand, world * aWorld);
  void checkScanUnknownHint(Command * aCommand, world * aWorld);
  void checkSetCoordSystem(Command * aCommand, world * aWorld);
  void checkSetCsyTransformation(Command * aCommand, world * aWorld);
  void checkSetProp(Command * aCommand, world * aWorld);
  void checkSetTool(Command * aCommand, world * aWorld);
  void checkStartSession(Command * aCommand, world * aWorld);
  void checkStopAllDaemons(Command * aCommand, world * aWorld);
  void checkStopDaemon(Command * aCommand, world * aWorld);
  void checkTiltCenterPart(Command * aCommand, world * aWorld);
  void checkTiltPart(Command * aCommand, world * aWorld);
};

#endif /* #ifndef CHECKERCMD_HH */

