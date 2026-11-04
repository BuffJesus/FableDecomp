"""Apply reviewed stack-scalar token identities before object lifetime renaming.

The typed exporter has no source-address token map. Evidence therefore pins the
exported source and native body hashes, and identifies individual identifier
tokens with their native stack access witnesses. This can only rename operands;
it cannot replace expressions, calls, branches, or object lifetime operations.
Stale or malformed evidence stops conversion instead of producing plausible Lua.
"""
import hashlib
import re


def restore_scalar_operands(fn, evidence, rdata):
    source = fn['decompile']
    row = evidence.get(str(fn['address']).lower())
    if row is None:
        return source

    def require(condition, message):
        if not condition:
            raise ValueError(f"scalar operand evidence {fn['address']}: {message}")

    start, size = int(fn['address'], 16), int(fn['size'])
    raw = rdata.bytes_at(start, size)
    require(raw is not None and len(raw) == size, 'native bytes unavailable')
    require(hashlib.sha256(raw).hexdigest() == row['nativeSha256'], 'native body changed')
    require(hashlib.sha256(source.encode()).hexdigest() == row['sourceSha256'], 'exported source changed')
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from capstone.x86 import X86_OP_MEM, X86_REG_ESP
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = {i.address: i for i in decoder.disasm(raw, start)}
    edits, used = [], set()
    for binding in row['bindings']:
        offset = binding['entryOffset']
        require(isinstance(offset, int) and offset < 0 and offset % 4 == 0, 'invalid scalar slot')
        name = f'native_scalar_{-offset:x}'
        require(re.search(r'\b' + name + r'\b', source) is None, 'scalar name collision')
        witnesses = set()
        for witness in binding['accesses']:
            site = int(witness['site'], 16)
            ins = instructions.get(site)
            require(ins is not None and ins.mnemonic == 'mov', 'missing MOV witness')
            require(ins.bytes.hex() == witness['bytes'], 'access bytes changed')
            memory = [op for op in ins.operands if op.type == X86_OP_MEM]
            require(len(memory) == 1 and memory[0].size == 4, 'not a dword access')
            mem = memory[0].mem
            require(mem.base == X86_REG_ESP and not mem.index, 'not a plain ESP access')
            require(mem.disp + witness['entryEspDelta'] == offset, 'stack identity mismatch')
            witnesses.add(witness['site'])
        require(bool(witnesses), 'no native accesses')
        for token in binding['tokens']:
            pos, old = token['start'], token['name']
            require(re.fullmatch(r'[A-Za-z_]\w*', old) is not None, 'not an identifier')
            end = pos + len(old)
            require(source[pos:end] == old and
                    (pos == 0 or not re.match(r'\w', source[pos - 1])) and
                    (end == len(source) or not re.match(r'\w', source[end])), 'token mismatch')
            require(token['site'] in witnesses, 'token has no access witness')
            require(not used.intersection(range(pos, end)), 'overlapping tokens')
            used.update(range(pos, end))
            edits.append((pos, end, name))
    for a, b, replacement in sorted(edits, reverse=True):
        source = source[:a] + replacement + source[b:]
    return source
