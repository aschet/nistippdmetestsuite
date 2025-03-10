#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <fstream>  //hui-101802
#include <iostream> //hui-101802

#include "../Cmm/world.h"
#include "../Server/ServerQueues.h"
#include "../CmdResClasses/IppCmdRes.h"
#include "../TrajGen/TrajGen.h"

#include <stdlib.h>
#include <vector>

using namespace std;
using namespace ipp;



class Executor
{
 public:
  
  Executor(world &);
  ~Executor(){};
  void executeCommand(Command *CmdToCarryOut);
  void doThis();
  ResponseList checkStatus();
  void resetExecutor();

 private:
/* the following functions (executeCommandName) will execute 
   the command by modifing the world, and send a response string
   to the response queue.  If a command performs a move, the trajectory
   generator is called
*/


  void executeAbortE();
  void executeAlignPart();
  void executeAlignTool();
  void executeCenterPart();
  void executeChangeTool();
  void executeClearAllErrors();
  void executeDisableUser();
  void executeEnableUser();
  void executeEndSession();
  void executeEnumAllProp();
  void executeEnumProp();
  void executeEnumTools();
  void executeFindTool();
  void executeGet();
  void executeGetChangeToolAction();
  void executeGetCoordSystem();
  void executeGetCsyTransformation();
  void executeGetDMEVersion();
  void executeGetErrorInfo();
  void executeGetErrStatusE();
  void executeGetMachineClass();
  void executeGetProp();
  void executeGetPropE();
  void executeGetXtdErrStatus();
  void executeGoTo();
  void executeHome();
  void executeIsHomed();
  void executeIsUserEnabled();
  void executeLockAxis();
  void executeOnMoveReportE();
  void executeOnPtMeasReport();
  void executeOnScanReport();
  void executePtMeas();
  void executeReQualify();
  void executeScanInCylEndIsPlane();
  void executeScanInCylEndIsSphere();
  void executeScanInPlaneEndIsCyl();
  void executeScanInPlaneEndIsPlane();
  void executeScanInPlaneEndIsSphere();
  void executeScanOnCircle();
  void executeScanOnCircleHint();
  void executeScanOnLine();
  void executeScanOnLineHint();
  void executeScanUnknownHint();
  void executeSetCoordSystem();
  void executeSetCsyTransformation();
  void executeSetProp();
  void executeSetTool();
  void executeStartSession();
  void executeStopAllDaemons();
  void executeStopDaemon();
  void executeTiltCenterPart();
  void executeTiltPart();

  void (Executor::*ExecutionList[55])();

  void clearResponseList();

  //helper functions
  void initExecutionList();
  int testInSession();
  void generateCompletedResponse();
  void generateErrorResponse(errorNameType, char *);
  void EnumPropHelper(bool);

  void computeRetract();
  void onMoveDaemonReport();
  void onScanReportData();

  void initWorld();

  //private data members
  double cmdVel;   //102502-hui moved here since the val set by goto but also used in later ptmeas
  ResponseList returningValues;
  world * OutsideWorld;
  ServerQueues * ServerData; 
  Command *CurrentCommand;
  Command *MultiCycleCommand;  //101702-hui per joe
  bool multi_cmd_executing;    //101702-hui per joe
  bool all_errors_cleared;
  bool abort_flag;
  bool end_session_waiting;

  ToolChanger *tools;

  KTool * previousTool;
  
  TrajGen * trajectory;

};

#endif




