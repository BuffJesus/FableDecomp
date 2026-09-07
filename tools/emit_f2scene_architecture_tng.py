#!/usr/bin/env python3
"""Emit a dry-run architecture placement plan into disposable TNG files."""
from __future__ import annotations
import argparse, json, shutil, subprocess
from collections import defaultdict
from pathlib import Path

UID_BASE = 0xFFFFFE0200000000

def main():
    ap=argparse.ArgumentParser(); ap.add_argument('plan',type=Path); ap.add_argument('empty_tng',type=Path); ap.add_argument('forge',type=Path); ap.add_argument('out',type=Path); a=ap.parse_args()
    plan=json.loads(a.plan.read_text(encoding='utf-8')); grouped=defaultdict(list)
    for row in plan['placements']: grouped[row['level_name']].append(row)
    a.out.mkdir(parents=True,exist_ok=True); emitted=[]; ordinal=0
    for level in sorted(grouped):
        target=a.out/f'{level}.tng'; shutil.copyfile(a.empty_tng,target)
        for row in sorted(grouped[level],key=lambda x:x['instance_id']):
            uid=UID_BASE+ordinal; ordinal+=1; x,y,z=row['tng_local_position']
            cmd=[str(a.forge),'tng','place',str(target),'--out',str(target),'--def',row['object_definition'],
                 '--local',f'{x:.9g},{y:.9g}','--z',f'{z:.9g}','--yaw',f"{row['yaw_degrees']:.9g}",
                 '--uid',str(uid),'--not-targetable','--not-game-persistent','--not-level-persistent','--no-health','--json']
            subprocess.run(cmd,check=True,stdout=subprocess.DEVNULL)
            emitted.append({**row,'uid':uid,'tng':target.name})
    manifest={'schema':'fableforge.f2scene_architecture_tng.v1','source_plan':str(a.plan.resolve()),
              'source_template':str(a.empty_tng.resolve()),'uid_base':UID_BASE,
              'authoring_policy':{'targetable':False,'thing_game_persistent':False,'thing_level_persistent':False,'health':'omitted'},
              'counts':{'files':len(grouped),'things':len(emitted)},'placements':emitted}
    (a.out/'tng_manifest.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(manifest['counts'],indent=2)); return 0
if __name__=='__main__': raise SystemExit(main())
