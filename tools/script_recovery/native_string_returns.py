"""Recover a proven CCharString value return obscured by the native output pointer.

The reviewed prototype, output construction, caller stack operands and callee RET
must agree. Keep the hidden output in calls until stack identity has been restored;
the lifter then gives both that slot and the returned pointer the same Lua value.
"""
import re
from tools.script_recovery.native_arguments import split_arguments


def _literal_input(source, constructor, call_start, ctor_sites):
    args = constructor[2]
    if len(args) != 3 or args[2] != '-1' or not re.fullmatch(r'"(?:[^"\\]|\\.)*"', args[1]):
        return None
    slot = re.search(r'(?:\w*Stack_\w+|stack0x[0-9a-f]+)', args[0])
    if not slot:
        return None
    begin = constructor[1] + 1
    between = source[begin:call_start]
    if re.search(r'[{}]|\b(?:if|else|goto|while|for|switch|return)\b', between):
        return None
    if re.search(r'\b' + re.escape(slot[0]) + r'\b', between):
        return None  # modified, aliased or otherwise observed before consumption
    # The only intervening calls permitted construct separate local string values.
    for other in sorted(ctor_sites, key=lambda s: s[0], reverse=True):
        if begin <= other[0] < call_start:
            if other[3].get('ecxStack') == constructor[3].get('ecxStack'):
                return None
            head = re.search(r'CCharString\s*::\s*CCharString\s*\($', source[begin:other[0]])
            if not head:
                return None
            between = between[:head.start()] + between[other[1] + 1 - begin:]
    # Ignore the final helper call head/assignment, but allow only simple local
    # assignments before it. Branches, memory writes and other calls invalidate it.
    complete = between.rsplit(';', 1)[0] if ';' in between else ''
    for statement in complete.split(';'):
        statement = statement.strip()
        if not statement:
            continue
        if not re.fullmatch(r'\w+\s*=\s*[^;{}]+', statement):
            return None
        if re.search(r'\b\w+\s*\(', statement) or re.search(r'\b(?:goto|return)\b', statement):
            return None
    return args[1]


def _target_sites(source, caller, calls):
    """Use the exported token order for this target even if unrelated calls cannot pair."""
    order = caller.get('callOrder') or []
    if not calls:
        return []
    by_site = {c['site'].lower(): c for c in calls}
    selected = [by_site[s.lower()] for s in order if s.lower() in by_site]
    if len(selected) != len(calls):
        return []
    names = {c.get('currentName', '') for c in calls}
    names |= {n.replace('::', '__') for n in names}
    pattern = re.compile(r'(?<![\w:])(?:' + '|'.join(re.escape(n).replace('::', r'\s*::\s*')
                         for n in names if n) + r')\s*\(')
    matches = list(pattern.finditer(source))
    if len(matches) != len(calls):
        return []
    result = []
    for match, site in zip(matches, selected):
        end, depth = match.end(), 1
        while end < len(source) and depth:
            depth += {'(': 1, ')': -1}.get(source[end], 0)
            end += 1
        if depth:
            return []
        result.append((match.end(), end - 1, split_arguments(source[match.end():end-1]),
                       site, site['currentName'], False))
    return result


def recover_string_returns(functions, stack_words, ordered_sites):
    recovered = set()
    for address, fn in functions.items():
        source = fn.get('decompile') or ''
        if re.search(r'\b(?:hiddenStringResult|dialogueSuffix)\b', source):
            continue
        comment = re.search(r'/\*\s*\[bsim[^\]]*\]([\s\S]*?)\*/', source)
        if not comment or not re.search(
                r'class\s+CCharString\s+__thiscall\s+[\w:]+\(class\s+CCharString\s+const\s*&\)', comment[1]):
            continue
        header = re.search(r'CCharString\s*\*\s*__thiscall\s+[\w:]+\('
                           r'(?P<receiver>\w+\s*\*this),CCharString\s*\*(?P<out>\w+)\)', source)
        if not header or stack_words(int(address, 16)) != 2:
            continue
        output = header['out']
        labels = {c.get('currentName') for c in fn.get('calls', [])
                  if int(c.get('target', '0'), 16) == 0x99F570}
        labels |= {n.replace('::', '__') for n in labels if n}
        constructors = [re.search(re.escape(label) + r'\(\(CCharString_bv \*\)'
                        + re.escape(output) + r',\w+,\(CCharString_bv \*\)in_stack_00000008\);', source)
                        for label in labels if label]
        if not any(constructors) or not re.search(r'\breturn ' + re.escape(output) + r';', source):
            continue
        if len(re.findall(r'\b' + re.escape(output) + r'\b', source[header.end():])) != 2:
            continue  # pointer escapes, multiple writes or a different return path
        if len(re.findall(r'\bin_stack_00000008\b', source)) != 2:
            continue
        if not re.search(r'CCharString_bv\s+in_stack_00000008;', source):
            continue
        plans = []
        valid = True
        for caller in functions.values():
            calls = [c for c in caller.get('calls', []) if int(c.get('target', '0'), 16) == int(address, 16)]
            if not calls:
                continue
            sites = ordered_sites(caller.get('decompile') or '', caller)
            matches = [s for s in sites or [] if not s[5] and s[3] in calls]
            if sites is None:
                matches = _target_sites(caller.get('decompile') or '', caller, calls)
            if len(matches) != len(calls):
                valid = False
                break
            ctors = [c for c in caller.get('calls', []) if int(c.get('target', '0'), 16) == 0x99EBF0]
            ctor_sites = _target_sites(caller.get('decompile') or '', caller, ctors)
            edits = []
            for start, end, args, site, _, _ in matches:
                pushes = site.get('pushedStack', [])
                if len(args) != 2 or args[0] != 'this' or len(pushes) < 2 or not all(
                        isinstance(v, int) and v < 0 for v in pushes[-2:]):
                    valid = False
                    break
                candidates = [s for s in ctor_sites if s[3].get('ecxStack') == pushes[-2]
                              and int(s[3]['site'], 16) < int(site['site'], 16) and s[0] < start]
                if not candidates:
                    valid = False
                    break
                constructor = max(candidates, key=lambda s: int(s[3]['site'], 16))
                literal = _literal_input(caller['decompile'], constructor, start, ctor_sites)
                if literal is None:
                    valid = False
                    break
                # The actual pushed slot matches this unmodified string constructor.
                # Pass its value directly; generic slot pooling loses these lifetimes.
                edits.append((start, end, ','.join(args + [literal]), site))
            plans.append((caller, edits))
        if not valid or not plans:
            continue
        for caller, edits in plans:
            text = caller['decompile']
            for start, end, replacement, site in sorted(edits, key=lambda e: e[0], reverse=True):
                text = text[:start] + replacement + text[end:]
                site['pushedStack'] = site['pushedStack'][-2:]
                if site.get('pushedValue'):
                    site['pushedValue'] = site['pushedValue'][-2:]
                site.pop('edxStack', None)
                site.pop('edxValue', None)
            caller['decompile'] = text
        body = source[header.end():]
        body = re.sub(r'CCharString_bv\s+in_stack_00000008;', 'CCharString hiddenStringResult;', body)
        body = re.sub(r'\b' + re.escape(output) + r'\b', 'hiddenStringResult', body)
        body = re.sub(r'\bin_stack_00000008\b', 'dialogueSuffix', body)
        new_header = re.sub(r'^CCharString\s*\*', 'CCharString ', header[0])
        new_header = re.sub(r'CCharString\s*\*' + re.escape(output) + r'\)',
                            'CCharString *dialogueSuffix)', new_header)
        fn['decompile'] = source[:header.start()] + new_header + body
        recovered.add(int(address, 16))
    return recovered


def recover_void_string_returns(functions, stack_words):
    """A `class CCharString __thiscall X(void)` member (ego_r) whose only native operand is the hidden output:
    every use of the output is the copy-construction of the result (`CCharString((CCharString *)out, src)`) or
    `return out;`, and the callee purges exactly that one word (`ret 4`). The output becomes a local result and
    the function returns it; callers keep the pushed slot, which the lifter binds to the helper's result.
    (V_BeardyBaldy CAC_Owner::GetRandomSpeech 0x00E53CB0.)"""
    recovered = set()
    for address, fn in functions.items():
        source = fn.get('decompile') or ''
        if re.search(r'\bhiddenStringResult\b', source):
            continue
        comment = re.search(r'/\*\s*\[bsim[^\]]*\]([\s\S]*?)\*/', source)
        if not comment or not re.search(r'class\s+CCharString\s+__thiscall\s+[\w:]+\(void\)', comment[1]):
            continue
        header = re.search(r'(?:int|CCharString\s*\*|undefined4)\s*__thiscall\s+[\w:]+\((?P<receiver>\w+\s*\*this),'
                           r'\s*(?:int|undefined4|CCharString\s*\*)\s*(?P<out>\w+)\)', source)
        if not header or stack_words(int(address, 16)) != 1:
            continue
        out, body = header['out'], source[header.end():]
        uses = len(re.findall(r'\b' + re.escape(out) + r'\b', body))
        ctors = len(re.findall(r'CCharString::CCharString\(\(CCharString \*\)' + re.escape(out) + r',', body))
        returns = len(re.findall(r'\breturn ' + re.escape(out) + r';', body))
        if not ctors or not returns or uses != ctors + returns or re.search(r'\breturn\b(?! ' + re.escape(out) + r';)', body):
            continue
        body = body.replace('{', '{\n  CCharString hiddenStringResult;', 1)
        body = re.sub(r'CCharString::CCharString\(\(CCharString \*\)' + re.escape(out) + r',',
                      'CCharString::CCharString(&hiddenStringResult,', body)
        body = re.sub(r'\breturn ' + re.escape(out) + r';', 'return hiddenStringResult;', body)
        # the copy source is a pointer temp set to a local on every path into the shared exit (`other = &xStack_8;
        # goto LAB_..`): copy at each assignment instead, so each path's value reaches the result
        source_ptr = re.findall(r'CCharString::CCharString\(&hiddenStringResult,(\w+)\);', body)
        if len(set(source_ptr)) == 1 and re.search(r'CCharString(?:_bv)? \*' + source_ptr[0] + r';', body):
            ptr = source_ptr[0]
            assign = r'\b' + ptr + r' = (?:\(CCharString(?:_bv)? \*\))?(&?\w+);'
            sets = re.findall(assign, body)
            if sets and len(re.findall(r'\b' + ptr + r'\b', body)) == len(sets) + len(source_ptr) + 1:
                body = re.sub(assign, lambda m: f'hiddenStringResult = {m[1].lstrip("&")};', body)
                body = re.sub(r'[ \t]*CCharString::CCharString\(&hiddenStringResult,' + ptr + r'\);[ \t]*\r?\n', '', body)
                body = re.sub(r'[ \t]*CCharString(?:_bv)? \*' + ptr + r';[ \t]*\r?\n', '', body)
        new_header = re.sub(r'^\w+\s*\*?\s*__thiscall', 'CCharString __thiscall', header[0])
        new_header = new_header[:new_header.index('(') + 1] + header['receiver'] + ')'
        fn['decompile'] = source[:header.start()] + new_header + body
        recovered.add(int(address, 16))
    return recovered
