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
00060 GoTo(X(10), Y(11), Z(12))
\\
00070 Get(X(), Y(), Z())
\\
00080 Home()
\\
00090 Get(X(), Y(), Z())
\\
00100 IsHomed()
\\
09999 EndSession()
\\
:
:
