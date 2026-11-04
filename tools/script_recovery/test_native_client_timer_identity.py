"""The registered timer outlives the register names later used for script state."""
from copy import deepcopy
import hashlib,itertools,json,re
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_stack_aliases import restore_lua
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');fn=c.native('0x00e44ea0');unit=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text());row=unit['reviewedLuaScalarAliases']['0x00e44ea0']
 return (R/'tools/script_recovery/fixtures/scalar_lifetimes/client_timer_input.lua').read_text(),fn,row,c.rdata
def test_exact_timer_separation_without_changing_other_register_values(evidence):
 source,fn,row,rd=evidence;expected=source
 for a,b in row['edits']:expected=expected.replace(a,b)
 out=restore_lua(source,fn,row,rd);assert out==expected
 assert out.count('iVar6 = ')==source.count('iVar6 = ')-1
 assert out.count('quest:DeregisterTimer(registeredTimerID)')==3
 assert source.count('me:Speak(')==out.count('me:Speak(')==9
 assert restore_lua(source,fn,None,rd)==source
@pytest.mark.parametrize('fault',['nativeSha256','sourceSha256','textSha256','missingPhase','nativeInstruction','aliasUsed','aliasDuplicate','missingEdit','repeatedEdit','badCount','missingCounts','countType','countLength','countBool','emptyDependencies','missingDependencies','dependencyAddress','dependencyBytes','duplicateDependency'])
def test_changed_alias_certificate_fails_closed(evidence,fault):
 source,fn,row,rd=evidence;row=deepcopy(row)
 if fault in ['nativeSha256','sourceSha256','textSha256']:row[fault]='0'*64
 elif fault=='missingPhase':row.pop('textSha256')
 elif fault=='nativeInstruction':row['nativeInstructions'][next(iter(row['nativeInstructions']))]='00'
 elif fault=='aliasUsed':row['alias']='iVar6'
 elif fault=='aliasDuplicate':row['additionalAliases']=[row['alias']]
 elif fault=='missingEdit':row['edits'][0][0]='missing'
 elif fault=='repeatedEdit':row['edits'].append(row['edits'][0]);row['editCounts'].append(1)
 elif fault=='badCount':row['editCounts'][4]+=1
 elif fault=='missingCounts':row.pop('editCounts')
 elif fault=='countType':row['editCounts']='wrong'
 elif fault=='countLength':row['editCounts'].pop()
 elif fault=='countBool':row['editCounts'][0]=True
 elif fault=='emptyDependencies':row['nativeDependencies']=[]
 elif fault=='missingDependencies':row.pop('nativeDependencies')
 elif fault=='dependencyAddress':row['nativeDependencies'][0]['address']='0xcd4471'
 elif fault=='dependencyBytes':row['nativeDependencies'][0]['bytes']='00'*20
 elif fault=='duplicateDependency':row['nativeDependencies'].append(deepcopy(row['nativeDependencies'][0]))
 with pytest.raises(ValueError):restore_lua(source,fn,row,rd)
def test_lua_cleanup_keeps_registered_id_after_state_task_and_hero_reuse(evidence):
 source,fn,row,rd=evidence;out=restore_lua(source,fn,row,rd);lua=LuaRuntime();cases=0
 declaration=re.search(r'(?m)^    local [\w, ]+$',out)[0]
 register=re.search(r'(?m)^\s*registeredTimerID = quest:RegisterTimer\(\)$',out)[0]
 cleanups=[m[0].strip() for m in re.finditer(r'quest:DeregisterTimer\(registeredTimerID\)',out)]
 for ident,kind,cleanup in itertools.product([-1,0,1,42,2147483647],['state','task','hero'],cleanups):
  run=lua.execute('return function(quest,reused)\n'+declaration+'\n'+register+'\niVar6 = reused\n'+cleanup+'\nend')
  values=[];q=lua.table_from({'RegisterTimer':lambda _:ident,'DeregisterTimer':lambda _,timer:values.append(timer)})
  reused=3 if kind=='state' else False if kind=='task' else lua.table_from({'actor':'hero'})
  run(q,reused);assert values==[ident];cases+=1
 assert cases==45
def test_native_destructor_and_full_cfg_lifetime(evidence):
 source,fn,row,rd=evidence
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime
 from tools.script_recovery.native_bounded_switch import resolve
 a,b=int(fn['address'],16),int(fn['bodyEndExclusive'],16);decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;ins=list(decoder.disasm(rd.bytes_at(a,b-a),a))
 events={site:(kind,('frame',16)) for site,kind in [(0xe44fd4,'start'),(0xe459b9,'use'),(0xe459f4,'use'),(0xe4565c,'end'),(0xe45c5e,'end'),(0xe469cf,'end')]}
 assert check_single_resource_lifetime(ins,events,indirect_branches=resolve(ins,rd))
 events.pop(0xe469cf);assert not check_single_resource_lifetime(ins,events,indirect_branches=resolve(ins,rd))
