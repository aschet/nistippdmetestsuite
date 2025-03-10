00001 StartSession()
\\
00005 ChangeTool("Probe1")
\\
00007 FindTool("RefTool")
\\
00010 SetProp(Tool.PtMeasPar.Accel(5.42E01))
\\
00015 GetProp(Tool.PtMeasPar.Accel())
\\
00020 SetProp(Tool.PtMeasPar.Accel.Act(6.42))
\\
00025 GetProp(Tool.PtMeasPar.Accel.Act())
\\
00030 SetProp(Tool.PtMeasPar.Speed(1.45))
\\
00035 GetProp(Tool.PtMeasPar.Speed())
\\
00040 SetProp(Tool.PtMeasPar.Speed.Act(1.55))
\\
00045 GetProp(Tool.PtMeasPar.Speed.Act())
\\
00050 SetProp(Tool.PtMeasPar.Approach(2.77))
\\
00055 GetProp(Tool.PtMeasPar.Approach())
\\
00060 SetProp(Tool.PtMeasPar.Approach.Act(3.77))
\\
00065 GetProp(Tool.PtMeasPar.Approach.Act())
\\
00070 SetProp(Tool.PtMeasPar.Retract(2.09))
\\
00075 GetProp(Tool.PtMeasPar.Retract())
\\
00080 SetProp(Tool.PtMeasPar.Retract.Act(3.09))
\\
00085 GetProp(Tool.PtMeasPar.Retract.Act())
\\
00090 SetProp(Tool.PtMeasPar.Search(1.508))
\\
00095 GetProp(Tool.PtMeasPar.Search())
\\
00100 SetProp(Tool.PtMeasPar.Search.Act(2.508))
\\
00105 GetProp(Tool.PtMeasPar.Search.Act())
\\
00110 SetProp(Tool.GoToPar.Accel(5.42E01))
\\
00115 GetProp(Tool.GoToPar.Accel())
\\
00120 SetProp(Tool.GoToPar.Accel.Act(6.42E01))
\\
00125 GetProp(Tool.GoToPar.Accel.Act())
\\
00130 SetProp(Tool.GoToPar.Speed(5.45))
\\
00135 GetProp(Tool.GoToPar.Speed())
\\
00140 SetProp(Tool.GoToPar.Speed.Act(5.55))
\\
00145 GetProp(Tool.GoToPar.Speed.Act())
\\
00150 SetProp(FoundTool.PtMeasPar.Accel(5.42E01))
\\
00155 GetProp(FoundTool.PtMeasPar.Accel())
\\
00160 SetProp(FoundTool.PtMeasPar.Accel.Act(6.42E01))
\\
00165 GetProp(FoundTool.PtMeasPar.Accel.Act())
\\
00170 SetProp(FoundTool.PtMeasPar.Speed(2.45))
\\
00175 GetProp(FoundTool.PtMeasPar.Speed())
\\
00180 SetProp(FoundTool.PtMeasPar.Speed.Act(2.55))
\\
00185 GetProp(FoundTool.PtMeasPar.Speed.Act())
\\
00190 SetProp(FoundTool.PtMeasPar.Approach(2.77))
\\
00195 GetProp(FoundTool.PtMeasPar.Approach())
\\
00200 SetProp(FoundTool.PtMeasPar.Approach.Act(3.77))
\\
00205 GetProp(FoundTool.PtMeasPar.Approach.Act())
\\
00210 SetProp(FoundTool.PtMeasPar.Retract(2.09))
\\
00215 GetProp(FoundTool.PtMeasPar.Retract())
\\
00220 SetProp(FoundTool.PtMeasPar.Retract.Act(3.09))
\\
00225 GetProp(FoundTool.PtMeasPar.Retract.Act())
\\
00230 SetProp(FoundTool.PtMeasPar.Search(1.508))
\\
00235 GetProp(FoundTool.PtMeasPar.Search())
\\
00240 SetProp(FoundTool.PtMeasPar.Search.Act(2.508))
\\
00245 GetProp(FoundTool.PtMeasPar.Search.Act())
\\
00250 SetProp(FoundTool.GoToPar.Accel(5.42E01))
\\
00255 GetProp(FoundTool.GoToPar.Accel())
\\
00260 SetProp(FoundTool.GoToPar.Accel.Act(6.42E01))
\\
00265 GetProp(FoundTool.GoToPar.Accel.Act())
\\
00270 SetProp (FoundTool.GoToPar.Speed(6.45))
\\
00275 GetProp (FoundTool.GoToPar.Speed())
\\
00280 SetProp (FoundTool.GoToPar.Speed.Act(6.55))
\\
00285 GetProp (FoundTool.GoToPar.Speed.Act())
\\
00290 SetProp(FoundTool.GoToPar.Speed(7.45), FoundTool.PtMeasPar.Speed(3.45))
\\
00295 GetProp(FoundTool.GoToPar.Speed(), FoundTool.PtMeasPar.Speed())
\\
00300 SetProp(Tool.PtMeasPar.Approach(2.77), Tool.PtMeasPar.Retract(2.09) , Tool.PtMeasPar.Search(1.508))
\\
00305 GetProp(Tool.PtMeasPar.Approach(), Tool.PtMeasPar.Retract() , Tool.PtMeasPar.Search())
\\
09999 EndSession()
\\
:
:
