00010 StartSession()
\\
00020 Home()
\\
00030 ChangeTool("Probe1")
\\
00040 SetProp(Tool.GoToPar.Accel(50.0))
\\
00050 SetProp(Tool.GoToPar.Speed(5.0))
\\
E0060 OnMoveReportE(Dis(1), X(), Y(), Z())
\\
00070 GoTo(X(20), Y(40), Z(60))
\\
E0090 AbortE()
\\
00100 ClearAllErrors()
\\
E0110 OnMoveReportE(Dis(1), X(), Y(), Z())
\\
00120 Home()
\\
00130 StopAllDaemons()
\\
09999 EndSession()
\\
:
:
