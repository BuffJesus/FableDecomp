#!/usr/bin/env python3
"""Join routed F2 scene instances to compiled TLC mesh/object identities."""
from __future__ import annotations
import argparse, hashlib, json, math, re
from collections import Counter
from pathlib import Path

ENGINE_ID = re.compile(r"^engine-(\d+)-")

def sha(path): return hashlib.sha256(path.read_bytes()).hexdigest().upper()

def main():
    ap=argparse.ArgumentParser(); ap.add_argument('routing',type=Path); ap.add_argument('instances',type=Path); ap.add_argument('graphics_manifest',type=Path); ap.add_argument('out',type=Path); a=ap.parse_args()
    routing=json.loads(a.routing.read_text(encoding='utf-8'))
    authoritative=json.loads(a.instances.read_text(encoding='utf-8'))
    transforms={x['instance_id']:x for x in authoritative['instances']}
    graphics=json.loads(a.graphics_manifest.read_text(encoding='utf-8'))
    models={int(x['block']):x for x in graphics['assigned_models']}
    rows=[]; skipped=Counter(); tiles=Counter()
    for src in routing['routed_instances']:
        m=ENGINE_ID.match(src['instance_id'])
        if not m: skipped['not_engine_instance']+=1; continue
        block=int(m.group(1))
        if block not in models: skipped['no_compiled_architecture_model']+=1; continue
        model=models[block]; local=src['tng_local_position']
        if not all(math.isfinite(float(v)) for v in local): raise ValueError(src['instance_id'])
        transform=transforms[src['instance_id']]
        yaw=float(transform['rotation']['decoded_yaw_radians'])
        scale=[float(v) for v in transform['scale']]
        unit_scale=all(abs(v-1.0)<1e-7 for v in scale)
        row={'instance_id':src['instance_id'],'block':block,'level_name':src['level_name'],
             'tng_local_position':local,'object_definition':f"OBJECT_F2_BWS_BLOCK_{block:04d}",
             'mesh_name':model['mesh_name'],'model_id':model['model_id'],
             'yaw_radians':yaw,'yaw_degrees':math.degrees(yaw),'scale':scale,
             'rotation_status':'decoded_authoritative_yaw',
             'scale_status':'direct' if unit_scale else 'tng_emitter_support_unresolved'}
        rows.append(row); tiles[src['level_name']]+=1
    out={'schema':'fableforge.f2scene_tlc_object_placement_plan.v1','dry_run':True,
         'sources':{'routing':str(a.routing.resolve()),'routing_sha256':sha(a.routing),
                    'instances':str(a.instances.resolve()),'instances_sha256':sha(a.instances),
                    'graphics_manifest':str(a.graphics_manifest.resolve()),'graphics_manifest_sha256':sha(a.graphics_manifest)},
         'counts':{'placements':len(rows),'by_tile':dict(sorted(tiles.items())),'skipped':dict(skipped)},
         'tng_mutation':'none','placements':rows}
    a.out.parent.mkdir(parents=True,exist_ok=True); a.out.write_text(json.dumps(out,indent=2)+'\n',encoding='utf-8'); print(json.dumps(out['counts'],indent=2)); return 0
if __name__=='__main__': raise SystemExit(main())
