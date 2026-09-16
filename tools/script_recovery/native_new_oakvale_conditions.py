"""Restore verified native scheduler conditions before entity Main's first frame."""
import hashlib
import json
import re
from pathlib import Path

from tools.script_recovery.lift_native_lua import RData, ROOT


def verify(owner, rdata=None):
    witness = json.loads(Path(__file__).with_name('native_new_oakvale_conditions_witness.json').read_text())
    entries = [entry for entry in witness['entries'] if entry['owner'] == owner]
    if not entries:
        return None
    if len(entries) != 1:
        raise ValueError('ambiguous native entry condition')
    entry = entries[0]
    data = rdata or RData()
    unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
    fn = next(f for f in unit['functions'] if int(f['address'], 16) == int(entry['address'], 16))
    if hashlib.sha256(fn['decompile'].encode()).hexdigest() != entry['sourceSha256']:
        raise ValueError('native entry condition source changed')
    regions = witness['regions'] + [{'address': entry['address'], 'size': entry['entrySize'],
                                    'sha256': entry['entrySha256']}]
    for region in regions:
        raw = data.bytes_at(int(region['address'], 16), region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            raise ValueError('native entry condition bytes changed')
    return dict(entry, status='verified native scheduler condition',
                lifetime='registration clones bound Thing; caller copy destroyed before first frame')


def recover(owner, source, rdata=None):
    entry = verify(owner, rdata)
    if entry is None:
        return source, None
    if 'quest:RegisterBoundConsciousCondition(' in source or 'quest:RegisterBoundAliveCondition(' in source:
        raise ValueError('entry condition already present; review duplicate registration')
    main = re.search(r'(?m)^(?:local )?function (?:__resource_main|Main)\(quest, me(?:, resources)?\)\n', source)
    if main is None:
        raise ValueError('native entry condition Main correspondence changed')
    frame = re.search(r'(?m)^( *)alive = quest:NewScriptFrame\(me\)\n', source[main.end():])
    if frame is None:
        raise ValueError('native entry condition first frame changed')
    prefix = source[main.end():main.end()+frame.start()]
    # Only declarations/comments may precede registration. Nested cleanup helper
    # definitions in resource candidates are inert until called.
    if hashlib.sha256(prefix.encode()).hexdigest() not in entry['luaPrefixSha256s']:
        raise ValueError('native entry condition prefix correspondence changed')
    offset = main.end()+frame.start()
    result = source[:offset] + frame[1] + 'quest:' + entry['method'] + '()\n' + source[offset:]
    return result, entry
