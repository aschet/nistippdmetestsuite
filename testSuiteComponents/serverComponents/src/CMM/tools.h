#ifndef TOOLS_HH
#define TOOLS_HH

/*

For this version of tools.h, "the spec" is I++ DME Version 1.4.

The spec provides no method for adding a tool to the set of defined
tools.  Without such a method, tools have to be hard coded, which is
clearly not a practical way to add tools in a real system.  This file
provides an addTool method in the ToolChanger class to fix that problem.

The object model and the C++ code in the spec provide various
CanChange methods (see pages 29, 109). These are not needed and are not
used in this file. The server must be able to change anything related
to tools. What the client can change is implemented in the command
interface; there is no need to mirror that in the server.

A Param (parameter) can take on its maximum or minimum value.

The spec (on page 65) provides that when setting the actual value of a
parameter, and the requested value is out of the range given by the
min and max, then the value is set to its maximum if the setVal value
is larger than the maximum, and the value is set to its minimum if the
setVal value is smaller than the minimum. This behavior is implemented
here. Setting the default value is handled similarly.

All of the setting functions here return the value that was actually
set.

If a call to setMax tries to set the maximum less than the larger of
the current actual value and the current default value, the maximum is
set instead to the larger of the two.

If a call to setMin tries to set the minimum greater than the smaller
of the current actual value and the current default value, the minimum
is set instead to the smaller of the two.

In this file, the methods are defined so that there is no way that
the max of a parameter can be made less than its min and there is no way
that act or def can become out of range.

The abbreviations used here are the same as those in the spec. Act
means actual. Def means default.

*/

#include <vector>
#include "../CmdResClasses/IppCmdRes.h"  // for OnePropertyCollisionVolume

#define MAXTOOLS 50

class Param
{
private:  
  double act;
  double def;
  double max;
  double min;

public:
  Param() {act = 1.0; def = 1.0; max=10.0; min = 0.1;}
  ~Param(){}

  double getAct() {return act;}
  double setAct(double newAct)
    {
      act = ((newAct > max) ? max : (newAct < min) ? min : newAct);
      return act;
    }
  double getDef() {return def;}
  double setDef(double newDef)
    {
      def = ((newDef > max) ? max : (newDef < min) ? min : newDef);
      return def;
    }
  double getMax() {return max;}
  double setMax(double newMax)
    {
      double bigger;
      bigger = ((act > def) ? act : def);
      max = ((newMax > bigger) ? newMax : bigger); 
      return max;
    }
  double getMin() {return min;}
  double setMin(double newMin)
    {
      double smaller;
      smaller = ((act < def) ? act : def);
      min = ((newMin < smaller) ? newMin : smaller);
      return min;
    }
};

class GoToPars
{
private:
  Param speed;
  Param accel;

public:
  GoToPars(){};
  ~GoToPars(){};
  double getActAccel() {return accel.getAct();}
  double getActSpeed() {return speed.getAct();}
  double getDefAccel() {return accel.getDef();}
  double getDefSpeed() {return speed.getDef();}
  double getMaxAccel() {return accel.getMax();}
  double getMaxSpeed() {return speed.getMax();}
  double getMinAccel() {return accel.getMin();}
  double getMinSpeed() {return speed.getMin();}

  double setActAccel(double a) {return accel.setAct(a);}
  double setActSpeed(double s) {return speed.setAct(s);}
  double setDefAccel(double a) {return accel.setDef(a);}
  double setDefSpeed(double s) {return speed.setDef(s);}
  double setMaxAccel(double a) {return accel.setMax(a);}
  double setMaxSpeed(double s) {return speed.setMax(s);}
  double setMinAccel(double s) {return accel.setMin(s);}
  double setMinSpeed(double s) {return speed.setMin(s);}
};

class PtMeasPars
{
private:
  Param speed;
  Param accel;
  Param approach;
  Param search;
  Param retract;

public:
  PtMeasPars(){};
  ~PtMeasPars(){};

  double getActAccel() {return accel.getAct();}
  double getActApproach() {return approach.getAct();}
  double getActRetract() {return retract.getAct();}
  double getActSearch() {return search.getAct();}
  double getActSpeed() {return speed.getAct();}
  double getDefAccel() {return accel.getDef();}
  double getDefApproach() {return approach.getDef();}
  double getDefRetract() {return retract.getDef();}
  double getDefSearch() {return search.getDef();}
  double getDefSpeed() {return speed.getDef();}
  double getMaxAccel() {return accel.getMax();}
  double getMaxApproach() {return approach.getMax();}
  double getMaxRetract() {return retract.getMax();}
  double getMaxSearch() {return search.getMax();}
  double getMaxSpeed() {return speed.getMax();}
  double getMinAccel() {return accel.getMin();}
  double getMinApproach() {return approach.getMin();}
  double getMinRetract() {return retract.getMin();}
  double getMinSearch() {return search.getMin();}
  double getMinSpeed() {return speed.getMin();}

  double setActAccel(double a) {return accel.setAct(a);}
  double setActApproach(double a) {return approach.setAct(a);}
  double setActRetract(double a) {return retract.setAct(a);}
  double setActSearch(double s) {return search.setAct(s);}
  double setActSpeed(double s) {return speed.setAct(s);}
  double setDefAccel(double a) {return accel.setDef(a);}
  double setDefApproach(double a) {return approach.setDef(a);}
  double setDefRetract(double a) {return retract.setDef(a);}
  double setDefSearch(double s) {return search.setDef(s);}
  double setDefSpeed(double s) {return speed.setDef(s);}
  double setMaxAccel(double a) {return accel.setMax(a);}
  double setMaxApproach(double a) {return approach.setMax(a);}
  double setMaxRetract(double r) {return retract.setMax(r);}
  double setMaxSearch(double s) {return search.setMax(s);}
  double setMaxSpeed(double s) {return speed.setMax(s);}
  double setMinAccel(double s) {return accel.setMin(s);}
  double setMinApproach(double a) {return approach.setMin(a);}
  double setMinRetract(double r) {return retract.setMin(r);}
  double setMinSearch(double s) {return search.setMin(s);}
  double setMinSpeed(double s) {return speed.setMin(s);}

};

/* KTool class

A Ktool (tool) is the combination of (1) a stylus, (2) a tool holder,
and (3) a set of operational parameters.

Each tool has a collisionVolume which is a OnePropertyCollisionVolume
(defined in IppCmdRes.h). See pages 67-70 of the spec. In the tool
model given in this file, the coordinate system in which this volume
is defined has its origin at the center of the tip of the tool and its
positive Z axis along the axis of the stylus. The X axis direction is
a problem. We will assume here that each tool has an X axis marked on
it perpendicular to the axis of the stylus. To use a tool's
collision_volume for real collision detection, the location and
orientation of the tool in the current coordinate system must be known
and the collision_volume must be transformed to put it in the right
place. For constructing a pro-forma response to a
GetProp(Tool.CollisionVolume) command, the untransformed
collisionVolume can be used. The spec seems to intend that there must
be at least one Oriented Bounding Box in the collision volume of a
tool, although it does not explicitly prohibit having no boxes.

A tool has a radius, which is the radius of the ball at the end of the
stylus.

A tool has a length, which is the distance from the point on the tool
that is at the mount point (the point that is controlled when there
is no tool) to the center of the ball at the tip. This is a crude
geometric item that can be used for calculating a response to
GetChangeToolAction.

A tool has zero to three rotary joints that may be adjusted indirectly
(using an AlignTool command). The angles of these three joints are
represented by angleA, angleB, and angleC.  The number of actual
joints of a tool is not represented. If a tool does not have one of
the joints, the angle for that joint is always zero. Here, the angles
stay fixed (physically or logically) even when a tool is not being
used, so the angles are properties of the tool, not of the tool
changer.

It is not clear whether the spec intends that the angles of a tool
stay fixed between tool changes. If the same physical tool is two
logical tools, and they have different angles, motion will be required
when changing from one to the other. An alternative to having angles
persist between tool changes would be for the logical tool to inherit
its angles from the physical angles of the tool holder when it is
changed in, so that no motion would be required.

Currently, there is no geometric model of a tool, so the angles never
change.

A tool has a name and and id; both are strings. The spec says the id
will eventually identify the manufacturer and other information.

*/

class KTool
{
  friend class ToolChanger;

private:
  double angleA;
  double angleB;
  double angleC;
  OnePropertyCollisionVolume * collisionVolume;
  GoToPars * goToPar;
  char * id;
  bool isQualified;
  double length;
  char * name;
  PtMeasPars * ptMeasPar;
  double radius;
 
public:
  KTool(){}
  KTool(char * inName, char * inId, GoToPars * inGoToPar,
	PtMeasPars * inPtMeasPar, bool inIsQualified,
	OnePropertyCollisionVolume * inCollisionVolume,
	double inLength, double inRadius)
    {
      angleA = 0;
      angleB = 0;
      angleC = 0;
      collisionVolume = inCollisionVolume;
      goToPar = inGoToPar;
      id = inId;
      isQualified = inIsQualified;
      length = inLength;
      name = inName;
      ptMeasPar = inPtMeasPar;
      radius = inRadius;
    }
  ~KTool(){}
  double getAngleA() {return angleA;}
  double getAngleB() {return angleB;}
  double getAngleC() {return angleC;}
  OnePropertyCollisionVolume * getCollisionVolume() {return collisionVolume; }
  GoToPars * getGoToPar() {return goToPar;}
  char * getId() {return id;}
  bool getIsQualified() {return isQualified;}
  double getLength() {return length;}
  char * getName() {return name;}
  PtMeasPars * getPtMeasPar() {return ptMeasPar;}
  double getRadius() {return radius;}

  void setAngleA(double inAngleA)
    {angleA = inAngleA;}
  void setAngleB(double inAngleB)
    {angleB = inAngleB;}
  void setAngleC(double inAngleC)
    {angleC = inAngleC;}
  void setCollisionVolume(OnePropertyCollisionVolume * inCollisionVolume)
    {collisionVolume = inCollisionVolume;}
  void setGoToPar(GoToPars * inGoToPar)
    {goToPar = inGoToPar;}
  void setIsQualified(bool inIsQualified)
    {isQualified = inIsQualified;}
  void setLength(double inLength)
    {length = inLength;}
  void setName(char * inName)
    {name = inName;}
  void setPtMeasPar(PtMeasPars * inPtMeasPar)
    {ptMeasPar = inPtMeasPar;}
  void setRadius(double inRadius)
    {radius = inRadius;}
};

/* ToolChanger class

See pages 53-58 of the spec.

There is currently no data giving the location of a tool. It
may be useful to add this (either to ToolChanger or to KTool).

The NoTool tool is (1) active when there is definitely no tool
currently being held and (2) what FoundTool returns when the tool last
sought by FindTool was not found. The UnDefTool is active when the
system does not know whether or not a tool is being held.

It is not clear how the BaseTool (15-word description) and the RefTool
(5-word description) should be handled.  They are mentioned only on
pages 53 and 56 the spec. It is not clear how the BaseTool differs
from the NoTool.

The "tools" data member is modeled as a vector of pointers to KTool
because when it was modeled as a vector of KTool, adding tools had to
be done by push_back(*noTool) and similar statements.  But that makes
a copy of noTool and then adds a pointer to the copy. Then findTool
finds the copy and it is not equal to noTool, so equality tests
fail. C++ strikes again.

The checkIn TargetVector gives the location in machine coordinates
(assuming there is no tool) to which the machine will be moved (for an
automatic tool change) or must be moved (for a manual tool changer) in
order to change the tool. The default checkIn location is (0, 0, 0).
The checkIn location can be changed with setCheckIn. The checkIn can
be used to produce a response to a GetChangeToolAction command (see
pages 57-58). To be correct, the data in a GetChangeToolActionResponse
(1) must be transformed to the current coordinate system, and (2)
since the center of the ball is the position that is being controlled,
must be offset according to the tool's dimensions and angles.

The current tool has two alignment vectors of type TargetVector
(defined in IppCmdRes.h). These are maintained in machine coordinates
so that they do not change if the coordinate system changes. The
alignment vectors change when an AlignTool command is given (see page
70). The vectors must be transformed from the current coordinate
system to the machine coordinate system when the AlignTool command is
carried out and must be transformed from the machine coordinate system
to the current coordinate system when a response to
GetProp(Tool.Alignment()) command is made. Since only the current tool
has alignment vectors, they are properties of the ToolChanger, not of
KTool. The default alignment is (0, 0, 0).

*/

using namespace std;

class ToolChanger
{
private:
  KTool * activeTool;
  KTool * foundTool;
  KTool * baseTool;
  KTool * refTool;
  KTool * noTool;
  KTool * unDefTool;
  vector<KTool *> tools; /* all the tools the toolchanger knows about */
  vector<KTool *>::iterator p;
  TargetVector * checkIn;
  TargetVector * alignment1;
  TargetVector * alignment2;

public:
  ToolChanger ()
    {
      noTool = new KTool("NoTool", "NoToolId", new GoToPars,
			 new PtMeasPars, false, 
			 new OnePropertyCollisionVolume(0, NULL), 0, 0);
      baseTool = new KTool("BaseTool", "BaseToolId", new GoToPars,
			   new PtMeasPars, false, 
			   new OnePropertyCollisionVolume(0, NULL), 0, 0);
      refTool = new KTool("RefTool", "RefToolId", new GoToPars,
			  new PtMeasPars, false,
			  new OnePropertyCollisionVolume(0, NULL), 0, 0);
      unDefTool = new KTool("UnDefTool", "UnDefToolId", new GoToPars,
			    new PtMeasPars, false,
			    new OnePropertyCollisionVolume(0, NULL), 0, 0);
      activeTool = unDefTool;
      foundTool = noTool;
      tools.reserve(MAXTOOLS);
      tools.push_back(noTool);
      tools.push_back(unDefTool);
      tools.push_back(baseTool);
      tools.push_back(refTool);
      checkIn = new TargetVector(0, 0, 0);
      alignment1 = new TargetVector(0, 0, 1);
      alignment2 = new TargetVector(0, 0, 0);
    }
  ~ToolChanger (){}

  KTool * getActiveTool() {return activeTool;}
  TargetVector * getCheckIn() {return checkIn;}
  KTool * getFoundTool(){return foundTool;}
  KTool * getNoTool(){return noTool;}
  vector<KTool *> getToolList() {return tools;}
  KTool * getUnDefTool() {return unDefTool;}
  TargetVector * getAlignment1() {return alignment1;}
  TargetVector * getAlignment2() {return alignment2;}

  void setActiveTool(KTool * inTool) {if (inTool) activeTool = inTool;}
  void setCheckIn(TargetVector * inCheckIn) {checkIn = inCheckIn;}
  void setFoundTool(KTool * inTool) {if (inTool) foundTool = inTool;}

  void addTool(KTool * addIt){ tools.push_back(addIt); }

  KTool * removeTool(KTool * removeIt)
    {
      for (p = tools.begin(); p != tools.end(); p++)
	{
	  if ((*p) == removeIt)
	    {
	      tools.erase(p);
	      return removeIt;
	    }
	}
      return NULL;
    }

  KTool * findTool(char * name)
    {
      for (p = tools.begin(); p != tools.end(); p++)
	{
	  if (strcmp(name, (*p)->getName()) == 0)
	    return (*p);
	}
      return NULL;
    }
};
#endif /* #ifndef TOOLS_HH */
