00001 StartSession()
\\
00002 Home()
\\
00003 ChangeTool("Probe1")
\\
00101 AlignTool(1, 0, 0, 45)
\\
00102 AlignTool(1, 0, 0, 0.6, 0.0, 0.8, 30, 45)
\\
00201 CenterPart(0.707, 0.707, 0.0, 0.11)
\\
00301 EnumProp(Tool.GoToPar.Speed())
\\
00401 StopDaemon(E1234)
\\
00004 ClearAllErrors()
\\
00501 GetChangeToolAction("Probe1")
\\
00601 GetCoordSystem()
\\
00701 GetCsyTransformation(PartCsy)
\\
00801 GetDMEVersion()
\\
E0901 GetErrStatusE()
\\
01001 GetMachineClass()
\\
01101 GetProp(Tool.Name())
\\
01102 GetProp(Tool.PtMeasPar.Accel(), Tool.PtMeasPar.Accel.Max())
\\
01201 IsHomed()
\\
01301 IsUserEnabled()
\\
01401 GetProp(Tool.Alignment())
\\
01501 GetProp(Tool.CollisionVolume())
\\
01601 GetProp(Tool.Id())
\\
01701 GetProp(Tool.Name())
\\
01801 GetProp(Tool.GoToPar.Accel())
\\
00005 OnPtMeasReport(X(), Y())
\\
01901 PtMeas(X(3), Y(4), IJK(0,0,1))
\\
01902 Get(X(), Y())
\\
00006 OnScanReport(X(), Y(), Z())
\\
02001 ScanOnCircle(0,0,0,4,5,6,6,0,-4,10,1,2)
\\
02101 GetErrorInfo(0513)
\\
02201 TiltCenterPart(0.707, 0.707, 0.0, 1, 2, 3, 0.11)
\\
02301 TiltPart(0.707, 0.707, 0.0, 0.11)
\\
09999 EndSession()
\\
:
:
