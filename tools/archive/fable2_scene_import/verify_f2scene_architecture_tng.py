#!/usr/bin/env python3
"""Independently verify emitted architecture TNG blocks against their manifest."""
from __future__ import annotations
import argparse, json, math, re
from collections import defaultdict
from pathlib import Path

def fields(block):
    out={}
    for line in block.splitlines():
        m=re.match(r'([A-Za-z0-9_]+)\s+(.*);$',line.strip())
        if m: out[m.group(1)]=m.group(2).strip('"')
    return out

def main():
    ap=argparse.ArgumentParser(); ap.add_argument('package',type=Path); a=ap.parse_args()
    manifest=json.loads((a.package/'tng_manifest.json').read_text(encoding='utf-8')); expected=defaultdict(dict)
    for p in manifest['placements']: expected[p['tng']][str(p['uid'])]=p
    checks=fail=things=0; seen=set()
    for name,want in expected.items():
        path=a.package/name; checks+=1
        if not path.is_file(): fail+=1; continue
        text=path.read_text(encoding='utf-8-sig'); blocks=re.findall(r'NewThing\s+.*?\r?\n(.*?)EndThing;',text,re.S)
        things+=len(blocks); checks+=1
        if len(blocks)!=len(want): fail+=1
        for block in blocks:
            f=fields(block); uid=f.get('UID'); checks+=8
            if uid not in want or uid in seen: fail+=1; continue
            seen.add(uid); p=want[uid]
            if f.get('DefinitionType')!=p['object_definition']: fail+=1
            got=[float(f[k]) for k in ('PositionX','PositionY','PositionZ')]
            if any(abs(x-y)>2e-5 for x,y in zip(got,p['tng_local_position'])): fail+=1
            yaw=math.radians(p['yaw_degrees']); fx,fy=float(f['RHSetForwardX']),float(f['RHSetForwardY'])
            if abs(fx-math.cos(yaw))>2e-5 or abs(fy-math.sin(yaw))>2e-5: fail+=1
            if f.get('Targetable') not in (None,'FALSE'): fail+=1
            if f.get('ThingGamePersistent')!='FALSE' or f.get('ThingLevelPersistent')!='FALSE': fail+=1
    checks+=2
    if things!=manifest['counts']['things'] or len(seen)!=things: fail+=1
    print(json.dumps({'checks':checks,'failures':fail,'files':len(expected),'things':things},indent=2)); return 1 if fail else 0
if __name__=='__main__': raise SystemExit(main())
