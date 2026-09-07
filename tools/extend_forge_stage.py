#!/usr/bin/env python3
"""Safely extend an existing forge stage with a manifest-backed overlay.

Existing .forgebak files remain the rollback authority. New targets are backed
up once, and the forge_stage_manifest is extended so `forge unstage` restores
the complete pre-stage state. Copies go through same-directory temporary files.
"""
from __future__ import annotations
import argparse, hashlib, json, os, shutil
from pathlib import Path

def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest().upper()
def main():
    ap=argparse.ArgumentParser(); ap.add_argument('game_root',type=Path); ap.add_argument('overlay',type=Path); a=ap.parse_args()
    stage_path=a.game_root/'forge_stage_manifest.json'; overlay_path=a.overlay/'stage_manifest.json'
    stage=json.loads(stage_path.read_text(encoding='utf-8')); overlay=json.loads(overlay_path.read_text(encoding='utf-8'))
    known={x['path']:x for x in stage['files']}; results=[]
    for row in overlay['files']:
        rel=row['path']; src=a.overlay/Path(rel); dst=a.game_root/Path(rel); bak=Path(str(dst)+'.forgebak')
        if sha(src)!=row['sha256'] or src.stat().st_size!=row['bytes']: raise ValueError(f'overlay hash mismatch: {rel}')
        had=dst.exists()
        if rel not in known:
            if had and not bak.exists(): shutil.copyfile(dst,bak)
            known[rel]={'path':rel,'had_original':had}; stage['files'].append(known[rel])
        elif known[rel]['had_original'] and not bak.exists():
            raise ValueError(f'existing staged target lacks rollback backup: {rel}')
        dst.parent.mkdir(parents=True,exist_ok=True); tmp=dst.with_name(dst.name+'.architecture-stage-tmp')
        shutil.copyfile(src,tmp); os.replace(tmp,dst)
        if sha(dst)!=row['sha256']: raise ValueError(f'post-copy hash mismatch: {rel}')
        results.append({'path':rel,'sha256':row['sha256'],'rollback_backup':str(bak) if had else None})
    tmp=stage_path.with_suffix('.json.tmp'); tmp.write_text(json.dumps(stage,indent=2)+'\n',encoding='utf-8'); os.replace(tmp,stage_path)
    receipt={'schema':'fableforge.extended_stage_receipt.v1','overlay':str(a.overlay.resolve()),'files':results,'rollback':'forge unstage <game-root>'}
    receipt_path=a.overlay/'runtime_stage_receipt.json'; receipt_path.write_text(json.dumps(receipt,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'staged':len(results),'manifest_entries':len(stage['files']),'receipt':str(receipt_path)},indent=2)); return 0
if __name__=='__main__': raise SystemExit(main())
