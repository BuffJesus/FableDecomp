"""Recover named CScriptDef member addresses from the pinned retail transfer method.

The PDB supplies candidate names/types, never retail member offsets. The retail
constructor's vtable identifies the transfer method; that method passes each
literal name together with its destination member. Reader bodies and field
values are outside this evidence boundary.
"""
from __future__ import annotations

import hashlib
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_ESI, X86_REG_ESP

START, LIMIT = 0xf2d6e0, 0xf333b0
TRANSFER_SHA256 = 'e2710eb7d1091926075533eb7f7e5c5d131644d955c07f34d89cd9259e604560'
NAMES_SHA256 = '4388784c8d529e29f6ea99ec53234350beb2942faa7fce33ea39afe440d667db'


def recover(image, names):
    """Return name -> {offset, call}; raise ValueError on incomplete evidence.

    This profile deliberately fails closed for other executable builds. The
    byte pin also bounds the two reviewed exceptional setup shapes below.
    """
    def require(condition, message):
        if not condition:
            raise ValueError('CScriptDef evidence: ' + message)

    raw = image.bytes_at(START, LIMIT - START)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == TRANSFER_SHA256,
            'unrecognized retail transfer bytes')
    require(image.bytes_at(0xf28fb9, 6) == bytes.fromhex('c706c8b52f01'),
            'constructor vtable store differs')
    require(image.bytes_at(0x12fb5c8 + 18 * 4, 4) == struct.pack('<I', START),
            'transfer vtable slot differs')
    require(image.bytes_at(0x12fb5c8 + 20 * 4, 4) == struct.pack('<I', 0xf29500)
            and image.bytes_at(0xf29500, 6) == bytes.fromhex('b888100000c3'),
            'retail object size evidence differs')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, START))
    require(instructions and instructions[-1].address + instructions[-1].size == LIMIT,
            'incomplete transfer disassembly')
    writes = [(ins.address, ins.mnemonic, ins.op_str) for ins in instructions
              if X86_REG_ESI in ins.regs_access()[1]]
    require(writes == [(0xf2d6e4, 'mov', 'esi, ecx'),
                       (0xf33391, 'add', 'esi, 0x1084'), (0xf333a5, 'pop', 'esi')],
            'receiver identity is not stable')
    fields = {}
    literals = {}
    repeated = []
    for index, ins in enumerate(instructions):
        if ins.mnemonic != 'push' or ins.operands[0].type != X86_OP_IMM:
            continue
        name = image.string_at(ins.operands[0].imm)
        if name not in names:
            continue
        # A second transfer of the same field reuses callee-preserved EBX.
        # The earlier direct LEA proves its address; do not invent a new field.
        if ins.address == 0xf31115:
            require(name == 'TemplePrayerFactorHighest' and name in fields,
                    'unexpected repeated transfer name')
            repeated.append(name)
            continue
        require(index >= 3 and index + 2 < len(instructions), 'truncated setup')
        lea, push = instructions[index-2:index]
        if push.mnemonic == 'xor' and push.op_str == 'ebx, ebx':
            lea, push = instructions[index-3:index-1]
        direct = (lea.mnemonic == 'lea' and len(lea.operands) == 2
                  and lea.operands[0].type == X86_OP_REG
                  and lea.operands[1].type == X86_OP_MEM
                  and lea.operands[1].mem.base == X86_REG_ESI
                  and lea.operands[1].mem.index == 0
                  and push.mnemonic == 'push' and push.operands[0].type == X86_OP_REG
                  and push.operands[0].reg == lea.operands[0].reg)
        tail = (lea.address == 0xf33391 and lea.mnemonic == 'add'
                and lea.op_str == 'esi, 0x1084' and push.mnemonic == 'push'
                and push.op_str == 'esi')
        receiver = instructions[index+1]
        call_index = index + 2
        while call_index < len(instructions):
            default = instructions[call_index]
            if not (default.mnemonic == 'mov' and default.operands[0].type == X86_OP_MEM
                    and default.operands[0].mem.base == X86_REG_ESP
                    and default.operands[0].mem.index == 0
                    and default.operands[0].mem.disp >= 8):
                break
            call_index += 1
        require(call_index < len(instructions), 'missing transfer call')
        call = instructions[call_index]
        require((direct or tail) and receiver.mnemonic == 'mov'
                and receiver.op_str == 'ecx, edi' and call.mnemonic == 'call'
                and call.operands[0].type == X86_OP_IMM,
                'unsupported member/name setup for ' + name)
        offset = lea.operands[1].mem.disp if direct else 0x1084
        require(4 <= offset < 0x1088 and offset % 4 == 0, 'invalid member address')
        require(name not in fields, 'duplicate named member')
        fields[name] = dict(offset=offset, call=call.address)
        literals[ins.operands[0].imm] = name
    require(set(fields) == set(names) and len(fields) == 943,
            'named PDB fields do not match retail transfers')
    require(len({row['offset'] for row in fields.values()}) == len(fields),
            'colliding member addresses')
    require(repeated == ['TemplePrayerFactorHighest'], 'missing repeated field transfer')
    strings = '\n'.join(f'{address:x}:{name}' for address, name in sorted(literals.items()))
    require(hashlib.sha256(strings.encode()).hexdigest() == NAMES_SHA256,
            'named-member literals differ')
    return fields
