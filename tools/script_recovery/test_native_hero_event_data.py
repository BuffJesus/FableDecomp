"""Require matching native operands, nullable guard and event scheduling."""
import copy,json
from pathlib import Path
import pytest
from tools.script_recovery.native_hero_event_data import restore,PATTERN
from tools.script_recovery.lift_native_lua import RData
W=Path(__file__).resolve().parent
FIXTURE=W if (W/'restore_source.c').exists()else W/'fixtures/hero_event_data'
SOURCE=(FIXTURE/'restore_source.c').read_text(encoding='utf-8')
FN=json.loads((FIXTURE/'function.json').read_text(encoding='utf-8'))

def test_original_bytes_restore_snapshot_before_hero_event():
 out=restore(SOURCE,FN,RData())
 assert 'ENGINE_SendEntityEventFromHeroWithThingData(19, xStack_28, xStack_28);'in out
 assert 'CScriptThing::GetPThing'not in out and 'GSI->SendEntityEvent('not in out

@pytest.mark.parametrize('slot',[None,0,8,'unknown'])
def test_unrelated_unknown_slot_is_preserved(slot):
 fn=copy.deepcopy(FN);fn['indirectCalls'].append(dict(slot=slot,site='0x00000000'))
 assert restore(SOURCE,fn,RData())==restore(SOURCE,FN,RData())

@pytest.mark.parametrize('fault',['event','data','duplicate','extra_read','receiver_read','sender_read','event_depth','getter_depth','getter_slot','getter_operand','hero_depth','hero_operands','prefix','event_call','nullable_guard','no_function','no_bytes'])
def test_changed_or_ambiguous_evidence_is_preserved(fault):
 fn=copy.deepcopy(FN);source=SOURCE;rd=RData();match=next(PATTERN.finditer(source))
 site=next(c for c in fn['indirectCalls']if int(c['slot'],0)==0x3c4)
 getter=next(c for c in fn['indirectCalls']if c['site']=='0x00EEB54F')
 hero=next(c for c in fn['indirectCalls']if c['site']=='0x00EEB561')
 if fault=='event':source=source.replace('SendEntityEvent(0x13','SendEntityEvent(0x14')
 elif fault=='data':source=source.replace('xStack_28','xStack_24')
 elif fault=='duplicate':source+='\n'+match[0]
 elif fault.endswith('_read'):source+='\nuse('+match[{'extra_read':'extra','receiver_read':'receiver','sender_read':'sender'}[fault]]+');'
 elif fault=='event_depth':site['depth']+=4
 elif fault=='getter_depth':getter['depth']+=4
 elif fault=='getter_slot':getter['slot']='0x28'
 elif fault=='getter_operand':getter['ecxValue']+=4
 elif fault=='hero_depth':hero['depth']+=4
 elif fault=='hero_operands':hero['pushedStack']=[None,-36]
 elif fault=='no_function':fn=None
 elif fault=='no_bytes':rd=None
 else:
  original=rd.bytes_at
  address={'prefix':0xeeb54d,'event_call':0xeeb56d,'nullable_guard':0xeeb4ee}[fault]
  class Changed:
   def bytes_at(self,a,n):
    value=original(a,n)
    if value is not None and a<=address<a+n:
     value=bytearray(value);value[address-a]^=1;return bytes(value)
    return value
  rd=Changed()
 assert restore(source,fn,rd)==source

@pytest.mark.parametrize('style',['draft','readable'])
def test_complete_generated_main_snapshots_before_mutating_hero_lookup(style):
 from lupa.lua54 import LuaRuntime
 lua=LuaRuntime(unpack_returned_tuples=True)
 path=(W/'candidate_single'/('draft/FSE/Expression_Pickpocket/Expression_Pickpocket.lua'if style=='draft'else 'readable/Expression_Pickpocket.lua'))if FIXTURE==W else FIXTURE/(style+'.lua')
 lua.execute(path.read_text(encoding='utf-8'))
 case=dict(state=2,pickable=True,inMovie=False,chance=100,window=0,releaseAt=0,hitAt=0,switchAt=0,mutate=True,payloadZero=False,allowGetter=False)
 result=lua.execute((FIXTURE/'harness.lua').read_text(encoding='utf-8'),lua.table_from(case))
 assert len(result['packets'])==1
 assert result['packets'][1]['payload']==100
 events=list(result['events'].values());index=events.index('send')
 assert events[index-2:index]==['data1','hero']
