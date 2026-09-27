"""Conservative operand recovery for Pickpocket's refreshed handle and countdown."""
import pytest
from tools.script_recovery.native_evidence_lowering import fold_local_thing_copies, drop_dead_local_stores


COPY = '''Use((CScriptThing *)xStack_28);
piVar3 = *(int **)(pCVar11 + 0x8);
piVar4 = *(int **)(pCVar11 + 0x4);
if (xStack_20 != piVar3) {
    xStack_28._4_4_ = piVar4;
    xStack_20 = piVar3;
    if (piVar3 != (int *)0x0) {
        *piVar3 = *piVar3 + 1;
    }
}
'''


def test_refresh_copies_the_new_target():
    assert fold_local_thing_copies(COPY) == 'Use((CScriptThing *)xStack_28);\nxStack_28 = pCVar11;\n'


@pytest.mark.parametrize('before,after', [
    ('xStack_20', 'xStack_1c'),
    ('(CScriptThing *)', '(Other *)'),
    ('xStack_28._4_4_ = piVar4', 'xStack_28._4_4_ = other'),
    ('*(int **)(pCVar11 + 0x4)', '*(int **)(other + 0x4)'),
    ('*piVar3 = *piVar3 + 1', '*piVar3 = *other + 1'),
])
def test_incomplete_handle_evidence_is_not_folded(before, after):
    source = COPY.replace(before, after)
    assert fold_local_thing_copies(source) == source


def test_countdown_is_not_mistaken_for_a_dead_pointer_alias():
    source = '''xStack_3c = GetDuration();
while (xStack_3c > 0) {
    NewFrame();
    xStack_3c = xStack_3c + -1;
}
'''
    assert drop_dead_local_stores(source) == source
