"""Extract direct state/data offsets used by Guild native lifecycle bodies."""
from __future__ import annotations

import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CLUSTERS = ROOT / 'refs/script_recovery/native_clusters'
TU = ROOT / 'refs/script_recovery/guild_training/translation_unit.json'
OUT = ROOT / 'refs/script_recovery/guild_training/state_access.json'


def build():
    translation = json.loads(TU.read_text(encoding='utf-8-sig'))
    by_address = {f['address'].lower(): f for f in translation['functions']}
    rows = []
    for cluster_path in sorted(CLUSTERS.glob('Q_GuildTraining*.json')):
        cluster = json.loads(cluster_path.read_text(encoding='utf-8-sig'))
        script = cluster['script']
        for life in cluster['lifecycle']:
            fn = by_address.get(life['address'].lower())
            if not fn or not fn.get('decompile'):
                continue
            source = fn['decompile']
            offsets = {}
            for match in re.finditer(r'(?:this|param_1) \+ 0x([0-9a-fA-F]+)', source):
                offset = int(match.group(1), 16)
                offsets.setdefault(f'0x{offset:X}', {'count': 0, 'context': []})
                entry = offsets[f'0x{offset:X}']; entry['count'] += 1
                context = source[max(0, match.start()-90):match.end()+100].replace('\r',' ').replace('\n',' ')
                if context not in entry['context'] and len(entry['context']) < 3:
                    entry['context'].append(context)
            indirect = []
            compact = re.sub(r'\s+', ' ', source)
            for match in re.finditer(r'\*\([^)]*\)\(\*\(int \*\)\((?:this|param_1) \+ 0x([0-9a-fA-F]+)\) \+ 0x([0-9a-fA-F]+)\)', compact):
                indirect.append({'ownerOffset': f'0x{int(match.group(1),16):X}',
                                 'fieldOffset': f'0x{int(match.group(2),16):X}',
                                 'expression': match.group(0)})
            if offsets or indirect:
                rows.append({'script': script, 'role': life['role'], 'address': life['address'],
                             'offsets': offsets, 'indirectStateAccess': indirect})
    inventory_path = ROOT / 'refs/script_recovery/guild_training/inventory.json'
    if inventory_path.is_file():
        inventory = json.loads(inventory_path.read_text(encoding='utf-8-sig'))
        for quest in inventory['quests']:
            for entity in quest['entities']:
                for role, address in entity['functions'].items():
                    fn = by_address.get(address.lower())
                    if not fn or not fn.get('decompile'):
                        continue
                    source = fn['decompile']; compact = re.sub(r'\s+', ' ', source)
                    offsets = {}
                    for match in re.finditer(r'(?:this|param_1) \+ 0x([0-9a-fA-F]+)', compact):
                        key = f'0x{int(match.group(1),16):X}'
                        offsets.setdefault(key, {'count': 0, 'context': []})['count'] += 1
                    indirect = [{'ownerOffset': f'0x{int(a,16):X}', 'fieldOffset': f'0x{int(b,16):X}'}
                                for a, b in re.findall(r'\*\([^)]*\)\(\*\(int \*\)\((?:this|param_1) \+ 0x([0-9a-fA-F]+)\) \+ 0x([0-9a-fA-F]+)\)', compact)]
                    if offsets or indirect:
                        rows.append({'script': quest['script'] + '.' + entity['name'], 'role': role,
                                     'address': address, 'offsets': offsets, 'indirectStateAccess': indirect})
    result = {'schema': 'guild-native-state-access/0.1',
              'sourceTranslationUnit': str(TU), 'lifecycleFunctions': len(rows), 'functions': rows,
              'limits': 'Offsets are native evidence only; semantic field names require class/layout and runtime confirmation.'}
    OUT.write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({'lifecycleFunctions': len(rows),
                      'indirectAccesses': sum(len(r['indirectStateAccess']) for r in rows),
                      'output': str(OUT)}, indent=2))
    return result


if __name__ == '__main__':
    build()
