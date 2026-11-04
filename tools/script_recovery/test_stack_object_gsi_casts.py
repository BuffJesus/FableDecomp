"""Preserve return values across colliding stack-object member spellings."""
import re
import pytest
from tools.script_recovery.native_evidence_lowering import canonicalise_stack_objects


def source(cast='(int *)', other='xStack_340', call='Face', gap=''):
    target = 'GSI->Face' if call == 'Face' else call
    return ('  xStack_344 = RESOURCE_StartMovie("");\n'
            f'  xStack_344 = {cast}GSI->GetHero();\n'
            f'  {other} = p0;\n' + gap +
            f'  {target}(me,xStack_344,0);\n')

@pytest.mark.parametrize('cast', ['', '(int *)', '(CScriptThing *)', '(const CScriptThing **)'])
@pytest.mark.parametrize('indirect', [False, True])
def test_call_retains_return_value(cast, indirect):
    text = source(cast)
    if indirect:
        text=text.replace('GSI->GetHero', '(**(code **)(gsi + 0x118))').replace('GSI->Face', '(**(code **)(gsi + 0x76c))')
    out=canonicalise_stack_objects(text)
    temp=re.search(r'(gsi_stack_result_\w+) = ',out).group(1)
    assert f'(me,{temp},0);' in out
    assert 'xStack_344 = p0;' in out
    assert out.count('0x118' if indirect else 'GetHero') == 1

@pytest.mark.parametrize('other,gap,call', [
    ('xStack_344','','Face'), # explicit overwrite really replaces the result
    ('xStack_320','','Face'), # non-overlapping slot
    ('xStack_340','  if (flag) {\n','Face'), # no straight-line evidence
    ('xStack_340','','Other'), # not an identified GSI call
])
def test_ambiguous_or_noncolliding_sequence_is_not_snapshotted(other,gap,call):
    assert 'gsi_stack_result_' not in canonicalise_stack_objects(source(other=other,gap=gap,call=call))

def test_real_member_zero_still_folds():
    text='  xStack_344 = RESOURCE_StartMovie("");\n  xStack_340 = (int *)0x0;\n'
    assert canonicalise_stack_objects(text)==text.replace('xStack_340','xStack_344')

def test_multiple_calls_receive_distinct_snapshots():
    out=canonicalise_stack_objects(source()+source())
    names=re.findall(r'(gsi_stack_result_\w+) = .*GetHero',out)
    assert len(names)==len(set(names))==2
