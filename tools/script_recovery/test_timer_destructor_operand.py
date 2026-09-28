"""Recover timer destruction from the retail receiver, not a drifted export slot."""
import ast
import json
from types import SimpleNamespace

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import (
    _register_operand_slot, restore_stack_operands, unwrap_statements,
)
from tools.script_recovery.quest_unit_evidence import ROOT


def test_woman_timer_destructor_uses_registered_slot():
    tu = json.loads((ROOT / 'refs/script_recovery/sick_child/translation_unit_typed.json').read_text())
    fn = next(f for f in tu['functions'] if f['address'].lower() == '0x00ecf7f0')
    for key in ('calls', 'indirectCalls'):
        if isinstance(fn[key], str):
            fn[key] = ast.literal_eval(fn[key])
    destructor = next(c for c in fn['calls'] if c.get('target') == '0x00CD4470')
    assert destructor['ecxStack'] == -0xcc  # stale exporter depth
    assert _register_operand_slot(fn, 0xed0aec, 'ecx', address=True) == -0xd0
    restored = restore_stack_operands(unwrap_statements(fn['decompile']), fn)
    assert 'CTimer::~CTimer((CTimer *)&xStack_d0);' in restored
    assert destructor['ecxStack'] == -0xcc  # evidence remains immutable


def test_receiver_address_requires_adjacent_lea(monkeypatch):
    # sub esp, 0x20; push ebx; lea ecx, [esp+4]; call ...
    prefix = bytes.fromhex('83ec20538d4c2404')
    raw = prefix + bytes.fromhex('e8000000005b83c420c3')
    fn = {'address': '0x1000', 'size': len(raw)}
    monkeypatch.setattr(_register_operand_slot, 'rdata',
                        SimpleNamespace(bytes_at=lambda *_: raw), raising=False)
    assert _register_operand_slot(fn, 0x1008, 'ecx', address=True) == -0x20
    # ECX has been replaced: the old address is not evidence for the call.
    raw = prefix + bytes.fromhex('31c9e8000000005b83c420c3')
    fn['size'] = len(raw)
    assert _register_operand_slot(fn, 0x100a, 'ecx', address=True) is None

    # An interior branch gives no straight-line proof of the frame depth.
    raw = prefix + bytes.fromhex('e800000000eb005b83c420c3')
    fn['size'] = len(raw)
    assert _register_operand_slot(fn, 0x1008, 'ecx', address=True) is None
    # A late saved register is reflected in the epilogue, even when the
    # initial prologue scan stops at an unrelated instruction.
    raw = bytes.fromhex('83ec20538d3424558d4c2404e8000000005d5b83c420c3')
    fn['size'] = len(raw)
    assert _register_operand_slot(fn, 0x100c, 'ecx', address=True) == -0x24
    # Missing bytes must not produce a guessed timer slot.
    monkeypatch.setattr(_register_operand_slot, 'rdata',
                        SimpleNamespace(bytes_at=lambda *_: None))
    monkeypatch.setattr('tools.script_recovery.lift_native_lua.RData',
                        lambda: SimpleNamespace(bytes_at=lambda *_: None))
    assert _register_operand_slot(fn, 0x100a, 'ecx', address=True) is None


def run_cleanup(source, exit_at):
    lua = LuaRuntime()
    lua.execute(source)
    lua.globals().exit_at = exit_at
    return lua.execute('''
        local terminating, released, registered = false, nil, 0
        local handle = 731
        local resources = {
            MemberResource = function() return {} end,
            PrepareResource = function() end,
        }
        local me = {AcquireControl = function() return true end}
        local quest = {
            RetailResources = function() return resources end,
            NewScriptFrame = function() return true end,
            IsActiveThreadTerminating = function() return terminating end,
            GetStateBool = function() return false end,
            SetIsPushableByHero = function() end,
            SetThingHasInformation = function() end,
            RegisterTimer = function()
                registered = registered + 1
                if exit_at == 'register' then terminating = true end
                return handle
            end,
            DeregisterTimer = function(_, timer)
                assert(released == nil, 'double release')
                assert(timer == handle, 'wrong timer handle')
                released = timer
            end,
            GetTimer = function(_, timer) assert(timer == handle); return 0 end,
            GetHero = function() return {} end,
            IsDistanceBetweenThingsUnder = function()
                terminating = true
                return true
            end,
        }
        Init(quest, me)
        Main(quest, me)
        assert(registered == 1 and released == handle)
        return released
    ''')


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('exit_at', ['register', 'distance'])
def test_generated_cleanup_releases_exact_handle(stage, exit_at):
    source = (ROOT / 'refs/script_recovery/lifted/SickChild' / stage /
              'FSE/V_SickChild/Entities/WomanToAttract.lua').read_text()
    assert run_cleanup(source, exit_at) == 731
