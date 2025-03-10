00010 StartSession()
\\
E0020 AbortE()
\\
00021 ClearAllErrors()
\\
00029 Home()
\\
00030 AlignPart(0, -1, 0, 0.6, 0.0, 0.8, 1.0)
\\
00040 AlignPart(1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 2.0)
\\
00050 AlignTool(0, 0, 1, 30)
\\
00060 AlignTool(0, 0, 1, 1, 0, 0, 30, 45)
\\
00070 CenterPart(2.0, 3.0, 4.0, 0.1)
\\
00080 ChangeTool("Probe1")
\\
00090 ClearAllErrors()
\\
00095 EnableUser()
\\
00100 DisableUser()
\\
00110 EnableUser()
\\
00120 EnumAllProp(Tool())
\\
00130 EnumProp(Tool.GoToPar())
\\
00140 EnumProp(Tool.PtMeasPar())
\\
00150 EnumTools()
\\
00160 FindTool("BaseTool")
\\
00170 Get(X())
\\
00180 Get(Tool.A(), Tool.B(), Tool.C())
\\
00190 Get(Z(), R(), Y(), Tool.C(), X())
\\
00200 GetChangeToolAction("Probe1")
\\
00210 GetCoordSystem()
\\
00220 GetCsyTransformation(JogDisplayCsy)
\\
00230 GetCsyTransformation(JogMoveCsy)
\\
00240 GetCsyTransformation(MultipleArmCsy)
\\
00250 GetCsyTransformation(PartCsy)
\\
00260 GetCsyTransformation(SensorCsy)
\\
00270 GetDMEVersion()
\\
00280 GetErrorInfo(0513)
\\
E0290 GetErrStatusE()
\\
00300 GetMachineClass()
\\
00310 GetProp(FoundTool.PtMeasPar.Accel())
\\
00320 GetProp(FoundTool.PtMeasPar.Speed())
\\
00330 GetProp(Tool.PtMeasPar.Accel())
\\
00340 GetProp(Tool.PtMeasPar.Accel.Max(), Tool.PtMeasPar.Speed.Max())
\\
00350 GetProp(Tool.PtMeasPar.Accel.Min(),Tool.PtMeasPar.Speed.Min())
\\
00360 GetProp(Tool.CollisionVolume(), Tool.Id(), Tool.Name())
\\
E0370 GetPropE(Tool.PtMeasPar.Accel())
\\
E0380 GetPropE(Tool.PtMeasPar.Accel.Max())
\\
E0390 GetPropE(Tool.PtMeasPar.Speed.Max())
\\
E0400 GetPropE(Tool.PtMeasPar.Accel.Min())
\\
E0410 GetPropE(Tool.PtMeasPar.Speed.Min())
\\
E0420 GetPropE(Tool.PtMeasPar.Speed())
\\
00430 GetXtdErrStatus()
\\
00440 GoTo(X(10), Y(11), Z(12))
\\
00450 Home()
\\
00460 IsHomed()
\\
00470 IsUserEnabled()
\\
00480 LockAxis(X(), Phi(), C())
\\
E0490 OnMoveReportE(Time(1), Dis(10), X(), Y(), Z())
\\
00491 StopDaemon(E0490)
\\
00500 OnPtMeasReport(X(), Y(), Z())
\\
00510 OnScanReport(X(), Y(), Tool.A(), R(), Tool.B(), Z(), Tool.C())
\\
00520 PtMeas(X(1), Y(2), Z(3), IJK(2,2,2))
\\
00530 ReQualify()
\\
00540 ScanInCylEndIsPlane(0,0,0,0,0,1,2,0,0,1,0,0,2,-1,0,0.9,0,2,7,0,0,1,1,0,-1,0)
\\
00550 ScanInCylEndIsSphere(0,0,0,0,0,1,2,0,0,1,0,0,2,1,0,0.8,0,2,7,4,0,1,0)
\\
00560 ScanInPlaneEndIsCyl(0,0,0,0,0,1,2,2,0,0.5,8,8,0,0,0,1,0.7,1,0,0,1)
\\
00570 ScanInPlaneEndIsPlane(0,0,0,0,0,1,2,0,0,0.5,9,0,0,-1,0,0,1,0,0,1)
\\
00580 ScanInPlaneEndIsSphere(0,0,0,0,0,1,2,0,0,0.5,9,0,0,1.4,0,0,1)
\\
00590 ScanOnCircle(0,0,0,4,5,6,6,0,-4,10,1,2)
\\
00600 ScanOnCircleHint(10.0, 1)
\\
00610 ScanOnLine(0,1,2,14,15,2,0,0,1,0.5)
\\
00620 ScanOnLineHint(20, 2.45)
\\
00630 ScanUnknownHint(100.0)
\\
00640 SetCoordSystem(MachineCsy)
\\
00650 SetCoordSystem(MoveableMachineCsy)
\\
00660 SetCoordSystem(MultipleArmCsy)
\\
00670 SetCoordSystem(PartCsy)
\\
00680 SetCsyTransformation(JogDisplayCsy, 1, 1, 1, 180, 180, 180)
\\
00690 SetCsyTransformation(JogMoveCsy, 1, 1, 1, 90, 90, 90)
\\
00700 SetCsyTransformation(MultipleArmCsy, 1, 1, 1, 45, 45, 45)
\\
00710 SetCsyTransformation(PartCsy, 1, 1, 1, 30, 30, 30)
\\
00720 SetCsyTransformation(SensorCsy, 2, 2, 2, 45, 45, 45)
\\
00730 SetProp(Tool.PtMeasPar.Accel(54))
\\
00740 SetProp(Tool.PtMeasPar.Approach.Act(99))
\\
00750 SetProp(Tool.PtMeasPar.Search(10), Tool.PtMeasPar.Retract(78))
\\
00760 SetProp(Tool.PtMeasPar.Retract(78))
\\
00770 SetProp(Tool.GoToPar.Speed(400),Tool.PtMeasPar.Speed.Act(400))
\\
00780 SetProp(Tool.PtMeasPar.Speed(45))
\\
00790 SetTool("BaseTool")
\\
E0795 OnMoveReportE(Time(15), Dis(3), X(), Y(), Z())
\\
00800 StopAllDaemons()
\\
E0805 OnMoveReportE(Time(15), Dis(3), X(), Y(), Z())
\\
00810 StopDaemon(E0805)
\\
00820 TiltCenterPart(0, 0, 0, 0, 0, 3, 0.2)
\\
00830 TiltPart(0.6, 0.8, 0.0, 9)
\\
09999 EndSession()
\\
:
:
