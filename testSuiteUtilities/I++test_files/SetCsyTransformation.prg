00001 StartSession()
\\
00002 Home()
\\
00003 ChangeTool("RefTool")
\\
00004 GoTo(X(0), Y(0), Z(0))
\\
00005 Get(X(), Y(), Z())
\\
00006 SetCoordSystem(PartCsy)
\\
00007 Get(X(), Y(), Z())
\\
00008 GoTo(X(2), Y(3), Z(4))
\\
00009 Get(X(), Y(), Z())
\\
00010 SetCsyTransformation(PartCsy, 0, 0, 0, 0, 180, 0)
\\
00015 Get(X(), Y(), Z())
\\
00020 SetCsyTransformation(PartCsy, 0, 0, 0, 0, 0, 180)
\\
00025 Get(X(), Y(), Z())
\\
00030 SetCsyTransformation(PartCsy, 0, 0, 0, 0, 29, 151)
\\
00035 Get(X(), Y(), Z())
\\
00040 SetCsyTransformation(PartCsy, 0, 0, 0, 180, 0, 0)
\\
00045 Get(X(), Y(), Z())
\\
00050 SetCsyTransformation(PartCsy, 5, 6, 7, 0, 0, 0)
\\
00055 Get(X(), Y(), Z())
\\
00056 SetCoordSystem(MoveableMachineCsy)
\\
00057 Get(X(), Y(), Z())
\\
00058 SetCoordSystem(MultipleArmCsy)
\\
00059 Get(X(), Y(), Z())
\\
00060 SetCsyTransformation(MultipleArmCsy, 1, 0, 0, 90, 0, 0)
\\
00061 Get(X(), Y(), Z())
\\
00062 SetCoordSystem(MoveableMachineCsy)
\\
00063 Get(X(), Y(), Z())
\\
00070 SetCsyTransformation(MoveableMachineCsy, 1, 0, 0, 90, 0, 0)
\\
00075 Get(X(), Y(), Z())
\\
00076 SetCoordSystem(MachineCsy)
\\
00077 Get(X(), Y(), Z())
\\
00080 SetCsyTransformation(JogDisplayCsy, 0, 2, 0, 90, 0, 0)
\\
00090 SetCsyTransformation(JogMoveCsy, 0, 3, 0, 90, 0, 0)
\\
00100 SetCsyTransformation(SensorCsy, 0, 0, 4, 90, 0, 0)
\\
09999 EndSession()
\\
:
:
