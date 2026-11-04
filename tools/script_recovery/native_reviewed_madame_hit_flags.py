"""Restore Madame's two retail hit-response byte flags."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_ESP


SITES = (
    ('LAB_00e3cab0', 'LAB_00e3cb3c', 'xStack_124_b3'),
    ('LAB_00e3dfb3', 'LAB_00e3e06a', 'xStack_f8_b3'),
)


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed Madame hit flags {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for address, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(address, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {address}')
    for site, (displacement, value) in row['stores'].items():
        ins = indexed[int(site, 16)]
        require(ins.mnemonic == 'mov' and ins.operands[0].type == X86_OP_MEM
                and ins.operands[0].mem.base == X86_REG_ESP
                and ins.operands[0].mem.disp == displacement
                and ins.operands[1].type == X86_OP_IMM
                and ins.operands[1].imm == value,
                f'hit flag store changed at {site}')
    require(row['stores'] == {'0x00E3CB35': [0x13, 0],
                              '0x00E3CB3C': [0x13, 1],
                              '0x00E3E063': [0x13, 0],
                              '0x00E3E06A': [0x13, 1]},
            'reviewed hit sites changed')
    for site, target in (('0x00E3CB98', 0xE3CDE7), ('0x00E3E0C9', 0xE3E2B0)):
        branch = indexed[int(site, 16)]
        require(branch.mnemonic == 'je' and branch.operands[0].type == X86_OP_IMM
                and branch.operands[0].imm == target,
                f'hit flag branch changed at {site}')
    for anchor, label, name in SITES:
        start_at = source.find('::' + anchor + '::')
        require(start_at >= 0 and source.count('::' + anchor + '::') == 1,
                f'lifted anchor changed: {anchor}')
        old_test = f'if {name} ~= 0 then'
        end_at = source.find(old_test, start_at)
        require(end_at >= 0 and '::' + label + '::' in source[start_at:end_at],
                f'lifted branch changed: {name}')
        end_at += len(old_test)
        segment = source[start_at:end_at]
        old_zero = f"-- TODO(native): {name} = '\\0';"
        old_one = f"-- TODO(native): {name} = '\\x01';"
        require(segment.count(old_zero) == segment.count(old_one) == segment.count(old_test) == 1,
                f'lifted byte stores changed: {name}')
        for literal in ('\\0', '\\x01'):
            diagnostic = f"unlifted: {name} = '{literal}';"
            require(todo.count(diagnostic) >= 1, f'diagnostic changed: {diagnostic}')
            todo.remove(diagnostic)
        segment = segment.replace(old_zero, f'{name} = false', 1)
        segment = segment.replace(old_one, f'{name} = true', 1)
        segment = segment.replace(old_test, f'if {name} then', 1)
        source = source[:start_at] + segment + source[end_at:]
    return source, todo
