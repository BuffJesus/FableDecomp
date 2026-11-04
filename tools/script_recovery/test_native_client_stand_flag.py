"""The female stand byte is one boolean lifetime at all three native readers."""
from copy import deepcopy
import json,re
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_stack_aliases import restore_lua
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');fn=c.native('0x00e44ea0');row=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedLuaScalarAliases']['0x00e44ea0'];source=(R/'tools/script_recovery/fixtures/scalar_lifetimes/client_timer_input.lua').read_text()
 return source,fn,row,c.rdata
def test_only_the_two_lost_alias_conditions_and_shared_flag_name_change(evidence):
 source,fn,row,rd=evidence;old=deepcopy(row);old.pop('additionalAliases');old['edits']=old['edits'][:-2];old['editCounts']=old['editCounts'][:-2]
 before=restore_lua(source,fn,old,rd);out=restore_lua(source,fn,row,rd)
 assert before.count('xStack_e0_b3')==4 and before.count('xStack_e8_2_b3 ~= 0')==2
 assert out==before.replace('xStack_e0_b3','clientAtStand').replace('xStack_e8_2_b3 ~= 0','clientAtStand')
 assert out.count('if clientAtStand then')==2 and out.count('if not clientAtStand then')==1
 assert out.count('quest:DeregisterTimer(registeredTimerID)')==3
 assert all(out.count(':'+name+'(')==before.count(':'+name+'(') for name in ['Speak','PrepareResource','MoveToPosition','GetHero','DestroyMovie','ReleaseResource'])
def test_original_stack_byte_width_and_zero_one_producers(evidence):
 source,fn,row,rd=evidence;a,b=int(fn['address'],16),int(fn['bodyEndExclusive'],16);decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;ins=list(decoder.disasm(rd.bytes_at(a,b-a),a));by={i.address:i for i in ins}
 for site in [0xe4503e,0xe450c5]:
  i=by[site];op=i.operands;assert i.mnemonic=='mov' and op[0].type==x86.X86_OP_MEM and op[0].size==1 and op[0].mem.base==x86.X86_REG_ESP and op[0].mem.disp==0x43 and not op[0].mem.index
 assert by[0xe4503e].op_str=='byte ptr [esp + 0x43], al' and by[0xe450c5].op_str=='byte ptr [esp + 0x43], 1'
 assert by[0xe45034].op_str=='al, al' and by[0xe45036].mnemonic=='jne' and by[0xe45036].op_str=='0xe469cb'
 for site in [0xe45050,0xe452f7,0xe455f3]:
  i=by[site];assert i.mnemonic=='mov' and i.op_str=='al, byte ptr [esp + 0x43]' and i.operands[1].size==1
 assert row['standByteEvidence']['frameOffset']==67 and row['standByteEvidence']['booleanProducers']==[False,True]
def test_boolean_false_skips_both_post_dialogue_resets(evidence):
 source,fn,row,rd=evidence;out=restore_lua(source,fn,row,rd);lua=LuaRuntime()
 conditions=re.findall(r'(?m)^\s*if (clientAtStand) then$',out);assert len(conditions)==2
 for arrived in [False,True]:
  for condition in conditions:
   actual=lua.execute('return function(clientAtStand) if '+condition+' then return 1 else return 0 end end')(arrived)
   original=int(arrived);assert actual==original
   old=lua.execute('local unassigned;return unassigned ~= 0');assert old is True
   if not arrived:assert old!=bool(actual)
 assert out.count('clientAtStand = bVar4')==1 and out.count('clientAtStand = true')==1
@pytest.mark.parametrize('fault',['initialWitness','arrivalWitness','movementWitness','chatWitness','hitWitness','duplicateAlias','existingAlias','missingInitialName','missingCondition','badInitialCount','badConditionCount','missingPhase'])
def test_stale_or_ambiguous_stand_review_fails_closed(evidence,fault):
 source,fn,row,rd=evidence;row=deepcopy(row)
 if fault.endswith('Witness'):
  site={'initialWitness':'0xe4503e','arrivalWitness':'0xe450c5','movementWitness':'0xe45050','chatWitness':'0xe452f7','hitWitness':'0xe455f3'}[fault];row['nativeInstructions'][site]='00'
 elif fault=='duplicateAlias':row['additionalAliases'].append('clientAtStand')
 elif fault=='existingAlias':row['additionalAliases']=['bVar4']
 elif fault=='missingInitialName':row['edits'][-2][0]='missing_initial'
 elif fault=='missingCondition':row['edits'][-1][0]='missing_condition'
 elif fault=='badInitialCount':row['editCounts'][-2]+=1
 elif fault=='badConditionCount':row['editCounts'][-1]+=1
 elif fault=='missingPhase':row.pop('textSha256')
 with pytest.raises(ValueError):restore_lua(source,fn,row,rd)
