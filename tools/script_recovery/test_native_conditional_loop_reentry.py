import itertools,sys
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT))
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery import lift_native_lua as lift
from tools.script_recovery import native_loop_reentry as candidate

SOURCE='''{
iVar1 = 0;
bVar1 = ready;
while (iVar2 = iVar1, iVar3 = iVar2, bVar1) {
GSI->SetTimer(10,iVar1);
if (enter) {
LAB_00123456:
GSI->SetTimer(11,iVar1);
iVar1 = iVar1 + 1;
if (iVar1 >= limit) {
return iVar1;
}
bVar1 = false;
}
bVar1 = false;
}
GSI->SetTimer(12,iVar3);
goto LAB_00123456;
}'''

@pytest.mark.parametrize('ready,enter,limit',list(itertools.product((False,True),(False,True),(1,2,4))))
@pytest.mark.parametrize('nested,outer_continue',[(False,False),(True,False),(True,True)])
def test_nested_conditional_tail_preserves_headers_and_loop_transfers(ready,enter,limit,nested,outer_continue):
    source=SOURCE
    if nested:
        source=source.replace('iVar1 = iVar1 + 1;', '''iVar4 = 0;
while (iVar4 < 2) {
iVar4 = iVar4 + 1;
if (iVar4 != 1) {
GSI->SetTimer(14,iVar1);
}
if (iVar4 == 2) break;
}
iVar1 = iVar1 + 1;''')
    if outer_continue:source=source.replace('if (iVar1 >= limit) {\nreturn iVar1;\n}', 'if (iVar1 >= limit) {\nreturn iVar1;\n}\nif (iVar1 == 1) continue;')
    lifter=make();lifter.native_gotos=True;lifter.accessor_kinds=True
    body='\n'.join(lifter.lift('Main',source,parameters={'ready':'bool','enter':'bool','limit':'int'}))
    lua=LuaRuntime();lua.execute("debug.sethook(function()error('budget')end,'',10000)")
    events=[];q=lua.table_from({'SetTimer':lambda _,t,v:events.append((t,v))})
    assert lua.execute('return function(Quest,ready,enter,limit)\n'+body+'\nend')(q,ready,enter,limit)==limit
    expected=[(10,0)] if ready else []
    if not (ready and enter):expected.append((12,0))
    for i in range(limit):
        expected.append((11,i))
        if nested:expected.append((14,i))
        if i+1<limit:
            if outer_continue and i==0 and ready and enter:expected.append((10,1))
            else:expected.append((12,i+1))
    assert events==expected,(body,events,expected)
    # Tail copying can leave the old, now unused label as a diagnostic.
    assert all(todo=='label LAB_00123456' for todo in lifter.todo),lifter.todo
    assert '-- TODO(native): goto' not in body

@pytest.mark.parametrize('source',[
    SOURCE.replace('bVar1) {','bVar1 && ready) {'),
    SOURCE.replace('iVar2 = iVar1','iVar2 = GetValue()'),
    SOURCE.replace('if (enter) {','while (enter) {'),
    SOURCE.replace('GSI->SetTimer(11,iVar1);','return;'),
    SOURCE.replace('GSI->SetTimer(10,iVar1);','switch (value) {\ncase 0:\nbreak;\n}'),
    'goto LAB_00123456;\n'+SOURCE.replace('goto LAB_00123456;',''),
    SOURCE.replace('bVar1) {','1) {'),
    SOURCE.replace('GSI->SetTimer(10,iVar1);','while (inner) {\nif (skip) continue;\nif (done) break;\n}'),
    SOURCE+'}',
])
def test_ambiguous_or_unsupported_entries_remain_unchanged(source):
    statements=lift.strip_declarations(source)
    assert candidate.flatten(statements)==statements

def test_fresh_labels_and_idempotence():
    statements=['FLOW_loop_head_lab_00123456:','FLOW_loop_exit_lab_00123456:']+lift.strip_declarations(SOURCE)
    result=candidate.flatten(statements)
    assert 'FLOW_loop_head_lab_00123456_1:' in result
    assert 'FLOW_loop_exit_lab_00123456_1:' in result
    assert candidate.flatten(result)==result
