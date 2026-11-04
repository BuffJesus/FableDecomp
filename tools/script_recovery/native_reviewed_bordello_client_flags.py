"""Restore BordelloClient's native stack-byte control flags."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_ESP


REWRITES = (
    ("-- TODO(native): xStack_e0_b3 = '\\x01';", 'xStack_e0_b3 = true',
     "unlifted: xStack_e0_b3 = '\\x01';"),
    ("-- TODO(native): xStack_c4_b3 = '\\0';", 'xStack_c4_b3 = false',
     "unlifted: xStack_c4_b3 = '\\0';"),
    ("-- TODO(native): xStack_c4_b3 = '\\x01';", 'xStack_c4_b3 = true',
     "unlifted: xStack_c4_b3 = '\\x01';"),
)
OLD_TEST = 'if xStack_c4_b3 ~= 0 then'
NEW_TEST = 'if xStack_c4_b3 then'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloClient flags {fn["address"]}: {why}')

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
                f'flag store changed at {site}')
    require(row['stores'] == {'0x00E450C5': [0x43, 1],
                              '0x00E453CB': [0x63, 0],
                              '0x00E453D2': [0x63, 1]},
            'reviewed flag sites changed')
    branch = indexed[0xE4541C]
    require(branch.mnemonic == 'je' and branch.operands[0].type == X86_OP_IMM
            and branch.operands[0].imm == 0xE45638,
            'zero flag branch changed')
    require(source.count(OLD_TEST) == 1
            and source.count('xStack_e0_b3 = bVar4') == 1
            and source.count('if not xStack_e0_b3 then') == 1,
            'lifted flag use changed')
    for old, new, diagnostic in REWRITES:
        require(source.count(old) == 1 and todo.count(diagnostic) == 1,
                f'lifted byte store changed: {diagnostic}')
        source = source.replace(old, new, 1)
        todo.remove(diagnostic)
    return source.replace(OLD_TEST, NEW_TEST, 1), todo
