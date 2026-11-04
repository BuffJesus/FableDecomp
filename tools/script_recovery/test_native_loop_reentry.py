import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import lift_native_lua as lift
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery.native_loop_reentry import flatten

SOURCE='''{
iVar1 = 0;
while (true) {
GSI->SetTimer(10,iVar1);
if (iVar1 < limit) break;
LAB_00123456:
iVar1 = iVar1 + 1;
if (iVar1 >= 3) {
return;
}
}
GSI->SetTimer(11,iVar1);
goto LAB_00123456;
}'''

@pytest.mark.parametrize('limit',[-1,0,1,2,3,10])
def test_loop_header_repeats_after_backward_tail_entry(limit):
    lifter=make();lifter.native_gotos=True;lifter.accessor_kinds=True
    body='\n'.join(lifter.lift('Main',SOURCE,parameters={'limit':'int'}))
    lua=LuaRuntime();events=[]
    lua.execute("debug.sethook(function() error('instruction budget') end,'',5000)")
    q=lua.table_from({'SetTimer':lambda _,timer,value:events.append((timer,value))})
    run=lua.execute('return function(Quest,limit)\n'+body+'\nend');run(q,limit)
    expected=[]
    for i in range(3):
        expected.append((10,i))
        if i<limit:expected.append((11,i))
    assert events==expected,(body,events)
    assert not lifter.todo,lifter.todo


@pytest.mark.parametrize('limit',[0,1,3])
def test_continue_returns_to_header_instead_of_running_tail(limit):
    source=SOURCE.replace('if (iVar1 >= 3) {', '''if (iVar1 == 1) continue;
GSI->SetTimer(12,iVar1);
if (iVar1 >= 3) {''')
    lifter=make();lifter.native_gotos=True;lifter.accessor_kinds=True
    body='\n'.join(lifter.lift('Main',source,parameters={'limit':'int'}))
    lua=LuaRuntime();lua.execute("debug.sethook(function() error('instruction budget') end,'',5000)")
    events=[];q=lua.table_from({'SetTimer':lambda _,timer,value:events.append((timer,value))})
    lua.execute('return function(Quest,limit)\n'+body+'\nend')(q,limit)
    expected=[]
    for i in range(3):
        expected.append((10,i))
        if i<limit:expected.append((11,i))
        if i!=0:expected.append((12,i+1))
    assert events==expected


def lines(source=SOURCE):
    return lift.strip_declarations(source)


@pytest.mark.parametrize('source',[
    SOURCE.replace('while (true)','while (ready)'),
    SOURCE.replace('while (true)','for (;;)'),
    SOURCE.replace('GSI->SetTimer(10,iVar1);','if (ready) {\nGSI->SetTimer(10,iVar1);\n}'),
    SOURCE.replace('goto LAB_00123456;',''),
    SOURCE.replace('if (iVar1 < limit) break;',''),
    'goto LAB_00123456;\n'+SOURCE.replace('goto LAB_00123456;',''),
    SOURCE.replace('LAB_00123456:','if (ready) {\nLAB_00123456:').replace('iVar1 = iVar1 + 1;','}\niVar1 = iVar1 + 1;'),
    SOURCE.replace('GSI->SetTimer(10,iVar1);','switch (value) {\ncase 0:\nbreak;\n}'),
    SOURCE.replace('LAB_00123456:','LAB_00123456:\nLAB_00123456:'),
])
def test_other_control_flow_is_left_to_existing_passes(source):
    statements=lines(source)
    assert flatten(statements)==statements


def test_nested_loop_is_left_to_existing_passes():
    nested=['while (inner) {','if (skip) continue;','if (done) break;','}']
    statements=lines();at=statements.index('LAB_00123456:')+1;statements[at:at]=nested
    result=flatten(statements)
    assert result==statements


def test_unbalanced_input_is_not_rewritten():
    statements=lines()+['}']
    assert flatten(statements)==statements


def test_fresh_labels_do_not_collide_and_result_is_idempotent():
    statements=['FLOW_loop_head_lab_00123456:','FLOW_loop_exit_lab_00123456:']+lines()
    result=flatten(statements)
    assert 'FLOW_loop_head_lab_00123456_1:' in result
    assert 'FLOW_loop_exit_lab_00123456_1:' in result
    assert flatten(result)==result
