"""Recover reviewed deed operands from pinned native code and retail SCRIPT_DEF data."""
import hashlib
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DATA_SHA256 = '3d6d09939b612478f70c6aa1eb45a5be0e850b27d534f78d996a00466501c1d9'


def morality_amount(root=ROOT):
    from tools.parse_frontend import decode_entry, load_all
    base = root / 'work/ui_proto/base'
    data = base / 'data/CompiledDefs/script.bin'
    if hashlib.sha256(data.read_bytes()).hexdigest() != DATA_SHA256:
        raise ValueError('retail script.bin changed')
    entries, schema, _ = load_all(str(base), str(root / 'ghidra_out/def_schema.json'), 'script.bin')
    entry = next(e for e in entries if e['index'] == 597)
    decoded, leftover = decode_entry(entry, schema)
    if entry['name'] != 'SCRIPT_DEF' or entry['definition'] != 'CScriptDef' or leftover:
        raise ValueError('SCRIPT_DEF decode changed')
    value = decoded['OVI_MoralityChangePerDeed']
    if not isinstance(value, (int, float)) or not 0 < value < 1:
        raise ValueError('invalid deed morality value')
    return value


def recover_deed_operands(function, source, read_bytes):
    witnesses = json.loads(Path(__file__).with_name('native_deed_witnesses.json').read_text())['functions']
    witness = next((w for w in witnesses if w['address'].lower() == str(function.get('address', '')).lower()), None)
    if witness is None:
        return source, []
    evidence = {'function': witness['address'], 'status': 'rejected'}
    for value, key in ((function.get('decompile', '').encode(), 'sourceSha256'),
                       (source.encode(), 'annotatedSha256'),
                       (read_bytes(int(witness['address'], 16), witness['size']), 'bytesSha256')):
        if value is None or hashlib.sha256(value).hexdigest() != witness[key]:
            return source, [dict(evidence, reason=key + ' changed or unavailable')]
    try:
        amount = morality_amount()
    except (OSError, ValueError, KeyError, StopIteration) as error:
        return source, [dict(evidence, reason=str(error))]
    load = (r'pCVar6 = \*\(CCharString \*\*\)\(DAT_0143e90c \+ 0xd64\);' if witness['sign'] == 1 else
            r'pCVar5 = \(CCharString \*\)-\*\(float \*\)\(DAT_0143e90c \+ 0xd64\);')
    pattern = load + r'\s*GSI->GiveHeroMorality\(\);'
    if len(list(re.finditer(pattern, source))) != 1:
        return source, [dict(evidence, reason='morality load/call shape changed')]
    source = re.sub(pattern, f'GSI->GiveHeroMorality({amount * witness["sign"]!r});', source)
    if witness['sign'] == -1:
        # At DAEA98, EDI receives the incoming stack argument ([esp+0xc] after two
        # pushes); DAEB4B/DAEB67/DAEBE2 use [ESI+EDI+0xfc]. Ghidra incorrectly
        # labels this argument unaff_retaddr. Full source/body hashes above pin the proof.
        source = source.replace('(unaff_retaddr + 0xfc)[(int)this]',
                                '*(undefined1 *)(this + 0xfc + native_arg_param_1)')
        # Phantom return-address/register aliases feed only incorrect decompiler
        # signatures for string cleanup and the reviewed one-argument logbook helper.
        source = re.sub(r'\bpCVar3 = unaff_retaddr;\s*', '', source)
        source = re.sub(r'ppVar4 = \(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> \*\)register0x00000010;\s*', '', source)
    return source, [dict(evidence, status='recovered', amount=amount * witness['sign'],
                         dataSha256=DATA_SHA256, nativeSha256=witness['bytesSha256'])]
