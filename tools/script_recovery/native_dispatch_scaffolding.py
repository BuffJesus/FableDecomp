"""Remove dead loads of the script interface/vtable after calls are resolved.

These loads select native dispatch tables; generated calls already use the Lua
quest receiver. Preserve the entire chain if any use survives call lowering.
This deliberately does not discard arbitrary entity/state/pointer reads.
"""
import re
import hashlib
import json
from pathlib import Path


def recover_barrel_dispatch(function, source, rdata):
    """Remove reviewed dispatch-only definitions despite earlier local reuse."""
    witness = json.loads(Path(__file__).with_name('native_barrel_dispatch_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(witness['address'], 16), witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native dispatch evidence changed')
    if any(source.count(edit['old']) != edit['count'] for edit in witness['edits']):
        return reject('dispatch-only definitions changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]


def prune_dispatch_loads(lines, *, entity):
    offset = '(?:4|0x4)' if entity else '(?:64|0x40)'
    seed = re.compile(r'\s*(?:local )?(\w+) = \*\((?:param_1|this) \+ ' + offset + r'\)\s*$')
    indirect = re.compile(r'\s*(?:local )?(\w+) = \*(\w+)\s*$')
    candidates = {i: (m[1], None) for i, line in enumerate(lines) if (m := seed.fullmatch(line))}
    if not candidates:
        return lines, []
    changed = True
    while changed:
        changed = False
        names = {name for name, _ in candidates.values()}
        for i, line in enumerate(lines):
            match = indirect.fullmatch(line)
            if i not in candidates and match and match[2] in names:
                candidates[i] = (match[1], match[2])
                changed = True
    names = [name for name, _ in candidates.values()]
    if len(set(names)) != len(names):
        return lines, []
    # Includes comments and literals as a conservative guard: an unresolved
    # native diagnostic mentioning a dispatch alias keeps that alias visible.
    outside = '\n'.join(line for i, line in enumerate(lines) if i not in candidates)
    if any(re.search(r'\b' + re.escape(name) + r'\b', outside) for name in names):
        return lines, []
    evidence = [{'line': i + 1, 'local': candidates[i][0], 'original': lines[i],
                 'reason': 'dead script-interface dispatch load; no surviving use'} for i in sorted(candidates)]
    return [line for i, line in enumerate(lines) if i not in candidates], evidence
