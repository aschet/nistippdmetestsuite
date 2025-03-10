// This is a trajectory generator for the executor.

#ifndef TRAJGEN_H
#define TRAJGEN_H

#include <fstream>  //hui-101802
#include <iostream> //hui-101802

#include "../Cmm/world.h"
#include "../Server/ServerQueues.h"
#include "../CmdResClasses/IppCmdRes.h"
#include "Helix.h"
#include <stdlib.h>
#include <vector>


enum trajectoryType {
  Linear	 = 1,
  Circular   = 2,
  Helical    = 3 };

enum trajectoryState {
  TrajInit       = 1,
  TrajAccel      = 2,
  TrajConstVel   = 3,
  TrajDecel      = 4, 
  TrajDone       = 5 };
  

enum movingState {
  MoveInitialize   = 1,
  MoveApproach     = 2,
  MoveSearch       = 3,
  MoveScan         = 4,
  MoveRetract      = 5,
  MoveDone         = 6 };

enum scanReportOrder {
  ScanQ            = 1,
  ScanR            = 2,
  ScanA            = 3,
  ScanB            = 4,
  ScanC            = 5,
  ScanX            = 6,
  ScanY            = 7,
  ScanZ            = 8,
  ScanER           = 9,
  ScanIJK          = 10 };

// add a trajectory state to make it more intuitive


using namespace std;
using namespace ipp;



class TrajGen 
{
  public:

	TrajGen(world*, ResponseList*);
    ~TrajGen(){};

	void executeTraj(Command*);
	void stepTraj();
	bool reportMoveStatus();
	void abortCommandExecution();
	void resetTrajectoryGenerator();


  private:

  void stepTrajGoTo();
  void stepTrajHome();
  void stepTrajPtMeas();
  void stepTrajAlignTool();
  void stepTrajScan();
  void initializeVariables();
  void getWorldProperties();
  void setWorldPosition();

  void computeLinearMotionComponents();
  void computeCircularMotionComponents();
  void computeHelicalMotionComponents();


  double computeLinearDiff(double,double,double,double,double,double);

  void computeLinearStep();
  void computeCircularStep();
  void computeHelicalStep();

  void generatePtMeasReport();
  void onScanSetData();
  void onScanCollectData();
  void reportScanData();

//private data members

  world * OutsideWorld;
  ResponseList *returningValues;
  Command inCmd;
  Command *trajCommand;
  char* trajCommandName;

  trajectoryType trajType;
  trajectoryState trajState;
  movingState moveState;
  
  double bgnPosX, bgnPosY, bgnPosZ;
  double curPosX, curPosY, curPosZ;
  double cmdPosX, cmdPosY, cmdPosZ;
  double prvPosX, prvPosY, prvPosZ;
  double nomPosX, nomPosY, nomPosZ;
  double appPosX, appPosY, appPosZ;
  double srchPosX, srchPosY, srchPosZ;
  double retPosX, retPosY, retPosZ;

  // Scan variables
  bool scanning;
  bool hasX, hasY, hasZ;
  bool hasToolA, hasToolB, hasToolC;
  bool hasQ, hasR;
  bool hasER, hasIJK;
  double timeInc, timeIncTotal;
  int steps, scanDataCount, scanVals, num;
  queue <double> scanData;
  double cx, cy, cz;
  double ci, cj, ck;
  double sx, sy, sz;
  double si, sj, sk;
  double dx, dy, dz;
  double sfa, delta, stepW, dia, radius;
  double ex, ey, ez;
  double ei, ej, ek;
  double px, py, pz;
  double pi, pj, pk;
  double rx, ry, rz;
  double vx, vy, vz;
  double nx, ny, nz;
  double totalAngle, stepAngle, appAngle;

  Helix helix;
  double beta, d;

  double deltaDist, deltaX, deltaY, deltaZ, vecLth;

  double currentDist;

  double dirX, dirY, dirZ;

  double yaw, pitch, tmpSq, tmp2;

  ToolChanger *tools;
  KTool *activeTool;
  GoToPars *goToPars;
  PtMeasPars *ptMeasPars;

  double probe_radius;

  double cmdVel, cmdVelX, cmdVelY, cmdVelZ;
  double cmdAcl, cmdAclX, cmdAclY, cmdAclZ;
  double angAcl, angVel;
  
  double aclTransTime, velTransTime, totalExecTime;
  double commandTime, reportTime, onMoveReportTime;
  
  double aclDist, aclDistX, aclDistY, aclDistZ;
  double velDist, velDistX, velDistY, velDistZ;
  double reportDist, onMoveReportDist;

  double appDst, srchDst, retDst;

  double ii, jj, kk;

  clock_t lastTime, currentTime;
  double timeStep;

  commandNameType cnt;	

  OnMoveReportECommand *currentOnMoveReportE;
  GoToCommand *currentGoTo;
  PtMeasCommand *currentPtMeas;
  Command *currentHome;
  AlignToolCommand *currentAlignTool;
  ScanOnLineCommand *currentScanOnLine;
  ScanInPlaneEndIsSphereCommand *currentScanInPlaneEndIsSphere;
  ScanInPlaneEndIsPlaneCommand *currentScanInPlaneEndIsPlane;
  ScanInPlaneEndIsCylCommand *currentScanInPlaneEndIsCyl;
  ScanOnCircleCommand *currentScanOnCircle;
  ScanInCylEndIsPlaneCommand *currentScanInCylEndIsPlane;
  ScanInCylEndIsSphereCommand *currentScanInCylEndIsSphere;
};

#endif 
