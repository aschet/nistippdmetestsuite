00001 StartSession()
\\
00005 ChangeTool("Probe1")
\\
00007 FindTool("RefTool")
\\
00010 GetProp(Tool.Name())
\\
00020 GetProp(FoundTool.Name(), FoundTool.Id())
\\
00030 GetProp(Tool.GoToPar.Speed())
\\
00040 GetProp(Tool.PtMeasPar.Accel(), Tool.PtMeasPar.Accel.Max())
\\
00050 GetProp(Tool.GoToPar.Accel.Act(), Tool.Name())
\\
00060 GetProp(Tool.PtMeasPar.Approach(), Tool.PtMeasPar.Retract(), Tool.PtMeasPar.Search())
\\
00070 GetProp(Tool.Alignment())
\\
00080 GetProp(Tool.Alignment())
\\
00090 GetProp(Tool.CollisionVolume())
\\
00100 GetProp(Tool.CollisionVolume(), Tool.Name())
\\
00110 GetProp(FoundTool.GoToPar.Speed.Def(), Tool.PtMeasPar.Approach.Def())
\\
00120 GetProp(Tool.CollisionVolume(), Tool.PtMeasPar.Approach.Def())
\\
09999 EndSession()
\\
:
:
