import copy
import hashlib
import json
import math
import re
import struct
from pathlib import Path
from unittest.mock import patch

import pytest
from lupa.lua54 import LuaRuntime

ROOT = next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())


@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
    from tools.script_recovery import native_reviewed_x87_thresholds as adapter
    from tools.script_recovery.convert_quest_unit import UnitConverter
    from tools.script_recovery.build_readable_unit import readable_file
    converter = UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json')
    unit = json.loads((ROOT/'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text())
    captured = {}
    original = adapter.restore

    class Captured(Exception):
        pass

    def capture(source, fn, row, rd):
        restored = original(source, fn, row, rd)
        if row is not None:
            captured.update(source=source, fn=fn, row=row, rd=rd, generated=restored,
                            readable=readable_file(restored)[0])
            raise Captured()
        return restored

    with patch.object(converter, 'constructor_defaults', return_value={}), patch.object(adapter, 'restore', capture):
        with pytest.raises(Captured):
            converter.convert(unit, tmp_path_factory.mktemp('x87_thresholds'))
    return captured


@pytest.mark.parametrize('fault', ('native', 'source', 'lua', 'frame', 'slot', 'identity',
                                 'pointer', 'offset', 'target', 'chain', 'missing_witness', 'store', 'condition'))
def test_stale_review_rejected(review_input, fault):
    from tools.script_recovery.native_reviewed_x87_thresholds import restore
    c = review_input
    row, fn, source = copy.deepcopy(c['row']), copy.deepcopy(c['fn']), c['source']
    if fault == 'native': row['nativeSha256'] = '0'*64
    elif fault == 'source': fn['decompile'] += '\n'
    elif fault == 'lua': source += '\n'
    elif fault == 'frame': row['frameBytes'] += 4
    elif fault == 'slot': row['entrySlot'] += 4
    elif fault == 'identity': row['integerSlot'] = 'xStack_378'
    elif fault == 'pointer': row['globalPointer'] += 4
    elif fault == 'offset': row['comparisons'][0]['offset'] += 4
    elif fault == 'target': row['comparisons'][-1]['lowerTarget'] += 1
    elif fault == 'chain': row['comparisons'][1]['start'] += 4
    elif fault == 'missing_witness': row['nativeInstructions'].pop('0xe68241')
    elif fault == 'store':
        source = source.replace('f_stk_374 = xStack_374', 'f_stk_374 = 0')
        row['luaSha256'] = hashlib.sha256(source.encode()).hexdigest()
    elif fault == 'condition':
        source = source.replace('f_stk_374 < quest:ReadGlobalGameDataFloat(0xfb0)', 'f_stk_374 <= quest:ReadGlobalGameDataFloat(0xfb0)')
        row['luaSha256'] = hashlib.sha256(source.encode()).hexdigest()
    with pytest.raises(ValueError): restore(source, fn, row, c['rd'])


@pytest.mark.parametrize('address,old,new', ((0xe68241, 'f6c401', 'f6c441'),
                                           (0xe68244, '0f85be010000', '0f84be010000'),
                                           (0xe68235, 'd9542440', 'd95c2440'),
                                           (0xe68408, 'd9442440', 'd9442444')))
def test_changed_instruction_rejected_after_refreshing_hashes(review_input, address, old, new):
    from tools.script_recovery.native_reviewed_x87_thresholds import restore
    c = review_input
    row = copy.deepcopy(c['row'])
    start, end = int(c['fn']['address'], 16), int(c['fn']['bodyEndExclusive'], 16)
    raw = bytearray(c['rd'].bytes_at(start, end-start))
    assert raw[address-start:address-start+len(old)//2].hex() == old
    raw[address-start:address-start+len(old)//2] = bytes.fromhex(new)

    class Changed:
        def bytes_at(self, address, size):
            if start <= address and address+size <= end:
                return bytes(raw[address-start:address-start+size])
            return c['rd'].bytes_at(address, size)

    row['nativeSha256'] = hashlib.sha256(raw).hexdigest()
    row['nativeInstructions'][hex(address)] = new
    with pytest.raises(ValueError): restore(c['source'], c['fn'], row, Changed())


@pytest.fixture(scope='module')
def native_cases(review_input):
    import unicorn as u
    from unicorn import x86_const as x
    rd = review_input['rd']
    vm = u.Uc(u.UC_ARCH_X86, u.UC_MODE_32)
    for a, n in ((0xe68000,0x1000),(0x143e000,0x1000),(0x300000,0x1000),(0x400000,0x2000),(0x500000,0x1000)):
        vm.mem_map(a, n)
    vm.mem_write(0xe68000, rd.bytes_at(0xe68000,0x1000))
    vm.mem_write(0x143e90c, struct.pack('<I',0x400000))
    vm.mem_write(0x500000,b'\xdb\xe3')
    bits = lambda n: struct.unpack('<I',struct.pack('<f',n))[0]
    value = lambda n: struct.unpack('<f',struct.pack('<I',n))[0]
    cases = []
    for score in [*range(501),-2147483648,-16777217,16777217,2147483647]:
        thresholds = {0,0x80000000,0x7f800000,0xff800000,0x7fc00000,0xffc00000,0x7f800001,0xff800001,bits(score),bits(score-1),bits(score+1)}
        for raw in thresholds:
            for stage,(start,end,offset) in enumerate(((0xe6822b,0xe68244,0xfb0),(0xe68408,0xe68417,0xfac),(0xe68615,0xe68626,0xfa8))):
                vm.emu_start(0x500000,0x500002,count=1)
                vm.reg_write(x.UC_X86_REG_FPCW,0x27f)
                vm.reg_write(x.UC_X86_REG_ESP,0x300000)
                vm.reg_write(x.UC_X86_REG_ECX,0x400000)
                vm.mem_write(0x300040,struct.pack('<i',score) if stage==0 else struct.pack('<I',bits(score)))
                vm.mem_write(0x400000+offset,struct.pack('<I',raw))
                vm.emu_start(start,end,count=12)
                native = (vm.reg_read(x.UC_X86_REG_EFLAGS)&0x40)==0
                cases.append((score,value(raw),stage,native,bytes(vm.mem_read(0x300040,4))))
    assert len(cases)==16641
    return cases


@pytest.mark.parametrize('style', ('generated','readable'))
def test_actual_main_predicates_match_native(review_input, native_cases, style):
    source = review_input[style]
    store = next(line.strip() for line in source.splitlines() if 'string.pack("<f"' in line and '2147483648' in line)
    floating = store.split(' = ',1)[0].removeprefix('local ')
    integer = re.search(r'(\w+) \+ 2147483648',store)[1]
    lua = LuaRuntime(encoding=None)
    lua.execute('SCRIPT_DEF={CHK_HighPrize=0xfb0,CHK_MidPrize=0xfac,CHK_LowPrize=0xfa8}')
    functions = []
    headers = [line.strip() for line in source.splitlines()
               if ':ReadGlobalGameDataFloat(' in line and line.strip().startswith('if not (')]
    assert len(headers) == 3
    for line in headers:
        assert line.startswith('if ') and line.endswith(' then')
        condition = line[3:-5]
        functions.append(lua.execute('return function(raw,threshold) local '+integer+'=raw;local '+floating+
            ';local quest={ReadGlobalGameDataFloat=function() return threshold end};'+store+
            ';return ('+condition+'),string.pack("<f",'+floating+') end'))
    for score, threshold, stage, expected, stored in native_cases:
        for raw in (score,score&0xffffffff):
            result = functions[stage](raw,threshold)
            assert result[0] is expected
            assert result[1] == stored
