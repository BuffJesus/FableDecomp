from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery import lift_native_lua as lift
SOURCE='{\npCVar5 = GSI->GetThingWithScriptName(&xStack_1c,"Enemy");\nxStack_c0 = pCVar5;\npCVar5 = GSI->GetHero();\nGSI->GiveThingBestEnemyTarget(&xStack_c0,pCVar5);\n}'
def test_explicit_thing_reference():
 l=make();l.accessor_kinds=True
 l.manifest.update({'GetThingWithScriptName':{'scope':'Quest','returnType':'CScriptThing','parameters':[{'name':'name','type':'const std::string&'}]},'GetHero':{'scope':'Quest','returnType':'CScriptThing','parameters':[]},'GiveThingBestEnemyTarget':{'scope':'Quest','returnType':'void','parameters':[{'name':'thing','type':'CScriptThing*'},{'name':'target','type':'CScriptThing*'}]}})
 body='\n'.join(l.lift('Main',SOURCE))
 lua=LuaRuntime();enemy=lua.table_from({'id':'enemy'});hero=lua.table_from({'id':'hero'});calls=[]
 q=lua.table_from({'GetThingWithScriptName':lambda _,name:enemy,'GetHero':lambda _:hero,'GiveThingBestEnemyTarget':lambda _,thing,target:calls.append((thing.id,target.id if target is not None else None))})
 lua.execute('return function(Quest)\n'+body+'\nend')(q)
 assert calls==[('enemy','hero')],(body,calls)


@pytest.mark.parametrize('kind',[None,'unknown','bool'])
def test_uncertain_reference_stays_unresolved(kind):
 l=make();l.accessor_kinds=True;l.locals.update(('enemy','hero'));l.kinds['hero']='thing'
 if kind is not None:l.kinds['enemy']=kind
 l.manifest['GiveThingBestEnemyTarget']={'scope':'Quest','returnType':'void','parameters':[{'name':'thing','type':'CScriptThing*'},{'name':'target','type':'CScriptThing*'}]}
 l.interface_call(None,'GiveThingBestEnemyTarget','&enemy,hero')
 assert 'enemy' not in l.out[0]
 assert any('args' in diagnostic for diagnostic in l.todo)
