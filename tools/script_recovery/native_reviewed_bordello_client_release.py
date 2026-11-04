"""Restore BordelloClient's indexed `ClientInUse` release."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_EAX, X86_REG_ECX


OLD = '-- TODO(native): *(undefined1 *)(*(int *)(this + 0x20) + 0x58 + *(int *)(this + 0x14)) = 0;'
NEW = 'quest:SetStateBool(("ClientInUse_" .. (__native_entity_state:GetStateInt("ClientID") or 0)), false)'
DIAGNOSTIC = 'unlifted: *(undefined1 *)(*(int *)(this + 0x20) + 0x58 + *(int *)(this + 0x14)) = 0;'


def restore(source, todo, fn, row, rdata, quest, entity):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloClient release {fn["address"]}: {why}')

    require(quest['fields']['0x54'] == ['ClientsAlive', 'Int']
            and [quest['fields'][f'0x{0x58 + i:x}'] for i in range(3)] == [
                [f'ClientInUse_{i}', 'Bool'] for i in range(3)]
            and entity['fields']['0x20'] == ['ClientID', 'Int']
            and any(a['name'] == 'ClientInUse' and a['base'] == '0x58'
                    and a['stride'] == 1 and a['count'] == 3 and a['element'] == 'bool'
                    for a in quest['arrays']),
            'quest/entity field layout changed')
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
    store = indexed[0xE3B23C]
    mem, value = store.operands
    require(store.mnemonic == 'mov' and mem.type == X86_OP_MEM
            and mem.mem.base == X86_REG_EAX and mem.mem.index == X86_REG_ECX
            and mem.mem.disp == 0x58 and value.type == X86_OP_IMM and value.imm == 0,
            'indexed zero store changed')
    require(source.count(OLD) == 1 and todo.count(DIAGNOSTIC) == 1
            and source.count('quest:SetStateInt("ClientsAlive", quest:GetStateInt("ClientsAlive") + -1)') == 1,
            'lifted release changed')
    todo.remove(DIAGNOSTIC)
    return source.replace(OLD, NEW, 1), todo
