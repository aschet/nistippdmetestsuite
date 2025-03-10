00001 StartSession()
\\
00010 Home()
\\
00020 ChangeTool("Probe1")
\\
00030 SetProp(Tool.GoToPar.Speed(5))
\\
00040 SetProp(Tool.GoToPar.Accel(500))
\\
00050 SetProp(Tool.PtMeasPar.Accel(50))
\\
00060 SetProp(Tool.PtMeasPar.Speed(1))
\\
00070 OnPtMeasReport(X(), Y(), Z(), IJK())
\\
00080 PtMeas(X(1.33), Y(-2.6E01), Z(30.998), IJK(2,2,2))
\\
00090 PtMeas(X(1.33), IJK(0, 0, 1.0), Y(-2.6E01), Z(30.998))
\\
00100 OnPtMeasReport(IJK(), X(), Y())
\\
00110 PtMeas(IJK(0.2, 0.3, 0.4), X(-2.6E01), Y(0.779))
\\
00120 OnPtMeasReport(Z(), X(), Y(), Tool.A(), Tool.B(), Tool.C(), ER())
\\
00130 PtMeas(Z(+1.9473294E-01), X(-2.6E01), Y(0.77998))
\\
00140 OnPtMeasReport(IJK(), Z())
\\
00150 PtMeas(IJK(2,2,2), Z(2.1))
\\
00160 OnPtMeasReport(X(), Y(), Z(), Tool.A(), Tool.B(), Tool.C(), ER())
\\
00170 PtMeas(X(-2.6E01), Y(0.779))
\\
00180 OnPtMeasReport(X(), Tool.A(), ER())
\\
00190 PtMeas(X(-2.6E01))
\\
00200 OnPtMeasReport(Y())
\\
00210 PtMeas(Y(0.779))
\\
09999 EndSession()
\\
:
:
