
#ifndef HELIX_CPP
#define HELIX_CPP

#include "../Server/StdAfx.h"

/*


In this file, points are denoted by two capital letters, the first of
which is P, and vectors are denoted by two capital letters, the first
of which is V. The first four points and one vectors below are shown
in the figure on page 79 of the I++ spec Release 1.30. The others are
all additions to the figure.


PC = [Cx, Cy, Cz]
PD = [Dx, Dy, Dz]
PE = [Ex, Ey, Ez]
PS = [Sx, Sy, Sz]
VC = [Ci, Cj, Ck]


Let VR = [Rx, Ry, Rz] be the radius vector from PC to PS.
Let VH = [Hx, Hy, Hz] be the vector from PS to PD.
Let VK = [Kx, Ky, Kz] be the cross product (VR x VH) of VR and VH.


*/


/**********************************************************************/


#include <stdio.h>
#include <math.h>
#define M_PI 3.141593

class Helix
{
	int IsHelixCounterclockwise(double,double,double,double,double,double,double,double,
		double,double,double,double);

	int findHelixParameters(double,double,double,double,double,double,double,double,
		double,double,double,double,double,double,double,double*,double*);
};

/**********************************************************************/


/* IsCounterclockwise


Point PD is used (only) to determine which way to go around the cylinder.
The assumption is that the way to go around the cylinder is whichever
way is shortest to go from from PS to PD, since PD is intended to be
close to PS.


The idea of this function is to find the cross product VK of VR and VH
and then take the dot product of VK with the axis vector. If the dot
product is positive, go counterclockwise. If the dot product is negative,
go clockwise. The dot product should never be zero.


This function returns 1 if the helix goes counterclockwise and zero
otherwise.


*/


int Helix::IsHelixCounterclockwise(
 double Cx,
 double Cy,
 double Cz,
 double Dx,
 double Dy,
 double Dz,
 double Sx,
 double Sy,
 double Sz,
 double Ci,
 double Cj,
 double Ck)
{
  double Rx, Ry, Rz;
  double Hx, Hy, Hz;
  double Kx, Ky, Kz;


  Rx = (Sx - Cx);
  Ry = (Sy - Cy);
  Rz = (Sz - Cz);
  Hx = (Dx - Cx);
  Hy = (Dy - Cy);
  Hz = (Dz - Cz);
  Kx = ((Ry * Hz) - (Hy * Rz));
  Ky = ((Rz * Hx) - (Hz * Rx));
  Kz = ((Rx * Hy) - (Hx * Ry));
  return (((Kx * Ci) + (Ky * Cj) + (Kz * Ck)) > 0);
}


/**********************************************************************/


/* findHelixParameters


Let VF be the vector from PC to PE.


Let PQ be the point on the circle directly below PE. E must be on
the cylinder for this point to exist.


Let PJ be the point at which a perpendicular from PE hits the axis
of the cylinder.


Let VM be the vector from PC to PJ. Note that it is multiple of VC


Let VL be the vector from PC to PQ.


Let beta be the angle from VR to VL.


Let d be the (signed!) distance from PQ to PE. This is the same as
the distance from PC to PJ.


Let r be the radius of the circle.


Let beta be the angle between VR and VL.


This function finds beta and d and stores them in the arguments
with those names. The returned value of the function is not
currently good for anything, but if error checks were added, it
could be used to indicate error status.


Since PJ is the projection of VF on the axis:
d = VF dot VC


Since PQ is directly below E,


PQ = (PE - (d * VC))


Then beta must satisfy the equation:
cos(beta) = ((VR dot VL) / (r * r))


But there are four solutions to this that might be correct, since beta
may be positive or negative and its absolute value may be greater or
less than 180 degrees. To determine positive or negative, the
IsCounterclockwise function is used. To determine greater or less than
180 degrees, the cross product VG = (VR x VL) is computed and then the
dot product of that with VC is taken.
For counterclockwise rotation, if the dot product is negative,
the angle is greater than 180 degrees.
For clockwise rotation, if the dot product is positive,
the angle is greater than 180 degrees.


*/


 int Helix::findHelixParameters(
 double Cx,
 double Cy,
 double Cz,
 double Dx,
 double Dy,
 double Dz,
 double Ex,
 double Ey,
 double Ez,
 double Sx,
 double Sy,
 double Sz,
 double Ci,
 double Cj,
 double Ck,
 double * beta,
 double * d)
{
  double Fx, Fy, Fz;
  double Gx, Gy, Gz;
  double Lx, Ly, Lz;
  double Qx, Qy, Qz;
  double Rx, Ry, Rz;
  double alpha;
  double r;


  Fx = (Ex - Cx);
  Fy = (Ey - Cy);
  Fz = (Ez - Cz);
  Rx = (Sx - Cx);
  Ry = (Sy - Cy);
  Rz = (Sz - Cz);
  *d = ((Fx * Ci) + (Fy * Cj) + (Fz * Ck));
  Qx = (Ex - (*d * Ci));
  Qy = (Ey - (*d * Cj));
  Qz = (Ez - (*d * Ck));
  r  = sqrt((Rx * Rx) + (Ry * Ry) + (Rz * Rz));
  Lx = (Qx - Cx);
  Ly = (Qy - Cy);
  Lz = (Qz - Cz);
  alpha = acos(((Rx * Lx) + (Ry * Ly) + (Rz * Lz)) / (r * r));
  Gx = ((Ry * Lz) - (Ly * Rz));
  Gy = ((Rz * Lx) - (Lz * Rx));
  Gz = ((Rx * Ly) - (Lx * Ry));
  if (IsHelixCounterclockwise(Cx, Cy, Cz, Dx, Dy, Dz, Sx, Sy, Sz, Ci, Cj, Ck))
    {
      if (((Gx * Ci) + (Gy * Cj) + (Gz * Ck)) < 0)
        *beta = ((2 * M_PI) - alpha);
      else
        *beta = alpha;
    }
  else
    {
      if (((Gx * Ci) + (Gy * Cj) + (Gz * Ck)) > 0)
        *beta = -((2 * M_PI) - alpha);
      else
        *beta = -alpha;
    }
  return 0;
}


/**********************************************************************/


/*


WARNING - no function here, just equations that should be put into a
function.


Assuming the probe starts along the helix from PS at time t=0,
we can find a point PU on the helix at time t as follows.


Using the pythagorean theorem on the unwrapped helix, we can find
the length L of the path from PS to PE:
L = sqrt((d * d) + (l * l))
where l is the length of the arc around the circle from PS to PQ.


But l is just the angle times the radius:
l = (beta * r)


Let the speed along the path be S. Then the total time T from PS to PE
is the length L divided by the speed.
T = (L / S)


Let PW be the point on the circle directly below PU.
Let theta be the angle between VR and [the vector from PC to PW].
Let f be the distance from PW to PU.


At any time t,


f = (d * (t / T))
theta = (beta * (t / T))


Now that theta is known, point PW can be found using the circle method
previously programmed. Then PU can be found by:


PU = (PW + (b * VC))  <-- vector equation


*/


/**********************************************************************/


/*


This main is only for testing the two functions.


Sample data lines:
Cx Cy Cz Dx Dy Dz Ex Ey Ez Sx Sy Sz Ci Cj Ck
 0  0  0  2  1  0  0  2  7  2  0  0  0  0  1
 0  0  0  2 -1  0  0  2  7  2  0  0  0  0  1
 0  0  0 -2 -1  0  0  2  7 -2  0  0  0  0  1
 0  0  0 -2  1  0  0  2  7 -2  0  0  0  0  1


*/

/*
main()
{
  double Cx, Cy, Cz;
  double Dx, Dy, Dz;
  double Ex, Ey, Ez;
  double Sx, Sy, Sz;
  double Ci, Cj, Ck;
  double d;
  double beta;


  for(;;)
    {
      printf("enter Cx Cy Cz Dx Dy Dz Ex Ey Ez Sx Sy Sz Ci Cj Ck\n");
      scanf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
            &Cx, &Cy, &Cz, &Dx, &Dy, &Dz, &Ex, &Ey, &Ez,
            &Sx, &Sy, &Sz, &Ci, &Cj, &Ck);
      printf("IsCounterclockwise returns %d\n",
             IsCounterclockwise(Cx, Cy, Cz, Dx, Dy, Dz,
                                Sx, Sy, Sz, Ci, Cj, Ck));
      findHelixParameters(Cx, Cy, Cz, Dx, Dy, Dz, Ex, Ey, Ez, Sx, Sy, Sz,
                          Ci, Cj, Ck, &beta, &d);
      printf("beta = %lf radians,  d = %lf\n", beta, d);
    }
  return 0;
}

*/
/**********************************************************************/
#endif 
