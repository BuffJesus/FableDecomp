"""Recover a reviewed signed-integer/float32 threshold chain and its x87 C0 branches."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, reason):
        if not ok:
            raise ValueError(f'reviewed x87 thresholds {fn["address"]}: {reason}')

    normalized = source.replace('\r\n', '\n').replace('\r', '\n')
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    for key, data in (('nativeSha256', raw), ('sourceSha256', fn['decompile'].encode()),
                      ('luaSha256', normalized.encode())):
        require(data is not None and hashlib.sha256(data).hexdigest() == row[key], key + ' changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    ins = list(decoder.disasm(raw, start))
    require(ins and ins[-1].address + ins[-1].size == end, 'incomplete native body')
    indexed = {i.address: i for i in ins}
    for address, encoded in row['nativeInstructions'].items():
        i = indexed.get(int(address, 16))
        require(i is not None and i.bytes.hex() == encoded, 'instruction changed')

    def reg(operand, expected):
        return operand.type == x86.X86_OP_REG and operand.reg == expected

    def mem(operand, base, offset):
        return (operand.type == x86.X86_OP_MEM and operand.size == 4
                and operand.mem.base == base and operand.mem.index == 0 and operand.mem.disp == offset)

    # A fixed local frame with saved general registers, before the first call.
    prologue = ins[:next(k for k, i in enumerate(ins) if i.mnemonic == 'call')]
    require(prologue[0].mnemonic == 'sub' and reg(prologue[0].operands[0], x86.X86_REG_ESP)
            and prologue[0].operands[1].type == x86.X86_OP_IMM, 'unsupported frame allocation')
    frame = prologue[0].operands[1].imm
    for i in prologue[1:]:
        if i.mnemonic == 'push':
            require(i.operands[0].type == x86.X86_OP_REG, 'non-register frame push')
            frame += 4
        else:
            require(i.mnemonic in ('mov', 'lea', 'xor') and
                    not (i.operands[0].type == x86.X86_OP_REG and i.operands[0].reg == x86.X86_REG_ESP),
                    'ambiguous frame setup')
    require(frame == row['frameBytes'], 'frame size changed')
    displacement = frame + row['entrySlot']
    integer, floating, receiver = row['integerSlot'], row['floatSlot'], row['receiver']
    require(all(re.fullmatch(r'[A-Za-z_]\w*', s) for s in (integer, floating, receiver)), 'invalid Lua identity')
    require(integer == f'xStack_{-row["entrySlot"]:x}' and floating == f'f_stk_{-row["entrySlot"]:x}',
            'stack identity changed')

    def sequence(first, last):
        found = [i for i in ins if first <= i.address < last]
        require(found and found[0].address == first and found[-1].address + found[-1].size == last,
                'instruction window changed')
        require(all(hex(i.address) in row['nativeInstructions'] for i in found), 'missing instruction witness')
        return found

    conversion = sequence(row['conversionStart'], row['comparisons'][0]['compare'])
    require([i.mnemonic for i in conversion] == ['fild', 'mov', 'fst'], 'integer conversion/store changed')
    require(mem(conversion[0].operands[0], x86.X86_REG_ESP, displacement)
            and mem(conversion[2].operands[0], x86.X86_REG_ESP, displacement), 'conversion stack slot changed')
    require(reg(conversion[1].operands[0], x86.X86_REG_ECX)
            and mem(conversion[1].operands[1], 0, row['globalPointer']), 'global pointer load changed')

    comparisons = row['comparisons']
    require(comparisons and len({part['offset'] for part in comparisons}) == len(comparisons), 'ambiguous threshold fields')
    old_conditions = []
    for k, part in enumerate(comparisons):
        block = sequence(part['start'], part['end'])
        if k:
            require(block[0].mnemonic == 'fld' and mem(block[0].operands[0], x86.X86_REG_ESP, displacement),
                    'float32 reload changed')
            block = block[1:]
        compare = block.pop(0)
        require(compare.address == part['compare'] and compare.mnemonic == 'fcomp'
                and mem(compare.operands[0], x86.X86_REG_ECX, part['offset']), 'threshold compare changed')
        # The last compare may restore a receiver after consuming the global pointer.
        if block[0].mnemonic == 'mov':
            require(k == len(comparisons) - 1 and reg(block[0].operands[0], x86.X86_REG_ECX)
                    and block[0].operands[1].type == x86.X86_OP_REG, 'intervening receiver restore changed')
            block = block[1:]
        require([i.mnemonic for i in block] == ['fnstsw', 'test', 'jne'], 'C0 branch shape changed')
        status, test, branch = block
        require(reg(status.operands[0], x86.X86_REG_AX) and reg(test.operands[0], x86.X86_REG_AH)
                and test.operands[1].type == x86.X86_OP_IMM and test.operands[1].imm == 1,
                'C0 status mask changed')
        require(branch.operands[0].type == x86.X86_OP_IMM and branch.operands[0].imm == part['lowerTarget'],
                'lower branch target changed')
        if k + 1 < len(comparisons):
            require(part['lowerTarget'] == comparisons[k + 1]['start'], 'threshold chain changed')
        condition = floating + ' < ' + receiver + ':ReadGlobalGameDataFloat(' + hex(part['offset']) + ')'
        require(normalized.count('if ' + condition + ' then') == 1, 'Lua comparison changed')
        old_conditions.append(condition)

    old_store = floating + ' = ' + integer
    require(len(re.findall(r'^\s*' + re.escape(old_store) + r'\s*$', normalized, re.M)) == 1, 'Lua conversion store changed')
    signed = '((' + integer + ' + 2147483648) % 4294967296 - 2147483648)'
    narrowed = '(string.unpack("<f", string.pack("<f", ' + signed + ')))'
    normalized = re.sub(r'^(\s*)' + re.escape(old_store) + r'\s*$',
                        lambda m: m[1] + floating + ' = ' + narrowed, normalized, flags=re.M)
    for k, condition in enumerate(old_conditions):
        right = condition.split(' < ', 1)[1]
        operand = signed if k == 0 else floating
        normalized = normalized.replace('if ' + condition + ' then', 'if not (' + operand + ' >= ' + right + ') then')
    return normalized
