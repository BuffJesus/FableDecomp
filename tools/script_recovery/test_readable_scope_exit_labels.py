"""Composition retains close/GC/error and return behavior after scope exits."""
import pytest
from tools.script_recovery import test_readable_scope_exit as S
from tools.script_recovery.readable_scope_exit import fold
from tools.script_recovery.readable_style import prune_unused_generated_labels

@pytest.mark.parametrize('kind',['nil','false','true','zero','empty','object'])
@pytest.mark.parametrize('fail',['','root','cond','child','use','result','close:child','close:root'])
@pytest.mark.parametrize('end_target',[False,True])
def test_scope_exit_then_unused_generated_label_preserves_observations(kind,fail,end_target):
    source=S.SOURCE.replace('done','FLOW_scope_exit')
    if end_target:source=source.replace('    return q:Result()\n','')
    middle,proof=fold(source);assert proof and proof['removedGotos']==1
    after,count=prune_unused_generated_labels(middle);assert count==1
    assert after==middle.replace('    ::FLOW_scope_exit::\n','')
    assert S.run(source,kind,True,fail)==S.run(after,kind,True,fail)
    assert prune_unused_generated_labels(after)==(after,0)
