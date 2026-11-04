"""Explicit smoke-only CDefString vector fixtures; synthetic pointers, stock strings.

These fixtures model the x86 begin/end/capacity shape, not a live definition
object. Empty vectors stay empty. Scalar tunables retain the ordinary smoke
defaults. No fixture is installed in the game or used by generated Lua.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SCHEMA = 'script-definition-vector-fixture/1'
CONVERSATION_SCHEMA = 'script-definition-vector-fixture/2'
VALUE_SCHEMA = 'script-definition-vector-fixture/3'
ARENA_SCHEMA = 'script-definition-vector-fixture/4'
VECTOR = 'Vector_VCDefString__'


def decode_vectors(metadata, names):
    vectors = []
    for key, field in metadata.items():
        if field.get('defType') != VECTOR:
            continue
        if field.get('verified') is False or not field.get('retailTransferCall'):
            raise ValueError('definition vector lacks a native member anchor: ' + key)
        try:
            raw = bytes.fromhex(field['value'])
        except (KeyError, TypeError, ValueError) as exc:
            raise ValueError('invalid serialized definition vector: ' + key) from exc
        if len(raw) < 4:
            raise ValueError('truncated definition vector count: ' + key)
        count = struct.unpack_from('<I', raw)[0]
        if count > 4096 or len(raw) != 4 + count * 4:
            raise ValueError('definition vector count/size mismatch: ' + key)
        identifiers = struct.unpack_from('<' + 'I' * count, raw, 4)
        if any(identifier not in names for identifier in identifiers):
            raise ValueError('unresolved definition string token: ' + key)
        vectors.append(dict(offset=int(key, 0), name=field['name'],
                            items=[names[identifier] for identifier in identifiers]))
    fixture = dict(schema=SCHEMA, vectors=vectors)
    validate(fixture, metadata)
    return fixture


def validate(fixture, metadata):
    if not isinstance(fixture, dict) or fixture.get('schema') not in (SCHEMA, CONVERSATION_SCHEMA, VALUE_SCHEMA, ARENA_SCHEMA) or not isinstance(fixture.get('vectors'), list):
        raise ValueError('unsupported definition-vector fixture schema')
    if ('conversations' in fixture) != (fixture['schema'] in (CONVERSATION_SCHEMA, VALUE_SCHEMA, ARENA_SCHEMA)):
        raise ValueError('conversation fixture requires explicit schema version 2 or 3')
    if ('inlineStrings' in fixture) != (fixture['schema'] in (VALUE_SCHEMA, ARENA_SCHEMA)):
        raise ValueError('inline-string fixture requires explicit schema version 3')
    if ('arenaRounds' in fixture) != (fixture['schema'] == ARENA_SCHEMA):
        raise ValueError('Arena fixture requires explicit schema version 4')
    occupied = set()
    members = {int(key, 0) for key in metadata}
    for row in fixture['vectors']:
        if not isinstance(row, dict):
            raise ValueError('definition-vector fixture row must be an object')
        offset, items = row.get('offset'), row.get('items')
        if type(offset) is not int or offset < 4 or offset + 12 > 0x1088 or offset % 4:
            raise ValueError('invalid definition-vector member offset')
        field = metadata.get(hex(offset), {})
        if (field.get('defType') != VECTOR or field.get('name') != row.get('name')
                or field.get('verified') is False or not field.get('retailTransferCall')):
            raise ValueError('fixture member does not match native metadata')
        if not isinstance(items, list) or len(items) > 4096 or any(not isinstance(item, str) for item in items):
            raise ValueError('fixture items must be a bounded string list')
        words = {offset, offset + 4, offset + 8}
        if occupied & words or members & {offset + 4, offset + 8}:
            raise ValueError('overlapping definition-vector members')
        occupied |= words
    if 'conversations' in fixture:
        from tools.script_recovery.definition_conversation_fixture import validate as validate_conversations
        words = validate_conversations(fixture['conversations'], metadata)
        if occupied & words:
            raise ValueError('overlapping definition fixture members')
        occupied |= words
    if 'inlineStrings' in fixture:
        from tools.script_recovery.definition_inline_string_fixture import validate as validate_inline
        words = validate_inline(fixture['inlineStrings'], metadata)
        if occupied & words:
            raise ValueError('overlapping definition fixture members')
        occupied |= words
    if 'arenaRounds' in fixture:
        from tools.script_recovery.definition_arena_fixture import validate as validate_arena
        if occupied & validate_arena(fixture['arenaRounds'], metadata):
            raise ValueError('overlapping definition fixture members')
    return fixture


def to_lua(lua, fixture, metadata):
    validate(fixture, metadata)
    words, vectors = lua.table(), lua.table()
    for row in fixture['vectors']:
        offset, items = row['offset'], row['items']
        first = 0x10000000 + offset * 0x10000
        words[offset], words[offset+4], words[offset+8] = first, first+4*len(items), first+4*len(items)
        vectors[offset] = lua.table_from(items)
    result = lua.table(words=words, vectors=vectors)
    if 'conversations' in fixture:
        from tools.script_recovery.definition_conversation_fixture import to_lua as conversations_to_lua
        result.conversations = conversations_to_lua(lua, fixture['conversations'], metadata)
        for row in fixture['conversations']:
            offset = row['offset']
            first = 0x10000000 + offset * 0x10000
            words[offset], words[offset+4], words[offset+8] = first, first+0x5C*len(row['records']), first+0x5C*len(row['records'])
    if 'inlineStrings' in fixture:
        inline = lua.table()
        for row in fixture['inlineStrings']:
            offset, items = row['offset'], row['items']
            first = 0x10000000 + offset * 0x10000
            words[offset], words[offset+4], words[offset+8] = first, first+4*len(items), first+4*len(items)
            inline[offset] = lua.table_from(items)
        result.inline_strings = inline
    if 'arenaRounds' in fixture:
        from tools.script_recovery.definition_arena_fixture import flatten
        result.arena_rounds = lua.table_from(flatten(fixture['arenaRounds']))
        for row in fixture['arenaRounds']:
            offset = row['offset']
            first = 0x10000000 + offset * 0x10000
            words[offset], words[offset+4], words[offset+8] = first, first+0x38*len(row['rounds']), first+0x38*len(row['rounds'])
    return result


def load_fixture(path):
    from tools.script_recovery.script_def_offsets import load
    fixture = json.loads(Path(path).read_text(encoding='utf-8'))
    metadata = load()
    validate(fixture, metadata)
    hashes = fixture.get('sourceHashes', {})
    if not isinstance(hashes, dict):
        raise ValueError('invalid fixture source hashes')
    for name, digest in hashes.items():
        source = (ROOT / name).resolve()
        if (not source.is_relative_to(ROOT) or not source.is_file()
                or hashlib.sha256(source.read_bytes()).hexdigest() != digest):
            raise ValueError('definition-vector fixture source changed: ' + name)
    return fixture, metadata


def stock_fixture(*, include_conversations=False, include_values=False, include_arena_rounds=False):
    from tools.script_recovery.script_def_offsets import load
    sys.path.insert(0, str(ROOT / 'tools'))
    from parse_frontend import load_all, decode_entry
    metadata = load()
    source = ROOT / 'work/ui_proto/base'
    entries, schema, _ = load_all(str(source), str(ROOT / 'ghidra_out/def_schema.json'), 'script.bin')
    entry = next(row for row in entries if row['index'] == 597)
    decoded, _ = decode_entry(entry, schema)
    include_values = include_values or include_arena_rounds
    include_conversations = include_conversations or include_values
    supported = {VECTOR}
    if include_conversations: supported.add('vector<CConversation>')
    if include_values: supported.add('Vector_VCCharString__')
    if include_arena_rounds: supported.add('vector<CArenaRoundDef>')
    for field in metadata.values():
        if field.get('defType') in supported and field['value'] != decoded.get(field['name']):
            raise ValueError('metadata and stock snapshot differ: ' + field['name'])
    fixture = decode_vectors(metadata, entry['_names'])
    if include_conversations:
        from tools.script_recovery.definition_conversation_fixture import decode_conversations
        fixture['conversations'] = decode_conversations(metadata)
        fixture['schema'] = CONVERSATION_SCHEMA
        validate(fixture, metadata)
    if include_values:
        from tools.script_recovery.definition_inline_string_fixture import decode_vectors as decode_inline
        fixture['inlineStrings'] = decode_inline(metadata)
        fixture['schema'] = VALUE_SCHEMA
        validate(fixture, metadata)
    if include_arena_rounds:
        from tools.script_recovery.definition_arena_fixture import decode_vectors as decode_arena
        fixture['arenaRounds'] = decode_arena(metadata)
        fixture['schema'] = ARENA_SCHEMA
        validate(fixture, metadata)
    inputs = [ROOT / 'refs/script_recovery/script_def_offsets.json', ROOT / 'ghidra_out/def_schema.json',
              source / 'data/CompiledDefs/script.bin', source / 'data/CompiledDefs/names.bin']
    fixture['sourceHashes'] = {path.relative_to(ROOT).as_posix(): hashlib.sha256(path.read_bytes()).hexdigest()
                               for path in inputs}
    fixture['qualification'] = 'Stock snapshot strings and counts; synthetic x86 vector pointers; no gameplay proof.'
    if include_conversations:
        fixture['qualification'] = 'Stock snapshot strings/counts and conversation values; synthetic x86 vector pointers; no gameplay proof.'
    if include_values:
        fixture['qualification'] = 'Stock CDefString tokens, inline strings and conversation values; synthetic x86 vector pointers; scalar defaults unchanged; no gameplay proof.'
    if include_arena_rounds:
        fixture['qualification'] = 'Stock typed vectors, conversations and Arena round values; synthetic x86 pointers; scalar defaults unchanged; no level spawn markers or gameplay proof.'
    return fixture


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--include-conversations', action='store_true', help='Also decode anchored stock conversation values')
    parser.add_argument('--include-values', action='store_true', help='Also include inline CCharString vectors and conversation values (schema 3)')
    parser.add_argument('--include-arena-rounds', action='store_true', help='Also include Arena definition snapshots (schema 4); does not supply level spawn markers')
    args = parser.parse_args()
    fixture = stock_fixture(include_conversations=args.include_conversations, include_values=args.include_values, include_arena_rounds=args.include_arena_rounds)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(fixture, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(dict(vectors=len(fixture['vectors']),
                         empty=[row['name'] for row in fixture['vectors'] if not row['items']],
                         qualification=fixture['qualification'])))


if __name__ == '__main__':
    main()
