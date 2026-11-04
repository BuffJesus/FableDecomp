"""Restore the single sampled definition sound reused by Trader Escort entities."""
import hashlib
import re


def restore(source, todo, calls, fn, row, rdata):
    if row is None:
        return source, todo, calls

    def require(ok, reason):
        if not ok:
            raise ValueError(f'Trader Escort sound {fn["address"]}: {reason}')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'reviewed source changed')
    require(hashlib.sha256(rdata.bytes_at(start, end - start)).hexdigest() == row['nativeSha256'],
            'native body changed')
    a, b = int(row['windowStart'], 16), int(row['windowEnd'], 16)
    require(start <= a < b <= end and rdata.bytes_at(a, b - a).hex() == row['windowHex'],
            'conversion, copy, or release instructions changed')
    native = fn['decompile']
    require('*(int *)(DAT_0143e90c + 0x3ac)' in native and
            '*(int *)(DAT_0143e90c + 0x3b0)' in native and
            'uVar5 % (uint)(' in native and
            ' + 0xac8)' in native and row['nativeCopy'] in native,
            'native sound selection changed')

    first, last, random = row['first'], row['last'], row['random']
    sample = f'{random} = quest:RetailRandModulo(32768)'
    sound = (f'local sampledSound = quest:ReadGlobalGameDataDefStringAt(0x3ac, '
             f'{random} % (({last} - {first}) >> 2))')
    require(source.count(sample) == 1 and
            source.count(f'{first} = quest:ReadGlobalGameData(0x3ac)') == 1 and
            source.count(f'{last} = quest:ReadGlobalGameData(0x3b0)') == 1,
            'lifted selection changed')
    source = source.replace(sample, sample + '\n' + ' ' * row['indent'] + sound, 1)
    for fragment in row['todoFragments']:
        lines = source.splitlines(keepends=True)
        found = [line for line in lines if '-- TODO(native): ' + fragment in line]
        require(len(found) == 1, 'lifted temporary conversion changed')
        source = source.replace(found[0], '', 1)
        diagnostic = next((x for x in todo if fragment in x), None)
        require(diagnostic is not None, 'conversion diagnostic changed')
        todo.remove(diagnostic)
    old = 'nil --[[missing]])'
    pattern = re.compile(r'(quest:PlaySoundOnThing\([^,\n]+, )' + re.escape(old))
    require(len(pattern.findall(source)) == row['soundCalls'], 'lifted sound calls changed')
    source = pattern.sub(r'\1sampledSound)', source)
    if row.get('retainedLiteralCall'):
        literal = row['retainedLiteralCall']
        require(source.count(literal) == 1, 'retained sound alias changed')
        a = int(row['retainedCallStart'], 16)
        raw = bytes.fromhex(row['retainedCallHex'])
        require(start <= a < a + len(raw) <= end and rdata.bytes_at(a, len(raw)) == raw,
                'retained sound call instructions changed')
        source = source.replace(literal, literal.replace('"DarkwoodAssassin"', 'sampledSound'), 1)
    missing = 'PlaySoundOnThing: missing soundName (1 args for 2 params)'
    require(todo.count(missing) == row['soundCalls'], 'sound call diagnostics changed')
    for _ in range(row['soundCalls']):
        todo.remove(missing)
    return source, todo, calls
