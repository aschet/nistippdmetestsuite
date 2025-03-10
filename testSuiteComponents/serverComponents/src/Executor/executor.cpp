#include "../Server/StdAfx.h"
#include "executor.h"
#include "../CmdParser/parserCmd.h"
#include "../Server/Logger.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
 
extern Logger serverLog;
//char msag[1024] = "";

ResponseList * retVals;

//OnMoveReportECommand *currentOnMoveReportE;

using namespace ipp;

//////////////////////////////////////////////////////////////////
//executor must get the world information

Executor::Executor(world & Outside)
{
  initExecutionList();
  retVals = &returningValues;
  OutsideWorld = &Outside;
  tools = OutsideWorld->getTools();
  trajectory = new TrajGen(OutsideWorld, retVals);
  initWorld();
  previousTool = tools->getActiveTool();
}

////////////////////////////////////////////////////////////////
void Executor::executeCommand(Command * CmdToCarryOut)
{
  returningValues.reserve(25);
  CurrentCommand = CmdToCarryOut;

  commandNameType cnt = CurrentCommand->getCommandName();

  (*this.*ExecutionList[(int)cnt])();  // Command specific execution
}

//////////////////////////////////////////////////////////////////

void Executor::initExecutionList()
{
  ExecutionList[0]  = 0;
  ExecutionList[1]  = 0;
  ExecutionList[2]  = executeAbortE;
  ExecutionList[3]  = executeAlignPart;
  ExecutionList[4]  = executeAlignTool;
  ExecutionList[5]  = executeCenterPart;
  ExecutionList[6]  = executeChangeTool;
  ExecutionList[7]  = executeClearAllErrors;
  ExecutionList[8]  = executeDisableUser;
  ExecutionList[9]  = executeEnableUser;
  ExecutionList[10] = executeEndSession;
  ExecutionList[11] = executeEnumAllProp;
  ExecutionList[12] = executeEnumProp;
  ExecutionList[13] = executeEnumTools;
  ExecutionList[14] = executeFindTool;
  ExecutionList[15] = executeGet;
  ExecutionList[16] = executeGetChangeToolAction;
  ExecutionList[17] = executeGetCoordSystem;
  ExecutionList[18] = executeGetCsyTransformation;
  ExecutionList[19] = executeGetDMEVersion;
  ExecutionList[20] = executeGetErrorInfo;
  ExecutionList[21] = executeGetErrStatusE;
  ExecutionList[22] = executeGetMachineClass;
  ExecutionList[23] = executeGetProp;
  ExecutionList[24] = executeGetPropE;
  ExecutionList[25] = executeGetXtdErrStatus;
  ExecutionList[26] = executeGoTo;
  ExecutionList[27] = executeHome;
  ExecutionList[28] = executeIsHomed;
  ExecutionList[29] = executeIsUserEnabled;
  ExecutionList[30] = executeLockAxis;
  ExecutionList[31] = executeOnMoveReportE;
  ExecutionList[32] = executeOnPtMeasReport;
  ExecutionList[33] = executeOnScanReport;
  ExecutionList[34] = executePtMeas;
  ExecutionList[35] = executeReQualify;
  ExecutionList[36] = executeScanInCylEndIsPlane;
  ExecutionList[37] = executeScanInCylEndIsSphere;
  ExecutionList[38] = executeScanInPlaneEndIsCyl;
  ExecutionList[39] = executeScanInPlaneEndIsPlane;
  ExecutionList[40] = executeScanInPlaneEndIsSphere;
  ExecutionList[41] = executeScanOnCircle;
  ExecutionList[42] = executeScanOnCircleHint;
  ExecutionList[43] = executeScanOnLine;
  ExecutionList[44] = executeScanOnLineHint;
  ExecutionList[45] = executeScanUnknownHint;
  ExecutionList[46] = executeSetCoordSystem;
  ExecutionList[47] = executeSetCsyTransformation;
  ExecutionList[48] = executeSetProp;
  ExecutionList[49] = executeSetTool;
  ExecutionList[50] = executeStartSession;
  ExecutionList[51] = executeStopAllDaemons;
  ExecutionList[52] = executeStopDaemon;
  ExecutionList[53] = executeTiltCenterPart;
  ExecutionList[54] = executeTiltPart;
}

//////////////////// 2 - executeAbortE() //////////////////////////////
void  Executor::executeAbortE(){

  if (OutsideWorld->getReadiness() == moving)
	trajectory->abortCommandExecution();

  OutsideWorld->killDaemons();

  generateCompletedResponse();

}  // end executeAbortE()


///////////////////// 3 - executeAlignPart() ////////////////////////

void Executor::executeAlignPart()
{
  /* Not Implemented in this release.
     This would have to be implemented based on some rotary table model 

     For now, simply returns the a response with the values sent and then returns done.	
  */
  AlignPartCommand * theAlign = (AlignPartCommand*)CurrentCommand;
  AlignResponse *r;

  double inI1=0, inJ1=0, inK1=0, inI2=0, inJ2=0, inK2=0;
  bool inHas2;
  unsigned int inTagNumber;

  inTagNumber = theAlign->getTag()->getTagNumber();

  inHas2 = theAlign->getHas2();
  inI1 = theAlign->getMachineVector1i();
  inJ1 = theAlign->getMachineVector1j();
  inK1 = theAlign->getMachineVector1k();
  
  if (inHas2){
	  inI2 = theAlign->getMachineVector2i();
	  inJ2 = theAlign->getMachineVector2j();
	  inK2 = theAlign->getMachineVector2k();
  }
  
  r = new AlignResponse(inTagNumber, inI1, inJ1, inK1, inI2, inJ2, inK2, inHas2);

  returningValues.push_back(r);

  generateCompletedResponse();

} // end executeAlignPart()


///////////////////// 4 - executeAlignTool() ////////////////////////

void  Executor::executeAlignTool()
{
  /* Not Implemented in this release.
     This would have to be implemented based on some tool model 

     For now, simply returns the a response with the values sent and then returns done.	
  */
  AlignToolCommand * theAlign = (AlignToolCommand*)CurrentCommand;
  AlignResponse * r;

  double inI1=0, inJ1=0, inK1=0, inI2=0, inJ2=0, inK2=0;
  bool inHas2;
  unsigned int inTagNumber;

  inTagNumber = theAlign->getTag()->getTagNumber();

  inHas2 = theAlign->getHas2();
  inI1 = theAlign->getVector1i();
  inJ1 = theAlign->getVector1j();
  inK1 = theAlign->getVector1k();
  
  if (inHas2){
	  inI2 = theAlign->getVector2i();
	  inJ2 = theAlign->getVector2j();
	  inK2 = theAlign->getVector2k();
  }
  
  r = new AlignResponse(inTagNumber, inI1, inJ1, inK1, inI2, inJ2, inK2, inHas2);

  returningValues.push_back(r);

  generateCompletedResponse();
  
} // end executeAlignTool()


/////////////////////// 5 - executeCenterPart() //////////////////////

void Executor::executeCenterPart()
{

	OutsideWorld->update(CurrentCommand);

	CenterPartResponse *r = new CenterPartResponse(
			CurrentCommand->getTag()->getTagNumber(), true); //isCentered may come from world
    returningValues.push_back(r);
	
	generateCompletedResponse();


}

/////////////////////// 6 - executeChangeTool() ////////////////////////

void  Executor::executeChangeTool()
{
  previousTool = tools->getActiveTool();
  OutsideWorld->update(CurrentCommand); // Execution world model changes
                                        // Sets the active tool

  generateCompletedResponse();

} // end executeChangeTool()


///////////////////// 7 - executeClearAllErrors() //////////////////////

void  Executor::executeClearAllErrors()
{
  OutsideWorld->update(CurrentCommand); // Execution world model changes
                                        // World model readiness set to ready
  generateCompletedResponse();

} // end executeClearAllErrors()




/////////////////////// 8 - executeDisableUser() //////////////////////

void  Executor::executeDisableUser()
{
	OutsideWorld->update(CurrentCommand); // Execution world model changes
                                          // WM user_enabled flag to false
	generateCompletedResponse();

} // end executeDisableUser()


/////////////////////// 9 - executeEnableUser() ///////////////////////

void  Executor::executeEnableUser()
{
	OutsideWorld->update(CurrentCommand); // Execution world model changes
                                          // WM user_enabled flag to true
	generateCompletedResponse();

} // end executeEnableUser()


/////////////////////// 10 - executeEndSession() ///////////////////////

void  Executor::executeEndSession()
{
  OutsideWorld->update(CurrentCommand);   // Execution world model changes
                                          // WM in_session flag to false
                                          // WM is_homed flag to false
  generateCompletedResponse();

} // end executeEndSession()


///////////////////// 11 - executeEnumAllProp() ////////////////////////

void  Executor::executeEnumAllProp()
{
   EnumAllPropCommand * theSet = (EnumAllPropCommand*)CurrentCommand;
  EnumPropResponse * r;

  unsigned int tag_num = theSet->getTag()->getTagNumber();	
  int numCmds = theSet->getNumberKeys();

   if(numCmds == 1 && (theSet->getKey1() == Tool || theSet->getKey1() == FoundTool)) {
	r = new EnumPropResponse(tag_num, "GoToPar", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "PtMeasPar", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract.Min", "Number");
	returningValues.push_back(r);

  }
  
  else if(numCmds == 2 && theSet->getKey2() == PtMeasPar){
	r = new EnumPropResponse(tag_num, "Speed", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract.Min", "Number");
	returningValues.push_back(r);
  }

  else if(numCmds == 2 && theSet->getKey2() == GoToPar){
    r = new EnumPropResponse(tag_num, "Speed", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed.Min", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel.Min", "Number");
	returningValues.push_back(r);
  }

  else if(numCmds == 3) {
	r = new EnumPropResponse(tag_num, "Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Min", "Number");
	returningValues.push_back(r);
  }

	generateCompletedResponse();

}  // end executeEnumAllProp()


/////////////////////// 12 - executeEnumProp() ///////////////////////

void Executor::executeEnumProp()  ///***needs work - talk to tom about format
{
  EnumPropCommand * theSet = (EnumPropCommand*)CurrentCommand;
  EnumPropResponse * r;

  unsigned int tag_num = theSet->getTag()->getTagNumber();	
  int numCmds = theSet->getNumberKeys();

  if(numCmds == 1 && (theSet->getKey1() == Tool || theSet->getKey1() == FoundTool)) {
	r = new EnumPropResponse(tag_num, "GoToPar", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "PtMeasPar", "Property");
	returningValues.push_back(r);
  }
  
  else if(numCmds == 2 && theSet->getKey2() == PtMeasPar){
	r = new EnumPropResponse(tag_num, "Speed", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Speed", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Approach", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Search", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Retract", "Property");
	returningValues.push_back(r);
  }

  else if(numCmds == 2 && theSet->getKey2() == GoToPar){
	r = new EnumPropResponse(tag_num, "Speed", "Number");
	returningValues.push_back(r);
    r = new EnumPropResponse(tag_num, "Speed", "Property");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Accel", "Property");
	returningValues.push_back(r);
	
	
  }
  else if(numCmds == 3) {
	r = new EnumPropResponse(tag_num, "Act", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Def", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Max", "Number");
	returningValues.push_back(r);
	r = new EnumPropResponse(tag_num, "Min", "Number");
	returningValues.push_back(r);
  }

	generateCompletedResponse();

} // end executeEnumProp()


//!!/////////////////// 13 - executeEnumTools() ////////////////////////

void Executor::executeEnumTools()
{
  vector<KTool *> listOfTools = tools->getToolList();
  vector<KTool *>::iterator p;
  unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
  for (p = listOfTools.begin(); p != listOfTools.end(); p++)
  {
	KTool * kt = *p;
	if(kt->getName() != "UnDefTool" && kt->getName() != "BaseTool") {
      StringResponse *r = new StringResponse(tag_num, kt->getName());
      returningValues.push_back(r);
	}
  }
  
  generateCompletedResponse();

} // end executeEnumTools()


/////////////////////// 14 - executeFindTool() /////////////////////////
void  Executor::executeFindTool()
{
  OutsideWorld->update(CurrentCommand);		//World updates found tool

  generateCompletedResponse();

} // end executeFindTool()


/////////////////////// 15 - executeGet() //////////////////////////////

void  Executor::executeGet()
{
  double er=0, i=0, j=0, k=0, r=0, x=0, y=0, z=0, toolA=0, toolB=0, toolC=0;
  bool hasER = false, hasIJK = false, hasR = false;
  bool hasX = false, hasY = false, hasZ = false;
  bool hasToolA = false, hasToolB = false, hasToolC = false;
  GetCommand * cmd = (GetCommand *) CurrentCommand;

  
  if (cmd->getHasR()){
	r = OutsideWorld->getCurrentR();
	hasR = true;
  }
  if (cmd->getHasX()){
    x = OutsideWorld->getCurrentX();
    hasX = true;
  }
  if (cmd->getHasY()){
    y = OutsideWorld->getCurrentY();
    hasY = true;
  }
  if (cmd->getHasZ()){
    z = OutsideWorld->getCurrentZ();
    hasZ = true;
  }
  if (cmd->getHasToolA()){
    toolA = OutsideWorld->getCurrentToolA();
    hasToolA = true;
  }
  if (cmd->getHasToolB()){
    toolB = OutsideWorld->getCurrentToolB();
    hasToolB = true;
  }
  if (cmd->getHasToolC()){
    toolC = OutsideWorld->getCurrentToolC();
    hasToolC = true;
  }

  if(hasER==true || hasIJK==true || hasR==true || hasX==true || hasY==true || hasZ==true || 
		hasToolA==true || hasToolB==true || hasToolC==true)
  {
    unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
    PtMeasOrGetResponse * res = new PtMeasOrGetResponse(tag_num, 
		hasER, hasIJK, hasR, hasToolA, hasToolB, hasToolC, hasX, hasY, hasZ, 
	    er, i, j, k, r, toolA, toolB, toolC, x, y, z);
  
    returningValues.push_back(res);

    generateCompletedResponse();
  }

}  // end executeGet()


//////////////// 16 - executeGetChangeToolAction() //////////////////// **??**

void Executor::executeGetChangeToolAction()
{
	double toolDiff = 0;
	toolDiff = previousTool->getLength() - (tools->getActiveTool()->getLength()); 
	unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
	GetChangeToolActionResponse * res = new GetChangeToolActionResponse(tag_num, 
		Switch,0,0,toolDiff);
			                                //  changeToolActionType inChangeType,
			                                //  double inX, double inY, double inZ);

	returningValues.push_back(res);

    generateCompletedResponse();

}

////////////////// 17 - executeGetCoordSystem() ///////////////////////

void  Executor::executeGetCoordSystem()
{
   unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
  coordSysType i = OutsideWorld->getCurrentCsy();
  Response * x = new GetCoordSystemResponse(tag_num, i);
  returningValues.push_back(x);

  generateCompletedResponse();

} // end executeGetCoordSystem()


///////////////// 18 - executeGetCsyTransfromation() //////////////////

void  Executor::executeGetCsyTransformation()
{
  unsigned int tag_num; 
  tag_num = CurrentCommand->getTag()->getTagNumber();
  GetCsyTransformationCommand * gcsytc;
  gcsytc = (GetCsyTransformationCommand*)CurrentCommand;
  coordSysType cst = gcsytc->getCoordSys();
  int cccc = (int)(gcsytc->getCoordSys());
 
  csyTransform * cT;
 
  switch (cccc){
    case JogDisplayCsy :
      cT = OutsideWorld->getJogDisplayCsyTransform();
      break;
    case JogMoveCsy :
      cT = OutsideWorld->getJogMoveCsyTransform();
      break;
    case MoveableMachineCsy :
      cT = OutsideWorld->getMoveableCsyTransform();
      break;
    case MultipleArmCsy :
      cT = OutsideWorld->getMultipleCsyTransform();
      break;
    case PartCsy :
      cT = OutsideWorld->getPartCsyTransform();
      break;
    case SensorCsy :
      cT = OutsideWorld->getSensorCsyTransform(); 
      break;
  }
 
  Response * x = new GetCsyTransformationResponse
	(tag_num, cT->getX(), cT->getY(), cT->getZ(), 
	    cT->getTheta(), cT->getPsi(), cT->getPhi());
  returningValues.push_back(x);

  generateCompletedResponse();

} // end executeGetCsyTransformation()


///////////////////19 - executeGetDMEVersion() ///////////////////////

void Executor::executeGetDMEVersion()
{
	unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
	GetDMEVersionResponse *r= new GetDMEVersionResponse(tag_num, "1.4");
	returningValues.push_back(r);
	generateCompletedResponse();
}

///////////////////20 - executeGetErrorInfo() ////////////////////////

void  Executor::executeGetErrorInfo()
{
 GetErrorInfoCommand * cmd = (GetErrorInfoCommand *) CurrentCommand;
  char * error_txt = (char *)cmd->getTheError()->getText();
  
  unsigned int tag_num = cmd->getTag()->getTagNumber();
  StringResponse *r = new StringResponse(tag_num, error_txt);
  returningValues.push_back(r);

  generateCompletedResponse();

} // end executeGetErrorInfo()


/////////////////// 21 - executeGetErrStatusE() /////////////////////////

void  Executor::executeGetErrStatusE()
{
  unsigned int tag_ID = CurrentCommand->getTag()->getTagNumber();
  bool errExists = false;
  if(OutsideWorld->getReadiness() == erred)
	  errExists = true;
  GetErrStatusEResponse * x = new GetErrStatusEResponse (tag_ID, errExists);
  returningValues.push_back(x);

  generateCompletedResponse();

} // end executeGetErrStatusE()


/////////////////// 22 - executeGetMachineClass() ////////////////////////

void  Executor::executeGetMachineClass()
{
  unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
  GetMachineClassResponse * r = new GetMachineClassResponse(tag_num);
  r->setClassName(OutsideWorld->getMachineClass());

  returningValues.push_back(r);

  generateCompletedResponse();

} // end executeGetMachineClass()


/////////////////// 23 - executeGetProp() ////////////////////////////////

//1.4  Need to see how Tom handles the world.  Once implemented this must be duplicated for GetPropE

void  Executor::executeGetProp()
{
  GetPropCommand * theSet;
  theSet = (GetPropCommand *)CurrentCommand;

  double val = 0;
  bool isFound;

  KTool * theTool;
  GoToPars * pars1;
  PtMeasPars * pars2;

  int numberProps = theSet->getNumberProps();
 
  OneProperty ** oneProps;
  oneProps = new OneProperty * [numberProps];
 

  GetPropResponse * res = new GetPropResponse(CurrentCommand->getTag()->getTagNumber(),
                                              CurrentCommand->getTag()->getTagType(),
											  numberProps, oneProps);
for (int n=0; n<numberProps; n++){

    if (theSet->getKey1(n) == Tool) {
      theTool = tools->getActiveTool();
	  isFound = false;
	}
    else if (theSet->getKey1(n) == FoundTool){
      theTool = tools->getFoundTool();
	  isFound = true;
	}
    


	if (theSet->getKey2(n) == Alignment)							//**??**
	{		
		res->setOneProp(OutsideWorld->getCurrentAlignment(),n);		
	}

	else if (theSet->getKey2(n) == CollisionVolume)
	{
		res->setOneProp(theTool->getCollisionVolume(),n);
	}

	else if (theSet->getKey2(n) == Id)
	{
		res->setOneProp(new OnePropertyId(isFound, theTool->getId()),n);
	}

	else if (theSet->getKey2(n) == Name)
	{
		res->setOneProp(new OnePropertyName(isFound, theTool->getName()),n);
	}

	else if (theSet->getKey2(n) == GoToPar &&
			 theTool != tools->getUnDefTool())
	{
	    pars1 = theTool->getGoToPar();
	    switch (theSet->getKey3(n))
		{
	      case Speed:
			if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars1->getActSpeed();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
					   val = pars1->getActSpeed();
						break;
					case Default:
						val = pars1->getDefSpeed();
						break;
					case Max:
						val = pars1->getMaxSpeed();
						break;
					case Min:
						val = pars1->getMinSpeed();
						break;
					default:
						break;
				}
			}
	        break;

		  case Accel:
			if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars1->getActAccel();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
						val = pars1->getActAccel();
						break;
					case Default:
						val = pars1->getDefAccel();
						break;
					case Max:
						val = pars1->getMaxAccel();
						break;
					case Min:
						val = pars1->getMinAccel();
						break;
					default:
						break;
				}
			  }
	        break;

		  default:
	        break;
		}
		
		if (theSet->getKey4(n) == NULL)
			res->setOneProp(new OnePropertyNumber(theSet->getKey1(n),
                                                  theSet->getKey2(n),
												  theSet->getKey3(n),
												  val),n);
		else
			res->setOneProp(new OnePropertyNumber(theSet->getKey1(n), 
                                                  theSet->getKey2(n),
												  theSet->getKey3(n),
												  theSet->getKey4(n),
												  val),n);
	}

	else if (theSet->getKey2(n) == PtMeasPar &&
		     theTool != tools->getNoTool())
	{   
	    pars2 = theTool->getPtMeasPar();
		switch (theSet->getKey3(n))
		{
	      case Speed:
			if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars2->getActSpeed();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
					   val = pars2->getActSpeed();
						break;
					case Default:
						val = pars2->getDefSpeed();
						break;
					case Max:
						val = pars2->getMaxSpeed();
						break;
					case Min:
						val = pars2->getMinSpeed();
						break;
					default:
						break;
				}
			}
	        break;

		  case Accel:
			if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars2->getActAccel();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
						val = pars2->getActAccel();
						break;
					case Default:
						val = pars2->getDefAccel();
						break;
					case Max:
						val = pars2->getMaxAccel();
						break;
					case Min:
						val = pars2->getMinAccel();
						break;
					default:
						break;
				}
			  }
	        break;
	    
		  case Approach:
	          if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars2->getActApproach();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
						val = pars2->getActApproach();
						break;
					case Default:
						val = pars2->getDefApproach();
						break;
					case Max:
						val = pars2->getMaxApproach();
						break;
					case Min:
						val = pars2->getMinApproach();
						break;
					default:
						break;
				}
			  }
	        break;

	      case Retract:
	          if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars2->getActRetract();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
						val = pars2->getActRetract();
						break;
					case Default:
						val = pars2->getDefRetract();
						break;
					case Max:
						val = pars2->getMaxRetract();
						break;
					case Min:
						val = pars2->getMinRetract();
						break;
					default:
						break;
				}
			  }
	        break;

	      case Search:
	          if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars2->getActSearch();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
						val = pars2->getActSearch();
						break;
					case Default:
						val = pars2->getDefSearch();
						break;
					case Max:
						val = pars2->getMaxSearch();
						break;
					case Min:
						val = pars2->getMinSearch();
						break;
					default:
						break;
				}
			  }
	        break;

		  default:
	        break;
		}

		if (theSet->getKey4(n) == NULL)
			res->setOneProp(new OnePropertyNumber(theSet->getKey1(n), 
                                                  theSet->getKey2(n),
												  theSet->getKey3(n),
												  val),n);
		else
			res->setOneProp(new OnePropertyNumber(theSet->getKey1(n), 
                                                  theSet->getKey2(n),
												  theSet->getKey3(n),
												  theSet->getKey4(n),
												  val),n);
	}
  }

  

  returningValues.push_back(res);

  generateCompletedResponse();

} // end executeGetProp()


/////////////////// 24 - executeGetPropE() //////////////////////////
void  Executor::executeGetPropE()
{
  GetPropECommand * theSet;
  theSet = (GetPropECommand *)CurrentCommand;

  double val = 0;
  bool isFound;

  KTool * theTool;
  GoToPars * pars1;
  PtMeasPars * pars2;

  int numberProps = theSet->getNumberProps();
 
  OneProperty ** oneProps;
  oneProps = new OneProperty * [numberProps];
 

  GetPropResponse * res = new GetPropResponse(CurrentCommand->getTag()->getTagNumber(),
                                              CurrentCommand->getTag()->getTagType(),
											  numberProps, oneProps);
for (int n=0; n<numberProps; n++){

    if (theSet->getKey1(n) == Tool) {
      theTool = tools->getActiveTool();
	  isFound = false;
	}
    else if (theSet->getKey1(n) == FoundTool){
      theTool = tools->getFoundTool();
	  isFound = true;
	}
    


	if (theSet->getKey2(n) == Alignment)							//**??**
	{		
		res->setOneProp(OutsideWorld->getCurrentAlignment(),n);		
	}

	else if (theSet->getKey2(n) == CollisionVolume)
	{
		res->setOneProp(theTool->getCollisionVolume(),n);
	}

	else if (theSet->getKey2(n) == Id)
	{
		res->setOneProp(new OnePropertyId(isFound, theTool->getId()),n);
	}

	else if (theSet->getKey2(n) == Name)
	{
		res->setOneProp(new OnePropertyName(isFound, theTool->getName()),n);
	}

	else if (theSet->getKey2(n) == GoToPar &&
			 theTool != tools->getUnDefTool())
	{
	    pars1 = theTool->getGoToPar();
	    switch (theSet->getKey3(n))
		{
	      case Speed:
			if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars1->getActSpeed();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
					   val = pars1->getActSpeed();
						break;
					case Default:
						val = pars1->getDefSpeed();
						break;
					case Max:
						val = pars1->getMaxSpeed();
						break;
					case Min:
						val = pars1->getMinSpeed();
						break;
					default:
						break;
				}
			}
	        break;

		  case Accel:
			if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars1->getActAccel();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
						val = pars1->getActAccel();
						break;
					case Default:
						val = pars1->getDefAccel();
						break;
					case Max:
						val = pars1->getMaxAccel();
						break;
					case Min:
						val = pars1->getMinAccel();
						break;
					default:
						break;
				}
			  }
	        break;

		  default:
	        break;
		}
		
		if (theSet->getKey4(n) == NULL)
			res->setOneProp(new OnePropertyNumber(theSet->getKey1(n), 
                                                  theSet->getKey2(n),
												  theSet->getKey3(n),
												  val),n);
		else
			res->setOneProp(new OnePropertyNumber(theSet->getKey1(n), 
                                                  theSet->getKey2(n),
												  theSet->getKey3(n),
												  theSet->getKey4(n),
												  val),n);
	}

	else if (theSet->getKey2(n) == PtMeasPar &&
		     theTool != tools->getNoTool())
	{   
	    pars2 = theTool->getPtMeasPar();
		switch (theSet->getKey3(n))
		{
	      case Speed:
			if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars2->getActSpeed();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
					   val = pars2->getActSpeed();
						break;
					case Default:
						val = pars2->getDefSpeed();
						break;
					case Max:
						val = pars2->getMaxSpeed();
						break;
					case Min:
						val = pars2->getMinSpeed();
						break;
					default:
						break;
				}
			}
	        break;

		  case Accel:
			if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars2->getActAccel();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
						val = pars2->getActAccel();
						break;
					case Default:
						val = pars2->getDefAccel();
						break;
					case Max:
						val = pars2->getMaxAccel();
						break;
					case Min:
						val = pars2->getMinAccel();
						break;
					default:
						break;
				}
			  }
	        break;
	    
		  case Approach:
	          if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars2->getActApproach();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
						val = pars2->getActApproach();
						break;
					case Default:
						val = pars2->getDefApproach();
						break;
					case Max:
						val = pars2->getMaxApproach();
						break;
					case Min:
						val = pars2->getMinApproach();
						break;
					default:
						break;
				}
			  }
	        break;

	      case Retract:
	          if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars2->getActRetract();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
						val = pars2->getActRetract();
						break;
					case Default:
						val = pars2->getDefRetract();
						break;
					case Max:
						val = pars2->getMaxRetract();
						break;
					case Min:
						val = pars2->getMinRetract();
						break;
					default:
						break;
				}
			  }
	        break;

	      case Search:
	          if (theSet->getKey4(n) == EmptyKey)
			{
	            val = pars2->getActSearch();
			}
			else 
			{
				switch (theSet->getKey4(n)) {
					case Actual:
						val = pars2->getActSearch();
						break;
					case Default:
						val = pars2->getDefSearch();
						break;
					case Max:
						val = pars2->getMaxSearch();
						break;
					case Min:
						val = pars2->getMinSearch();
						break;
					default:
						break;
				}
			  }
	        break;

		  default:
	        break;
		}

		if (theSet->getKey4(n) == NULL)
			res->setOneProp(new OnePropertyNumber(theSet->getKey1(n), 
                                                  theSet->getKey2(n),
												  theSet->getKey3(n),
												  val),n);
		else
			res->setOneProp(new OnePropertyNumber(theSet->getKey1(n), 
                                                  theSet->getKey2(n),
												  theSet->getKey3(n),
												  theSet->getKey4(n),
												  val),n);
	}
  }

  

  returningValues.push_back(res);

  generateCompletedResponse();
  

} // end executeGetPropE()


//////////////// 25 - executeGetXtdErrStatus() //////////////////////

void  Executor::executeGetXtdErrStatus()
{
  unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();

  IsUserEnabledResponse *x = new IsUserEnabledResponse(tag_num, OutsideWorld->getUserEnabled());
  returningValues.push_back(x);
  
  IsHomedResponse *y = new IsHomedResponse(tag_num, OutsideWorld->getIsHomed());
  returningValues.push_back(y);

  generateCompletedResponse();

} // end executeGetXtdErrStatus()



//////////////////// 26 - executeGoTo() /////////////////////////////


void Executor::executeGoTo()
{
	trajectory->executeTraj(CurrentCommand);

} // end executeGoTo()



////////////////////// 27 - executeHome() ////////////////////////////

void Executor::executeHome()
{
	//OutsideWorld->update(CurrentCommand); this is bad

	trajectory->executeTraj(CurrentCommand);

} // end executeHome()


///////////////////// 28 - executeIsHomed() //////////////////////////

void  Executor::executeIsHomed()
{
	unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
	IsHomedResponse *x = new IsHomedResponse 
		(tag_num, OutsideWorld->getIsHomed());
	returningValues.push_back(x);

	generateCompletedResponse();

} // end executeIsHomed()


///////////////////// 29 - executeIsUserEnabled() //////////////////////

void  Executor::executeIsUserEnabled()
{
	unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
	IsUserEnabledResponse *x = new IsUserEnabledResponse
		(tag_num, OutsideWorld->getUserEnabled());
	returningValues.push_back(x);

	generateCompletedResponse();

} // end executeIsUserEnabled()


//////////////////////// 30 - executeLockAxis() /////////////////////

void Executor::executeLockAxis()
{
	OutsideWorld->update(CurrentCommand);

	generateCompletedResponse();
}

///////////////////// 31 - executeOnMoveReportE() //////////////////

void  Executor::executeOnMoveReportE()
{

  // for this implementation only one OnMoveReport daemon at a time
  // another OnMoveReportE command kills the active daemon and starts
  // a new one.
  
  //currentOnMoveReportE = (OnMoveReportECommand *)CurrentCommand;

  if(OutsideWorld->getOnMoveTag() != 0)
  {
	char cmdStr[IPPSIZE/2] = "";
	CurrentCommand->getCommandString(cmdStr,IPPSIZE-1);
    serverLog.replaceQuotes(cmdStr);
  
	Response * x = new ErrorResponse(CurrentCommand->getTag()->getTagNumber(), 
		                               CurrentCommand->getTag()->getTagType(),
									   DaemonAlreadyExists, cmdStr);
  }
  else
  {
	OutsideWorld->update(CurrentCommand);
	generateCompletedResponse();
  }

  
  
} // end executeOnMoveReportE()


/////////////////// 32 - executeOnPtMeasReport() /////////////////////

void  Executor::executeOnPtMeasReport()
{
  OutsideWorld->update(CurrentCommand);

  generateCompletedResponse();

} // end executeOnPtMeasReport()



///////////////// 33 - executeOnScanReport() /////////////////////

void  Executor::executeOnScanReport()
{
  OutsideWorld->update(CurrentCommand);

  generateCompletedResponse();

} // end executeOnScanReport()



////////////////////// 34 - executePtMeas() ////////////////////////

void  Executor::executePtMeas()

{
	trajectory->executeTraj(CurrentCommand);

} // end executePtMeas()


//////////////////// 35 - executeReQualify() ////////////////////////

void  Executor::executeReQualify()
{
  OutsideWorld->update(CurrentCommand);

  generateCompletedResponse();

} // executeReQualify()


/////////////// 36 - executeScanInCylEndIsPlane() ////////////////////

void  Executor::executeScanInCylEndIsPlane()
{
  trajectory->executeTraj(CurrentCommand);

} // executeScanInCylEndIsPlane()


/////////////// 37 - executeScanInCylEndIsSphere() ///////////////////

void  Executor::executeScanInCylEndIsSphere()
{
  trajectory->executeTraj(CurrentCommand);

} // executeScanInCylEndIsSphere()


/////////////// 38 - executeScanInPlaneEndIsCyl() ///////////////////

void  Executor::executeScanInPlaneEndIsCyl()
{
  trajectory->executeTraj(CurrentCommand);

} // executeScanInPlaneEndIsCyl()


/////////////// 39 - executeScanInPlaneEndIsPlane() //////////////////

void  Executor::executeScanInPlaneEndIsPlane()
{
  trajectory->executeTraj(CurrentCommand);

} // executeScanInPlaneEndIsPlane()


////////////// 40 - executeScanInPlaneEndIsSphere() //////////////////

void  Executor::executeScanInPlaneEndIsSphere()
{
  trajectory->executeTraj(CurrentCommand);

} // executeScanInPlaneEndIsSphere()


/////////////////// 41 - executeScanOnCircle() ///////////////////////

void  Executor::executeScanOnCircle()
{
  trajectory->executeTraj(CurrentCommand);

} // executeScanOnCircle()


////////////////// 42 - executeScanOnCircleHint() ////////////////////

void  Executor::executeScanOnCircleHint()
{
  OutsideWorld->update(CurrentCommand);

  generateCompletedResponse();

} // executeScanOnCircleHint()


//////////////////// 43 - executeScanOnLine() ////////////////////////

void  Executor::executeScanOnLine()
{
  trajectory->executeTraj(CurrentCommand);

} // executeScanOnLine()


////////////////// 44 - executeScanOnLineHint() //////////////////////

void  Executor::executeScanOnLineHint()
{
  OutsideWorld->update(CurrentCommand);

  generateCompletedResponse();

} // executeScanOnLineHint()


//////////////// 45 - executeScanUnknownHint() //////////////////////

void  Executor::executeScanUnknownHint()
{
  OutsideWorld->update(CurrentCommand);

  generateCompletedResponse();

} // executeScanUnknownHint()


//////////////// 46 - executeSetCoordSystem() ///////////////////////

void  Executor::executeSetCoordSystem()
{
  OutsideWorld->update(CurrentCommand);

  generateCompletedResponse();

} // end executeSetCoordSystem()


///////////// 47 - executeSetCsyTransformation() ////////////////////

void  Executor::executeSetCsyTransformation()
{
  OutsideWorld->update(CurrentCommand);  // world performs update

  generateCompletedResponse();

} // end executeSetCsyTransformation()


//////////////////// 48 - executeSetProp() //////////////////////////

void  Executor::executeSetProp()
{
  char cmdStr[IPPSIZE/2] = "";
  OutsideWorld->update(CurrentCommand);  // world performs update

  CurrentCommand->getCommandString(cmdStr,IPPSIZE-1);
  serverLog.replaceQuotes(cmdStr);

  if(OutsideWorld->getOutOfRangeFlag()){
	Response * x = new ErrorResponse(CurrentCommand->getTag()->getTagNumber(), 
		                               CurrentCommand->getTag()->getTagType(),
									   ArgumentOutOfRange, cmdStr);
	returningValues.push_back(x);

  }

  generateCompletedResponse();

} // end executeSetProp() 



//////////////////// 49 - executeSetTool() //////////////////////////

void  Executor::executeSetTool()
{
  OutsideWorld->update(CurrentCommand);  // world performs update

  generateCompletedResponse();

} // end executeSetTool()



/////////////////// 50 - executeStartSession() //////////////////////

void  Executor::executeStartSession()
{
  OutsideWorld->update(CurrentCommand);  // world performs update

  generateCompletedResponse();

} // end executeStartSession()


////////////////// 51 - executeStopAllDaemons() /////////////////////

void  Executor::executeStopAllDaemons()
{
  //if (OutsideWorld->getOnMoveTag() != 0) 
  //{
	//onMoveDaemonReport();
	//Response * x = new Response(OutsideWorld->getOnMoveTag(), EventTag, Complete);
    //returningValues.push_back(x);
  //}
  
  OutsideWorld->update(CurrentCommand);

  generateCompletedResponse();

} // end executeStopAllDaemons()


////////////////// 52 - executeStopDaemon() //////////////////////

void  Executor::executeStopDaemon()
{
  //if (OutsideWorld->getOnMoveTag() != 0) 
  //{
	//onMoveDaemonReport();
	//Response * x = new Response(OutsideWorld->getOnMoveTag(), EventTag, Complete);
    //returningValues.push_back(x);
    //}

  OutsideWorld->update(CurrentCommand);

  generateCompletedResponse();

} // end executeStopDaemon()


////////////////// 53 - executeTiltCenterPart() //////////////////////

void Executor::executeTiltCenterPart()
{
	OutsideWorld->update(CurrentCommand);

	unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
	TiltCenterPartResponse * res = new TiltCenterPartResponse(tag_num, true); 
	                                                          //get values from world

	returningValues.push_back(res);

	generateCompletedResponse();
}

////////////////// 54 - executeTiltPart() //////////////////////

void Executor::executeTiltPart()
{
	OutsideWorld->update(CurrentCommand);

	unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
	TiltPartResponse * res = new TiltPartResponse(tag_num, true);  //get values from world

	returningValues.push_back(res);

	generateCompletedResponse();
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////// command helpers ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Executor::generateCompletedResponse()
{
  unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
  Response * x = new Response(tag_num, CurrentCommand->getTag()->getTagType(), Complete);
  returningValues.push_back(x);
}

//////////////////////////////////////////////////////////////
void Executor::generateErrorResponse(errorNameType errorNum, char * str)
{
  unsigned int tag_num = CurrentCommand->getTag()->getTagNumber();
  Response * x = new ErrorResponse(tag_num, CommandTag, errorNum, str);
  returningValues.push_back(x);
}
//IllegalCommand

/////////////////////////////////////////////////////////////////////

int Executor::testInSession()
{
  if (! OutsideWorld->getInSession()){

     return 0;
  }
  return 1;
}

/////////////////////////////////////////////////////////////////////
ResponseList Executor::checkStatus(){

	// If a multicycle command is executing,
	// see which one is active and step it.

	if (OutsideWorld->getReadiness() == moving)
	{
		trajectory->stepTraj();
	

	//	sprintf(msag, "***** = %d\n", OutsideWorld->getOnMoveTag());
	//	serverLog.writeFile(msag);
	//	memset(msag, 0, sizeof(msag));

	  if (OutsideWorld->getOnMoveTag() != 0)
		if(trajectory->reportMoveStatus() == true)
			onMoveDaemonReport();
	}

	// Return and clear vector of responses
	ResponseList rl;
	rl = returningValues; //temp buff
	if (returningValues.size() > 0)
		returningValues.erase(returningValues.begin(), returningValues.end());

 	return rl;
}


///////////////////////////////////////////////////////////////////////
void Executor::onMoveDaemonReport(){

	double r = -1, x = -1, y = -1, z = -1, toolA = -1, toolB = -1, toolC = -1;
	bool hasR = false, hasX = false, hasY = false, hasZ = false;
	bool hasToolA = false, hasToolB = false, hasToolC = false;
	
	if (OutsideWorld->getOnMoveHasR()){
	 r = OutsideWorld->getCurrentR();
	 hasR = true;
	}
	if (OutsideWorld->getOnMoveHasX()){
	 x = OutsideWorld->getCurrentX();
	 hasX = true;
	}
	if (OutsideWorld->getOnMoveHasY()){
	 y = OutsideWorld->getCurrentY();
	 hasY = true;
	}
	 if (OutsideWorld->getOnMoveHasZ()){
	  z = OutsideWorld->getCurrentZ();
	  hasZ = true;
	}
	 if (OutsideWorld->getOnMoveHasToolA()){
	 toolA = OutsideWorld->getCurrentToolA();
	 hasToolA = true;
	}
	 if (OutsideWorld->getOnMoveHasToolB()){
	 toolB = OutsideWorld->getCurrentToolB();
	 hasToolB = true;
	}
	 if (OutsideWorld->getOnMoveHasToolC()){
	 toolC = OutsideWorld->getCurrentToolC();
	 hasToolC = true;
	}
	 if(hasR==true || hasX==true || hasY==true || hasZ==true || 
		hasToolA==true || hasToolB==true || hasToolC==true){

		unsigned int tag_num = OutsideWorld->getOnMoveTag();
		OnMoveReportEResponse * res = new OnMoveReportEResponse 
		(tag_num, hasR, hasToolA, hasToolB, hasToolC, hasX, hasY, hasZ, 
		          r, toolA, toolB, toolC, x, y, z);
  
		returningValues.push_back(res);
	 }
}

void Executor::resetExecutor(){

	trajectory->resetTrajectoryGenerator();
	OutsideWorld->killDaemons();
	OutsideWorld->setIsHomed(false);
	OutsideWorld->setInSession(false);
	OutsideWorld->setReadiness(ready);
}


void Executor::initWorld(){

	GoToPars * pars1;
	PtMeasPars * pars2;
	KTool *theTool;

	

	// Initialize NoTool
	theTool = tools->findTool("NoTool");

	pars1 = theTool->getGoToPar();
	
	pars1->setMaxSpeed(500.0);
	pars1->setMinSpeed(1.0);
	pars1->setActSpeed(20.0);
	pars1->setDefSpeed(5.0);
		
	pars1->setMaxAccel(2000.0);
	pars1->setMinAccel(1.0);
	pars1->setActAccel(100.0);
	pars1->setDefAccel(10.0);

	// Set the active tool to NoTool
	tools->setActiveTool(tools->findTool("NoTool"));

	// Initialize BaseTool
	theTool = tools->findTool("BaseTool");

	pars1 = theTool->getGoToPar();

	pars1->setMaxSpeed(500.0);
	pars1->setMinSpeed(1.0);
	pars1->setActSpeed(20.0);
	pars1->setDefSpeed(5.0);
		
	pars1->setMaxAccel(2000.0);
	pars1->setMinAccel(1.0);
	pars1->setActAccel(100.0);
	pars1->setDefAccel(10.0);
	
	pars2 = theTool->getPtMeasPar();

	pars2->setMaxSpeed(500.0);
	pars2->setMinSpeed(1.0);
	pars2->setActSpeed(20.0);
	pars2->setDefSpeed(5.0);
		
	pars2->setMaxAccel(2000.0);
	pars2->setMinAccel(1.0);
	pars2->setActAccel(100.0);
	pars2->setDefAccel(10.0);

	pars2->setMaxApproach(100.0);
	pars2->setMinApproach(1.0);
	pars2->setActApproach(10.0);
	pars2->setDefApproach(20.0);

	pars2->setMaxSearch(100.0);
	pars2->setMinSearch(1.0);
	pars2->setActSearch(10.0);
	pars2->setDefSearch(20.0);

	pars2->setMaxRetract(100.0);
	pars2->setMinRetract(1.0);
	pars2->setActRetract(10.0);
	pars2->setDefRetract(20.0);
	

	// Initialize RefTool
	theTool = tools->findTool("RefTool");

	pars1 = theTool->getGoToPar();

	pars1->setMaxSpeed(500.0);
	pars1->setMinSpeed(1.0);
	pars1->setActSpeed(20.0);
	pars1->setDefSpeed(5.0);
		
	pars1->setMaxAccel(2000.0);
	pars1->setMinAccel(1.0);
	pars1->setActAccel(100.0);
	pars1->setDefAccel(10.0);
	
	pars2 = theTool->getPtMeasPar();

	pars2->setMaxSpeed(500.0);
	pars2->setMinSpeed(1.0);
	pars2->setActSpeed(20.0);
	pars2->setDefSpeed(5.0);
		
	pars2->setMaxAccel(2000.0);
	pars2->setMinAccel(1.0);
	pars2->setActAccel(100.0);
	pars2->setDefAccel(10.0);

	pars2->setMaxApproach(100.0);
	pars2->setMinApproach(1.0);
	pars2->setActApproach(10.0);
	pars2->setDefApproach(20.0);

	pars2->setMaxSearch(100.0);
	pars2->setMinSearch(1.0);
	pars2->setActSearch(10.0);
	pars2->setDefSearch(20.0);

	pars2->setMaxRetract(100.0);
	pars2->setMinRetract(1.0);
	pars2->setActRetract(10.0);
	pars2->setDefRetract(20.0);  
	
	// Create a collision volume 

	Box * box1 = new Box(20, 20, 20, 10, 10, 10, 1, 0, 0, 0, 1, 0, 0, 0, 1);
      
	OnePropertyCollisionVolume * collisionVolume1 = new OnePropertyCollisionVolume(1, box1);

	// Add a probe "Probe1"
	tools->addTool(new KTool("Probe1", "123", new GoToPars, new PtMeasPars, false, collisionVolume1, 8, 3));

	// Initialize Probe1
	theTool = tools->findTool("Probe1");

	pars1 = theTool->getGoToPar();

	pars1->setMaxSpeed(500.0);
	pars1->setMinSpeed(1.0);
	pars1->setActSpeed(20.0);
	pars1->setDefSpeed(5.0);
		
	pars1->setMaxAccel(2000.0);
	pars1->setMinAccel(1.0);
	pars1->setActAccel(100.0);
	pars1->setDefAccel(10.0);
	
	pars2 = theTool->getPtMeasPar();

	pars2->setMaxSpeed(500.0);
	pars2->setMinSpeed(1.0);
	pars2->setActSpeed(20.0);
	pars2->setDefSpeed(5.0);
		
	pars2->setMaxAccel(2000.0);
	pars2->setMinAccel(1.0);
	pars2->setActAccel(100.0);
	pars2->setDefAccel(10.0);

	pars2->setMaxApproach(100.0);
	pars2->setMinApproach(1.0);
	pars2->setActApproach(10.0);
	pars2->setDefApproach(20.0);

	pars2->setMaxSearch(100.0);
	pars2->setMinSearch(1.0);
	pars2->setActSearch(10.0);
	pars2->setDefSearch(20.0);

	pars2->setMaxRetract(100.0);
	pars2->setMinRetract(1.0);
	pars2->setActRetract(10.0);
	pars2->setDefRetract(20.0); 
	

}