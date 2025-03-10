00010 StartSession()
\\
00020 Home()
\\
00030 ChangeTool("Probe1")
\\
00040 SetProp(Tool.GoToPar.Accel(1.0))
\\
00050 SetProp(Tool.GoToPar.Speed(5.0))
\\
E0060 OnMoveReportE(Dis(5), X(), Y(), Z())
\\
00070 GoTo(X(20), Y(25), Z(30))
\\
00080 StopAllDaemons()
\\
09999 EndSession()
\\
:
:
