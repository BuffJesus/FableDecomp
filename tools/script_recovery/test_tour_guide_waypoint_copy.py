"""Retained waypoint input and ownership diagnostics require exact evidence."""
from copy import deepcopy
import hashlib,json
from pathlib import Path
import pytest
from lupa import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_thing_outputs import restore
R=Path(__file__).resolve().parents[2]
F=R/'tools/script_recovery/fixtures/tour_guide_waypoint_copy'
@pytest.fixture(scope='module')
def evidence():
    c=UnitConverter(R/'refs/script_recovery/tour_guide/translation_unit_typed.json')
    return c.native('0x00ee6850'),json.loads((F/'rows.json').read_text()),c.rdata

@pytest.mark.parametrize('form',['master','shared'])
def test_recovers_copy_assignment_and_cleanup_with_exact_diagnostics(evidence,form):
    fn,rows,rd=evidence;row=rows[form];source=(F/(form+'_input.lua')).read_text()
    todo=[*row['resolvedTodoDiagnostics'],'label retained']
    output,remaining,calls=restore(source,todo,['prior'],fn,row,rd,R)
    assert output==(F/(form+'_output.lua')).read_text()
    assert remaining==['label retained'] and todo==[*row['resolvedTodoDiagnostics'],'label retained']
    assert calls==['prior',*row['calls']]
    assert 'resources:MoveToPosition(native_arg_param_2' in output
    assert 'owned:MoveToPosition' not in output

@pytest.mark.parametrize('form',['master','shared'])
def test_existing_position_list_diagnostic_dictionary_is_not_reinterpreted(evidence,form):
    fn,rows,rd=evidence;row=deepcopy(rows[form]);source=(F/(form+'_input.lua')).read_text()
    todo=row.pop('resolvedTodoDiagnostics')
    row['resolvedDiagnostics']={todo[0]:4,'verify cleanup at another label':2}
    output,remaining,_=restore(source,todo,[],fn,row,rd,R)
    assert output==(F/(form+'_output.lua')).read_text()
    assert remaining is todo

@pytest.mark.parametrize('form',['master','shared'])
@pytest.mark.parametrize('fault',['native','export','phase','dependency','sidecar','operation','output',
    'diagnostic_missing','diagnostic_duplicate','diagnostic_unknown','diagnostic_wrong_kind',
    'diagnostic_nonlist','diagnostic_nonstring','comment_retained'])
def test_rejects_changed_copy_or_diagnostic_evidence(evidence,form,fault):
    fn,rows,rd=evidence;fn=deepcopy(fn);row=deepcopy(rows[form]);source=(F/(form+'_input.lua')).read_text()
    todo=list(row['resolvedTodoDiagnostics'])
    if fault=='native':row['nativeSha256']='invalid'
    elif fault=='export':fn['decompile']+='changed'
    elif fault=='phase':source+='changed'
    elif fault=='dependency':row['dependencies'][next(iter(row['dependencies']))]['sha256']='invalid'
    elif fault=='sidecar':row['sidecar'][next(iter(row['sidecar']))]['sha256']='invalid'
    elif fault=='operation':row['operations'][1]['before']='unknown'
    elif fault=='output':row['recoveredSha256']='invalid'
    elif fault=='diagnostic_missing':todo.pop()
    elif fault=='diagnostic_duplicate':row['resolvedTodoDiagnostics'].append(row['resolvedTodoDiagnostics'][0])
    elif fault=='diagnostic_unknown':row['resolvedTodoDiagnostics'][0]='unlifted: other();'
    elif fault=='diagnostic_wrong_kind':row['resolvedTodoDiagnostics'][0]='label retained';todo.append('label retained')
    elif fault=='diagnostic_nonlist':row['resolvedTodoDiagnostics']={}
    elif fault=='diagnostic_nonstring':row['resolvedTodoDiagnostics']=[{}]
    elif fault=='comment_retained':
        row['operations']=row['operations'][:-3]
        out=source
        for operation in row['operations']:out=out.replace(operation['before'],operation['after'])
        row['recoveredSha256']=hashlib.sha256(out.encode()).hexdigest()
    with pytest.raises(ValueError,match='reviewed Thing outputs'):
        restore(source,todo,[],fn,row,rd,R)

@pytest.mark.parametrize('form',['master','shared'])
@pytest.mark.parametrize('mode,failure',[('changes','none'),('dead','lookup'),('dead','move')])
def test_retains_second_predicate_and_releases_locals_on_errors(form,mode,failure):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute((F/(form+'_output.lua')).read_text())
    fixture=lua.execute((F/'harness.lua').read_text())
    for key,value in dict(mode=mode,failure=failure,cancel=0,foundAfter=1).items():fixture[key]=value
    args=[fixture['quest'],fixture['input'],991] if form=='master' else [fixture['quest'],lua.table(),fixture['input'],991]
    if failure=='none':
        lua.globals().helper_EE6850(*args)
        assert fixture['queries']==2 and fixture['frames']==0 and fixture['moves']==1
    else:
        with pytest.raises(Exception,match='fixture '+failure+' failure'):lua.globals().helper_EE6850(*args)
    assert fixture['created']==fixture['retired'] and fixture['refs']==1
