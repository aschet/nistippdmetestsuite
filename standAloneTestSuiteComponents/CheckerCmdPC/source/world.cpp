/* #include "../Server/StdAfx.h" */
#include "world.h"

/*******************************************************************/

/* csyTransform::update

*/

void csyTransform::update(
 double x0_in,
 double y0_in,
 double z0_in,
 double theta_in,
 double psi_in,
 double phi_in)
{
  double c1;
  double s1;
  double c2;
  double s2;
  double c3;
  double s3;
  
  x0 = x0_in;
  y0 = y0_in;
  z0 = z0_in;
  theta = theta_in;
  psi = psi_in;
  phi = phi_in;
  c1 = cos((theta * IPP_PI) / 180.0);
  s1 = sin((theta * IPP_PI) / 180.0);
  c2 = cos((psi * IPP_PI) / 180.0);
  s2 = sin((psi * IPP_PI) / 180.0);
  c3 = cos((phi * IPP_PI) / 180.0);
  s3 = sin((phi * IPP_PI) / 180.0);
  M11 = ((c2 * c3) - (c1 * s2 * s3));
  M12 = ((s2 * c3) + (c1 * c2 * s3));
  M13 = (s1 * s3);
  M21 = (-(c2 * s3) - (c1 * s2 * c3));
  M22 = (-(s2 * s3) + (c1 * c2 * c3));
  M23 = (s1 * c3);
  M31 = (s1 * s2);
  M32 = -(s1 * c2);
  M33 = c1;
}

/*******************************************************************/

/* world::daemonExists

There is only one daemon in the world that may be alive some of the
time.  That is the onMoveReportE daemon. Thus, this function returns
true only if the on_move_tag_number is not zero. There is no need for
an OnPtMeasReport daemon or an OnScanReport daemon. In those cases a
point is reported whenever it is taken. What gets reported is
controlled by whatever the last OnPtMeasReport or OnScanReport command
specified.

The spec creates the impression that many daemons may be alive in the
world at the same time, but there is no need for this. On page 21 the
spec suggests a daemon should come alive while an Event command is
being executed and then die immediately, but that seems pointless
since the daemon does not do anything while it is alive.

*/

bool world::daemonExists(int tagNumber)
{
  return ((tagNumber == 0) ? false:
	  (bool)(tagNumber == onMoveTagNumber));
}

/*******************************************************************/

/* world::getCurrentAlignment

This makes a OnePropertyAlignment and returns a pointer to it. The
OnePropertyAlignment is made from alignment1 and alignment2 (which
belong to the world's toolChanger) converted into the current
coordinate system (which belongs to the world). This function is
defined to make it easy to construct a response to a
GetProp(Tool.Alignment()) command.

*/

OnePropertyAlignment * world::getCurrentAlignment()
{
  double i1;             /* i1 of OnePropertyAlignment in current csy  */
  double j1;             /* j1 of OnePropertyAlignment in current csy  */
  double k1;             /* k1 of OnePropertyAlignment in current csy  */
  double i2;             /* i2 of OnePropertyAlignment in current csy  */
  double j2;             /* j2 of OnePropertyAlignment in current csy  */
  double k2;             /* k2 of OnePropertyAlignment in current csy  */
  bool has2;             /* has2 of OnePropertyAlignment               */
  csyTransform * theSys; /* current coordinate system transform        */
  double mi;             /* i component in machine coordinates         */
  double mj;             /* j component in machine coordinates         */
  double mk;             /* k component in machine coordinates         */

  theSys = ((Csy == MoveableMachineCsy) ? &csyMoveable :
	    (Csy == MultipleArmCsy) ? &csyMultiple :
	    (Csy == PartCsy) ? &csyPart :
	    NULL); /* for MachineCsy */
  mi = tools.getAlignment1()->getI();
  mk = tools.getAlignment1()->getJ();
  mj = tools.getAlignment1()->getK();

  if (theSys)
    {
      i1 = theSys->fwdDirectionX(mi, mj, mk);
      j1 = theSys->fwdDirectionY(mi, mj, mk);
      k1 = theSys->fwdDirectionZ(mi, mj, mk);
    }
  else
    {
      i1 = mi;
      j1 = mj;
      k1 = mk;
    }
  mi = tools.getAlignment2()->getI();
  mk = tools.getAlignment2()->getJ();
  mj = tools.getAlignment2()->getK();
  if ((mi == 0) && (mj == 0) && (mk == 0))
    {
      has2 = false;
      i2 = 0.0;
      j2 = 0.0;
      k2 = 0.0;
    }
  else if (theSys)
    {
      has2 = true;
      i2 = theSys->fwdDirectionX(mi, mj, mk);
      j2 = theSys->fwdDirectionY(mi, mj, mk);
      k2 = theSys->fwdDirectionZ(mi, mj, mk);
    }
  else
    {
      has2 = true;
      i2 = mi;
      j2 = mj;
      k2 = mk;
    }
  return new OnePropertyAlignment(i1, j1, k1, i2, j2, k2, has2);
}

/*******************************************************************/

/* world::inWorkVolume

This returns true if point (x,y,z) is in the work volume of the
machine and false if not. The point (x,y,z) is given in terms of the
current coordinate system.

This is not currently correcting for the tool offset. That should be
added. A long tool offset may put the CMM mechanism out of bounds
while the tool tip is still in bounds (or vice versa).

*/

bool world::inWorkVolume(
 double x,
 double y,
 double z)
{
  csyTransform * theSys; /* current coordinate system transform */
  double mx;             /* current X in machine coordinates    */
  double my;             /* current Y in machine coordinates    */
  double mz;             /* current Z in machine coordinates    */

  theSys = ((Csy == MoveableMachineCsy) ? &csyMoveable :
	    (Csy == MultipleArmCsy) ? &csyMultiple :
	    (Csy == PartCsy) ? &csyPart :
	    NULL); /* for MachineCsy */
  if (theSys)
    {
      mx = theSys->invPointX(x, y, z);
      my = theSys->invPointY(x, y, z);
      mz = theSys->invPointZ(x, y, z);
    }
  else
    {
      mx = x;
      my = y;
      mz = z;
    }
  return (bool)((mx > limitLowerX) && (mx < limitUpperX) &&
		(my > limitLowerY) && (my < limitUpperY) &&
		(mz > limitLowerZ) && (mz < limitUpperZ));
}

/*******************************************************************/

/* world::update

This update function updates the world model to what it should be
after the Command (that is the argument to update) is fully executed.
This is a crude simulation, since every command appears to execute
instantaneously. For better simulations, for example when commands
that take time (all motion commands) are executed, the simulation
program should either (1) not call update until the motion is ended
or (2) not call update at all if the simulation is updating the
world gradually itself.

If the modifications to the world model are simple, this function
updates the model itself. If the modifications are not simple, this
calls a subordinate function to do the updating.

The userEnabled flag is set to false only by a DisableUser command.
It is not clear in the discussion on pages 39, 42, 44 of the spec
whether the flag should also be set to false in other situations
(such as GoTo or PtMeas being executed).

This is assuming ChangeTool does not change the FoundTool.

This assumes that if ChangeTool is called and the named tool is not
known, nothing will change. An alternative is to remove the current
tool but replace it with nothing (in which case the active tool would
be NoTool after the command was executed).

This assumes that if FindTool is called and the named tool is not
found, nothing will change. An alternative is to have the found tool
be NoTool after the command is executed.

Executing AbortE should flush the slow command queue, but that is
not implemented here on the assumption that the queue is being
moved out of the world into the server.

Changing the Tool will normally change the location of the controlled
point, but this is not being modeled here.

The AlignPart command should probably update currentR, but it is not
clear how this should be done, and it is not currently being done.

In the case of CenterPart, the spec (page 96) says the relationship
between machine coordinates and part coordinates is lost. Here, we
are not updating the world in any way when CenterPart is executed.
TiltCenterPart and TiltPart also do not change the world.

*/

void world::update(
 Command * aCommand)
{
  outOfRangeFlag = false;
  switch (aCommand->getCommandName())
    {
    case AbortE:
      readiness = aborted;
      onMoveTagNumber = 0;
      break;
    case AlignPart:
      break;
    case AlignTool:
      updateAlignTool((AlignToolCommand *)aCommand);
      break;
    case CenterPart:
      break;
    case ChangeTool:
      updateChangeTool((ChangeToolCommand *)aCommand);
      break;
    case ClearAllErrors:
      readiness = ready;
      break;
    case DisableUser:
      userEnabled = false;
      break;
    case EnableUser:
      userEnabled = true;
      break;
    case EndSession:
      updateEndSession();
      break;
    case EnumAllProp:
      break;
    case EnumProp:
      break;
    case EnumTools:
      break;
    case FindTool:
      FindToolCommand * theFind;
      theFind = (FindToolCommand *)aCommand;
      tools.setFoundTool(tools.findTool(theFind->getToolName()));
      break;
    case Get:
      /* printf("current location (%lf, %lf, %lf)\n", */
      /*        currentX, currentY, currentZ); */
      break;
    case GetChangeToolAction:
      break;
    case GetCoordSystem:
      break;
    case GetCsyTransformation:
      break;
    case GetDMEVersion:
      break;
    case ipp::GetErrorInfo:
      break;
    case GetErrStatusE:
      break;
    case GetMachineClass:
      break;
    case ipp::GetProp:
      break;
    case GetPropE:
      break;
    case GetXtdErrStatus:
      break;
    case GoTo:
      GoToCommand * theGoTo;
      theGoTo = (GoToCommand *)aCommand;
      if (theGoTo->getHasX())
	currentX = theGoTo->getX();
      if (theGoTo->getHasY())
	currentY = theGoTo->getY();
      if (theGoTo->getHasZ())
	currentZ = theGoTo->getZ();
      break;
    case Home:
      updateHome();
      break;
    case IsHomed:
      break;
    case IsUserEnabled:
      break;
    case LockAxis:
      updateLockAxis((LockAxisCommand *)aCommand);
      break;
    case OnMoveReportE:
      updateOnMoveReportE((OnMoveReportECommand *)aCommand);
      break;
    case OnPtMeasReport:
      updateOnPtMeasReport((OnPtMeasReportCommand *)aCommand);
      break;
    case OnScanReport:
      updateOnScanReport((OnScanReportCommand *)aCommand);
      break;
    case PtMeas:
      updatePtMeas((PtMeasCommand *)aCommand);
      break;
    case ReQualify:
      if ((tools.getActiveTool() != tools.getUnDefTool()) &&
	  (tools.getActiveTool() != tools.getNoTool()))
	tools.getActiveTool()->setIsQualified(true);
      break;
    case ScanInCylEndIsPlane:
      updateScanInCylEndIsPlane((ScanInCylEndIsPlaneCommand *)aCommand);
      break;
    case ScanInCylEndIsSphere:
      updateScanInCylEndIsSphere((ScanInCylEndIsSphereCommand *)aCommand);
      break;
    case ScanInPlaneEndIsCyl:
      updateScanInPlaneEndIsCyl((ScanInPlaneEndIsCylCommand *)aCommand);
      break;
    case ScanInPlaneEndIsPlane:
      updateScanInPlaneEndIsPlane((ScanInPlaneEndIsPlaneCommand *)aCommand);
      break;
    case ScanInPlaneEndIsSphere:
      updateScanInPlaneEndIsSphere((ScanInPlaneEndIsSphereCommand *)aCommand);
      break;
    case ScanOnCircle:
      updateScanOnCircle((ScanOnCircleCommand *)aCommand);
      break;
    case ScanOnCircleHint:
      break;
    case ScanOnLine:
      updateScanOnLine((ScanOnLineCommand *)aCommand);
      break;
    case ScanOnLineHint:
      break;
    case ScanUnknownHint:
      break;
    case SetCoordSystem:
      updateSetCoordSystem((SetCoordSystemCommand *)aCommand);
      break;
    case SetCsyTransformation:
      updateSetCsyTransformation((SetCsyTransformationCommand *)aCommand);
      break;
    case ipp::SetProp:
      updateSetProp((SetPropCommand *)aCommand);
      break;
    case SetTool:
      SetToolCommand * theSet;
      theSet = (SetToolCommand *)aCommand;
      tools.setActiveTool(tools.findTool(theSet->getToolName()));
      break;
    case StartSession:
      updateStartSession();
      break;
    case StopAllDaemons:
      onMoveTagNumber = 0;
      break;
    case StopDaemon:
      StopDaemonCommand * theStop;
      int tagNumber;
      theStop = (StopDaemonCommand *)aCommand;
      tagNumber = theStop->getDaemonTagNumber();
      if (daemonExists(tagNumber))
	onMoveTagNumber = 0;
      break;
    case TiltCenterPart:
      break;
    case TiltPart:
      break;
    default: // should never get here
      printf("Bug in update - unknown command\n");
      break;
    }
}

/*******************************************************************/

/* world::updateAlignTool

This sets the alignment of the current tool (in the tools). This
does not set any angles of the current tool because its geometry is
unknown.

The command gives the alignment in the current coordinate system, but
the data is stored in machine coordinates, so the data is transformed
(if necessary) before being stored.

*/

void world::updateAlignTool(
 AlignToolCommand * theAlignTool)
{
  double i;              /* i component in current csy          */
  double j;              /* j component in current csy          */
  double k;              /* k component in current csy          */
  csyTransform * theSys; /* current coordinate system transform */
  double mi;             /* i component in machine coordinates  */
  double mj;             /* j component in machine coordinates  */
  double mk;             /* k component in machine coordinates  */

  theSys = ((Csy == MoveableMachineCsy) ? &csyMoveable :
	    (Csy == MultipleArmCsy) ? &csyMultiple :
	    (Csy == PartCsy) ? &csyPart :
	    NULL); /* for MachineCsy */
  i = theAlignTool->getVector1i();
  j = theAlignTool->getVector1j();
  k = theAlignTool->getVector1k();
  if (theSys)
    {
      mi = theSys->invDirectionX(i, j, k);
      mj = theSys->invDirectionY(i, j, k);
      mk = theSys->invDirectionZ(i, j, k);
      tools.getAlignment1()->setValues(mi, mj, mk);
    }
  else
    tools.getAlignment1()->setValues(i, j, k);
  if (theAlignTool->getHas2())
    {
      i = theAlignTool->getVector2i();
      j = theAlignTool->getVector2j();
      k = theAlignTool->getVector2k();
      if (theSys)
	{
	  mi = theSys->invDirectionX(i, j, k);
	  mj = theSys->invDirectionY(i, j, k);
	  mk = theSys->invDirectionZ(i, j, k);
	  tools.getAlignment2()->setValues(mi, mj, mk);
	}
      else
	tools.getAlignment2()->setValues(i, j, k);
    }
}

/*******************************************************************/

/* world::updateChangeTool

Executing ChangeTool changes the tool and unlocks all axes (see
page 98 concerning locking axes). It also resets alignment1 of the
world's toolChanger to (0, 0, 1) and alignment2 to (0, 0, 0).

*/

void world::updateChangeTool(
 ChangeToolCommand * theChangeTool)
{
  tools.setActiveTool(tools.findTool(theChangeTool->getToolName()));
  lockedC = false;
  lockedPhi = false;
  lockedR = false;
  lockedX = false;
  lockedY = false;
  lockedZ= false;
  tools.getAlignment1()->setValues(0, 0, 1);
  tools.getAlignment2()->setValues(0, 0, 0);
}

/*******************************************************************/

/* world::updateEndSession

This:
1. kills any on-move daemon (by setting onMoveTagNumber to zero).
2. resets all the onMove items to false (spec does not say what to do)
3. sets onPtmeasHasX, onPtmeasHasY, and onPtmeasHasZ to true
   and all other onPtmeas items to false (see page 41).
4. set onScanHasQ, onScanHasX, onScanHasY, and onScanHasZ to true
   and all other onScan items to false (see page 41).
5. sets inSession to false.
6. sets isHomed to false (but we may want to change this).

This does NOT change:
1. readiness.
2. userEnabled.
3. locking of axes (see page 98, which does not say what to do).

It probably is not actually necessary to reset anything except
inSession, since StartSession does the same things this does, except
for the way it sets inSession. 

*/

void world::updateEndSession()
{
  inSession = false;
  isHomed = false;
  onMoveHasDis = false;
  onMoveHasR = false;
  onMoveHasTime = false;
  onMoveHasToolA = false;
  onMoveHasToolB = false;
  onMoveHasToolC = false;
  onMoveHasX = false;
  onMoveHasY = false;
  onMoveHasZ = false;
  onMoveTagNumber = 0;
  onPtmeasHasER = false;
  onPtmeasHasIJK = false;
  onPtmeasHasR = false;
  onPtmeasHasToolA = false;
  onPtmeasHasToolB = false;
  onPtmeasHasToolC = false;
  onPtmeasHasX = true;
  onPtmeasHasY = true;
  onPtmeasHasZ = true;
  onScanHasER = false;
  onScanHasIJK = false;
  onScanHasQ = true;
  onScanHasR = false;
  onScanHasToolA = false;
  onScanHasToolB = false;
  onScanHasToolC = false;
  onScanHasX = true;
  onScanHasY = true;
  onScanHasZ = true;
}

/*******************************************************************/

/* world::updateHome

The spec says the Home command results in moving to a fixed home
location. Here we assume the Home command leaves the controlled point
for NoTool at (homeX, homeY, homeZ) in the machine coordinate system. The
values in the current coordinate system are calculated and stored in
currentX, currentY, and currentZ. This really should also be
adjusted for the offset of the current tool, but that is not being
done here.

*/

void world::updateHome()
{
  csyTransform * theSys;

  isHomed = true;
  theSys = ((Csy == MoveableMachineCsy) ? &csyMoveable :
	    (Csy == MultipleArmCsy) ? &csyMultiple :
	    (Csy == PartCsy) ? &csyPart :
	    NULL); /* for MachineCsy */
  if (theSys)
    {
      currentX = theSys->fwdPointX(homeX, homeY, homeZ);
      currentY = theSys->fwdPointY(homeX, homeY, homeZ);
      currentZ = theSys->fwdPointZ(homeX, homeY, homeZ);
    }
  else
    {
      currentX = homeX;
      currentY = homeY;
      currentZ = homeZ;
    }
}

/*******************************************************************/

/* world::updateLockAxis

The LockAxis command is described on page 98. It has the following
problems, which are handled has described here. The basic approach is
to set the axis to locked if any LockAxis command locks it, to unlock
all axes when a tool is changed, and otherwise to ignore whether an
axis is locked or not.

1. The only way to unlock an axis is with a ChangeTool Command.
This seems unwise.

2. The spec does not say whether the axis being locked is an actual
hardware axis, which would normally be an MCS axis, or whether the
axis being locked is an axis in the current coordinate system (which
would be implemented in software). Here, it does not matter, since
locking axes has no effect except to change lockedness.

3. The spec says axis motion on locked axes will be disabled even
for those commands that explicitly move the axis, and no error should
be reported in this case. This seems very risky. For commands like
PtMeas, what to do if an axis is locked is not specified, so that the
spec is ambiguous. Here, all motion commands are executed the same
whether axes are locked or not.

4. The spec does not say whether the lockedness of an axis persists
between sessions. Here it does persist.

5. The spec includes locking a Phi axis. The Phi axis is not defined
anywhere.

6. The spec says "Locking Phi() will only enable moves in R, Z, and C."
It is not clear whether this is intended to mean that locking Phi
also locks X and Y. It is not clear whether locking Phi unlocks R, Z
and C. Here, locking Phi only locks Phi; it does not lock or unlock any
other axis.

*/

void world::updateLockAxis(
 LockAxisCommand * theLockAxis)
{
  if (theLockAxis->getHasC())
    lockedC = true;
  if (theLockAxis->getHasPhi())
    lockedPhi = true;
  if (theLockAxis->getHasR())
    lockedR = true;
  lockedX = false;
  lockedY = false;
  lockedZ= false;
}

/*******************************************************************/

/* world::updateOnMoveReportE

*/

void world::updateOnMoveReportE(
 OnMoveReportECommand * theOnMove)
{
  onMoveDis = theOnMove->getTheDis();
  onMoveHasDis = theOnMove->getHasDis();
  onMoveHasR = theOnMove->getHasR();
  onMoveHasTime = theOnMove->getHasTime();
  onMoveHasToolA = theOnMove->getHasToolA();
  onMoveHasToolB = theOnMove->getHasToolB();
  onMoveHasToolC = theOnMove->getHasToolC();
  onMoveHasX = theOnMove->getHasX();
  onMoveHasY = theOnMove->getHasY();
  onMoveHasZ = theOnMove->getHasZ();
  onMoveTime = theOnMove->getTheTime();
  if (onMoveHasR || onMoveHasX || onMoveHasY || onMoveHasZ ||
      onMoveHasToolA || onMoveHasToolB || onMoveHasToolC ||
      onMoveHasDis || onMoveHasTime)
    onMoveTagNumber = theOnMove->getTag()->getTagNumber();
  else
    onMoveTagNumber = 0;
}

/*******************************************************************/

/* world::updateOnPtMeasReport

*/

void world::updateOnPtMeasReport(
 OnPtMeasReportCommand * theOnPtMeas)
{
  onPtmeasHasER = theOnPtMeas->getHasER();
  onPtmeasHasIJK = theOnPtMeas->getHasIJK();
  onPtmeasHasR = theOnPtMeas->getHasR();
  onPtmeasHasToolA = theOnPtMeas->getHasToolA();
  onPtmeasHasToolB = theOnPtMeas->getHasToolB();
  onPtmeasHasToolC = theOnPtMeas->getHasToolC();
  onPtmeasHasX = theOnPtMeas->getHasX();
  onPtmeasHasY = theOnPtMeas->getHasY();
  onPtmeasHasZ = theOnPtMeas->getHasZ();
}

/*******************************************************************/

/* world::updateOnScanReport

*/

void world::updateOnScanReport(
 OnScanReportCommand * theOnScan)
{
  onScanHasER = theOnScan->getHasER();
  onScanHasIJK = theOnScan->getHasIJK();
  onScanHasQ = theOnScan->getHasQ();
  onScanHasR = theOnScan->getHasR();
  onScanHasToolA = theOnScan->getHasToolA();
  onScanHasToolB = theOnScan->getHasToolB();
  onScanHasToolC = theOnScan->getHasToolC();
  onScanHasX = theOnScan->getHasX();
  onScanHasY = theOnScan->getHasY();
  onScanHasZ = theOnScan->getHasZ();
}

/*******************************************************************/

/* world::updatePtMeas

For each of X, Y, and Z specified in the PtMeas, this sets the
measuredX1, measuredY1, or measuredZ1 to the value specified by the
PtMeas. If any of X, Y, or Z is not specified in the PtMeas, the
corresponding measured coordinate continues to have its current
value. The effect of this is that the 1st measured point is reported
to be exactly at the nominal point. This sets pointTag to the tag
number of the tag of the PtMeas to indicate that a point was found
during execution of the PtMeas.

As specified on page 43 of the spec, if the retract distance for the
current tool is negative, this sets the current location after the
PtMeas to be at the approach point. If the retract distance is not
negative, this sets the current location to be one retract distance
in the IJK direction from the nominal point.

The details of this are a little messy because IJK may or may not be
specified in the PtMeas, and the function is written so it will not
crash the system in case of two errors (1) the active tool is NoTool
or UnDefTool or (2) the nominal point is the current point. Both of
these errors should be detected earlier and never occur here, but
it is best to be prepared. If either error occurs, the current
location after the PtMeas is set to the nominal point.

*/

void world::updatePtMeas(
 PtMeasCommand * thePtMeas)
{
  double i;
  double j;
  double k;
  double length; /* length of IJK */
  double move;   /* how far to move away after PtMeas */

  measuredX1 = ((thePtMeas->getHasX()) ? thePtMeas->getX() : currentX);
  measuredY1 = ((thePtMeas->getHasY()) ? thePtMeas->getY() : currentY);
  measuredZ1 = ((thePtMeas->getHasZ()) ? thePtMeas->getZ() : currentZ);
  pointTag = thePtMeas->getTag()->getTagNumber();
  if ((tools.getActiveTool() == tools.getUnDefTool()) ||
      (tools.getActiveTool() == tools.getNoTool()))
    move = 0;
  else
    move = tools.getActiveTool()->getPtMeasPar()->getActRetract();
  if (move < 0)
    move = tools.getActiveTool()->getPtMeasPar()->getActApproach();
  if (thePtMeas->getHasIJK())
    {
      i = thePtMeas->getI();
      j = thePtMeas->getJ();
      k = thePtMeas->getK();
    }
  else
    {
      i = (currentX - measuredX1);
      j = (currentY - measuredY1);
      k = (currentZ - measuredZ1);
    }
  length = sqrt((i * i) + (j * j) + (k * k));
  if (length == 0)
    {
      currentX = measuredX1;
      currentY = measuredY1;
      currentZ = measuredZ1;
    }
  else
    {
      currentX = (measuredX1 + (move * (i/length)));
      currentY = (measuredY1 + (move * (j/length)));
      currentZ = (measuredZ1 + (move * (k/length)));
    }
}

/*******************************************************************/

/* world::updateScanInCylEndIsPlane

This sets:
1st measured point to the start point of the scan
2nd measured point to the direction point of the scan
3rd measured point to the defining point of the stop plane
current point to the defining point of the stop plane

The three measured points can be used to construct a data response.

The definition of the scan does not provide an end point, so the
point on the plane is used instead. This is the least bad of the
several bad choices available for updating the current point.

*/

void world::updateScanInCylEndIsPlane(
 ScanInCylEndIsPlaneCommand * theScan)
{
  measuredX1 = theScan->getSx();
  measuredY1 = theScan->getSy();
  measuredZ1 = theScan->getSz();
  measuredX2 = theScan->getDx();
  measuredY2 = theScan->getDy();
  measuredZ2 = theScan->getDz();
  measuredX3 = theScan->getPx();
  measuredY3 = theScan->getPy();
  measuredZ3 = theScan->getPz();
  currentX = theScan->getPx();
  currentY = theScan->getPy();
  currentZ = theScan->getPz();
}


/*******************************************************************/

/* world::updateScanInCylEndIsSphere

This sets:
1st measured point to the start point of the scan
2nd measured point to the direction point of the scan
3rd measured point to the center point of the stop sphere
current point to the center point of the stop sphere

The three measured points can be used to construct a data response.

The definition of the scan does not provide an end point, so the
center point of the stop sphere is used instead. This is probably
a reasonable point to use, even though it is not necessarily the
end point of the scan.

*/

void world::updateScanInCylEndIsSphere(
 ScanInCylEndIsSphereCommand * theScan)
{
  measuredX1 = theScan->getSx();
  measuredY1 = theScan->getSy();
  measuredZ1 = theScan->getSz();
  measuredX2 = theScan->getDx();
  measuredY2 = theScan->getDy();
  measuredZ2 = theScan->getDz();
  measuredX3 = theScan->getEx();
  measuredY3 = theScan->getEy();
  measuredZ3 = theScan->getEz();
  currentX = theScan->getEx();
  currentY = theScan->getEy();
  currentZ = theScan->getEz();
}

/*******************************************************************/

/* world::updateScanInPlaneEndIsCyl

This sets:
1st measured point to the start point of the scan
2nd measured point to the direction point of the scan
3rd measured point to the axis point of the stop cylinder
current point to the axis point of the stop cylinder

The three measured points can be used to construct a data response.

The definition of the scan does not provide an end point, so the
center point of the stop cylinder is used instead. This is the least
bad choice.

*/

void world::updateScanInPlaneEndIsCyl(
 ScanInPlaneEndIsCylCommand * theScan)
{
  measuredX1 = theScan->getSx();
  measuredY1 = theScan->getSy();
  measuredZ1 = theScan->getSz();
  measuredX2 = theScan->getDx();
  measuredY2 = theScan->getDy();
  measuredZ2 = theScan->getDz();
  measuredX3 = theScan->getCx();
  measuredY3 = theScan->getCy();
  measuredZ3 = theScan->getCz();
  currentX = theScan->getCx();
  currentY = theScan->getCy();
  currentZ = theScan->getCz();
}

/*******************************************************************/

/* world::updateScanInPlaneEndIsPlane

This sets:
1st measured point to the start point of the scan
2nd measured point to the direction point of the scan
3rd measured point to the definition point of the stop plane
current point to the definition point of the stop plane

The three measured points can be used to construct a data response.

The definition of the scan does not provide an end point, so the
definition point of the stop plane is used instead. This is the least
bad choice.

*/

void world::updateScanInPlaneEndIsPlane(
 ScanInPlaneEndIsPlaneCommand * theScan)
{
  measuredX1 = theScan->getSx();
  measuredY1 = theScan->getSy();
  measuredZ1 = theScan->getSz();
  measuredX2 = theScan->getDx();
  measuredY2 = theScan->getDy();
  measuredZ2 = theScan->getDz();
  measuredX3 = theScan->getPx();
  measuredY3 = theScan->getPy();
  measuredZ3 = theScan->getPz();
  currentX = theScan->getPx();
  currentY = theScan->getPy();
  currentZ = theScan->getPz();
}

/*******************************************************************/

/* world::updateScanInPlaneEndIsSphere

This sets:
1st measured point to the start point of the scan
2nd measured point to the direction point of the scan
3rd measured point to the end point of the scan
current point to the end point of the scan

The three measured points can be used to construct a data response.

*/

void world::updateScanInPlaneEndIsSphere(
 ScanInPlaneEndIsSphereCommand * theScan)
{
  measuredX1 = theScan->getSx();
  measuredY1 = theScan->getSy();
  measuredZ1 = theScan->getSz();
  measuredX2 = theScan->getDx();
  measuredY2 = theScan->getDy();
  measuredZ2 = theScan->getDz();
  measuredX3 = theScan->getEx();
  measuredY3 = theScan->getEy();
  measuredZ3 = theScan->getEz();
  currentX = theScan->getEx();
  currentY = theScan->getEy();
  currentZ = theScan->getEz();
}

/*******************************************************************/

/* world::updateScanOnCircle

This sets:
1st measured point to the start point of the scan
2nd measured point to the direction point of the scan
3rd measured point to the end point of the scan
current point to the end point of the scan

The three measured points can be used to construct a data response.

The end point of the scan is not given and must be calculated from
the given data. The calculation is based on the usual method of
converting translated polar coordinates to cartesian coordinates
in the XY plane.
P = C + ((r * cos(angle))X + (r * sin(angle))Y)
where C is the translation, X is the unit vector on the X-axis and
Y is the unit vector on the Y-axis.

Here, X is replaced by the vector R from the center of the circle to
the start point, and Y is replaced by V = NxR normalized, (cross product)
where N is the normal to the plane of the circle.

This is not checking for R or N being zero. That is done by the
command checker, and the command checker should have been run before
this is called.


*/

void world::updateScanOnCircle(
 ScanOnCircleCommand * theScan)
{
  double Rx;
  double Ry;
  double Rz;
  double Nx;
  double Ny;
  double Nz;
  double Vx;
  double Vy;
  double Vz;
  double Dx;
  double Dy;
  double Dz;
  double Ex;
  double Ey;
  double Ez;
  double r;
  double n;
  double angle;

  Rx = (theScan->getSx() - theScan->getCx());
  Ry = (theScan->getSy() - theScan->getCy());
  Rz = (theScan->getSz() - theScan->getCz());
  r = sqrt((Rx * Rx) + (Ry * Ry) + (Rz * Rz));
  Rx = (Rx / r);
  Ry = (Ry / r);
  Rz = (Rz / r);
  Nx = theScan->getI();
  Ny = theScan->getJ();
  Nz = theScan->getK();
  n = sqrt((Nx * Nx) + (Ny * Ny) + (Nz * Nz));
  Nx = (Nx / n);
  Ny = (Ny / n);
  Nz = (Nz / n);
  Vx = ((Ny * Rz) - (Ry * Nz));
  Vy = ((Nz * Rx) - (Rz * Nx));
  Vz = ((Nx * Ry) - (Rx * Ny));
  angle = ((IPP_PI / 180) * theScan->getDelta());
  Ex = (theScan->getCx() + (r * ((Rx * cos(angle)) + (Vx * sin(angle)))));
  Ey = (theScan->getCy() + (r * ((Ry * cos(angle)) + (Vy * sin(angle)))));
  Ez = (theScan->getCz() + (r * ((Rz * cos(angle)) + (Vz * sin(angle)))));
  angle = ((IPP_PI / 180) * theScan->getStepW());
  Dx = (theScan->getCx() + (r * ((Rx * cos(angle)) + (Vx * sin(angle)))));
  Dy = (theScan->getCy() + (r * ((Ry * cos(angle)) + (Vy * sin(angle)))));
  Dz = (theScan->getCz() + (r * ((Rz * cos(angle)) + (Vz * sin(angle)))));
  measuredX1 = theScan->getSx();
  measuredY1 = theScan->getSy();
  measuredZ1 = theScan->getSz();
  measuredX2 = Dx;
  measuredY2 = Dy;
  measuredZ2 = Dz;
  measuredX3 = Ex;
  measuredY3 = Ey;
  measuredZ3 = Ez;
  currentX = Ex;
  currentY = Ey;
  currentZ = Ez;
}

/*******************************************************************/

void world::updateScanOnLine(
 ScanOnLineCommand * theScan)
{
  double Ex;
  double Ey;
  double Ez;
  double Sx;
  double Sy;
  double Sz;
  double dx;
  double dy;
  double dz;
  double f;

  Ex = theScan->getEx();
  Ey = theScan->getEy();
  Ez = theScan->getEz();
  Sx = theScan->getSx();
  Sy = theScan->getSy();
  Sz = theScan->getSz();
  dx = (Ex - Sx);
  dy = (Ey - Sy);
  dz = (Ez - Sz);
  f = (theScan->getStepW() / sqrt((dx * dx) + (dy * dy) + (dz * dz)));
  measuredX1 = Sx;
  measuredY1 = Sy;
  measuredZ1 = Sz;
  measuredX2 = (Sx + (f * dx));
  measuredY2 = (Sy + (f * dy));
  measuredZ2 = (Sz + (f * dz));
  measuredX3 = Ex;
  measuredY3 = Ey;
  measuredZ3 = Ez;
  currentX = Ex;
  currentY = Ey;
  currentZ = Ez;
}

/*******************************************************************/

/* world::updateSetCoordSystem

This takes a SetCoordSystemCommand and, if the coordinate system given
by the command is not the currently active coordinate system, updates
the current coordinate system and the location of the controlled point.

Setting the system for MachineCsy to &csyPart is not a bug; it
just has to be something valid and non-NULL in this case.

*/

void world::updateSetCoordSystem(
 SetCoordSystemCommand * theSet)
{
  coordSysType theNewSysType;
  csyTransform * theNewSys;
  csyTransform * theOldSys;
  double x;
  double y;
  double z;

  theNewSysType = theSet->getCsyType();
  if (theNewSysType != Csy)
    {
      theNewSys = ((theNewSysType == MoveableMachineCsy) ? &csyMoveable :
		   (theNewSysType == MultipleArmCsy) ? &csyMultiple :
		   (theNewSysType == PartCsy) ? &csyPart :
		   (theNewSysType == MachineCsy) ? &csyPart : NULL);
      theOldSys = ((Csy == MoveableMachineCsy) ? &csyMoveable :
		   (Csy == MultipleArmCsy) ? &csyMultiple :
		   (Csy == PartCsy) ? &csyPart :
		   (Csy == MachineCsy) ? &csyPart : NULL);
      if (theNewSys && (Csy != MachineCsy))
	{
	  x = currentX;
	  y = currentY;
	  z = currentZ;
	  currentX = theOldSys->invPointX(x, y, z);
	  currentY = theOldSys->invPointY(x, y, z);
	  currentZ = theOldSys->invPointZ(x, y, z);
	}
      if (theNewSys && (theNewSysType != MachineCsy))
	{
	  x = currentX;
	  y = currentY;
	  z = currentZ;
	  currentX = theNewSys->fwdPointX(x, y, z);
	  currentY = theNewSys->fwdPointY(x, y, z);
	  currentZ = theNewSys->fwdPointZ(x, y, z);
	}
      Csy = theNewSysType;
    }
}

/*******************************************************************/

/* world::updateSetCsyTransformation

This takes a SetCsyTransformationCommand and updates the given
coordinate system. If the given coordinate system is the currently
active coordinate system, this also updates the location of the
controlled point.

*/

void world::updateSetCsyTransformation(
 SetCsyTransformationCommand * theSet)
{
  coordSysType theSysType;
  csyTransform * theSys;
  double x;
  double y;
  double z;

  theSysType = theSet->getCoordSys();
  theSys = ((theSysType == JogDisplayCsy) ? &csyJogDisplay :
	    (theSysType == JogMoveCsy) ? &csyJogMove :
	    (theSysType == MultipleArmCsy) ? &csyMultiple :
	    (theSysType == PartCsy) ? &csyPart :
	    (theSysType == SensorCsy) ? &csySensor : NULL);
  if (theSys)
    {
      if (theSysType == Csy)
	{ /* if theSys in use, convert current location to machine coords */
	  x = currentX;
	  y = currentY;
	  z = currentZ;
	  currentX = theSys->invPointX(x, y, z);
	  currentY = theSys->invPointY(x, y, z);
	  currentZ = theSys->invPointZ(x, y, z);
	}
      theSys->update(theSet->getX0(), theSet->getY0(), theSet->getZ0(),
		     theSet->getTheta(), theSet->getPsi(), theSet->getPhi());
      if (theSysType == Csy)
	{ /* if theSys in use, convert current location from machine coords */
	  x = currentX;
	  y = currentY;
	  z = currentZ;
	  currentX = theSys->fwdPointX(x, y, z);
	  currentY = theSys->fwdPointY(x, y, z);
	  currentZ = theSys->fwdPointZ(x, y, z);
	}
    }
}

/*******************************************************************/

/* world::updateSetProp

SetProp updates PtMeas parameters or GoTo parameters of a tool.  A
parameter has a maximum, a minimum, a default value, and an actual
value. Only the actual value can be set. The parser signals an error
and does not parse the command if the command is a SetProp that tries
to set the maximum, minimum, or default. Thus, when the SetProp
command is executed only the actual value of a property being set is
considered. If an argument has four keywords, the only thing the
fourth keyword can be is "Act". The spec (version 1.4.0) implies that
both forms Tool.PtMeasPars.Speed() and Tool.PtMeasPars.Speed.Act()
are allowed, and they mean the same thing. In version 1.4.1, this is
stated explicitly. The same rule applies to all other parameters.
Thus, in updateSetProp, we do not look at the fourth keyword.

If the SetProp cannot be executed for one reason or another, this
does nothing.

If the tool is UnDefTool, SetProp cannot be executed.

If the tool is NoTool, SetProp of PtMeas parameters cannot be
executed.

In tools.h, if an attempt is made (by calling any of the set methods
used here) to set a parameter value greater than the maximum, the
value is set to the maximum, and if an attempt is made to set the
value less than the minimum, the value is set to the minimum.

In addition to what tools.h does, this function sets the world's
outOfRangeFlag to true if a value specified in an argument to SetProp
is greater than the allowed maximum or less than the allowed minimum.
The outOfRangeFlag is set back to false (by world::update()) just
before this function runs. After this function runs, the
outOfRangeFlag stays unchanged until the next time world::update()
is called.

It is clear from the spec that maxima and minima of properties are not
settable by SetProp, so that is not allowed here.

Note that, because of the way this behaves, the effect of a SetProp
may not be what is expected. To find out what SetProp did, it
can be followed by a GetProp.

*/

void world::updateSetProp(
 SetPropCommand * theSet)
{
  KTool * theTool;
  GoToPars * pars1;
  PtMeasPars * pars2;
  int n;
  int numberProps;
  double requested;

  numberProps = theSet->getNumberProps();
  for (n = 0; n < numberProps; n++)
    {
      requested = theSet->getValue(n);
      if (theSet->getKey1(n) == Tool)
	theTool = tools.getActiveTool();
      else if (theSet->getKey1(n) == FoundTool)
	theTool = tools.getFoundTool();
      else
	theTool = NULL;
      if (!(theTool));
      else if (theTool == tools.getUnDefTool());
      else if (theSet->getKey2(n) == GoToPar)
	{
	  pars1 = theTool->getGoToPar();
	  switch (theSet->getKey3(n))
	    {
	    case Speed:
	      if ((pars1->getMaxSpeed() < requested) ||
		  (pars1->getMinSpeed() > requested))
		outOfRangeFlag = true;
	      pars1->setActSpeed(requested);
	      break;
	    case Accel:
	      if ((pars1->getMaxAccel() < requested) ||
		  (pars1->getMinAccel() > requested))
		outOfRangeFlag = true;
	      pars1->setActAccel(requested);
	      break;
	    default:
	      break;
	    }
	}
      else if (theTool == tools.getNoTool());
      else if (theSet->getKey2(n) == PtMeasPar)
	{
	  pars2 = theTool->getPtMeasPar();
	  switch (theSet->getKey3(n))
	    {
	    case Speed:
	      if ((pars2->getMaxSpeed() < requested) ||
		  (pars2->getMinSpeed() > requested))
		outOfRangeFlag = true;
	      pars2->setActSpeed(requested);
	      break;
	    case Accel:
	      if ((pars2->getMaxAccel() < requested) ||
		  (pars2->getMinAccel() > requested))
		outOfRangeFlag = true;
	      pars2->setActAccel(requested);
	      break;
	    case Approach:
	      if ((pars2->getMaxApproach() < requested) ||
		  (pars2->getMinApproach() > requested))
		outOfRangeFlag = true;
	      pars2->setActApproach(requested);
	      break;
	    case Retract:
	      if ((pars2->getMaxRetract() < requested) ||
		  (pars2->getMinRetract() > requested))
		outOfRangeFlag = true;
	      pars2->setActRetract(requested);
	      break;
	    case Search:
	      if ((pars2->getMaxSearch() < requested) ||
		  (pars2->getMinSearch() > requested))
		outOfRangeFlag = true;
	      pars2->setActSearch(requested);
	      break;
	    default:
	      break;
	    }
	}
    }
}

/*******************************************************************/

/* world::updateStartSession

This:
1. sets readiness to ready.
2. sets inSession to true.
3. sets onPtmeasHasX, onPtmeasHasY, and onPtmeasHasZ to true
   and all other onPtmeas items to false (see page 41).
4. sets onScanHasQ, onScanHasX, onScanHasY, and onScanHasZ to true
   and all other onScan items to false (see page 41).
5. resets all the onMove items to false (spec does not say what to do).
6. kills any on-move daemon (by setting onMoveTagNumber to zero).

This does NOT change:
1. userEnabled.
2. locking of axes (see page 98, which does not say what to do)

*/

void world::updateStartSession()
{
  inSession = true;
  onMoveHasDis = false;
  onMoveHasR = false;
  onMoveHasTime = false;
  onMoveHasToolA = false;
  onMoveHasToolB = false;
  onMoveHasToolC = false;
  onMoveHasX = false;
  onMoveHasY = false;
  onMoveHasZ = false;
  onMoveTagNumber = 0;
  onPtmeasHasER = false;
  onPtmeasHasIJK = false;
  onPtmeasHasR = false;
  onPtmeasHasToolA = false;
  onPtmeasHasToolB = false;
  onPtmeasHasToolC = false;
  onPtmeasHasX = true;
  onPtmeasHasY = true;
  onPtmeasHasZ = true;
  onScanHasER = false;
  onScanHasIJK = false;
  onScanHasQ = true;
  onScanHasR = false;
  onScanHasToolA = false;
  onScanHasToolB = false;
  onScanHasToolC = false;
  onScanHasX = true;
  onScanHasY = true;
  onScanHasZ = true;
  readiness = ready;
}

/*******************************************************************/

/* world::world

This is the constructor for the world class.

*/

world::world()
{
  Csy = MachineCsy;
  // csyJogDisplay initialized automatically by the csyTransform constructor.
  // csyJogMove    initialized automatically by the csyTransform constructor.
  // csyMoveable   initialized automatically by the csyTransform constructor.
  // csyMultiple   initialized automatically by the csyTransform constructor.
  // csyPart       initialized automatically by the csyTransform constructor.
  // csySensor     initialized automatically by the csyTransform constructor.
  currentR = 0;
  currentX = 0;
  currentY = 0;
  currentZ = 0;
  homeX = 0.0;
  homeY = 0.0;
  homeZ = 0.0;
  inSession = false;
  isHomed = false;
  limitLowerX = -500;
  limitLowerY = -500;
  limitLowerZ = -500;
  limitUpperX = 500;
  limitUpperY = 500;
  limitUpperZ = 500;
  lockedC = false;
  lockedPhi = false;
  lockedR = false;
  lockedX = false;
  lockedY = false;
  lockedZ= false;
  sprintf(machineClass,"CartCMMWithRotaryTable");
  measuredX1 = 0;
  measuredY1 = 0;
  measuredZ1 = 0;
  measuredX2 = 0;
  measuredY2 = 0;
  measuredZ2 = 0;
  measuredX3 = 0;
  measuredY3 = 0;
  measuredZ3 = 0;
  onMoveHasDis = false;
  onMoveHasR = false;
  onMoveHasTime = false;
  onMoveHasToolA = false;
  onMoveHasToolB = false;
  onMoveHasToolC = false;
  onMoveHasX = false;
  onMoveHasY = false;
  onMoveHasZ = false;
  onMoveTagNumber = 0;
  onMoveTime = 0;
  onPtmeasHasER = false;
  onPtmeasHasIJK = false;
  onPtmeasHasR = false;
  onPtmeasHasToolA = false;
  onPtmeasHasToolB = false;
  onPtmeasHasToolC = false;
  onPtmeasHasX = true; // see page 41
  onPtmeasHasY = true; // see page 41
  onPtmeasHasZ = true; // see page 41
  onScanHasER = false;
  onScanHasIJK = false;
  onScanHasQ = true; // see page 41
  onScanHasR = false;
  onScanHasToolA = false;
  onScanHasToolB = false;
  onScanHasToolC = false;
  onScanHasX = true; // see page 41
  onScanHasY = true; // see page 41
  onScanHasZ = true; // see page 41
  outOfRangeFlag = false;
  pointTag = 0;
  readiness = ready;
  // tools initialized automatically by the ToolChanger constructor.
  userEnabled = false;
}

/*******************************************************************/
