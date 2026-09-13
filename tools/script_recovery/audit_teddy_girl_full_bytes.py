#!/usr/bin/env python3
from __future__ import annotations
import json
from pathlib import Path
def audit(root:Path)->dict:
 snap=json.loads((root/'ghidra_out/script_recovery/teddy_girl_full_retail_bytes.json').read_text());lua=(root/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_TeddyGirl.lua').read_text(encoding='utf-8-sig');inv=json.loads((root/'refs/script_recovery/new_oakvale_intro/entities/NOVI_TeddyGirl.json').read_text(encoding='utf-8-sig'));e=[];r=snap.get('regions',[])
 if [(x.get('name'),x.get('rangeSize'),x.get('terminal')) for x in r]!=[('Init',82,'ret'),('Main',5504,'ret'),('GivenTeddy',91,'ret')]:e.append('retail regions differ')
 required=('given_teddy','Deeds.add_good','Deeds.add_bad','TeddySolution','"B"','"C"','RemoveThing(me, false, true)','TakeObjectFromHero')
 if not all(x in lua for x in required):e.append('Lua TeddyGirl dispatcher differs')
 if len(inv.get('operations',[]))!=51:e.append('inventory operation count differs')
 if not any(x.get('name')=='fullDispatcher' for x in inv.get('retailByteSnapshots',[])):e.append('inventory snapshot citation missing')
 return {'schema':'teddy-girl-full-byte-audit/0.1','ok':not e,'retailBytes':sum(x.get('rangeSize',0) for x in r),'instructionCount':sum(x.get('instructionCount',0) for x in r),'errors':e}
if __name__=='__main__':print(json.dumps(audit(Path(__file__).resolve().parents[2]),indent=2))
