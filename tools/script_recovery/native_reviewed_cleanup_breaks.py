"""Restore a reviewed native cleanup return misprinted as a nested loop break."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone import x86


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError('reviewed cleanup break: ' + why)

    normalized = source.replace('\r\n', '\n').replace('\r', '\n')
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    for key, value in [('nativeSha256', raw), ('sourceSha256', fn['decompile'].encode()),
                       ('loweredSha256', normalized.encode())]:
        require(value is not None and hashlib.sha256(value).hexdigest() == row[key], key + ' changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == end, 'incomplete native body')
    indexed = {i.address: i for i in instructions}
    for address, encoded in row['nativeInstructions'].items():
        require(int(address, 16) in indexed and indexed[int(address, 16)].bytes.hex() == encoded,
                'native tail instruction changed')
    for address, encoded in row['helperBytes'].items():
        data = rdata.bytes_at(int(address, 16), len(encoded) // 2)
        require(data is not None and data.hex() == encoded, 'cleanup helper changed')
    movie_site, release_site, tail_end = row['movieSite'], row['releaseSite'], row['tailEnd']
    tail = [i for i in instructions if movie_site <= i.address < tail_end]
    require(len(tail) == 9 and [i.mnemonic for i in tail] ==
            ['call', 'lea', 'call', 'pop', 'pop', 'pop', 'pop', 'add', 'ret'], 'cleanup tail is not a return')
    require(tail_end <= end and tail[-1].address + tail[-1].size == tail_end, 'cleanup tail boundary changed')
    require(all(hex(i.address) in row['nativeInstructions'] for i in tail), 'incomplete cleanup tail witness')
    for instruction, target in [(tail[0], 0x6e7b80), (tail[2], 0x7e74d0)]:
        require(instruction.operands[0].type == x86.X86_OP_IMM and instruction.operands[0].imm == target,
                'cleanup target changed')
        require(hex(target) in row['helperBytes'], 'missing cleanup helper witness')
    lea = tail[1]
    dest, operand = lea.operands
    require(dest.type == x86.X86_OP_REG and dest.reg == x86.X86_REG_ECX
            and operand.type == x86.X86_OP_MEM and operand.mem.base == x86.X86_REG_ESP
            and operand.mem.index == 0 and operand.mem.disp - row['frameBytes'] == row['resourceSlot'],
            'cleanup resource storage changed')
    require(tail[2].address == release_site and
            [i.op_str for i in tail[3:7]] == ['edi', 'esi', 'ebp', 'ebx'], 'cleanup epilogue changed')
    require(tail[7].operands[0].type == x86.X86_OP_REG and tail[7].operands[0].reg == x86.X86_REG_ESP
            and tail[7].operands[1].type == x86.X86_OP_IMM
            and tail[7].operands[1].imm + 16 == row['frameBytes'], 'cleanup frame changed')
    calls = {int(c['site'], 16): c for c in fn['calls']}
    for site, target in [(movie_site, 0x6e7b80), (release_site, 0x7e74d0)]:
        call = calls.get(site)
        require(call is not None and int(call['target'], 16) == target and call.get('depth') == row['frameBytes'],
                'cleanup call metadata changed')
    require(calls[release_site].get('ecxStack') == row['resourceSlot'], 'cleanup receiver metadata changed')
    label, exit_label = f'LAB_{movie_site:08x}', f'LAB_{release_site:08x}'
    require(re.fullmatch(r'[A-Za-z_]\w*', row['movieAlias']) is not None, 'invalid movie alias')
    source_call = f'RESOURCE_DestroyMovie({row["movieAlias"]});'
    pattern = re.compile(r'(' + label + r':\n\s*' + re.escape(source_call) + r'\n\s*)break;')
    require(len(pattern.findall(normalized)) == 1 and normalized.count(exit_label + ':') == 1,
            'cleanup source is ambiguous')
    return pattern.sub(lambda m: m[1] + f'goto {exit_label};', normalized)
