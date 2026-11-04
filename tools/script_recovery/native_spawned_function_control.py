"""Recover literal-name cancellation only from the witnessed retail ABI."""
import hashlib
import json
import re
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

PATTERN = re.compile(r'CCharString::CCharString\(&(?P<temporary>\w+),\s*(?P<name>"[^"\\]*"),\s*-1\);\s*CScriptBase::KillSpawnedFunction\(\(CScriptBase\s*\*\)this,\s*\(int\)&(?P=temporary),\s*0\);')
HELPER_HASH = '2cd4099a0f6e6b723e4a2905841a033e79a938dee63c119b7856154b033ec00c'
COMPARE_HASH = '3538aba2bf10b5c18df2ecfcefb32ca1ab44a96369aa4a04c68f43f2a69b047d'

def restore(text, fn, rdata):
    if not fn or rdata is None:
        return text
    sites = [c for c in fn.get('calls', []) if str(c.get('target', '')).lower() == '0x00cb7aa0']
    matches = list(PATTERN.finditer(text))
    if len(sites) != 1 or len(matches) != 1 or text.count('CScriptBase::KillSpawnedFunction(') != 1:
        return text
    for address, size, expected in ((0xcb7aa0, 92, HELPER_HASH), (0x411570, 40, COMPARE_HASH)):
        raw = rdata.bytes_at(address, size)
        if raw is None or hashlib.sha256(raw).hexdigest() != expected:
            return text
    site, match = sites[0], matches[0]
    address = int(site['site'], 16)
    raw = rdata.bytes_at(address - 24, 38) or b''
    if len(raw) != 38 or raw[:3] != bytes.fromhex('6aff68') or raw[7:12] != bytes.fromhex('8d4c2414e8') or raw[16:24] != bytes.fromhex('538d4c2410518bce') or raw[24] != 0xe8 or raw[29:34] != bytes.fromhex('8d4c240ce8'):
        return text
    for offset, target in ((11, 0x99ebf0), (24, 0xcb7aa0), (33, 0x99eae0)):
        if address - 24 + offset + 5 + struct.unpack_from('<i', raw, offset + 1)[0] != target:
            return text
    literal = rdata.string_at(struct.unpack_from('<I', raw, 3)[0])
    if literal != json.loads(match['name']):
        return text
    depth = site.get('depth')
    suffix = re.search(r'(?:Stack_|local_)([0-9a-f]+)$', match['temporary'])
    if not isinstance(depth, int) or suffix is None or site.get('pushedStack') != [None, 20 - depth] or int(suffix[1], 16) != depth - 20:
        return text
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    body = rdata.bytes_at(start, end - start)
    if body is None:
        return text
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(body, start))
    by_address = {i.address: i for i in instructions}
    receiver_writes = [i for i in instructions if 'esi' in [i.reg_name(r) for r in i.regs_access()[1]]]
    if not receiver_writes or receiver_writes[0].mnemonic != 'mov' or receiver_writes[0].op_str != 'esi, ecx' or receiver_writes[0].address != start + 5:
        return text
    if any(i.mnemonic != 'pop' or i.address <= address for i in receiver_writes[1:]):
        return text
    for instruction in instructions:
        if instruction.mnemonic.startswith('j'):
            try:
                target = int(instruction.op_str, 16)
            except ValueError:
                return text
            if address - 24 < target < address + 14:
                return text
    writes = [i for i in instructions if 'ebx' in [i.reg_name(r) for r in i.regs_access()[1]]]
    # The sole value-producing EBX write must be zero, before the call. The
    # epilogue restoration is allowed only after it.
    zeros = [i for i in writes if i.mnemonic == 'xor' and i.op_str == 'ebx, ebx']
    if len(zeros) != 1 or zeros[0].address >= address or any(i != zeros[0] and not (i.mnemonic == 'pop' and i.address > address) for i in writes):
        return text
    zero = zeros[0].address
    # Reject any entry-to-call path that bypasses the witnessed initialization.
    pending, visited = [start], set()
    while pending:
        current = pending.pop()
        if current == zero or current in visited:
            continue
        if current == address:
            return text
        visited.add(current)
        instruction = by_address.get(current)
        if instruction is None:
            return text
        if instruction.mnemonic.startswith('ret'):
            continue
        following = current + instruction.size
        if instruction.mnemonic.startswith('j'):
            try:
                target = int(instruction.op_str, 16)
            except ValueError:
                return text
            if target not in by_address:
                return text
            pending.append(target)
            if instruction.mnemonic == 'jmp':
                continue
        pending.append(following)
    replacement = f'ENGINE_KillSpawnedFunction({match["name"]}, 0);'
    return text[:match.start()] + replacement + text[match.end():]
