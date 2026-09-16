"""Convert a native quest unit (quest + entities) to draft Lua from evidence files only.

This is the New Oakvale converter path (`convert_new_oakvale.py`) generalised: the same `Lifter`,
the same annotation and parameter recovery, but every input comes from a
`quest_unit_evidence.py` unit JSON instead of hand-curated Oakvale inventories, and no
per-function evidence hooks are consulted. Output layout mirrors the Oakvale draft:

    <out>/FSE/<Package>/<Package>.lua                quest lifecycle, threads, helpers
    <out>/FSE/<Package>/Entities/<Entity>.lua        one file per entity binding
    <out>/FSE/<Package>/native_quest_helpers.lua     quest helpers also called from entities
    <out>/CONVERSION_REPORT.json / .md

Registration stays disabled (`Quests = {}`); readable structuring and packaging are later passes.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.lift_native_lua import (  # noqa: E402
    Lifter, RData, annotate, known_callee_aliases, load_manifest, load_slots, load_thing_tables,
    thing_signatures, lift_persist,
)
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker  # noqa: E402
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters  # noqa: E402
from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower, finish_lua, strip_receiver_arguments, lower_after_annotate  # noqa: E402
from tools.script_recovery.annotate_interface_slots import load_thing_slots  # noqa: E402

ENTITY_STATE = '''local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end
'''
NUMBER_TYPES = ('long', 'int', 'uint', 'unsigned long', 'unsigned int', 'short', 'EBadDeeds')
SKIP_ROLES = {'destructor', 'GetParentScript', 'OnInterrupted', 'RegisterMain'}


def state_map(fields):
    return {offset.lower(): (name, kind) for offset, (name, kind) in fields.items()}


class UnitConverter:
    def __init__(self, tu_path, *, flat_control=False):
        self.manifest, self.slots, self.rdata = load_manifest(), load_slots(), RData()
        # sol::this_state is a binding artefact, never a Lua argument: drop it for arity checks.
        for spec in self.manifest.values():
            if isinstance(spec, dict) and 'parameters' in spec:
                spec['parameters'] = [p for p in spec['parameters'] if p.get('type') != 'sol::this_state']
        self.thing_slots = load_thing_slots()
        self.things, self.returning = load_thing_tables(self.manifest, self.slots)
        tu = json.loads(Path(tu_path).read_text(encoding='utf-8-sig'))
        self.by_address = {f['address'].lower(): f for f in tu['functions']}
        self.checker = LuaSyntaxChecker()
        self.flat_control = flat_control

    def native(self, address):
        fn = self.by_address.get(address.lower())
        return fn if fn and fn.get('decompile') else None

    def convert(self, unit, out):
        package = unit['package']
        quest_functions = {n: f for n, f in unit['quest']['functions'].items() if n not in SKIP_ROLES}
        quest_state = state_map(unit['quest']['fields'])
        helpers = {f['address'].lower(): n for n, f in quest_functions.items()
                   if n not in ('Main', 'Init', 'OnPersist')}
        report = {'schema': 'quest-unit-converter/1', 'script': unit['script'], 'package': package,
                  'packages': [], 'functions': [], 'missing': [], 'syntax': {},
                  'controlMode': 'flat experimental' if self.flat_control else 'structured draft'}
        all_sources, shared_names, shared_inputs = {}, set(), {}
        shared_module = f'{package}.native_quest_helpers'
        owners = [('quest', unit['script'], quest_functions, quest_state, {})]
        # Several bindings can share one native class (GuardTeamMember/BanditTeamMember -> CCrateTeamMember):
        # emit that class once and register every binding name against the shared file.
        by_class = {}
        for name, ent in unit['entities'].items():
            by_class.setdefault(ent['nativeClass'], []).append(name)
        binding_files, class_owner = {}, {}
        for klass, names in by_class.items():
            owner = names[0] if len(names) == 1 else klass.split('::')[-1][1:]
            for name in names:
                binding_files[name] = f'{package}/Entities/{owner}'
                class_owner[name] = owner
        report['bindingFiles'] = binding_files
        seen_classes = set()
        for name, ent in unit['entities'].items():
            if ent['nativeClass'] in seen_classes:
                continue
            seen_classes.add(ent['nativeClass'])
            ent_functions = {n: f for n, f in ent['functions'].items() if n not in SKIP_ROLES}
            ent_functions.update(ent.get('helpers', {}))   # entity-class members, lifted into the same file
            owners.append(('entity', class_owner[name], ent_functions, state_map(ent['fields']), quest_state))
        for kind, owner, functions, state, parent_state in owners:
            entity = kind == 'entity'
            lifter = Lifter(self.manifest, state, 'quest', entity, package, self.rdata,
                            thing_sigs=thing_signatures(self.things), parent_state=parent_state,
                            state_receiver='__native_entity_state' if entity else 'quest',
                            live_termination=True, native_gotos=True, readable_locals=True,
                            flat_control=self.flat_control)
            local_names = {f['address'].lower(): n for n, f in functions.items()
                           if re.fullmatch(r'[A-Za-z_]\w*', n) and n not in ('Main', 'Init', 'OnPersist', 'OnPredicateFail')}
            lifter.helper_names = set(local_names.values())
            lifter.binding_files = binding_files
            signatures = {}
            for address, helper in local_names.items():
                fn = self.native(address)
                if fn:
                    signatures[helper] = function_parameters(fn['decompile'], member=True)
                    lifter.helper_parameters[helper] = [p['lua'] for p in signatures[helper]['parameters']]
                    if signatures[helper]['returnKind']:
                        lifter.helper_return_kinds[helper] = signatures[helper]['returnKind']
            relative = f'FSE/{package}/Entities/{owner}.lua' if entity else f'FSE/{package}/{package}.lua'
            chunks = [f'-- Generated native draft: {owner}. Review coverage report before use.',
                      '-- Registration remains disabled until the package is verified.', '']
            if entity:
                chunks.append(ENTITY_STATE)
            for name, spec in functions.items():
                row = {'owner': owner, 'function': name, 'address': spec['address'], 'path': relative,
                       'evidence': spec.get('evidence')}
                fn = self.native(spec['address'])
                if not fn:
                    report['missing'].append(row)
                    continue
                lifter.callee_names = known_callee_aliases(fn)
                lifter.parent_helpers = {}
                if entity:
                    # Entity code calling quest helpers goes through the shared module.
                    candidates = {}
                    for call in fn.get('calls', []):
                        address = str(call.get('target', '')).lower()
                        if call.get('currentName') and address in helpers:
                            for label in (call['currentName'], call['currentName'].split('::')[-1]):
                                candidates.setdefault(label, set()).add(address)
                    for label, addresses in candidates.items():
                        if len(addresses) != 1:
                            continue
                        address = next(iter(addresses))
                        helper_fn = self.native(address)
                        if not helper_fn:
                            continue
                        sig = function_parameters(helper_fn['decompile'], member=True)
                        if sig.get('problem'):
                            continue
                        lifter.parent_helpers[label] = {'module': shared_module, 'name': helpers[address],
                                                        'arity': len(sig['parameters']), 'returnKind': sig['returnKind']}
                        shared_names.add(helpers[address])
                candidates = {}
                for call in fn.get('calls', []):
                    if call.get('currentName') and str(call.get('target', '')).lower() in local_names:
                        candidates.setdefault(call['currentName'], set()).add(local_names[call['target'].lower()])
                lifter.callee_names.update({label: next(iter(names)) for label, names in candidates.items() if len(names) == 1})
                signature = signatures.get(name) or function_parameters(fn['decompile'], member=True)
                representative = next((n for n, o in class_owner.items() if o == owner), owner) if entity else owner
                spec_l = LoweringSpec(unit, representative, entity=entity, thing_slots=self.thing_slots)
                spec_l.resolve_string = self.rdata.string_at
                spec_l.call_labels = {c['currentName']: int(c['target'], 16) for c in fn.get('calls', []) if c.get('currentName')}
                lowered, lowering_diag = lower(rename_parameters(fn['decompile'], signature), spec_l)
                source = lower_after_annotate(strip_receiver_arguments(annotate(lowered, self.slots, self.things, self.returning, entity=entity)))
                if name == 'OnPersist':
                    body, _, calls = lift_persist(source, 'quest')
                    todo, params = [], 'quest, context'
                else:
                    parameter_kinds = {p['lua']: 'number' if p['type'] in NUMBER_TYPES
                                       else 'bool' if p['type'] == 'bool' else 'unknown'
                                       for p in signature['parameters']}
                    body = lifter.lift(name, source, native_function=fn, parameters=parameter_kinds)
                    calls, todo = list(lifter.calls), list(lifter.todo)
                    params = 'quest, me' if entity else 'quest'
                    params += ''.join(', ' + p['lua'] for p in signature['parameters'])
                    if signature.get('problem'):
                        todo.append(signature['problem'])
                    todo.extend('lowering: ' + d for d in lowering_diag)
                row['nativeSignature'] = signature
                function_source = finish_lua('\n'.join([f'function {name}({params})'] + body + ['end', '']))
                if 'resources:' in function_source:
                    function_source = function_source.replace('\n', '\n    local resources = quest:RetailResources()\n', 1)
                if not entity and name in helpers.values():
                    shared_inputs[name] = (source, fn, signature, dict(lifter.callee_names))
                row.update(todo=todo, calls=calls, lines=len(body),
                           syntax=self.checker.check({relative + ':' + name: function_source}))
                if name != 'OnPersist':
                    row['readableLocalNames'] = lifter.readable_local_names
                    row['nativeJumps'] = sorted(lifter.lua_jumps)
                    row['nativeLabels'] = sorted(lifter.lua_labels)
                report['functions'].append(row)
                chunks.append(function_source)
            source = '\n'.join(chunks)
            destination = out / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_text(source + '\n', encoding='utf-8')
            all_sources[relative] = source
            report['packages'].append({'owner': owner, 'path': relative})
        # transitive helper dependencies stay inside the shared module
        changed = True
        while changed:
            changed = False
            for address, helper in helpers.items():
                if helper not in shared_names:
                    continue
                for call in self.by_address.get(address, {}).get('calls', []):
                    dependency = helpers.get(str(call.get('target', '')).lower())
                    if dependency and dependency not in shared_names:
                        shared_names.add(dependency); changed = True
        shared_path = f'FSE/{package}/native_quest_helpers.lua'
        if shared_names:
            shared_sources, shared_diagnostics = {}, {}
            for name in sorted(shared_names):
                if name not in shared_inputs:
                    continue
                source, fn, signature, aliases = shared_inputs[name]
                shared_lifter = Lifter(self.manifest, quest_state, 'quest', False, package, self.rdata,
                                       callee_names=aliases, thing_sigs=thing_signatures(self.things),
                                       live_termination=True, execution_entity=True, native_gotos=True,
                                       readable_locals=True, flat_control=self.flat_control)
                shared_lifter.helper_names = set(helpers.values())
                for address, helper in helpers.items():
                    helper_fn = self.native(address)
                    if not helper_fn:
                        continue
                    sig = function_parameters(helper_fn['decompile'], member=True)
                    shared_lifter.helper_parameters[helper] = [p['lua'] for p in sig['parameters']]
                    if sig['returnKind']:
                        shared_lifter.helper_return_kinds[helper] = sig['returnKind']
                kinds = {p['lua']: 'number' if p['type'] in NUMBER_TYPES else 'unknown' for p in signature['parameters']}
                body = shared_lifter.lift(name, source, native_function=fn, parameters=kinds)
                params = 'quest, me' + ''.join(', ' + p['lua'] for p in signature['parameters'])
                shared_sources[name] = finish_lua('\n'.join([f'function {name}({params})'] + body + ['end', '']))
                shared_diagnostics[name] = list(shared_lifter.todo)
            names = sorted(shared_sources)
            shared = '\n'.join(['-- Generated from the same native helper bodies as the quest draft.',
                                'local ' + ', '.join(names)] + [shared_sources[n] for n in names] +
                               ['return {' + ', '.join(f'{n} = {n}' for n in names) + '}\n'])
            (out / shared_path).write_text(shared, encoding='utf-8')
            report['sharedHelpers'] = {'path': shared_path, 'functions': names, 'todo': shared_diagnostics,
                                       'syntax': self.checker.check({shared_path: shared})}
            all_sources[shared_path] = shared
        report['syntax'] = self.checker.check(all_sources)
        report['summary'] = {'owners': len(owners), 'functions': len(report['functions']),
                             'missing': len(report['missing']),
                             'functionSyntaxPassed': sum(r['syntax']['passed'] for r in report['functions']),
                             'fileSyntaxPassed': report['syntax']['passed'],
                             'fileSyntaxChecked': report['syntax']['checked'],
                             'todo': sum(len(r['todo']) for r in report['functions'])}
        return report


def write_reports(reports, out, title):
    (out / 'FSE/quests.lua').write_text('-- Generated drafts are not enabled.\nQuests = {}\n', encoding='utf-8')
    total = {'owners': 0, 'functions': 0, 'missing': 0, 'functionSyntaxPassed': 0,
             'fileSyntaxPassed': 0, 'fileSyntaxChecked': 0, 'todo': 0}
    lines = [f'# {title}', '', 'Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.', '',
             '| Script | Owner | Function | Address | Compiles | TODO |', '|---|---|---|---|---|---:|']
    for report in reports:
        for k in total:
            total[k] += report['summary'][k]
        for row in report['functions']:
            lines.append(f"| {report['script']} | {row['owner']} | {row['function']} | {row['address']} | "
                         f"{row['syntax']['passed'] == 1} | {len(row['todo'])} |")
    lines += ['', 'Summary: `' + json.dumps(total) + '`', '']
    (out / 'CONVERSION_REPORT.json').write_text(json.dumps({'units': reports, 'summary': total}, indent=2) + '\n', encoding='utf-8')
    (out / 'CONVERSION_REPORT.md').write_text('\n'.join(lines), encoding='utf-8')
    return total


def main():
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--unit', default='guild_training')
    a.add_argument('--units', type=Path)
    a.add_argument('--tu', type=Path)
    a.add_argument('--out', type=Path)
    a.add_argument('--title')
    a.add_argument('--only', nargs='*', help='unit script names to convert')
    a.add_argument('--flat-control', action='store_true')
    args = a.parse_args()
    from tools.script_recovery.script_units import unit as script_unit
    spec = script_unit(args.unit)
    args.units = args.units or spec['evidence'] / 'units'
    typed = spec['evidence'] / 'translation_unit_typed.json'
    args.tu = args.tu or (typed if typed.is_file() else spec['evidence'] / 'translation_unit.json')
    args.out = args.out or ROOT / 'refs/script_recovery/lifted' / spec.get('package', args.unit.title().replace('_', '')) / 'draft'
    args.title = args.title or f'Full {args.unit.replace("_", " ")} native conversion coverage'
    converter = UnitConverter(args.tu, flat_control=args.flat_control)
    reports = []
    for path in sorted(args.units.glob('*.json')):
        unit = json.loads(path.read_text(encoding='utf-8'))
        if args.only and unit['script'] not in args.only:
            continue
        reports.append(converter.convert(unit, args.out))
    print(json.dumps(write_reports(reports, args.out, args.title), indent=2))


if __name__ == '__main__':
    main()
