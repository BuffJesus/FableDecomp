from pathlib import Path
from copy import deepcopy
import hashlib,importlib.util,json
W=Path(__file__).resolve().parent/'fixtures/tail_copy/book_main'
from tools.script_recovery import native_goto_scopes as candidate
before=(W/'02_duplicate_sibling_tails_before.cstatements').read_text().splitlines()
expected=(W/'02_duplicate_sibling_tails_after.cstatements').read_text().splitlines()
sha=lambda statements:hashlib.sha256('\n'.join(statements).encode()).hexdigest()

def test_real_book_default_and_traced_outputs_are_identical():
    trace=[]
    assert candidate.duplicate_sibling_tails(before)==candidate.duplicate_sibling_tails(before,copy_trace=trace)==expected
    assert len(trace)==7
    for event in trace:
        assert event['inputSha256']==sha(before) and event['outputSha256']==sha(expected)
        start=event['outputStartIndex'];end=start+event['outputStatementCount']
        assert expected[start:end]==event['copiedStatements']

def test_selected_clone_retains_condition_and_terminal_cleanup_order():
    trace=[];candidate.duplicate_sibling_tails(before,copy_trace=trace)
    matches=[r for r in trace if r['target']=='LAB_00e55316'];assert len(matches)==1
    event=matches[0]
    assert event['originalJump'].strip()=='if (bVar6) goto LAB_00e55316;'
    assert [line.strip() for line in event['copiedStatements']]==[
        'if (bVar6) {','iVar8 = *(int *)CVar12;','GSI->PauseAllNonScriptedEntities(0);','RESOURCE_DestroyMovie(xStack_10);','return;','}']
    assert event['terminated'] and event['syntheticExit'] is None

def test_reusing_trace_list_does_not_rewrite_prior_call_events():
    trace=[];candidate.duplicate_sibling_tails(before,copy_trace=trace);saved=deepcopy(trace)
    second=['']+before;result=candidate.duplicate_sibling_tails(second,copy_trace=trace)
    assert trace[:len(saved)]==saved
    assert len(trace)==2*len(saved)
    for event in trace[len(saved):]:assert event['inputSha256']==sha(second) and event['outputSha256']==sha(result)

def test_no_copy_does_not_add_trace_events():
    trace=[];assert candidate.duplicate_sibling_tails(['return;'],copy_trace=trace)==['return;']
    assert trace==[]

def test_rejected_unbalanced_input_does_not_invent_a_copy():
    trace=[];source=['if (bad) {','goto LAB_00e55316;']
    assert candidate.duplicate_sibling_tails(source,copy_trace=trace)==source and not trace
