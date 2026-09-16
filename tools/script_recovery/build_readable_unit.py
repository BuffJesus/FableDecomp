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
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker  # noqa: E402
from tools.script_recovery.script_units import unit as script_unit  # noqa: E402

# converter-only spellings the role renamer should treat as generated temporaries
assert readable_lua.GENERATED.pattern.endswith(r')\Z')
readable_lua.GENERATED = re.compile(readable_lua.GENERATED.pattern[:-3] + r'|\w*_stk_[0-9a-f]+|local_[0-9a-f]+|__native_condition_\d+|native_arg_sequence_\d+)\Z')

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
    """A loop index stepped by 0xc and divided by 0xc at every use is an element index."""
    count = 0
    for var in sorted(set(re.findall(r'\(\s*(\w+)\s*\) / 0xc', chunk))):
        uses = re.findall(r'\b' + re.escape(var) + r'\b[^\n]*', chunk)
        forms = re.findall(r'(?:\(\s*' + re.escape(var) + r'\s*\) / 0xc|' + re.escape(var) + r' = ' + re.escape(var) + r' \+ 0xc|' + re.escape(var) + r' = 0|' + re.escape(var) + r'\b)', chunk)
        plain = [f for f in forms if f == var]
        if len(plain) > 2 * len([f for f in forms if '0xc' in f]) or not any(f.endswith('+ 0xc') for f in forms):
            continue
        others = re.sub(r'\(\s*' + re.escape(var) + r'\s*\) / 0xc|' + re.escape(var) + r' = ' + re.escape(var) + r' \+ 0xc|' + re.escape(var) + r' = 0\b|local [^\n]*\b' + re.escape(var) + r'\b', '', chunk)
        if re.search(r'\b' + re.escape(var) + r'\b', others):
            continue
        chunk = re.sub(r'\(\s*' + re.escape(var) + r'\s*\) / 0xc', var, chunk)
        chunk = re.sub(re.escape(var) + r' = ' + re.escape(var) + r' \+ 0xc', f'{var} = {var} + 1', chunk)
        count += 1
    return chunk, count


NIL_BEFORE_RETURN = re.compile(r'^(?P<ind>[ \t]*)\w+ = nil\n(?=(?P=ind)return\b)', re.M)
NIL_BEFORE_END = re.compile(r'^(?P<ind>[ \t]*)\w+ = nil\n(?=end\n)', re.M)


def prune_nil_releases(chunk):
    """`thing = nil` right before a return / function end is the native destructor's release."""
    chunk, a = NIL_BEFORE_RETURN.subn('', chunk)
    chunk, b = NIL_BEFORE_END.subn('', chunk)
    return chunk, a + b


EMPTY_IF = re.compile(r'^(?P<ind>[ \t]*)if [^\n]+ then\n(?P=ind)end\n', re.M)


def prune_empty_ifs(chunk):
    count = 0
    while (m := EMPTY_IF.search(chunk)):
        chunk = chunk[:m.start()] + chunk[m.end():]
        count += 1
    return chunk, count


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
    """`alive` is only a read-back of the scheduler predicate; when nothing reads it, its pure
    assignments go and `alive = quest:NewScriptFrame(...)` becomes the bare call."""
    reads = re.sub(r'^[ \t]*(?:local )?alive = [^\n]*\n', '', chunk, flags=re.M)
    if 'alive' not in chunk or re.search(r'\balive\b', reads):
        return chunk, 0
    count = 0
    chunk, k = re.subn(r'^[ \t]*(?:local )?alive = (?:true|not \w+:IsActiveThreadTerminating\(\))\n', '', chunk, flags=re.M); count += k
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


def tidy_declarations(chunk):
    """Drop names that no statement uses from declaration-only `local` lines."""
    count = 0
    body = re.sub(r'^[ \t]*local [\w, ]+\n', '', chunk, flags=re.M)
    used = set(re.findall(r'\b\w+\b', body))

    def repl(m):
        nonlocal count
        names = [n for n in m.group(2).split(', ') if n in used]
        count += len(m.group(2).split(', ')) - len(names)
        return f"{m.group(1)}local {', '.join(names)}\n" if names else ''
    chunk = re.sub(r'^([ \t]*)local ([\w, ]+)\n', repl, chunk, flags=re.M)
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


def readable_file(source):
    """Apply every pass per function chunk; return (text, report). The folds run before and after the
    local-name pass: splitting reused temporaries exposes more dead stores and single-use flags."""
    functions = {}
    text = fold_functions(source, functions)
    text, mappings = readable_source(text, split_reused=True, inline_literals=True)
    text = fold_functions(text, functions, literals=True)
    text, wrapped = wrap_local_declarations(text)
    return text, {'functions': [{'function': n, 'rewrites': st} for n, st in functions.items()],
                  'locals': mappings, 'wrappedDeclarations': len(wrapped)}


def build(unit_name, *, draft=None, out=None):
    u = script_unit(unit_name)
    lifted = ROOT / 'refs/script_recovery/lifted' / u['package']
    draft = draft or lifted / 'draft'
    out = out or lifted / 'readable'
    if out.exists():
        shutil.rmtree(out)
    checker = LuaSyntaxChecker()
    report = {'schema': 'readable-unit/1', 'unit': unit_name, 'draft': str(draft.relative_to(ROOT)), 'files': {}}
    sources = {}
    for path in sorted(draft.rglob('*.lua')):
        rel = path.relative_to(draft).as_posix()
        text, file_report = readable_file(path.read_text(encoding='utf-8'))
        text = text.replace('-- Generated native draft:', '-- Readable native conversion:', 1)
        target = out / rel
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(text, encoding='utf-8')
        sources[rel] = text
        report['files'][rel] = file_report
    report['syntax'] = checker.check(sources)
    (out / 'READABLE_REPORT.json').write_text(json.dumps(report, indent=1) + '\n', encoding='utf-8')
    summary = {'files': len(sources), 'syntaxOk': report['syntax']['ok'],
               'errors': [e['path'] for e in report['syntax'].get('errors', [])],
               'rewrites': sum(v for f in report['files'].values() for fn in f['functions'] for v in fn['rewrites'].values()),
               'renamedLocals': sum(len(m['locals']) for f in report['files'].values() for m in f['locals'])}
    print(json.dumps(summary, indent=2))
    return report


def main():
    a = argparse.ArgumentParser(description=__doc__)
    a.add_argument('--unit', default='orchard_farm')
    args = a.parse_args()
    build(args.unit)


if __name__ == '__main__':
    main()
