#!/usr/bin/env python3
from __future__ import annotations
import json
from pathlib import Path
def audit(root:Path)->dict:
 snap=json.loads((root/'ghidra_out/script_recovery/affair_wife_full_retail_bytes.json').read_text());lua=(root/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_AffairWife.lua').read_text(encoding='utf-8-sig');inv=json.loads((root/'refs/script_recovery/new_oakvale_intro/entities/NOVI_AffairWife.json').read_text(encoding='utf-8-sig'));e=[];r=snap.get('regions',[])
 expected=[('Init',160,'ret'),('Main',4890,'ret')]
 if [(x.get('name'),x.get('rangeSize'),x.get('terminal')) for x in r]!=expected:e.append('retail regions differ')
 required=('start_running_to_husband','wait_until_husband_reached','argue_with_husband','NOVI_AffairMan','RUN_TO_HUSBAND_RADIUS','HUSBAND_REACHED_DISTANCE','TEXT_RUNNING_TO_HUBBY','TEXT_MAN_IN_TROUBLE')
 if not all(x in lua for x in required):e.append('Lua Wife dispatcher differs')
 if len(inv.get('operations',[]))!=62:e.append('inventory operation count differs')
 if not any(x.get('name')=='fullInitMain' for x in inv.get('retailByteSnapshots',[])):e.append('inventory snapshot citation missing')
 return {'schema':'affair-wife-full-byte-audit/0.1','ok':not e,'retailBytes':sum(x.get('rangeSize',0) for x in r),'instructionCount':sum(x.get('instructionCount',0) for x in r),'errors':e}
if __name__=='__main__':print(json.dumps(audit(Path(__file__).resolve().parents[2]),indent=2))
