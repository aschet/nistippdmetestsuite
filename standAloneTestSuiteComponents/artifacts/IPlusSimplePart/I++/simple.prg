00010 StartSession()
\\
00020 Home()
\\
00030 IsHomed()
\\
00040 SetTool("RefTool")
\\
00050 GoTo(X(50), Y(50), Z(175))
\\
00060 ChangeTool("RefTool")
\\
00070 SetProp(Tool.PtMeasPar.Approach(10))
\\
00080 GetProp(Tool.PtMeasPar.Approach())
\\
00090 SetProp(Tool.PtMeasPar.Search(10))
\\
00100 GetProp(Tool.PtMeasPar.Search())
\\
00110 SetProp(Tool.PtMeasPar.Retract(10))
\\
00120 GetProp(Tool.PtMeasPar.Retract())
\\
00130 SetProp(Tool.GoToPar.Speed(30))
\\
00140 GetProp(Tool.GoToPar.Speed())
\\
00150 OnPtMeasReport(X(), Y(), Z(), R())
\\
00160 GoTo(X(50), Y(50), Z(75))
\\
00170 GoTo(X(50), Y(50), Z(40))
\\
00180 PtMeas(X(75.0), Y(50.0), Z(40.0), IJK(-1.0, 0.0, 0.0))
\\
00190 PtMeas(X(25.0), Y(50.0), Z(40.0), IJK(+1.0, 0.0, 0.0))
\\
00200 PtMeas(X(50.0), Y(75.0), Z(40.0), IJK(+0.0,-1.0, 0.0))
\\
00210 PtMeas(X(50.0), Y(25.0), Z(40.0), IJK(-0.0,+1.0, 0.0))
\\
00220 GoTo(X(50), Y(50), Z(40))
\\
00230 GoTo(X(50), Y(50), Z(100))
\\
09999 EndSession()
\\
:
:
