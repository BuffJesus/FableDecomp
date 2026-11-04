import copy
from pathlib import Path

import pytest
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

from tools.script_recovery.convert_quest_unit import UnitConverter, _text_order_sites
from tools.script_recovery.native_inline_self_argument import restore, CALLEE, CALLEE_SIZE

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def converter():
    return UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')


@pytest.fixture(params=['0x00e58d40', '0x00e5b3b0'])
def native(converter, request):
    fn = converter.native(request.param)
    source = fn['decompile']
    entries = _text_order_sites(source, fn)
    site = next(e for e in entries if e[4] == '0xd30')
    return fn, source, entries, site, converter.rdata


def test_inline_copy_preserves_call_and_reused_source_temporaries(native):
    fn, source, entries, site, data = native
    a, e = site[:2]
    expected = source[:a] + '(CScriptThing *)((int)this + 8),0' + source[e:]
    assert restore(source, fn, entries, data) == expected
    # By-value copy bookkeeping and other assignments remain for existing passes;
    # this recovery changes only the proven argument list.
    assert 'uVar8 = *(undefined4 *)((int)this + 0xc);' in expected


class ChangedBytes:
    def __init__(self, data, patches):
        self.data, self.patches = data, patches

    def bytes_at(self, address, size):
        raw = self.data.bytes_at(address, size)
        if raw is None:
            return None
        raw = bytearray(raw)
        for start, value in self.patches.items():
            for offset, byte in enumerate(value):
                at = start+offset-address
                if 0 <= at < len(raw):
                    raw[at] = byte
        return bytes(raw)


@pytest.mark.parametrize('kind', ['owner', 'data_field', 'info_field', 'vtable', 'refcount',
                                   'null_branch', 'null_target', 'zero_flag', 'slot',
                                   'callee', 'interface_table', 'interior_entry'])
def test_changed_or_incomplete_native_copy_is_not_recovered(native, kind):
    fn, source, entries, site, data = native
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    ins = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(data.bytes_at(start, end-start), start))
    last = next(k for k, op in enumerate(ins) if op.address == int(site[3]['site'], 16))
    block = ins[last-14:last+1]
    op = {'data_field':5, 'info_field':7, 'vtable':4, 'refcount':11,
          'null_branch':10, 'null_target':10, 'slot':14}.get(kind)
    if op is not None:
        at = block[op].address
        raw = bytearray(block[op].bytes)
        if kind == 'data_field':raw[-1] = 0x14
        elif kind == 'info_field':raw[-1] = 0xc
        elif kind == 'vtable':raw[-4] ^= 4
        elif kind == 'refcount':raw[-1] = 0x09
        elif kind == 'null_branch':raw[0] = 0x75
        elif kind == 'null_target':raw[-1] = 1
        elif kind == 'slot':raw[-4] ^= 4
    elif kind == 'owner':
        original = next(op for op in ins[:12] if op.mnemonic == 'mov' and op.op_str == 'ebp, ecx')
        at, raw = original.address, bytearray.fromhex('8bea')
    elif kind == 'zero_flag':
        flag = block[0].op_str
        original = next(op for op in ins[:20] if op.mnemonic == 'xor' and op.op_str == flag+', '+flag)
        at, raw = original.address, bytearray.fromhex('9090')
    elif kind == 'callee':at, raw = CALLEE, b'\x90'
    elif kind == 'interface_table':at, raw = 0x01260f0c+0xd30, b'\x00'*4
    elif kind == 'interior_entry':
        original = ins[last-15]
        assert original.mnemonic == 'jne' and original.size == 6
        at = original.address
        raw = original.bytes[:2] + (block[2].address-(at+6)).to_bytes(4,'little',signed=True)
    changed = ChangedBytes(data, {at:raw})
    assert restore(source, fn, entries, changed) == source


@pytest.mark.parametrize('kind', ['receiver', 'arity', 'flag', 'effect', 'not_vtable'])
def test_source_shape_and_metadata_must_match(native, kind):
    fn, source, entries, site, data = native
    altered = copy.deepcopy(entries)
    index = next(i for i, e in enumerate(altered) if e[4] == '0xd30')
    a, e, args, record, key, vtable = altered[index]
    if kind == 'receiver':record['ecxValue'] = 8
    elif kind == 'arity':args.pop()
    elif kind == 'flag':args[-1] = '1'
    elif kind == 'effect':args[1] = '++uVar8'
    elif kind == 'not_vtable':vtable = False
    altered[index] = (a,e,args,record,key,vtable)
    assert restore(source,fn,altered,data) == source


def test_native_callee_consumes_copy_and_flag(converter):
    ops = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(converter.rdata.bytes_at(CALLEE,CALLEE_SIZE),CALLEE))
    assert (ops[-1].mnemonic,ops[-1].op_str) == ('ret','0x10')
    assert any(op.mnemonic=='mov' and op.op_str=='byte ptr [eax + 0x79], cl' for op in ops)
