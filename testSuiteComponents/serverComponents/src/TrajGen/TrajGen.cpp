#include "../Server/StdAfx.h"
#include "TrajGen.h"
#include "../Server/Logger.h"
#include "../CmdParser/parserCmd.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

// Simulation variables

#define PI 3.1416
#define MAXSCAN 50

extern Logger serverLog;

bool debug = false;

bool showXYZ = false;

char msg[1024] = "";

char responseBuffer[1024] = "";

using namespace ipp;

TrajGen::TrajGen(world* Outside, ResponseList* retVals)
{
  OutsideWorld = Outside;
  returningValues = retVals; 
  tools = OutsideWorld->getTools();
  
}


// Each command requiring a simulated trajectory is passed the executeTraj
// from the executor.  The command is assigned a trajType. This flexability 
// is coded for the scan commands so that more complicated trajectories for
// "OnLine and InPlane" can be easily integrated in the future.  

// Trajectories currently supported are Circular, Helical and Linear.
// There are two methods for each of the trajectory types. Each has
// a compute----MotionComponents and a compute----Step method.



void TrajGen::executeTraj(Command * cmd)
{
  OutsideWorld->setReadiness(moving);
  trajCommand = cmd;
  cnt = trajCommand->getCommandName();
  initializeVariables();
  lastTime = clock();
  

  switch (cnt) {
  case GoTo                      : trajType = Linear;
								   trajCommandName="GoTo";
	                               stepTrajGoTo();
		                           break;
  case Home                      : trajType = Linear;
								   trajCommandName="Home";
	                               stepTrajHome();
		                           break;
  case PtMeas                    : trajType = Linear;
								   trajCommandName="PtMeas";
	                               moveState = MoveInitialize;
		                           stepTrajPtMeas();
		                           break;
  case AlignTool                 : trajCommandName="AlignTool";
								   stepTrajAlignTool();
		                           break;
  case ScanOnCircle              : trajType = Circular;
								   trajCommandName="ScanOnCircle";
	                               moveState = MoveInitialize;
	                               stepTrajScan();
		                           break;
  case ScanOnLine                : trajType = Linear;
								   trajCommandName="ScanOnLine";
	                               moveState = MoveInitialize;
	                               stepTrajScan();
		                           break;
  case ScanInPlaneEndIsSphere    : trajType = Linear;
								   trajCommandName="ScanInPlaneEndIsSphere";
	                               moveState = MoveInitialize;
	                               stepTrajScan();
		                           break;
  case ScanInPlaneEndIsPlane     : trajType = Linear;
								   trajCommandName="ScanInPlaneEndIsPlane";
	                               moveState = MoveInitialize;
	                               stepTrajScan();
		                           break;
  case ScanInPlaneEndIsCyl       : trajType = Linear;
								   trajCommandName="ScanInPlaneEndIsCyl";
	                               moveState = MoveInitialize;
	                               stepTrajScan();
		                           break;
  case ScanInCylEndIsSphere      : trajType = Helical;
								   trajCommandName="ScanInCylEndIsSphere";
	                               moveState = MoveInitialize;
	                               stepTrajScan();
			                       break;
  case ScanInCylEndIsPlane       : trajType = Helical;
								   trajCommandName="ScanInCylEndIsPlane";
	                               moveState = MoveInitialize;
	                               stepTrajScan();
			                       break;
  default                        : ;

  }

} // end executeTraj(Command * cmd)



void TrajGen::stepTrajGoTo()
{
	
  if(trajState == TrajInit) {

    currentGoTo = (GoToCommand *) trajCommand;

	bgnPosX = OutsideWorld->getCurrentX();
    bgnPosY = OutsideWorld->getCurrentY();
    bgnPosZ = OutsideWorld->getCurrentZ();

    // get current tool
    activeTool = tools->getActiveTool();

	// get commanded GoTo velocity and acceleration
    goToPars = activeTool->getGoToPar();
    cmdVel = goToPars->getActSpeed();
    cmdAcl = goToPars->getActAccel();

	// get Commanded coordinates
	cmdPosX = ((currentGoTo->getHasX()) ? currentGoTo->getX() : bgnPosX );
	cmdPosY = ((currentGoTo->getHasY()) ? currentGoTo->getY() : bgnPosY );
	cmdPosZ = ((currentGoTo->getHasZ()) ? currentGoTo->getZ() : bgnPosZ );

  } 

  commandTime += timeStep;
  computeLinearStep();

  if(trajState == TrajDone) // complete
  {
	  // generate complete	  
	  unsigned int tagNum = currentGoTo->getTag()->getTagNumber();
	  Response * x = new Response(tagNum, currentGoTo->getTag()->getTagType(), Complete);
	  returningValues->push_back(x);

	  OutsideWorld->setReadiness(ready);
  }
} // end startGoTo()



void TrajGen::stepTrajHome()
{
  if(trajState == TrajInit)
  {
	  if(debug)
	  {
		sprintf(msg, "ExecutingHome");
		serverLog.writeFile(msg);
		memset(msg, 0, sizeof(msg));
	  }

    currentHome = trajCommand;

	bgnPosX = OutsideWorld->getCurrentX();
    bgnPosY = OutsideWorld->getCurrentY();
    bgnPosZ = OutsideWorld->getCurrentZ();

	// get current tool
    activeTool = tools->getActiveTool();

	// get commanded GoTo velocity and acceleration
    goToPars = activeTool->getGoToPar();
    cmdVel = goToPars->getActSpeed();
    cmdAcl = goToPars->getActAccel();

	// set Commanded coordinates to home pos
	cmdPosX = 0;
	cmdPosY = 0;
	cmdPosZ = 0;
  }

  commandTime += timeStep;
  computeLinearStep();
	    
  if(trajState == TrajDone)
  {
    OutsideWorld->setIsHomed(true);

    //Generate Completed Response:  
    unsigned int tagNum = currentHome->getTag()->getTagNumber();
    Response * x = new Response(tagNum, currentHome->getTag()->getTagType(), Complete);
    returningValues->push_back(x);

    OutsideWorld->setReadiness(ready);
  }



} // end startTrajHome()


void TrajGen::stepTrajPtMeas()
{

	switch (moveState) {

	case MoveInitialize : 

	  if (debug) 
	  {
		sprintf(msg, "***************** MoveInitialize *******************");
		serverLog.writeFile(msg);
	    memset(msg, 0, sizeof(msg));
	  }
		
	  currentPtMeas = (PtMeasCommand *) trajCommand;

	  probe_radius = OutsideWorld->getProbeRadius();

	  // get current coordinates
	  bgnPosX = OutsideWorld->getCurrentX();
      bgnPosY = OutsideWorld->getCurrentY();
      bgnPosZ = OutsideWorld->getCurrentZ();

      // get commanded coordinates
      cmdPosX = nomPosX = ((currentPtMeas->getHasX()) ? currentPtMeas->getX() : bgnPosX );
      cmdPosY = nomPosY = ((currentPtMeas->getHasY()) ? currentPtMeas->getY() : bgnPosY );
      cmdPosZ = nomPosZ = ((currentPtMeas->getHasZ()) ? currentPtMeas->getZ() : bgnPosZ );

      // get current tool
      activeTool = tools->getActiveTool();

      // create PtMeasPars
      ptMeasPars = activeTool->getPtMeasPar();

      // create GoToPars
      goToPars = activeTool->getGoToPar();

     // If ijk vector present, normalize it. 
     // If not present, compute from nominal XYZ to bgnPos
     // Compute approach position from above shifting nominal pos in IJK
     // by Part.Clearance, Tool.Approach, and Tool.Radius

     if (currentPtMeas->getHasIJK())
	 {
	   ii = currentPtMeas->getI();
	   jj = currentPtMeas->getJ();
	   kk = currentPtMeas->getK();

	   appDst = ptMeasPars->getActApproach();
	   srchDst = ptMeasPars->getActSearch();
	   retDst = ptMeasPars->getActRetract();

	   vecLth = sqrt ((ii * ii) + (jj * jj) + (kk * kk));

	   nomPosX = nomPosX + probe_radius * ii / vecLth ;
	   nomPosY = nomPosY + probe_radius * jj / vecLth ;
	   nomPosZ = nomPosZ + probe_radius * kk / vecLth ;

       if ( vecLth == 0.0 )
	   {
	     appPosX = nomPosX;
		 appPosY = nomPosY;
		 appPosZ = nomPosZ;
	   }
	   else
	   {
	     appPosX = nomPosX + ( appDst * ii / vecLth );
		 appPosY = nomPosY + ( appDst * jj / vecLth );
	   	 appPosZ = nomPosZ + ( appDst * kk / vecLth );

		 srchPosX = nomPosX; //- (srchDst * ii / vecLth ); just nominal
		 srchPosY = nomPosY; //- (srchDst * jj / vecLth ); on simulated
		 srchPosZ = nomPosZ; //- (srchDst * kk / vecLth ); search
 
		 // compute retract pos based on a nominal touch
		 // since this is a simulation using all nominals
		 retPosX = nomPosX + ( appDst * ii / vecLth );
		 retPosY = nomPosY + ( appDst * jj / vecLth );
		 retPosZ = nomPosZ + ( appDst * kk / vecLth );
	   }
	 }
     else //compute vector based on nominal and previous positions
	 {
	   computeLinearMotionComponents();

	   nomPosX = nomPosX + probe_radius * (cos(pitch)) * cos(yaw);
	   nomPosY = nomPosY + probe_radius * (cos(pitch)) * sin(yaw);
	   nomPosZ = nomPosZ + probe_radius * sin(pitch);
	 
	   appPosX = nomPosX - appDst * (cos(pitch)) * cos(yaw);		
	   appPosY = nomPosY - appDst * (cos(pitch)) * sin(yaw);
	   appPosZ = nomPosZ - appDst * sin(pitch);

	   srchPosX = nomPosX + srchDst * (cos(pitch)) * cos(yaw);		
	   srchPosY = nomPosY + srchDst * (cos(pitch)) * sin(yaw);
	   srchPosZ = nomPosZ + srchDst * sin(pitch);

	   retPosX = nomPosX - retDst * (cos(pitch)) * cos(yaw);		
	   retPosY = nomPosY - retDst * (cos(pitch)) * sin(yaw);
	   retPosZ = nomPosZ - retDst * sin(pitch);
	 }
	 moveState = MoveApproach;

	 if(debug){

	 sprintf(msg, "appPosX = %.4lf  appPosY = %.4lf  appPosZ = %.4lf",appPosX,appPosY,appPosZ);
	 serverLog.writeFile(msg);
	 memset(msg, 0, sizeof(msg));

	 sprintf(msg, "srchPosX = %.4lf  srchPosY = %.4lf  srchPosZ = %.4lf",srchPosX,srchPosY,srchPosZ);
	 serverLog.writeFile(msg);
	 memset(msg, 0, sizeof(msg));

	 sprintf(msg, "retPosX = %.4lf  retPosY = %.4lf  retPosZ = %.4lf",retPosX,retPosY,retPosZ);
	 serverLog.writeFile(msg);
	 memset(msg, 0, sizeof(msg));

	 }
	 break;

	case MoveApproach :

		if (trajState == TrajInit)
		{
		  if(debug)
		  {
			  sprintf(msg, "***************** MoveApproach *******************");
	          serverLog.writeFile(msg);
	          memset(msg, 0, sizeof(msg));
		  }

          // set commanded coordinates
          cmdPosX = appPosX;
          cmdPosY = appPosY;
          cmdPosZ = appPosZ;

		  //Get PtMeasPars velocity and acceleration
		  cmdVel = goToPars->getActSpeed();
          cmdAcl = goToPars->getActAccel();
		}

		commandTime += timeStep;
		computeLinearStep();

		if (trajState == TrajDone)
		{
			moveState = MoveSearch;
			trajState = TrajInit;
		}
		break;

	case MoveSearch :

		if (trajState == TrajInit)
		{
		  if(debug)
		  {
		     sprintf(msg, "***************** MoveSearch *******************");
	         serverLog.writeFile(msg);
	         memset(msg, 0, sizeof(msg));
		  }

		  // get current coordinates
	      bgnPosX = OutsideWorld->getCurrentX();
          bgnPosY = OutsideWorld->getCurrentY();
          bgnPosZ = OutsideWorld->getCurrentZ();

          // set commanded coordinates
          cmdPosX = srchPosX;
          cmdPosY = srchPosY;
          cmdPosZ = srchPosZ;

		  //Get PtMeasPars velocity and acceleration
		  cmdVel = ptMeasPars->getActSpeed();
          cmdAcl = ptMeasPars->getActAccel();	 
		}

		commandTime += timeStep;
		computeLinearStep();

		if (trajState == TrajDone)
		{
			
			generatePtMeasReport();  //Report Nominal Touch
			moveState = MoveRetract;
			trajState = TrajInit;
		}
		break;
		
	case MoveRetract :

		if (trajState == TrajInit)
		{
		  if(debug) 
		  {
		    sprintf(msg, "***************** MoveRetract *******************");
	        serverLog.writeFile(msg);
	        memset(msg, 0, sizeof(msg));
		  }

			// get current coordinates
	      bgnPosX = OutsideWorld->getCurrentX();
          bgnPosY = OutsideWorld->getCurrentY();
          bgnPosZ = OutsideWorld->getCurrentZ();

          // set commanded coordinates
          cmdPosX = retPosX;
          cmdPosY = retPosY;
          cmdPosZ = retPosZ;

		  //Get GoToPars velocity and acceleration
		  cmdVel = goToPars->getActSpeed();
          cmdAcl = goToPars->getActAccel();
		}

		commandTime += timeStep;
		computeLinearStep();

		if (trajState == TrajDone)
		{
		  //Generate Completed Response:  
          unsigned int tagNum = currentPtMeas->getTag()->getTagNumber();
          Response * x = new Response(tagNum, currentPtMeas->getTag()->getTagType(), Complete);
          returningValues->push_back(x);

          OutsideWorld->setReadiness(ready);
		  moveState = MoveDone;
		}
		break;

	default: ;;
  }

} // end startTrajPtMeas()

void TrajGen::stepTrajAlignTool()
{
  /* Not Implemented in this release.
     This would have to be implemented based on some tool model */

} // end startTrajAlignTool()

void TrajGen::stepTrajScan()
{
  int i=0;
  switch (moveState) {

	case MoveInitialize : 

	  if (debug) 
	  {
		sprintf(msg, "***************** MoveInitialize *******************");
		serverLog.writeFile(msg);
	    memset(msg, 0, sizeof(msg));
	  }

	  scanning = false;

	  scanData.empty();

	  onScanSetData();

	  probe_radius = OutsideWorld->getProbeRadius();

	  // get current coordinates
	  bgnPosX = OutsideWorld->getCurrentX();
      bgnPosY = OutsideWorld->getCurrentY();
      bgnPosZ = OutsideWorld->getCurrentZ();

      // get current tool
      activeTool = tools->getActiveTool();

      // create PtMeasPars
      ptMeasPars = activeTool->getPtMeasPar();

      // create GoToPars
      goToPars = activeTool->getGoToPar();

	  appDst = ptMeasPars->getActApproach();
	  srchDst = ptMeasPars->getActSearch();
	  retDst = ptMeasPars->getActRetract();
		
	  switch (cnt) {
		
		case ScanOnLine : 

		    currentScanOnLine = (ScanOnLineCommand *) trajCommand; 
			
			// get ScanOnLineCommand variables
			sx = currentScanOnLine->getSx();
			sy = currentScanOnLine->getSy();
			sz = currentScanOnLine->getSz();
			ex = currentScanOnLine->getEx();
			ey = currentScanOnLine->getEy();
			ez = currentScanOnLine->getEz();
			ii = currentScanOnLine->getI();
			jj = currentScanOnLine->getJ();
			kk = currentScanOnLine->getK();
			stepW = currentScanOnLine->getStepW();

		
			// modify start and end points to account
			// for a probe radius

			srchPosX = sx + (probe_radius * ii / vecLth); 
	        srchPosY = sy + (probe_radius * jj / vecLth);
	        srchPosZ = sz + (probe_radius * kk / vecLth); 

			nomPosX = ex + (probe_radius * ii / vecLth); 
	        nomPosY = ey + (probe_radius * jj / vecLth);
	        nomPosZ = ez + (probe_radius * kk / vecLth); 

			vecLth = sqrt ((ii * ii) + (jj * jj) + (kk * kk));

			appPosX = sx + ( appDst * ii / vecLth );
			appPosY = sy + ( appDst * jj / vecLth );
			appPosZ = sz + ( appDst * kk / vecLth );   
 
			// compute retract pos based on a nominal touch
			// since this is a simulation using all nominals
			retPosX = ex + ( retDst * ii / vecLth );
			retPosY = ey + ( retDst * jj / vecLth );
			retPosZ = ez + ( retDst * kk / vecLth );

			break;

		case ScanInPlaneEndIsSphere : 

			currentScanInPlaneEndIsSphere = (ScanInPlaneEndIsSphereCommand *) trajCommand;

			// get ScanOnLineCommand variables
			sx = currentScanInPlaneEndIsSphere->getSx();
			sy = currentScanInPlaneEndIsSphere->getSy();
			sz = currentScanInPlaneEndIsSphere->getSz();
			dx = currentScanInPlaneEndIsSphere->getDx();
			dy = currentScanInPlaneEndIsSphere->getDy();
			dz = currentScanInPlaneEndIsSphere->getDz();
			ex = currentScanInPlaneEndIsSphere->getEx();
			ey = currentScanInPlaneEndIsSphere->getEy();
			ez = currentScanInPlaneEndIsSphere->getEz();
			si = currentScanInPlaneEndIsSphere->getSI();
			sj = currentScanInPlaneEndIsSphere->getSJ();
			sk = currentScanInPlaneEndIsSphere->getSK();
			ei = currentScanInPlaneEndIsSphere->getEI();
			ej = currentScanInPlaneEndIsSphere->getEJ();
			ek = currentScanInPlaneEndIsSphere->getEK();
			stepW = currentScanInPlaneEndIsSphere->getStepW();
			dia = currentScanInPlaneEndIsSphere->getDia();

			// modify start and end points to account
			// for a probe radius

			vecLth = sqrt ((si * si) + (sj * sj) + (sk * sk));

			srchPosX = sx + (probe_radius * si / vecLth); 
	        srchPosY = sy + (probe_radius * sj / vecLth);
	        srchPosZ = sz + (probe_radius * sk / vecLth); 

			appPosX = sx + ( appDst * si / vecLth );
			appPosY = sy + ( appDst * sj / vecLth );
			appPosZ = sz + ( appDst * sk / vecLth ); 
			
			vecLth = sqrt ((ei * ei) + (ej * ej) + (ek * ek));

			nomPosX = ex + (probe_radius * ei / vecLth); 
	        nomPosY = ey + (probe_radius * ej / vecLth);
	        nomPosZ = ez + (probe_radius * ek / vecLth); 	
 
			retPosX = ex + ( retDst * ei / vecLth );
			retPosY = ey + ( retDst * ej / vecLth );
			retPosZ = ez + ( retDst * ek / vecLth );
			
			break;

		case ScanInPlaneEndIsPlane : 

			currentScanInPlaneEndIsPlane = (ScanInPlaneEndIsPlaneCommand *) trajCommand;

			// get ScanOnLineCommand variables
			sx = currentScanInPlaneEndIsPlane->getSx();
			sy = currentScanInPlaneEndIsPlane->getSy();
			sz = currentScanInPlaneEndIsPlane->getSz();
			dx = currentScanInPlaneEndIsPlane->getDx();
			dy = currentScanInPlaneEndIsPlane->getDy();
			dz = currentScanInPlaneEndIsPlane->getDz();
			si = currentScanInPlaneEndIsPlane->getSI();
			sj = currentScanInPlaneEndIsPlane->getSJ();
			sk = currentScanInPlaneEndIsPlane->getSK();
			ei = currentScanInPlaneEndIsPlane->getEI();
			ej = currentScanInPlaneEndIsPlane->getEJ();
			ek = currentScanInPlaneEndIsPlane->getEK();
			px = currentScanInPlaneEndIsPlane->getPx();
			py = currentScanInPlaneEndIsPlane->getPy();
			pz = currentScanInPlaneEndIsPlane->getPz();
			pi = currentScanInPlaneEndIsPlane->getPI();
			pj = currentScanInPlaneEndIsPlane->getPJ();
			pk = currentScanInPlaneEndIsPlane->getPK();
			stepW = currentScanInPlaneEndIsPlane->getStepW();
			num = currentScanInPlaneEndIsPlane->getNum();


			// modify start and end points to account
			// for a probe radius

			vecLth = sqrt ((si * si) + (sj * sj) + (sk * sk));

			srchPosX = sx + (probe_radius * si / vecLth); 
	        srchPosY = sy + (probe_radius * sj / vecLth);
	        srchPosZ = sz + (probe_radius * sk / vecLth);
			
			appPosX = sx + ( appDst * si / vecLth );
			appPosY = sy + ( appDst * sj / vecLth );
			appPosZ = sz + ( appDst * sk / vecLth );

			vecLth = sqrt ((ei * ei) + (ej * ej) + (ek * ek));

			nomPosX = px + (probe_radius * ei / vecLth); 
	        nomPosY = py + (probe_radius * ej / vecLth);
	        nomPosZ = pz + (probe_radius * ek / vecLth); 

			retPosX = px + ( retDst * ei / vecLth );
			retPosY = py + ( retDst * ej / vecLth );
			retPosZ = pz + ( retDst * ek / vecLth );

			break;

		case ScanInPlaneEndIsCyl : 

			currentScanInPlaneEndIsCyl = (ScanInPlaneEndIsCylCommand *) trajCommand;

			// get ScanOnLineCommand variables
			sx = currentScanInPlaneEndIsCyl->getSx();
			sy = currentScanInPlaneEndIsCyl->getSy();
			sz = currentScanInPlaneEndIsCyl->getSz();
			si = currentScanInPlaneEndIsCyl->getSI();
			sj = currentScanInPlaneEndIsCyl->getSJ();
			sk = currentScanInPlaneEndIsCyl->getSK();
			dx = currentScanInPlaneEndIsCyl->getDx();
			dy = currentScanInPlaneEndIsCyl->getDy();
			dz = currentScanInPlaneEndIsCyl->getDz();
			ei = currentScanInPlaneEndIsCyl->getEI();
			ej = currentScanInPlaneEndIsCyl->getEJ();
			ek = currentScanInPlaneEndIsCyl->getEK();
			cx = currentScanInPlaneEndIsCyl->getCx();
			cy = currentScanInPlaneEndIsCyl->getCy();
			cz = currentScanInPlaneEndIsCyl->getCz();
			ci = currentScanInPlaneEndIsCyl->getCI();
			cj = currentScanInPlaneEndIsCyl->getCJ();
			ck = currentScanInPlaneEndIsCyl->getCK();
			stepW = currentScanInPlaneEndIsCyl->getStepW();
			num = currentScanInPlaneEndIsCyl->getNum();

			// modify start and end points to account
			// for a probe radius

			vecLth = sqrt ((si * si) + (sj * sj) + (sk * sk));

			srchPosX = sx + (probe_radius * si / vecLth); 
	        srchPosY = sy + (probe_radius * sj / vecLth);
	        srchPosZ = sz + (probe_radius * sk / vecLth); 

			appPosX = sx + ( appDst * si / vecLth );
			appPosY = sy + ( appDst * sj / vecLth );
			appPosZ = sz + ( appDst * sk / vecLth );

			vecLth = sqrt ((ei * ei) + (ej * ej) + (ek * ek));

			nomPosX = cx + (probe_radius * ei / vecLth); 
	        nomPosY = cy + (probe_radius * ej / vecLth);
	        nomPosZ = cz + (probe_radius * ek / vecLth); 	
 
			retPosX = cx + ( retDst * ei / vecLth );
			retPosY = cy + ( retDst * ej / vecLth );
			retPosZ = cz + ( retDst * ek / vecLth );
			
			break;

		case ScanOnCircle :

			currentScanOnCircle = (ScanOnCircleCommand *) trajCommand;
			cx = currentScanOnCircle->getCx();
			cy = currentScanOnCircle->getCy();
			cz = currentScanOnCircle->getCz();
			sx = currentScanOnCircle->getSx();
			sy = currentScanOnCircle->getSy();
			sz = currentScanOnCircle->getSz();
			nx = currentScanOnCircle->getI();
			ny = currentScanOnCircle->getJ();
			nz = currentScanOnCircle->getK();
			delta = currentScanOnCircle->getDelta();
			sfa = currentScanOnCircle->getSfa();
			stepW = currentScanOnCircle->getStepW();
	

			rx = sx - cx;
			ry = sy - cy;
			rz = sz - cz;

			radius = sqrt((rx * rx) + (ry * ry) + (rz * rz));

			rx = (rx / radius);
			ry = (ry / radius);
			rz = (rz / radius);

			// account for probe radius
			if(sfa >= 90 && sfa <270)
				radius = radius - probe_radius;
			else
				radius = radius + probe_radius;

			if(debug){
				sprintf(msg, "probe_radius = %.4lf",probe_radius);
				serverLog.writeFile(msg);
				memset(msg, 0, sizeof(msg));

				sprintf(msg, "radius = %.4lf",radius);
				serverLog.writeFile(msg);
				memset(msg, 0, sizeof(msg));
			}
			
			// recalculate sx, sy, and sz based on the new radius
			sx = cx + (radius * rx);
			sy = cy + (radius * ry);
			sz = cz + (radius * rz);

			if(debug){
				sprintf(msg, "sx = %.4lf  sy = %.4lf  sz = %.4lf",sx,sy,sz);
				serverLog.writeFile(msg);
				memset(msg, 0, sizeof(msg));
			}

			srchPosX = sx; 
	        srchPosY = sy; 
	        srchPosZ = sz;

			num = sqrt((nx * nx) + (ny * ny) + (nz * nz));

			nx = nx / num;
			ny = ny / num;
			nz = nz / num;

			vx = (ny * rz) - (ry * nz);
			vy = (nz * rx) - (rz * nx);
			vz = (nx * ry) - (rx * ny);

			totalAngle = (PI / 180) * delta;

			ex = cx + (radius * ((rx * cos(totalAngle)) + (vx * sin(totalAngle))));
			ey = cy + (radius * ((ry * cos(totalAngle)) + (vy * sin(totalAngle))));
			ez = cz + (radius * ((rz * cos(totalAngle)) + (vz * sin(totalAngle))));

			nomPosX = ex;
			nomPosY = ey;
			nomPosZ = ez;

			stepW = (PI / 180) * stepW;

			dx = cx + (radius * ((rx * cos(stepW)) + (vx * sin(stepW))));
			dy = cy + (radius * ((ry * cos(stepW)) + (vy * sin(stepW))));
			dz = cz + (radius * ((rz * cos(stepW)) + (vz * sin(stepW))));

			//compute approach and retract positions

			appAngle = (PI/180) * (180 - sfa);

			appPosX = sx + (appDst * ((rx * cos(appAngle)) + (nx * sin(appAngle))));
			appPosY = sy + (appDst * ((ry * cos(appAngle)) + (ny * sin(appAngle))));
			appPosZ = sz + (appDst * ((rz * cos(appAngle)) + (nz * sin(appAngle))));

			retPosX = ex + (retDst * ((rx * cos(appAngle)) + (nx * sin(appAngle))));
			retPosY = ey + (retDst * ((ry * cos(appAngle)) + (ny * sin(appAngle))));
			retPosZ = ez + (retDst * ((rz * cos(appAngle)) + (nz * sin(appAngle))));

			break;

		case ScanInCylEndIsSphere :

			currentScanInCylEndIsSphere = (ScanInCylEndIsSphereCommand *) trajCommand;
		
			cx = currentScanInCylEndIsSphere->getCx();
			cy = currentScanInCylEndIsSphere->getCy();
			cz = currentScanInCylEndIsSphere->getCz();
			ci = currentScanInCylEndIsSphere->getCI();
			cj = currentScanInCylEndIsSphere->getCJ();
			ck = currentScanInCylEndIsSphere->getCK();
			sx = currentScanInCylEndIsSphere->getSx();
			sy = currentScanInCylEndIsSphere->getSy();
			sz = currentScanInCylEndIsSphere->getSz();
			si = currentScanInCylEndIsSphere->getSI();
			sj = currentScanInCylEndIsSphere->getSJ();
			sk = currentScanInCylEndIsSphere->getSK();
			dx = currentScanInCylEndIsSphere->getDx();
			dy = currentScanInCylEndIsSphere->getDy();
			dz = currentScanInCylEndIsSphere->getDz();
			ex = currentScanInCylEndIsSphere->getEx();
			ey = currentScanInCylEndIsSphere->getEy();
			ez = currentScanInCylEndIsSphere->getEz();
			ei = currentScanInCylEndIsSphere->getEI();
			ej = currentScanInCylEndIsSphere->getEJ();
			ek = currentScanInCylEndIsSphere->getEK();
			stepW = currentScanInCylEndIsSphere->getStepW();
			dia = currentScanInCylEndIsSphere->getDia();

			helix.findHelixParameters(cx,cy,cz,dz,dy,dz,ex,ey,ez,sx,sy,sz,ci,cj,ck,&beta,&d);

			rx = sx - cx;
			ry = sy - cy;
			rz = sz - cz;

			radius = sqrt((rx * rx) + (ry * ry) + (rz * rz));

			rx = (rx / radius);
			ry = (ry / radius);
			rz = (rz / radius);

			radius += probe_radius;

			nx = ci;
			ny = cj;
			nz = ck;
			
			num = sqrt((nx * nx) + (ny * ny) + (nz * nz));

			nx = nx / num;
			ny = ny / num;
			nz = nz / num;

			vx = (ny * rz) - (ry * nz);
			vy = (nz * rx) - (rz * nx);
			vz = (nx * ry) - (rx * ny);

			nomPosX = ex;
			nomPosY = ey;
			nomPosZ = ez;

			srchPosX = sx; 
	        srchPosY = sy; 
	        srchPosZ = sz;

			vecLth = sqrt((si * si) + (sj * sj) + (sk * sk));

			appPosX = sx + ( appDst * si/vecLth );
			appPosY = sy + ( appDst * sj/vecLth );
			appPosZ = sz + ( appDst * sk/vecLth );

			vecLth = sqrt((ei * ei) + (ej * ej) + (ek * ek)); 

			ei = ei/vecLth;
			ej = ej/vecLth;
			ek = ek/vecLth;

			retPosX = ex + ( retDst * ei/vecLth );
			retPosY = ey + ( retDst * ej/vecLth );
			retPosZ = ez + ( retDst * ek/vecLth );

			break;

		case ScanInCylEndIsPlane :

			currentScanInCylEndIsPlane = (ScanInCylEndIsPlaneCommand *) trajCommand;

			cx = currentScanInCylEndIsPlane->getCx();
			cy = currentScanInCylEndIsPlane->getCy();
			cz = currentScanInCylEndIsPlane->getCz();
			ci = currentScanInCylEndIsPlane->getCI();
			cj = currentScanInCylEndIsPlane->getCJ();
			ck = currentScanInCylEndIsPlane->getCK();
			sx = currentScanInCylEndIsPlane->getSx();
			sy = currentScanInCylEndIsPlane->getSy();
			sz = currentScanInCylEndIsPlane->getSz();
			si = currentScanInCylEndIsPlane->getSI();
			sj = currentScanInCylEndIsPlane->getSJ();
			sk = currentScanInCylEndIsPlane->getSK();
			dx = currentScanInCylEndIsPlane->getDx();
			dy = currentScanInCylEndIsPlane->getDy();
			dz = currentScanInCylEndIsPlane->getDz();
			px = currentScanInCylEndIsPlane->getPx();
			py = currentScanInCylEndIsPlane->getPy();
			pz = currentScanInCylEndIsPlane->getPz();
			pi = currentScanInCylEndIsPlane->getPI();
			pj = currentScanInCylEndIsPlane->getPJ();
			pk = currentScanInCylEndIsPlane->getPK();
			ei = currentScanInCylEndIsPlane->getEI();
			ej = currentScanInCylEndIsPlane->getEJ();
			ek = currentScanInCylEndIsPlane->getEK();
			num = currentScanInCylEndIsPlane->getNum();
			stepW = currentScanInCylEndIsPlane->getStepW();


			helix.findHelixParameters(cx,cy,cz,dz,dy,dz,px,py,pz,sx,sy,sz,ci,cj,ck,&beta,&d);

			rx = sx - cx;
			ry = sy - cy;
			rz = sz - cz;

			radius = sqrt((rx * rx) + (ry * ry) + (rz * rz));

			rx = (rx / radius);
			ry = (ry / radius);
			rz = (rz / radius);

			radius += probe_radius;

			nx = ci;
			ny = cj;
			nz = ck;
			
			num = sqrt((nx * nx) + (ny * ny) + (nz * nz));

			nx = nx / num;
			ny = ny / num;
			nz = nz / num;

			vx = (ny * rz) - (ry * nz);
			vy = (nz * rx) - (rz * nx);
			vz = (nx * ry) - (rx * ny);

			srchPosX = sx; 
	        srchPosY = sy; 
	        srchPosZ = sz;

			nomPosX = px;
			nomPosY = py;
			nomPosZ = pz;

			vecLth = sqrt((si * si) + (sj * sj) + (sk * sk));

			appPosX = sx + ( appDst * si/vecLth );
			appPosY = sy + ( appDst * sj/vecLth );
			appPosZ = sz + ( appDst * sk/vecLth );

			vecLth = sqrt((ei * ei) + (ej * ej) + (ek * ek)); 

			ei = ei/vecLth;
			ej = ej/vecLth;
			ek = ek/vecLth;

			retPosX = px + ( retDst * ei/vecLth );
			retPosY = py + ( retDst * ej/vecLth );
			retPosZ = pz + ( retDst * ek/vecLth );

			break;


		default : ;

	  }	    

	  if(debug)
	  {
		sprintf(msg, "probe_radius = %.4lf",probe_radius);
	    serverLog.writeFile(msg);
	    memset(msg, 0, sizeof(msg));

	    sprintf(msg, "appPosX = %.4lf  appPosY = %.4lf  appPosZ = %.4lf",appPosX,appPosY,appPosZ);
	    serverLog.writeFile(msg);
	    memset(msg, 0, sizeof(msg));

	    sprintf(msg, "srchPosX = %.4lf  srchPosY = %.4lf  srchPosZ = %.4lf",srchPosX,srchPosY,srchPosZ);
	    serverLog.writeFile(msg);
	    memset(msg, 0, sizeof(msg));

	    sprintf(msg, "retPosX = %.4lf  retPosY = %.4lf  retPosZ = %.4lf",retPosX,retPosY,retPosZ);
	    serverLog.writeFile(msg);
	    memset(msg, 0, sizeof(msg));
	  }

	  moveState = MoveApproach;
	  break;

	case MoveApproach :

		if (trajState == TrajInit)
		{
		  if(debug)
		  {
			  sprintf(msg, "***************** MoveApproach *******************");
	          serverLog.writeFile(msg);
	          memset(msg, 0, sizeof(msg));
		  }

          // set commanded coordinates
          cmdPosX = appPosX;
          cmdPosY = appPosY;
          cmdPosZ = appPosZ;

		  //Get PtMeasPars velocity and acceleration
		  cmdVel = goToPars->getActSpeed();
          cmdAcl = goToPars->getActAccel();
		}

		commandTime += timeStep;
		computeLinearStep();

		if (trajState == TrajDone)
		{
			moveState = MoveSearch;
			trajState = TrajInit;
		}
		break;

	case MoveSearch :

		if (trajState == TrajInit)
		{
		  if(debug)
		  {
		     sprintf(msg, "***************** MoveSearch *******************");
	         serverLog.writeFile(msg);
	         memset(msg, 0, sizeof(msg));
		  }

		  // get current coordinates
	      bgnPosX = OutsideWorld->getCurrentX();
          bgnPosY = OutsideWorld->getCurrentY();
          bgnPosZ = OutsideWorld->getCurrentZ();

          // set commanded coordinates
          cmdPosX = srchPosX;
          cmdPosY = srchPosY;
          cmdPosZ = srchPosZ;

		  //Get PtMeasPars velocity and acceleration
		  cmdVel = ptMeasPars->getActSpeed();
          cmdAcl = ptMeasPars->getActAccel();
		  
		}

		commandTime += timeStep;
		computeLinearStep();

		if (trajState == TrajDone)
		{
			moveState = MoveScan;
			trajState = TrajInit;
		}
		break;

	case MoveScan :
		if (trajState == TrajInit)
		{
			if(debug)
		  {
		     sprintf(msg, "***************** MoveScan *******************");
	         serverLog.writeFile(msg);
	         memset(msg, 0, sizeof(msg));
		  }

		  // get current coordinates
	      bgnPosX = srchPosX;
          bgnPosY = srchPosY;
          bgnPosZ = srchPosZ;

          // set commanded coordinates
          cmdPosX = nomPosX;
          cmdPosY = nomPosY;
          cmdPosZ = nomPosZ;

		  // compute substeps based on stepW
		  if(trajType == Linear)
			computeLinearMotionComponents();
		  else if(trajType == Circular)
			computeCircularMotionComponents();
		  else if(trajType == Helical)
			computeHelicalMotionComponents();
		 
		  timeInc = totalExecTime/(deltaDist/stepW);

		  timeIncTotal = timeInc;

		  if (debug){
			  sprintf(msg, "timeInc = %.4lf  timeStep = %.4lf",timeInc,timeStep);
			  serverLog.writeFile(msg);
	          memset(msg, 0, sizeof(msg));
		  }

		}

		// compute steps based on time est w/ stepW

		steps = (int)(timeStep/timeInc);

		if (debug){
			sprintf(msg, "steps = %d  timeStep= %.4lf timeInc= %.4lf",steps, timeStep,timeInc);
			serverLog.writeFile(msg);
			memset(msg, 0, sizeof(msg));
		}

		if(steps > 0) { 
		  
		  for (i=0; i<steps; i++)
		  {
			scanning = true;
			timeStep = timeInc * (i+1);
			commandTime += timeStep;

			//commandTime += timeInc;

			if(trajType == Circular) {
				if(debug){	
					sprintf(msg, "for loop = %d", i);
					serverLog.writeFile(msg);
					memset(msg, 0, sizeof(msg));
				}
				computeCircularStep();
			}
			else if(trajType == Linear)
				computeLinearStep();
			else if(trajType == Helical)
				computeHelicalStep();

			if(trajState == TrajDone){
				if(scanVals > 0) reportScanData();
				moveState = MoveRetract;
				trajState = TrajInit;
				break;
			}
		  }
		}

		else  
		{		  
		  commandTime += timeStep;
		  if (timeIncTotal < commandTime)
		  {
			scanning = true;
		    double holdCommandTime = commandTime;
		    commandTime = timeIncTotal;
		    if(trajType == Circular)
			  computeCircularStep();
		    else if(trajType == Linear)
			  computeLinearStep();
		    else if(trajType == Helical)
		  	  computeHelicalStep();

		    if(trajState == TrajDone){
				if(scanVals > 0) reportScanData();
				moveState = MoveRetract;
				trajState = TrajInit;
				break;
			}
			timeIncTotal += timeInc;
		    commandTime = holdCommandTime;
			scanning = false;
		  }

		  if(trajState != TrajDone){
		    if(trajType == Circular)
			  computeCircularStep();
		    else if(trajType == Linear)
		  	  computeLinearStep();
		    else if(trajType == Helical)
			  computeHelicalStep();

		    if(trajState == TrajDone){
				if(scanVals > 0) reportScanData();
				moveState = MoveRetract;
				trajState = TrajInit;
				break;
			}
		  }
		}

		break;

	case MoveRetract :

		if (trajState == TrajInit)
		{
		  if(debug) 
		  {
		    sprintf(msg, "***************** MoveRetract *******************");
	        serverLog.writeFile(msg);
	        memset(msg, 0, sizeof(msg));
		  }

			// get current coordinates
	      bgnPosX = OutsideWorld->getCurrentX();
          bgnPosY = OutsideWorld->getCurrentY();
          bgnPosZ = OutsideWorld->getCurrentZ();

          // set commanded coordinates
          cmdPosX = retPosX;
          cmdPosY = retPosY;
          cmdPosZ = retPosZ;

		  //Get GoToPars velocity and acceleration
		  cmdVel = goToPars->getActSpeed();
          cmdAcl = goToPars->getActAccel();
		}
		
		commandTime += timeStep;
		computeLinearStep();

		if (trajState == TrajDone)
		{
		  //Generate Completed Response:  
          unsigned int tagNum = trajCommand->getTag()->getTagNumber();
          Response * x = new Response(tagNum, trajCommand->getTag()->getTagType(), Complete);
          returningValues->push_back(x);

          OutsideWorld->setReadiness(ready);
		  moveState = MoveDone;
		  trajState = TrajInit;
		}
		break;

	default: ;;
  }


} // end startTrajScanStraight()


void TrajGen::initializeVariables()
{
	trajState = TrajInit;

	scanning = false;

	bgnPosX = 0; bgnPosY = 0; bgnPosZ = 0;
	curPosX = 0; curPosY = 0; curPosZ = 0;
	cmdPosX = 0; cmdPosY = 0; cmdPosZ = 0;
	prvPosX = 0; prvPosY = 0; prvPosZ = 0;

	nomPosX = 0; nomPosY = 0; nomPosZ = 0;
	appPosX = 0; appPosY = 0; appPosZ = 0;
	srchPosX = 0; srchPosY = 0; srchPosY = 0;
	retPosX = 0; retPosY = 0; retPosZ = 0;


	rx = 0; ry = 0; rz = 0;
	vx = 0; vy = 0; vz = 0;


	deltaDist = 0, deltaX = 0; deltaY = 0, deltaZ = 0;

	currentDist;

	dirX=0; dirY=0; dirZ=0;

	yaw = 0; pitch = 0;

    cmdVel = 0; cmdVelX = 0; cmdVelY = 0, cmdVelZ = 0;
	cmdAcl = 0; cmdAclX = 0; cmdAclY = 0, cmdAclZ = 0;

	aclTransTime = 0; velTransTime = 0; totalExecTime = 0;
	commandTime = 0; reportTime = 0, onMoveReportTime = 0;

	aclDist = 0; aclDistX = 0; aclDistY = 0; aclDistZ = 0;
	velDist = 0; velDistX = 0; velDistY = 0; velDistZ = 0;

	appDst = 0; srchDst = 0; retDst = 0;

	ii = 0; jj = 0; kk = 0;

	reportDist = 0; onMoveReportDist = 0;

	timeStep = 0, timeInc = 0, timeIncTotal = 0;

} // end initailizeVariables()

void TrajGen::stepTraj()
{
  currentTime = clock();
  timeStep = (double)(currentTime - lastTime) / CLOCKS_PER_SEC;
  lastTime = currentTime;
	  
  commandNameType cnt = trajCommand->getCommandName();
  
  switch (cnt) {
  case GoTo                      : stepTrajGoTo();
		                           break;
  case Home                      : stepTrajHome();
		                           break;
  case PtMeas                    : stepTrajPtMeas();
		                           break;
  case AlignTool                 : stepTrajAlignTool();
		                           break;
  case ScanOnCircle              : stepTrajScan();
		                           break;
  case ScanOnLine                : stepTrajScan();
		                           break;
  case ScanInPlaneEndIsSphere    : stepTrajScan();
		                           break;
  case ScanInPlaneEndIsPlane     : stepTrajScan();
		                           break;
  case ScanInPlaneEndIsCyl       : stepTrajScan();
		                           break;
  case ScanInCylEndIsSphere      : stepTrajScan();
			                       break;
  case ScanInCylEndIsPlane       : stepTrajScan();
			                       break;
  default                        : ;
  }
} // end stepTraj()


bool TrajGen::reportMoveStatus()
{
	bool generateReport = false;

	if ( OutsideWorld->getOnMoveHasDis() )
	{
	  onMoveReportDist = OutsideWorld->getOnMoveDis();

	  if(debug){
	    sprintf(msg, "reportDist = %.4lf  onMoveReportDist = %.4lf",reportDist,onMoveReportDist);
	    serverLog.writeFile(msg);
	    memset(msg, 0, sizeof(msg));
	  }
	  if(reportDist > onMoveReportDist) 
	  {
		generateReport = true;
		reportDist = 0;
	  }
	}
	if ( OutsideWorld->getOnMoveHasTime() )
	{
	  onMoveReportTime = OutsideWorld->getOnMoveTime();
	  if(reportTime > onMoveReportTime)
	  {
		 generateReport = true;
		 reportTime = 0;
	  }
	}
		
	return generateReport;
} // end reportMoveStatus
 

double TrajGen::computeLinearDiff(double x1, double y1, double z1, 
							double x2, double y2, double z2)
{
	double diffX = (x2 - x1);
	double diffY = (y2 - y1);
	double diffZ = (z2 - z1);

	return sqrt(diffX*diffX + diffY*diffY + diffZ*diffZ);

}  // end computeLinearDiff()


void TrajGen::computeLinearMotionComponents()
{

	// Compute distance components
	deltaX = (cmdPosX - bgnPosX);
	deltaY = (cmdPosY - bgnPosY);
	deltaZ = (cmdPosZ - bgnPosZ);

	dirX = ((deltaX < 0) ? -1 : 1 );
	dirY = ((deltaY < 0) ? -1 : 1 );
	dirZ = ((deltaZ < 0) ? -1 : 1 );

	tmpSq = deltaX * deltaX + deltaY * deltaY;
	tmp2 = tmpSq + deltaZ * deltaZ;

	deltaDist = sqrt( tmp2 );

  if (deltaDist > 0)
  {


	// Compute angles

	if ( fabs ( deltaX ) > 0.01 )
	{
	  yaw = atan ( deltaY / deltaX ); //signs taken cared in pos 
	    if ( yaw < 0.0 )
		  yaw = - yaw;
	}
	else
	  yaw = PI/2.0;

	if ( tmpSq > 0.0001 )
	{
	  pitch = atan ( deltaZ / sqrt ( tmpSq ));
	  if ( pitch < 0.0 )
		pitch = - pitch;
	}
	else
	  pitch = PI/2.0;


	// Compute velocity components
	cmdVelX = cmdVel * (cos(pitch)) * cos(yaw);		
	cmdVelY = cmdVel * (cos(pitch)) * sin(yaw);
	cmdVelZ = cmdVel * sin(pitch);

	// Compute acceleration components
	cmdAclX = cmdAcl * (cos(pitch)) * cos(yaw);		
	cmdAclY = cmdAcl * (cos(pitch)) * sin(yaw);
	cmdAclZ = cmdAcl * sin(pitch);

	// Compute motion transition times
	aclTransTime = cmdVel/cmdAcl;
	aclDist = cmdAcl * aclTransTime * aclTransTime / 2;
	velDist = deltaDist - aclDist * 2;
	if (velDist < 0 ) { velDist = 0; }
	velTransTime = velDist/cmdVel;
	totalExecTime = aclTransTime * 2 + velTransTime;

	// Compute acceleration/deceleration distance components
	aclDistX = cmdAclX * aclTransTime * aclTransTime / 2;
    aclDistY = cmdAclY * aclTransTime * aclTransTime / 2;
    aclDistZ = cmdAclZ * aclTransTime * aclTransTime / 2;

	// Compute constant velocity distance components
	velDistX = cmdVelX * velTransTime;
	velDistY = cmdVelY * velTransTime;
	velDistZ = cmdVelZ * velTransTime;
  }

  if (debug) 
  {
	
	sprintf(msg, "********** linear motion parameters ***********");
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "deltaX = %.4lf  deltaY = %.4lf  deltaZ = %.4lf",deltaX,deltaY,deltaZ);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "dirX = %.4lf  dirY = %.4lf  dir = %.4lf",dirX,dirY,dirZ);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "tmpSq = %.4lf  tmp2 = %.4lf",tmpSq,tmp2);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "deltaDist = %.4lf  yaw = %.4lf  pitch = %.4lf", deltaDist,yaw,pitch);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "cmdVelX = %.4lf  cmdVelY = %.4lf  cmdVelZ = %.4lf", cmdVelX,cmdVelY,cmdVelZ);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "cmdAclX = %.4lf  cmdAclY = %.4lf  cmdAclZ = %.4lf", cmdAclX,cmdAclY,cmdAclZ);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "aclTransTime = %.4lf  aclDist = %.4lf", aclTransTime,aclDist);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "velTransTime = %.4lf  velDist = %.4lf", velTransTime,velDist);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "totalExecTime = %.4lf", totalExecTime);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "aclDistX = %.4lf  aclDistY = %.4lf  aclDistZ = %.4lf", aclDistX,aclDistY,aclDistZ);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));

	sprintf(msg, "velDistX = %.4lf  velDistY = %.4lf  velDistZ = %.4lf", velDistX,velDistY,velDistZ);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));
  }

} // end computLinearMotionComponents()

void TrajGen::computeCircularMotionComponents()
{
	deltaDist = totalAngle;
	angVel = cmdVel / radius;
	totalExecTime = deltaDist/angVel;
	
	if(debug){
	  sprintf(msg, "********** circular motion parameters ***********");
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));

	  sprintf(msg, "deltaDist = %.4lf  totalExecTime = %.4lf  angVel = %.4lf",
		            deltaDist,totalExecTime,angVel);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));
	}

} // end computeCircularMotionComponents()


void TrajGen::computeHelicalMotionComponents()
{
	deltaDist = sqrt((d*d) + (beta*radius*beta*radius));		
    totalExecTime = deltaDist/cmdVel;

	if (debug) {
	  sprintf(msg, "********** helical motion parameters ***********");
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));

	  sprintf(msg, "deltaDist = %.4lf  totalExecTime = %.4lf",deltaDist,totalExecTime);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));
	}
}

void TrajGen::computeLinearStep()
{
  /******************************************/
  /*   state 0: initialize parameters       */
  /*   state 1: accelerate to velocity      */
  /*   state 2: move at velocity            */
  /*   state 3: decelerate to stop          */
  /*   state 4: done                        */
  /******************************************/

	long double timeFactor;

    //commandTime += timeStep;

	if(debug){
	  sprintf(msg, "commandTime = %.4lf", commandTime);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));
	}

	

  if(trajState == TrajInit) {

    // relative command distance initialization
	  currentDist = 0;

	// get start coordinates
	prvPosX = bgnPosX;
	prvPosY = bgnPosY;
	prvPosZ = bgnPosZ;

	if(debug)
	{
      sprintf(msg, "Entering traj state %d", trajState);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));

	  sprintf(msg, "bgnPosX = %.4lf  bgnPosY = %.4lf  bgnPosZ = %.4lf", bgnPosX,bgnPosY,bgnPosZ);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));

	  sprintf(msg, "cmdVel = %.4lf  cmdAcl = %.4lf", cmdVel,cmdAcl);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));

	  sprintf(msg, "cmdPosX = %.4lf  cmdPosY = %.4lf  cmdPosZ = %.4lf", cmdPosX,cmdPosY,cmdPosZ);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));
	}

	computeLinearMotionComponents();

	trajState = TrajAccel;  // end of goto state 0

  } 

  if(trajState == TrajAccel) // accelerating
  {
	 
	if (debug)
	{
	  sprintf(msg, "Entering traj state %d", trajState);
  	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));
	}

	if (commandTime < aclTransTime && currentDist < deltaDist)
	{
	  // compute current position based on simulation time.
	  curPosX = bgnPosX + dirX * (cmdAclX * commandTime * commandTime/2);
	  curPosY = bgnPosY + dirY * (cmdAclY * commandTime * commandTime/2);
	  curPosZ = bgnPosZ + dirZ * (cmdAclZ * commandTime * commandTime/2);

	  OutsideWorld->putCurrentXYZ(curPosX, curPosY, curPosZ);
	  if(scanning) onScanCollectData();
	}
	else 
	{
	  trajState = TrajConstVel;
	}
  }

  if(trajState == TrajConstVel) // const velocity
  { 

	if(debug){
	  sprintf(msg, "Entering traj state %d", trajState);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));
	}

	if ((commandTime < (aclTransTime + velTransTime) && 
		commandTime >= aclTransTime) && currentDist < deltaDist) 
	{	
		// compute current position based on simulation time.
		timeFactor = commandTime - aclTransTime;
		curPosX = bgnPosX + dirX * (aclDistX + cmdVelX*(timeFactor));
		curPosY = bgnPosY + dirY * (aclDistY + cmdVelY*(timeFactor)); 
		curPosZ = bgnPosZ + dirZ * (aclDistZ + cmdVelZ*(timeFactor));

		OutsideWorld->putCurrentXYZ(curPosX, curPosY, curPosZ);
		if(scanning) onScanCollectData();
	}
	else 
	{
	  trajState = TrajDecel;
	}

  }

  if(trajState == TrajDecel) // decelerating
  {
	if(debug){
	  sprintf(msg, "Entering traj state %d", trajState);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg)); 
	}

	if ((commandTime < totalExecTime &&
		commandTime > (aclTransTime + velTransTime)) && currentDist < deltaDist)
	{
	  // compute current position based on simulation time.
	  timeFactor = commandTime - aclTransTime - velTransTime;
	  curPosX = bgnPosX + dirX * (aclDistX + velDistX + 
	            cmdVelX * timeFactor - cmdAclX * timeFactor * timeFactor/2);
	  curPosY = bgnPosY + dirY * (aclDistY + velDistY +
		        cmdVelY * timeFactor - cmdAclY * timeFactor * timeFactor/2); 
	  curPosZ = bgnPosZ + dirZ * (aclDistZ + velDistZ +
		        cmdVelZ * timeFactor - cmdAclZ * timeFactor * timeFactor/2);

	  OutsideWorld->putCurrentXYZ(curPosX, curPosY, curPosZ);
	  if(scanning) onScanCollectData();
	}
	else 
	{
	  //curPosX = bgnPosX + dirX * (aclDistX * 2 + velDistX);
	  //curPosY = bgnPosY + dirY * (aclDistY * 2 + velDistY);
	  //curPosZ = bgnPosZ + dirZ * (aclDistZ * 2 + velDistZ);

	  curPosX = cmdPosX;
	  curPosY = cmdPosY;
	  curPosZ = cmdPosZ;

	  OutsideWorld->putCurrentXYZ(curPosX, curPosY, curPosZ);
	  if(scanning) onScanCollectData();

	  trajState = TrajDone;
	  commandTime = 0;
	}
  }

  // compute relative command distance
  currentDist = computeLinearDiff(bgnPosX,bgnPosY,bgnPosZ,curPosX,curPosY,curPosZ);

  // compute report variables
  reportDist += computeLinearDiff(prvPosX,prvPosY,prvPosZ,curPosX,curPosY,curPosZ);
  reportTime += timeStep;

  prvPosX = curPosX;
  prvPosY = curPosY;
  prvPosZ = curPosZ;

  scanning = false;

  if (showXYZ)
  {
    sprintf(msg, "curPosX = %.4lf  curPosY = %.4lf  curPosZ = %.4lf\n", curPosX,curPosY,curPosZ);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));
	
	sprintf(msg, "currentDist = %.4lf\n", currentDist);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));
  }
	

}

void TrajGen::computeCircularStep(){

  double currentAngle;

  if(debug){	
	sprintf(msg, "commandTime = %.4lf", commandTime);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));
  }

  if(trajState == TrajInit) 
  {
	if (debug) {
	  sprintf(msg, "Circular traj state %d", trajState);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));
	}

	prvPosX = bgnPosX;
	prvPosY = bgnPosY;
	prvPosZ = bgnPosZ;

	computeCircularMotionComponents();

	trajState = TrajConstVel;  

  } 
  if(trajState == TrajConstVel) // const velocity
  { 

	if(debug){
	  sprintf(msg, "Circular traj state %d", trajState);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));
	}

	if (commandTime < totalExecTime) 
	{	
	  // compute current position based on simulation time.
	  currentAngle = angVel * commandTime;

	  curPosX = cx + (radius * ((rx * cos(currentAngle)) + (vx * sin(currentAngle))));
	  curPosY = cy + (radius * ((ry * cos(currentAngle)) + (vy * sin(currentAngle)))); 
	  curPosZ = cz + (radius * ((rz * cos(currentAngle)) + (vz * sin(currentAngle)))); 

	  OutsideWorld->putCurrentXYZ(curPosX, curPosY, curPosZ);
	  if(scanning) onScanCollectData();
	}
	else 
	{
	  curPosX = nomPosX;
	  curPosY = nomPosY;
	  curPosZ = nomPosZ;

	  OutsideWorld->putCurrentXYZ(curPosX, curPosY, curPosZ);
	  if(scanning) onScanCollectData();

	  trajState = TrajDone;
	  commandTime = 0;
	}
  }

  // compute report variables
  //reportDist += currentAngle * radius;
  reportDist += computeLinearDiff(prvPosX,prvPosY,prvPosZ,curPosX,curPosY,curPosZ);
  reportTime += timeStep;

  prvPosX = curPosX;
  prvPosY = curPosY;
  prvPosZ = curPosZ;

  scanning = false;

  if (showXYZ)
  {
    sprintf(msg, "curPosX = %.4lf  curPosY = %.4lf  curPosZ = %.4lf\n", curPosX,curPosY,curPosZ);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));
  }

}

void TrajGen::computeHelicalStep(){

  double currentAngle;

  if(debug){
	sprintf(msg, "commandTime = %.4lf", commandTime);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));
  }

  if(trajState == TrajInit) {

	if (debug) {
	  sprintf(msg, "Helical traj state %d", trajState);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));
	}

	prvPosX = bgnPosX;
	prvPosY = bgnPosY;
	prvPosZ = bgnPosZ;

    computeHelicalMotionComponents();

	trajState = TrajConstVel;  

  } 

  if(trajState == TrajConstVel) // const velocity
  { 

	if(debug){
	  sprintf(msg, "Helical traj state %d", trajState);
	  serverLog.writeFile(msg);
	  memset(msg, 0, sizeof(msg));
	}

	if (commandTime < totalExecTime) 
	{	
		// compute current position based on simulation time.
		double f = d * (commandTime/totalExecTime);
		currentAngle = beta * (commandTime/totalExecTime);

		double wx = cx + (radius * ((rx * cos(currentAngle)) + (vx * sin(currentAngle))));
		double wy = cy + (radius * ((ry * cos(currentAngle)) + (vy * sin(currentAngle)))); 
		double wz = cz + (radius * ((rz * cos(currentAngle)) + (vz * sin(currentAngle))));
		
		curPosX = wx + (f * ci);
		curPosY = wy + (f * cj);
		curPosZ = wz + (f * ck);

		if (debug)
		{
           sprintf(msg, "f = %.4lf  currentAngle = %.4lf  beta = %.4lf d = %.4lf", 
			   f,currentAngle,beta, d);
	       serverLog.writeFile(msg);
	       memset(msg, 0, sizeof(msg));

		   sprintf(msg, "cx = %.4lf  cy = %.4lf  cz = %.4lf", cx,cy,cz);
		   serverLog.writeFile(msg);
	       memset(msg, 0, sizeof(msg));

		   sprintf(msg, "rx = %.4lf  ry = %.4lf  rz = %.4lf", rx,ry,rz);
		   serverLog.writeFile(msg);
	       memset(msg, 0, sizeof(msg));

		   sprintf(msg, "vx = %.4lf  vy = %.4lf  vz = %.4lf", vx,vy,vz);
		   serverLog.writeFile(msg);
	       memset(msg, 0, sizeof(msg));

		   sprintf(msg, "wx = %.4lf  wy = %.4lf  wz = %.4lf", wx,wy,wz);
		   serverLog.writeFile(msg);
	       memset(msg, 0, sizeof(msg));
		   
		}

		OutsideWorld->putCurrentXYZ(curPosX, curPosY, curPosZ);
		if(scanning) onScanCollectData();
	}
	else 
	{		
		curPosX = cx + (radius * ((rx * cos(beta)) + (vx * sin(beta)))) + (d * ci);
		curPosY = cy + (radius * ((ry * cos(beta)) + (vy * sin(beta)))) + (d * cj);
		curPosZ = cz + (radius * ((rz * cos(beta)) + (vz * sin(beta)))) + (d * ck);
		
		OutsideWorld->putCurrentXYZ(curPosX, curPosY, curPosZ);
		if(scanning) onScanCollectData();

		trajState = TrajDone;
		commandTime = 0;
	}
  }

  // compute report variables
  reportDist += computeLinearDiff(prvPosX,prvPosY,prvPosZ,curPosX,curPosY,curPosZ);
  reportTime += timeStep;

  prvPosX = curPosX;
  prvPosY = curPosY;
  prvPosZ = curPosZ;

  scanning = false;

  if (showXYZ)
  {
    sprintf(msg, "curPosX = %.4lf  curPosY = %.4lf  curPosZ = %.4lf\n", curPosX,curPosY,curPosZ);
	serverLog.writeFile(msg);
	memset(msg, 0, sizeof(msg));
  }

}

void TrajGen::generatePtMeasReport()
{
	// Generate response based on data request by the last OnPtmeasReport command
	double r = -1, x = -1, y = -1, z = -1, toolA = -1, toolB = -1, toolC = -1, 
          er = -1, i = -1, j = -1, k = -1;
	bool hasR = false, hasX = false, hasY = false, hasZ = false, hasER = false;
	bool hasToolA = false, hasToolB = false, hasToolC = false, hasIJK = false;
	
	if (OutsideWorld->getOnPtmeasHasR()){
		r = OutsideWorld->getCurrentR();
		hasR = true;
	}
	if (OutsideWorld->getOnPtmeasHasX()){
		x = OutsideWorld->getCurrentX();
		hasX = true;
	}
	if (OutsideWorld->getOnPtmeasHasY()){
		y = OutsideWorld->getCurrentY();
		hasY = true;
	}
	if (OutsideWorld->getOnPtmeasHasZ()){
		z = OutsideWorld->getCurrentZ();
		hasZ = true;
	}
	if (OutsideWorld->getOnPtmeasHasToolA()){
		toolA = OutsideWorld->getCurrentToolA();
		hasToolA = true;
	}
	if (OutsideWorld->getOnPtmeasHasToolB()){
		toolB = OutsideWorld->getCurrentToolB();
		hasToolB = true;
	}
	if (OutsideWorld->getOnPtmeasHasToolC()){
		toolC = OutsideWorld->getCurrentToolC();
		hasToolC = true;
	}
	if (OutsideWorld->getOnPtmeasHasER()){
		er = OutsideWorld->getProbeRadius();
		hasER = true;
	}

	if (OutsideWorld->getOnPtmeasHasIJK()){
		// Note if scanning us 001, however, if
		// single point must compute ijk from 
		// start and destination.
		i = 0;
		j = 0;
		k = 1;
		hasIJK = true;
	}

	if(hasR==true || hasX==true || hasY==true || hasZ==true || 
		hasToolA==true || hasToolB==true || hasToolC==true ||
		hasIJK==true || hasER==true) {

		unsigned int tag_num = currentPtMeas->getTag()->getTagNumber();
		Response * res = new PtMeasOrGetResponse(tag_num, hasER, hasIJK, 
			hasR, hasToolA, hasToolB, hasToolC, hasX, hasY, hasZ, 
			er,i, j, k, r, toolA, toolB, toolC, x, y, z);
		returningValues->push_back(res);
	}
}

void TrajGen::onScanSetData(){

	scanVals = 0;

	hasQ = OutsideWorld->getOnScanHasQ();
	hasR = OutsideWorld->getOnScanHasR();
	hasToolA = OutsideWorld->getOnScanHasToolA();
	hasToolB = OutsideWorld->getOnScanHasToolB();
	hasToolC = OutsideWorld->getOnScanHasToolC();
	hasX = OutsideWorld->getOnScanHasX();
	hasY = OutsideWorld->getOnScanHasY();
	hasZ = OutsideWorld->getOnScanHasZ();
	hasER = OutsideWorld->getOnScanHasER();
	hasIJK = OutsideWorld->getOnScanHasIJK();

	if(hasQ) scanVals++;
	if(hasR) scanVals++;
	if(hasToolA) scanVals++;
	if(hasToolB) scanVals++;
	if(hasToolC) scanVals++;
	if(hasX) scanVals++;
	if(hasY) scanVals++;
	if(hasZ) scanVals++;
	if(hasER) scanVals++;
	if(hasIJK) scanVals += 3;
}

void TrajGen::onScanCollectData(){

	if(hasQ)
	{
		scanData.push(0);
	}
	if(hasR)
	{
		scanData.push(OutsideWorld->getCurrentR());
	}
	if(hasToolA)
	{
		scanData.push(OutsideWorld->getCurrentToolA());
	}
	if(hasToolB)
	{
		scanData.push(OutsideWorld->getCurrentToolB());
	}
	if(hasToolC)
	{
		scanData.push(OutsideWorld->getCurrentToolC());
	}
	if(hasX)
	{
		scanData.push(OutsideWorld->getCurrentX());
	}
	if(hasY)
	{
		scanData.push(OutsideWorld->getCurrentY());
	}
	if(hasZ)
	{
		scanData.push(OutsideWorld->getCurrentZ());
	}
	if(hasER)
	{
	//	scanData.push(tools->getActiveTool()->getLength());
	}
	if(hasIJK)
	{
		scanData.push(0);
		scanData.push(0);
		scanData.push(1);
	}

}


void TrajGen::reportScanData()
{
	int z =0;
	// max is # data values per line (ie. MAX 50 and scanVals is 7, then max is 49
	int max =  (int(MAXSCAN/scanVals))*scanVals;
	int dataCount = scanData.size();
	int divisions;
	int processed = 0;
	double * data;

	
	if (dataCount%max == 0)
		divisions= dataCount/max;
	else
		divisions= dataCount/max + 1;

	if(debug){
		sprintf(msg, "dataCount = %d  divisions %d  scanVals = %d  max = %d", dataCount, divisions, scanVals, max);
		serverLog.writeFile(msg);
		memset(msg, 0, sizeof(msg));
	}

	for(int i=0; i<divisions; i++)
	{
		processed = 0;
		if (scanData.size() > max){
			processed = max;
			data = new double[max];
		}
		else{
			processed = scanData.size();
			data = new double[processed];
		}
		
		if(debug){
		  sprintf(msg, "divisions = %d scanDataSize = %d  processed = %d", divisions, scanData.size(), processed);
		  serverLog.writeFile(msg);
		  memset(msg, 0, sizeof(msg));
		}
	
		for(int l=0; l<processed; l++)
		{
			data[l] = scanData.front();
			if(debug){
			  sprintf(msg, "data pop = %.6lf", scanData.front());
			  serverLog.writeFile(msg);
			  memset(msg, 0, sizeof(msg));
			}
			scanData.pop();
		}
	
		//generate scan data reponse
        unsigned int tagNum = trajCommand->getTag()->getTagNumber();
        Response * x = new ScanResponse(tagNum, processed, data);
        returningValues->push_back(x);

	}
}

void TrajGen::abortCommandExecution()
{
	 char cmdStr[IPPSIZE/2] = "";
     trajCommand->getCommandString(cmdStr,IPPSIZE-1);
	 serverLog.replaceQuotes(cmdStr);

	 unsigned int tag_num = trajCommand->getTag()->getTagNumber();
     Response * x = new ErrorResponse(tag_num, CommandTag, 0006, cmdStr);
     returningValues->push_back(x);

     x = new Response(tag_num, CommandTag, Complete);
     returningValues->push_back(x);

	 trajState = TrajInit;
	 moveState = MoveDone;
	 OutsideWorld->setReadiness(aborted);

	 //trajCommand = NULL;
}

void TrajGen::resetTrajectoryGenerator()
{

	 trajState = TrajInit;
	 moveState = MoveDone;
	 //OutsideWorld->putCurrentXYZ(0, 0, 0);  I don't believe this should happen
}
