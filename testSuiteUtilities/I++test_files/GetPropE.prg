00001 StartSession()
\\
00005 ChangeTool("Probe1")
\\
00007 FindTool("RefTool")
\\
E0010 GetPropE(Tool.Name())
\\
E0020 GetPropE(FoundTool.Name(), FoundTool.Id())
\\
E0030 GetPropE(Tool.GoToPar.Speed())
\\
E0040 GetPropE(Tool.PtMeasPar.Accel(), Tool.PtMeasPar.Accel.Max())
\\
E0050 GetPropE(Tool.GoToPar.Accel.Act(), Tool.Name())
\\
E0060 GetPropE(Tool.PtMeasPar.Approach(), Tool.PtMeasPar.Retract(), Tool.PtMeasPar.Search())
\\
E0070 GetPropE(Tool.Alignment())
\\
E0080 GetPropE(Tool.Alignment())
\\
E0090 GetPropE(Tool.CollisionVolume())
\\
E0100 GetPropE(Tool.CollisionVolume(), Tool.Name())
\\
E0110 GetPropE(FoundTool.GoToPar.Speed.Def(), Tool.PtMeasPar.Approach.Def())
\\
E0120 GetPropE(Tool.CollisionVolume(), Tool.PtMeasPar.Approach.Def())
\\
09999 EndSession()
\\
:
:
