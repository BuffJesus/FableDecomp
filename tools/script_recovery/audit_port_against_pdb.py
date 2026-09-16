"""Check a hand-ported FSE Lua package against the debug PDB: which native entities/threads/helpers
exist, and which string literals the native class references that the Lua never mentions.

    python tools/script_recovery/audit_port_against_pdb.py --pdb-tsv work/aeon_port_audit/CQ_WaspBossScript.tsv \
        --class NScript::CQ_WaspBossScript --package work/aeon_lua_ports/WaspBoss

Strings come from ego_r.exe (debug build compiled from the same script source as retail), so they
are evidence of behaviour the retail script also has. Absence in the Lua is a lead, not proof.
"""
import argparse, json, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.quest_unit_evidence import DebugImage, pdb_functions  # noqa: E402

NOISE = re.compile(r'^(\.\?AV|__|\?\?|\.PA|\.text|\.rdata|[A-Za-z]:\\|%|\.\.|\w{1,3}$)')


def main():
    a = argparse.ArgumentParser(description=__doc__)
    a.add_argument('--pdb-tsv', type=Path, required=True)
    a.add_argument('--class', dest='klass', required=True)
    a.add_argument('--package', type=Path, required=True)
    a.add_argument('--json', type=Path)
    args = a.parse_args()
    pdb = pdb_functions(args.pdb_tsv)
    image = DebugImage()
    lua = '\n'.join(p.read_text(encoding='utf-8', errors='replace') for p in args.package.rglob('*.lua'))
    lua_strings = set(re.findall(r'"([^"\n]{3,})"', lua)) | set(re.findall(r"'([^'\n]{3,})'", lua))
    members, entities = {}, {}
    for qname, (rva, size) in pdb.items():
        if not qname.startswith(args.klass + '::'):
            continue
        rest = qname[len(args.klass) + 2:]
        if rest.startswith('`') or rest in ('Alloc',) or 'CActiveEntityScript' in qname or 'CEntityScriptBinding' in qname \
                or 'CSpawnedFunc' in qname or rest.endswith('GetParentScript'):
            continue
        strings = {s for s in image.strings_in(rva, size) if not NOISE.match(s) and not s.startswith('NScript')}
        if '::' in rest:
            ent, fn = rest.split('::', 1)
            entities.setdefault(ent[1:] if ent.startswith('C') else ent, {})[fn] = strings
        else:
            members[rest] = strings
    lua_entities = {p.stem for p in args.package.rglob('Entities/*.lua')}
    report = {'class': args.klass, 'package': str(args.package), 'entities': {}, 'questFunctions': {}}
    print(f'== {args.klass}  (Lua files: {len(list(args.package.rglob("*.lua")))})')
    print('-- entities (native -> Lua file present?)')
    for ent, fns in sorted(entities.items()):
        present = ent in lua_entities or any(ent.lower() in e.lower() for e in lua_entities)
        strings = set().union(*fns.values())
        missing = sorted(s for s in strings if s not in lua_strings and not any(s in x for x in lua_strings))
        report['entities'][ent] = {'luaFile': present, 'nativeFunctions': sorted(fns), 'nativeStrings': sorted(strings), 'missingStrings': missing}
        print(f"   {ent:24s} lua={'Y' if present else 'NO'}  fns={','.join(sorted(fns))}  strings={len(strings)} missing={len(missing)}"
              + (f"  e.g. {missing[:4]}" if missing else ''))
    print('-- quest member functions (native -> name mentioned in Lua?)')
    for fn, strings in sorted(members.items()):
        mentioned = re.search(r'\b' + re.escape(fn) + r'\b', lua) is not None
        missing = sorted(s for s in strings if s not in lua_strings and not any(s in x for x in lua_strings))
        report['questFunctions'][fn] = {'mentioned': mentioned, 'nativeStrings': sorted(strings), 'missingStrings': missing}
        print(f"   {fn:24s} mentioned={'Y' if mentioned else 'no'}  strings={len(strings)} missing={len(missing)}"
              + (f"  e.g. {missing[:5]}" if missing else ''))
    all_native = set().union(*(set().union(*f.values()) for f in entities.values()), *members.values())
    all_missing = sorted(s for s in all_native if s not in lua_strings and not any(s in x for x in lua_strings))
    report['summary'] = {'nativeStrings': len(all_native), 'missingFromLua': len(all_missing), 'missing': all_missing}
    print(f'-- native strings {len(all_native)}, not found anywhere in Lua: {len(all_missing)}')
    for s in all_missing:
        print('     ', s)
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True); args.json.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')


if __name__ == '__main__':
    main()
