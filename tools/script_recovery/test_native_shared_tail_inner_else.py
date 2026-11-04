import importlib.util,itertools
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import lift_native_lua as lift
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery import native_goto_scopes as m
SOURCE='''{
if (enter) {
if (ready) {
LAB_00123456:
if (choice == 0) {
GSI->SetTimer(17,11);
}
else if (choice == 1) {
GSI->SetTimer(17,22);
}
else {
GSI->SetTimer(17,33);
}
GSI->SetTimer(17,44);
}
else {
GSI->SetTimer(17,55);
}
GSI->SetTimer(17,66);
}
else {
goto LAB_00123456;
}
GSI->SetTimer(17,77);
return;
}'''
@pytest.mark.parametrize('enter,ready,choice',list(itertools.product((False,True),(False,True),(-1,0,1))))
def test_copied_inner_else_chain_and_skipped_outer_else(enter,ready,choice):
 source=SOURCE
 lifter=make();lifter.native_gotos=True;lifter.accessor_kinds=True
 with patch.object(lift,'duplicate_sibling_tails',m.duplicate_sibling_tails),patch.object(lift,'hoist_shared_tails',lambda x:x):
  body='\n'.join(lifter.lift('Main',source,parameters={'enter':'bool','ready':'bool','choice':'int'}))
 lua=LuaRuntime();events=[];q=lua.table_from({'SetTimer':lambda _,timer,value:events.append(value)})
 lua.execute('return function(Quest,enter,ready,choice)\n'+body+'\nend')(q,enter,ready,choice)
 expected=([55] if enter and not ready else [{0:11,1:22}.get(choice,33),44])+[66,77]
 assert events==expected,(body,events,expected)
 assert not [x for x in lifter.todo if not x.startswith('label ')],lifter.todo

def test_combined_enclosing_else_stays_unresolved():
 statements=lift.strip_declarations(SOURCE.replace('}\nelse','} else'))
 assert m.duplicate_sibling_tails(statements)==statements
