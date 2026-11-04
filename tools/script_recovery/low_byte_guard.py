"""Conservative whole-word observation guard for decompiler low-byte splitting."""
import re


def word_is_observed(text, var, own, rebuild, fold_colours):
    first = own.search(text)
    if first is None:
        return False
    literal = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'')
    blank = lambda source: literal.sub(lambda m: re.sub(r'[^\n]', ' ', m[0]), source)
    word = re.compile(r'\b' + re.escape(var) + r'\b')
    byte = re.compile(r'\(char\)' + re.escape(var) + r'\b')

    def observations(source):
        # Existing colour lowering consumes a separate complete-word lifetime.
        # Use its proof only for analysis; the actual pass order stays unchanged.
        source = fold_colours(source)
        source = own.sub(lambda m: m.group(1), source)
        source = rebuild.sub('', source)
        source = byte.sub('', source)
        source = re.sub(r'^[ \t]*' + re.escape(var) + r'(?= = )', '', source, flags=re.M)
        return bool(word.search(blank(source)))

    if observations(text[first.start():]):
        return True
    prefix = blank(text[:first.start()])
    tail = blank(text[first.start():])
    if byte.search(prefix):
        return True  # the new split local would be read before initialization
    if re.search(r'&\s*(?:\(\s*)*' + re.escape(var) + r'\b', prefix):
        return True  # an earlier escaped address may observe the later store
    # A backward edge may observe the modified word before its textual store.
    for label in re.finditer(r'^\s*(\w+):', prefix, re.M):
        if re.search(r'\bgoto\s+' + re.escape(label[1]) + r'\s*;', tail):
            if observations(text[label.start():first.start()]):
                return True
    # Likewise for an enclosing structured loop. Closed earlier loops do not
    # make input-parameter reads at function entry part of the byte lifetime.
    stack = []
    cursor = 0
    for brace in re.finditer(r'[{}]', prefix):
        if brace[0] == '{':
            header = prefix[cursor:brace.start()]
            loop = re.search(r'\b(?:do\s*|(?:for|while)\s*\([^{}]*\))\s*$', header)
            stack.append(cursor + loop.start() if loop else None)
        elif stack:
            stack.pop()
        cursor = brace.end()
    for start in stack:
        if start is not None and observations(text[start:first.start()]):
            return True
    pending = re.search(r'\b(?:for|while)\s*\([^{}]*\)\s*$', prefix[cursor:])
    if pending and observations(text[cursor+pending.start():first.start()]):
        return True
    return False
