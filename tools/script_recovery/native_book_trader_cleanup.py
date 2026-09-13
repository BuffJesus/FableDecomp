"""Expand reviewed cleanup jumps, preserving terminal versus normal continuation."""
import hashlib
import json
import re
from collections import Counter
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM


JUMP = re.compile(r'(?m)^([ \t]*)(?:if \(([^\r\n]*)\) )?goto (LAB_00db(?:46da|4f39|4f3f|4f45|4f4d|4f51|4bc5|4ce1));[ \t\r]*$')


def recover_book_trader_cleanup(function, source, rdata):
    return _recover_cleanup(function, source, rdata, 'native_book_trader_cleanup_witness.json', JUMP)


WIFE_JUMP = re.compile(r'(?m)^([ \t]*)(?:if \(([^\r\n]*)\) )?goto (LAB_00db(?:2e83|3293|32a5|3dd9|3de0|3def|3dff|3e05|3e0d|3e11|3d61));[ \t\r]*$')


def recover_affair_wife_cleanup(function, source, rdata):
    return _recover_cleanup(function, source, rdata, 'native_affair_wife_cleanup_witness.json', WIFE_JUMP)


def _recover_cleanup(function, source, rdata, filename, jump_pattern):
    witness = json.loads(Path(__file__).with_name(filename).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    base = int(witness['address'], 16)
    raw = rdata.bytes_at(base, witness.get('size', 4042))
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native cleanup evidence changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = {i.address: i for i in decoder.disasm(raw, base)}
    effects = ({int(site, 16): effect for site, effect in witness['effects'].items()}
               if 'effects' in witness else
               {0xDB46E0: 'unpause', 0xDB4F47: 'unpause', 0xDB4F51: 'movie_end',
                0xDB4BCD: 'unpause', 0xDB4CE1: 'movie_end'})
    for label, expected in witness['paths'].items():
        address, visited, actual = int(label[4:], 16), set(), []
        stop = int(witness['destinations'][label]['native'], 16)
        while address != stop:
            if address in visited or address not in instructions:
                return reject('cleanup path is not a terminal suffix')
            visited.add(address)
            ins = instructions[address]
            if ins.mnemonic == 'call':
                if address not in effects:
                    return reject('unknown cleanup effect')
                actual.append(effects[address])
            elif ins.mnemonic == 'jmp' and ins.operands[0].type == X86_OP_IMM:
                address = ins.operands[0].imm
                continue
            elif ins.mnemonic not in ('mov', 'lea', 'push'):
                return reject('cleanup path has unreviewed control or effect')
            address += ins.size
        if actual != expected:
            return reject('cleanup effect order changed')
    if Counter(m[3] for m in jump_pattern.finditer(source)) != Counter(witness['counts']):
        return reject('cleanup jump correspondence changed')
    def expand(match):
        indent, condition, label = match.groups()
        statements = ['GSI->PauseAllNonScriptedEntities(false);' if effect == 'unpause'
                      else 'GSI->EndMovieSequence();' for effect in witness['paths'][label]]
        statements.append('goto ' + witness['destinations'][label]['source'] + ';')
        if condition is not None:
            statements = ['if (' + condition + ') {'] + ['  ' + line for line in statements] + ['}']
        return '\n'.join(indent + line for line in statements)
    return jump_pattern.sub(expand, source), [dict(witness, status='recovered', actorCleanup='unresolved')]
