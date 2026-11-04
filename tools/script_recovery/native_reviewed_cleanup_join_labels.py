"""Classify reviewed native cleanup joins after structured tail duplication.

Keep the address comment and Lua operations unchanged. A certificate pins the
complete native/exported/lifted bodies and the exact joined cleanup path.
"""
import hashlib
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32


def classify(source, todo, fn, row, rdata):
    if row is None:
        return source, list(todo), []

    def require(ok, reason):
        if not ok:
            raise ValueError(f'reviewed cleanup join {fn["address"]}: {reason}')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == int(row['address'], 16), 'function identity changed')
    raw = rdata.bytes_at(start, end - start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'], 'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'], 'exported source changed')
    body = re.fullmatch(r'(?:local )?function [^\n]+\n(.*?)\nend\s*', source, re.S)
    require(body is not None and hashlib.sha256(body[1].rstrip().encode()).hexdigest() == row['luaBodySha256'], 'structured Lua body changed')
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == end, 'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    updated = list(todo)
    reviewed = []
    for join in row['joins']:
        entry, limit = int(join['entry'], 16), int(join['endExclusive'], 16)
        label = join['label']
        require(label == f'LAB_{entry:08x}', 'label address changed')
        require(start <= entry < limit <= end and rdata.bytes_at(entry, limit - entry).hex() == join['bytes'], 'cleanup path changed')
        for edge in join['edges']:
            ins = indexed.get(int(edge['site'], 16))
            require(ins is not None and ins.mnemonic == edge['mnemonic'] and ins.bytes.hex() == edge['bytes'], 'native cleanup edge changed')
            require((edge['kind'] == 'branch' and ins.op_str == hex(entry)) or
                    (edge['kind'] == 'fallthrough' and ins.address + ins.size == entry and ins.mnemonic in ('je', 'jne')), 'cleanup edge does not reach entry')
        calls = {int(call['site'], 16): call for call in fn['calls']}
        for witness in join['calls']:
            site = int(witness['site'], 16)
            ins, call = indexed.get(site), calls.get(site)
            require(ins is not None and ins.mnemonic == 'call' and ins.op_str == hex(int(witness['target'], 16)), 'native cleanup call changed')
            require(call is not None and int(call['target'], 16) == int(witness['target'], 16) and
                    call.get('depth') == witness['depth'] and call.get('ecxStack') == witness['ecxStack'], 'cleanup receiver metadata changed')
        marker = f'-- {label}: (native jump target)'
        require(source.count(marker) == 1 and not re.search(r'\bgoto\s+' + re.escape(label) + r'\b|::' + re.escape(label) + r'::', source), 'cleanup label still has a Lua jump')
        diagnostic = 'label ' + label
        require(updated.count(diagnostic) == 1, 'label diagnostic changed')
        updated.remove(diagnostic)
        reviewed.append({'label': label, 'nativeEntry': join['entry'], 'classification': 'resolved structured cleanup join',
                         'addressCommentRetained': True, 'note': 'Reviewed native/source/Lua-shape certificate; separate from engine/gameplay proof.'})
    require(reviewed, 'empty cleanup join inventory')
    return source, updated, reviewed
