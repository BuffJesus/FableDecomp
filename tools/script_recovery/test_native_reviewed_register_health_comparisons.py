import copy
import functools
import json
import struct
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_FPCW

from types import SimpleNamespace
from tools.script_recovery import native_health_copy_correspondence as COPY
from tools.script_recovery import native_reviewed_register_health_comparisons as LOWER
from tools.script_recovery.lift_native_lua import RData
ROOT = Path(__file__).resolve().parents[2]
FIXTURE = Path(__file__).resolve().parent/'fixtures/beardy_ordered_comparison'

def inputs():
    image=RData()
    if not image.ok:
        pytest.skip('original retail image unavailable for native comparison proof')
    phase=json.loads((FIXTURE/'phase.json').read_text(encoding='utf-8'))
    unit=json.loads((ROOT/'refs/script_recovery/beardy_baldy/units/V_BeardyBaldy.json').read_text(encoding='utf-8'))
    row=unit['reviewedRegisterHealthComparisons']['0x00e50fd0']
    return dict(source=(FIXTURE/'phase.lua').read_text(encoding='utf-8'),todo=phase['todo'],calls=phase['calls'],
        fn=phase['fn'],row=row,unit=unit,owner=row['owner'],image=image,manifest={'GetHealth':phase['hostContract']},
        before=phase['before'],after=phase['after'])

C=SimpleNamespace(LOWER=LOWER,COPY=COPY,inputs=inputs)

def test_default_without_retail_or_metadata():
    assert LOWER.recover('source',[],[],{},None,{},'owner',None,{},[],[])==('source',[],[],[])



@pytest.fixture(scope='module')
def data():
    # Replay each distinct real frame once. Forged frame/native inputs generate
    # a different key and cannot borrow the accepted result.
    original = C.LOWER.prove_frame
    cache = {}

    def checked(raw, fn, image, **kw):
        key = (raw, C.LOWER.serial(fn), C.LOWER.serial(kw), id(image))
        if key not in cache:
            cache[key] = original(raw, fn, image, **kw)
        return cache[key]

    C.LOWER.prove_frame = checked
    result = C.inputs()
    yield result
    C.LOWER.prove_frame = original


def clone(data):
    return {k: v if k == 'image' else copy.deepcopy(v) for k, v in data.items()}


def reseal(data, key):
    value = data[key].encode() if key == 'source' else C.LOWER.serial(data[key])
    name = dict(source='luaPhaseSha256', fn='fnSha256', before='beforeSha256',
                after='afterSha256', todo='todoSha256', calls='callsSha256')[key]
    data['row'][name] = C.LOWER.digest(value)


def test_exact_comparison_only(data):
    out, todo, calls, reviews = C.LOWER.recover(**data)
    assert out == data['source'].replace('if fret_10 <= fVar4 then', 'if not (fret_10 > fVar4) then', 1)
    assert todo == data['todo'] and calls == data['calls']
    assert len(reviews) == 1 and reviews[0]['luaFalseLabel'] == 'LAB_00e52d63_c38'
    LuaRuntime().execute(out)


def test_default_identity(data):
    d = clone(data)
    d['row'] = None
    assert C.LOWER.recover(**d) == (d['source'], d['todo'], d['calls'], [])


def test_original_default_spacing_guard_retained(data):
    with pytest.raises(ValueError, match='binding/normalization correspondence'):
        C.COPY.prove(data['fn'], data['before'], data['after'], data['source'])


@pytest.mark.parametrize('key', ['nativeSha256', 'fnSha256', 'luaPhaseSha256', 'todoSha256', 'callsSha256', 'beforeSha256', 'afterSha256'])
def test_stale_input_pins(data, key):
    d = clone(data)
    d['row'][key] = '0' * 64
    with pytest.raises(ValueError, match=key):
        C.LOWER.recover(**d)


@pytest.mark.parametrize('field,value', [('owner', 'Other'), ('entityVtable', '0x1'),
    ('actorProfile', 'entry-ebp'), ('scopes', []), ('allowWrappedCallSpacing', 1)])
def test_forged_review(data, field, value):
    d = clone(data)
    d['row'][field] = value
    with pytest.raises((ValueError, AssertionError)):
        C.LOWER.recover(**d)


@pytest.mark.parametrize('field,value', [('falseExit', '0xe52474'), ('positiveExit', '0xe52d63'),
    ('threshold', 1), ('unordered', True), ('flagRegister', 'AL'), ('nativeNodeSha256', '0' * 64)])
def test_forged_native_predicate(data, field, value):
    d = clone(data)
    d['row']['scopes'][0]['predicate'][field] = value
    with pytest.raises((ValueError, AssertionError)):
        C.LOWER.recover(**d)


@pytest.mark.parametrize('old,new', [('if fret_10 <= fVar4 then', 'if fret_10 < fVar4 then'),
    ('-- LAB_00e52d63_c38:', '-- LAB_00e52d63_c39:'),
    ('fret_10 = quest:GetHealth(pCVar10)', 'fret_10 = quest:GetHealth(me)'),
    ('fVar4 = 0.0\n                    if fret_10', 'fVar4 = 1.0\n                    if fret_10')])
def test_resealed_lua_forgery(data, old, new):
    d = clone(data)
    assert old in d['source']
    d['source'] = d['source'].replace(old, new, 1)
    reseal(d, 'source')
    with pytest.raises((ValueError, AssertionError)):
        C.LOWER.recover(**d)


def test_resealed_original_branch_target_forgery(data):
    d = clone(data)
    old = 'if (extraout_ST0_10 <= fVar4) goto LAB_00e52d63;'
    assert old in d['fn']['decompile']
    d['fn']['decompile'] = d['fn']['decompile'].replace(old, old.replace('e52d63', 'e52474'))
    reseal(d, 'fn')
    with pytest.raises((ValueError, AssertionError)):
        C.LOWER.recover(**d)


def test_duplicate_scope_rejected(data):
    d = clone(data)
    d['row']['scopes'] *= 2
    with pytest.raises(ValueError, match='duplicate scope'):
        C.LOWER.recover(**d)


BITS = [0, 0x80000000, 1, 0x80000001, 0x3f800000, 0xbf800000,
        0x7f800000, 0xff800000, 0x7fc00000, 0x7f800001, 0xffc12345]


@pytest.mark.parametrize('bits', BITS)
@pytest.mark.parametrize('clobber', [False, True])
def test_original_native_branch_and_lua_nan_correction(data, bits, clobber):
    # Execute the original 11-node x87/BL branch. GetHealth is represented by
    # the raw float32 placed in ST0; cleanup is a normal callee-saved ret stub.
    # Full native getter/copy/cleanup paths remain separate W449 evidence.
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for page in [0xe52000, 0x122d000, 0x4aa000, 0x200000]:
        u.mem_map(page, 4096)
    begin, end = 0xe5244d, 0xe52474
    u.mem_write(begin, data['image'].bytes_at(begin, end - begin))
    u.mem_write(0x122dedc, bytes(4))
    u.mem_write(0x200100, struct.pack('<I', bits))
    # fld dword [0x200100]; jmp begin
    u.mem_write(0x200000, b'\xd9\x05\x00\x01\x20\x00\xe9' + struct.pack('<i', begin - 0x20000b))
    stub = b'\xb8\xef\xbe\xad\xde\xb9\xef\xbe\xad\xde\xba\xef\xbe\xad\xde\xc3' if clobber else b'\xc3'
    u.mem_write(0x4aa840, stub)
    u.reg_write(UC_X86_REG_ESP, 0x200800)
    u.reg_write(UC_X86_REG_FPCW, 0x37f)

    def stop(uc, address, size, user):
        if address in [0xe52474, 0xe52d63]:
            uc.emu_stop()

    u.hook_add(UC_HOOK_CODE, stop)
    u.emu_start(0x200000, 0, count=40)
    native_exit = u.reg_read(UC_X86_REG_EIP) == 0xe52d63
    value = struct.unpack('<f', struct.pack('<I', bits))[0]
    lua = LuaRuntime()
    old = lua.eval('function(h) return h <= 0.0 end')(value)
    new = lua.eval('function(h) return not (h > 0.0) end')(value)
    assert new == native_exit
    nan = value != value
    assert (old != new) == nan
    assert u.reg_read(UC_X86_REG_ESP) == 0x200800
