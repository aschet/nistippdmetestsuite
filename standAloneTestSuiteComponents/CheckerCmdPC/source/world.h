/* world.h

This is a semi-dummy file. It defines data members and methods for
a world modeler that keeps track of the state of a system executing
I++ DME Interface functions. Main in parser.cc calls some of the world's
methods.

*/

#ifndef WORLD_HH
#define WORLD_HH

#include "IppCmdRes.h"
#include "tools.h"

using namespace std;

/*******************************************************************/

/*

The IPP_NORMAL_ERROR is the amount by which the length of a vector
that is supposed to be normalized may differ from 1.0. If the
difference is more than this amount, the vector should not be
considered to be normalized. With the current setting (0.0001),
numbers representing vector components will have to be accurate to at
least five decimal places. If only four decimal place accuracy is
used, rounding errors in computing vector length will frequently be
greater than the IPP_NORMAL_ERROR.

*/

#define IPP_NORMAL_ERROR 0.0001
#define IPP_PI 3.14159265358979323846

/* goness

It may be desirable to have more (add "unready" which would be the
setting when the system starts up) or fewer (combine "erred" and
"aborted", in which case a boolean would do) choices for goness.

*/

typedef enum goness {ready, moving, erred, aborted};

/*******************************************************************/

/* csyTransform

The parameters for SetCsyTransformation are X0, Y0, Z0, Theta, Psi,
and Phi. X0, Y0, and Z0 are described in the spec as the origin of the
machine coordinate system in "Part coordinates" (the coordinate system
whose tranformation is being assigned). The spec says Theta, Psi, and
Phi are Euler angles that define the rotation matrix of the
transformation. The spec does not say how Theta, Psi and Phi are
intended to be used in performing the rotation. The spec does provide
an error message saying that Theta is out of range, so the intended
method should have a limited range for Theta. The spec does not say what
the range is.

Appendix A to the spec gives C++ code for building a rotation
matrix from Theta, Psi, and Phi and for the inverse operation of
calculating Theta, Psi, and Phi given a rotation matrix. Appendix
A does not say how the matrix is intended to be used in performing
transformations.

The term "Euler angles" does not have a universally accepted meaning.
There is one widely accepted meaning that matches the spec,
so that is, presumably, what is intended. The gaps left by the spec
are filled in the following paragraphs.

The description below is the one given in the book Classical
Mechanics, by Herbert Goldstein, except that the roles of Psi and Phi
are switched. Goldstein notes that European authors often make this
switch. My copy of this book was printed in 1965, and the description
is on pages 107 and 108. A search of the web for Euler angles gets
many statements such as "numerous other definitions are in use" and
"there are as many different conventions as authors".

For the purposes of description, "PartCsy" is used here as the other
coordinate system in addition to MachineCsy.

The parameters X0, Y0, Z0, Theta, Psi, and Phi are used as follows to
move the MachineCsy so that it is coincident with the PartCsy.

First, translate the MachineCsy so that its origin ((X0, Y0, Z0) in
the part coordinates) is at (0, 0, 0) in part coordinates.
Call the new coordinate system MachineCsy1.

Second, rotate MachineCsy1 about the MachineCsy1 Z-axis so that the
MachineCsy1 X-axis is on the line of intersection of the MachineCsy1
XY-plane with the PartCsy XY-plane. Call the angle of rotation Psi. If
the two planes are the same, Psi = 0. Call the new coordinate
system MachineCsy2.

Third, rotate MachineCsy2 about the MachineCsy2 X-axis so that the
positive MachineCsy2 Z-axis is coincident with the positive PartCsy
Z-axis.  Call the angle of rotation Theta.  If the positive
MachineCsy2 Z-axis is already coincident with the positive PartCsy
Z-axis, Theta = 0.  Call the new coordinate system MachineCsy3.

[In the preceding step, it is not obvious that any rotation about the
MachineCsy2 X-axis will cause the two Z-axes to be coincident. To see
that this is possible, imagine a rotation about the MachineCsy2 X-axis
that puts the MachineCsy2 Y-axis in the PartCsy XY-plane. This same
rotation will cause the MachineCsy2 Z-axis to be coincident with the
PartCsy Z-axis, since the two coordinate systems now have the same
XY-plane and same origin.]

Fourth, rotate the MachineCsy3 about the MachineCsy3 Z-axis so that
the positive MachineCsy3 X-axis is coincident with the positive
PartCsy X-axis. Call the angle of rotation Phi. If the X-axes are
already coincident, Phi = 0.

With these meanings for the angles, the rotation matrix for each
individual rotation can be easily written. The rotation matrix for
the net effect of the three rotations is found by taking the product
of the three matrices. As stated on page 68 of the spec, using normal
matrix indexing (row index first, column index second, top to bottom
for rows, left to right for columns, indexes starting at 1)
this matrix is given by:

M11 = [(c2 * c3) - (c1 * s2 * s3)]
M12 = [(s2 * c3) + (c1 * c2 * s3)]
M13 = [(s1 * s3)]
M21 = [-(c2 * s3) - (c1 * s2 * c3)]
M22 = [-(s2 * s3) + (c1 * c2 * c3)]
M23 = [(s1 * c3)]
M31 = [(s1 * s2)]
M32 = [-(s1 * c2)]
M33 = [(c1)]

where:
c1 = cos(Theta)
s1 = sin(Theta)
c2 = cos(Psi)
s2 = sin(Psi)
c3 = cos(Phi)
s3 = sin(Phi)

If matrix M is put in a 2D C++ array (call it Marray), the indexes are
zero-based, so M11 = Marray[0,0], M32 = Marray[2,1] and so forth.

The transformation matrix M may be used as follows:

If P is a point (xm, ym, zm) given in machine coordinates, to find its
coordinates (xp, yp, zp) in part coordinates, first find its
coordinates (xmr, ymr, zmr) in rotated machine coordinates:

xmr = [(M11 * xm) + (M12 * ym) + (M13 * zm)]
ymr = [(M21 * xm) + (M22 * ym) + (M23 * zm)]
zmr = [(M31 * xm) + (M32 * ym) + (M33 * zm)]

Then translate:
xp = (xmr + X0)
yp = (ymr + Y0)
zp = (zmr + Z0)

If P is a point (xp, yp, zp) in part coordinates, to
find its coordinates (xm, ym, zm) in machine coordinates, first find its
coordinates (xpt, ypt, zpt) in translated part coordinates:

xpt = (xp - X0)
ypt = (yp - Y0)
zpt = (zp - Z0)

Then rotate:

xm = [(M11 * xpt) + (M21 * ypt) + (M31 * zpt)]
ym = [(M12 * xpt) + (M22 * ypt) + (M32 * zpt)]
zm = [(M13 * xpt) + (M23 * ypt) + (M33 * zpt)]

The rotation matrix used here is the transpose of the one used earlier.
This is also the inverse, since for rotation matrices, the transpose
is the inverse. The inverse of a rotation, of course, is an equally large
rotation about the same axis in the opposite direction.

Comment 1: Theta, Psi, and Phi correspond well with widely used
equipment, namely rotary tables (which rotate around an axis parallel
to the MachineCsy Z-axis) and tilt tables (which rotate around an axis
parallel to the MachineCsy XY-plane -- this rotation axis is often
also parallel to the X-axis).

For a rotary table, Theta is zero, and either Psi or Phi (or even both,
but that would be silly) can be used for the rotation angle.

For a tilt table, Psi and Phi are both zero, and Theta is used for
the rotation angle if the rotation axis is parallel to the X-axis.
If the rotation angle is not parallel to the X-axis, Psi can be used
before Theta, since the effect is the same as a tilt table mounted
on a rotary table.

For a tilt table mounted on a rotary table with the rotary axis passing
through the tilt axis, Psi and Theta, or Theta and Phi can be used.

For a rotary table mounted on a tilt table with the rotary axis passing
through the tilt axis, Theta and Phi can be used.

Comment 2: The range of Theta, Psi and Phi is a full 360 degrees,
which might reasonably be either 0 to 360 or -180 to +180. It is not
clear why Theta has been singled out for possibly being out of
range. There does not need to be any limitation of range for any of
the angles, since every rotation outside any 360 degree range has the
same effect as some rotation inside the range.


Comment 3: The spec is not clear about which transformation
is being set by SetCsyTransformation. There are two choices:
1. The transformation that transforms from machine coordinates to
   PartCsy, or JogDisplayCsy, or whateverCsy.
2. The transformation that transforms from the previous coordinate
   system in a coordinate system chain starting from the MachineCsy.

The diagram on page 59 of release 1.4 implies choice 2 is what is
intended, since the PartCsy transformation is based on the Rotary
Table coordinate system. In this case, X0, Y0, Z0 would be the
location of the origin of the rotary table coordinate system in part
coordinates. But the text in the section about SetCsyTransformation on
page 60 implies choice 1 is intended, since it says "X0, Y0, and Z0
define the zero point of the machine coordinate system in part
coordinates."

We are assuming here that choice 1 is intended.

Comment 4: An alternative parameterization for SetCsyTransformation
would be to use nine parameters (X0, Y0, Z0, Zx, Zy, Zz, Xx, Xy, Xz)
where the Euler angles have been replaced by (Zx, Zy, Zz) [which is
the direction in the PartCsy of the vector (0, 0, 1) in the
MachineCsy] and (Xx, Xy, Xz) [which is the direction in the PartCsy of
(1, 0, 0) in the MachineCsy].  The values of (Yx, Yy, Yz) can be
computed easily (since the transformed Y axis is the cross product of
the transformed Z axis with the transformed X axis) or they can be
provided as additional parameters. The rotation matrix then has (Xx,
Xy, Xz), (Yx, Yy, Yz), and (Zx, Zy, Zz) as its rows (or columns,
depending on how the matrix is used). This parameterization is the one
used in STEP geometry.

In the implementation below, two sets of data about the transformation
are included, namely both the data provided in the SetCsyTransformation
command and the Matrix that may be derived from that data.

The first constructor makes an identity transformation.

In the method names below, "fwd" (forward) means from machine
coordinates to part (or whatever) coordinates, and "inv" (inverse)
means from part (or whatever) coordinates to machine coordinates.

The equations for transforming the coordinates of points are given
above. The equations for transforming directions are simpler. The
translation can be ignored. This is obvious in the forward direction,
since the translation occurs after the rotation and has no effect on
directions. We can also show this mathematically. Mathematically, a
direction may be viewed as a vector from the origin O of a coordinate
system to a point P having the same coordinates as the direction.

For transforming forward, we can compute the direction in the
new system as the difference between the transformed P and the
transformed O. For the X coordinate, for example,

newDirectionX = X_of(P_TransformedForward) - X_of(OtransformedForward)

              =    ((M11 * Px) + (M12 * Py) + (M13 * Pz) + x0)
                 - ((M11 * 0)  + (M12 * 0)  + (M13 * 0)  + x0)

              =    ((M11 * Px) + (M12 * Py) + (M13 * Pz))

For transforming backward (inverse), we can compute the direction
in machine coordinates as the same difference, but now P and O
are expressed in the new coordinate system.

machineDirectionX = X_of(P_TransformedInverse) - X_of(O_TransformedInverse)

          =   ((M11 * (Px - x0)) + (M21 * (Py - y0)) + (M31 * (Pz - z0)))
            - ((M11 * ( 0 - x0)) + (M21 * ( 0 - y0)) + (M31 * ( 0 - z0)))

          =   ((M11 * Px) + (M21 * Py) + (M31 * Pz))

The calculations for Y and Z are similar. Thus, for directions, we have
proved we can just ignore the translation for both forward and inverse
applications of the csyTransform.

*/

class csyTransform
{
private:
  double x0;
  double y0;
  double z0;
  double theta;
  double psi;
  double phi;
  double M11;
  double M12;
  double M13;
  double M21;
  double M22;
  double M23;
  double M31;
  double M32;
  double M33;

public:
  csyTransform(){x0 = 0; y0 = 0; z0 = 0; theta = 0; psi = 0; phi = 0;
                 M11 = 1; M12 = 0; M13 = 0;
		 M21 = 0; M22 = 1; M23 = 0;
		 M31 = 0; M32 = 0; M33 = 1;}
  ~csyTransform(){};
  void update(double x0In, double y0In, double z0In,
          double thetaIn, double psiIn, double phiIn);

  double fwdPointX(double xIn, double yIn, double zIn)
    {return ((M11 * xIn) + (M12 * yIn) + (M13 * zIn) + x0);}
  double fwdPointY(double xIn, double yIn, double zIn)
    {return ((M21 * xIn) + (M22 * yIn) + (M23 * zIn) + y0);}
  double fwdPointZ(double xIn, double yIn, double zIn)
    {return ((M31 * xIn) + (M32 * yIn) + (M33 * zIn) + z0);}
  double invPointX(double xIn, double yIn, double zIn)
    {return ((M11 * (xIn - x0)) + (M21 * (yIn - y0)) + (M31 * (zIn - z0)));}
  double invPointY(double xIn, double yIn, double zIn)
    {return ((M12 * (xIn - x0)) + (M22 * (yIn - y0)) + (M32 * (zIn - z0)));}
  double invPointZ(double xIn, double yIn, double zIn)
    {return ((M13 * (xIn - x0)) + (M23 * (yIn - y0)) + (M33 * (zIn - z0)));}

  double fwdDirectionX(double xIn, double yIn, double zIn)
    {return ((M11 * xIn) + (M12 * yIn) + (M13 * zIn));}
  double fwdDirectionY(double xIn, double yIn, double zIn)
    {return ((M21 * xIn) + (M22 * yIn) + (M23 * zIn));}
  double fwdDirectionZ(double xIn, double yIn, double zIn)
    {return ((M31 * xIn) + (M32 * yIn) + (M33 * zIn));}
  double invDirectionX(double xIn, double yIn, double zIn)
    {return ((M11 * xIn) + (M21 * yIn) + (M31 * zIn));}
  double invDirectionY(double xIn, double yIn, double zIn)
    {return ((M12 * xIn) + (M22 * yIn) + (M32 * zIn));}
  double invDirectionZ(double xIn, double yIn, double zIn)
    {return ((M13 * xIn) + (M23 * yIn) + (M33 * zIn));}


  double getX() {return x0;}
  double getY() {return y0;}
  double getZ() {return z0;}
  double getTheta() {return theta;}
  double getPsi() {return psi;}
  double getPhi() {return phi;}

};

/*******************************************************************/

/* world

The onMoveTagNumber is:
1. 0 at the beginning of a session
2. 0 if StopAllDaemons has been executed since the last OnMoveReportE
   command was executed.
3. 0 if StopDaemons has been executed using the tag of the last
   OnMoveReportE command that was executed.
4. 0 if the last OnMoveReportE command that was executed had no
   arguments.
5. The tag number of the last OnMoveReportE command with arguments
   that was executed successfully, otherwise.

The onScanTagNumber follows similar rules with regard to the last
OnScanReport command.

*/

class world
{
private:
  coordSysType Csy; /* current Csy */
  csyTransform csyJogDisplay;
  csyTransform csyJogMove;
  csyTransform csyMoveable;
  csyTransform csyMultiple;
  csyTransform csyPart;
  csyTransform csySensor;
  double currentR;    /* current rotation angle of rotary table             */
  double currentX;    /* X coord in current Csy of controlled point         */
  double currentY;    /* Y coord in current Csy of controlled point         */
  double currentZ;    /* Z coord in current Csy of controlled point         */
  double homeX;       /* X coord in machineCsy of home point                */
  double homeY;       /* Y coord in machineCsy of home point                */
  double homeZ;       /* Z coord in machineCsy of home point                */
  bool inSession;     /* true if session in progress, else false            */
  bool isHomed;       /* true if homed, else false                          */
  double limitLowerX; /* minus-X limit in machineCsy of work volume         */
  double limitLowerY; /* minus-Y limit in machineCsy of work volume         */
  double limitLowerZ; /* minus-Z limit in machineCsy of work volume         */
  double limitUpperX; /* plus-X  limit in machineCsy of work volume         */
  double limitUpperY; /* plus-Y  limit in machineCsy of work volume         */
  double limitUpperZ; /* plus-Z  limit in machineCsy of work volume         */
  bool lockedC;       /* true if C axis is locked, else false               */
  bool lockedPhi;     /* true if Phi axis is locked, else false             */
  bool lockedR;       /* true if R axis is locked, else false               */
  bool lockedX;       /* true if X axis is locked, else false               */
  bool lockedY;       /* true if Y axis is locked, else false               */
  bool lockedZ;       /* true if Z axis is locked, else false               */
  char machineClass[24]; /* string for name of machine class                */
  double measuredX1;  /* X coord in current Csy of 1st recently measured pt */
  double measuredY1;  /* Y coord in current Csy of 1st recently measured pt */
  double measuredZ1;  /* Z coord in current Csy of 1st recently measured pt */
  double measuredX2;  /* X coord in current Csy of 2nd recently measured pt */
  double measuredY2;  /* Y coord in current Csy of 2nd recently measured pt */
  double measuredZ2;  /* Z coord in current Csy of 2nd recently measured pt */
  double measuredX3;  /* X coord in current Csy of 3rd recently measured pt */
  double measuredY3;  /* Y coord in current Csy of 3rd recently measured pt */
  double measuredZ3;  /* Z coord in current Csy of 3rd recently measured pt */
  double onMoveDis;
  bool onMoveHasDis;
  bool onMoveHasR;
  bool onMoveHasTime;
  bool onMoveHasToolA;
  bool onMoveHasToolB;
  bool onMoveHasToolC;
  bool onMoveHasX;
  bool onMoveHasY;
  bool onMoveHasZ;
  unsigned int onMoveTagNumber; /* 0 if no daemon, else tag number */
  double onMoveTime;
  bool onPtmeasHasER;
  bool onPtmeasHasIJK;
  bool onPtmeasHasR;
  bool onPtmeasHasToolA;
  bool onPtmeasHasToolB;
  bool onPtmeasHasToolC;
  bool onPtmeasHasX;
  bool onPtmeasHasY;
  bool onPtmeasHasZ;
  bool onScanHasER;
  bool onScanHasIJK;
  bool onScanHasQ;
  bool onScanHasR;
  bool onScanHasToolA;
  bool onScanHasToolB;
  bool onScanHasToolC;
  bool onScanHasX;
  bool onScanHasY;
  bool onScanHasZ;
  bool outOfRangeFlag;
  unsigned int pointTag; /* tag of PtMeas or Scan for measuredX, Y, Z */
  goness readiness;
  ToolChanger tools;
  bool userEnabled;

  void updateAlignTool(AlignToolCommand * theAlignTool);
  void updateChangeTool(ChangeToolCommand * theChangeTool);
  void updateEndSession();
  void updateHome();
  void updateLockAxis(LockAxisCommand * theLockAxis);
  void updateOnMoveReportE(OnMoveReportECommand * theOnMove);
  void updateOnPtMeasReport(OnPtMeasReportCommand * theOnPtMeas);
  void updateOnScanReport(OnScanReportCommand * theOnScan);
  void updatePtMeas(PtMeasCommand * thePtMeas);
  void updateScanInCylEndIsPlane(ScanInCylEndIsPlaneCommand * theScan);
  void updateScanInCylEndIsSphere(ScanInCylEndIsSphereCommand * theScan);
  void updateScanInPlaneEndIsCyl(ScanInPlaneEndIsCylCommand * theScan);
  void updateScanInPlaneEndIsPlane(ScanInPlaneEndIsPlaneCommand * theScan);
  void updateScanInPlaneEndIsSphere(ScanInPlaneEndIsSphereCommand * theScan);
  void updateScanOnCircle(ScanOnCircleCommand * theScan);
  void updateScanOnLine(ScanOnLineCommand * theScan);
  void updateSetCoordSystem(SetCoordSystemCommand * theSet);
  void updateSetCsyTransformation(SetCsyTransformationCommand * theSet);
  void updateSetProp(SetPropCommand * theSet);
  void updateStartSession();

public:
  world();
  ~world(){}

  void setInSession(bool inInSession) { inSession = inInSession; }
  void setIsHomed(bool inIsHomed) { isHomed = inIsHomed; }
  void setReadiness(goness inReadiness){ readiness = inReadiness; }

  OnePropertyAlignment * getCurrentAlignment();
  coordSysType   getCurrentCsy()            { return Csy;               }
  double         getCurrentR()              { return currentR;          }
  double         getCurrentToolA()
                            {return tools.getActiveTool()->getAngleA();}
  double         getCurrentToolB()
                            {return tools.getActiveTool()->getAngleB();}
  double         getCurrentToolC()
                            {return tools.getActiveTool()->getAngleC();}
  double         getCurrentX()              { return currentX;          }
  double         getCurrentY()              { return currentY;          }
  double         getCurrentZ()              { return currentZ;          }
  bool           getInSession()             { return inSession;         }
  bool           getIsHomed()               { return isHomed;           }
  double         getLimitLowerX()           { return limitLowerX;       }
  double         getLimitLowerY()           { return limitLowerY;       }
  double         getLimitLowerZ()           { return limitLowerZ;       }
  double         getLimitUpperX()           { return limitUpperX;       }
  double         getLimitUpperY()           { return limitUpperY;       }
  double         getLimitUpperZ()           { return limitUpperZ;       }
  csyTransform * getJogDisplayCsyTransform(){ return &csyJogDisplay;    }
  csyTransform * getJogMoveCsyTransform()   { return &csyJogMove;       }
  char *	 getMachineClass()          { return machineClass;      }
  double         getMeasuredX1()            { return measuredX1;        }
  double         getMeasuredY1()            { return measuredY1;        }
  double         getMeasuredZ1()            { return measuredZ1;        }
  double         getMeasuredX2()            { return measuredX2;        }
  double         getMeasuredY2()            { return measuredY2;        }
  double         getMeasuredZ2()            { return measuredZ2;        }
  double         getMeasuredX3()            { return measuredX3;        }
  double         getMeasuredY3()            { return measuredY3;        }
  double         getMeasuredZ3()            { return measuredZ3;        }
  csyTransform * getMoveableCsyTransform()  { return &csyMoveable;      }
  csyTransform * getMultipleCsyTransform()  { return &csyMultiple;      }
  double         getOnMoveDis()             { return onMoveDis;         }
  bool           getOnMoveHasDis()          { return onMoveHasDis;      }
  bool           getOnMoveHasR()            { return onMoveHasR;        }
  bool           getOnMoveHasTime()         { return onMoveHasTime;     }
  bool           getOnMoveHasToolA()        { return onMoveHasToolA;    }
  bool           getOnMoveHasToolB()        { return onMoveHasToolB;    }
  bool           getOnMoveHasToolC()        { return onMoveHasToolC;    }
  bool           getOnMoveHasX()            { return onMoveHasX;        }
  bool           getOnMoveHasY()            { return onMoveHasY;        }
  bool           getOnMoveHasZ()            { return onMoveHasZ;        }
  unsigned int   getOnMoveTag()             { return onMoveTagNumber;   }
  double         getOnMoveTime()            { return onMoveTime;        }
  bool           getOnPtmeasHasER()         { return onPtmeasHasER;     }
  bool           getOnPtmeasHasIJK()        { return onPtmeasHasIJK;    }
  bool           getOnPtmeasHasR()          { return onPtmeasHasR;      }
  bool           getOnPtmeasHasToolA()      { return onPtmeasHasToolA;  }
  bool           getOnPtmeasHasToolB()      { return onPtmeasHasToolB;  }
  bool           getOnPtmeasHasToolC()      { return onPtmeasHasToolC;  }
  bool           getOnPtmeasHasX()          { return onPtmeasHasX;      }
  bool           getOnPtmeasHasY()          { return onPtmeasHasY;      }
  bool           getOnPtmeasHasZ()          { return onPtmeasHasZ;      }
  bool           getOnScanHasER()           { return onScanHasER;       }
  bool           getOnScanHasIJK()          { return onScanHasIJK;      }
  bool           getOnScanHasQ()            { return onScanHasQ;        }
  bool           getOnScanHasR()            { return onScanHasR;        }
  bool           getOnScanHasToolA()        { return onScanHasToolA;    }
  bool           getOnScanHasToolB()        { return onScanHasToolB;    }
  bool           getOnScanHasToolC()        { return onScanHasToolC;    }
  bool           getOnScanHasX()            { return onScanHasX;        }
  bool           getOnScanHasY()            { return onScanHasY;        }
  bool           getOnScanHasZ()            { return onScanHasZ;        }
  bool           getOutOfRangeFlag()        { return outOfRangeFlag;    }
  csyTransform * getPartCsyTransform()      { return &csyPart;          }
  double         getProbeRadius()
                            {return tools.getActiveTool()->getRadius(); }
  goness         getReadiness()             { return readiness;         }
  csyTransform * getSensorCsyTransform()    { return &csySensor;        }
  ToolChanger *  getTools()                 { return &tools;            }
  bool           getUserEnabled()           { return userEnabled;       }

  bool daemonExists(int tagNumber);
  bool inWorkVolume(double x, double y, double z);
  void killDaemons(){ onMoveTagNumber = 0; }
  void putCurrentXYZ(double x, double y, double z)
    {
      currentX = x;
      currentY = y;
      currentZ = z;
    }
  void setMeasuredX1(double inX1){ measuredX1 = inX1; }
  void setMeasuredY1(double inY1){ measuredY1 = inY1; }
  void setMeasuredZ1(double inZ1){ measuredZ1 = inZ1; }
  void setMeasuredX2(double inX2){ measuredX2 = inX2; }
  void setMeasuredY2(double inY2){ measuredY2 = inY2; }
  void setMeasuredZ2(double inZ2){ measuredZ2 = inZ2; }
  void setMeasuredX3(double inX3){ measuredX3 = inX3; }
  void setMeasuredY3(double inY3){ measuredY3 = inY3; }
  void setMeasuredZ3(double inZ3){ measuredZ3 = inZ3; }
  void update(Command * aCommand);
};

#endif /* #ifndef WORLD_HH */
