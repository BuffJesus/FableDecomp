#!/usr/bin/env python3
"""Measure BULLYRUN1 movement completion across archived Forge logs."""
from __future__ import annotations
import argparse,json,math,re
from pathlib import Path

MARKER=(3596.740,851.895,13.375)
# Installed StartOakValeEast.tng: authored teleport immediately before RunTo.
RUN_START=(3604.479,860.742,13.707)
POS=r'\(([-+0-9.]+),([-+0-9.]+),([-+0-9.]+)\)'

def _point(match:re.Match[str],start:int)->tuple[float,float,float]:
 return tuple(float(match.group(start+i)) for i in range(3))

def analyze_text(text:str)->list[dict]:
 before=list(re.finditer(r'NOVI_PROBE BullyRun before bully='+POS,text))
 after=list(re.finditer(r'NOVI_PROBE BullyRun after1 bully='+POS,text))
 rows=[]
 for index,(a,b) in enumerate(zip(before,after)):
  start=_point(a,1);end=_point(b,1)
  moved=math.dist(start,end);remaining=math.dist(end,MARKER)
  rows.append({'index':index,'start':list(start),'end':list(end),'moved':round(moved,3),
               'markerRemaining':round(remaining,3),'reachedRetailTolerance':remaining<=2.0,
               'runStartDisplacement':round(math.dist(RUN_START,end),3),
               'endedAtPreRunTeleport':math.dist(RUN_START,end)<=0.01})
 return rows

def analyze_directory(folder:Path)->dict:
 runs=[]
 for path in sorted(folder.glob('*.log')):
  for row in analyze_text(path.read_text(encoding='utf-8',errors='replace')):
   row['log']=path.name;runs.append(row)
 complete=sum(row['reachedRetailTolerance'] for row in runs)
 return {'schema':'new-oakvale-bully-run-variance/0.2','marker':list(MARKER),'runStartMarker':list(RUN_START),
         'measurementNote':'moved includes the authored pre-run teleport; runStartDisplacement is endpoint displacement, not sampled path length.', 'runs':runs,
         'runCount':len(runs),'complete':complete,'incomplete':len(runs)-complete,
         'nondeterministic':complete>0 and complete<len(runs)}

def main()->int:
 ap=argparse.ArgumentParser();ap.add_argument('folder',type=Path);ap.add_argument('--output',type=Path);a=ap.parse_args()
 result=analyze_directory(a.folder);payload=json.dumps(result,indent=2)+'\n'
 if a.output:a.output.write_text(payload,encoding='utf-8')
 else:print(payload,end='')
 return 0
if __name__=='__main__':raise SystemExit(main())
