#!/usr/bin/env python3
from __future__ import annotations
import json
from pathlib import Path
def audit(root:Path)->dict:
 snap=json.loads((root/'ghidra_out/script_recovery/new_oakvale_quest_full_retail_bytes.json').read_text());lua=(root/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text(encoding='utf-8-sig');inv=json.loads((root/'refs/script_recovery/new_oakvale_intro/entities/Q_NewOakValeIntro.json').read_text(encoding='utf-8-sig'));e=[];r=snap.get('regions',[])
 expected={'RegisterMain':139,'OnPersist':33,'Init':3306,'Main':2036,'AddBadDeed':383,'AddGoodDeed':544,'StartBarrelTimer':562,'DoMission':1172,'WatchForGotGold':212,'AttackStuff':285,'ManageQuestCoreMarkers':944,'WatchBarrels':647,'PostAttackStuff':1095,'DeletingDestructor':30,'DestructorImplementation':450}
 if {x.get('name'):x.get('rangeSize') for x in r}!=expected:e.append('quest retail regions differ')
 if any(x.get('terminal')!='ret' for x in r[:-1]) or not r or r[-1].get('terminal')!='jmp':e.append('quest terminal topology differs')
 required=('function Main','function Init','function OnPersist','function DoMission','function AttackStuff','function PostAttackStuff','function ManageQuestCoreMarkers','function StartBarrelTimer','function WatchBarrels','function WatchForGotGold')
 if not all(x in lua for x in required):e.append('Lua quest dispatcher differs')
 if len(inv.get('operations',[]))!=59:e.append('inventory operation count differs')
 if not any(x.get('name')=='fullQuestLifecycle' for x in inv.get('retailByteSnapshots',[])):e.append('inventory full quest snapshot citation missing')
 return {'schema':'new-oakvale-quest-full-byte-audit/0.1','ok':not e,'retailBytes':sum(x.get('rangeSize',0) for x in r),'instructionCount':sum(x.get('instructionCount',0) for x in r),'regions':len(r),'errors':e}
if __name__=='__main__':print(json.dumps(audit(Path(__file__).resolve().parents[2]),indent=2))
