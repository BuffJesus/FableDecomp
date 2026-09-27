"""Value lowering for local aliases of reviewed CCharString members."""
import re


def fold_string_member_aliases(text, member, getter, setter):
    """Keep writes through an unescaped member alias attached to script state.

    A string pointer is not a captured Lua string: each assignment must update
    both the member and its local value. Refuse other pointer uses or rebinding.
    """
    pattern = re.compile(r'^([ \t]*)(\w+) = ' + member + r';[ \t]*$', re.M)
    for original in list(pattern.finditer(text)):
        alias = original[2]
        a = re.escape(alias)
        # Multiple aliases/direct accesses could change the member behind this
        # local value; leave those to a future reference-aware lowering.
        if len(re.findall(member, text)) != 1:
            continue
        declaration = re.compile(r'^[ \t]*CCharString \*' + a + r';[ \t]*\n', re.M)
        store = re.compile(r'^([ \t]*)CCharString::operator=\(' + a + r',\s*(\w+)\);[ \t]*$', re.M)
        compare = re.compile(r'CBasicString<char>::Compare\(\(void \*\)\*\*\(undefined4 \*\*\)'
                             + a + r',\s*("[^"]*")\)')
        null = re.compile(r'\*\(undefined4 \*\*\)' + a + r' (==|!=) \(undefined4 \*\)0x0')
        # Only value-comparison null fallbacks are representable without the
        # native string buffer. Arbitrary buffer-identity branches stay native.
        nulls = list(null.finditer(text))
        from tools.script_recovery.native_evidence_lowering import RE_INLINE_STRNCMP
        valid_nulls = True
        for test in nulls:
            opening = re.match(r'\) \{\s*\n', text[test.end():])
            inline = RE_INLINE_STRNCMP.match(text, test.end() + opening.end()) if opening else None
            if (test[1] != '==' or not inline or inline['l2'] != '""'
                    or inline['l1'] == '""'):
                valid_nulls = False
                break
        if not valid_nulls:
            continue
        probe = declaration.sub('', text)
        probe = pattern.sub('', probe)
        probe = store.sub('', probe)
        probe = compare.sub('', probe)
        probe = null.sub('', probe)
        if re.search(r'\b' + a + r'\b', probe):
            continue
        # Never move a state read across a native helper that receives this.
        cast = r'(?:\([\w :*]+\)\s*)*'
        if (re.search(r'\b[\w:]+\(\s*' + cast + r'this\s*(?:,|\))', text)
                or re.search(r'\b\w+\s*=\s*' + cast + r'this\s*;', text)):
            continue
        text = pattern.sub(lambda m: f'{m[1]}{m[2]} = {getter};', text)
        text = store.sub(lambda m: f'{m[1]}{alias} = {m[2]};\n{m[1]}{setter(m[2])};', text)
        text = compare.sub(lambda m: f'ENGINE_StrCmp({alias}, {m[1]})', text)
        text = null.sub(lambda m: f'{alias} {m[1]} (CCharString *)0x0', text)
    return text
