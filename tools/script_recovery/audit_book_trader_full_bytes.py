#!/usr/bin/env python3
from __future__ import annotations
import json
from pathlib import Path
def audit(root:Path)->dict:
 snap=json.loads((root/'ghidra_out/script_recovery/book_trader_full_retail_bytes.json').read_text());lua=(root/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BookTrader.lua').read_text(encoding='utf-8-sig');inv=json.loads((root/'refs/script_recovery/new_oakvale_intro/entities/NOVI_BookTrader.json').read_text(encoding='utf-8-sig'));e=[];r=snap.get('regions',[])
 if [(x.get('name'),x.get('rangeSize'),x.get('terminal')) for x in r]!=[('Init',152,'ret'),('Main',4042,'ret')]:e.append('retail regions differ')
 required=('return_home','react_to_hit','sell_sweets','maybe_shout','NOVI_Theresa','OBJECT_CHOCOLATE_BOX_UNGIVEABLE','local SWEETS_PRICE','TEXT_QST_048_TRADER_ROLL_UP')
 if not all(x in lua for x in required):e.append('Lua trader dispatcher differs')
 if len(inv.get('operations',[]))!=31:e.append('inventory operation count differs')
 if not any(x.get('name')=='fullInitMain' for x in inv.get('retailByteSnapshots',[])):e.append('inventory snapshot citation missing')
 return {'schema':'book-trader-full-byte-audit/0.1','ok':not e,'retailBytes':sum(x.get('rangeSize',0) for x in r),'instructionCount':sum(x.get('instructionCount',0) for x in r),'errors':e}
if __name__=='__main__':print(json.dumps(audit(Path(__file__).resolve().parents[2]),indent=2))
