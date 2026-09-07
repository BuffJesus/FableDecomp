#!/usr/bin/env python3
"""Audit transformed TLC architecture bounds against the imported terrain."""
from __future__ import annotations
import argparse, json, math, sys
from pathlib import Path

def main():
    ap=argparse.ArgumentParser(); ap.add_argument('mesh_package',type=Path); ap.add_argument('placements',type=Path); ap.add_argument('routing',type=Path); ap.add_argument('out',type=Path); a=ap.parse_args()
    sys.path.insert(0,str(Path(__file__).resolve().parent/'blender_addon/io_scene_fable')); import fable_core as fc
    mm=json.loads((a.mesh_package/'mesh_manifest.json').read_text()); pp=json.loads(a.placements.read_text()); routing=json.loads(a.routing.read_text())
    meshes={}
    for row in mm['meshes']:
        if row['status']!='compiled': continue
        payload=(a.mesh_package/row['payload']).read_bytes(); _n,prims,_b=fc._parse_mesh_primitives(payload,1)
        meshes[row['mesh_name']]=[v for p in prims for v in fc._prim_geometry(p,1)[0]]
    tiles={x['level_name']:x for x in routing['tiles']}; rows=[]; outside=extreme=0
    for p in pp['placements']:
        verts=meshes[p['mesh_name']]; ang=math.radians(p['yaw_degrees']); c,s=math.cos(ang),math.sin(ang); lx,ly,lz=p['tng_local_position']; tile=tiles[p['level_name']]
        world=[]
        for x,y,z in verts:
            wx=tile['world_origin'][0]+lx+c*x-s*y; wy=tile['world_origin'][1]+ly+s*x+c*y; wz=lz+z; world.append((wx,wy,wz))
        lo=[min(v[i] for v in world) for i in range(3)]; hi=[max(v[i] for v in world) for i in range(3)]
        terrain=[routing['deployment_translation'][0],routing['deployment_translation'][1],
                 routing['deployment_translation'][0]+routing['tile_plan']['source_extent'][0],
                 routing['deployment_translation'][1]+routing['tile_plan']['source_extent'][1]]
        is_out=hi[0]<terrain[0] or lo[0]>=terrain[2] or hi[1]<terrain[1] or lo[1]>=terrain[3]
        is_ext=max(hi[i]-lo[i] for i in range(3))>128 or lo[2]<-128 or hi[2]>256
        outside+=is_out; extreme+=is_ext
        rows.append({'instance_id':p['instance_id'],'level_name':p['level_name'],'mesh_name':p['mesh_name'],'world_aabb_min':lo,'world_aabb_max':hi,'entirely_outside_terrain':is_out,'extreme_bounds':is_ext})
    all_lo=[min(r['world_aabb_min'][i] for r in rows) for i in range(3)]; all_hi=[max(r['world_aabb_max'][i] for r in rows) for i in range(3)]
    out={'schema':'fableforge.f2scene_architecture_bounds_audit.v1','counts':{'instances':len(rows),'entirely_outside_terrain':outside,'extreme_bounds':extreme},'aggregate_world_aabb':{'min':all_lo,'max':all_hi},'instances':rows}
    a.out.write_text(json.dumps(out,indent=2)+'\n',encoding='utf-8'); print(json.dumps({'counts':out['counts'],'aggregate_world_aabb':out['aggregate_world_aabb']},indent=2)); return 1 if outside or extreme else 0
if __name__=='__main__': raise SystemExit(main())
