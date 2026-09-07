#!/usr/bin/env python3
"""Assemble verified scene components into a non-installed stage directory."""
from __future__ import annotations
import argparse, hashlib, json, shutil
from pathlib import Path

def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest().upper()
def main():
    ap=argparse.ArgumentParser(); ap.add_argument('graphics',type=Path); ap.add_argument('defs_root',type=Path); ap.add_argument('wad',type=Path); ap.add_argument('out',type=Path)
    ap.add_argument('--textures',type=Path); ap.add_argument('--stb',type=Path)
    ap.add_argument('--baseline-wld',type=Path); ap.add_argument('--baseline-bwd',type=Path)
    a=ap.parse_args()
    if bool(a.baseline_wld) != bool(a.baseline_bwd):
        raise ValueError('--baseline-wld and --baseline-bwd must be supplied together')
    sources={'data/graphics/graphics.big':a.graphics,
             'data/CompiledDefs/game.bin':a.defs_root/'data/CompiledDefs/game.bin',
             'data/CompiledDefs/names.bin':a.defs_root/'data/CompiledDefs/names.bin',
             'data/Levels/FinalAlbion.wad':a.wad}
    if a.textures: sources['data/graphics/pc/textures.big']=a.textures
    if a.stb: sources['data/Levels/FinalAlbion_RT.stb']=a.stb
    rows=[]
    for rel,src in sources.items():
        if not src.is_file(): raise FileNotFoundError(src)
        dst=a.out/Path(rel); dst.parent.mkdir(parents=True,exist_ok=True); shutil.copyfile(src,dst)
        rows.append({'path':rel,'bytes':dst.stat().st_size,'sha256':sha(dst),'source':str(src.resolve())})
    baselines=[]
    for rel,src in (('data/Levels/FinalAlbion.wld',a.baseline_wld),
                    ('data/Levels/FinalAlbion.bwd',a.baseline_bwd)):
        if src:
            if not src.is_file(): raise FileNotFoundError(src)
            baselines.append({'path':rel,'bytes':src.stat().st_size,'sha256':sha(src)})
    complete=bool(a.textures and a.stb and baselines)
    m={'schema':('fableforge.f2scene_coordinated_stage.v1' if complete else
                 'fableforge.f2scene_architecture_stage.v1'),'installed':False,
       'scope':('six-file atomic textured architecture + terrain overlay' if complete else
                f'{len(rows)}-file architecture overlay; runtime files not changed'),
       'requiredBaseline':baselines,'files':rows}
    (a.out/'stage_manifest.json').write_text(json.dumps(m,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'files':len(rows),'bytes':sum(x['bytes'] for x in rows)},indent=2)); return 0
if __name__=='__main__': raise SystemExit(main())
