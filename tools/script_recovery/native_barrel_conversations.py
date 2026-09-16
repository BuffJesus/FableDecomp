"""Restore the two conversation lines and temporary listener/string scopes."""
import hashlib
import json
from pathlib import Path


def recover(source,data):
    witness=json.loads(Path(__file__).with_name('native_barrel_conversations_witness.json').read_text())
    for region in witness['blocks']+witness['helpers']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Barrel conversation native instructions changed')
    for block in witness['blocks']:
        if data.string_at(block['literal'])!=block['text']:raise ValueError('Barrel conversation literal changed')
        if source.count(block['oldLua'])!=1:raise ValueError('Barrel conversation source correspondence changed')
        source=source.replace(block['oldLua'],block['newLua'],1)
    return source,dict(witness,status='recovered',semantics='Conversation(false,false), text CString, native empty Thing constructor, line(false,bound actor,constructor result), destroy Thing then CString. Host exceptions clean live temporaries preserving original error.')
