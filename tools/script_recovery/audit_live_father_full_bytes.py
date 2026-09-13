#!/usr/bin/env python3
from __future__ import annotations
import json
from pathlib import Path
def audit(root:Path)->dict:
 snap=json.loads((root/'ghidra_out/script_recovery/live_father_full_retail_bytes.json').read_text());lua=(root/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_LiveFather.lua').read_text(encoding='utf-8-sig');inv=json.loads((root/'refs/script_recovery/new_oakvale_intro/entities/NOVI_LiveFather.json').read_text(encoding='utf-8-sig'));e=[];r=snap.get('regions',[])
 if [(x.get('name'),x.get('rangeSize'),x.get('terminal')) for x in r]!=[('Init',141,'ret'),('Main',4325,'ret')]:e.append('retail regions differ')
 required=('DadFinishedIntro','GoodDeedsPerformed','BadDeedsPerformed','GUIGoodDeedCounter','OBJECT_CHOCOLATE_BOX_UNGIVEABLE','CS_OAKVALE_INTRO_FATHER','Deeds.add_bad')
 if not all(x in lua for x in required):e.append('Lua LiveFather dispatcher differs')
 if len(inv.get('operations',[]))!=56:e.append('inventory operation count differs')
 if not any(x.get('name')=='fullInitMain' for x in inv.get('retailByteSnapshots',[])):e.append('inventory snapshot citation missing')
 return {'schema':'live-father-full-byte-audit/0.1','ok':not e,'retailBytes':sum(x.get('rangeSize',0) for x in r),'instructionCount':sum(x.get('instructionCount',0) for x in r),'errors':e}
if __name__=='__main__':print(json.dumps(audit(Path(__file__).resolve().parents[2]),indent=2))
