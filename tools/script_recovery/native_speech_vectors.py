"""Recover immutable villager speech tables from reviewed native Init construction."""
import hashlib
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'ghidra_out/script_recovery/native_quest_construction/0x00DAADD0.c'
SOURCE_HASH = '7901a4c00649b4dcc574ff067a2175d5e9ac9877f3e794ece13c9fe2593c9f91'
NATIVE_HASH = 'e40adc41d9f7625e7348ee34cc1bab9218eedbc7d85eb18405ff578166325f97'
KEY = re.compile(r'"(TEXT_QST_048_VILLAGER_DONE_[A-Z0-9_]+)"[^\n]*\n\s*\w+\s*=\s*[^\n]*\(param_1 \+ (0x[0-9a-f]+|\d+)\);')


def recover_vectors(read_bytes):
    source = SOURCE.read_bytes()
    native = read_bytes(0x00DAADD0, 3306)
    if hashlib.sha256(source).hexdigest() != SOURCE_HASH or native is None or hashlib.sha256(native).hexdigest() != NATIVE_HASH:
        raise ValueError('speech vector Init source/native evidence changed')
    vectors = {}
    for key, end in KEY.findall(source.decode()):
        vectors.setdefault(hex(int(end, 0) - 4), []).append(key)
    inventory = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/entities/NOVI_Villager.json').read_text())
    expected = {v['offset']: v['keys'] for k, v in inventory['textKeys'].items() if k.startswith('VillagerSpeech')}
    if vectors != expected or len(vectors) != 8:
        raise ValueError('speech vector construction differs from reviewed inventory')
    return vectors


def vector_prelude(vectors):
    return 'local __native_vectors = {\n' + '\n'.join(
        f'    [{offset}] = {{' + ', '.join(json.dumps(key) for key in keys) + '},'
        for offset, keys in vectors.items()) + '\n}\n'


def recover_init_vector_construction(function, source, rdata, vectors):
    """Replace reviewed immutable construction only when tables are materialized.

    The caller supplies the same static-vector contract used by speech consumers
    and must emit vector_prelude. This does not support mutable/repeated appends.
    """
    witness = json.loads(Path(__file__).with_name('native_speech_vector_init_witness.json').read_text())
    if str(function.get('address', '')).lower() != '0x00daadd0':
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    try:
        expected = recover_vectors(rdata.bytes_at)
    except ValueError as error:
        return reject(str(error))
    if vectors != expected:
        return reject('immutable speech tables not materialized')
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    if source.count(witness['old']) != 1:
        return reject('speech construction source changed')
    return source.replace(witness['old'], 'return;\n}\n'), [dict(witness, status='recovered',
        vectorCount=len(vectors), keyCount=sum(map(len, vectors.values())),
        storage='immutable __native_vectors tables emitted by converter')]
