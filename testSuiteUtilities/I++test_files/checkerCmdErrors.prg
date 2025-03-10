00001 StartSession()
\\
00001 ChangeTool("START NOT HOMED ERRORS")
\\
09990 ClearAllErrors()
\\
00201 AlignPart(0, 0, 1, 0, 0, 1, 30)
\\
09990 ClearAllErrors()
\\
00303 AlignTool(0, 0, 1, 30)
\\
09990 ClearAllErrors()
\\
00401 CenterPart(0, 0, 0, 1)
\\
09990 ClearAllErrors()
\\
02501 GoTo(X(0))
\\
09990 ClearAllErrors()
\\
03301 PtMeas(X(0))
\\
09990 ClearAllErrors()
\\
03401 ReQualify()
\\
09990 ClearAllErrors()
\\
03501 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03601 ScanInCylEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03701 ScanInPlaneEndIsCyl(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03801 ScanInPlaneEndIsPlane(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03901 ScanInPlaneEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
04001 ScanOnCircle(1,2,3,4,5,6,7,8,9,10,1,2)
\\
09990 ClearAllErrors()
\\
04201 ScanOnLine(1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
00001 ChangeTool("START UNDEFTOOL ERRORS")
\\
09990 ClearAllErrors()
\\
00001 Home()
\\
01001 EnumAllProp(Tool.PtMeasPar())
\\
09990 ClearAllErrors()
\\
01002 EnumAllProp(Tool.GoToPar())
\\
09990 ClearAllErrors()
\\
01101 EnumProp(Tool.PtMeasPar())
\\
09990 ClearAllErrors()
\\
01102 EnumProp(Tool.GoToPar())
\\
09990 ClearAllErrors()
\\
02201 GetProp(Tool.PtMeasPar.Accel())
\\
09990 ClearAllErrors()
\\
E2301 GetPropE(Tool.PtMeasPar.Accel())
\\
09990 ClearAllErrors()
\\
02502 GoTo(X(0))
\\
09990 ClearAllErrors()
\\
03304 PtMeas(X(0))
\\
09990 ClearAllErrors()
\\
03402 ReQualify()
\\
09990 ClearAllErrors()
\\
03502 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03602 ScanInCylEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03702 ScanInPlaneEndIsCyl(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03802 ScanInPlaneEndIsPlane(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03902 ScanInPlaneEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
04002 ScanOnCircle(1,2,3,4,5,6,7,8,9,10,1,2)
\\
09990 ClearAllErrors()
\\
04202 ScanOnLine(1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
04701 SetProp(Tool.PtMeasPar.Accel(54))
\\
09990 ClearAllErrors()
\\
00001 ChangeTool("START NOTOOL ERRORS")
\\
09990 ClearAllErrors()
\\
00001 ChangeTool("NoTool")
\\
01003 EnumAllProp(Tool.PtMeasPar())
\\
09990 ClearAllErrors()
\\
01103 EnumProp(Tool.PtMeasPar())
\\
09990 ClearAllErrors()
\\
02202 GetProp(Tool.PtMeasPar.Accel())
\\
09990 ClearAllErrors()
\\
E2302 GetPropE(Tool.PtMeasPar.Accel())
\\
09990 ClearAllErrors()
\\
03305 PtMeas(X(0))
\\
09990 ClearAllErrors()
\\
03403 ReQualify()
\\
09990 ClearAllErrors()
\\
03503 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03603 ScanInCylEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03703 ScanInPlaneEndIsCyl(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03803 ScanInPlaneEndIsPlane(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03903 ScanInPlaneEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
04003 ScanOnCircle(1,2,3,4,5,6,7,8,9,10,1,2)
\\
09990 ClearAllErrors()
\\
04203 ScanOnLine(1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
04702 SetProp(Tool.PtMeasPar.Accel(54))
\\
09990 ClearAllErrors()
\\
00001 ChangeTool("START ALL OTHER ERRORS")
\\
09990 ClearAllErrors()
\\
00001 ChangeTool("BaseTool")
\\
00301 AlignTool(0, 0, 2, 30)
\\
09990 ClearAllErrors()
\\
00302 AlignTool(0, 0, 1, 0, 0, 2, 30, 30)
\\
09990 ClearAllErrors()
\\
00501 ChangeTool("UnDefTool")
\\
09990 ClearAllErrors()
\\
00502 ChangeTool("Bleep")
\\
09990 ClearAllErrors()
\\
01501 GetChangeToolAction("UnDefTool")
\\
09990 ClearAllErrors()
\\
01502 GetChangeToolAction("Bleep")
\\
09990 ClearAllErrors()
\\
01901 GetErrorInfo(0013)
\\
09990 ClearAllErrors()
\\
02502 GoTo(X(123456789))
\\
09990 ClearAllErrors()
\\
E3001 OnMoveReportE(Time(0.09))
\\
09990 ClearAllErrors()
\\
E3002 OnMoveReportE(Dis(-3), X())
\\
09990 ClearAllErrors()
\\
E0001 OnMoveReportE(Dis(3), X())
\\
E3003 OnMoveReportE(Dis(3), X())
\\
09990 ClearAllErrors()
\\
03002 PtMeas(X(0), IJK(0,0,0))
\\
09990 ClearAllErrors()
\\
03303 PtMeas(X(123456789))
\\
09990 ClearAllErrors()
\\
03504 ScanInCylEndIsPlane(1,2,3,0,0,0,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03505 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,0,0,3,4,5,6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03506 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,0,0,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03507 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,0,0,0)
\\
09990 ClearAllErrors()
\\
03508 ScanInCylEndIsPlane(1,2,3,4,5,6,123456789,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03509 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,123456789,4,5,6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03510 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,7,8,9,6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03511 ScanInCylEndIsPlane(1,2,3,4,5,6,1,2,3,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03604 ScanInCylEndIsSphere(1,2,3,0,0,0,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03605 ScanInCylEndIsSphere(1,2,3,4,5,6,7,8,9,0,0,0,3,4,5,6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03606 ScanInCylEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,0,0,0)
\\
09990 ClearAllErrors()
\\
03607 ScanInCylEndIsSphere(1,2,3,4,5,6,123456789,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03608 ScanInCylEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,123456789,4,5,6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03609 ScanInCylEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,7,8,9,6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03610 ScanInCylEndIsSphere(1,2,3,4,5,6,1,2,3,10,1,2,3,4,5,6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03704 ScanInPlaneEndIsCyl(1,2,3,4,5,6,7,8,9,10,1,2,3,0,0,0,7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03705 ScanInPlaneEndIsCyl(1,2,3,0,0,0,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03706 ScanInPlaneEndIsCyl(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,0,0,0)
\\
09990 ClearAllErrors()
\\
03707 ScanInPlaneEndIsCyl(123456789,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03708 ScanInPlaneEndIsCyl(1,2,3,4,5,6,123456789,8,9,10,1,2,3,4,5,6,7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03709 ScanInPlaneEndIsCyl(1,2,3,4,5,6,1,2,3,10,1,2,3,4,5,6,7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03804 ScanInPlaneEndIsPlane(1,2,3,0,0,0,7,8,9,10,1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03805 ScanInPlaneEndIsPlane(1,2,3,4,5,6,7,8,9,10,1,2,3,0,0,0,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03806 ScanInPlaneEndIsPlane(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,0,0,0)
\\
09990 ClearAllErrors()
\\
03807 ScanInPlaneEndIsPlane(123456789,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03808 ScanInPlaneEndIsPlane(1,2,3,4,5,6,123456789,8,9,10,1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03809 ScanInPlaneEndIsPlane(1,2,3,4,5,6,1,2,3,10,1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03904 ScanInPlaneEndIsSphere(1,2,3,0,0,0,7,8,9,10,1,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
03905 ScanInPlaneEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,0,0,0)
\\
09990 ClearAllErrors()
\\
03906 ScanInPlaneEndIsSphere(123456789,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
03907 ScanInPlaneEndIsSphere(1,2,3,4,5,6,123456789,8,9,10,1,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
03908 ScanInPlaneEndIsSphere(1,2,3,4,5,6,7,8,9,10,123456789,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
03909 ScanInPlaneEndIsSphere(1,2,3,4,5,6,1,2,3,10,1,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
04004 ScanOnCircle(1,2,3,4,5,6,0,0,0,10,1,2)
\\
09990 ClearAllErrors()
\\
04005 ScanOnCircle(1,2,3,1,2,3,7,8,9,10,1,2)
\\
09990 ClearAllErrors()
\\
04006 ScanOnCircle(1,2,3,4,5,6,7,8,9,10,1,2)
\\
09990 ClearAllErrors()
\\
04007 ScanOnCircle(1,2,3,123456789,5,6,0,1,-1,10,1,2)
\\
09990 ClearAllErrors()
\\
04204 ScanOnLine(1,2,3,1,2,3,7,8,9,10)
\\
09990 ClearAllErrors()
\\
04205 ScanOnLine(123456789,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
04206 ScanOnLine(1,2,3,123456789,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
04601 SetCsyTransformation(PartCsy, 0, 0, 0, 180.1, 0, 0)
\\
09990 ClearAllErrors()
\\
04801 SetTool("UnDefTool")
\\
09990 ClearAllErrors()
\\
04802 SetTool("Bleep")
\\
09990 ClearAllErrors()
\\
04901 StartSession()
\\
09990 ClearAllErrors()
\\
05101 StopDaemon(E1111)
\\
09990 ClearAllErrors()
\\
05201 TiltCenterPart(1, 0, 0, 1, 0, 0, 1)
\\
09990 ClearAllErrors()
\\
05301 TiltPart(0, 0, 0, 1)
\\
09990 ClearAllErrors()
\\
00001 EndSession()
\\
05401 Get(X())
\\
00001 StartSession()
\\
E0001 AbortE()
\\
05403 Get(X())
\\
09990 ClearAllErrors()
\\
09999 EndSession()
\\
:
:
