"""Recover creation operands for both husband ambient conversation paths."""
import hashlib
import json
from pathlib import Path


def recover_affair_man_near_lines(function, source, rdata, manifest):
    return recover_affair_man_conversation(function, source, rdata, manifest,
        witness_file='native_affair_man_near_lines_witness.json')


def recover_affair_man_ranged_lines(function, source, rdata, manifest):
    return recover_affair_man_conversation(function, source, rdata, manifest,
        witness_file='native_affair_man_ranged_lines_witness.json')


def recover_affair_man_affection_setup(function, source, rdata, manifest):
    return recover_affair_man_conversation(function, source, rdata, manifest,
        witness_file='native_affair_man_affection_setup_witness.json')


def recover_affair_man_animation_operands(function, source, rdata, manifest):
    return recover_affair_man_conversation(function, source, rdata, manifest,
        witness_file='native_affair_man_animation_operands_witness.json')


def recover_affair_man_question(function, source, rdata, manifest):
    return recover_affair_man_conversation(function, source, rdata, manifest,
        witness_file='native_affair_man_question_witness.json')


def recover_affair_man_question_cleanup(function, source, rdata, manifest):
    return recover_affair_man_conversation(function, source, rdata, manifest,
        witness_file='native_affair_man_question_cleanup_witness.json')


def recover_affair_man_home(function, source, rdata, manifest):
    return recover_affair_man_conversation(function, source, rdata, manifest,
        witness_file='native_affair_man_home_witness.json')


def recover_affair_man_question_entry(function, source, rdata, manifest):
    return recover_affair_man_conversation(function, source, rdata, manifest,
        witness_file='native_affair_man_question_entry_witness.json')


def recover_affair_man_talk_continuation(function, source, rdata, manifest):
    return recover_affair_man_conversation(function, source, rdata, manifest,
        witness_file='native_affair_man_talk_continuation_witness.json')


def recover_affair_man_conversation(function, source, rdata, manifest, *,
                                    witness_file='native_affair_man_conversation_witness.json'):
    w=json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(function.get('address','')).lower()!=w['address'].lower():
        return source,[]
    def reject(reason):
        return source,[{'status':'rejected','reason':reason}]
    for value,key in ((function.get('decompile',''),'sourceSha256'),(source,'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest()!=w[key]:
            return reject(key+' changed')
    regions=w.get('regions') or [{'address':0xdb09e0,'size':5058,'sha256':w['bytesSha256']}]
    for region in regions:
        raw=rdata.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            return reject('native conversation evidence changed')
    if any(manifest.get(n)!=c for n,c in w['contracts'].items()):
        return reject('conversation contract changed')
    if any(source.count(e['old'])!=e['count'] for e in w['edits']):
        return reject('conversation source correspondence changed')
    for e in w['edits']:
        source=source.replace(e['old'],e['new'])
    return source,[dict(w,status='recovered')]
