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
    thing_signatures, lift_persist,
)
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker  # noqa: E402
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters  # noqa: E402
from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower, finish_lua, strip_receiver_arguments, lower_after_annotate, _split_top  # noqa: E402
from tools.script_recovery.annotate_interface_slots import load_thing_slots  # noqa: E402
from tools.script_recovery.native_cleanup_regions import hoist_cleanup_regions  # noqa: E402

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


SIDECAR_PATCHES = ROOT / 'tools' / 'script_recovery' / 'sidecar_patches'
RE_SIDECAR_BINDING = re.compile(r'^\+\s*(quest|thing)\["(\w+)"\]\s*=\s*\[\]\(([^)]*)\)\s*(?:->\s*([^{]+?))?\s*\{', re.M)


def sidecar_bindings():
    """Manifest entries for the bindings the NoviCompatibility sidecar adds on top of stock ForgeFSE
    (evidence: the added `quest["X"]` / `thing["X"]` lines of tools/script_recovery/sidecar_patches/*.patch)."""
    entries = {}
    for patch in sorted(SIDECAR_PATCHES.glob('*.patch')) if SIDECAR_PATCHES.is_dir() else []:
        for scope, name, params, ret in RE_SIDECAR_BINDING.findall(patch.read_text(encoding='utf-8', errors='replace')):
            parameters = []
            for param in [x.strip() for x in params.split(',') if x.strip()]:
                if 'LuaQuestState' in param or 'sol::this_state' in param:
                    continue
                kind, _, pname = param.rpartition(' ')
                kind = kind.replace('const ', '').replace('&', '').strip()
                if kind == 'CScriptThing*' and pname == 'me':
                    parameters.append({'name': 'pMe', 'type': 'CScriptThing*', 'optional': False})
                else:
                    parameters.append({'name': pname or f'arg{len(parameters)}', 'type': 'CScriptThing*' if kind == 'sol::object' else kind, 'optional': False})
            # lambdas without a trailing return type: reviewed against the patch bodies
            ret = (ret or {'GetStateThing': 'CScriptThing*', 'GetStateListCount': 'int'}.get(name, 'void')).strip()
            ret = {'std::string': 'const std::string&', 'std::shared_ptr<CScriptThing>': 'CScriptThing*'}.get(ret, ret)
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


def disambiguate_call_labels(decompile, calls):
    """bsim propagates one label over byte-similar bodies, so a function may call e.g. the scripted-thing
    resource ctor (0x7E72A0) and the movie ctor (0x6E7B60) under the same printed name. The lowering keys
    on {label: target}, so the last site would win for all. Rename the k-th printed occurrence to
    `<label>__at<target>` following the site order (the decompiler prints straight-line calls in address
    order); when the printed count differs from the site count the text is left alone."""
    by_label = {}
    for c in calls:
        if c.get('currentName'):
            by_label.setdefault(c['currentName'], []).append(int(c['target'], 16))
    renamed = {}
    for label, targets in by_label.items():
        if len(set(targets)) < 2 or label in LABEL_TEXT_RULES:
            continue
        pattern = re.compile(r'(?<![\w:])' + re.escape(label) + r'(?=\s*\()')
        if len(pattern.findall(decompile)) != len(targets):
            continue
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


def _align(site, args):
    '''Entry-relative slots aligned with the printed arguments of one call, or None when the printed count
    cannot be reconciled with the recorded register/push operands (a by-value slot, a hidden return pointer
    push the decompiler folded away, ...). Printed order is (ecx, edx, pushes right-to-left).'''
    pushed = list(reversed(site.get('pushedStack', [])))
    values = list(reversed(site.get('pushedValue') or [None] * len(pushed)))
    ecx, edx = site.get('ecxStack'), site.get('edxStack')
    lead = len(args) - len(pushed)          # printed register arguments (this / __fastcall ecx, edx)
    if lead < 0 or lead > 2 or (ecx is not None and lead < 1) or (edx is not None and lead < 2):
        return None                         # a stack-loaded register that is not printed: an unprinted push is hiding
    # (address slot, value slot) per printed argument: `&X` / pointer casts name the object at the address,
    # a bare `X` names the slot whose value was loaded
    return list(zip([ecx, edx][:lead] + pushed, [site.get('ecxValue'), site.get('edxValue')][:lead] + values))


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
    r'|\(\*\(code \*\)\w+(?:\[\d+\])?\[(?P<index>\d+)\]\)\s*\('
    r'|\(\*\*\(code \*\*\)\*\w+\)\s*\(')


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
    labels = {c['currentName'] for c in fn.get('calls', []) if c.get('currentName')}
    for label in labels:
        spans = _call_spans(text, label)
        if not spans and re.search(r'[?@]', label):
            spans = _call_spans(text, re.sub(r'[^\w:]', '_', label))
        if not spans and re.match(r'\w+\.DLL::', label):
            spans = _call_spans(text, label.split('::', 1)[1]) + _call_spans(text, '::' + label.split('::', 1)[1])   # imports print bare (`operator_new(` / `::operator_new(`)
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
        if is_vtable != vtable:
            return None
        if vtable:
            if site.get('slot') is not None and hex(int(site['slot'], 16)) != key:
                return None
        elif site.get('currentName') != key:
            return None
        if vtable or 'pushedStack' in site or 'ecxStack' in site or 'edxStack' in site:
            out.append((a, e, _split_top(text[a:e]), site, key, vtable))
    return out


def restore_stack_operands(decompile, fn):
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

    def collect(pos, end, args, slots, label, vtable):
        sites.append((pos, end, args, slots, label))
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
                ctor = not by_value and ((not vtable and k == 0 and _is_ctor_label(label)) or (vtable and k >= 1 and bool(RE_THING_SLOT_CAST.match(arg.strip()))))
                # the argument's address is the object; the bare name only stands for it when no offset is added
                uses.append((pos, k, m.group('name') if not plus else None, off, plus, ctor))

    ordered = _text_order_sites(text, fn)
    if ordered is not None:
        # exact pairing through the decompiler's token stream (callOrder): printed calls in text order
        for pos, end, args, site, key, vtable in ordered:
            collect(pos, end, args, _align(site, args), key, vtable)
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
            collect(he, j - 1, args, _align(site, args), slot, True)
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

    # objects: constructed names per true slot, in order of first construction
    ctor_names = {}   # slot -> [(first pos, ghidra name or None for an offset spelling)]
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
        return object_name[(off, lst[0][1])] if lst else f'xStack_{-off:x}'

    by_name = {}
    for pos, k, name, off, plus, ctor in uses:
        if name:
            by_name.setdefault(name, []).append((pos, off))
    edits = []
    for start, end, args, slots, label in sites:
        out = list(args)
        for k, arg in enumerate(args):
            m = RE_STACK_OPERAND.match(arg.strip())
            if not m:
                continue
            name = m.group('name')
            plus = int(m.group('plus'), 0) if m.group('plus') else 0
            by_value = not m.group('amp') and not (m.group('cast') and '*' in m.group('cast')) and not m.group('plus')
            slot = ((slots[k][1] if slots[k][1] is not None else slots[k][0]) if by_value else slots[k][0]) if slots is not None and k < len(slots) else None
            addr = slot if slot is not None and slot < 0 else None
            if addr is None:
                near = sorted((abs(p - start), p, o) for p, o in by_name.get(name, []))
                if not near:
                    continue
                addr = near[0][2] + plus
            new = name_at(addr, name if not plus else None, start)
            out[k] = (m.group('cast') or '') + m.group('amp') + new
        if out != args:
            edits.append((start, end, ','.join(out)))
    for start, end, repl in sorted(edits, reverse=True):
        text = text[:start] + repl + text[end:]
    # remaining (non-call) spellings: a name with one true slot follows that object; a name Ghidra spread
    # over several slots follows the object of its nearest call-site use (field reads `N._4_4_`, `&N`,
    # inlined constructor stores all sit next to the call that produced or consumed the object)
    for name, lst in by_name.items():
        offs = {o for _, o in lst}
        if len(offs) == 1:
            off = next(iter(offs))
            text = re.sub(r'\b' + re.escape(name) + r'\b', name_at(off, name, min(p for p, _ in lst)), text)
        else:
            def nearest(m, lst=lst, name=name):
                pos, off = min(lst, key=lambda u: abs(u[0] - m.start()))
                return name_at(off, name, pos)
            text = re.sub(r'\b' + re.escape(name) + r'\b', nearest, text)
    return text


_MASK = re.compile(r'"(?:[^"\\\n]|\\.)*"|\'(?:[^\'\\\n]|\\.)*\'|/\*[\s\S]*?\*/|//[^\n]*')


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
            buf = head + (tail if tail[:1] in ',)' or (tail[:1] == '(' and head.endswith(')')) else ' ' + tail)
        masked = _MASK.sub(lambda m: ' ' * len(m[0]), buf)
        depth = masked.count('(') - masked.count(')')
        if depth > 0 and not masked.rstrip().endswith('{'):
            continue
        out.append(buf)
        buf = None
    if buf is not None:
        out.append(buf)
    return '\n'.join(out)


class UnitConverter:
    def float_at(self, va):
        raw = self.rdata.bytes_at(va, 4)
        if not raw or len(raw) != 4:
            return None
        value = struct.unpack('<f', raw)[0]
        return value if value == value and abs(value) < 1e12 else None   # NaN / absurd = not a float constant

    def __init__(self, tu_path, *, flat_control=False):
        self.manifest, self.slots, self.rdata = load_manifest(), load_slots(), RData()
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
        self.code_range = tuple(int(a, 16) for a in tu['range']) if tu.get('range') else None
        self.hidden_thing_returns = {int(f['address'], 16) for f in tu['functions']
                                     if re.search(r'\*in_stack_\w+ = &PTR_\w*_01238c8c;', f.get('decompile') or '')}
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
        timers = {unit['script']: unit['quest'].get('timers', [])}
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
                # a call whose target is one of this owner's helpers takes the helper's local name whatever
                # bsim called it (`CQ_CinemaTestScript::EndMission` = this quest's helper_D66EE0)
                by_target = {}
                for call in fn.get('calls', []):
                    address = str(call.get('target', '')).lower()
                    if call.get('currentName') and address in local_names:
                        for label in (call['currentName'], call['currentName'].removeprefix('NScript::'), call['currentName'].split('::')[-1]):
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
                decompile, renamed = disambiguate_call_labels(restore_stack_operands(unwrap_statements(fn['decompile']), fn), fn.get('calls', []))
                spec_l.call_labels.update(renamed)
                # Ghidra prints some namespaced labels with `__` in C output, and mangled names
                # (`Ns::?Fn@Cls@@UBE?AV...@@XZ`) with every non-identifier character as `_`
                spec_l.call_labels.update({k.replace('::', '__'): v for k, v in list(spec_l.call_labels.items()) if '::' in k})
                spec_l.call_labels.update({re.sub(r'[^\w:]', '_', k): v for k, v in list(spec_l.call_labels.items()) if re.search(r'[?@]', k)})
                spec_l.hidden_thing_returns = self.hidden_thing_returns
                spec_l.code_range = self.code_range
                if signature.get('bsimVoid'):
                    decompile = re.sub(r'\breturn [^;]+;', 'return;', decompile)   # void per ego_r: Ghidra's int result is a stale register
                lowered, lowering_diag = lower(rename_parameters(decompile, signature), spec_l)
                source = lower_after_annotate(strip_receiver_arguments(annotate(lowered, self.slots, self.things, self.returning, entity=entity)), self.things)
                if os.environ.get('CONVERT_DUMP') and name in os.environ['CONVERT_DUMP'].split(','):
                    print(f'===== LOWERED {owner}.{name}', source, sep='\n', file=sys.stderr)
                if name == 'OnPersist':
                    body, _, calls = lift_persist(source, 'quest')
                    todo, params = [], 'quest, context'
                else:
                    parameter_kinds = {p['lua']: 'number' if p['type'] in NUMBER_TYPES
                                       else 'bool' if p['type'] == 'bool'
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
            source, hoisted = hoist_cleanup_regions('\n'.join(chunks))
            if hoisted:
                report.setdefault('cleanupRegions', {})[relative] = hoisted
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
                shared_lifter.accessor_kinds = True
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
                kinds = {p['lua']: 'number' if p['type'] in NUMBER_TYPES else 'bool' if p['type'] == 'bool' else 'thing' if 'CScriptThing' in p['type']
                         else 'string' if 'CCharString' in p['type'] else 'unknown' for p in signature['parameters']}
                body = shared_lifter.lift(name, source, native_function=fn, parameters=kinds)
                params = 'quest, me' + ''.join(', ' + p['lua'] for p in signature['parameters'])
                shared_sources[name] = finish_lua('\n'.join([f'function {name}({params})'] + body + ['end', '']))
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
