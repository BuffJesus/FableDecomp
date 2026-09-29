"""Recover string buffers and local string-map ownership from explicit native shapes."""
import re


def fold_string_buffers(text, byte_at):
    # The null representation points at the shared empty buffer; otherwise the
    # first word of the CCharString representation is its character data.
    pattern = re.compile(
        r'^(?P<ind>[ \t]*)if \((?P<src>\w+) == \(CCharString\)0x0\) \{\s*'
        r'(?P<dst>\w+) = \(int \*\)&DAT_(?P<empty>[0-9a-f]+);\s*\}\s*else \{\s*'
        r'(?P=dst) = \*\(int \*\*\)(?P=src);\s*\}[ \t]*\r?\n', re.M)
    def replace(m):
        if byte_at is None or byte_at(int(m['empty'], 16)) != 0:
            return m[0]
        return f"{m['ind']}{m['dst']} = ENGINE_StringValue({m['src']});\n"
    return pattern.sub(replace, text)


def fold_case_compare(text, labels):
    # Only equality-to-zero users: no claim about the CRT's particular nonzero
    # difference, ordering, or locale-sensitive comparisons of arbitrary bytes.
    for label, target in labels.items():
        if target != 0xBFEBA8:
            continue
        # Ghidra retains the import namespace in call metadata but emits its
        # ordinary CRT spelling after applying the reviewed prototype.
        heads = '(?:' + re.escape(label) + '|_stricmp)'
        pattern = re.compile(r'^(?P<ind>[ \t]*)(?P<result>\w+) = ' + heads
                             + r'\((?P<a>\w+),(?P<b>\w+)\);', re.M)
        def replace(m):
            tail = text[m.end():]
            uses = list(re.finditer(r'\b' + re.escape(m['result']) + r'\b', tail))
            tests = list(re.finditer(r'\b' + re.escape(m['result']) + r' (?:==|!=) 0(?=\s*\))', tail))
            if not uses or len(uses) != len(tests):
                return m[0]
            return f"{m['ind']}{m['result']} = ENGINE_StrCaseCmp({m['a']}, {m['b']});"
        text = pattern.sub(replace, text)
    return text


def fold_string_maps(text, labels):
    # Infer the value type from the proven string-map operator[], never malloc
    # size alone. Both full construction and destruction must be present.
    for label, target in labels.items():
        if target != 0x9AC700:
            continue
        call = re.escape(label).replace('::', r'\s*::\s*')
        maps = set(re.findall(call + r'\((?:\([^()]*\*\))?&(?P<map>\w+),', text))
        for name in maps:
            n = re.escape(name)
            ctor = re.compile(
                r'^(?P<ind>[ \t]*)' + n + r' = \(undefined1 \*\)0x0;\s*'
                + n + r' = malloc\(0x18\);\s*(?P<size>\w+) = 0;\s*'
                r'\*' + n + r' = 0;\s*\*\(undefined4 \*\)\(' + n + r' \+ 4\) = 0;\s*'
                r'\*\(undefined1 \*\*\)\(' + n + r' \+ 8\) = ' + n + r';\s*'
                r'\*\(undefined1 \*\*\)\(' + n + r' \+ 0xc\) = ' + n + r';[ \t]*\r?\n', re.M)
            cm = ctor.search(text)
            if not cm:
                continue
            size = re.escape(cm['size'])
            destroys = [k for k, v in labels.items() if v == 0x9AA830]
            for destroy in destroys:
                dtor = re.compile(
                    r'^[ \t]*if \(' + size + r' != 0\) \{\s*'
                    + re.escape(destroy).replace('::', r'\s*::\s*')
                    + r'\((?:\([^()]*\*\))?&' + n + r',\*\(int \*\)\(' + n + r' \+ 4\)\);\s*'
                    r'\*\(undefined1 \*\*\)\(' + n + r' \+ 8\) = ' + n + r';\s*'
                    r'\*\(undefined4 \*\)\(' + n + r' \+ 4\) = 0;\s*'
                    r'\*\(undefined1 \*\*\)\(' + n + r' \+ 0xc\) = ' + n + r';\s*'
                    + size + r' = 0;\s*\}\s*if \(' + n + r' != \(undefined1 \*\)0x0\) \{\s*'
                    r'free\(' + n + r'\);\s*\}', re.M)
                dm = dtor.search(text, cm.end())
                if dm:
                    text = text[:dm.start()] + cm['ind'] + f'STRINGMAP_Destroy({name});' + text[dm.end():]
                    text = text[:cm.start()] + cm['ind'] + f'{name} = STRINGMAP_New();\n' + text[cm.end():]
                    break
    # A by-reference assignment operand saved immediately before operator[].
    # Keep this local to the store, so register reuse cannot change another use.
    for label, target in labels.items():
        if target != 0x9AC700:
            continue
        call = re.escape(label).replace('::', r'\s*::\s*')
        pattern = re.compile(
            r'^[ \t]*(?P<alias>\w+) = &(?P<value>\w+);\s*'
            r'(?P<store>[ \t]*(?P<node>\w+) = ' + call + r'\([^;]+\);\s*'
            r'CCharString::operator=\(\(CCharString \*\)(?P=node),)(?P=alias)(?P<end>\);)', re.M)
        def alias_value(m):
            # &DAT is a char literal pointer, not a CCharString object. Its
            # existing literal lowering must keep the address until resolution.
            if m['value'].startswith('DAT_'):
                return m[0]
            return (f"  {m['alias']} = ENGINE_StringValue({m['value']});\n"
                    + '  ' + m['store'] + m['alias'] + m['end'])
        text = pattern.sub(alias_value, text)
    return text


def apply(text, spec):
    labels = getattr(spec, 'call_labels', None) or {}
    text = fold_string_buffers(text, getattr(spec, 'byte_at', None))
    text = fold_case_compare(text, labels)
    return fold_string_maps(text, labels)
