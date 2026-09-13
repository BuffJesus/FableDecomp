#!/usr/bin/env python3
"""Audit Barrel Man outcome keys and localized lines against reconstructed Lua."""
from __future__ import annotations
import json
from pathlib import Path

EXPECTED={
 'TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN':[
  'A fat lot of good you were.',
  'Now my stock is damaged! And to think I trusted you with it.',
  "If I find out it was you, well... you better hope I don't!"],
 'TEXT_QST_048_BARRELMAN_THANKS':[
  "Excellent! Thanks, lad. You've done me a big favour.",
  "I'll let your dad know what a splendid watchman you made."],
}

def audit(workspace:Path)->dict:
 snapshot=json.loads((workspace/'ghidra_out/script_recovery/barrel_man_text_assets.json').read_text(encoding='utf-8-sig'))
 lua=(workspace/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BarrelMan.lua').read_text(encoding='utf-8-sig')
 errors=[]
 for group,lines in EXPECTED.items():
  actual=[entry['text'] for entry in snapshot.get('groups',{}).get(group,{}).get('members',[])]
  if actual!=lines:errors.append(f'{group} member order/text differs')
 if 'local TEXT_LETDOWN_BROKEN      = "TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN"' not in lua:
  errors.append('Lua broken-stock group key differs')
 broken=lua.find('F.get(quest, F.BarrelBrokenPersistent)')
 rebuke=lua.find('speak_to_hero(quest, me, TEXT_LETDOWN_BROKEN)',broken)
 thanks=lua.find('speak_to_hero(quest, me, TEXT_THANKS)',broken)
 if broken<0 or rebuke<broken or (thanks>=0 and thanks<rebuke):
  errors.append('Lua does not select the broken-stock group before any thanks path')
 return {'schema':'new-oakvale-barrel-man-text-assets-audit/0.1','groupsChecked':len(EXPECTED),
         'errors':errors,'ok':not errors}

if __name__=='__main__':print(json.dumps(audit(Path(__file__).resolve().parents[2]),indent=2))
