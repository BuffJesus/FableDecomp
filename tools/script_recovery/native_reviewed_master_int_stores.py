"""Restore reviewed dword master-state stores obscured by an object cast."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86


def restore(source, todo, calls, fn, unit, rdata):
    row = unit.get('reviewedMasterIntStores', {}).get(str(fn['address']).lower())
    if row is None:
        return source, todo, calls

    def require(ok, why):
        if not ok:
            raise ValueError('Reviewed master integer store: ' + why)

    source = source.replace('\r\n', '\n').replace('\r', '\n')
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    for key, value in (('nativeSha256', raw), ('sourceSha256', fn['decompile'].encode()),
                       ('luaSha256', source.encode())):
        require(value is not None and hashlib.sha256(value).hexdigest() == row[key], key + ' changed')
    name, field, owner = row['fieldName'], row['fieldOffset'], row['ownerOffset']
    require(unit.get('master', {}).get('fields', {}).get(hex(field)) == [name, 'Int'],
            'mapped master field is not the reviewed integer')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == end,
            'incomplete native body')
    sites = set()
    for window in row['stores']:
        seq = [i for i in instructions if window['start'] <= i.address < window['end']]
        require(seq and seq[0].address == window['start']
                and seq[-1].address + seq[-1].size == window['end'], 'invalid store window')
        require(''.join(i.bytes.hex() for i in seq) == window['bytes'], 'store window changed')
        load, store = seq[0], seq[-1]
        require(load.mnemonic == 'mov' and len(load.operands) == 2, 'missing owner load')
        dest, memory = load.operands
        require(dest.type == x86.X86_OP_REG and memory.type == x86.X86_OP_MEM
                and memory.size == 4 and not memory.mem.index and memory.mem.disp == owner
                and load.reg_name(memory.mem.base) == row['ownerRegister'], 'owner identity changed')
        require(store.mnemonic == 'mov' and len(store.operands) == 2, 'missing dword store')
        target, value = store.operands
        require(target.type == x86.X86_OP_MEM and target.size == 4
                and target.mem.base == dest.reg and not target.mem.index and target.mem.disp == field
                and value.type == x86.X86_OP_REG and store.reg_name(value.reg) == row['valueRegister'],
                'store operand changed')
        require(store.address not in sites, 'duplicate store')
        sites.add(store.address)
        for ins in seq[1:-1]:
            require(ins.mnemonic in ('push', 'lea'), 'unsupported intervening operation')
            _, writes = ins.regs_access()
            require(dest.reg not in writes and value.reg not in writes, 'live store register clobbered')
    value, receiver = row['valueLocal'], row['receiver']
    require(all(re.fullmatch(r'[A-Za-z_]\w*', s) for s in (value, receiver, name)), 'invalid Lua identity')
    require(re.search(r'\blocal\s+[^\n]*\b' + re.escape(value) + r'\b', source), 'value is not local')
    require(source.count(value + ' = 0\n') == 1, 'counter initializer changed')
    for amount, count in row['increments'].items():
        require(source.count(f'{value} = {value} + {amount}\n') == count, 'counter increments changed')
    old = f'*(CCharString *)(*(int *)(this + {owner:#x}) + {field:#x}) = {value};'
    comment = '-- TODO(native): ' + old
    require(source.count(comment) == len(sites) and len(sites) > 0, 'Lua store count changed')
    # Native MOV stores low 32 bits; the mapped Int getter exposes signed32.
    signed = f'(({value} + 2147483648) % 4294967296 - 2147483648)'
    replacement = f'{receiver}:SetMasterGameState("{name}", {signed})'
    result = source.replace(comment, replacement)
    # Preserve unrelated diagnostics, including the still-unbound online score call.
    require(todo.count('unlifted: ' + old) == len(sites), 'diagnostic store count changed')
    remaining = [item for item in todo if item != 'unlifted: ' + old]
    return result, remaining, list(calls) + ['SetMasterGameState'] * len(sites)
