#!/usr/bin/env python3
from __future__ import annotations
import json
from pathlib import Path
def audit(root:Path)->dict:
 snap=json.loads((root/'ghidra_out/script_recovery/affair_woman_full_retail_bytes.json').read_text());
 lua=(root/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_AffairWoman.lua').read_text(encoding='utf-8-sig')
 inv=json.loads((root/'refs/script_recovery/new_oakvale_intro/entities/NOVI_AffairWoman.json').read_text(encoding='utf-8-sig')); errors=[];r=snap.get('regions',[])
 if [(x.get('name'),x.get('rangeSize'),x.get('terminal')) for x in r] != [('Init',116,'ret'),('Main',2714,'ret')]:errors.append('retail regions differ')
 required=('NOVI_AffairMan','NOVI_AffairWife','AffairWomanRunOffPoint','react_to_hit','talked_to','walk_home','run_off','quest:RemoveThing(me, false, true)')
 if not all(x in lua for x in required):errors.append('Lua dispatcher differs')
 if len(inv.get('operations',[]))!=42:errors.append('inventory operation count differs')
 if not any(x.get('name')=='fullInitMain' for x in inv.get('retailByteSnapshots',[])):errors.append('inventory snapshot citation missing')
 return {'schema':'affair-woman-full-byte-audit/0.1','ok':not errors,'retailBytes':sum(x.get('rangeSize',0) for x in r),
         'instructionCount':sum(x.get('instructionCount',0) for x in r),'errors':errors}
if __name__=='__main__':print(json.dumps(audit(Path(__file__).resolve().parents[2]),indent=2))
