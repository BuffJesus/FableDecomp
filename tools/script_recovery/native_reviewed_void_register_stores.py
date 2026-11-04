"""Discard reviewed decompiler EAX stores at a proven void native exit."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32

from tools.script_recovery.native_reviewed_function_prototype import reviewed_function_parameters


def restore(text, fn, unit, rdata):
    row = unit.get('reviewedVoidRegisterStores', {}).get(str(fn['address']).lower())
    if row is None:
        return text

    def require(ok, why):
        if not ok:
            raise ValueError('Reviewed void register store: ' + why)

    # This independently verifies the original source, full native body and PDB
    # signature. A guessed void signature is insufficient to discard a value.
    signature = reviewed_function_parameters(fn, unit, rdata)
    require(signature.get('reviewedVoid') is True, 'reviewed void prototype required')
    require(hashlib.sha256(text.encode()).hexdigest() == row['intermediateSha256'],
            'intermediate source changed')
    statement = row['statement']
    match = re.fullmatch(r'(iVar\d+(?:_\d+)?) = extraout_EAX(?:_\d+)?;', statement)
    require(match is not None, 'not a scalar EAX residue')
    require(re.search(r'\bint\s+' + re.escape(match[1]) + r'\s*;', fn['decompile']),
            'destination is not a local integer')
    require(text.count(statement) == 1, 'store is missing or ambiguous')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(rdata.bytes_at(start, end-start), start))
    site = int(row['producerCall'], 16)
    tail = [ins for ins in instructions if ins.address >= site]
    require(tail and tail[0].address == site and tail[0].mnemonic == 'call',
            'producer is not a native call')
    require(tail[0].bytes.hex() == row['producerBytes'], 'producer call changed')
    require(len(tail) >= 2 and tail[-1].mnemonic == 'ret'
            and tail[-1].address + tail[-1].size == end, 'not a terminal return')
    # Deliberately narrow: no branch, memory access, call, or EAX read is
    # permitted between the producer and return. Other epilogues stay unresolved.
    require(all(ins.mnemonic == 'pop' and ins.op_str in ('ebx', 'ecx', 'edx', 'esi', 'edi', 'ebp')
                for ins in tail[1:-1]), 'return register may be observed')
    result = text.replace(statement, '')
    require(hashlib.sha256(result.encode()).hexdigest() == row['recoveredSha256'],
            'recovered source changed')
    return result
