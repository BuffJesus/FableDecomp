"""Thing calls on a merged receiver variable resolved by reaching definitions (Arena ArenaCellDoorGuard 0x00F17C70)."""
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_receiver_reaching import me_receiver_sites, respell_me_receivers


def test_hit_test_receiver_is_me_on_every_path():
    # `lea ebp,[esi+8]` at 0x00F17D9D reaches the MsgIsHitByHero call through the switch at 0x00F187A0; the
    # cutscene branch's `mov ebp,[esi+4]` does not
    sites = me_receiver_sites(RData(), 0x00F17C70, 0x00F19A8B)
    assert 0x00F194FF in sites


def test_printed_call_is_respelled_without_the_receiver_argument():
    text = 'bVar3 = (**(code **)(*(int *)pCVar6 + 0xa4))(pCVar6,0xe,&CStack_1a0);'
    start = text.index('(pCVar6,') + 1
    entries = [(start, len(text) - 2, None, {'site': '0x00F194FF'}, '0xa4', True)]
    fn = {'address': '0x00F17C70', 'bodyEndExclusive': '0x00F19A8B'}
    out = respell_me_receivers(text, fn, entries, RData())
    assert out == 'bVar3 = (**(code **)(*(int *)(this + 8) + 0xa4))(0xe,&CStack_1a0);'


def test_register_loaded_with_me_is_me_until_reassigned():
    from tools.script_recovery.native_evidence_lowering import respell_me_register_calls
    text = ('  piVar1 = (int *)(this + 8);\n  bVar5 = (**(code **)(*piVar1 + 0x54))(piVar1,&name);\n'
            '  piVar1 = *(int **)(this + 4);\n  (**(code **)(*piVar1 + 0x1c))(piVar1);\n')
    out = respell_me_register_calls(text)
    assert 'bVar5 = (**(code **)(*(int *)(this + 8) + 0x54))(&name);' in out
    assert '(**(code **)(*piVar1 + 0x1c))(piVar1);' in out
