from pathlib import Path
from copy import deepcopy
import hashlib,json,sys
import pytest
R=Path(__file__).resolve().parents[2];W=Path(__file__).resolve().parent/'fixtures/tail_copy/book_main'
from tools.script_recovery import native_reviewed_interface_table_staging as classifier
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from tools.script_recovery.annotate_interface_slots import VTABLE_BASE
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.native_register_snapshot_provenance import analyze
from tools.script_recovery.lua_terminal_cleanup_alias import prove_terminal_cleanup_alias
from tools.script_recovery.native_cleanup_regions import hoist_cleanup_regions
read=lambda p:json.loads(p.read_text(encoding='utf-8'))
sha=lambda value:hashlib.sha256(value).hexdigest()

@pytest.fixture(scope='module')
def baseline():
    unit=read(W/'candidate_unit.json');fn=read(W/'native_function.json');row=read(W/'candidate_row.json');target=int(row['target'],16)
    parts=[(int(fn['address'],16),(W/'native_function.bin').read_bytes()),
           (VTABLE_BASE+0x5ec,target.to_bytes(4,'little')),(target,bytes.fromhex(row['calleeWitnessPrefix']))]
    for copy in row['copies']:
        predicate=copy['predicate']
        for key in ['wrapper','accessor','tail']:
            parts.append((int(predicate[key]['address'],16),bytes.fromhex(predicate[key]['bytes'])))
        parts.append((int(predicate['vtable'],16)+0xc,int(predicate['accessor']['address'],16).to_bytes(4,'little')))
    image=FixtureImage(parts);row['imageSha256']=sha(image.data)
    from tools.script_recovery import convert_quest_unit
    with pytest.MonkeyPatch.context() as patch:
        patch.setattr(convert_quest_unit,'callee_stack_words',lambda address:1 if address==target else None)
        yield dict(source=(W/'phase.lua').read_text(encoding='utf-8'),todo=read(W/'phase_todo.json'),fn=fn,row=row,
                   spec=LoweringSpec(unit,'BS_Teacher',entity=True,thing_slots={}),image=image,copy_trace=read(W/'copy_trace.json'))

def run(baseline,**changes):
    args=dict(baseline);args.update(changes)
    return classifier.classify(**args,snapshot_evidence=analyze,liveness=prove_unused_staging,
                                        hoister=hoist_cleanup_regions,whole_liveness=prove_terminal_cleanup_alias)

def test_exact_copy_reviews_both_aliases_without_executable_change(baseline):
    from tools.script_recovery.readable_lua import tokens
    out,todo,reviews=run(baseline)
    sig=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
    assert sig(out)==sig(baseline['source'])
    assert len(todo)==len(baseline['todo'])-2 and baseline['row']['diagnostic'] not in todo
    assert len(reviews[0]['copies'])==1 and reviews[0]['copies'][0]['liveness']['visitedNodes']>0

def test_legacy_row_does_not_require_trace_or_change_old_output(baseline):
    row=deepcopy(baseline['row']);row.pop('copies')
    out,todo,reviews=run(baseline,row=row,copy_trace=None)
    assert todo.count(row['diagnostic'])==1 and 'copies' not in reviews[0]

@pytest.mark.parametrize('trace',[None,[]])
def test_missing_trace_rejects(baseline,trace):
    with pytest.raises(ValueError):run(baseline,copy_trace=trace)

def test_wrong_or_ambiguous_actual_trace_rejects(baseline):
    selected=baseline['row']['copies'][0]['copyTrace']
    for trace in [[event for event in baseline['copy_trace'] if event!=selected],baseline['copy_trace']+[deepcopy(selected)]]:
        with pytest.raises(ValueError):run(baseline,copy_trace=trace)

@pytest.mark.parametrize('key,value',[
 ('acceptedEvidenceSha256','wrong'),('scopeLimit','proved'),('conditionAlias','other'),('movieAlias','other'),
 ('nativeBranch','0x00e55230'),('nativeBranch','0x00e55889'),('luaRegion','wrong'),
])
def test_copy_metadata_mismatch_rejects(baseline,key,value):
    row=deepcopy(baseline['row']);row['copies'][0][key]=value
    with pytest.raises(ValueError):run(baseline,row=row)

@pytest.mark.parametrize('key,value',[
 ('target','LAB_00e55a13'),('terminated',False),('syntheticExit','FLOW_after'),('originalJump','goto LAB_00e55316;'),
 ('outputStatementCount',99),('outputStartIndex',-1),('inputSha256','wrong'),('preparedInputSha256','wrong'),('outputSha256','wrong'),
 ('copiedStatements',['iVar8 = *(int *)CVar12;','return;']),
])
def test_forged_matching_trace_still_requires_supported_shape(baseline,key,value):
    row=deepcopy(baseline['row']);trace=deepcopy(baseline['copy_trace']);wanted=row['copies'][0]['copyTrace']
    actual=next(event for event in trace if event==wanted);actual[key]=value;wanted[key]=value
    with pytest.raises(ValueError):run(baseline,row=row,copy_trace=trace)

@pytest.mark.parametrize('key,value',[
 ('wrapper',dict(address='0x00cb7940',bytes='00')),
 ('accessor',dict(address='0x00e54620',bytes='00'*4)),
 ('tail',dict(address='0x00cb7940',bytes='00'*14)),
 ('vtable','0x012e5f44'),('accessorOffset',16),('stateOffset',48),('flagOffset',6),
])
def test_predicate_native_witness_mismatch_rejects(baseline,key,value):
    row=deepcopy(baseline['row']);row['copies'][0]['predicate'][key]=value
    with pytest.raises(ValueError):run(baseline,row=row)

def test_changed_caller_window_rejects(baseline):
    row=deepcopy(baseline['row']);row['copies'][0]['nativeCaller']['bytes']='00'*15
    with pytest.raises(ValueError):run(baseline,row=row)

def test_duplicate_copy_inventory_rejects(baseline):
    row=deepcopy(baseline['row']);row['copies']*=2;row['diagnosticMultiplicity']=3
    todo=baseline['todo']+[row['diagnostic']]
    with pytest.raises(ValueError):run(baseline,row=row,todo=todo)

@pytest.mark.parametrize('old,new',[
 ('quest:PauseAllNonScriptedEntities(false)','quest:PauseAllNonScriptedEntities(true)'),
 ('resources:DestroyMovie(xStack_10)','resources:DestroyMovie(otherMovie)'),
 ('if bVar6 then','if not bVar6 then'),
 ('iVar8 = nil --[[unresolved native value]]','iVar8 = nil --[[unresolved native value]]\n                                quest:Observe(iVar8)'),
])
def test_resealed_phase_drift_in_copy_rejects(baseline,old,new):
    row=deepcopy(baseline['row']);copy=row['copies'][0];region=copy['luaRegion'];changed=region.replace(old,new,1);assert changed!=region
    source=baseline['source'].replace(region,changed,1);copy['luaRegion']=changed;row['luaPhaseSha256']=sha(source.encode())
    with pytest.raises(ValueError):run(baseline,row=row,source=source)
