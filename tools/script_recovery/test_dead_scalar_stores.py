"""Dead pointer cleanup must preserve scalar handles and branch-dependent aliases."""
import pytest
from tools.script_recovery.native_evidence_lowering import drop_dead_local_stores


def test_saved_conversation_handle_survives_comparison_and_branch():
    source = """  int iVar6;
  int iStack_10;
  iStack_10 = -1;
  iVar6 = Compare(loop, literal);
  if (iVar6 == 0) goto LAB_animation;
  iVar6 = iStack_10;
  goto LAB_wait;
LAB_animation:
  PlayAnimation();
  iVar6 = iStack_10;
LAB_wait:
  WaitForConversation(iVar6);
"""
    assert drop_dead_local_stores(source) == source


@pytest.mark.parametrize('branch', ['if (flag) {\n', 'goto LAB_read;\n', 'LAB_join:\n'])
def test_pointer_alias_is_not_removed_across_control_flow(branch):
    source = '  pCVar1 = &local_20;\n' + branch + '  pCVar1 = Acquire();\n  Use(pCVar1);\n'
    assert drop_dead_local_stores(source) == source


def test_proven_straight_line_pointer_alias_can_still_be_removed():
    source = '  pCVar1 = &local_20;\n  pCVar1 = Acquire();\n  Use(pCVar1);\n'
    assert drop_dead_local_stores(source) == '  pCVar1 = Acquire();\n  Use(pCVar1);\n'
