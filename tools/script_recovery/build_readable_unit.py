"""Readable pass for a converter-generated quest unit (generic, no per-function hooks).

Input: `refs/script_recovery/lifted/<Unit>/draft` (output of `convert_quest_unit.py --unit`).
Output: `refs/script_recovery/lifted/<Unit>/readable` with the same file layout plus
`READABLE_REPORT.json` (per-function local-name maps and rewrite counts, all reversible or
evidence-preserving) and a syntax check of every file.

The rewrites are presentation only; native diagnostics (`-- TODO(native)`) stay in place:

  * termination idiom  `alive = not q:IsActiveThreadTerminating(); bVarN = not alive; if bVarN then return end`
    -> `if quest:IsActiveThreadTerminating() then return end` (only when bVarN has no other use)
  * `return extraout_EAX` (void natives) -> `return`
  * state-list arithmetic left by the lowering: `(count * 0xc) / 0xc` -> count,
    `n = count * 0xc; s = n >> 0x1f; if n / 0xc + s ~= s` -> `if count ~= 0`
  * generated temporaries renamed by role (`readable_lua.readable_source`, reused temporaries split,
    unused literal staging removed, single-use literals inlined)
  * small hex literals shown in decimal
"""
from __future__ import annotations

import argparse
import json
import re
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery import readable_lua  # noqa: E402
from tools.script_recovery.readable_lua import readable_source, wrap_local_declarations  # noqa: E402
from tools.script_recovery.readable_style import style_source, state_writers  # noqa: E402
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker  # noqa: E402
from tools.script_recovery.audit_readability import inspect_source  # noqa: E402
from tools.script_recovery.script_units import unit as script_unit  # noqa: E402

# converter-only spellings the role renamer should treat as generated temporaries. `scratchValue\d*` is the
# renamer's own fallback: it is listed so a second pass over the *styled* text can name what the style folds
# left behind (a slot with five disagreeing assignments in the draft often has one survivor afterwards).
assert readable_lua.GENERATED.pattern.endswith(r')\Z')
CONVERTER_GENERATED = re.compile(readable_lua.GENERATED.pattern[:-3] + r'|\w*_stk_[0-9a-f]+|local_[0-9a-f]+|__native_condition_\d+|native_arg_sequence_\d+|scratchValue\d*)\Z')

TERMINATION = re.compile(
    r'^(?P<ind>[ \t]*)alive = not (?P<recv>\w+):IsActiveThreadTerminating\(\)\n'
    r'(?P=ind)(?P<flag>\w+) = not alive\n'
    r'(?P=ind)if (?P=flag) then(?P<inline> [^\n]*? end|\n(?P=ind)    (?P<body>[^\n]*)\n(?P=ind)end)\n', re.M)
FUNCTION_SPLIT = re.compile(r'(?m)^(?=function |local function )')


def _uses(source, name):
    """Occurrences outside declaration-only `local a, b` lines."""
    body = re.sub(r'^[ \t]*local [\w, ]+\n', '', source, flags=re.M)
    return len(re.findall(r'\b' + re.escape(name) + r'\b', body))


def fold_termination(chunk):
    """Collapse the scheduler-termination idiom when the flag variable is otherwise unused."""
    count = 0
    while True:
        m = TERMINATION.search(chunk)
        if not m:
            break
        flag = m.group('flag')
        if _uses(chunk, flag) != 2:      # the assignment and the test
            # keep the flag; still shorten the query
            chunk = chunk[:m.start()] + (f"{m.group('ind')}{flag} = {m.group('recv')}:IsActiveThreadTerminating()\n"
                                         f"{m.group('ind')}if {flag} then{m.group('inline')}\n") + chunk[m.end():]
            count += 1
            continue
        inline = m.group('inline')
        if inline.startswith('\n'):
            inline = ' ' + m.group('body') + ' end'
        chunk = chunk[:m.start()] + f"{m.group('ind')}if {m.group('recv')}:IsActiveThreadTerminating() then{inline}\n" + chunk[m.end():]
        count += 1
    return chunk, count


LIST_COUNT = r'(\w+:GetStateListCount\("[^"]+"\))'
COUNT_TEST = re.compile(
    r'^(?P<ind>[ \t]*)(?:local )?(?P<n>\w+) = \(' + LIST_COUNT + r' \* 0xc\)\n'
    r'(?P=ind)(?P<s>\w+) = (?P=n) >> 0x1f\n'
    r'(?P<between>(?:(?P=ind)[^\n]*\n)*?)'
    r'(?P=ind)if (?P=n) / 0xc \+ (?P=s) ~= (?P=s) then\n', re.M)


def fold_list_arithmetic(chunk):
    count = 0
    chunk, k = re.subn(r'\(\(' + LIST_COUNT + r' \* 0xc\)\) / 0xc', r'\1', chunk); count += k
    chunk, k = re.subn(r'\(' + LIST_COUNT + r' \* 0xc\) / 0xc', r'\1', chunk); count += k
    while (m := COUNT_TEST.search(chunk)):
        n, s = m.group('n'), m.group('s')
        head, tail = chunk[:m.start()], chunk[m.end():]
        def redefined_first(name, text):
            first = re.search(r'\b' + re.escape(name) + r'\b', text)
            return first is None or re.match(r'^[ \t]*(?:local )?' + re.escape(name) + r' = ', text[text.rfind('\n', 0, first.start()) + 1:])
        if not redefined_first(s, tail) or not redefined_first(n, tail) or _uses(m.group('between'), n) or _uses(m.group('between'), s):
            break
        keep = f"{m.group('ind')}local {n}\n" if m.group(0).lstrip().startswith('local ') else ''
        chunk = head + keep + m.group('between') + f"{m.group('ind')}if {m.group(3)} ~= 0 then\n" + tail
        count += 1
    return chunk, count


RETURN_TERMINATION = re.compile(
    r'^(?P<ind>[ \t]*)alive = not (?P<recv>\w+):IsActiveThreadTerminating\(\)\n'
    r'(?P=ind)(?P<flag>\w+) = not alive\n(?P=ind)return not (?P=flag)\n', re.M)
CONST_FALSE_IF = re.compile(r'^(?P<ind>[ \t]*)(?P<flag>\w+) = false\n(?P=ind)if (?P=flag) then\n(?:(?P=ind)    [^\n]*\n)*(?P=ind)end\n', re.M)
CMP_AS_INT = re.compile(r'^(?P<ind>[ \t]*)(?P<v>\w+) = \(\((?P<cond>[^\n]+?)\) and 0 or 1\)\n(?P=ind)if (?P=v) (?P<op>==|~=) 0 then', re.M)
EXTRAOUT = re.compile(r'^[ \t]*\w+ = extraout_\w+\n', re.M)


def fold_flags(chunk):
    """Boolean temporaries the decompiler materialised: `return not flag`, constant-false guards,
    `((cond) and 0 or 1) == 0`."""
    count = 0
    chunk, k = EXTRAOUT.subn('', chunk); count += k          # undefined decompiler outputs (nil)
    while (m := RETURN_TERMINATION.search(chunk)):
        if _uses(chunk, m.group('flag')) != 2:
            break
        chunk = chunk[:m.start()] + f"{m.group('ind')}return not {m.group('recv')}:IsActiveThreadTerminating()\n" + chunk[m.end():]
        count += 1
    while (m := CONST_FALSE_IF.search(chunk)):
        if _uses(chunk, m.group('flag')) != 2:
            break
        chunk = chunk[:m.start()] + chunk[m.end():]
        count += 1
    while (m := CMP_AS_INT.search(chunk)):
        if _uses(chunk, m.group('v')) != 2:
            break
        cond = m.group('cond') if m.group('op') == '==' else f"not ({m.group('cond')})"
        chunk = chunk[:m.start()] + f"{m.group('ind')}if {cond} then" + chunk[m.end():]
        count += 1
    return chunk, count


def fold_byte_indices(chunk):
    """A loop index stepped by 0xc, divided by 0xc at element uses and compared against `count * 0xc`
    is an element index."""
    count = 0
    for var in sorted(set(re.findall(r'\(\s*(\w+)\s*\) / 0xc', chunk))):
        v = re.escape(var)
        index_forms = [r'\(\s*' + v + r'\s*\) / 0xc', v + r' = ' + v + r' \+ 0xc', v + r' (?:==|~=|<|>=) \(' + LIST_COUNT + r' \* 0xc\)']
        if not re.search(v + r' = ' + v + r' \+ 0xc', chunk):
            continue
        others = chunk
        for f in index_forms + [v + r' = 0\b', r'local [^\n]*\b' + v + r'\b']:
            others = re.sub(f, '', others)
        if re.search(r'\b' + v + r'\b', others):
            continue
        chunk = re.sub(index_forms[0], var, chunk)
        chunk = re.sub(index_forms[1], f'{var} = {var} + 1', chunk)
        chunk = re.sub(v + r' (==|~=|<|>=) \(' + LIST_COUNT + r' \* 0xc\)', var + r' \1 \2', chunk)
        count += 1
    return chunk, count


def prune_nil_releases(chunk):
    """Preserve nil writes: proximity to an exit does not prove a dead release.

    Return expressions, escaped closures, globals and close handlers can observe
    the assignment. Keep this report slot for compatibility; later passes must
    establish their own dead-store proof rather than infer it from an exit.
    """
    return chunk, 0


EMPTY_IF = re.compile(
    r'^(?P<ind>[ \t]*)if (?P<condition>true|false|'
    r'\((?:0x[0-9a-fA-F]+|\d+) & (?:0x[0-9a-fA-F]+|\d+)\) (?:~=|==) 0) then\n'
    r'(?P=ind)end\n', re.M)


def prune_empty_ifs(chunk):
    """Drop only closed literal predicates with no lookup/call side effects."""
    code=''.join(re.sub(r'[^\n]', ' ', t[0])
        if t.lastgroup in ('string','longstring','comment','longcomment') else t[0]
        for t in readable_lua.tokens(chunk))
    matches = []
    for match in EMPTY_IF.finditer(code):
        if chunk[match.start():match.end()] != match[0]:
            continue
        operands = re.findall(r'0x[0-9a-fA-F]+|\d+', match['condition'])
        if any(int(value, 16 if value.startswith('0x') else 10) > 0xffffffff for value in operands):
            continue
        matches.append(match)
    for match in reversed(matches):
        chunk=chunk[:match.start()]+chunk[match.end():]
    return chunk,len(matches)


def prune_dead_stores(chunk):
    """Generated temporaries that are only ever assigned literals / nil and never read."""
    count = 0
    for var in sorted(set(re.findall(r'^[ \t]*(?:local )?(\w*_stk_\w+|local_[0-9a-f]+|[A-Za-z]{1,3}Var\d+(?:_\d+)?|scratchValue\d*|predicateResult\d*) = (?:nil|-?\d+(?:\.\d+)?|0x[0-9a-f]+|true|false|\w+:GetState\w+\([^\n]*\))\n', chunk, re.M))):
        reads = re.sub(r'^[ \t]*(?:local )?' + re.escape(var) + r' = (?:nil|-?\d+(?:\.\d+)?|0x[0-9a-f]+|true|false|\w+:GetState\w+\([^\n]*\))\n', '', chunk, flags=re.M)
        reads = re.sub(r'^[ \t]*local [^\n=]*\n', lambda m: re.sub(r'\b' + re.escape(var) + r'\b', '', m.group(0)), reads, flags=re.M)
        if re.search(r'\b' + re.escape(var) + r'\b', reads):
            continue
        chunk = re.sub(r'^[ \t]*(?:local )?' + re.escape(var) + r' = (?:nil|-?\d+(?:\.\d+)?|0x[0-9a-f]+|true|false|\w+:GetState\w+\([^\n]*\))\n', '', chunk, flags=re.M)
        chunk = re.sub(r'^([ \t]*local (?:\w+, )*)' + re.escape(var) + r'(?:, |\n)', lambda m: m.group(1) if m.group(0).endswith(', ') else m.group(1).rstrip(', ') + '\n', chunk, flags=re.M)
        chunk = re.sub(r'^[ \t]*local\n', '', chunk, flags=re.M)
        count += 1
    return chunk, count


def fold_returns(chunk):
    chunk, k = re.subn(r'\breturn extraout_\w+\b', 'return', chunk)
    return chunk, k


def decimal_literals(chunk):
    count = 0

    def repl(m):
        nonlocal count
        value = int(m.group(0), 16)
        if value > 4096:
            return m.group(0)
        count += 1
        return str(value)
    # outside strings/comments only
    out, buffer = [], ''
    for t in readable_lua.tokens(chunk):
        if t.lastgroup in ('string', 'longstring', 'comment', 'longcomment'):
            out.append(re.sub(r'\b0x[0-9a-fA-F]+\b', repl, buffer)); buffer = ''
            out.append(t[0])
        else:
            buffer += t[0]
    out.append(re.sub(r'\b0x[0-9a-fA-F]+\b', repl, buffer))
    return ''.join(out), count


def drop_unused_alive(chunk):
    """Drop unused flags while preserving scheduler queries and frame calls.

    A discarded result does not authorize discarding the call: termination
    queries must still occur at their native resource-lifetime boundary.
    """
    reads = re.sub(r'^[ \t]*(?:local )?alive = [^\n]*\n', '', chunk, flags=re.M)
    if 'alive' not in chunk or re.search(r'\balive\b', reads):
        return chunk, 0
    count = 0
    chunk, k = re.subn(r'^[ \t]*(?:local )?alive = true\n', '', chunk, flags=re.M); count += k
    chunk, k = re.subn(r'^([ \t]*)(?:local )?alive = not (\w+:IsActiveThreadTerminating\(\))\n', r'\1\2\n', chunk, flags=re.M); count += k
    chunk, k = re.subn(r'^([ \t]*)alive = (\w+:NewScriptFrame\([^\n]*\))\n', r'\1\2\n', chunk, flags=re.M); count += k
    return chunk, count


FLAG_TEST = re.compile(r'^(?P<ind>[ \t]*)(?P<flag>\w+) = (?P<call>\w+:IsActiveThreadTerminating\(\))\n(?P=ind)if (?P=flag) then', re.M)


def fold_flag_tests(chunk):
    count = 0
    pos = 0
    while (m := FLAG_TEST.search(chunk, pos)):
        if _uses(chunk, m.group('flag')) != 2:
            pos = m.end()
            continue
        chunk = chunk[:m.start()] + f"{m.group('ind')}if {m.group('call')} then" + chunk[m.end():]
        count += 1
    return chunk, count


DECLARATION_ONLY = re.compile(
    r'^([ \t]*)local ([A-Za-z_]\w*(?:[ \t]*,[ \t]*[A-Za-z_]\w*)*)[ \t]*\n', re.M)


def tidy_declarations(chunk):
    """Prune unused names only from complete, plain local declarations."""
    count = 0
    body = DECLARATION_ONLY.sub('', chunk)
    used = set(re.findall(r'\b\w+\b', body))
    used.add('_ENV')  # global lookups refer to this local implicitly
    code = ''.join(re.sub(r'[^\n]', ' ', t[0])
                   if t.lastgroup in ('string', 'longstring', 'comment', 'longcomment') else t[0]
                   for t in readable_lua.tokens(chunk))
    for match in reversed(list(DECLARATION_ONLY.finditer(code))):
        # A newline can continue the name list or introduce its initializer.
        # Comments/literals remain opaque, including trailing annotations.
        if code[match.end():].lstrip().startswith((',', '=')) or chunk[match.start():match.end()] != match[0]:
            continue
        declared = [name.strip() for name in match[2].split(',')]
        names = [name for name in declared if name in used]
        count += len(declared) - len(names)
        replacement = f"{match[1]}local {', '.join(names)}\n" if names else ''
        chunk = chunk[:match.start()] + replacement + chunk[match.end():]
    return chunk, count


PASSES = [('flags', fold_flags), ('termination', fold_termination), ('listArithmetic', fold_list_arithmetic),
          ('byteIndices', fold_byte_indices), ('voidReturns', fold_returns), ('nilReleases', prune_nil_releases), ('emptyIfs', prune_empty_ifs), ('deadStores', prune_dead_stores),
          ('unusedAlive', drop_unused_alive), ('flagTests', fold_flag_tests), ('declarations', tidy_declarations)]


def fold_functions(source, functions, *, literals=False):
    parts = FUNCTION_SPLIT.split(source)
    out = []
    for part in parts:
        if not part.startswith(('function ', 'local function ')):
            out.append(part)
            continue
        name = re.match(r'(?:local )?function (\w+)', part).group(1)
        stats = functions.setdefault(name, {})
        for key, fn in PASSES:
            part, k = fn(part)
            stats[key] = stats.get(key, 0) + k
        if literals:
            part, k = decimal_literals(part)
            stats['decimalLiterals'] = stats.get('decimalLiterals', 0) + k
        out.append(part)
    return ''.join(out)


def readable_file(source, *, style=True, frame_returns_alive=True, rel=None, writers=None, integer_registrations=(), integer_manifest_bytes=None):
    """Apply every pass per function chunk; return (text, report). The folds run before and after the
    local-name pass: splitting reused temporaries exposes more dead stores and single-use flags; the
    quest-script style pass (readable_style) runs last on the renamed text."""
    # These line-oriented passes only model emitted single-line text tokens.
    # Preserve unsupported multiline text verbatim instead of treating its
    # contents as local declarations, statements or formatting whitespace.
    if any(t.lastgroup in ('string', 'longstring', 'longcomment') and '\n' in t[0]
           for t in readable_lua.tokens(source)):
        return source, {'functions': [], 'locals': [], 'wrappedDeclarations': 0,
                        'style': {}, 'preservedMultilineText': True}
    declaration_code = ''.join(re.sub(r'[^\n]', ' ', t[0])
        if t.lastgroup in ('string', 'longstring', 'comment', 'longcomment') else t[0]
        for t in readable_lua.tokens(source))
    if re.search(r'\b_ENV\b', declaration_code):
        # Global lookup dependencies are implicit in Lua. The line-oriented
        # flow passes do not model changes to the lexical environment.
        return source, {'functions': [], 'locals': [], 'wrappedDeclarations': 0,
                        'style': {}, 'preservedEnvironment': True}
    if re.search(r'^[ \t]*local [^=\n]*?(?:,[ \t\r]*\n|\n[ \t\r\n]*[,=])', declaration_code, re.M):
        # The subsequent style passes model one declaration per physical line.
        # Keep unsupported continued declarations intact, just like multiline text.
        return source, {'functions': [], 'locals': [], 'wrappedDeclarations': 0,
                        'style': {}, 'preservedMultilineDeclaration': True}
    from tools.script_recovery.lua_binding_guard import ambiguous_bindings
    ambiguous = ambiguous_bindings(source)
    if ambiguous:
        return source, {'functions': [], 'locals': [], 'wrappedDeclarations': 0,
                        'style': {}, 'preservedAmbiguousBindings': sorted(ambiguous)}
    from tools.script_recovery.readable_callback_scopes import rewrite as rewrite_callbacks
    source, callback_reports = rewrite_callbacks(source, readable_file, style=style,
        frame_returns_alive=frame_returns_alive, rel=rel, writers=writers)
    functions = {}
    text = fold_functions(source, functions)
    text, mappings = readable_source(text, split_reused=True, inline_literals=True, generated=CONVERTER_GENERATED)
    text = fold_functions(text, functions, literals=True)
    # Function order survives helper renaming in both style rounds. Protect
    # only accepted callback wrappers, preserving the parent's frame contract
    # for all unrelated functions in the same file.
    frame_keep_check_indices = tuple(i for i, name in enumerate(
        re.findall(r'(?m)^(?:local )?function (\w+)', text)) if name in callback_reports)
    style_report = {}
    if style:
        # two rounds: the style folds expose more dead stores / nil releases / unused declarations to the
        # older passes, and those in turn expose loop / guard shapes to the style folds
        text, style_report = style_source(text, frame_returns_alive=frame_returns_alive, rel=rel, writers=writers, frame_keep_check_indices=frame_keep_check_indices)
        text = fold_functions(text, functions)
        text, second = style_source(text, frame_returns_alive=frame_returns_alive, rel=rel, writers=writers, frame_keep_check_indices=frame_keep_check_indices)
        text = fold_functions(text, functions)
        for name, entry in second['functions'].items():
            first = style_report['functions'].setdefault(name, {'rewrites': {}, 'before': entry['before'], 'after': entry['after']})
            for key, value in entry['rewrites'].items():
                first['rewrites'][key] = first['rewrites'].get(key, 0) + value
            first['after'] = entry['after']
            if entry.get('nativeDispatchSeeds'):
                first.setdefault('nativeDispatchSeeds', []).extend(entry['nativeDispatchSeeds'])
        # the style folds dropped assignments, so a temporary the first pass could only call `scratchValue`
        # (its assignments disagreed) may now have one role left: name it from what survived
        text, renamed = readable_source(text, split_reused=False, generated=CONVERTER_GENERATED)
        mappings = mappings + renamed
    text, wrapped = wrap_local_declarations(text)
    # A folded native producer can leave a local that is now nil for its
    # entire lifetime. Prune only bindings with complete token/lexical proof;
    # populated aliases retain their release stores. Run after final naming.
    from tools.script_recovery.readable_nil_only_locals import prune as prune_nil_only
    nil_reviews = {}
    parts = FUNCTION_SPLIT.split(text)
    for i, part in enumerate(parts):
        if not part.startswith(('function ', 'local function ')):
            continue
        name = re.match(r'(?:local )?function (\w+)', part).group(1)
        parts[i], reviews = prune_nil_only(part)
        if reviews:
            nil_reviews[name] = reviews
            stats = functions.setdefault(name, {})
            stats['declarations'] = stats.get('declarations', 0) + len(reviews)
    integer_reviews = {}
    if style and integer_registrations:
        from tools.script_recovery.readable_integer_zero_stores import prune as prune_integer
        for i, part in enumerate(parts):
            if not part.startswith(('function ', 'local function ')):
                continue
            name = re.match(r'(?:local )?function (\w+)', part).group(1)
            for registration in integer_registrations:
                for review in registration.get('reviewedReadableIntegerZeroStores', {}).values():
                    if review.get('file') != rel or review.get('function') != name:
                        continue
                    parts[i], proof = prune_integer(parts[i], review=review,
                        registration=registration, manifest_bytes=integer_manifest_bytes,
                        rel=rel, phase='after-final-naming-wrap-and-nil-only-pruning')
                    integer_reviews[name] = proof
    text = ''.join(parts)
    report = {'callbackScopes': callback_reports, 'functions': [{'function': n, 'rewrites': st} for n, st in functions.items()],
                  'locals': mappings, 'wrappedDeclarations': len(wrapped), 'style': style_report}
    if nil_reviews:
        report['removedNilOnlyLocals'] = nil_reviews
    if integer_reviews:
        report['reviewedIntegerZeroStores'] = integer_reviews
    return text, report


def bsim_helper_names(sources, conversion, unit):
    """`helper_XXXXXX` (no PDB name) takes the method name bsim gave the retail body when that name is a plain
    identifier, unique across the unit and free in every file (`CQ_CinemaTestScript::EndMission` -> `EndMission`,
    kept as a comment on the definition). Calls are renamed across files (`helpers.EndMission`)."""
    tu_path = unit['evidence'] / 'translation_unit_typed.json'
    if not tu_path.exists():
        return sources, {}
    tu = json.loads(tu_path.read_text(encoding='utf-8'))
    by_address = {f['address'].lower(): f.get('currentName', '') for f in (tu['functions'] if isinstance(tu, dict) else tu)}
    rows = [row for u in conversion.get('units', []) for row in u.get('functions', []) if row['function'].startswith('helper_')]
    chosen, renamed = {}, {}
    for row in rows:
        label = by_address.get(row['address'].lower(), '')
        tail = label.split('::')[-1]
        if not re.fullmatch(r'[A-Z]\w+', tail) or tail.startswith(('FUN_', 'thunk')) or tail == label \
                or (tail, ) in {(t,) for t, _ in chosen.values()}:
            continue
        # the name must be free in every file that mentions the helper (Lua names are per file)
        users = [text for text in sources.values() if re.search(r'\b' + row['function'] + r'\b', text)]
        if any(re.search(r'(?:\bfunction |[.:]|\b)' + tail + r'\(', text) for text in users):
            continue
        chosen[row['function']] = (tail, label)
    for old, (new, label) in chosen.items():
        for rel in sources:
            text = sources[rel]
            if not re.search(r'\b' + old + r'\b', text):
                continue
            text = re.sub(r'\b' + old + r'\b', new, text)
            text = re.sub(r'^-- helper 0x[0-9A-F]+ \(named after the state it writes\)\n(?=function ' + new + r'\()', '', text, flags=re.M)
            text = text.replace(f'function {new}(', f'-- {old[7:]}: bsim names this body {label} (a homologous script member); no PDB name\nfunction {new}(', 1) \
                if f'function {new}(' in text else text
            sources[rel] = text
        renamed[old] = new
    return sources, renamed


def function_headers(text, rel, conversion):
    """One comment line per function: the native owner and retail address from the draft report."""
    rows = {}
    for unit in conversion.get('units', []):
        for row in unit.get('functions', []):
            if row.get('path') == rel and row.get('address'):
                rows[row['function']] = row
    def repl(m):
        row = rows.get(m.group(1))
        if not row:
            return m.group(0)
        return f"-- {row['owner']}.{row['function']} (retail {row['address']})\n{m.group(0)}"
    return re.sub(r'^function (\w+)\(', repl, text, flags=re.M)


def integer_registrations(unit):
    registrations = []
    for path in sorted((unit['evidence'] / 'units').glob(unit['ir_glob'])):
        registration = json.loads(path.read_text(encoding='utf-8'))
        if registration.get('reviewedReadableIntegerZeroStores'):
            registrations.append(registration)
    return registrations


def build(unit_name, *, draft=None, out=None, style=True, frame_returns_alive=True):
    u = script_unit(unit_name)
    lifted = ROOT / 'refs/script_recovery/lifted' / u['package']
    draft = draft or lifted / 'draft'
    out = out or lifted / 'readable'
    if out.exists() and not (out / 'READABLE_REPORT.json').exists() and any(out.iterdir()):
        raise SystemExit(f'{out} is not a generated readable stage (no READABLE_REPORT.json) — a hand-reviewed artifact? '
                         f'pass --out to write elsewhere')
    if out.exists():
        shutil.rmtree(out)
    checker = LuaSyntaxChecker()
    report = {'schema': 'readable-unit/1', 'unit': unit_name, 'draft': str(draft.relative_to(ROOT)), 'files': {}}
    sources = {}
    drafts = {p.relative_to(draft).as_posix(): p.read_text(encoding='utf-8') for p in sorted(draft.rglob('*.lua'))}
    conversion = json.loads((draft / 'CONVERSION_REPORT.json').read_text(encoding='utf-8')) if (draft / 'CONVERSION_REPORT.json').exists() else {}
    writers = state_writers(drafts)
    integer_reviews = integer_registrations(u) if style else []
    integer_manifest = (ROOT / 'refs/fse_api_manifest.json').read_bytes() if integer_reviews else None
    for path in sorted(draft.rglob('*.lua')):
        rel = path.relative_to(draft).as_posix()
        try:
            text, file_report = readable_file(drafts[rel], style=style, frame_returns_alive=frame_returns_alive, rel=rel, writers=writers, integer_registrations=integer_reviews, integer_manifest_bytes=integer_manifest)
        except ValueError as exc:            # a pass refused (irreversible rename, ...): ship the draft text for this file
            text, file_report = drafts[rel], {'functions': [], 'locals': [], 'wrappedDeclarations': 0, 'style': {}, 'error': str(exc)}
            print(f'{rel}: readable pass skipped: {exc}', file=sys.stderr)
        text = text.replace('-- Generated native draft:', '-- Readable native conversion:', 1)
        if style:
            text = function_headers(text, rel, conversion)
        sources[rel] = text
        report['files'][rel] = file_report
    if style:
        sources, renamed = bsim_helper_names(sources, conversion, u)
        report['bsimHelperNames'] = renamed
    for rel, text in sources.items():
        if style:
            # Keep layout after phase-sensitive passes and final helper naming.
            # Every Lua token, including comments and semicolons, stays exact.
            from tools.script_recovery.readable_compound_layout import expand
            text, compound_layout = expand(text)
            sources[rel] = text
            if compound_layout:
                report['files'][rel]['expandedCompoundBranches'] = compound_layout
        if style:
            from tools.script_recovery.readable_initial_literals import prune
            parts = FUNCTION_SPLIT.split(text)
            reviews = {}
            for i, part in enumerate(parts):
                if not part.startswith(('function ', 'local function ')):
                    continue
                name = re.match(r'(?:local )?function (\w+)', part).group(1)
                parts[i], proof = prune(part)
                if proof:
                    reviews[name] = proof
            text = ''.join(parts)
            sources[rel] = text
            if reviews:
                report['files'][rel]['removedInitialLiteralStores'] = reviews
        if style:
            from tools.script_recovery.readable_retry_loop import restructure
            parts = FUNCTION_SPLIT.split(text)
            retry_reviews = {}
            for i, part in enumerate(parts):
                if not part.startswith(('function ', 'local function ')):
                    continue
                name = re.match(r'(?:local )?function (\w+)', part).group(1)
                parts[i], proof = restructure(part)
                if proof:
                    retry_reviews[name] = proof
            text = ''.join(parts)
            sources[rel] = text
            if retry_reviews:
                report['files'][rel]['structuredRetryLoops'] = retry_reviews
        if style:
            from tools.script_recovery.readable_literal_query_chain import fold
            parts = FUNCTION_SPLIT.split(text)
            query_reviews = {}
            for i, part in enumerate(parts):
                if not part.startswith(('function ', 'local function ')):
                    continue
                name = re.match(r'(?:local )?function (\w+)', part).group(1)
                parts[i], proof = fold(part)
                if proof:
                    query_reviews[name] = proof
            text = ''.join(parts)
            sources[rel] = text
            if query_reviews:
                report['files'][rel]['structuredLiteralQueries'] = query_reviews
        if style:
            from tools.script_recovery.readable_boolean_query_chain import fold
            parts = FUNCTION_SPLIT.split(text)
            boolean_reviews = {}
            for i, part in enumerate(parts):
                if not part.startswith(('function ', 'local function ')):
                    continue
                name = re.match(r'(?:local )?function (\w+)', part).group(1)
                parts[i], proof = fold(part)
                if proof:
                    boolean_reviews[name] = proof
            text = ''.join(parts)
            sources[rel] = text
            if boolean_reviews:
                report['files'][rel]['structuredBooleanQueries'] = boolean_reviews
        if style:
            from tools.script_recovery.readable_unreachable_goto_tail import prune
            parts = FUNCTION_SPLIT.split(text)
            tail_reviews = {}
            for i, part in enumerate(parts):
                if not part.startswith(('function ', 'local function ')):
                    continue
                name = re.match(r'(?:local )?function (\w+)', part).group(1)
                parts[i], proof = prune(part)
                if proof:
                    tail_reviews[name] = proof
            text = ''.join(parts)
            sources[rel] = text
            if tail_reviews:
                report['files'][rel]['removedUnreachableGotoTail'] = tail_reviews
        if style:
            from tools.script_recovery.readable_guard_region import fold
            parts = FUNCTION_SPLIT.split(text)
            region_reviews = {}
            for i, part in enumerate(parts):
                if not part.startswith(('function ', 'local function ')):
                    continue
                name = re.match(r'(?:local )?function (\w+)', part).group(1)
                parts[i], proof = fold(part)
                if proof:
                    region_reviews[name] = proof
            text = ''.join(parts)
            sources[rel] = text
            if region_reviews:
                report['files'][rel]['structuredGuardRegions'] = region_reviews
        if style:
            from tools.script_recovery.readable_tail_else import fold
            parts = FUNCTION_SPLIT.split(text)
            tail_else_reviews = {}
            for i, part in enumerate(parts):
                if not part.startswith(('function ', 'local function ')):
                    continue
                name = re.match(r'(?:local )?function (\w+)', part).group(1)
                parts[i], proof = fold(part)
                if proof:
                    tail_else_reviews[name] = proof
            text = ''.join(parts)
            sources[rel] = text
            if tail_else_reviews:
                report['files'][rel]['structuredTailElseRegions'] = tail_else_reviews
        if style:
            from tools.script_recovery.readable_empty_then import fold
            parts = FUNCTION_SPLIT.split(text)
            empty_then_reviews = {}
            for i, part in enumerate(parts):
                if not part.startswith(('function ', 'local function ')):
                    continue
                name = re.match(r'(?:local )?function (\w+)', part).group(1)
                parts[i], proof = fold(part)
                if proof:
                    empty_then_reviews[name] = proof
            text = ''.join(parts)
            sources[rel] = text
            if empty_then_reviews:
                report['files'][rel]['normalizedEmptyThenGuards'] = empty_then_reviews
        if style:
            from tools.script_recovery.readable_scope_exit import fold
            parts = FUNCTION_SPLIT.split(text)
            scope_exit_reviews = {}
            for i, part in enumerate(parts):
                if not part.startswith(('function ', 'local function ')):
                    continue
                name = re.match(r'(?:local )?function (\w+)', part).group(1)
                parts[i], proof = fold(part, allow_intervening_labels=True)
                if proof:
                    scope_exit_reviews[name] = proof
            text = ''.join(parts)
            sources[rel] = text
            if scope_exit_reviews:
                report['files'][rel]['removedTransparentScopeExitJumps'] = scope_exit_reviews
        if style:
            from tools.script_recovery.readable_style import prune_unused_generated_labels
            text, late_unused_labels = prune_unused_generated_labels(text)
            sources[rel] = text
            if late_unused_labels:
                report['files'][rel]['removedLateUnusedGeneratedLabels'] = late_unused_labels
        if style:
            from tools.script_recovery.readable_label_alias import fold
            parts = FUNCTION_SPLIT.split(text)
            alias_reviews = {}
            for i, part in enumerate(parts):
                if not part.startswith(('function ', 'local function ')):
                    continue
                name = re.match(r'(?:local )?function (\w+)', part).group(1)
                parts[i], proof = fold(part)
                if proof:
                    alias_reviews[name] = proof
            text = ''.join(parts)
            if alias_reviews:
                report['files'][rel]['coalescedGeneratedLabelAliases'] = alias_reviews
                text, alias_unused_labels = prune_unused_generated_labels(text)
                if alias_unused_labels:
                    report['files'][rel]['removedAliasedUnusedGeneratedLabels'] = alias_unused_labels
            sources[rel] = text
        target = out / rel
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(text, encoding='utf-8')
        report['files'][rel]['readability'] = inspect_source(text)
    report['syntax'] = checker.check(sources)
    (out / 'READABLE_REPORT.json').write_text(json.dumps(report, indent=1) + '\n', encoding='utf-8')
    metrics = {}
    for f in report['files'].values():
        for fn in f.get('style', {}).get('functions', {}).values():
            for stage in ('before', 'after'):
                for key, value in fn[stage].items():
                    metrics.setdefault(stage, {})[key] = metrics.setdefault(stage, {}).get(key, 0) + value
    report['styleMetrics'] = metrics
    # a file whose passes raised shipped as the raw draft: that is a silent loss of the whole readable stage
    # for it, so it belongs in the summary, not only in files[rel].error
    summary = {'files': len(sources), 'syntaxOk': report['syntax']['ok'],
               'readabilityIssues': {rel: f['readability']['counts'] for rel, f in report['files'].items()
                                     if f['readability']['findings']},
               'shippedAsDraft': {rel: f['error'] for rel, f in report['files'].items() if f.get('error')},
               'errors': [e['path'] for e in report['syntax'].get('errors', [])],
               'rewrites': sum(v for f in report['files'].values() for fn in f['functions'] for v in fn['rewrites'].values()),
               'renamedLocals': sum(len(m['locals']) for f in report['files'].values() for m in f['locals']),
               'styleMetrics': metrics}
    print(json.dumps(summary, indent=2))
    return report


def main():
    a = argparse.ArgumentParser(description=__doc__)
    a.add_argument('--unit', default='orchard_farm')
    a.add_argument('--out', type=Path, help='output directory (default refs/script_recovery/lifted/<Package>/readable)')
    a.add_argument('--no-style', action='store_true', help='skip the quest-script style pass (readable_style.py)')
    a.add_argument('--frame-keeps-checks', action='store_true',
                   help='do not fold NewScriptFrame + termination check (units run under the NewOakValeIntro lifetime, '
                        'where the DLL always returns true from NewScriptFrame)')
    args = a.parse_args()
    build(args.unit, out=args.out, style=not args.no_style, frame_returns_alive=not args.frame_keeps_checks)


if __name__ == '__main__':
    main()
