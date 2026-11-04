import sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R));sys.path.insert(0,str(W))
if W.parent.name=='work' and 'tools.script_recovery.native_entry_me_snapshots' not in sys.modules:
 from candidate_bootstrap import install
 install()
from tools.script_recovery.lift_native_lua import Lifter
from tools.script_recovery.test_lift_native_lua import MANIFEST,NoRData

@pytest.mark.parametrize('reassigned_first',[False,True])
@pytest.mark.parametrize('copy_twice',[False,True])
def test_saved_actor_survives_register_changes(reassigned_first,copy_twice):
 manifest={**MANIFEST,'GetHero':{'scope':'Quest','returnType':'CScriptThing*','parameters':[]}}
 lifter=Lifter(manifest,{},'quest',True,'Pkg',NoRData());lifter.accessor_kinds=True
 change='  pCVar8 = GSI->GetHero();\n'
 source='{\n  CScriptThing *pCVar8;\n  CScriptThing *xStack_9c;\n  CScriptThing *xStack_98;\n  pCVar8 = (CScriptThing *)(this + 8);\n'
 if reassigned_first:source+=change
 source+='  xStack_9c = pCVar8;\n'
 if not reassigned_first:source+=change
 if copy_twice:source+='  xStack_98 = xStack_9c;\n'
 source+='  GSI->RemoveThing('+('xStack_98' if copy_twice else 'xStack_9c')+');\n}'
 body='\n'.join(lifter.lift('Main',source));lua=LuaRuntime();me=lua.table();hero=lua.table();calls=[]
 quest=lua.table_from({'GetHero':lambda _:hero,'RemoveThing':lambda _,actor:calls.append(actor)})
 lua.execute('return function(quest,me)\n'+body+'\nend')(quest,me)
 eq=lua.eval('function(a,b)return rawequal(a,b)end')
 assert len(calls)==1 and eq(calls[0],hero if reassigned_first else me),body
