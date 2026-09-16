"""Build per-quest conversion evidence ("units") for a native quest translation unit.

Inputs (all already on disk for Guild training):
  inventory.json        entity bindings, lifecycle slots, thread registrations (retail addresses)
  native_clusters/*.json quest lifecycle slots (retail addresses)
  translation_unit.json retail decompiles + calls + strings for the whole range
  Ego_r-pdb-locals.tsv  debug-build function symbols (qualified name, RVA, size)
  ego_r.exe             debug-build image; used only to read string references per function
  struct_layouts_egor.tsv PDB class layouts (quest + entity fields)

Output: refs/script_recovery/<unit>/units/<Script>.json, one per quest, with
  quest.functions   name -> {address, evidence}   (lifecycle + threads + transitively called helpers)
  quest.fields      retail this-offset -> [name, kind]
  entities[name]    {nativeClass, functions, fields}

Helper names come from the debug PDB. A retail helper is matched to a PDB member function by the set
of string literals both reference (retail: translation unit; debug: pointers into ego_r .rdata). Ties
and empty string sets fall back to call order inside the caller, and are reported, never invented.
"""
from __future__ import annotations

import argparse
import json
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
LAYOUTS = ROOT / 'ghidra_out/struct_layouts_egor.tsv'
EGO_R = ROOT / 'debug_build/ego_r.exe'
KIND = {'bool': 'Bool', 'long': 'Int', 'int': 'Int', 'unsigned long': 'Int', 'unsigned int': 'Int',
        'short': 'Int', 'unsigned short': 'Int', 'char': 'Int', 'unsigned char': 'Int', 'float': 'Float',
        'CTimer': 'Int', 'CCharString': 'String', 'CWideString': 'String'}
LIFECYCLE = {'RegisterMain', 'Main', 'Init', 'OnPersist', 'destructor'}


def load_layouts(path=LAYOUTS):
    """class name -> list of {offset, type, name} (duplicate class names keep every copy, in order).
    load_layouts.sizes holds the PDB size of every class (first copy wins)."""
    classes, sizes, current = {}, {}, None
    for line in path.read_text(encoding='utf-8', errors='replace').splitlines():
        cols = line.split('	')
        if cols[0] == '@STRUCT':
            current = []
            classes.setdefault(cols[1], []).append(current)
            try:
                sizes.setdefault(cols[1], int(cols[2]))
            except (ValueError, IndexError):
                pass
        elif current is not None and len(cols) >= 5 and cols[0] != '@STRUCT':
            try:
                current.append({'offset': int(cols[2]), 'type': cols[3], 'name': cols[4]})
            except ValueError:
                pass
    load_layouts.sizes = sizes
    return classes


CONTAINER = re.compile(r'^(std::)?(vector|list|map|set|deque|multimap)<|^CVectorMap<')
QUEST_RETAIL_DELTA = -0x14   # retail CQuestScript base is 0x14 bytes smaller than the debug build's (Oakvale + Guild cross-checked)
THING_TYPES = {'CScriptThing'}
ARRAY = re.compile(r'^(.+?)\[(\d+)\]$')


def retail_size(layouts, name):
    """Retail size of a PDB class: PDB size minus 4 per STL container member."""
    size = load_layouts.sizes.get(name)
    if size is None:
        return None
    return size - 4 * sum(1 for f in layouts.get(name, [[]])[0] if CONTAINER.match(f['type']))


def expand_member(layouts, prefix, offset, ftype, fields, skipped, things, pdb_offset):
    """Map one member (scalar, thing, array or nested struct) onto retail offsets."""
    kind = KIND.get(ftype)
    if kind is None and (ftype.startswith('<unnamed-type-') or re.fullmatch(r'E[A-Z]\w+', ftype)):
        kind = 'Int'   # anonymous / named enums are 4-byte ints
    if kind is not None:
        fields[hex(offset)] = [prefix, kind]
        return
    if ftype in THING_TYPES:
        things[hex(offset)] = prefix
        return
    array = ARRAY.match(ftype)
    if array:
        element, count = array.group(1), int(array.group(2))
        element_size = 4 if KIND.get(element) else (
            12 if element in THING_TYPES else retail_size(layouts, element))
        if element_size is None:
            skipped.append({'offset': hex(offset), 'pdbOffset': hex(pdb_offset), 'type': ftype, 'name': prefix})
            return
        for index in range(count):
            expand_member(layouts, f'{prefix}_{index}', offset + index * element_size, element, fields, skipped, things, pdb_offset)
        return
    if ftype in layouts and not CONTAINER.match(ftype) and not ftype.endswith('*'):
        shift = 0
        for f in sorted(layouts[ftype][0], key=lambda f: f['offset']):
            if f['name'] == '_padding_' or f['type'].endswith('*'):
                if CONTAINER.match(f['type']):
                    shift += 4
                continue
            expand_member(layouts, f'{prefix}_{f["name"]}', offset + f['offset'] - shift, f['type'], fields, skipped, things, pdb_offset)
            if CONTAINER.match(f['type']):
                shift += 4
        return
    skipped.append({'offset': hex(offset), 'pdbOffset': hex(pdb_offset), 'type': ftype, 'name': prefix})


def array_descriptors(layouts, name, parent_class=None, delta=0):
    """Struct/scalar arrays of a class as {name, base, stride, count, members:{rel_off:[name,kind]}, things:{rel_off:name}}
    so dynamic indexing (idx*stride + base + member) can be lowered to keyed state names."""
    copies = layouts.get(name, [])
    if parent_class:
        copies = [c for c in copies if any(f['name'] == 'ParentClass' and f['type'].rstrip(' *') == parent_class for f in c)]
    if not copies:
        return []
    out, shift = [], 0
    for f in sorted(copies[0], key=lambda f: f['offset']):
        retail = f['offset'] + delta - shift
        if CONTAINER.match(f['type']):
            shift += 4
        array = ARRAY.match(f['type'])
        if not array or f['name'] == '_padding_':
            continue
        element, count = array.group(1), int(array.group(2))
        if KIND.get(element):
            out.append({'name': f['name'], 'element': element, 'base': hex(retail), 'stride': 4, 'count': count,
                        'members': {'0x0': ['', KIND[element]]}, 'things': {}})
        elif element in layouts:
            members, skipped, things, pointers = {}, [], {}, {}
            inner_shift = 0
            for m in sorted(layouts[element][0], key=lambda m: m['offset']):
                if m['type'] == element + ' *':
                    pointers[hex(m['offset'] - inner_shift)] = m['name']
                if m['name'] == '_padding_' or m['type'].endswith('*'):
                    if CONTAINER.match(m['type']):
                        inner_shift += 4
                    continue
                expand_member(layouts, m['name'], m['offset'] - inner_shift, m['type'], members, skipped, things, m['offset'])
                if CONTAINER.match(m['type']):
                    inner_shift += 4
            out.append({'name': f['name'], 'element': element, 'base': hex(retail), 'stride': retail_size(layouts, element), 'count': count,
                        'members': {hex(k): v for k, v in ((int(k, 16), v) for k, v in members.items())},
                        'things': {hex(k): v for k, v in ((int(k, 16), v) for k, v in things.items())},
                        'pointers': pointers})
    return out


def class_fields(layouts, name, parent_class=None, delta=0):
    """Fields of one PDB class copy at retail offsets. When several classes share a name,
    parent_class selects the copy. Returns (scalar fields, unmapped members, thing fields)."""
    copies = layouts.get(name, [])
    if parent_class:
        copies = [c for c in copies if any(f['name'] == 'ParentClass' and f['type'].rstrip(' *') == parent_class for f in c)]
    if not copies:
        return {}, [], {}
    fields, skipped, things = {}, [], {}
    shift = 0   # debug-build STL containers carry one extra (iterator-debugging) pointer each
    for f in sorted(copies[0], key=lambda f: f['offset']):
        retail = f['offset'] + delta - shift
        if CONTAINER.match(f['type']):
            shift += 4
        if f['name'] in ('_padding_', 'ParentClass', 'PMasterData'):
            continue
        if f['type'].endswith('*'):
            skipped.append({'offset': hex(retail), 'pdbOffset': hex(f['offset']), 'type': f['type'], 'name': f['name']})
            continue
        expand_member(layouts, f['name'], retail, f['type'], fields, skipped, things, f['offset'])
    return fields, skipped, things


def pdb_functions(tsv):
    out = {}
    for line in Path(tsv).read_text(encoding='utf-8', errors='replace').splitlines():
        cols = line.split('\t')
        if cols[0] == 'FUNCTION' and len(cols) >= 4:
            out[cols[1]] = (int(cols[2]), int(cols[3]))
    return out


class DebugImage:
    def __init__(self, path=EGO_R):
        self.data = Path(path).read_bytes()
        pe = struct.unpack_from('<I', self.data, 0x3c)[0]
        nsec = struct.unpack_from('<H', self.data, pe + 6)[0]
        opt = struct.unpack_from('<H', self.data, pe + 20)[0]
        self.base = struct.unpack_from('<I', self.data, pe + 24 + 28)[0]
        self.sections = []
        off = pe + 24 + opt
        for i in range(nsec):
            name = self.data[off + i * 40:off + i * 40 + 8].rstrip(b'\0').decode()
            vsize, va, rsize, raw = struct.unpack_from('<IIII', self.data, off + i * 40 + 8)
            self.sections.append((name, va, vsize, raw, rsize))

    def raw(self, rva):
        for _, va, vsize, raw, rsize in self.sections:
            if va <= rva < va + max(vsize, rsize):
                return raw + (rva - va)
        return None

    def strings_in(self, rva, size):
        """String literals referenced by absolute pointer from the function bytes."""
        start = self.raw(rva)
        found = set()
        if start is None:
            return found
        body = self.data[start:start + size]
        readable = {va: (va, vsize) for name, va, vsize, _, _ in self.sections if name in ('.rdata', '.data')}
        for i in range(len(body) - 3):
            value = struct.unpack_from('<I', body, i)[0] - self.base
            for va, vsize in readable.values():
                if va <= value < va + vsize:
                    raw = self.raw(value)
                    if raw is None:
                        break
                    end = self.data.find(b'\0', raw, raw + 256)
                    if end < 0:
                        break
                    text = self.data[raw:end]
                    if len(text) >= 4 and all(32 <= b < 127 for b in text):
                        found.add(text.decode())
                    break
        return found


def build_unit(script, inventory, cluster, tu_by_address, tu_range, pdb, image, layouts, package=None):
    class_name = f'C{script}Script'
    qualified = f'NScript::{class_name}'
    quest_inv = next(q for q in inventory['quests'] if q['script'] == script)
    functions = {}
    for slot in cluster['lifecycle']:
        role = slot['role']
        if role in LIFECYCLE:
            functions[role] = {'address': slot['address'].lower(), 'evidence': 'cluster lifecycle slot'}
    lo, hi = int(tu_range[0], 16), int(tu_range[1], 16)
    threads = {t['body'].lower(): t['name'] for t in inventory['threads']}
    thread_registration = {t['body'].lower(): t['registrationFunction'].lower() for t in inventory['threads']}
    # Every lifecycle address of every entity in the whole unit (never a "helper").
    entity_addrs = set()
    for q in inventory['quests']:
        for e in q['entities']:
            entity_addrs.update(str(a).lower() for a in e['functions'].values())
            entity_addrs.update(str(e[k]).lower() for k in ('factory', 'registrationFunction') if e.get(k))
    # Closure roots: quest lifecycle (destructor excluded: member destructors are not script) and
    # this quest's entity lifecycle. Each discovered in-range callee is a thread body or a helper;
    # helpers remember which root class reached them (quest, or an entity binding name).
    roots = [(v['address'].lower(), 'quest') for k, v in functions.items() if k != 'destructor']
    for e in quest_inv['entities']:
        roots += [(str(a).lower(), e['name']) for k, a in e['functions'].items() if k != 'destructor']
    seen = {a for a, _ in roots}
    pending = list(roots)
    helper_addrs = {}          # address -> {'callers': set(owner)}
    def visit(addr, owner):
        for call in tu_by_address.get(addr, {}).get('calls', []):
            target = str(call.get('target', '')).lower()
            if not target.startswith('0x') or target not in tu_by_address or target in entity_addrs:
                continue
            if not (lo <= int(target, 16) < hi):
                continue
            if target in threads:
                if threads[target] not in functions:
                    functions[threads[target]] = {'address': target, 'spawnedAs': threads[target],
                                                  'evidence': 'inventory thread registration'}
                new_owner = 'quest'
            else:
                helper_addrs.setdefault(target, {'callers': set()})['callers'].add(owner)
                new_owner = owner
            if target not in seen:
                seen.add(target)
                pending.append((target, new_owner))
    while pending:
        addr, owner = pending.pop(0)
        visit(addr, owner)
        # threads registered (stored) by this function but not directly called
        for body, reg in thread_registration.items():
            if reg == addr and body not in seen:
                seen.add(body)
                functions[threads[body]] = {'address': body, 'spawnedAs': threads[body],
                                            'evidence': 'inventory thread registration'}
                pending.append((body, 'quest'))
    # PDB members: quest-level (not nested, not lifecycle/threads/compiler) and per nested class.
    members, nested_members = {}, {}
    for qname, (rva, size) in pdb.items():
        if not qname.startswith(qualified + '::'):
            continue
        rest = qname[len(qualified) + 2:]
        if rest.startswith('`') or rest in ('Alloc',) or rest == class_name or rest == '~' + class_name:
            continue
        if '::' in rest:
            klass, fn = rest.split('::', 1)
            if fn in LIFECYCLE or fn in ('Alloc', 'GetParentScript', 'OnPredicateFail', 'OnInterrupted')                     or fn.startswith('`') or fn.startswith('~') or fn == klass:
                continue
            nested_members.setdefault(klass, {})[fn] = (rva, size)
        elif rest not in LIFECYCLE and rest not in threads.values():
            members[rest] = (rva, size)
    member_strings = {name: image.strings_in(rva, size) for name, (rva, size) in members.items()}

    def jaccard(a, b):
        return len(a & b) / len(a | b) if a and b else 0.0

    # Resolve each entity binding's PDB class first (needed to classify entity-level helpers).
    nested_main_strings = {}
    for qname, (rva, size) in pdb.items():
        if qname.startswith(qualified + '::C') and qname.endswith('::Main') and qname.count('::') == 3:
            nested_main_strings[qname.split('::')[2]] = image.strings_in(rva, size)
    binding_class, binding_evidence = {}, {}
    for e in quest_inv['entities']:
        main = str(e['functions'].get('Main', '')).lower()
        retail_strings = {s['value'] for s in tu_by_address.get(main, {}).get('strings', []) if s.get('value')}
        klass, how, best_score = 'C' + e['name'], 'name', 0.0
        if klass not in nested_main_strings:
            for candidate, strings in nested_main_strings.items():
                if jaccard(retail_strings, strings) > best_score:
                    klass, best_score = candidate, jaccard(retail_strings, strings)
            how = f'string-set match (jaccard {best_score:.2f})' if best_score > 0 else 'unresolved'
            if best_score == 0:
                klass = 'C' + e['name']
        binding_class[e['name']], binding_evidence[e['name']] = klass, how
    matches, unmatched, used = {}, [], set()
    entity_helpers = {}        # entity class -> {helper name: row}
    entity_helper_addrs = {}   # entity class -> [address]
    for addr, info in helper_addrs.items():
        owner_classes = {binding_class.get(c, c) for c in info['callers']}
        if 'quest' not in owner_classes and len(owner_classes) == 1:
            entity_helper_addrs.setdefault(next(iter(owner_classes)), []).append(addr)
            continue
        retail_strings = {s['value'] for s in tu_by_address[addr].get('strings', []) if s.get('value')}
        bsim = str(tu_by_address[addr].get('currentName', ''))
        bsim_name = bsim.split('::')[-1] if bsim.startswith(qualified + '::') else None
        best, best_score = None, 0.0
        for name, strings in member_strings.items():
            if name not in used and jaccard(retail_strings, strings) > best_score:
                best, best_score = name, jaccard(retail_strings, strings)
        if best and best_score >= 0.5:
            used.add(best)
            matches[best] = {'address': addr, 'evidence': f'PDB-name via string-set match (jaccard {best_score:.2f})',
                             'bsimAgrees': bsim_name == best, 'callers': sorted(info['callers'])}
        elif bsim_name and bsim_name in members and bsim_name not in used:
            used.add(bsim_name)
            matches[bsim_name] = {'address': addr, 'evidence': 'bsim label only; string sets inconclusive',
                                  'bsimAgrees': True, 'callers': sorted(info['callers'])}
        else:
            unmatched.append({'address': addr, 'callers': sorted(info['callers']), 'bestPdbCandidate': best,
                              'score': round(best_score, 2), 'retailStrings': sorted(retail_strings)[:8]})
    functions.update(matches)
    for klass, addrs in entity_helper_addrs.items():
        candidates = dict(nested_members.get(klass, {}))
        rows, leftovers = {}, []
        for addr in sorted(addrs):
            fn = tu_by_address[addr]
            retail_strings = {s['value'] for s in fn.get('strings', []) if s.get('value')}
            bsim = str(fn.get('currentName', ''))
            bsim_name = bsim.split('::')[-1] if bsim.startswith(f'{qualified}::{klass}::') else None
            best, best_score = None, 0.0
            for name, (rva, size) in candidates.items():
                score = jaccard(retail_strings, image.strings_in(rva, size))
                if score > best_score:
                    best, best_score = name, score
            if best and best_score >= 0.5:
                rows[best] = {'address': addr, 'evidence': f'PDB-name via string-set match (jaccard {best_score:.2f})'}
                candidates.pop(best)
            elif bsim_name and bsim_name in candidates:
                rows[bsim_name] = {'address': addr, 'evidence': 'bsim label agrees with a PDB nested member'}
                candidates.pop(bsim_name)
            else:
                leftovers.append(addr)
        # remaining: pair by size rank when one candidate is left per leftover of plausible size, else unnamed
        if candidates and leftovers:
            by_size = sorted(candidates.items(), key=lambda kv: kv[1][1])
            plausible = sorted((a for a in leftovers if tu_by_address[a]['size'] >= 64), key=lambda a: tu_by_address[a]['size'])
            if len(plausible) == len(by_size):
                for (name, (rva, size)), addr in zip(by_size, plausible):
                    rows[name] = {'address': addr, 'evidence': f'WEAK: size-rank pairing (retail {tu_by_address[addr]["size"]} B vs debug {size} B)'}
                    leftovers.remove(addr)
        for addr in leftovers:
            rows[f'helper_{addr[2:].lstrip("0").upper()}'] = {'address': addr, 'evidence': 'unnamed entity helper',
                                                               'size': tu_by_address[addr]['size']}
        entity_helpers[klass] = rows
    for row in unmatched:
        functions[f'helper_{row["address"][2:].lstrip("0").upper()}'] = {'evidence': 'unmatched retail helper', **row}
    fields, skipped, things = class_fields(layouts, class_name, delta=QUEST_RETAIL_DELTA)
    entities = {}
    for e in quest_inv['entities']:
        klass = binding_class[e['name']]
        ent_fields, ent_skipped, ent_things = class_fields(layouts, klass, parent_class=class_name)
        entities[e['name']] = {'nativeClass': f'{qualified}::{klass}', 'classEvidence': binding_evidence[e['name']],
                               'vtable': e.get('vtable'),
                               'functions': {k: {'address': str(v).lower()} for k, v in e['functions'].items()},
                               'helpers': entity_helpers.get(klass, {}),
                               'fields': ent_fields, 'thingFields': ent_things, 'unmappedFields': ent_skipped,
                               'arrays': array_descriptors(layouts, klass, parent_class=class_name)}
    return {'schema': 'quest-unit-evidence/1', 'script': script, 'package': package or script[2:],
            'nativeClass': qualified, 'allocator': quest_inv['allocator'], 'vtable': quest_inv['vtable'],
            'master': dict(zip(('fields', 'unmapped', 'things'), class_fields(layouts, 'CQ_SunnyvaleMasterData'))),
            'quest': {'functions': functions, 'fields': fields, 'thingFields': things, 'unmappedFields': skipped,
                      'arrays': array_descriptors(layouts, class_name, delta=QUEST_RETAIL_DELTA), 'retailOffsetDelta': QUEST_RETAIL_DELTA,
                      'pdbMembersUnused': sorted(set(members) - used), 'nestedPdbMembers': {k: sorted(v) for k, v in nested_members.items()}},
            'entities': entities}


def main():
    from tools.script_recovery.script_units import unit as script_unit
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--unit', default='guild_training')
    a.add_argument('--clusters', type=Path, default=ROOT / 'refs/script_recovery/native_clusters')
    args = a.parse_args()
    spec = script_unit(args.unit)
    evidence = spec['evidence']
    inventory = json.loads((evidence / 'inventory.json').read_text(encoding='utf-8-sig'))
    tu = json.loads((evidence / 'translation_unit.json').read_text(encoding='utf-8-sig'))
    by_address = {f['address'].lower(): f for f in tu['functions']}
    pdb, image, layouts = pdb_functions(evidence / 'pdb/Ego_r-pdb-locals.tsv'), DebugImage(), load_layouts()
    out = evidence / 'units'
    out.mkdir(exist_ok=True)
    summary = []
    for q in inventory['quests']:
        cluster = json.loads((args.clusters / f"{q['script']}.json").read_text(encoding='utf-8-sig'))
        unit = build_unit(q['script'], inventory, cluster, by_address, tu['range'], pdb, image, layouts)
        (out / f"{q['script']}.json").write_text(json.dumps(unit, indent=2) + '\n', encoding='utf-8')
        fns = unit['quest']['functions']
        summary.append({'script': q['script'], 'questFunctions': len(fns),
                        'unmatchedHelpers': sum(1 for f in fns.values() if f['evidence'] == 'unmatched retail helper'),
                        'pdbMembersUnused': unit['quest']['pdbMembersUnused'],
                        'questFields': len(unit['quest']['fields']), 'entities': len(unit['entities']),
                        'entityClasses': {n: (e['nativeClass'].split('::')[-1], e['classEvidence']) for n, e in unit['entities'].items()},
                        'entityFieldsTotal': sum(len(e['fields']) for e in unit['entities'].values())})
    print(json.dumps(summary, indent=2))


if __name__ == '__main__':
    main()
