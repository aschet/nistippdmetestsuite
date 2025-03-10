00001 StartSession()
\\
00010 Get(R())
\\
00020 Get(X())
\\
00030 Get(Y())
\\
00040 Get(Z())
\\
00050 Get(Tool.A())
\\
00060 Get(Tool.B())
\\
00070 Get(Tool.C())
\\
00080 Get(X(), Y())
\\
00090 Get(Y(), Z())
\\
00100 Get(X(), R())
\\
00110 Get(X(), Y(), Z())
\\
00120 Get(Y(), Z(), X())
\\
00130 Get(Z(), X(), Y())
\\
00140 Get(Tool.A(), Z(), X(), Y())
\\
00150 Get(Tool.A(), Z(), Tool.B(), X(), Y())
\\
00160 Get(Tool.A(), Z(), Tool.B(), X(), Tool.C(), Y())
\\
00170 Get(R(), Tool.A(), Z(), Tool.B(), X(), Tool.C(), Y())
\\
09999 EndSession()
\\
:
:
