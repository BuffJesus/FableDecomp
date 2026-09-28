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
import struct
import os
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.lift_native_lua import (  # noqa: E402
    Lifter, RData, annotate, known_callee_aliases, load_manifest, load_slots, load_thing_tables,
    thing_signatures, lift_persist, parse_thing_signature, persist_local_name,
)
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker  # noqa: E402
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters  # noqa: E402
from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower, finish_lua, strip_receiver_arguments, lower_after_annotate, _split_top, fold_stack_vector_builds  # noqa: E402
from tools.script_recovery.annotate_interface_slots import load_thing_slots  # noqa: E402
from tools.script_recovery.native_cleanup_regions import hoist_cleanup_regions  # noqa: E402
from tools.script_recovery.declare_free_locals import declare_free_locals  # noqa: E402
from tools.script_recovery import native_literal_string_vectors  # noqa: E402
from tools.script_recovery.native_vector_component_copies import fold_vector_component_copies, fold_split_dword_stores  # noqa: E402

ENTITY_STATE = '''local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end
'''
# bool AreAllThingsInVectorDead(const vector<CScriptThing>&) 0xCBED00 (disassembly 2026-09-20): the count of
# elements that are !IsAlive() (slot 0x12c) or IsUnconscious() (0xf4) equals the size. A file-level helper rather
# than an inline function expression: the readable-style passes cannot read the latter and ship the function raw.
ALL_DEAD_HELPER = '''local function __native_all_dead(list)
    local down = 0
    for _, thing in ipairs(list) do
        if (not thing:IsAlive()) or thing:IsUnconscious() then down = down + 1 end
    end
    return down == #list
end
'''
NUMBER_TYPES = ('long', 'int', 'uint', 'unsigned long', 'unsigned int', 'short', 'EBadDeeds')
SKIP_ROLES = {'destructor', 'GetParentScript', 'OnInterrupted', 'RegisterMain'}


def state_map(fields):
    return {offset.lower(): (name, kind) for offset, (name, kind) in fields.items()}


# CPersistContext::Transfer<T> instantiations FSE proved by address (FableAPI.cpp ASLR<tCPersistContext_Transfer_*>);
# the typing spec carries them as `CPersistContext_Transfer_<k>_API`. The bsim label on every other
# instantiation is `Transfer<signed_char>` (propagated), so the label's template argument is not evidence.
PERSIST_KIND_BY_TYPEDEF = {'bool': 'Bool', 'int': 'Int', 'uint': 'UInt', 'float': 'Float', 'string': 'String'}
PERSIST_STATE_KIND = {'Bool': 'Bool', 'Int': 'Int', 'UInt': 'Int', 'Float': 'Float', 'String': 'String'}
PERSIST_DEFAULT = {'Bool': 'false', 'Int': '0', 'UInt': '0', 'Float': '0.0', 'String': '""'}
RE_TRANSFER_CALL = re.compile(r'^[ \t]*(?:\w+ = )?(?:\([\w :<>*]+\))?\s*CPersistContext::Transfer<([^>]+)>(?:__at([0-9a-f]+))?\s*\((.*)\);[ \t]*$', re.M)


def out_thing_messages(spec_path, manifest):
    """Interface slots whose retail signature is `bool Msg(CScriptThing *out)` and whose Forge binding returns
    `sol::object` with no script operands: the binding hands back the out thing (nil when the message did not fire)."""
    names = set()
    if not Path(spec_path).is_file():
        return frozenset()
    for slot in json.loads(Path(spec_path).read_text(encoding='utf-8')).get('slots', {}).values():
        params = slot.get('params', [])
        spec = manifest.get(slot.get('name'))
        if (slot.get('ret') == 'bool' and len(params) == 1 and params[0].get('type', '').replace(' ', '') == 'CScriptThing*'
                and spec and spec.get('returnType') == 'sol::object'
                and all(p.get('type') == 'sol::this_state' for p in spec.get('parameters', []))):
            names.add(slot['name'])
    return frozenset(names)


def persist_kinds_from_spec(tu_path):
    """address -> kind for the Transfer<T> helpers the unit's typing spec (FSE typedefs) names."""
    spec = Path(tu_path).parent / 'typing_spec.json'
    kinds = {}
    if spec.is_file():
        for address, helper in json.loads(spec.read_text(encoding='utf-8-sig')).get('helpers', {}).items():
            m = re.fullmatch(r'CPersistContext_Transfer_(\w+)_API', str(helper.get('name', '')))
            if m and m.group(1) in PERSIST_KIND_BY_TYPEDEF:
                kinds[int(address, 16)] = PERSIST_KIND_BY_TYPEDEF[m.group(1)]
    return kinds


def lift_persist_evidence(source, unit, spec_l, persist_kinds):
    """OnPersist from the lowered decompile: every `CPersistContext::Transfer<T>(ctx, name, &field[, &default])`
    becomes get / PersistTransfer / set on the field the address evidence names (a quest member, a master-data
    member, or a member vector). The transfer kind comes from the callee address (FSE-proven), else from the
    template argument. Returns (lines, calls, todo)."""
    from tools.script_recovery.lift_native_lua import TYPE_MAP
    quest_fields = state_map(unit['quest'].get('fields', {}))
    master_fields = state_map(unit.get('master', {}).get('fields', {}))
    unmapped = {x['offset'].lower(): x for x in unit['quest'].get('unmappedFields', [])}
    out, calls, todo = [], [], []
    for m in RE_TRANSFER_CALL.finditer(source):
        template, at, arg_text = m.group(1), m.group(2), m.group(3)
        args = [a.strip() for a in _split_top(arg_text)]
        if len(args) < 3:
            todo.append(f'unlifted persist transfer: {m.group(0).strip()[:90]}')
            out.append(f'    -- TODO(native): {m.group(0).strip()}')
            continue
        target = int(at, 16) if at else spec_l.call_labels.get(f'CPersistContext::Transfer<{template}>')
        name_arg = args[1]
        if re.fullmatch(r'"(?:[^"\\]|\\.)*"', name_arg):
            name = name_arg[1:-1]
        else:
            literal = re.fullmatch(r'(?:\([\w *]+\))?(0x[0-9a-f]+)', name_arg)
            name = spec_l.resolve_string(int(literal.group(1), 16)) if literal and spec_l.resolve_string else None
        field = args[2]
        member = re.fullmatch(r'(?:\([\w *]+\))?\(?this \+ (0x[0-9a-f]+|\d+)\)?', field)
        master = re.fullmatch(r'(?:\([\w *]+\))?\(?\*\(int \*\)\(this \+ 0x44\) \+ (0x[0-9a-f]+|\d+)\)?', field)
        kind = persist_kinds.get(target) if target else None
        if not name:
            todo.append(f'persist transfer: unresolved name operand {name_arg} ({m.group(0).strip()[:60]})')
            out.append(f'    -- TODO(native): {m.group(0).strip()}')
            continue
        if member and unmapped.get(hex(int(member.group(1), 0))) and 'vector<' in unmapped[hex(int(member.group(1), 0))]['type']:
            row = unmapped[hex(int(member.group(1), 0))]
            if row['type'].startswith('vector<CCharString'):
                # `CPersistContext::Transfer<std::vector<CCharString>>` (0x49B8D0): the string-list transfer is
                # `quest:PersistTransferStringList(context, name, table)` returning the table (the sidecar binding
                # is a documented requirement, docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md). Nothing in the script
                # writes the member, so an empty table goes out and the loaded list comes back.
                var = persist_local_name(name)
                out.append(f'    local {var} = quest:PersistTransferStringList(context, "{name}", {{}})'
                           f'  -- {row["type"].split(",")[0]}> member `{row["name"]}` (this + {row["offset"]})')
                calls.append('PersistTransferStringList')
                continue
            todo.append(f'persist transfer: `{name}` is a `{row["type"]}` member ({row["name"]}, this + {row["offset"]}); '
                        f'no PersistTransfer list binding (callee {target:#x} is Transfer<vector>, {len(args)} operands)')
            out.append(f'    -- TODO(native): quest:PersistTransfer(context, "{name}", ...)  -- {row["type"]} member `{row["name"]}` (callee {target:#x}); binding missing')
            continue
        if kind is None:
            # not an FSE-typed instantiation: the member's PDB kind is the evidence (an enum member is a 4-byte
            # transfer; the retail body is the Transfer<int> shape), the bsim template argument the last resort
            evidence = (master_fields.get(hex(int(master.group(1), 0))) if master
                        else quest_fields.get(hex(int(member.group(1), 0))) if member else None)
            kind = evidence[1] if evidence else TYPE_MAP.get(template.replace(' ', '_'), 'Int')
            if not evidence and target:
                todo.append(f'persist transfer: kind of `{name}` from the bsim template argument `{template}` (callee {target:#x} not FSE-typed)')
        var = persist_local_name(name)
        state = PERSIST_STATE_KIND[kind]
        if master:
            f = master_fields.get(hex(int(master.group(1), 0)))
            label = f[0] if f else name
            out.append(f'    local {var} = quest:GetMasterGameState("{label}") or {PERSIST_DEFAULT[kind]}')
            out.append(f'    {var} = quest:PersistTransfer{kind}(context, "{name}", {var})')
            out.append(f'    quest:SetMasterGameState("{label}", {var})')
            calls += ['GetMasterGameState', f'PersistTransfer{kind}', 'SetMasterGameState']
            if not f:
                todo.append(f'persist transfer: master-data member at +{master.group(1)} has no PDB name; keyed by the transfer name `{name}`')
        else:
            f = quest_fields.get(hex(int(member.group(1), 0))) if member else None
            label = f[0] if f else name
            if member and not f:
                todo.append(f'persist transfer: quest member at this + {member.group(1)} has no PDB name; keyed by the transfer name `{name}`')
            elif not member:
                todo.append(f'persist transfer: field operand `{field}` not a member; keyed by the transfer name `{name}`')
            out.append(f'    local {var} = quest:GetState{state}("{label}") or {PERSIST_DEFAULT[kind]}')
            out.append(f'    {var} = quest:PersistTransfer{kind}(context, "{name}", {var})')
            out.append(f'    quest:SetState{state}("{label}", {var})')
            calls += [f'GetState{state}', f'PersistTransfer{kind}', f'SetState{state}']
    return out, calls, todo


SIDECAR_PATCHES = ROOT / 'tools' / 'script_recovery' / 'sidecar_patches'
RE_SIDECAR_BINDING = re.compile(r'^\s*(quest|thing)\["(\w+)"\]\s*=\s*\[\]\(([^)]*)\)\s*(?:->\s*([^{]+?))?\s*\{', re.M)


THING_OBJECT_NAMES = {'thing', 'value', 'result', 'other', 'me', 'pMe', 'target'}


def sidecar_bindings():
    """Manifest entries for the bindings the NoviCompatibility sidecar adds on top of stock ForgeFSE
    (evidence: the added `quest["X"]` / `thing["X"]` lines of tools/script_recovery/sidecar_patches/*.patch)."""
    entries = {}
    for patch in sorted(SIDECAR_PATCHES.glob('*.patch')) if SIDECAR_PATCHES.is_dir() else []:
        # only the added lines, with their `+` stripped: a lambda head wrapped over several lines (CreateEffect's
        # seven parameters, `-> std::shared_ptr<CScriptThing> {` on its own line) is one statement again
        added = '\n'.join(line[1:] for line in patch.read_text(encoding='utf-8', errors='replace').splitlines()
                          if line.startswith('+') and not line.startswith('+++'))
        for scope, name, params, ret in RE_SIDECAR_BINDING.findall(added):
            parameters = []
            for param in [x.strip() for x in params.split(',') if x.strip()]:
                if 'LuaQuestState' in param or 'sol::this_state' in param:
                    continue
                kind, _, pname = param.rpartition(' ')
                kind = kind.replace('const ', '').replace('&', '').strip()
                if kind == 'CScriptThing*' and pname == 'me':
                    parameters.append({'name': 'pMe', 'type': 'CScriptThing*', 'optional': False})
                else:
                    # a `sol::object` is a thing handle when the binding names it so; an overload-dispatching operand
                    # (CreateEffect's `where` = position table or thing, `arg4`/`arg5`) stays untyped and positional
                    parameters.append({'name': pname or f'arg{len(parameters)}',
                                       'type': 'CScriptThing*' if kind == 'sol::object' and pname in THING_OBJECT_NAMES else kind, 'optional': False})
            # lambdas without a trailing return type: reviewed against the patch bodies
            ret = (ret or {'GetStateThing': 'CScriptThing*', 'GetStateListCount': 'int'}.get(name, 'void')).strip()
            ret = {'std::string': 'const std::string&', 'std::shared_ptr<CScriptThing>': 'CScriptThing*'}.get(ret, ret)
            if parameters and parameters[0]['name'] == 'result':
                # a binding that takes the native hidden-result slot as its first Lua operand (CreateEffect): the
                # lifter's manifest placement models a hidden result as the return value and would drop or shuffle
                # the operands; left out, the call is emitted positionally from the typed export's operand list
                continue
            entries[name] = {'name': name, 'scope': 'Entity' if scope == 'thing' else 'Quest', 'returnType': ret,
                             'parameters': parameters, 'blocking': False, 'category': 'NoviCompatibility sidecar'}
    return entries


# The host DLL sources (canonical fork first, then the local sidecar build tree): the older SDK manifest
# lacks bindings ForgeFSE registers today (CloseDoor, GetStateFloat, IsActiveThreadTerminating, ...).
FORGEFSE_SOURCES = [Path(r'D:/Code/ForgeFSE-retail-shadow/FableScriptExtender'),
                    ROOT / 'work/new-oakvale-original-fse-20260912/sidecar-abi-v2/FableScriptExtender']
RE_HOST_BINDING = re.compile(r'^\s*(questState_type|cscriptThing_type)\["(\w+)"\]\s*=\s*&(?:LuaQuestState|CScriptThing)::(\w+)\s*;', re.M)
RE_HOST_LAMBDA = re.compile(r'^\s*(questState_type|cscriptThing_type)\["(\w+)"\]\s*=\s*\[[^\]]*\]\(([^)]*)\)\s*(?:->\s*([^{]+?))?\s*\{', re.M)


def host_bindings():
    """Manifest entries for `questState_type["X"] = &LuaQuestState::Y;` registrations whose prototype is in
    LuaQuestState.h, plus inline lambdas with an explicit parameter list. Missing sources -> nothing added."""
    sources = [d for d in FORGEFSE_SOURCES if (d / 'LuaManager.cpp').is_file()]
    if not sources:
        return {}
    # the sidecar build tree may carry bindings the canonical fork does not yet (GetStateFloat): union them
    manager = '\n'.join((d / 'LuaManager.cpp').read_text(encoding='utf-8', errors='replace') for d in sources)
    header = '\n'.join((d / 'LuaQuestState.h').read_text(encoding='utf-8', errors='replace') for d in sources if (d / 'LuaQuestState.h').is_file())
    protos = {m.group(2): (m.group(1).strip(), m.group(3))
              for m in re.finditer(r'^\s*((?:const\s+)?[\w:<>*&]+(?:\s*\*)?)\s+(\w+)\s*\(([^)]*)\)\s*(?:const)?\s*;', header, re.M)}

    def params(text, scope):
        out = []
        for param in [x.strip() for x in text.split(',') if x.strip()]:
            if 'LuaQuestState' in param or 'sol::this_state' in param or 'sol::variadic_args' in param:
                continue
            kind, _, pname = param.partition('=')[0].strip().rpartition(' ')
            kind = kind.replace('const ', '').replace('&', '').replace(' *', '*').strip()
            if scope == 'cscriptThing_type' and not out and kind == 'CScriptThing*':
                pname = 'pMe'
            out.append({'name': pname or f'arg{len(out)}', 'type': 'CScriptThing*' if kind == 'sol::object' else kind,
                        'optional': '=' in param})
        return out

    entries = {}
    for scope, name, member in RE_HOST_BINDING.findall(manager):
        if member not in protos:
            continue
        ret, plist = protos[member]
        parameters = params(plist, scope)
        if scope == 'cscriptThing_type':
            parameters.insert(0, {'name': 'pMe', 'type': 'CScriptThing*', 'optional': False})
        entries[name] = {'name': name, 'scope': 'Entity' if scope == 'cscriptThing_type' else 'Quest',
                         'returnType': {'std::string': 'const std::string&', 'std::shared_ptr<CScriptThing>': 'CScriptThing*'}.get(ret, ret),
                         'parameters': parameters, 'blocking': False, 'category': 'ForgeFSE host binding'}
    for scope, name, plist, ret in RE_HOST_LAMBDA.findall(manager):
        # a lambda without a trailing return type usually forwards a same-named member: take its prototype
        ret = (ret or (protos[name][0] if name in protos else 'void')).strip()
        entries.setdefault(name, {'name': name, 'scope': 'Entity' if scope == 'cscriptThing_type' else 'Quest',
                                  'returnType': {'std::string': 'const std::string&'}.get(ret, ret),
                                  'parameters': params(plist, scope), 'blocking': False, 'category': 'ForgeFSE host binding'})
    return entries


# labels whose lowering rules key on the printed text rather than the target address
LABEL_TEXT_RULES = {'CCharString::CCharString', 'CCharString::operator='}


def disambiguate_call_labels(decompile, calls, fn=None):
    """bsim propagates one label over byte-similar bodies, so a function may call e.g. the scripted-thing
    resource dtor (0x7E74D0) and the movie dtor (0x6E7B80) under the same printed name. The lowering keys
    on {label: target}, so the last site would win for all. Rename the k-th printed occurrence to
    `<label>__at<target>`: through the decompiler's token order (`callOrder`, exact even in restructured
    code) when the function export carries it, else following the site (address) order; when the printed
    count differs from the site count the text is left alone."""
    by_label = {}
    for c in calls:
        if c.get('currentName'):
            by_label.setdefault(c['currentName'], []).append(int(c['target'], 16))
    renamed = {}
    ordered = _text_order_sites(decompile, fn) if fn is not None and any(len(set(v)) > 1 for v in by_label.values()) else None
    for label, targets in by_label.items():
        if len(set(targets)) < 2 or label in LABEL_TEXT_RULES:
            continue
        pattern = re.compile(r'(?<![\w:])' + re.escape(label) + r'(?=\s*\()')
        if len(pattern.findall(decompile)) != len(targets):
            continue
        if ordered is not None:
            in_text = [int(site['target'], 16) for a, e, args, site, key, vtable in ordered if not vtable and key == label]
            if len(in_text) == len(targets):
                targets = in_text
        it = iter(targets)

        def repl(m, it=it, label=label):
            target = next(it)
            new = f'{label}__at{target:x}'
            renamed[new] = target
            return new
        decompile = pattern.sub(repl, decompile)
    return decompile, renamed


RE_STACK_OPERAND = re.compile(r'^(?P<cast>\((?:[\w :]+\*+|int|uint|undefined4)\))?(?P<amp>&?)\(?(?P<name>[A-Za-z]+Stack_[0-9a-f]+|local_[0-9a-f]+|stack0x[0-9a-f]+)(?: \+ (?P<plus>4|8|0xc|12))?\)?$')


def _call_spans(text, label):
    """(start, args_start, end) of every `label(<balanced>)` occurrence, in text order. Ghidra wraps long
    labels (`std::` newline `_Cons_val<...>` newline `(`), so whitespace is allowed at `::` and before `(`."""
    head = re.compile(r'(?<![\w:])' + re.escape(label).replace('::', r'\s*::\s*') + r'\s*\(')
    spans = []
    for m in head.finditer(text):
        j, depth = m.end(), 1
        while j < len(text) and depth:
            depth += {'(': 1, ')': -1}.get(text[j], 0)
            j += 1
        spans.append((m.start(), m.end(), j))
    return spans



def _balanced(text):
    depth = 0
    for ch in text:
        depth += {'(': 1, ')': -1}.get(ch, 0)
        if depth < 0:
            return False
    return depth == 0

_CALLEE_WORDS = {}


def callee_stack_words(target):
    """Stack words a direct callee purges, read from its RETs in the retail bytes (read-only): every `ret N`
    reached by a linear sweep up to the first int3 padding must agree. None when the bytes are unavailable,
    the sweep finds no RET (a jump thunk) or the RETs disagree. A `ret` (N = 0) is 0: a caller-cleaned
    callee's `add esp` is the caller's business (see `caller_cleanup_words`).
    (GetDataString 0x004AA900: `ret 4` on both paths = its hidden CCharString result; AppendData 0x0099F570,
    AppendCString 0x0099F600, operator+ 0x0099F690: `ret 4`.)"""
    if target is None:
        return None
    if target in _CALLEE_WORDS:
        return _CALLEE_WORDS[target]
    words = None
    try:
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        from tools.script_recovery.lift_native_lua import RData
        if not hasattr(callee_stack_words, 'rdata'):
            callee_stack_words.rdata = RData()
        raw = callee_stack_words.rdata.bytes_at(target, 0x800)
        if raw:
            rets = set()
            for ins in Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, target):
                if ins.mnemonic == 'ret':
                    rets.add(int(ins.op_str, 0) if ins.op_str else 0)
                elif ins.mnemonic == 'int3':
                    break
            if len(rets) == 1:
                n = rets.pop()
                words = n // 4 if n % 4 == 0 else None
    except ImportError:
        words = None
    _CALLEE_WORDS[target] = words
    return words


def caller_cleanup_words(insns, i):
    """Stack words the caller pops right after the call at insns[i] (`add esp, N`, a __cdecl callee)."""
    nxt = insns[i + 1] if i + 1 < len(insns) else None
    if nxt is not None and nxt.mnemonic == 'add' and nxt.op_str.startswith('esp, '):
        try:
            n = int(nxt.op_str.split(', ', 1)[1], 0)
        except ValueError:
            return None
        return n // 4 if n % 4 == 0 else None
    return 0


def _site_target(site):
    try:
        return int(site.get('target', ''), 16)
    except (TypeError, ValueError):
        return None


def _align(site, args, vtable=False, receiver_printed=False):
    '''Entry-relative slots aligned with the printed arguments of one call, or None when the printed count
    cannot be reconciled with the recorded register/push operands (a by-value slot, a hidden return pointer
    push the decompiler folded away, ...). Printed order is (ecx, edx, pushes right-to-left). A vtable call
    is `__thiscall`: exactly one register operand, so a shorter push record (the export's backward scan
    stopped at a call between the pushes -- `CreateObject(&r, &name, GetPos(elem), &script)` in
    RunTutorials 0x00D45DD0) cannot be aligned at all, rather than shifting every operand one to the left.'''
    pushed = list(reversed(site.get('pushedStack', [])))
    values = list(reversed(site.get('pushedValue') or [None] * len(pushed)))
    ecx, edx = site.get('ecxStack'), site.get('edxStack')
    if vtable and receiver_printed and len(pushed) == len(args) and len(pushed) > 0:
        # the record is one push too long and the receiver is printed: the EARLIEST push (the head of the
        # export's push-order list, the tail after the reversal) belongs to a later call -- VC7.1 pushes a
        # literal operand of the NEXT call before making the calls whose results it also pushes
        # (`push ebx` = EntityTeleportToThing's bool before GetThingWithScriptName / GetHero, Departure Init
        # 0x00D506B0: record [0, -16, -12] for two real pushes). ~190 GSI sites across the four units.
        pushed, values = pushed[:-1], values[:-1]
    if not vtable and ecx is not None and len(args) == 1 + (1 if edx is not None else 0) and pushed and str(site.get('currentName', '')).startswith('CCharString::'):
        # a direct __thiscall / __fastcall helper whose record is LONGER than its printed stack operands: the
        # export's backward scan swallowed the pushes VC7.1 made early for the NEXT call (TraderToRescue
        # 0x00DFF1DB: `CCharString::operator const char*(&key)` printed with its receiver only, record = Speak's
        # four pushes; unaligned, the receiver stayed `&stack0xfffffe84` and the trader spoke the wrong line,
        # third audit 2026-09-20). The nearest pushes (the head after the reversal) are this call's.
        # (only the receiver-only shape of a CCharString method: with real stack operands the push order is not
        # reliable enough to say which recorded pushes are this call's, and a resource / movie DESTRUCTOR's ecx is
        # the derived object one word below the base the acquire calls name -- aligning those split the resource
        # identity and broke every ACTORMAP_Set fold in CombatApprentice)
        pushed, values = [], []
    words = callee_stack_words(_site_target(site)) if not vtable and ecx is not None and len(pushed) > len(args) - 1 else None
    if words is not None and len(args) == 1 + words:
        # a direct `__thiscall` whose stack purge is proven from its RET: only its nearest `words` pushes are
        # its own, the earlier ones are the NEXT call's operands VC7.1 pushed first (MakeTraderComment
        # 0x00E022DF: GetDataString's record holds AddLineToConversation's hero / thing / flag and the concat
        # literals before its hidden result; unaligned, the receiver stayed Ghidra's drifted `auStack_24 + 4`
        # and lowered as the EH state constant 31). A __thiscall takes no EDX operand: a recorded edx is the
        # `lea edx` that fed one of the pushes.
        pushed, values, edx = pushed[:words], values[:words], None
    lead = len(args) - len(pushed)          # printed register arguments (this / __fastcall ecx, edx)
    # (a truncated record on a `__thiscall` site -- the export's backward scan stopped at a call between the
    # pushes, `CreateObject(&r, &def, GetPos(marker), &script)` -- is NOT padded into an alignment: when the
    # call between the pushes is a vcall the export may also have charged it a stack purge it does not make
    # (RunTutorials 0x00D46A73: depth 176 for a real 180, every recorded slot 4 too high; 0x00D464AE in the same
    # function is exact). Those sites stay unaligned and their literals come from the lifter's pool, 2026-09-20.)
    if lead < 0 or lead > 2 or (ecx is not None and lead < 1) or (edx is not None and lead < 2):
        return None                         # a stack-loaded register that is not printed: an unprinted push is hiding
    if vtable and lead > 1:       # (lead 0: Ghidra printed the call without its receiver -- SetTimer(iStack_258, iVar7))
        return None
    # (address slot, value slot) per printed argument: `&X` / pointer casts name the object at the address,
    # a bare `X` names the slot whose value was loaded
    return list(zip([ecx, edx][:lead] + pushed, [site.get('ecxValue'), site.get('edxValue')][:lead] + values))


def _receiver_printed(text, args_start, args):
    '''True when the first printed argument of the vtable call whose argument list starts at `args_start` is
    its receiver: the head's base expression (`(**(code **)(*X + SLOT))(X, ..)`, `(**(code **)(**(int **)(this
    + 4) + SLOT))(*(int **)(this + 4), ..)`) or a loaded-vtable register alias of it (`iVar1 = *this_00;` then
    `(**(code **)(iVar1 + SLOT))(this_00, ..)`).'''
    if not args:
        return False
    head = re.search(r'\(\*\*\(code \*\*\)\((.*?) \+ (?:0x[0-9a-f]+|\d+)\)\)\s*\($', text[max(0, args_start - 200):args_start])
    if head is None:
        return False
    base, first = head.group(1).strip(), args[0].strip()
    if base in (first, '*' + first) or (first.startswith('*') and base == '*' + first):
        return True
    if re.fullmatch(r'iVar\d+', base):
        # (the alias line and the printed receiver may differ only in the pointer cast Ghidra chose:
        # `iVar9 = **(int **)((int)this + 0x40);` for the receiver `*(void **)((int)this + 0x40)`,
        # RunTutorials 0x00D45DD0's CreateObject / CreateCreature sites -- unpaired, their string
        # literals came out rotated)
        cast = re.compile(r'\((?:void|int|undefined4) \*\*\)')
        for m in re.finditer(r'^[ \t]*' + re.escape(base) + r' = \*([^\r\n]+);[ \t]*\r?$', text, re.M):
            if cast.sub('', m.group(1).strip()) == cast.sub('', first):
                return True
        return False
    return False

CTOR_LABELS = {'StdMap_Construct_API'}
RE_THING_SLOT_CAST = re.compile(r'^\((?:CScriptThing(?:_bv)?|C3DVector(?:_bv)?|CCharString(?:_bv)?) \*\)')


def _is_ctor_label(label):
    if label in CTOR_LABELS:
        return True
    parts = label.split('::')
    return len(parts) >= 2 and parts[-1].split('<')[0] == parts[-2].split('<')[0]   # X::X


RE_VCALL_HEAD = re.compile(r'\(\*\*\(code \*\*\)\([^;\n]*?\+ (0x[0-9a-f]+|\d+)\)\)\s*\(')
# every spelling of a vtable call head with its slot: `(**(code **)(X + SLOT))(`, `(*(code *)X[N])(`,
# `(*(code *)X[N][M])(`, `(**(code **)*X)(` (slot 0)
RE_VCALL_HEAD_ANY = re.compile(
    r'\(\*\*\(code \*\*\)\([^;\n]*?\+ (?P<slot>0x[0-9a-f]+|\d+)\)\)\s*\('
    r'|\(\*\(code \*\)(?:\w+|\(\*\(int \*\*\)\([^()]*\)\))(?:\[\d+\])?\[(?P<index>\d+)\]\)\s*\('
    r'|\(\*\*\(code \*\*\)\*\w+\)\s*\(')


RE_PTR_CALL_HEAD = re.compile(r'\(\*\(code \*\)PTR_\w*?_(?P<addr>[0-9a-f]{8})\)\s*\(')
RE_PTR_INDEX_CALL_HEAD = re.compile(r'\(\*\(code \*\)(?P<base>[A-Za-z_]\w*)\[(?P<index>0x[0-9a-f]+)\]\)\s*\(')


def _code_pointer(va):
    """The dword stored at `va` in the retail image (a code pointer in .rdata), or None."""
    try:
        from tools.script_recovery.lift_native_lua import RData
    except ImportError:
        return None
    if not hasattr(callee_stack_words, 'rdata'):
        callee_stack_words.rdata = RData()
    raw = callee_stack_words.rdata.bytes_at(va, 4)
    return struct.unpack('<I', raw)[0] if raw else None


def respell_code_pointer_calls(text, fn):
    """A direct call made through a .rdata code pointer, printed without its receiver
    (`(*(code *)PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8)()` / `(*(code *)local_30[0x4b])()`), is respelled
    as the labelled call the export records at that site, `CScriptThing::_IsAlive_CScriptThing__UBE_NXZ(
    (CScriptThing *)&xStack_30)`, with the receiver slot the site's `lea ecx` names. Only receiver-only
    calls (no printed operands, a callee whose RET purges no stack operand -- the export's push record may hold the
    NEXT call's early pushes, 0x00E01938) whose pointer holds the site's target are respelled."""
    ordered = _text_order_sites(text, fn)
    if not ordered:
        return text
    edits = []
    for a, e, args, site, key, vtable in ordered:
        ecx = site.get('ecxStack')
        if vtable or any(x.strip() for x in args) or ecx is None or ecx >= 0 or callee_stack_words(_site_target(site)) != 0:
            continue
        head = None
        for rx in (RE_PTR_CALL_HEAD, RE_PTR_INDEX_CALL_HEAD, RE_VCALL_HEAD):     # (a vtable head paired to a direct site: its pointer was checked)
            for m in rx.finditer(text, max(0, a - 120), a):
                if m.end() == a:
                    head = m
        if head is None:
            continue
        label = re.sub(r'[^\w:]', '_', site.get('currentName') or '')
        if not label.startswith('CScriptThing::'):
            continue
        edits.append((head.start(), e, f'{label}((CScriptThing *)&xStack_{-ecx:x}'))
    for start, end, repl in sorted(edits, reverse=True):
        text = text[:start] + repl + text[end:]
    return text


def _text_order_sites(text, fn):
    '''Printed calls paired with export sites through `callOrder` (the call ops in the order the decompiler's
    token stream prints them). Every printed call head (vtable head or labelled direct call) is located in
    the text; when their number equals the exported order and every pair agrees on slot / label, the k-th
    printed call is the k-th site. None when the text cannot be reconciled (the caller falls back to the
    per-label address-order pairing).
    Returns [(args_start, args_end, args, site, slot or label, vtable)] for sites with operand slots.'''
    order = fn.get('callOrder')
    if not order:
        return None
    by_site = {}
    for c in fn.get('calls', []):
        by_site[c['site'].lower()] = (c, False)
    for c in fn.get('indirectCalls', []):
        by_site[c['site'].lower()] = (c, True)
    heads = []      # (start, args_start, args_end, key, vtable)
    for m in RE_VCALL_HEAD_ANY.finditer(text):
        j, depth = m.end(), 1
        while j < len(text) and depth:
            depth += {'(': 1, ')': -1}.get(text[j], 0)
            j += 1
        slot = int(m.group('slot'), 0) if m.group('slot') else 4 * int(m.group('index')) if m.group('index') else 0
        heads.append((m.start(), m.end(), j - 1, hex(slot), True))
    # a direct call made through a code pointer in .rdata (`call [0x01238DB8]`, a CScriptThing vtable entry VC7.1
    # called non-virtually: MakeTraderComment 0x00E01938 IsAlive) prints as `(*(code *)PTR_<name>_<addr>)(`; the
    # export records it as a direct call to the pointer's target. Keyed by that target, checked in the pairing.
    for m in RE_PTR_CALL_HEAD.finditer(text):
        j, depth = m.end(), 1
        while j < len(text) and depth:
            depth += {'(': 1, ')': -1}.get(text[j], 0)
            j += 1
        heads.append((m.start(), m.end(), j - 1, ('ptr', int(m.group('addr'), 16)), False))
    # ... or through a local Ghidra saw loaded with that vtable's address (`local_30 = &PTR_..._01238c8c;` then
    # `(*(code *)local_30[0x4b])()`, MakeTraderComment 0x00E019F2): the entry is base + 4 * index
    for m in RE_PTR_INDEX_CALL_HEAD.finditer(text):
        bases = set(re.findall(r'(?<![\w.>])' + re.escape(m.group('base')) + r' = &PTR_\w*?_([0-9a-f]{8});', text))
        if len(bases) != 1:
            continue
        j, depth = m.end(), 1
        while j < len(text) and depth:
            depth += {'(': 1, ')': -1}.get(text[j], 0)
            j += 1
        heads.append((m.start(), m.end(), j - 1, ('ptr', int(bases.pop(), 16) + 4 * int(m.group('index'), 16), 4 * int(m.group('index'), 16)), False))
    labels = {c['currentName'] for c in fn.get('calls', []) if c.get('currentName')}
    for label in labels:
        spans = _call_spans(text, label)
        if not spans and re.search(r'[^\w:]', label):
            spans = _call_spans(text, re.sub(r'[^\w:]', '_', label))     # `operator_char_const*` prints as `operator_char_const_`
        if not spans and '::' in label:
            spans = _call_spans(text, label.replace('::', '__'))           # `CCharString__AppendCString`
        if not spans and re.match(r'\w+\.DLL::', label):
            spans = _call_spans(text, label.split('::', 1)[1]) + _call_spans(text, '::' + label.split('::', 1)[1])   # imports print bare (`operator_new(` / `::operator_new(`)
        parts = label.split('::')
        for k in range(1, len(parts) - 1):
            if spans:
                break
            spans = _call_spans(text, '::'.join(parts[k:]))      # Ghidra drops the enclosing namespace(s) (`NScript::CQ_X::Fn` prints `CQ_X::Fn(`)
        if not spans and '::' in label:
            spans = _call_spans(text, label.rsplit('::', 1)[1])    # a retyped member prints without its class (`MakeTeamMemberComment(`)
        for i, a, e in spans:
            heads.append((i, a, e - 1, label, False))
    heads.sort()
    # a label that is a prefix of a longer label (`Foo::Bar` inside `Foo::Bar2`) is excluded by the head
    # regex' lookbehind; overlapping spans (the same call found under two labels) collapse to one
    dedup = []
    for h in heads:
        if dedup and h[1] == dedup[-1][1]:
            continue
        dedup.append(h)
    heads = dedup
    if len(heads) != len(order):
        return None
    out = []
    for (start, a, e, key, vtable), site_addr in zip(heads, order):
        entry = by_site.get(site_addr.lower())
        if entry is None:
            return None
        site, is_vtable = entry
        if is_vtable and not vtable and isinstance(key, tuple) and len(key) == 3 and site.get('slot') is not None                 and int(site['slot'], 16) == key[2]:
            # the local Ghidra saw loaded with a vtable's address is really the receiver's vtable (`call [esi+0x118]`
            # printed `(*(code *)ppuStack_308[0x46])()`, V_Bordello Magicman Main 0x00E414A0): the export's vtable
            # site when the index is its slot -- the whole function's pairing failed on this one head
            vtable, key = True, hex(key[2])
        if vtable and not is_vtable:
            # a direct call through a local Ghidra saw loaded with a vtable's address, printed as a vtable head
            # (`local_30._0_4_ = &PTR_..._01238c8c;` then `(**(code **)(local_30._0_4_ + 300))()`, MakeTraderComment
            # 0x00E019F2 in the EBP-fixed export): the site when base + slot holds its target
            head = re.match(r'\(\*\*\(code \*\*\)\((?P<base>[A-Za-z_][\w.]*) \+ (?:0x[0-9a-f]+|\d+)\)\)', text[start:a])
            ptr = None
            if head:
                bases = set(re.findall(r'(?<![\w.>])' + re.escape(head.group('base')) + r' = &PTR_\w*?_([0-9a-f]{8});', text))
                if len(bases) == 1:
                    ptr = _code_pointer(int(bases.pop(), 16) + int(key, 16))
            if ptr is None or ptr != _site_target(site):
                return None
            vtable, key = False, site.get('currentName')
        elif is_vtable != vtable:
            return None
        if vtable:
            if site.get('slot') is not None and hex(int(site['slot'], 16)) != key:
                return None
        elif isinstance(key, tuple):
            if _code_pointer(key[1]) != _site_target(site):
                return None
            key = site.get('currentName')
        elif site.get('currentName') != key:
            return None
        if vtable or 'pushedStack' in site or 'ecxStack' in site or 'edxStack' in site:
            out.append((a, e, _split_top(text[a:e]), site, key, vtable))
    return out


RE_BYTE_DECL = re.compile(r'^[ \t]+(?:char|bool|byte|undefined1)[ \t]+(?P<name>[A-Za-z]+Stack_(?P<slot>[0-9a-f]+));[ \t]*\r?$', re.M)
_SN = r'[A-Za-z]+Stack_[0-9a-f]+'
RE_BYTE_SLICE = re.compile(
    r'^(?P<ind>[ \t]*)(?:'
    r'(?P<clr>' + _SN + r') = (?P=clr) & 0xffffff;'                                                        # top byte cleared
    r'|(?P<cat>' + _SN + r') = CONCAT13\((?P<catv>0x[0-9a-f]+|\d+),\s*(?:\(undefined3\))?(?P=cat)\);'      # top byte set
    r'|(?P<sl>' + _SN + r')\._(?P<n>\d)_1_ = (?P<slv>0x[0-9a-f]+|\d+);'                                  # byte N stored
    r'|(?P<cpy>' + _SN + r') = (?:\([\w :]+\))?\(\(uint\)(?P<via>\w+) & 0xffffff\);'                       # cleared through a copy (`via = X;` above)
    r')[ \t]*\r?$'
    r'|(?P<rd>' + _SN + r')\._(?P<rn>\d)_1_\b(?! = (?:0x[0-9a-f]+|\d+);)'                                  # byte N read
    r'|\(char\)\(\(?(?:uint\))?(?P<top>' + _SN + r') >> 0x18\)',                                            # top byte read
    re.M)


def _drifted_byte_slices(text, uses):
    """(start, end, replacement) for the byte slices Ghidra mis-slotted after its stack model drifted.

    A `mov byte ptr [esp+N], imm` made where the decompiler's ESP model is off (it lost the purge of a vtable
    call it could not type: `(**(code **)(*piVar1 + 0x5ec))()` printed without its `push 1`) is named by the
    wrong slot: retail 0x00D52E90 clears its woods-loop flag `cStack_169` on the YES answer, printed as
    `uStack_170 = uStack_170 & 0xffffff;` (-0x16d), and the walk-back flag `cStack_161` as
    `CStack_168._3_1_ = 0;` (-0x165) -- both 4 bytes below the declared char, neither ever read, so the
    lifter dropped the writes and the loop became `until false` (the 2026-09-19 evening playtest). The
    temporaries constructed beside such a slice carry the same drift (their true slot is known from the
    export: `uses`), so the slice's true byte is `named + N + drift`; when a declared one-byte local sits
    exactly there (`mov byte ptr [esp+0x27], 1` at 0x00D5FD05 = -596 + 0x27 = `cStack_22d`, printed
    `CStack_234._3_1_ = 1`), every spelling of the slice -- store, read, clear through a register copy -- is
    that local's."""
    decls = {-int(m.group('slot'), 16): m.group('name') for m in RE_BYTE_DECL.finditer(text)}
    if not decls:
        return
    ctor_drift = sorted((pos, off + int(name.rsplit('_', 1)[1], 16))
                        for pos, k, name, off, plus, ctor in uses
                        if ctor and name and re.fullmatch(_SN, name))
    if not ctor_drift:
        return
    for m in RE_BYTE_SLICE.finditer(text):
        name = m.group('clr') or m.group('cat') or m.group('sl') or m.group('cpy') or m.group('rd') or m.group('top')
        byte = int(m.group('n') or m.group('rn') or 3)
        if name in decls.values():
            continue                      # already a byte local: not a merged slice
        before = [d for p, d in ctor_drift if p <= m.start()]
        after = [d for p, d in ctor_drift if p > m.start()]
        drift = before[-1] if before else after[0]
        target = -int(name.rsplit('_', 1)[1], 16) + byte + drift
        if target not in decls:
            # one push outstanding at the access (`mov al, [esp+0x1b]` inside an argument sequence, the flag at
            # [esp+0x1f]): the byte 4 above -- the Skill Guildmaster 0x00D5AE70's disqualified flag `cStack_215`,
            # printed `uStack_21c._3_1_` (a TIMER id's top byte) and lifted as an unassigned local, so `nil == 0`
            # sent every moving round to DISQUALIFIED (run 4, 2026-09-21)
            # (only when the sliced slot is a whole object whose bytes cannot be a flag: a constructor took its address)
            if target + 4 in decls and re.search(r'\w+::\w+\(\([\w ]+\*\)&' + re.escape(name) + r'\)', text):
                target += 4
            else:
                continue
        local = decls[target]
        if m.group('rd') or m.group('top'):
            yield m.start(), m.end(), local
            continue
        if m.group('cpy'):
            # `via = X;` a few statements up feeds the masked store: the copy is the slice's old value
            head = text[max(0, m.start() - 600):m.start()]
            copy = re.search(r'^([ \t]*)' + re.escape(m.group('via')) + r' = ' + re.escape(name) + r';[ \t]*\r?\n(?![\s\S]*^[ \t]*' + re.escape(m.group('via')) + r' = )', head, re.M)
            if not copy:
                continue
            if len(re.findall(r'\b' + re.escape(m.group('via')) + r'\b', text)) == 2:
                yield m.start() - len(head) + copy.start(), m.start() - len(head) + copy.end(), ''
        value = '0' if (m.group('clr') or m.group('cpy')) else (m.group('catv') or m.group('slv'))
        yield m.start(), m.end(), f"{m.group('ind')}{local} = {value};"


def _register_operand_slot(fn, site, reg, arg_index=None, receiver_printed=True):
    """The Ghidra stack slot (negative, entry-ESP relative) a register pushed at `site` was loaded from, read from the
    bytes: the prologue's `sub esp, N` plus its register pushes give the frame base; walking back from the call, the
    register's last definition `mov REG, [esp+X]` with m pushes already outstanding sits at
    `-(N + 4*pushed_regs + 4*m - X)`. None when the definition is not that shape."""
    if site is None:
        return None
    try:
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        from capstone.x86 import X86_OP_MEM, X86_OP_REG, X86_OP_IMM
    except ImportError:
        return None
    try:
        start, size = int(fn['address'], 16), int(fn.get('size') or 0)
    except (TypeError, ValueError):
        return None
    if not size or not (start <= site < start + size):
        return None
    from tools.script_recovery.lift_native_lua import RData
    raw = _register_operand_slot.rdata.bytes_at(start, size) if hasattr(_register_operand_slot, 'rdata') else None
    if raw is None:
        _register_operand_slot.rdata = RData()
        raw = _register_operand_slot.rdata.bytes_at(start, size)
    if not raw:
        return None
    cs = Cs(CS_ARCH_X86, CS_MODE_32)
    cs.detail = True
    insns = list(cs.disasm(raw, start))
    if not insns or insns[0].mnemonic != 'sub' or insns[0].operands[0].type != X86_OP_REG or insns[0].reg_name(insns[0].operands[0].reg) != 'esp':
        return None
    frame = insns[0].operands[1].imm
    for ins in insns[1:8]:
        # the callee-saved pushes may be interleaved with register moves (`push esi | mov ebp, ecx | push edi`)
        if ins.mnemonic == 'push' and ins.operands[0].type == X86_OP_REG:
            frame += 4
        elif ins.mnemonic == 'mov' and all(o.type == X86_OP_REG for o in ins.operands):
            continue
        else:
            break
    idx = next((i for i, ins in enumerate(insns) if ins.address == site), None)
    if idx is None:
        return None
    # the printed register name is Ghidra's guess (`unaff_EBX` for a `push esi`): when the operand's position is
    # known, the register is whatever the matching push (arguments are pushed right to left: the first pushed
    # argument is the one nearest the call) actually pushes
    if arg_index is not None:
        want = arg_index - (1 if receiver_printed else 0) + 1     # 1 = the push nearest the call
        seen = 0
        for ins in reversed(insns[:idx]):
            if ins.mnemonic == 'push':
                seen += 1
                if seen == want:
                    if ins.operands[0].type != X86_OP_REG:
                        return None
                    reg = ins.reg_name(ins.operands[0].reg)
                    break
            elif ins.mnemonic in ('call', 'ret') or ins.mnemonic.startswith('j'):
                return None
        else:
            return None
    # walk back: count the pushes between the register's definition and the call
    pushes_after = 0
    for ins in reversed(insns[:idx]):
        if ins.mnemonic == 'push':
            pushes_after += 1
            continue
        if ins.mnemonic == 'mov' and ins.operands[0].type == X86_OP_REG and ins.reg_name(ins.operands[0].reg) == reg:
            op = ins.operands[1]
            if op.type == X86_OP_MEM and ins.reg_name(op.mem.base) == 'esp' and op.mem.index == 0:
                # the pushes outstanding AT the mov = those made before it in this argument sequence: none of the
                # ones counted after it; the sequence starts after the previous call/cleanup, so m = 0 here
                return -(frame - op.mem.disp)
            return None
        if ins.mnemonic == 'call':
            if reg in ('ebx', 'esi', 'edi', 'ebp'):
                continue            # callee-saved: the value survives the call (its arguments were consumed)
            return None
        if ins.mnemonic == 'ret' or ins.mnemonic.startswith('j'):
            return None
        if any(o.type == X86_OP_REG and ins.reg_name(o.reg) == reg for o in ins.operands[:1]) and ins.mnemonic not in ('cmp', 'test', 'push'):
            return None     # redefined by something else
    return None


RE_BARE_VTABLE_HEAD = re.compile(r'\(\*\*\(code \*\*\)\((?P<base>[A-Za-z_]\w*) \+ (?:0x[0-9a-f]+|\d+)\)\)\s*\($')
RE_STACK_NAME_ONLY = re.compile(r'(?:[A-Za-z]+Stack_|local_)[0-9a-f]+')


def restore_stack_operands(decompile, fn, _byte_slices=True):
    """Ghidra's stack-variable naming drifts after callee-cleaned vtable calls (it lost the argument pops),
    so one slot appears under several names (`auStack_a8`, `&uStack_b8`, `auStack_b0 + 4`). The typed export
    records, per call site, the entry-relative slot each `lea`-loaded ECX/EDX/pushed argument points at
    (ExportTypedTranslationUnit: ecxStack/edxStack/pushedStack, exact callee stack purges). The k-th printed
    call of a vtable slot or label is the k-th site in address order.

    Identity is (true slot, lifetime): a Ghidra name constructed at a slot (constructor receiver or hidden
    return of a thing lookup) owns `xStack_<slot>` (a second constructed name at the same slot is a later
    object: `xStack_<slot>_2`, ...); a name Ghidra split off the same slot (distance argument, `+ 4`
    spelling) takes the name of the object constructed nearest before it."""
    if not fn.get('indirectCalls') and not any('pushedStack' in c or 'ecxStack' in c for c in fn.get('calls', [])):
        return decompile
    text = decompile
    uses = []       # (text position, arg index within site, ghidra name, true slot of the name, plus, constructed?)
    sites = []      # (start, end, args, slots or None, label)
    receivers = []  # (start, end, ghidra name, true slot, site position) of unprinted vtable receivers
    site_addr = {}  # start -> the machine-code call site (when the pairing knows it)

    def collect(pos, end, args, slots, label, vtable, ecx=None):
        sites.append((pos, end, args, slots, label))
        # A vtable call whose receiver is NOT printed anywhere (`(**(code **)(NAME + 0x12c))()`, no arguments)
        # hides the object inside the code-pointer expression, where no operand pass can reach it -- and Ghidra's
        # NAME there is its own drifted spelling of a nearby slot (TraderToRescue 0x00DFE0F0 spelled the hostage
        # keeper's `IsAlive()` receiver as the resource member `iStack_140`, four slots down, so the fold read the
        # two liveness tests as calls on the resource and the lifter left them TODO). The export's ecxStack is the
        # receiver's true slot: respell the head as a thing receiver on the object that lives there.
        if vtable and ecx is not None and ecx < 0 and not _receiver_printed(text, pos, args):
            head = RE_BARE_VTABLE_HEAD.search(text[max(0, pos - 200):pos])
            if head is not None and RE_STACK_NAME_ONLY.fullmatch(head.group('base')):
                base_start = max(0, pos - 200) + head.start('base')
                receivers.append((base_start, base_start + len(head.group('base')), head.group('base'), ecx, pos))
        if slots is None:
            return
        for k, (arg, (addr, value)) in enumerate(zip(args, slots)):
            m = RE_STACK_OPERAND.match(arg.strip())
            if not m:
                continue
            by_value = not m.group('amp') and not (m.group('cast') and '*' in m.group('cast')) and not m.group('plus')
            off = (value if value is not None else addr) if by_value else addr   # a bare array name is still passed by address
            if off is not None and off < 0:
                plus = int(m.group('plus'), 0) if m.group('plus') else 0
                # 2 = a constructor label on the receiver, 1 = a thing-slot cast on a vtable call (hidden return OR
                # a thing passed back in: TryAcquire's actor), 0 = plain use
                ctor = 2 if (not by_value and not vtable and k == 0 and _is_ctor_label(label)) else                     1 if (not by_value and vtable and k >= 1 and bool(RE_THING_SLOT_CAST.match(arg.strip()))) else 0
                # the argument's address is the object; the bare name only stands for it when no offset is added
                uses.append((pos, k, m.group('name') if not plus else None, off, plus, ctor))

    ordered = _text_order_sites(text, fn)
    if ordered is not None:
        # exact pairing through the decompiler's token stream (callOrder): printed calls in text order
        for pos, end, args, site, key, vtable in ordered:
            try:
                site_addr[pos] = int(site.get('site', ''), 16)
            except (TypeError, ValueError, AttributeError):
                pass
            collect(pos, end, args, _align(site, args, vtable, vtable and _receiver_printed(text, pos, args)), key, vtable, site.get('ecxStack'))
    by_slot = {}
    for c in fn.get('indirectCalls', []) if ordered is None else []:
        if c.get('slot'):
            by_slot.setdefault(c['slot'].lower(), []).append(c)
    for slot, group in by_slot.items():
        pat = re.compile(r'\(\*\*\(code \*\*\)\([^;\n]*?\+ ' + slot + r'\)\)\s*\(')
        heads = [(m.start(), m.end()) for m in pat.finditer(text)]
        if len(heads) != len(group):
            continue
        for (hs, he), site in zip(heads, sorted(group, key=lambda c: int(c['site'], 16))):
            j, depth = he, 1
            while j < len(text) and depth:
                depth += {'(': 1, ')': -1}.get(text[j], 0)
                j += 1
            args = _split_top(text[he:j - 1])
            collect(he, j - 1, args, _align(site, args, True, _receiver_printed(text, he, args)), slot, True, site.get('ecxStack'))
    by_label = {}
    for c in fn.get('calls', []) if ordered is None else []:
        if c.get('currentName') and ('pushedStack' in c or 'ecxStack' in c or 'edxStack' in c):
            by_label.setdefault(c['currentName'], []).append(c)
    for label, group in by_label.items():
        spans = _call_spans(text, label)
        if len(spans) != len(group) and re.search(r'[?@]', label):
            spans = _call_spans(text, re.sub(r'[^\w:]', '_', label))     # Ghidra's C spelling of a mangled name
        if not spans and '::' in label:
            spans = _call_spans(text, label.rsplit('::', 1)[1])
        if len(spans) != len(group):
            continue
        for (i, a, e), site in zip(spans, sorted(group, key=lambda c: int(c['site'], 16))):
            args = _split_top(text[a:e - 1])
            collect(a, e - 1, args, _align(site, args), label, False)
    if not uses:
        return text
    if _byte_slices:
        slices = sorted(_drifted_byte_slices(text, uses), reverse=True)
        if slices:
            for start, end, repl in slices:
                text = text[:start] + repl + text[end:]
            return restore_stack_operands(text, fn, _byte_slices=False)   # re-pair on the corrected text
    edits = []

    # objects: constructed names per true slot, in order of first construction
    ctor_names = {}   # slot -> [(first pos, ghidra name or None for an offset spelling)]
    for pos, k, name, off, plus, ctor in sorted(uses, key=lambda u: u[0]):
        if ctor and (name is None or all(n != name for _, n in ctor_names.get(off, []))):
            ctor_names.setdefault(off, []).append((pos, name))
    # One Ghidra thing name whose thing-cast / thing ctor-dtor sites the export spreads over several slots at
    # which NO other name is ever constructed is one object seen through a mis-tracked call depth
    # (CheckFriendlyAttacks 0x00D45060: GetThingWithScriptName("PreMeleeMaze") into auStack_a0, exported at
    # -0x90 / -0x80 / -0xa0 across its reset, destructor and TryAcquire-actor sites -> two xStack objects,
    # `thing` never assigned, "TryAcquire requires an actor" in-game 2026-09-20). Ghidra's single name is the
    # evidence: the object lives at its earliest constructed slot. A slot shared with another constructed
    # name is a genuine re-use and is left to the drift logic.
    canonical = {}
    for name in {u[2] for u in uses if u[2] and u[5] == 1}:
        ctorish = sorted((u[0], u[3]) for u in uses if u[2] == name and u[5])
        slots_ = {off for _, off in ctorish}
        if len(slots_) > 1 and all(all(n == name for _, n in ctor_names.get(off, [])) for off in slots_):
            canonical[name] = ctorish[0][1]
    if canonical:
        uses = [(pos, k, name, canonical[name] if name in canonical and not plus else off, plus, ctor) for pos, k, name, off, plus, ctor in uses]
        ctor_names = {}
        for pos, k, name, off, plus, ctor in sorted(uses, key=lambda u: u[0]):
            if ctor and (name is None or all(n != name for _, n in ctor_names.get(off, []))):
                ctor_names.setdefault(off, []).append((pos, name))
    object_name = {}  # (slot, ghidra name) -> xStack name
    for off, lst in ctor_names.items():
        for n, (pos, name) in enumerate(lst):
            object_name[(off, name)] = f'xStack_{-off:x}' + (f'_{n + 1}' if n else '')

    def name_at(off, name, pos):
        if (off, name) in object_name:
            return object_name[(off, name)]
        lst = ctor_names.get(off, [])
        before = [(p, n) for p, n in lst if p <= pos]
        if before:
            return object_name[(off, before[-1][1])]
        if lst:
            return object_name[(off, lst[0][1])]
        # no object was ever constructed at the slot: a Ghidra FLOAT local keeps GHIDRA'S OWN name (self-consistent
        # between its store and its by-value use: `fStack_a0 = angle + 0.25; SetFacingAngle(me, fStack_a0, true)`).
        # The object spelling made the lifter refuse the float store (WillDummy's spin passed nil: in-game sol error
        # on the first lightning hit, 2026-09-21); respelling by the export's per-site number collided with another
        # Ghidra float (SkillTarget's angle copy `fStack_164` -> `fStack_158`, the marker's z: the teleport lost its angle)
        # (the same for an INT local used by value: Skill Guildmaster's talk-comment toggle `iStack_1ec = 1 - iStack_1ec`, later
        # a quest-info tick handle in the same slot -- as `xStack_1ec` the toggle was a TODO and the second comment never played)
        if (name or '').startswith(('fStack_', 'iStack_')):
            return name
        return f'xStack_{-off:x}'

    by_name = {}
    for pos, k, name, off, plus, ctor in uses:
        if name:
            by_name.setdefault(name, []).append((pos, off))
    # a by-value operand Ghidra spelled as a stale register (`GSI->GetTimer(unaff_EBX)`, TraderToRescue 0x00DFE0F0)
    # while the bytes load it from a stack slot at every site (`mov esi, [esp+0x18]; push esi`): the export's pushed
    # VALUE names the slot and an object constructed there is the operand -- accepted only when every site of that
    # register resolves to the SAME object (the export's per-site slots drift, and one drifted site named the other
    # timer); otherwise the register is left as printed
    register_objects = {}
    for start, end, args, slots, label in sites:
        for k, arg in enumerate(args):
            if re.fullmatch(r'unaff_E[A-Z]{2}', arg.strip()) and slots is not None and k < len(slots):
                # the bytes first: `mov REG, [esp+X]` before the push, with the pushes outstanding at that point
                # (TraderToRescue: 0x174 + 16 - 0x18 = 0x16c at every site, where the export's values drifted)
                value = _register_operand_slot(fn, site_addr.get(start), arg.strip()[-3:].lower(), k, bool(args) and _receiver_printed(text, start, args))
                if value is None:
                    value = slots[k][1]
                resolved = name_at(value, None, start) if value is not None and value < 0 and ctor_names.get(value) else None
                register_objects.setdefault(arg.strip(), set()).add(resolved)
    register_objects = {r: next(iter(objs)) for r, objs in register_objects.items() if len(objs) == 1 and None not in objs}
    for start, end, args, slots, label in sites:
        out = list(args)
        for k, arg in enumerate(args):
            m = RE_STACK_OPERAND.match(arg.strip())
            if not m:
                if arg.strip() in register_objects:
                    out[k] = register_objects[arg.strip()]
                continue
            name = m.group('name')
            plus = int(m.group('plus'), 0) if m.group('plus') else 0
            by_value = not m.group('amp') and not (m.group('cast') and '*' in m.group('cast')) and not m.group('plus')
            slot = ((slots[k][1] if slots[k][1] is not None else slots[k][0]) if by_value else slots[k][0]) if slots is not None and k < len(slots) else None
            # an address operand the export places in the caller's own stack parameters (+4k = param_k of a
            # __thiscall): Ghidra drifted it onto a local (V_BookCollecting DoConversation 0x00E56C22 passes the
            # anim-loop string it built in place over param_2 -- `lea ecx,[esp+0x3c]` -- printed as `&local_18`)
            if (slot is not None and slot > 0 and slot % 4 == 0 and m.group('amp') and not plus
                    and re.search(r'\bparam_' + str(slot // 4) + r'\b', text)):
                out[k] = (m.group('cast') or '') + '&param_' + str(slot // 4)
                continue
            addr = slot if slot is not None and slot < 0 else None
            if addr is not None and not plus and name in canonical:
                addr = canonical[name]
            if addr is None:
                near = sorted((abs(p - start), p, o) for p, o in by_name.get(name, []))
                if not near:
                    continue
                addr = near[0][2] + plus
            new = name_at(addr, name if not plus else None, start)
            out[k] = (m.group('cast') or '') + m.group('amp') + new
        if out != args:
            edits.append((start, end, ','.join(out)))
    # one Ghidra head name whose sites the export spreads over several slots, none of which ever holds a
    # constructed object, is ONE object seen through a mis-tracked depth (the same rule the argument pass
    # applies to thing names): it lives at the slot of its earliest site
    receiver_slot = {}
    for _, _, name, off, pos in sorted(receivers, key=lambda r: r[4]):
        receiver_slot.setdefault(name, []).append((pos, off))
    unified = {name: min(lst)[1] for name, lst in receiver_slot.items()
               if len({o for _, o in lst}) > 1 and not any(ctor_names.get(o) for _, o in lst)}
    for start, end, name, off, pos in receivers:
        off = unified.get(name, off)
        # the object that lives at the receiver's true slot, never Ghidra's own spelling of the head (an
        # `iStack_` name there is the drifted one the export contradicts, and name_at would keep it). The
        # export's own slot drifts by a member too (TraderToRescue's second liveness test came out at the
        # keeper thing's +4), so a slot at which nothing is constructed takes the object whose extent covers
        # it -- the nearest construction below it, within one object (a CScriptThing is 8 bytes, a resource 16)
        known = {u[3] for u in uses} | set(ctor_names)
        if off not in known:
            inner = [b for b in known if 0 < off - b <= 12]
            off = max(inner) if inner else off
        new = name_at(off, None, pos)
        edits.append((start, end, f'*(int *){new}'))
    for start, end, repl in sorted(edits, reverse=True):
        text = text[:start] + repl + text[end:]
    # remaining (non-call) spellings: a name with one true slot follows that object; a name Ghidra spread
    # over several slots follows the object of its nearest call-site use (field reads `N._4_4_`, `&N`,
    # inlined constructor stores all sit next to the call that produced or consumed the object)
    # A slot Ghidra typed CCharString that ALSO serves as an integer counter (`X = (CCharString)0x0;`,
    # `X = (CCharString)((int)X + 0xc);`, `(int)X` in an index) keeps Ghidra's own name in that counter phase: its
    # integer uses are not object uses, and following the nearest string object moved OakValeFire 0x00EE8870's byte
    # index `[esp+0x10]` (Ghidra's `local_28`, the "fire" string's slot) onto the CreateEffect name temp at -0x18
    # while its initialisation stayed put -- an uninitialised counter (bytes: 0xEE898B / 0xEE89C1 / 0xEE8A69)
    def counter_phase(name):
        n = re.escape(name)
        if not re.search(r'^[ \t]*' + n + r' = \(CCharString(?:_bv)?\)\(\(int\)' + n + r' \+ (?:0x[0-9a-f]+|\d+)\);', text, re.M):
            return None
        return re.compile(r'\(int\)' + n + r'\b|^[ \t]*' + n + r'(?= = \(CCharString(?:_bv)?\)(?:0x0;|\(\(int\)' + n + r' \+ ))', re.M)
    for name, lst in by_name.items():
        offs = {o for _, o in lst}
        def target(start, lst=lst, name=name, offs=offs):
            if len(offs) == 1:
                return name_at(next(iter(offs)), name, min(p for p, _ in lst))
            pos, off = min(lst, key=lambda u: abs(u[0] - start))
            return name_at(off, name, pos)
        # keep the counter phase under Ghidra's name ONLY when the object renaming would split it across names
        # (its start and its index then read different variables); a consistent renaming is the old behaviour
        # (Trader Conflict / Trader Comment counters rely on it, 2026-09-27 A/B)
        phase = counter_phase(name)
        keep = set()
        if phase is not None:
            starts = [m.end() - len(name) for m in phase.finditer(text)]
            # different SLOTS, not just lifetimes of one slot (`xStack_64` / `xStack_64_2`, which the counter pass
            # unifies: Guild Training AppleGirl / BirdKiller)
            slots = {re.sub(r'^(\w*Stack_[0-9a-f]+)_\d+$', r'\1', target(s)) for s in starts}
            if len(slots) > 1:
                keep = set(starts)
        def rename(m, keep=keep, target=target):
            return m.group(0) if m.start() in keep else target(m.start())
        text = re.sub(r'\b' + re.escape(name) + r'\b', rename, text)
    # a thing object's Info field under its own Ghidra name (`uStack_9c` = `auStack_a0 + 4`: only ever nulled,
    # null-tested and the receiver of thing vcalls -- CheckFriendlyAttacks 0x00D45060's "did the hero hit the Maze"
    # checks) follows the object the thing-cast renamed (`auStack_a0` -> `xStack_90`), as `xStack_90._4_4_`: slot
    # arithmetic on the new name cannot find it once the canonical slot moved
    for name, lst in by_name.items():
        g = re.fullmatch(r'a[uc]Stack_([0-9a-f]+)', name)
        if not g or not any(c for _, _, n, _, _, c in uses if n == name and c == 1):
            continue
        field = f'uStack_{int(g.group(1), 16) - 4:x}'
        if not re.search(r'\(\*\*\(code \*\*\)\(\*' + field + r' \+ (?:0x[0-9a-f]+|\d+)\)\)\(', text):
            continue
        # register copies that only ever hold the field (or null) are the field too (V_StatueMaster Main 0x00ED3B30:
        # `piVar4 = uStack_38; ... uStack_38 = piVar4;` around SM_Center's inlined GetPos, which lifted the later
        # `(**(code **)(*piVar4 + 0x18))()` as the interface's IsXbox)
        nulls = {'(int *)0x0', '0', '0x0'}
        aliases = {field}
        grew = True
        while grew:
            grew = False
            for v in set(re.findall(r'^[ \t]*(\w+) = (?:\(int \*\))?' + field + r';', text, re.M)) - aliases:
                rhs = set(r.strip() for r in re.findall(r'^[ \t]*' + re.escape(v) + r' = ([^;]+);', text, re.M))
                if rhs <= nulls | aliases | {f'(int *){a}' for a in aliases} and not re.search(r'&' + re.escape(v) + r'\b', text):
                    aliases.add(v)
                    grew = True
        stores = set(s.strip() for a in aliases for s in re.findall(r'^[ \t]*' + re.escape(a) + r' = ([^;]+);', text, re.M))
        if not stores or not stores <= nulls | aliases | {f'(int *){a}' for a in aliases}:
            continue
        offs = {o for _, o in lst}
        obj = name_at(next(iter(offs)), name, min(p for p, _ in lst)) if len(offs) == 1 else None
        if obj is None:
            continue
        for a in aliases:
            ea = re.escape(a)
            text = re.sub(r'^[ \t]*(?:undefined4 |int \*\s*)' + ea + r';[ \t]*\r?\n', '', text, flags=re.M)
            text = re.sub(r'^[ \t]*' + ea + r' = [^;]+;[ \t]*\r?\n', '', text, flags=re.M)
            text = re.sub(r'(?<![\w.])' + ea + r'\b', f'{obj}._4_4_', text)
    return text


_MASK = re.compile(r'"(?:[^"\\\n]|\\.)*"|\'(?:[^\'\\\n]|\\.)*\'|/\*[\s\S]*?\*/|//[^\n]*')


def rename_stack_parameters(text):
    """Parameters the export dropped but the reviewed prototype restores (native_function_parameters: named after
    their stack slot, `stack0x00000008`) get their Lua names before the stack-slot passes rename the slot."""
    sig = function_parameters(text, member=True)
    aliases = {p['native']: p['lua'] for p in sig.get('parameters', []) if p['native'].startswith('stack0x')}
    for slot, lua in aliases.items():
        text = re.sub(r'\b' + slot + r'\b', lua, text)
    return text


def unwrap_statements(text):
    """Re-join statements Ghidra wrapped across lines (deeply indented code): while a line's
    parentheses are unbalanced, the next line continues it (`+\n  0x5d4))(`, `4)\n  ,0.5`)."""
    out, buf, depth = [], None, 0
    for line in text.split('\n'):
        if buf is None:
            buf = line
        else:
            tail = line.lstrip()
            head = buf.rstrip()
            buf = head + (tail if tail[:1] in ',)' or head.endswith('::') or (tail[:1] == '(' and head.endswith(')')) else ' ' + tail)
        masked = _MASK.sub(lambda m: ' ' * len(m[0]), buf)
        depth = masked.count('(') - masked.count(')')
        # a qualified name Ghidra wrapped at its `::` (`CScriptGameResourceObjectMovieBase::` newline
        # `~CScriptGameResourceObjectMovieBase(...)`, deep indentation) is one token: every pass keyed on
        # the printed label (`disambiguate_call_labels`, the resource folds) must see it on one line
        if (depth > 0 and not masked.rstrip().endswith('{')) or masked.rstrip().endswith('::'):
            continue
        out.append(buf)
        buf = None
    if buf is not None:
        out.append(buf)
    text = '\n'.join(out)
    # `&CStack_8c.field_0x8`: an address inside a drifted stack object, spelled as the offset form the
    # slot restoration understands (`&CStack_8c + 8`)
    text = re.sub(r'&(\w+Stack_[0-9a-f]+)\.field_0x([0-9a-f]+)\b', lambda m: f'&{m.group(1)} + {int(m.group(2), 16) if int(m.group(2), 16) != 12 else "0xc"}', text)
    return text


class UnitConverter:
    def float_at(self, va):
        # Ghidra prints a double constant exactly like a float one (`(float)_DAT_01238010`); the retail
        # instruction is the width evidence (`fcomp qword ptr [0x1238010]` = 0.25, SkillTarget's scoring rings
        # 0.25/0.5/0.75 -- read as 4-byte floats they were all 0.0 and the archery score never moved)
        if va in self.double_constants():
            raw = self.rdata.bytes_at(va, 8)
            if raw and len(raw) == 8:
                value = struct.unpack('<d', raw)[0]
                return value if value == value and abs(value) < 1e12 else None
        raw = self.rdata.bytes_at(va, 4)
        if not raw or len(raw) != 4:
            return None
        value = struct.unpack('<f', raw)[0]
        return value if value == value and abs(value) < 1e12 else None   # NaN / absurd = not a float constant

    def double_constants(self):
        """Absolute addresses every x87 instruction of the unit reads as `qword ptr` (8-byte constants)."""
        if not hasattr(self, '_double_constants'):
            self._double_constants = set()
            try:
                from capstone import Cs, CS_ARCH_X86, CS_MODE_32
                from capstone.x86 import X86_OP_MEM
            except ImportError:
                return self._double_constants
            cs = Cs(CS_ARCH_X86, CS_MODE_32)
            cs.detail = True
            for fn in self.by_address.values():
                try:
                    start, size = int(fn['address'], 16), int(fn.get('size') or 0)
                except (TypeError, ValueError):
                    continue
                raw = self.rdata.bytes_at(start, size) if size else None
                if not raw:
                    continue
                for ins in cs.disasm(raw, start):
                    if not ins.mnemonic.startswith('f'):
                        continue
                    for op in ins.operands:
                        if op.type == X86_OP_MEM and op.size == 8 and op.mem.base == 0 and op.mem.index == 0:
                            self._double_constants.add(op.mem.disp & 0xffffffff)
        return self._double_constants

    def __init__(self, tu_path, *, flat_control=False):
        self.manifest, self.slots, self.rdata = load_manifest(), load_slots(), RData()
        # GiveHeroObject: retail GSI slot 0x1e4 takes a third bool (`GiveHeroObject(&name, -1, true)` at Gameflow stage 0
        # and in GuildTraining RunTutorials at the teen transition, the five OBJECT_TATTOO_CARD_*_CUSTOM_01); the SDK
        # manifest lists two parameters and the sidecar binding (sol::optional<bool>, value_or(false)) then gave the
        # cards as visible pickups after the split AVI (2026-09-20 v6 run). Unit mode only: the Oakvale gate stays.
        give = self.manifest.get('GiveHeroObject', {})
        if give and [p.get('name') for p in give.get('parameters', [])] == ['objectDefName', 'amount']:
            give['parameters'].append({'name': 'bUnknown', 'type': 'sol::optional<bool>', 'optional': True})
        for name, spec in {**host_bindings(), **sidecar_bindings()}.items():
            self.manifest.setdefault(name, spec)
        # sol::this_state is a binding artefact, never a Lua argument: drop it for arity checks.
        for spec in self.manifest.values():
            if isinstance(spec, dict) and 'parameters' in spec:
                spec['parameters'] = [p for p in spec['parameters'] if p.get('type') != 'sol::this_state']
        self.thing_slots = load_thing_slots()
        self.things, self.returning = load_thing_tables(self.manifest, self.slots)
        tu = json.loads(Path(tu_path).read_text(encoding='utf-8-sig'))
        self.by_address = {f['address'].lower(): f for f in tu['functions']}
        self.name_by_value_string_parameters()
        self.pass_by_value_string_scalars()
        from tools.script_recovery.native_string_returns import recover_string_returns
        self.hidden_string_returns = recover_string_returns(self.by_address, callee_stack_words, _text_order_sites)
        from tools.script_recovery.native_string_returns import recover_void_string_returns
        self.hidden_string_returns |= recover_void_string_returns(self.by_address, callee_stack_words)
        self.code_range = tuple(int(a, 16) for a in tu['range']) if tu.get('range') else None
        self.hidden_thing_returns = {int(f['address'], 16) for f in tu['functions']
                                     if re.search(r'\*in_stack_\w+ = &PTR_\w*_01238c8c;', f.get('decompile') or '')}
        self.checker = LuaSyntaxChecker()
        self.flat_control = flat_control
        self.persist_kinds = persist_kinds_from_spec(tu_path)
        spec_path = Path(tu_path).parent / 'typing_spec.json'
        slot_params = {}
        if spec_path.is_file():
            slot_params = {int(k, 16): len(v.get('params', []))
                           for k, v in json.loads(spec_path.read_text(encoding='utf-8')).get('slots', {}).items()}
        self.slot_words = slot_params.get   # an interface vcall's stack operands (constructor emulation)
        self.out_thing_msgs = out_thing_messages(spec_path, self.manifest)

    def constructor_defaults(self, vtable, fields, exclude):
        """Constant scalar members the retail constructor leaves (ctor_defaults: emulated, read-only); {} when the
        constructor is not identified or not provably emulated."""
        from tools.script_recovery.ctor_defaults import constructor_constants
        try:
            return constructor_constants(list(self.by_address.values()), int(vtable, 16), exclude, fields,
                                         self.slot_words, rdata=self.rdata if getattr(self.rdata, 'ok', False) else None)
        except (TypeError, ValueError):
            return {}

    def vector_copy_targets(self):
        """Addresses of `std::vector<CScriptThing>` copy constructors in the unit (bsim mislabels them: a body that
        calls the vector initialiser and copy-constructs each element with the CScriptThing allocator)."""
        if not hasattr(self, '_vector_copy_targets'):
            self._vector_copy_targets = {int(a, 16) for a, f in self.by_address.items()
                                         if 'Std_Vector_Initialize(' in (f.get('decompile') or '')
                                         and '_Cons_val<std::allocator<CScriptThing>,CScriptThing,CScriptThing>' in f['decompile']}
        return self._vector_copy_targets

    def name_vector_copies(self, decompile, fn):
        """`Label((T *)&local);` where Label's target is a vector copy constructor and the source operand
        (a `lea reg, [this + OFF]; push reg` Ghidra dropped) is a member vector: `Vector_CopyFrom(&local, this + OFF)`."""
        targets = self.vector_copy_targets()
        if not targets:
            return decompile
        try:
            from capstone import Cs, CS_ARCH_X86, CS_MODE_32
            from capstone.x86 import X86_OP_MEM, X86_OP_REG
        except ImportError:
            return decompile
        cs = Cs(CS_ARCH_X86, CS_MODE_32)
        cs.detail = True
        for call in fn.get('calls', []):
            try:
                target = int(call.get('target', '0'), 16)
                site = int(call['site'], 16)
            except (TypeError, ValueError):
                continue
            if target not in targets or not call.get('currentName'):
                continue
            raw = self.rdata.bytes_at(site - 24, 24) or b''
            pushed = None
            insns = list(cs.disasm(raw, site - 24))
            for k, ins in enumerate(insns):
                if ins.mnemonic == 'push' and ins.operands and ins.operands[0].type == X86_OP_REG and k:
                    prev = insns[k - 1]
                    if prev.mnemonic == 'lea' and prev.operands[0].reg == ins.operands[0].reg and prev.operands[1].type == X86_OP_MEM:
                        pushed = prev.operands[1].mem.disp
            if pushed is None:
                continue
            label = call['currentName']
            for spelling in (label, label.removeprefix('NScript::'), label.split('::')[-1]):
                pat = re.compile(r'^([ \t]*)' + re.escape(spelling).replace('::', r'\s*::\s*') + r'\s*\(\(\w+ \*\)&(\w+)\);', re.M)
                decompile, n = pat.subn(lambda m: f'{m.group(1)}Vector_CopyFrom((void *)&{m.group(2)},(void *)(this + {pushed:#x}));', decompile, count=1)
                if n:
                    break
        return decompile

    CCHARSTRING_CTOR = 0x99EBF0     # CCharString::CCharString(const char*, int)
    THING_COPY_CTOR = 0x4ABE90      # CScriptThing::CScriptThing(const CScriptThing&): vtable 0x1238c8c, Data/Info copied, Info refcount++ (disasm 2026-09-21)

    def repair_literal_receiver_labels(self, decompile, renamed, fn):
        """`disambiguate_call_labels` falls back to address order when the token pairing fails (a 31 KB
        restructured Main), and then a label shared by a `__fastcall(int)` helper and two `__thiscall(CCharString*)`
        helpers (the three logbook entry helpers under bsim's `CSubtitleRenderer::SetText`) gets its `__at`
        suffixes crossed. A print whose receiver is a stack string constructed from a literal on the line
        before is re-paired with the site whose machine code constructs that same literal into ECX
        (`push LIT; ...; call CCharString ctor; lea ecx,[slot]; call target`): the literal is the evidence.
        Prints with no string receiver are handed the remaining (non-ECX-stack) targets when that is unique."""
        by_base = {}
        for new, target in renamed.items():
            base = new.rsplit('__at', 1)[0]
            by_base.setdefault(base, {})[new] = target
        try:
            from capstone import Cs, CS_ARCH_X86, CS_MODE_32
            from capstone.x86 import X86_OP_IMM
        except ImportError:
            return decompile
        cs = Cs(CS_ARCH_X86, CS_MODE_32)
        cs.detail = True

        def site_literal(site):
            raw = self.rdata.bytes_at(site - 48, 48) or b''
            literal, ctor_seen = None, False
            for ins in cs.disasm(raw, site - 48):
                if ins.mnemonic == 'push' and ins.operands and ins.operands[0].type == X86_OP_IMM:
                    s = self.rdata.string_at(ins.operands[0].imm)
                    if s is not None:
                        literal, ctor_seen = s, False
                elif ins.mnemonic == 'call' and ins.operands and ins.operands[0].type == X86_OP_IMM:
                    if ins.operands[0].imm == self.CCHARSTRING_CTOR:
                        ctor_seen = True
                    elif ins.address != site:
                        literal, ctor_seen = None, False
            return literal if ctor_seen else None

        for base, variants in by_base.items():
            targets = set(variants.values())
            sites = [c for c in fn.get('calls', []) if c.get('currentName') == base and int(c['target'], 16) in targets]
            if not sites or len(targets) < 2:
                continue
            literal_sites = {}
            for c in sites:
                if 'ecxStack' not in c:
                    continue
                lit = site_literal(int(c['site'], 16))
                if lit is not None:
                    literal_sites.setdefault(lit, set()).add(int(c['target'], 16))
            if not literal_sites or any(len(v) > 1 for v in literal_sites.values()):
                continue
            head = re.compile(r'^([ \t]*)CCharString::CCharString\((?:\(CCharString \*\))?&?(\w+),("(?:[^"\\]|\\.)*"),-1\);[ \t]*\r?\n'
                              r'([ \t]*(?:\w+ = )?(?:\([\w :*]+\))?)(' + re.escape(base) + r')__at[0-9a-f]+\((?:\([\w :*]+\))?&?\2\b', re.M)
            plain = {int(c['target'], 16) for c in sites if 'ecxStack' not in c}

            def fix(m):
                lit = m.group(3)[1:-1]
                target = literal_sites.get(lit)
                if not target:
                    return m.group(0)
                target = next(iter(target))
                new = f'{base}__at{target:x}'
                renamed[new] = target
                return f'{m.group(1)}CCharString::CCharString(&{m.group(2)},{m.group(3)},-1);\n{m.group(4)}{new}(&{m.group(2)}'
            decompile = head.sub(fix, decompile)
            if len(plain) == 1:
                other = next(iter(plain))
                new = f'{base}__at{other:x}'
                renamed[new] = other
                # a print with an immediate / register operand and no string receiver: the fastcall target
                decompile = re.sub(r'(?<![\w:])' + re.escape(base) + r'__at[0-9a-f]+(?=\((?:0x[0-9a-f]+|\d+|\w+)\);)', new, decompile)
        return decompile

    def name_by_value_string_parameters(self):
        """A helper taking a CCharString by value on the stack: Ghidra shows no parameter, the body reads
        `&stack0x00000004` and each caller constructs the string on its outgoing stack
        (`CCharString::CCharString((CCharString *)&stack0xffffffcc,"LIT",-1); Helper(this);`). The parameter
        gets a name in the prototype and the body; the callers pass the literal."""
        for address, fn in self.by_address.items():
            text = fn.get('decompile') or ''
            if '&stack0x00000004' not in text:
                continue
            header = re.search(r'^(\w[\w :<>,*]*?\b\w+)\((\w[\w *]*?\*?\s*\w+)\)\s*\r?\n\s*\{', text, re.M)
            if not header:
                continue
            # K strings by value (`&stack0x00000004` .. `&stack0x00000010`: V_BookCollecting AddGossip 0x00E55C60 takes
            # category, text key, village, faction -- only the first was recovered)
            count = 1
            while f'&stack0x{4 * (count + 1):08x}' in text:
                count += 1
            params = ''.join(f',CCharString *strParam_{k}' for k in range(1, count + 1))
            body = text[:header.start(2)] + header.group(2) + params + text[header.end(2):]
            for k in range(1, count + 1):
                slot = f'&stack0x{4 * k:08x}'
                body = body.replace(f'(CCharString_bv *){slot}', f'strParam_{k}').replace(slot, f'strParam_{k}')
            fn['decompile'] = body
            target = int(address, 16)
            for caller in self.by_address.values():
                labels = {c['currentName'] for c in caller.get('calls', []) if c.get('currentName') and int(c.get('target', '0'), 16) == target}
                for label in labels:
                    for spelling in (label, label.removeprefix('NScript::'), label.split('::')[-1]):
                        # the receiver is `this` or, in an entity binding that reaches its script through a field,
                        # that field (TraderToRescue 0x00DFE0F0's outro: `Helper(*(undefined4 *)(this + 0x14))`,
                        # the same slot its neighbours write -- the call kept its receiver and lost the string,
                        # so "CS_TRADERCON_GOOD_OUTRO" never reached the Lua, 2026-09-22)
                        one = r'[ \t]*CCharString::CCharString\s*\(\(CCharString \*\)&stack0x[0-9a-f]{8},\s*(?:"[^"]*"|\w+),-1\);[ \t]*\r?\n'
                        pat = re.compile(r'^(?P<ctor>(?:' + one + r'){' + str(count) + r'})'
                                         r'(?P<ind>[ \t]*)' + re.escape(spelling).replace('::', r'\s*::\s*')
                                         + r'\s*\((?P<recv>this|\*\((?:undefined4 \*|\w+ \*\*)\)\((?:\(int\))?this \+ (?:0x[0-9a-f]+|\d+)\))\);', re.M)

                        def pass_literals(m):
                            # the lowest outgoing slot is the first parameter
                            made = re.findall(r'&stack0x([0-9a-f]{8}),\s*("[^"]*"|\w+),-1', m.group('ctor'))
                            lits = [lit for _, lit in sorted(made, key=lambda p: int(p[0], 16))]
                            return f'{m.group("ctor")}{m.group("ind")}{spelling}({m.group("recv")},{",".join(lits)});'
                        # the temporaries' constructors stay: every printed call keeps its place in the callOrder pairing
                        caller['decompile'], n = pat.subn(pass_literals, caller.get('decompile') or '')
                        if n:
                            break

    def pass_by_value_string_scalars(self):
        """A member taking `(class CCharString, bool)` (ego_r) that Ghidra did type (`this, undefined4 param_2, char
        param_3`), called with only the receiver printed: each caller builds the string in the outgoing slot
        (`CCharString::CCharString((CCharString *)&stack0x..,"CS_BORDELLO_KICKEDOUT",-1);`) and pushed the bool
        before it (`push 0` / `push 1` / a callee-saved register written once, `xor edi,edi`). The call gets both
        operands back. (V_Bordello PlayCutscene 0x00E3E720: every entity cutscene lost its macro name.)"""
        try:
            from capstone import Cs, CS_ARCH_X86, CS_MODE_32
            from capstone.x86 import X86_OP_IMM, X86_OP_REG
        except ImportError:
            return
        cs = Cs(CS_ARCH_X86, CS_MODE_32)
        cs.detail = True
        targets = set()
        for address, fn in self.by_address.items():
            comment = re.search(r'/\*\s*\[bsim[^\]]*\]([\s\S]*?)\*/', fn.get('decompile') or '')
            if (comment and re.search(r'__thiscall\s+[\w:]+\(class\s+CCharString,\s*bool\)', comment[1])
                    and re.search(r'__thiscall\s+[\w:]+\(\w+\s*\*this,\s*undefined4\s+\w+,\s*(?:char|bool)\s+\w+\)', fn['decompile'])):
                targets.add(int(address, 16))
        if not targets:
            return
        ctor = r'CCharString::CCharString\s*\(\(CCharString \*\)&stack0x[0-9a-f]{8},\s*(?P<lit>"[^"]*"|\w+),\s*-1\);\s*'
        for caller in self.by_address.values():
            sites = [c for c in caller.get('calls', []) if int(c.get('target', '0'), 16) in targets]
            if not sites:
                continue
            text = caller.get('decompile') or ''
            from tools.script_recovery.native_string_returns import _target_sites
            entries = [(a, e, args, site, name, vt) for target in targets
                       for a, e, args, site, name, vt in _target_sites(text, caller, [c for c in sites if int(c['target'], 16) == target])]
            size = int(caller['bodyEndExclusive'], 16) - int(caller['address'], 16) if caller.get('bodyEndExclusive') else 0
            if not size:
                continue
            insns = list(cs.disasm(self.rdata.bytes_at(int(caller['address'], 16), size) or b'', int(caller['address'], 16)))
            index = {ins.address: i for i, ins in enumerate(insns)}

            def last_write(i, reg):
                # the nearest earlier write in address order: a callee-saved register VC7.1 keeps as a zero/one
                # constant (`xor edi,edi` in the prologue, re-zeroed later in the body)
                for ins in reversed(insns[:i]):
                    if not (ins.operands and ins.operands[0].type == X86_OP_REG and ins.reg_name(ins.operands[0].reg) == reg):
                        continue
                    if ins.mnemonic in ('push', 'cmp', 'test'):
                        continue
                    if ins.mnemonic == 'xor' and ins.operands[1].type == X86_OP_REG and ins.reg_name(ins.operands[1].reg) == reg:
                        return 0
                    if ins.mnemonic == 'mov' and ins.operands[1].type == X86_OP_IMM:
                        return ins.operands[1].imm
                    return None
                return None
            edits = []
            for a, e, args, site, key, vtable in entries:
                if vtable or int(site.get('target', '0'), 16) not in targets or len(args) != 1:
                    continue
                head = re.search(ctor + r'(?:[\w:]+\s*)$', text[:a - 1]) if text[a - 1] == '(' else None
                i = index.get(int(site['site'], 16))
                if not head or i is None or i < 7:
                    continue
                # push BOOL; [test/jcc]; push ecx; mov ecx,esp; push -1; push lit; call ctor; mov ecx,[..]; call target
                if [f'{x.mnemonic} {x.op_str}' for x in insns[i - 6:i - 4]] != ['push ecx', 'mov ecx, esp']:
                    continue
                k = i - 7
                while k > 0 and (insns[k].mnemonic.startswith('j') or insns[k].mnemonic in ('test', 'cmp')):
                    k -= 1
                if insns[k].mnemonic != 'push':
                    continue
                op = insns[k].operands[0]
                value = op.imm if op.type == X86_OP_IMM else last_write(k, insns[k].reg_name(op.reg)) if op.type == X86_OP_REG else None
                if value not in (0, 1):
                    continue
                edits.append((a, e, f'{args[0]},{head["lit"]},{"true" if value else "false"}'))
            for a, e, replacement in sorted(edits, reverse=True):
                text = text[:a] + replacement + text[e:]
            caller['decompile'] = text

    APPEND_CSTRING = 0x99F600    # CCharString::AppendCString(dest, src, const char*) — __fastcall + one stack operand

    def name_append_literals(self, decompile, fn):
        """`AppendCString(dest, src, piVar9)` with `piVar9 = &iStack_70;`: Ghidra lost the pushed literal behind the
        earlier hidden-result push of `GetDataString`. The nearest `push <.rdata string>` before the call site is
        the operand."""
        sites = [int(c['site'], 16) for c in fn.get('calls', []) if int(c.get('target', '0'), 16) == self.APPEND_CSTRING]
        if not sites:
            return decompile
        try:
            from capstone import Cs, CS_ARCH_X86, CS_MODE_32
            from capstone.x86 import X86_OP_IMM
        except ImportError:
            return decompile
        cs = Cs(CS_ARCH_X86, CS_MODE_32)
        cs.detail = True
        literals = []
        for site in sites:
            raw = self.rdata.bytes_at(site - 96, 96) or b''
            pushed = None
            for start in range(0, 16):      # find a decode alignment that reaches the call site exactly
                insns = list(cs.disasm(raw[start:], site - 96 + start))
                if insns and insns[-1].address + insns[-1].size == site:
                    for ins in insns:
                        if ins.mnemonic == 'push' and ins.operands and ins.operands[0].type == X86_OP_IMM:
                            literal = self.rdata.string_at(ins.operands[0].imm)
                            if literal is not None:
                                pushed = literal
                    break
            literals.append(pushed)
        by_site = dict(zip(sites, literals))
        # exact pairing through the decompiler's token stream (text order != address order in restructured code)
        entries = _text_order_sites(decompile, fn) or []
        edits = []
        for a, e, args, site, key, vtable in entries:
            literal = by_site.get(int(site['site'], 16))
            if vtable or literal is None or len(args) != 3:
                continue
            # (`(int *)pCVar4` too: the pointer temp Ghidra also handed to the GetDataString vcall as the hidden
            # result, TraderToRescue 0x00DFF2CF / 0x00E00118 / the _FREED_10 site -- the readable appended the stale
            # `getDataString` local instead of the suffix, 2026-09-20 audit)
            if re.fullmatch(r'(?:\(int \*\))?(?:&\w+|p[iC]Var\d+)', args[2].strip()):
                edits.append((a, e, f'{args[0]},{args[1]},"{literal}"'))
        for a, e, replacement in sorted(edits, reverse=True):
            decompile = decompile[:a] + replacement + decompile[e:]
        return decompile

    def recover_dropped_operands(self, decompile, fn):
        """A vtable call the decompiler printed with no operands at all (`(**(code **)(iVar11 + 0x5b4))();` in a
        function whose stack analysis broke) is rebuilt from the machine code: the last N pushes before the site,
        N = the binding's parameter count (+1 for a by-value result slot), nested calls skipping their own pushes.
        An immediate is the literal (a .rdata address is its string), a `lea`-loaded push is the stack slot
        (entry-relative through the site's recorded depth), `push eax` right after a call is that call's result
        (the printed void statement of the call gets a name), a register is traced to its last write (`this`,
        `this + 8`, a member load, an immediate); anything else stays unknown."""
        entries = _text_order_sites(decompile, fn)
        if not entries:
            return decompile
        try:
            from capstone import Cs, CS_ARCH_X86, CS_MODE_32
            from capstone.x86 import X86_OP_IMM, X86_OP_REG, X86_OP_MEM
        except ImportError:
            return decompile
        cs = Cs(CS_ARCH_X86, CS_MODE_32)
        cs.detail = True
        by_value_results = ('std::string', 'CCharString', 'C3DVector')     # a CScriptThing result travels in eax (0x118 GetHero: no slot)

        def expected_kinds(site):
            """Operand kinds of a vtable call from its signature ('string' / 'thing' / 'scalar' per push, a
            leading 'slot' for a by-value result): an interface call (ecx = this+4 / this+0x40) through the
            binding manifest, a CScriptThing call (ecx = a thing) through the decorated vtable name; None
            when the site is neither. The native push order is unknown (the binding may reorder), so the
            kinds are a multiset."""
            if site.get('kind') != 'vtable' or not site.get('slot'):
                return None
            slot = int(site['slot'], 16)
            # (a site whose ecx the export could not value: an interface slot no CScriptThing vtable has is the interface)
            iface = site.get('ecxValue') in (4, 0x40) or (site.get('ecxValue') is None and slot in self.slots and slot not in self.thing_slots)
            if iface:
                name = self.slots.get(slot)
                spec = self.manifest.get(name) if name else None
                if not spec:
                    return None
                kinds = []
                for prm in spec.get('parameters', []):
                    t = str(prm.get('type', ''))
                    kinds.append('string' if 'string' in t or 'CCharString' in t else 'thing' if 'CScriptThing' in t else 'scalar')
                if any(t in str(spec.get('returnType', '')) for t in by_value_results):
                    kinds.append('slot')
                return kinds
            entry = self.thing_slots.get(slot)
            sig = parse_thing_signature(entry[1]) if entry else None
            if sig is None:
                return None
            result, kinds, by_value = sig
            return [('string' if k == 'string' else 'thing' if k == 'thing' else 'scalar') for k in kinds] + (['slot'] if by_value else [])

        def expected_pushes(site):
            kinds = expected_kinds(site)
            return None if kinds is None else len(kinds)

        def compatible(kinds, found):
            """Every recovered push must fit a distinct expected operand: a string / by-value slot takes a stack
            address or a string-returning call result, a thing a pointer (call result, register expression,
            stack address), a scalar an immediate, register value or call result; a `.rdata` literal push
            never feeds a CCharString operand. A small exact matching (the binding may reorder operands)."""
            def fits(k, kind):
                return (kind == 'unknown' or
                        (k in ('string', 'slot') and kind in ('slot', 'result')) or
                        (k == 'thing' and kind in ('result', 'slot', 'expr')) or
                        (k == 'scalar' and kind in ('imm', 'expr', 'result')))

            def match(i, pool):
                if i == len(found):
                    return True
                kind = found[i][0]
                for j, k in enumerate(pool):
                    if fits(k, kind) and match(i + 1, pool[:j] + pool[j + 1:]):
                        return True
                return False
            return len(found) == len(kinds) and match(0, list(kinds))

        pushes_at = {}
        callee_words = {}   # direct call site -> stack words proven by the callee's RET (caller cleanup added in the walk)
        for c in list(fn.get('calls', [])) + list(fn.get('indirectCalls', [])):
            try:
                addr = int(c['site'], 16)
            except (KeyError, ValueError):
                continue
            pushes_at[addr] = expected_pushes(c) if expected_pushes(c) is not None else len(c.get('pushedStack') or [])
            # __fastcall string helpers with ONE stack operand that VC7.1 pushes early (TraderToRescue 0x00DFF2DB:
            # `push "_THREATEN"` sits above the GetDataString and operator+ calls until AppendCString at 0x00DFF308
            # consumes it): the export's backward scan stops at the calls in between and records no push, so the
            # walk skipped nothing and the enclosing AddLineToConversation took the literal as its speaker
            # (`(id, text, "_THREATEN", 0, me)` -> `(hero, nil)` in the readable, 2026-09-20 audit)
            try:
                target = int(c.get('target', '0'), 16)
            except ValueError:
                target = 0
            if target in (0x99F600, 0x99F690):        # CCharString::AppendCString(ecx dest, edx src, [const char*]); operator+(ecx dest, edx lit, [const CCharString&])
                pushes_at[addr] = 1
            elif target and c.get('kind') != 'vtable':
                # a direct callee's own stack operands are what its RET purges, not the export's push record: that
                # record also swallows the NEXT call's early pushes (MakeTraderComment 0x00E022DF: GetDataString
                # recorded 7 pushes for its one hidden result; AppendData 0x0099F570 recorded none for its one) and
                # the walk handed AddLineToConversation the concat literals as operands
                words = callee_stack_words(target)
                if words is not None:
                    callee_words[addr] = words
                    pushes_at[addr] = words
        by_site = {int(site['site'], 16): (a, e, args) for a, e, args, site, key, vtable in entries}
        entry = int(fn['address'], 16)
        body = None

        def window(site):
            nonlocal body
            if body is None:
                size = int(fn['bodyEndExclusive'], 16) - entry if fn.get('bodyEndExclusive') else 0
                raw = self.rdata.bytes_at(entry, max(size, site - entry + 16)) or b''
                body = list(cs.disasm(raw, entry))
            insns = [ins for ins in body if ins.address < site]
            return insns if insns and insns[-1].address + insns[-1].size == site else None

        CALLEE_SAVED = {'ebx', 'esi', 'edi', 'ebp'}

        def register_value(insns, i, reg, depth):
            if depth > 4:
                return None
            while i >= 0:
                ins = insns[i]
                if ins.mnemonic == 'call' and reg not in CALLEE_SAVED:
                    return ('result', ins.address) if reg == 'eax' else None      # the call's result
                if ins.mnemonic in ('mov', 'lea', 'xor') and ins.operands and ins.operands[0].type == X86_OP_REG and ins.reg_name(ins.operands[0].reg) == reg:
                    src = ins.operands[1]
                    if ins.mnemonic == 'xor' and src.type == X86_OP_REG and ins.reg_name(src.reg) == reg:
                        return '0'
                    if ins.mnemonic == 'mov' and src.type == X86_OP_IMM:
                        return hex(src.imm) if src.imm > 9 else str(src.imm)
                    if ins.mnemonic == 'mov' and src.type == X86_OP_REG:
                        return register_value(insns, i - 1, ins.reg_name(src.reg), depth + 1)     # (a copied call result stays a tuple)
                    if ins.mnemonic == 'lea' and src.type == X86_OP_MEM and src.mem.base and not src.mem.index and ins.reg_name(src.mem.base) == 'esp':
                        return ('lea_esp', src.mem.disp, i)                # a stack slot address (resolved at the push)
                    if src.type == X86_OP_MEM and src.mem.base and not src.mem.index and ins.reg_name(src.mem.base) != 'esp':
                        base = register_value(insns, i - 1, ins.reg_name(src.mem.base), depth + 1)
                        if base is None or isinstance(base, tuple):
                            return None
                        disp = src.mem.disp
                        addr = f'{base} + {disp if disp < 10 else hex(disp)}' if disp else base
                        return f'({addr})' if ins.mnemonic == 'lea' else f'*(int *)({addr})'
                    return None
                if ins.mnemonic == 'pop' and ins.operands and ins.reg_name(ins.operands[0].reg) == reg:
                    # an epilogue of an earlier return path (`pop ebp ... ret`) is not on the way to this site
                    j = i + 1
                    while j < len(insns) and insns[j].mnemonic in ('pop', 'add', 'mov', 'lea') and j - i < 8:
                        j += 1
                    if j < len(insns) and insns[j].mnemonic == 'ret':
                        i -= 1
                        continue
                    return None
                i -= 1
            return 'this' if reg == 'ecx' else None       # __thiscall: ecx at entry is the receiver

        def collect(insns, i, need, k=0):
            """Walk back from insns[i] gathering `need` pushes (last push first); `k` counts the pushes of the
            outer call already passed (the stack displacement for `lea esp` slots)."""
            out = []
            while need > 0 and i >= 0:
                ins = insns[i]
                if ins.mnemonic == 'call' and ins.operands and ins.operands[0].type == X86_OP_IMM and ins.operands[0].imm == self.THING_COPY_CTOR:
                    # a CScriptThing passed BY VALUE: `sub esp, 0xc | mov ecx, esp | push SRC | call CScriptThing::CScriptThing(const&)`
                    # is one 12-byte operand, the copy of SRC (TraderToRescue 0x00DFEC55: SetIsPushableByHero(hero, 1) printed
                    # with no operands; the walker used to stop at this call and the thing came back as `__unknown_push`)
                    if (i >= 3 and insns[i - 1].mnemonic == 'push' and insns[i - 1].operands[0].type == X86_OP_REG
                            and insns[i - 2].mnemonic == 'mov' and insns[i - 2].op_str == 'ecx, esp'
                            and insns[i - 3].mnemonic == 'sub' and insns[i - 3].op_str.startswith('esp, ')):
                        traced = register_value(insns, i - 2, insns[i - 1].op_str, 0)
                        out.append(traced if isinstance(traced, tuple) else ('expr', traced))
                        need -= 1
                        i -= 4
                        continue
                    return None
                if ins.mnemonic == 'call':
                    n = pushes_at.get(ins.address)
                    if n is None:
                        return None
                    if ins.address in callee_words and n == 0:
                        n = caller_cleanup_words(insns, i)     # a __cdecl callee: the caller's `add esp, N`
                        if n is None:
                            return None
                    skipped = collect(insns, i - 1, n, 0)
                    if skipped is None:
                        return None
                    i = skipped[1]
                    continue
                if ins.mnemonic == 'push':
                    op = ins.operands[0]
                    prev = insns[i - 1] if i > 0 else None
                    if op.type == X86_OP_IMM:
                        out.append(('imm', op.imm))
                    elif (op.type == X86_OP_REG and prev is not None and prev.mnemonic == 'lea' and ins.reg_name(prev.operands[0].reg) == ins.op_str
                          and prev.operands[1].type == X86_OP_MEM and ins.reg_name(prev.operands[1].mem.base) == 'esp' and not prev.operands[1].mem.index):
                        out.append(('slot', (prev.operands[1].mem.disp, k + len(out) + 1)))
                    elif op.type == X86_OP_REG:
                        traced = register_value(insns, i - 1, ins.op_str, 0)
                        if isinstance(traced, tuple) and traced[0] == 'lea_esp':
                            # pushes between the lea and this push moved esp down by 4 each
                            between = sum(1 for x in insns[traced[2] + 1:i] if x.mnemonic == 'push')
                            out.append(('slot', (traced[1], k + len(out) + between + 1)))   # +1: the site depth includes this push
                        else:
                            out.append(traced if isinstance(traced, tuple) else ('expr', traced))
                    else:
                        out.append(('unknown', None))
                    need -= 1
                i -= 1
            return (out, i) if need == 0 else None

        edits = []
        named = {}
        counter = [0]
        for a, e, args, site, key, vtable in entries:
            need = expected_pushes(site)
            printed = [x for x in args if x.strip()]
            if not need or len(printed) >= need:
                continue
            insns = window(int(site['site'], 16))
            if not insns:
                continue
            got = collect(insns, len(insns) - 1, need)
            if got is None or not compatible(expected_kinds(site), got[0]):
                continue
            rendered = []
            for kind, value in got[0]:
                if kind == 'imm':
                    literal = self.rdata.string_at(value) if value >= 0x400000 else None
                    if literal is None and (0x3a000000 <= value <= 0x4b000000 or 0xba000000 <= value <= 0xcb000000):
                        rendered.append(repr(struct.unpack('<f', struct.pack('<I', value))[0]))     # a pushed float (1.0f = 0x3f800000)
                    else:
                        rendered.append(f'"{literal}"' if literal is not None else (hex(value) if value > 9 else str(value)))
                elif kind == 'slot' and site.get('depth') is not None:
                    disp, after = value
                    off = -int(site['depth']) + 4 * after + disp      # entry-relative address of the slot
                    rendered.append(f'&xStack_{-off:x}' if off < 0 else f'&stack0x{off:08x}')
                elif kind == 'result' and value in by_site:
                    name = named.get(value)
                    if name is None:
                        ra, _re, _rargs = by_site[value]
                        # the statement holding that call (Ghidra wraps long labels over several lines)
                        head = max(decompile.rfind(';', 0, ra), decompile.rfind('{', 0, ra), decompile.rfind('}', 0, ra)) + 1
                        line = decompile[head:ra]
                        assigned = re.match(r'^\s*(\w+) = (?:\([\w *]+\))?\s*(?:\(\*\*\(code \*\*\)|(?:[\w:]+\s*)+\($)', line)
                        if assigned:
                            name = named[value] = assigned.group(1)
                        elif re.match(r'^\s*\(\*\*\(code \*\*\)|^\s*(?:[\w:]+\s*)+\($', line):
                            counter[0] += 1
                            name = named[value] = f'__push{counter[0]}'
                            indent = len(line) - len(line.lstrip())
                            edits.append((head + indent, head + indent, f'{name} = '))
                    rendered.append(name or '__unknown_push')
                elif kind == 'expr' and value is not None:
                    # (a register traced through a copy of an already-parenthesised lea comes back double-wrapped,
                    # `((this + 8))`: the lifter's `me` receiver shape wants one pair)
                    while value.startswith('((') and value.endswith('))') and _balanced(value[1:-1]):
                        value = value[1:-1]
                    rendered.append(value)
                else:
                    rendered.append('__unknown_push')
            if printed and not all(any(x.strip() == r for r in rendered) for x in printed):
                continue          # a partly printed call is only rebuilt when every printed operand was found again
            edits.append((a, e, ','.join(rendered)))
        for start, end, replacement in sorted(edits, key=lambda x: (x[0], x[1]), reverse=True):
            decompile = decompile[:start] + replacement + decompile[end:]
        return decompile

    def restore_local_helper_operands(self, decompile, fn):
        """Operands a local helper call lost (native_local_helper_operands), decoded from the pushes before it."""
        from tools.script_recovery.native_local_helper_operands import restore_local_helper_operands

        def arity_of(target):
            helper = self.native(target)
            if not helper or '__thiscall' not in helper['decompile'][:1500]:
                return None
            return len(function_parameters(helper['decompile'], member=True).get('parameters', []))
        return restore_local_helper_operands(decompile, fn, _text_order_sites(decompile, fn), arity_of, self.rdata)

    def native(self, address):
        fn = self.by_address.get(address.lower())
        return fn if fn and fn.get('decompile') else None

    def snapshot_container_helpers(self, unit):
        """Unit helpers reachable ONLY through a definition-snapshot fill call that the lowering drops
        (`definitionSnapshots`: V_BookCollecting's std::vector<CConversation> operator= 0x00E54AA0 and the copy /
        destroy helpers behind it): with the member read from the definitions they have no caller left."""
        snapshots = unit['quest'].get('definitionSnapshots', {})
        if not snapshots:
            return set()
        functions = [f for n, f in unit['quest']['functions'].items() if n not in SKIP_ROLES]
        for entity in unit['entities'].values():
            functions += [f for n, f in {**entity['functions'], **entity.get('helpers', {})}.items() if n not in SKIP_ROLES]
        names = {f['address'].lower(): n for n, f in unit['quest']['functions'].items()}
        for entity in unit['entities'].values():
            names.update({f['address'].lower(): n for n, f in {**entity['functions'], **entity.get('helpers', {})}.items()})
        addresses = {f['address'].lower() for f in functions}
        graph, fill_edges = {}, set()
        for address in addresses:
            fn = self.native(address)
            if not fn:
                return set()
            labels = {c.get('currentName'): str(c['target']).lower() for c in fn.get('calls', []) if c.get('currentName')}
            graph[address] = {str(c['target']).lower() for c in fn.get('calls', [])} & addresses
            for off, snap in snapshots.items():
                o = int(off, 16)
                spelled = r'(?:\(int\))?\(?(?:this|param_1)\)? \+ (?:' + hex(o) + '|' + str(o) + r')\b'
                for m in re.finditer(r'(\w+)\(\s*' + spelled + r'\)?,\s*DAT_0143e90c \+ ' + snap['global'] + r'\);', fn['decompile']):
                    if m.group(1) in labels:
                        fill_edges.add((address, labels[m.group(1)]))
        if not fill_edges:
            return set()
        def reachable(roots, cut):
            found, pending = set(), list(roots)
            while pending:
                a = pending.pop()
                if a in found:
                    continue
                found.add(a)
                pending.extend(t for t in graph.get(a, ()) if (a, t) not in cut)
            return found
        roots = {a for a in addresses if not names.get(a, '').startswith('helper_')}
        kept = reachable(roots, fill_edges)
        dropped = reachable({t for _, t in fill_edges}, set()) - kept
        return {a for a in dropped if names.get(a, '').startswith('helper_') or names.get(a) == 'Copy'}

    def convert(self, unit, out):
        package = unit['package']
        quest_functions = {n: f for n, f in unit['quest']['functions'].items() if n not in SKIP_ROLES}
        # std::vector<CScriptThing> copy constructors / initialisers the evidence pass took for helpers
        # (bsim-named library bodies inside the unit range): the lowering folds their call sites instead
        library = {f'0x{a:08x}' for a in self.vector_copy_targets()}
        library |= {f['address'].lower() for n, f in quest_functions.items()
                    if f['address'].lower() not in library and (nf := self.native(f['address']))
                    and nf.get('callers') and all(str(c.get('functionAddress', '')).lower() in library for c in nf['callers'])}
        from tools.script_recovery.native_arena_rounds import replaced_container_helpers
        arena_containers = replaced_container_helpers(unit, self.native)
        library |= arena_containers
        snapshot_containers = self.snapshot_container_helpers(unit)
        library |= snapshot_containers
        quest_functions = {n: f for n, f in quest_functions.items() if f['address'].lower() not in library}
        quest_state = state_map(unit['quest']['fields'])
        helpers = {f['address'].lower(): n for n, f in quest_functions.items()
                   if n not in ('Main', 'Init', 'OnPersist')}
        report = {'schema': 'quest-unit-converter/1', 'script': unit['script'], 'package': package,
                  'packages': [], 'functions': [], 'missing': [], 'syntax': {},
                  'controlMode': 'flat experimental' if self.flat_control else 'structured draft'}
        if snapshot_containers:
            report.setdefault('runtimeBoundaries', []).append({'method': 'GlobalConversations',
                'members': unit['quest'].get('definitionSnapshots', {}),
                'omittedUnreachableContainerBodies': sorted(snapshot_containers)})
        if arena_containers:
            report['runtimeBoundaries'] = report.get('runtimeBoundaries', []) + [{'method': 'InitialiseArenaRounds',
                'nativeCaller': '0x00f25840', 'replacedCall': '0x00f25980',
                'omittedUnreachableContainerBodies': sorted(arena_containers),
                'implementation': 'tools/script_recovery/runtime_bindings/NoviArenaRounds.h'}]
        all_sources, shared_names, shared_inputs = {}, set(), {}
        shared_module = f'{package}.native_quest_helpers'
        owners = [('quest', unit['script'], quest_functions, quest_state, {})]
        timers = {unit['script']: unit['quest'].get('timers', [])}
        def lifecycle(fns):
            return {int(f['address'], 16) for n, f in fns.items() if n == 'destructor' and f.get('address')}
        ctor_constants = {unit['script']: self.constructor_defaults(unit.get('vtable'), unit['quest']['fields'],
                                                                   lifecycle(unit['quest']['functions']))}
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
            timers[class_owner[name]] = ent.get('timers', [])
            ctor_constants[class_owner[name]] = self.constructor_defaults(ent.get('vtable'), ent['fields'],
                                                                          lifecycle(ent['functions']))
        for kind, owner, functions, state, parent_state in owners:
            entity = kind == 'entity'
            lifter = Lifter(self.manifest, state, 'quest', entity, package, self.rdata,
                            thing_sigs=thing_signatures(self.things), parent_state=parent_state,
                            state_receiver='__native_entity_state' if entity else 'quest',
                            live_termination=True, native_gotos=True, readable_locals=True,
                            flat_control=self.flat_control)
            local_names = {f['address'].lower(): n for n, f in functions.items()
                           if re.fullmatch(r'[A-Za-z_]\w*', n) and n not in ('Main', 'Init', 'OnPersist', 'OnPredicateFail')}
            lifter.accessor_kinds = True
            lifter.out_thing_msgs = self.out_thing_msgs
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
                    if int(address, 16) in self.hidden_thing_returns:
                        lifter.helper_return_kinds[helper] = 'thing'
                    if int(address, 16) in self.hidden_string_returns:
                        lifter.hidden_string_helpers.add(helper)
            relative = f'FSE/{package}/Entities/{owner}.lua' if entity else f'FSE/{package}/{package}.lua'
            chunks = [f'-- Generated native draft: {owner}. Review coverage report before use.',
                      '-- Registration remains disabled until the package is verified.', '']
            if entity:
                chunks.append(ENTITY_STATE)
            # member string vectors Init fills from literals and nothing else writes: immutable file-level tables
            owner_row = (next((e for n, e in unit['entities'].items() if class_owner[n] == owner), None)
                         if entity else unit['quest'])
            decompiles = {n: unwrap_statements(self.native(f['address'])['decompile'])
                          for n, f in functions.items() if self.native(f['address'])}
            literal_vectors = native_literal_string_vectors.recover(
                (owner_row or {}).get('unmappedFields', []), decompiles, self.rdata.wide_string_at)
            if literal_vectors:
                chunks.append(native_literal_string_vectors.prelude(
                    literal_vectors, (owner_row or {}).get('nativeClass', owner).split('::')[-1]))
                report.setdefault('literalStringVectors', {})[relative] = {
                    hex(o): {'name': v[0], 'count': len(v[1])} for o, v in literal_vectors.items()}
            for name, spec in functions.items():
                row = {'owner': owner, 'function': name, 'address': spec['address'], 'path': relative,
                       'evidence': spec.get('evidence')}
                fn = self.native(spec['address'])
                if not fn:
                    report['missing'].append(row)
                    continue
                lifter.callee_names = known_callee_aliases(fn)
                lifter.parent_helpers = {}
                # a call whose target is one of this owner's helpers takes the helper's local name whatever
                # bsim called it (`CQ_CinemaTestScript::EndMission` = this quest's helper_D66EE0)
                by_target = {}
                for call in fn.get('calls', []):
                    address = str(call.get('target', '')).lower()
                    if call.get('currentName') and address in local_names:
                        # (and Ghidra's `__` spelling of the namespaced label in C output: DarkwoodTrader Init printed
                        # `CCreatureAction_TrollWhackGroundBase__HandleTrader(this,2)` for its SetBrainState helper, which
                        # stayed a TODO, so the trader's state, health bar and follow setup never ran, 2026-09-24)
                        for label in (call['currentName'], call['currentName'].removeprefix('NScript::'), call['currentName'].split('::')[-1],
                                      call['currentName'].replace('::', '__')):
                            by_target.setdefault(label, set()).add(local_names[address])
                lifter.callee_names.update({label: next(iter(names)) for label, names in by_target.items()
                                            if len(names) == 1 and label not in lifter.callee_names})
                if entity:
                    # Entity code calling quest helpers goes through the shared module.
                    candidates = {}
                    for call in fn.get('calls', []):
                        address = str(call.get('target', '')).lower()
                        if call.get('currentName') and address in helpers:
                            for label in (call['currentName'], call['currentName'].split('::')[-1]):
                                candidates.setdefault(label, set()).add(address)
                    # a label bsim put on two quest helpers (`IsHeroWearingBeard` on 0x00E3E320 and its tash twin, V_Bordello
                    # Magicman Main) is printed per site as `<label>__at<target>` by disambiguate_call_labels
                    for label, addresses in list(candidates.items()):
                        if len(addresses) > 1:
                            for address in addresses:
                                candidates.setdefault(f'{label}__at{int(address, 16):x}', set()).add(address)
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
                spec_l.resolve_wide = self.rdata.wide_string_at
                spec_l.float_at = self.float_at
                spec_l.byte_at = lambda va: (self.rdata.bytes_at(va, 1) or bytes([255]))[0]
                spec_l.call_labels = {c['currentName']: int(c['target'], 16) for c in fn.get('calls', []) if c.get('currentName')}
                decompile, renamed = disambiguate_call_labels(restore_stack_operands(self.name_vector_copies(self.name_append_literals(self.restore_local_helper_operands(self.recover_dropped_operands(respell_code_pointer_calls(fold_stack_vector_builds(fold_split_dword_stores(fold_vector_component_copies(native_literal_string_vectors.apply(rename_stack_parameters(unwrap_statements(fn['decompile'])), name, literal_vectors)))), fn), fn), fn), fn), fn), fn), fn.get('calls', []), fn)
                decompile = self.repair_literal_receiver_labels(decompile, renamed, fn)
                spec_l.call_labels.update(renamed)
                # a label two local helpers share (bsim: `RunSaveXPCutscene2` on both 0xD496F0 and 0xD49A20 in
                # Q_GuildTraining's Main) is ambiguous above; once disambiguated per site (`__at<addr>`) each
                # spelling names exactly one converted function
                lifter.callee_names.update({label: local_names[f'0x{target:08x}'] for label, target in renamed.items()
                                            if f'0x{target:08x}' in local_names and label not in lifter.callee_names})
                # Ghidra prints some namespaced labels with `__` in C output, and mangled names
                # (`Ns::?Fn@Cls@@UBE?AV...@@XZ`) with every non-identifier character as `_`
                spec_l.call_labels.update({k.replace('::', '__'): v for k, v in list(spec_l.call_labels.items()) if '::' in k})
                spec_l.call_labels.update({re.sub(r'[^\w:]', '_', k): v for k, v in list(spec_l.call_labels.items()) if re.search(r'[^\w:]', k)})   # also `operator_char_const*` -> `operator_char_const_`
                spec_l.hidden_thing_returns = self.hidden_thing_returns
                spec_l.code_range = self.code_range
                spec_l.native_address = int(fn['address'], 16)
                spec_l.calls = fn.get('calls', [])
                spec_l.indirect_calls = fn.get('indirectCalls', [])
                if signature.get('bsimVoid'):
                    decompile = re.sub(r'\breturn [^;]+;', 'return;', decompile)   # void per ego_r: Ghidra's int result is a stale register
                lowered, lowering_diag = lower(rename_parameters(decompile, signature), spec_l)
                source = lower_after_annotate(strip_receiver_arguments(annotate(lowered, self.slots, self.things, self.returning, entity=entity)), self.things)
                if os.environ.get('CONVERT_DUMP') and name in os.environ['CONVERT_DUMP'].split(','):
                    print(f'===== LOWERED {owner}.{name}', source, sep='\n', file=sys.stderr)
                if name == 'OnPersist':
                    body, calls, todo = lift_persist_evidence(source, unit, spec_l, self.persist_kinds)
                    if not body:
                        body, _, calls = lift_persist(source, 'quest')
                        todo = []
                    # the host calls an ENTITY's OnPersist(quest, me, ctx) (LuaEntityHost::OnPersist), a quest's
                    # OnPersist(quest, ctx): with `quest, context` an entity's `context` was the `me` handle and
                    # Transfer<int> ran on a CScriptThing (ApprenticeSpeedTest RaceMode: crash on every save, 2026-09-19)
                    params = 'quest, me, context' if entity else 'quest, context'
                else:
                    parameter_kinds = {p['lua']: 'number' if p['type'] in NUMBER_TYPES
                                       else 'bool' if p['type'] == 'bool'
                                       else 'resource' if 'CScriptGameResourceObjectScriptedThingBase' in p['type']
                                       else 'thing' if 'CScriptThing' in p['type']
                                       else 'string' if 'CCharString' in p['type'] else 'unknown'
                                       for p in signature['parameters']}
                    body = lifter.lift(name, source, native_function=fn, parameters=parameter_kinds)
                    if name == 'Init' and timers.get(owner):
                        # The native class constructor registers every CTimer member through GSI RegisterTimer
                        # (slot 0x15c) before Init runs; the converter has no constructor body, so Init does it.
                        receiver = '__native_entity_state' if entity else 'quest'
                        body = [f'    {receiver}:SetStateInt("{t}", quest:RegisterTimer())  -- native constructor: CTimer member'
                                for t in timers[owner]] + body
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
            # The constructor's constant scalar members (emulated from the retail ctor): Init stands in for the
            # constructor, as for the CTimer registrations. A field OnPersist transfers is left alone -- the
            # save restores it, and resetting it here could overwrite a loaded value.
            defaults = ctor_constants.get(owner) or {}
            init_at = next((i for i, c in enumerate(chunks) if c.startswith('function Init(')), None)
            if defaults and init_at is not None:
                persisted = set()
                for c in chunks:
                    if c.startswith('function OnPersist('):
                        persisted |= set(re.findall(r'Persist\w*\(context, "(\w+)"', c))
                receiver = '__native_entity_state' if entity else 'quest'
                lines = []
                for field, (kind, value) in sorted(defaults.items()):
                    if field in persisted or field in timers.get(owner, []):
                        continue
                    lua = ('true' if value else 'false') if kind == 'Bool' else repr(value) if kind == 'Float' else str(value)
                    lines.append(f'    {receiver}:Set{"State" + kind}("{field}", {lua})  -- native constructor: initial value')
                if lines:
                    head, _, rest = chunks[init_at].partition('\n')
                    chunks[init_at] = head + '\n' + '\n'.join(lines) + '\n' + rest
                    report.setdefault('constructorDefaults', {})[owner] = len(lines)
            if any('__native_all_dead(' in c for c in chunks):
                chunks.insert(3, ALL_DEAD_HELPER)
            source, hoisted = hoist_cleanup_regions('\n'.join(chunks))
            if hoisted:
                report.setdefault('cleanupRegions', {})[relative] = hoisted
            # a slot the decompiler only ever read is missing from the function's `local` line and would
            # resolve to a global at runtime: nil on read, but a write escapes into the shared Lua state
            source, declared = declare_free_locals(source)
            if declared:
                report.setdefault('declaredFreeLocals', {})[relative] = declared
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
                if os.environ.get('CONVERT_DUMP') and name in os.environ['CONVERT_DUMP'].split(','):
                    print(f'===== LOWERED (shared) {name}', source, sep='\n', file=sys.stderr)
                shared_lifter = Lifter(self.manifest, quest_state, 'quest', False, package, self.rdata,
                                       callee_names=aliases, thing_sigs=thing_signatures(self.things),
                                       live_termination=True, execution_entity=True, native_gotos=True,
                                       readable_locals=True, flat_control=self.flat_control)
                shared_lifter.accessor_kinds = True
                shared_lifter.out_thing_msgs = self.out_thing_msgs
                shared_lifter.helper_names = set(helpers.values())
                for address, helper in helpers.items():
                    helper_fn = self.native(address)
                    if not helper_fn:
                        continue
                    sig = function_parameters(helper_fn['decompile'], member=True)
                    shared_lifter.helper_parameters[helper] = [p['lua'] for p in sig['parameters']]
                    if sig['returnKind']:
                        shared_lifter.helper_return_kinds[helper] = sig['returnKind']
                    if int(address, 16) in self.hidden_thing_returns:
                        shared_lifter.helper_return_kinds[helper] = 'thing'
                    if int(address, 16) in self.hidden_string_returns:
                        shared_lifter.hidden_string_helpers.add(helper)
                kinds = {p['lua']: 'number' if p['type'] in NUMBER_TYPES else 'bool' if p['type'] == 'bool'
                         else 'resource' if 'CScriptGameResourceObjectScriptedThingBase' in p['type'] else 'thing' if 'CScriptThing' in p['type']
                         else 'string' if 'CCharString' in p['type'] else 'unknown' for p in signature['parameters']}
                body = shared_lifter.lift(name, source, native_function=fn, parameters=kinds)
                params = 'quest, me' + ''.join(', ' + p['lua'] for p in signature['parameters'])
                shared_sources[name] = finish_lua('\n'.join([f'function {name}({params})'] + body + ['end', '']))
                if 'resources:' in shared_sources[name]:      # the same retail-resource handle the quest copy gets
                    shared_sources[name] = shared_sources[name].replace('\n', '\n    local resources = quest:RetailResources()\n', 1)
                shared_diagnostics[name] = list(shared_lifter.todo)
            names = sorted(shared_sources)
            shared = '\n'.join(['-- Generated from the same native helper bodies as the quest draft.',
                                'local ' + ', '.join(names)] + [shared_sources[n] for n in names] +
                               ['return {' + ', '.join(f'{n} = {n}' for n in names) + '}\n'])
            shared, shared_hoisted = hoist_cleanup_regions(shared)
            if shared_hoisted:
                report.setdefault('cleanupRegions', {})[shared_path] = shared_hoisted
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
