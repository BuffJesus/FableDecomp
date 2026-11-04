from pathlib import Path
import importlib.util,json,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent
from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_initial_literals as P

@pytest.mark.parametrize('name',['initialized_shadow','opaque_loop_prefix'])
def test_observed_counterexamples_preserved(name):
    source=(W/'testdata/initial_flow_safety'/(name+'_input.lua')).read_text()
    assert P.prune(source)==(source,[])

@pytest.mark.parametrize('declaration',[
    'local x = q:Value()',
    'local y, x = 0, q:Value()',
    'local x <const> = q:Value()',
])
def test_every_initialized_shadow_declaration_counts(declaration):
    source='function Main(q)\n    local x\n    x = false\n    do\n        '+declaration+'\n    end\n    return x\nend\n'
    LuaRuntime().compile(source)
    assert P.prune(source)==(source,[])

@pytest.mark.parametrize('prefix',[
    'repeat x = q:Resource() until true',
    'do x = q:Resource() end',
    'do goto past end',
    'if q:Choose() then x = q:Resource() end',
])
def test_opaque_prefixes_preserve(prefix):
    source='function Main(q)\n    local x\n    '+prefix+'; goto past\n    ::past::\n    x = false\n    q:Collect()\n    x = q:Value()\nend\n'
    LuaRuntime().compile(source)
    assert P.prune(source)==(source,[])

@pytest.mark.parametrize('spacing',['',' ','  ','\t'])
def test_anonymous_closure_capture_spacing_cannot_hide_definition(spacing):
    source='function Main(q)\n    local x\n    q:Invoke(function'+spacing+'() x = q:Resource() end); goto past\n    ::past::\n    x = false\n    q:Collect()\n    x = q:Value()\nend\n'
    LuaRuntime().compile(source)
    assert P.prune(source)==(source,[])

def test_simple_call_and_assignment_prefixes_remain_supported():
    source='function Main(q)\n    local x\n    q:Observe(); goto past\n    ::past::\n    x = false\n    q:Other()\n    x = q:Value()\nend\n'
    after,proof=P.prune(source)
    assert len(proof)==1 and 'x = false' not in after

def test_one_clean_initial_nil_store_still_removed():
    source='function Main(q)\n    local x\n    x = false\n    x = q:Value()\n    return x\nend\n'
    after,proof=P.prune(source)
    assert len(proof)==1 and proof[0]['hostReturnContractRequired'] is False
    assert after==source.replace('    x = false\n','')
