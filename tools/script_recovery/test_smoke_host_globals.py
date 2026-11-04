from pathlib import Path

import pytest

from tools.script_recovery.smoke_run_unit import free_globals

ROOT = Path(__file__).resolve().parents[2]


def test_presented_item_host_out_string_is_declared():
    source = '''function Main(quest, me)
        g_PresentedItemName = nil
        if me:MsgIsPresentedWithItem() then return g_PresentedItemName end
    end'''
    assert free_globals(source) == []


def test_similar_and_unrelated_globals_still_report():
    source = 'function Main(quest) return g_PresentedItemName_typo, g_UnknownNativeField end'
    assert free_globals(source) == ['g_PresentedItemName_typo', 'g_UnknownNativeField']


@pytest.mark.parametrize('name', ['Madame', 'Magicman'])
def test_actual_presented_item_consumers_have_no_free_global(name):
    source = (ROOT / 'refs/script_recovery/lifted/Bordello/readable/FSE/V_Bordello/Entities' / (name + '.lua')).read_text()
    assert free_globals(source) == []
