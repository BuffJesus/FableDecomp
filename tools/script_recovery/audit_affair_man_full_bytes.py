#!/usr/bin/env python3
"""Audit complete Affair Man retail bytes, Lua surface, and inventory citation."""
from __future__ import annotations
import json
from pathlib import Path

def audit(root: Path) -> dict:
    snap=json.loads((root/'ghidra_out/script_recovery/affair_man_full_retail_bytes.json').read_text(encoding='utf-8'))
    lua=(root/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_AffairMan.lua').read_text(encoding='utf-8-sig')
    inv=json.loads((root/'refs/script_recovery/new_oakvale_intro/entities/NOVI_AffairMan.json').read_text(encoding='utf-8-sig'))
    errors=[]; regions=snap.get('regions',[])
    if [(r.get('name'),r.get('rangeSize'),r.get('terminal')) for r in regions] != [('Init',144,'ret'),('Main',5058,'ret')]: errors.append('retail region boundaries differ')
    required=('NOVI_AffairWoman','NOVI_AffairWife','confronted_with_wife','ask_hero_to_keep_quiet',
              'walk_home','affair_at_home','near_conversation','ranged_conversation','kiss_or_hug',
              'F.set(quest, F.HeroDiscoveredInfidelity, true)')
    if not all(x in lua for x in required): errors.append('Lua Affair Man dispatcher differs')
    if len(inv.get('operations',[])) != 67: errors.append('Affair Man inventory operation count differs')
    if not any(x.get('name')=='fullInitMain' and x.get('file')=='ghidra_out/script_recovery/affair_man_full_retail_bytes.json'
               for x in inv.get('retailByteSnapshots',[])): errors.append('inventory lacks full byte snapshot citation')
    return {'schema':'affair-man-full-byte-audit/0.1','ok':not errors,
            'retailBytes':sum(r.get('rangeSize',0) for r in regions),
            'instructionCount':sum(r.get('instructionCount',0) for r in regions),'errors':errors}
if __name__=='__main__': print(json.dumps(audit(Path(__file__).resolve().parents[2]),indent=2))
