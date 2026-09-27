"""Recover reviewed four-byte scalar arrays without inventing struct pointers."""
import re


def lower_integer_array(text, array, base, tag):
    """Keep constant addresses for ordinary field lifting; recover typed accesses.

    Arena's decompiler represents its script receiver as a byte-addressed class
    pointer (retail offsets and ADD 4 establish the cursor stride). Other pointer
    types require their own stride evidence before their cursors can be lowered.
    """
    name = array['name']
    offset = '(?:' + str(array['base']) + '|' + hex(array['base']) + ')'
    key = lambda index: f'__key("{name}_" .. {index})'
    get = lambda index: f'{tag}STATE_GetInt({key(index)})'
    address = base + r' \+ ' + offset
    starts = re.compile(r'^([ \t]*)(\w+) = ' + address + r';', re.M)
    for var in dict.fromkeys(m.group(2) for m in starts.finditer(text)):
        if (name, array['base'], array['count'], base) != ('TotalCreatures', 0xd0, 3, 'this'):
            continue
        escaped = re.escape(var)
        declaration = re.compile(r'^[ \t]*CQ_ArenaScript \*' + escaped + r';', re.M)
        if not declaration.search(text):
            continue
        reset = re.compile(r'^([ \t]*)' + escaped + r' = ' + address + r';', re.M)
        increment = re.compile(r'\b' + escaped + r' = ' + escaped + r' \+ 4;')
        deref = re.compile(r'(?<![&*])\*\((?:int|undefined4) \*\)' + escaped + r'\b')
        first_reset = reset.search(text)
        if re.search(r'\b' + escaped + r'\b', declaration.sub('', text[:first_reset.start()])):
            continue
        # Validate the entire variable lifetime, including every reset. An alias,
        # escaped address, different store width, or reassignment declines recovery.
        remainder = declaration.sub('', reset.sub('', increment.sub('', deref.sub('', text))))
        if re.search(r'\b' + escaped + r'\b', remainder):
            continue
        if re.search(r'&\s*\*\([^)]*\)' + escaped + r'\b', text):
            continue
        index = name[0].lower() + name[1:] + 'Index'
        suffix = 2
        while re.search(r'\b' + re.escape(index) + r'\b', text):
            index = name[0].lower() + name[1:] + 'Index' + str(suffix)
            suffix += 1
        text = declaration.sub('  int ' + index + ';', text)
        text = reset.sub(lambda m: m.group(1) + index + ' = 0;', text)
        text = increment.sub(index + ' = ' + index + ' + 1;', text)
        store = re.compile(r'^([ \t]*)' + deref.pattern + r' = ([^;]+);', re.M)
        text = store.sub(lambda m: f'{m.group(1)}{tag}STATE_SetInt({key(index)}, {deref.sub(get(index), m.group(2))});', text)
        text = deref.sub(get(index), text)
    # Dynamic element indexing has explicit four-byte scaling. No pointer value
    # is fabricated for unsupported address-taking or cursor use.
    idx = r'(?P<idx>\*\(int \*\)\(this \+ 0x[0-9a-f]+\)|(?:QUEST|ENTITY)STATE_GetInt\("[^"]*"\)|[A-Za-z_]\w*)'
    for expression in (idx + r' \* (?:4|0x4) \+ ' + offset + r' \+ ' + base,
                       base + r' \+ ' + offset + r' \+ ' + idx + r' \* (?:4|0x4)'):
        deref = r'\*\((?:int|undefined4) \*\)\(' + expression + r'\)'
        text = re.sub(r'^([ \t]*)' + deref + r' = ([^;]+);',
                      lambda m: f'{m.group(1)}{tag}STATE_SetInt({key(m.group("idx"))}, {m.group(3)});', text, flags=re.M)
        text = re.sub(deref, lambda m: get(m.group('idx')), text)
    return text
