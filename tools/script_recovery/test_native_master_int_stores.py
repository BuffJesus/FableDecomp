import copy
import hashlib
import json
import random
import re
import struct
from pathlib import Path
from unittest.mock import patch

import pytest
from lupa.lua54 import LuaRuntime

ROOT = next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())


@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    from tools.script_recovery import native_reviewed_master_int_stores as adapter
    from tools.script_recovery.convert_quest_unit import UnitConverter
    from tools.script_recovery.build_readable_unit import readable_file
    converter = UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json')
    unit = json.loads((ROOT/'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text())
    result = {}
    original = adapter.restore

    class Done(Exception):
        pass

    def capture(source, todo, calls, fn, unit, rd):
        restored, remaining, new_calls = original(source, todo, calls, fn, unit, rd)
        if fn['address'].lower() == '0x00e64fb0':
            result.update(source=source, todo=todo, calls=calls, fn=fn, unit=unit, rd=rd,
                          draft=restored, readable=readable_file(restored)[0],
                          remaining=remaining, new_calls=new_calls)
            raise Done()
        return restored, remaining, new_calls

    with patch.object(converter, 'constructor_defaults', return_value={}), patch.object(adapter, 'restore', capture):
        with pytest.raises(Done):
            converter.convert(unit, tmp_path_factory.mktemp('master_int_stores'))
    return result


@pytest.mark.parametrize('fault', ['native', 'source', 'lua', 'type', 'field', 'owner', 'register',
                                  'duplicate', 'window', 'bytes', 'counter', 'increment', 'diagnostics'])
def test_rejects_stale_or_ambiguous_evidence(captured, fault):
    from tools.script_recovery.native_reviewed_master_int_stores import restore
    c = captured
    unit, fn, todo = copy.deepcopy(c['unit']), copy.deepcopy(c['fn']), list(c['todo'])
    row = unit['reviewedMasterIntStores']['0x00e64fb0']
    source = c['source']
    if fault == 'native': row['nativeSha256'] = '0'*64
    elif fault == 'source': fn['decompile'] += '\n'
    elif fault == 'lua': source += '\n'
    elif fault == 'type': unit['master']['fields']['0xf8'][1] = 'String'
    elif fault == 'field': row['fieldOffset'] += 4
    elif fault == 'owner': row['ownerOffset'] += 4
    elif fault == 'register': row['valueRegister'] = 'edi'
    elif fault == 'duplicate': row['stores'].append(copy.deepcopy(row['stores'][0]))
    elif fault == 'window': row['stores'][0]['start'] += 1
    elif fault == 'bytes': row['stores'][0]['bytes'] = '90'
    elif fault == 'counter': row['valueLocal'] = 'unproven_score'
    elif fault == 'increment': row['increments']['0x64'] += 1
    elif fault == 'diagnostics': todo = []
    with pytest.raises(ValueError):
        restore(source, todo, c['calls'], fn, unit, c['rd'])


def test_only_five_stores_recovered(captured):
    c = captured
    assert len(c['todo']) - len(c['remaining']) == 5
    assert len(c['new_calls']) - len(c['calls']) == 5
    assert c['draft'].count('quest:SetMasterGameState("MaxChickenKickingScore",') == 5
    assert '-- TODO(native): if ctr_CVar19 ==' in c['draft']
    assert 'UpdateOnlineScore_ChickenKick: not in FSE manifest' in c['remaining']
    before, after = c['source'].splitlines(), c['draft'].splitlines()
    assert len(before) == len(after)
    assert sum(a != b for a, b in zip(before, after)) == 5


def test_unreviewed_function_unchanged(captured):
    from tools.script_recovery.native_reviewed_master_int_stores import restore
    c = captured
    unit = copy.deepcopy(c['unit']); unit.pop('reviewedMasterIntStores')
    assert restore(c['source'], c['todo'], c['calls'], c['fn'], unit, c['rd']) == (c['source'], c['todo'], c['calls'])


@pytest.mark.parametrize('style', ['draft', 'readable'])
def test_actual_emitted_stores_match_original_native(captured, style):
    import unicorn as u
    from unicorn import x86_const as x
    c = captured
    row = c['unit']['reviewedMasterIntStores']['0x00e64fb0']
    lines = [s.strip() for s in c[style].splitlines() if ':SetMasterGameState("MaxChickenKickingScore",' in s]
    assert len(lines) == 5
    lua = LuaRuntime()
    funcs = []
    for line in lines:
        match = re.fullmatch(r'quest:SetMasterGameState\("MaxChickenKickingScore", \(?\(?([A-Za-z_]\w*) \+.*', line)
        assert match, line
        funcs.append(lua.eval('function('+match[1]+') local calls=0; local result; local quest={SetMasterGameState=function(self,k,v) assert(k=="MaxChickenKickingScore");calls=calls+1;result=v end}; '+line+'; return result,calls end'))
    vm = u.Uc(u.UC_ARCH_X86, u.UC_MODE_32)
    for a in (0xe68000, 0x300000, 0x400000, 0x500000): vm.mem_map(a, 0x1000)
    vm.mem_write(0xe68000, c['rd'].bytes_at(0xe68000, 0x1000))
    rng = random.Random(47)
    values = [0, 1, 10, 25, 50, 100, 500, 0x7fffffff, 0x80000000, 0xffffffff, 0x1000001] + [rng.getrandbits(32) for _ in range(512)]
    for layout in (0, 0x40, 0x100):
        owner, master = 0x400000 + layout, 0x500000 + layout
        vm.mem_write(owner+0x18, struct.pack('<I', master))
        for bits in values:
            signed = struct.unpack('<i', struct.pack('<I', bits))[0]
            for window, emitted in zip(row['stores'], funcs):
                vm.mem_write(master+0xf4, b'\xa5'*12)
                vm.reg_write(x.UC_X86_REG_ESI, owner)
                vm.reg_write(x.UC_X86_REG_EBX, bits)
                vm.reg_write(x.UC_X86_REG_ESP, 0x300800)
                vm.emu_start(window['start'], window['end'], count=20)
                assert bytes(vm.mem_read(master+0xf4,12)) == b'\xa5'*4 + struct.pack('<I', bits) + b'\xa5'*4
                for representation in (bits, signed):
                    assert emitted(representation) == (signed, 1)
