#!/usr/bin/env python3
from __future__ import annotations
import json
from pathlib import Path
def audit(root:Path)->dict:
 snap=json.loads((root/'ghidra_out/script_recovery/dead_father_full_retail_bytes.json').read_text());lua=(root/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/OVI_DeadFather.lua').read_text(encoding='utf-8-sig');inv=json.loads((root/'refs/script_recovery/new_oakvale_intro/entities/OVI_DeadFather.json').read_text(encoding='utf-8-sig'));e=[];r=snap.get('regions',[])
 if [(x.get('name'),x.get('rangeSize'),x.get('terminal')) for x in r]!=[('OnPredicateFail',1,'ret'),('Init',100,'ret'),('Main',534,'ret')]:e.append('retail regions differ')
 required=('DadFound','HUD_ORB_QUEST_CORE','MK_OVID_DAD','CS_DEAD_DAD','MiniMapRemoveMarker','NOVI.release')
 if not all(x in lua for x in required):e.append('Lua DeadFather lifecycle differs')
 if len(inv.get('operations',[]))!=11:e.append('inventory operation count differs')
 if not any(x.get('name')=='fullLifecycle' for x in inv.get('retailByteSnapshots',[])):e.append('inventory complete snapshot citation missing')
 return {'schema':'dead-father-full-byte-audit/0.1','ok':not e,'retailBytes':sum(x.get('rangeSize',0) for x in r),'instructionCount':sum(x.get('instructionCount',0) for x in r),'errors':e}
if __name__=='__main__':print(json.dumps(audit(Path(__file__).resolve().parents[2]),indent=2))
