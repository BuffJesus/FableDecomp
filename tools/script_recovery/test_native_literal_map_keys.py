import copy,json,re
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
@pytest.fixture(scope='module')
def captured(tmp_path_factory):
 from tools.script_recovery import native_reviewed_literal_map_keys as adapter
 from tools.script_recovery.convert_quest_unit import UnitConverter
 from tools.script_recovery.build_readable_unit import readable_file
 converter=UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text());result={};original=adapter.restore
 class Done(Exception):pass
 def capture(source,todo,fn,unit,rd):
  generated,remaining=original(source,todo,fn,unit,rd)
  if fn['address'].lower()=='0x00e64fb0':
   result.update(source=source,todo=todo,fn=fn,unit=unit,rd=rd,draft=generated,readable=readable_file(generated)[0],remaining=remaining);raise Done()
  return generated,remaining
 with patch.object(converter,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):
  with pytest.raises(Done):converter.convert(unit,tmp_path_factory.mktemp('map_keys'))
 return result

@pytest.mark.parametrize('fault',['native','source','lua','inventory','window','slot','keyAddress','key','valueAddress','value','memberOffset','mapRegister','keyPush','constructorLea','constructorCall','valuePush','mapLea','mapPush','mapOwner','mapOffset','mapCall','assignReceiver','assignCall','destructorLea','destructorCall','mapName','staleAlias','valueLocal'])
def test_stale_or_ambiguous_evidence_rejected(captured,fault):
 from tools.script_recovery.native_reviewed_literal_map_keys import restore
 c=captured;unit=copy.deepcopy(c['unit']);fn=copy.deepcopy(c['fn']);source=c['source'];row=unit['reviewedLiteralMapKeys']['0x00e64fb0'];site=row['sites'][0]
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='lua':source+='\n'
 elif fault=='inventory':row['sites'].pop()
 elif fault=='window':site['sha256']='0'*64
 elif fault=='slot':site['stackOffset']+=4
 elif fault in ['keyAddress','valueAddress','memberOffset']:site[fault]+=4
 elif fault in ['key','value']:site[fault]+='_wrong'
 elif fault=='mapRegister':site[fault]='edx'
 elif fault in ['mapName','staleAlias','valueLocal']:row[fault]='other'
 else:site[fault]=site['start']
 with pytest.raises(ValueError):restore(source,c['todo'],fn,unit,c['rd'])

def test_unreviewed_body_unchanged(captured):
 from tools.script_recovery.native_reviewed_literal_map_keys import restore
 c=captured;unit=copy.deepcopy(c['unit']);unit.pop('reviewedLiteralMapKeys')
 assert restore(c['source'],c['todo'],c['fn'],unit,c['rd'])==(c['source'],c['todo'])

def test_only_three_reviewed_keys_change(captured):
 c=captured;before=c['source'].splitlines();after=c['draft'].splitlines();assert len(before)==len(after)
 changes=[(a.strip(),b.strip()) for a,b in zip(before,after) if a!=b]
 old='resources:SetString(resources:MemberStringMap("csargs"), pCVar14, pcVar20)'
 assert changes==[(old,old.replace('pCVar14',json.dumps(key))) for key in ['$LOOP','$LINE','$LINE']]
 assert c['todo']==c['remaining']

@pytest.mark.parametrize('style',['draft','readable'])
def test_emitted_map_keys_ignore_stale_result(captured,style):
 text=captured[style];values=['CHICK_KICK_LOW_LOOP','TEXT_QST_B17_MASTER_NO_PRIZE_AGAIN_HIGH','TEXT_QST_B17_MASTER_NO_PRIZE_AGAIN']
 calls=[]
 for value in values:
  lines=text.splitlines();i=next(i for i,s in enumerate(lines) if '"'+value+'"' in s)
  calls.append(lines[i] if 'resources:SetString(' in lines[i] else '\n'.join(lines[i:i+2]))
 for stale in ['nil','"OldQuest"']:
  run=LuaRuntime(unpack_returned_tuples=True).eval('''function()
   local pCVar14='''+stale+''';local getActiveQuestName2=pCVar14;local pcVar20;local writes={}
   local resources={MemberStringMap=function(_,key) assert(key=='csargs');return 55 end,
    SetString=function(_,map,key,value) assert(map==55 and type(key)=='string');writes[#writes+1]=key..'='..value end}
   '''+'\n'.join(calls)+'''
   return table.concat(writes,',') end''')
  assert run()==','.join(key+'='+value for key,value in zip(['$LOOP','$LINE','$LINE'],values))
