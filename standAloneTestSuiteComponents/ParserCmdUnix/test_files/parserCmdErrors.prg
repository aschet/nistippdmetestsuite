00001 StartSession()
\\
E0101 AbortE(3)
\\
09990 ClearAllErrors()
\\
00201 AlignPart(0, -1)
\\
09990 ClearAllErrors()
\\
00202 AlignPart(0"oops"-1, 0, 0.6, 0.0, 0.8, 1.0)
\\
09990 ClearAllErrors()
\\
00203 AlignPart(1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0"oops"2.0)
\\
09990 ClearAllErrors()
\\
00301 AlignTool(0, 1, 30)
\\
09990 ClearAllErrors()
\\
00302 AlignTool(1"oops"0, 0, 45)
\\
09990 ClearAllErrors()
\\
00303 AlignTool(0, -1, 0, 0.6, 0.0, 0.8, 30"oops"45)
\\
09990 ClearAllErrors()
\\
00401 CenterPart(1)
\\
09990 ClearAllErrors()
\\
00402 CenterPart(1, 2, 3, "four")
\\
09990 ClearAllErrors()
\\
00501 ChangeTool("one","two")
\\
09990 ClearAllErrors()
\\
00502 ChangeTool(3)
\\
09990 ClearAllErrors()
\\
00601 ClearAllErrors("Now!")
\\
09990 ClearAllErrors()
\\
00701 DisableUser(,)
\\
09990 ClearAllErrors()
\\
00801 EnableUser(2)
\\
09990 ClearAllErrors()
\\
00901 EndSession(2)
\\
09990 ClearAllErrors()
\\
01001 EnumAllProp(Tool)
\\
09990 ClearAllErrors()
\\
01002 EnumAllProp(Speed())
\\
09990 ClearAllErrors()
\\
01003 EnumAllProp(Tool , )
\\
09990 ClearAllErrors()
\\
01004 EnumAllProp(Tool,GoToPar())
\\
09990 ClearAllErrors()
\\
01005 EnumAllProp(Tool.Speed())
\\
09990 ClearAllErrors()
\\
01006 EnumAllProp(Tool.GoToPar.Tool())
\\
09990 ClearAllErrors()
\\
01007 EnumAllProp(Tool.GoToPar.Speed.Name())
\\
09990 ClearAllErrors()
\\
01101 EnumProp(Tool)
\\
09990 ClearAllErrors()
\\
01102 EnumProp(Speed())
\\
09990 ClearAllErrors()
\\
01103 EnumProp(Tool , )
\\
09990 ClearAllErrors()
\\
01104 EnumProp(Tool,GoToPar())
\\
09990 ClearAllErrors()
\\
01105 EnumProp(Tool.Speed())
\\
09990 ClearAllErrors()
\\
01106 EnumProp(Tool.GoToPar.Tool())
\\
09990 ClearAllErrors()
\\
01107 EnumProp(Tool.GoToPar.Accel.Id())
\\
09990 ClearAllErrors()
\\
01201 EnumTools(2)
\\
09990 ClearAllErrors()
\\
01301 FindTool()
\\
09990 ClearAllErrors()
\\
01302 FindTool(1)
\\
09990 ClearAllErrors()
\\
01401 Get(X)
\\
09990 ClearAllErrors()
\\
01402 Get(X() , Y)
\\
09990 ClearAllErrors()
\\
01403 Get(X() , Y,)
\\
09990 ClearAllErrors()
\\
01404 Get(R(), R())
\\
09990 ClearAllErrors()
\\
01405 Get(X(), X())
\\
09990 ClearAllErrors()
\\
01406 Get(Y(), Y())
\\
09990 ClearAllErrors()
\\
01407 Get(Z(), Z())
\\
09990 ClearAllErrors()
\\
01408 Get(Tool())
\\
09990 ClearAllErrors()
\\
01409 Get(Tool.A(), Tool.A())
\\
09990 ClearAllErrors()
\\
01410 Get(Tool.B(), Tool.B())
\\
09990 ClearAllErrors()
\\
01411 Get(Tool.C(), Tool.C())
\\
09990 ClearAllErrors()
\\
01412 Get(Tool.Q())
\\
09990 ClearAllErrors()
\\
01413 Get(Q())
\\
09990 ClearAllErrors()
\\
01501 GetChangeToolAction()
\\
09990 ClearAllErrors()
\\
01502 GetChangeToolAction(3)
\\
09990 ClearAllErrors()
\\
01601 GetCoordSystem(MachineCsy)
\\
09990 ClearAllErrors()
\\
01701 GetCsyTransformation()
\\
09990 ClearAllErrors()
\\
01702 GetCsyTransformation(MachineCsy)
\\
09990 ClearAllErrors()
\\
01801 GetDMEVersion(2)
\\
09990 ClearAllErrors()
\\
01901 GetErrorInfo()
\\
09990 ClearAllErrors()
\\
01902 GetErrorInfo(2.7)
\\
09990 ClearAllErrors()
\\
E2001 GetErrStatusE(1)
\\
09990 ClearAllErrors()
\\
02101 GetMachineClass(1)
\\
09990 ClearAllErrors()
\\
02201 GetProp(FoundTool.PtMeasPar.Accel(), Tool)
\\
09990 ClearAllErrors()
\\
02202 GetProp(Speed.PtMeasPar.Accel())
\\
09990 ClearAllErrors()
\\
02203 GetProp(Tool,PtMeasPar.Accel())
\\
09990 ClearAllErrors()
\\
02204 GetProp(Tool.X.Accel())
\\
09990 ClearAllErrors()
\\
02205 GetProp(FoundTool.Alignment())
\\
09990 ClearAllErrors()
\\
02206 GetProp(Tool.Speed())
\\
09990 ClearAllErrors()
\\
02207 GetProp(Tool.Accel.Accel())
\\
09990 ClearAllErrors()
\\
02208 GetProp(Tool.PtMeasPar,Speed())
\\
09990 ClearAllErrors()
\\
02209 GetProp(Tool.PtMeasPar.Accel.Max)
\\
09990 ClearAllErrors()
\\
02210 GetProp(FoundTool.PtMeasPar.Dis())
\\
09990 ClearAllErrors()
\\
02211 GetProp(Tool.PtMeasPar.Speed,Max())
\\
09990 ClearAllErrors()
\\
02212 GetProp(Tool.PtMeasPar.Speed.Max)
\\
09990 ClearAllErrors()
\\
02213 GetProp(Tool.PtMeasPar.Speed.Dis())
\\
09990 ClearAllErrors()
\\
02214 GetProp(Tool.PtMeasPar.Speed.Max,)
\\
09990 ClearAllErrors()
\\
02215 GetProp(Tool.PtMeasPar.Speed.Max()())
\\
09990 ClearAllErrors()
\\
E2301 GetPropE(FoundTool.PtMeasPar.Accel(), Tool)
\\
09990 ClearAllErrors()
\\
E2302 GetPropE(Speed.PtMeasPar.Accel())
\\
09990 ClearAllErrors()
\\
E2303 GetPropE(Tool,PtMeasPar.Accel())
\\
09990 ClearAllErrors()
\\
E2304 GetPropE(Tool.X.Accel())
\\
09990 ClearAllErrors()
\\
E2305 GetPropE(FoundTool.Alignment())
\\
09990 ClearAllErrors()
\\
E2306 GetPropE(Tool.Speed())
\\
09990 ClearAllErrors()
\\
E2307 GetPropE(Tool.Accel.Accel())
\\
09990 ClearAllErrors()
\\
E2308 GetPropE(Tool.PtMeasPar,Speed())
\\
09990 ClearAllErrors()
\\
E2309 GetPropE(Tool.PtMeasPar.Accel.Max)
\\
09990 ClearAllErrors()
\\
E2310 GetPropE(FoundTool.PtMeasPar.Dis())
\\
09990 ClearAllErrors()
\\
E2311 GetPropE(Tool.PtMeasPar.Speed,Max())
\\
09990 ClearAllErrors()
\\
E2312 GetPropE(Tool.PtMeasPar.Speed.Max)
\\
09990 ClearAllErrors()
\\
E2313 GetPropE(Tool.PtMeasPar.Speed.Dis())
\\
09990 ClearAllErrors()
\\
E2314 GetPropE(Tool.PtMeasPar.Speed.Max,)
\\
09990 ClearAllErrors()
\\
E2315 GetPropE(Tool.PtMeasPar.Speed.Max()())
\\
09990 ClearAllErrors()
\\
02401 GetXtdErrStatus(1)
\\
09990 ClearAllErrors()
\\
02501 GoTo(10.7)
\\
09990 ClearAllErrors()
\\
02502 GoTo(10,)
\\
09990 ClearAllErrors()
\\
02503 GoTo(X())
\\
09990 ClearAllErrors()
\\
02504 GoTo(X(4), X(4))
\\
09990 ClearAllErrors()
\\
02505 GoTo(Y(4), Y(4))
\\
09990 ClearAllErrors()
\\
02506 GoTo(Z(4), Z(4))
\\
09990 ClearAllErrors()
\\
02507 GoTo(Tool.A())
\\
09990 ClearAllErrors()
\\
02508 GoTo(Tool.A(1), Tool.A(2))
\\
09990 ClearAllErrors()
\\
02509 GoTo(Tool.B(1), Tool.B(2))
\\
09990 ClearAllErrors()
\\
02510 GoTo(Tool.C(1), Tool.C(2))
\\
09990 ClearAllErrors()
\\
02511 GoTo(Q(3))
\\
09990 ClearAllErrors()
\\
02601 Home(1)
\\
09990 ClearAllErrors()
\\
02701 IsHomed(1)
\\
09990 ClearAllErrors()
\\
02801 IsUserEnabled(1)
\\
09990 ClearAllErrors()
\\
02901 LockAxis()
\\
09990 ClearAllErrors()
\\
02902 LockAxis(X(1))
\\
09990 ClearAllErrors()
\\
02903 LockAxis(C(), C())
\\
09990 ClearAllErrors()
\\
02904 LockAxis(Phi(), Phi())
\\
09990 ClearAllErrors()
\\
02905 LockAxis(R(), R())
\\
09990 ClearAllErrors()
\\
02906 LockAxis(X(), X())
\\
09990 ClearAllErrors()
\\
02907 LockAxis(Y(), Y())
\\
09990 ClearAllErrors()
\\
02908 LockAxis(Z(), Z())
\\
09990 ClearAllErrors()
\\
02909 LockAxis(Q())
\\
09990 ClearAllErrors()
\\
E3001 OnMoveReportE(X)
\\
09990 ClearAllErrors()
\\
E3002 OnMoveReportE(X(2))
\\
09990 ClearAllErrors()
\\
E3003 OnMoveReportE(R(), R())
\\
09990 ClearAllErrors()
\\
E3004 OnMoveReportE(X(), X())
\\
09990 ClearAllErrors()
\\
E3005 OnMoveReportE(Y(), Y())
\\
09990 ClearAllErrors()
\\
E3006 OnMoveReportE(Z(), Z())
\\
09990 ClearAllErrors()
\\
E3007 OnMoveReportE(Tool())
\\
09990 ClearAllErrors()
\\
E3008 OnMoveReportE(Tool.A(), Tool.A())
\\
09990 ClearAllErrors()
\\
E3009 OnMoveReportE(Tool.B(), Tool.B())
\\
09990 ClearAllErrors()
\\
E3010 OnMoveReportE(Tool.C(), Tool.C())
\\
09990 ClearAllErrors()
\\
E3011 OnMoveReportE(Time(1), Dis(), X(), Y())
\\
09990 ClearAllErrors()
\\
E3012 OnMoveReportE(Time(1), Dis(10), X(), Y(), Dis(8))
\\
09990 ClearAllErrors()
\\
E3013 OnMoveReportE(Time(), Dis(10), X(), Y(), Z())
\\
09990 ClearAllErrors()
\\
E3014 OnMoveReportE(Time(15), Time(3), Z())
\\
09990 ClearAllErrors()
\\
E3015 OnMoveReportE(Q())
\\
09990 ClearAllErrors()
\\
03101 OnPtMeasReport()
\\
09990 ClearAllErrors()
\\
03102 OnPtMeasReport(X(), Y)
\\
09990 ClearAllErrors()
\\
03103 OnPtMeasReport(X(), Y,)
\\
09990 ClearAllErrors()
\\
03104 OnPtMeasReport(ER(), Z(), ER())
\\
09990 ClearAllErrors()
\\
03105 OnPtMeasReport(IJK(), Z(), IJK())
\\
09990 ClearAllErrors()
\\
03106 OnPtMeasReport(R(), Z(), R())
\\
09990 ClearAllErrors()
\\
03107 OnPtMeasReport(X(), Z(), X())
\\
09990 ClearAllErrors()
\\
03108 OnPtMeasReport(Y(), Z(), Y())
\\
09990 ClearAllErrors()
\\
03109 OnPtMeasReport(Y(), Z(), Z())
\\
09990 ClearAllErrors()
\\
03110 OnPtMeasReport(Tool())
\\
09990 ClearAllErrors()
\\
03111 OnPtMeasReport(Tool.A(), Tool.A())
\\
09990 ClearAllErrors()
\\
03112 OnPtMeasReport(Tool.B(), Tool.B())
\\
09990 ClearAllErrors()
\\
03113 OnPtMeasReport(Tool.C(), Tool.C())
\\
09990 ClearAllErrors()
\\
03114 OnPtMeasReport(Q())
\\
09990 ClearAllErrors()
\\
03201 OnScanReport()
\\
09990 ClearAllErrors()
\\
03202 OnScanReport(X(), Y)
\\
09990 ClearAllErrors()
\\
03203 OnScanReport(X(), Y,)
\\
09990 ClearAllErrors()
\\
03204 OnScanReport(ER(), Z(), ER())
\\
09990 ClearAllErrors()
\\
03205 OnScanReport(IJK(), Z(), IJK())
\\
09990 ClearAllErrors()
\\
03206 OnScanReport(Q(), Z(), Q())
\\
09990 ClearAllErrors()
\\
03207 OnScanReport(R(), Z(), R())
\\
09990 ClearAllErrors()
\\
03208 OnScanReport(X(), Z(), X())
\\
09990 ClearAllErrors()
\\
03209 OnScanReport(Y(), Z(), Y())
\\
09990 ClearAllErrors()
\\
03210 OnScanReport(Y(), Z(), Z())
\\
09990 ClearAllErrors()
\\
03211 OnScanReport(Tool())
\\
09990 ClearAllErrors()
\\
03212 OnScanReport(Tool.A(), Tool.A())
\\
09990 ClearAllErrors()
\\
03213 OnScanReport(Tool.B(), Tool.B())
\\
09990 ClearAllErrors()
\\
03214 OnScanReport(Tool.C(), Tool.C())
\\
09990 ClearAllErrors()
\\
03215 OnScanReport(Dis())
\\
09990 ClearAllErrors()
\\
03301 PtMeas(X)
\\
09990 ClearAllErrors()
\\
03302 PtMeas(X()2)
\\
09990 ClearAllErrors()
\\
03303 PtMeas(X(), Y(2), Z(3))
\\
09990 ClearAllErrors()
\\
03304 PtMeas(X(1), X(2), Z(3))
\\
09990 ClearAllErrors()
\\
03305 PtMeas(X(2), Y(), Z(3))
\\
09990 ClearAllErrors()
\\
03306 PtMeas(Y(1), Y(2), Z(3))
\\
09990 ClearAllErrors()
\\
03307 PtMeas(X(2), Y(1), Z())
\\
09990 ClearAllErrors()
\\
03308 PtMeas(Y(1), Z(2), Z(3))
\\
09990 ClearAllErrors()
\\
03309 PtMeas(X(1), IJK,)
\\
09990 ClearAllErrors()
\\
03310 PtMeas(X(1), IJK(2,2,2), IJK(3,2,2))
\\
09990 ClearAllErrors()
\\
03311 PtMeas(IJK(3,2,2))
\\
09990 ClearAllErrors()
\\
03401 ReQualify(1)
\\
09990 ClearAllErrors()
\\
03501 ScanInCylEndIsPlane(1)
\\
09990 ClearAllErrors()
\\
03502 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,X,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03503 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,-6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03504 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,-3,4,5,6)
\\
09990 ClearAllErrors()
\\
03505 ScanInCylEndIsPlane(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3.1,4,5,6)
\\
09990 ClearAllErrors()
\\
03506 ScanInCylEndIsPlane(X,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6)
\\
09990 ClearAllErrors()
\\
03601 ScanInCylEndIsSphere(1)
\\
09990 ClearAllErrors()
\\
03602 ScanInCylEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,X,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03603 ScanInCylEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,-6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03604 ScanInCylEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,-10,1,2,3)
\\
09990 ClearAllErrors()
\\
03605 ScanInCylEndIsSphere(X,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1,2,3)
\\
09990 ClearAllErrors()
\\
03701 ScanInPlaneEndIsCyl(1)
\\
09990 ClearAllErrors()
\\
03702 ScanInPlaneEndIsCyl(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,X,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03703 ScanInPlaneEndIsCyl(1,2,3,4,5,6,7,8,9,-10,1,2,3,4,5,6,7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03704 ScanInPlaneEndIsCyl(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,-7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03705 ScanInPlaneEndIsCyl(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,-8,9,10,1)
\\
09990 ClearAllErrors()
\\
03706 ScanInPlaneEndIsCyl(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8.1,9,10,1)
\\
09990 ClearAllErrors()
\\
03707 ScanInPlaneEndIsCyl(X,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10,1)
\\
09990 ClearAllErrors()
\\
03801 ScanInPlaneEndIsPlane(1)
\\
09990 ClearAllErrors()
\\
03802 ScanInPlaneEndIsPlane(1,2,3,4,5,6,7,8,9,X,1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03803 ScanInPlaneEndIsPlane(1,2,3,4,5,6,7,8,9,-10,1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03804 ScanInPlaneEndIsPlane(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,-7,8,9,10)
\\
09990 ClearAllErrors()
\\
03805 ScanInPlaneEndIsPlane(1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7.1,8,9,10)
\\
09990 ClearAllErrors()
\\
03806 ScanInPlaneEndIsPlane(X,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
03901 ScanInPlaneEndIsSphere(1)
\\
09990 ClearAllErrors()
\\
03902 ScanInPlaneEndIsSphere(1,2,3,4,5,6,7,8,9,X,1,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
03903 ScanInPlaneEndIsSphere(1,2,3,4,5,6,7,8,9,-10,1,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
03904 ScanInPlaneEndIsSphere(1,2,3,4,5,6,7,8,9,10,1,2,3,-4,5,6,7)
\\
09990 ClearAllErrors()
\\
03905 ScanInPlaneEndIsSphere(X,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
04001 ScanOnCircle(1)
\\
09990 ClearAllErrors()
\\
04002 ScanOnCircle(1,2,3,4,5,6,7,8,9,10,1,X)
\\
09990 ClearAllErrors()
\\
04003 ScanOnCircle(X,2,3,4,5,6,7,8,9,10,1,2)
\\
09990 ClearAllErrors()
\\
04101 ScanOnCircleHint(1)
\\
09990 ClearAllErrors()
\\
04201 ScanOnLine(1)
\\
09990 ClearAllErrors()
\\
04202 ScanOnLine(1,2,3,4,5,6,7,8,9,X)
\\
09990 ClearAllErrors()
\\
04203 ScanOnLine(1,2,3,4,5,6,7,8,9,-10)
\\
09990 ClearAllErrors()
\\
04204 ScanOnLine(X,2,3,4,5,6,7,8,9,10)
\\
09990 ClearAllErrors()
\\
04301 ScanOnLineHint(1)
\\
09990 ClearAllErrors()
\\
04401 ScanUnknownHint()
\\
09990 ClearAllErrors()
\\
04402 ScanUnknownHint("bigger than a breadbox")
\\
09990 ClearAllErrors()
\\
04403 ScanUnknownHint(-1)
\\
09990 ClearAllErrors()
\\
04501 SetCoordSystem(MachineCsy, PartCsy)
\\
09990 ClearAllErrors()
\\
04502 SetCoordSystem(JogMoveCsy)
\\
09990 ClearAllErrors()
\\
04601 SetCsyTransformation(PartCsy)
\\
09990 ClearAllErrors()
\\
04602 SetCsyTransformation(1, 1, 1, 1, 180, 180, 180)
\\
09990 ClearAllErrors()
\\
04603 SetCsyTransformation(X, 1, 1, 1, 180, 180, 180)
\\
09990 ClearAllErrors()
\\
04604 SetCsyTransformation(PartCsy, 1, 1, 1, 200, 180, 180)
\\
09990 ClearAllErrors()
\\
04701 SetProp(Tool.PtMeasPar(54))
\\
09990 ClearAllErrors()
\\
04702 SetProp(X.PtMeasPar.Accel(3))
\\
09990 ClearAllErrors()
\\
04703 SetProp(FoundTool,PtMeasPar,Speed(25.0))
\\
09990 ClearAllErrors()
\\
04704 SetProp(FoundTool.Accel.Accel(2))
\\
09990 ClearAllErrors()
\\
04705 SetProp(FoundTool.PtMeasPar.Dis(1))
\\
09990 ClearAllErrors()
\\
04706 SetProp(FoundTool.PtMeasPar.Accel())
\\
09990 ClearAllErrors()
\\
04707 SetProp(FoundTool.PtMeasPar.Accel.Max)
\\
09990 ClearAllErrors()
\\
04708 SetProp(FoundTool.PtMeasPar.Accel.Max(3))
\\
09990 ClearAllErrors()
\\
04709 SetProp(FoundTool.PtMeasPar.Accel.Act())
\\
09990 ClearAllErrors()
\\
04710 SetProp(FoundTool.PtMeasPar.Accel.Act(4)(4))
\\
09990 ClearAllErrors()
\\
04801 SetTool()
\\
09990 ClearAllErrors()
\\
04802 SetTool(2)
\\
09990 ClearAllErrors()
\\
04901 StartSession(1)
\\
09990 ClearAllErrors()
\\
05001 StopAllDaemons(1)
\\
09990 ClearAllErrors()
\\
05101 StopDaemon()
\\
09990 ClearAllErrors()
\\
05102 StopDaemon(05101)
\\
09990 ClearAllErrors()
\\
05201 TiltCenterPart()
\\
09990 ClearAllErrors()
\\
05202 TiltCenterPart(X,2,3,4,5,6,7)
\\
09990 ClearAllErrors()
\\
05301 TiltPart()
\\
09990 ClearAllErrors()
\\
05302 TiltPart(X,2,3,4)
\\
09990 ClearAllErrors()
\\
05400 Errors from parseCommand
\\
09990 ClearAllErrors()
\\
05401 GoTo(X(0.1))2
\\
09990 ClearAllErrors()
\\
05402 AbortE()
\\
09990 ClearAllErrors()
\\
E5403 GoTo(X(0.1))
\\
09990 ClearAllErrors()
\\
05500 Errors from parseCmdArguments
\\
09990 ClearAllErrors()
\\
05501 GoTo(X(1,)
\\
09990 ClearAllErrors()
\\
05502 GoTo(X(#))
\\
09990 ClearAllErrors()
\\
05600 Errors from parseCmdName
\\
09990 ClearAllErrors()
\\
05601 CmdName()
\\
09990 ClearAllErrors()
\\
05700 Errors from parseKeyword
\\
09990 ClearAllErrors()
\\
05701 GoTo(X2)
\\
09990 ClearAllErrors()
\\
05702 GoTo(Keyword)
\\
09990 ClearAllErrors()
\\
05800 Errors from parseNumber
\\
09990 ClearAllErrors()
\\
05801 GoTo(X(+1.1.2))
\\
09990 ClearAllErrors()
\\
05802 GoTo(X(+.))
\\
09990 ClearAllErrors()
\\
05803 GoTo(X(12345671901234567))
\\
09990 ClearAllErrors()
\\
05804 GoTo(X(2.1E0001))
\\
09990 ClearAllErrors()
\\
05900 Errors from parseString
\\
09990 ClearAllErrors()
\\
05901 GoTo("hello)
\\
09990 ClearAllErrors()
\\
05902 GoTo("")
\\
09990 ClearAllErrors()
\\
06000 Errors from parseTag
\\
09990 ClearAllErrors()
\\
A2345 GoTo(X(3.1))
\\
09990 ClearAllErrors()
\\
012345 GoTo(X(3.1))
\\
09990 ClearAllErrors()
\\
E0000 AbortE()
\\
09990 ClearAllErrors()
\\
00000 GoTo(X(2.1))
\\
09990 ClearAllErrors()
\\
06100 Errors from parseIJK
\\
09990 ClearAllErrors()
\\
06101 GoTo(IJK(1+2+3))
\\
09990 ClearAllErrors()
\\
06102 GoTo(IJK(2,A))
\\
09990 ClearAllErrors()
\\
06103 GoTo(IJK(1,2,3,4))
\\
09990 ClearAllErrors()
\\
06200 Errors from unSpaceInputArray
\\
09990 ClearAllErrors()
\\
06201 GoTo(X(2.1 E2))
\\
09990 ClearAllErrors()
\\
09999 EndSession()
\\
:
:
