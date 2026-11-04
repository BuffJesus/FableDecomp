"""Recover value operations from reviewed CCharString representation idioms."""
import re


def _result_is_dead(tail, name):
    """Reject a read before an unconditional overwrite in its enclosing block."""
    defined = [False]
    token = re.compile(r'^[ \t]*\w+:[ \t]*$|"(?:[^"\\]|\\.)*"|[{}]|\b' + re.escape(name) + r'\b', re.M)
    for use in token.finditer(tail):
        if use[0].startswith('"'):
            continue
        if use[0].rstrip().endswith(':'):
            defined = [False] * len(defined)  # another path may jump past a local overwrite
            continue
        if use[0] == '{':
            defined.append(defined[-1])
        elif use[0] == '}':
            if len(defined) > 1:
                defined.pop()
            else:
                defined[0] = False
        else:
            assignment = re.match(re.escape(name) + r' = ([^;\n]+);', tail[use.start():])
            prefix = tail[tail.rfind('\n', 0, use.start()) + 1:use.start()]
            if assignment and not prefix.strip() and not re.search(r'\b' + re.escape(name) + r'\b', assignment[1]):
                defined[-1] = True
            elif not defined[-1]:
                return False
    return True


def fold_constructor_copy(text):
    """The literal constructor returns its receiver; the next copy consumes it.

    Require that the temporary pointer has no other use, including escapes.
    Retail 99EBF0 returns ESI in EAX; GuildMaster uses it at E91F37.
    """
    pattern = re.compile(
        r'^(?P<indent>[ \t]*)(?P<ptr>\w+) = (?P<ctor>CCharString::CCharString\('
        r'\(CCharString \*\)&(?P<src>\w+),(?:"[^"\n]*"|&DAT_[0-9a-f]+),-1\));\s*'
        r'CCharString::CCharString\(\(CCharString \*\)&(?P<dst>\w+),(?P=ptr)\);', re.M)
    def replace(m):
        ptr = re.escape(m['ptr'])
        declaration = re.compile(r'^[ \t]*CCharString \*' + ptr + r';[ \t]*$', re.M)
        if len(re.findall(r'\b' + ptr + r'\b', text)) != 3 or not declaration.search(text):
            return m[0]
        return (f'{m["indent"]}{m["ctor"]};\n'
                f'{m["indent"]}CCharString::CCharString((CCharString *)&{m["dst"]},&{m["src"]});')
    return pattern.sub(replace, text)


def fold_member_rep_read(text, member, value):
    """Read a string value only after its complete representation test folded.

    A raw rep can also be used for refcounts or pointer arithmetic. Those uses
    must stay unresolved instead of receiving a Lua string in place of memory.
    """
    pattern = re.compile(r'^(?P<indent>[ \t]*)(?P<var>\w+) = \*' + member +
                         r';\s*(?P<test>if \(ENGINE_StrEq\((?P=var), \w+\)\) \{)', re.M)
    def replace(m):
        if not _result_is_dead(text[m.end():], m['var']):
            return m[0]
        return f'{m["indent"]}{m["var"]} = {value};\n{m["indent"]}{m["test"]}'
    return pattern.sub(replace, text)


def fold_rep_equality(text):
    """Fold the full identity/null/length/content equality expression.

    Every repeated operand and the zero-result test must agree. The Compare
    temporary must be dead or overwritten afterwards; its incidental value
    differs depending on the short-circuit path and cannot be synthesized.
    """
    pattern = re.compile(
        r'if \(\((?P<a>\w+) == (?P<b>\w+)\) \|\| '
        r'\(\(\(\((?P=a) != \(CCharString\)0x0 && \((?P=b) != \(CCharString\)0x0\)\) && '
        r'\(\*\(int \*\)\(\(int\)(?P=a) \+ 4\) == \*\(int \*\)\(\(int\)(?P=b) \+ 4\)\)\) && '
        r'\((?P<tmp>\w+) = CBasicString<char>::Compare\(\*\(void \*\*\)(?P=a),\*\(void \*\*\)(?P=b)\), (?P=tmp) == 0\)\)\)\) \{')
    def replace(m):
        if m['tmp'] in (m['a'], m['b']) or not _result_is_dead(text[m.end():], m['tmp']):
            return m[0]
        return f'if (ENGINE_StrEq({m["a"]}, {m["b"]})) {{'
    return fold_array_rep_equality(pattern.sub(replace, text))


def fold_array_rep_equality(text):
    # The same complete string representation test can occur under a message
    # guard, with DWORD array indexing for the length field and buffer pointer.
    # Do not discard an observable Compare result from any subsequent path.
    pattern = re.compile(
        r"if \(\((?P<gate>\w+ != '\\0')\) && \(\((?P<a>\w+) == (?P<b>\w+) \|\| "
        r'\(\(\(\((?P=a) != \(undefined4 \*\)0x0 && \((?P=b) != \(undefined4 \*\)0x0\)\) && '
        r'\((?P=a)\[1\] == (?P=b)\[1\]\)\) && '
        r'\((?P<tmp>\w+) = CBasicString<char>::Compare\(\(void \*\)\*(?P=a),\(void \*\)\*(?P=b)\), (?P=tmp) == 0\)\)\)\)\)\) \{')
    def replace(m):
        if m['a']==m['b'] or m['tmp'] in (m['a'],m['b']) or not _result_is_dead(text[m.end():],m['tmp']):
            return m[0]
        return f"if (({m['gate']}) && ENGINE_StrEq({m['a']}, {m['b']})) {{"
    return pattern.sub(replace,text)
