"""Restore Magicman's final beer movie cleanup from native stack evidence."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed final beer movie {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    decompile = fn['decompile']
    require(hashlib.sha256(decompile.encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for site, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    indirect = {c['site'].lower(): c for c in fn['indirectCalls']}
    direct = {c['site'].lower(): c for c in fn['calls']}
    movie_start = indirect.get(row['startSite'].lower())
    require(movie_start is not None and movie_start.get('slot') == '0x5c8'
            and movie_start['pushedStack'][0] == row['startStack'],
            'native movie start changed')
    require(len(row['cleanupSites']) == 5, 'native cleanup coverage changed')
    for entry in row['cleanupSites']:
        call = direct.get(entry['site'].lower())
        require(call is not None and call['target'].lower() == '0x006e7b80'
                and call['ecxStack'] == entry['stack'],
                f'native cleanup changed at {entry["site"]}')

    constructor = row['rawObject'] + ' = &PTR__scalar_deleting_destructor__01260ef4;'
    require(decompile.count(constructor) == 2 and decompile.count(row['rawCleanup']) == 5,
            'raw movie anchors changed')
    begin = decompile.rindex(constructor)
    end = decompile.rindex(row['rawCleanup']) + len(row['rawCleanup'])
    scope = decompile[begin:end]
    require(scope.count('(**(code **)(**(int **)((int)this + 4) + 0x5c8))();') == 1
            and scope.count(row['rawCleanup']) == 5
            and all(scope.count(label) == 1 for label in row['rawLabels']),
            'raw movie lifetime changed')

    require(source.count(row['luaStart']) == 1, 'lifted movie start changed')
    begin = source.index(row['luaStart'])
    end = source.rindex(row['luaNormalCleanup'], begin) + len(row['luaNormalCleanup'])
    block = source[begin:end]
    require(block.count('resources:StartMovie(') == 1
            and block.count(row['luaWrongCleanup']) == row['luaWrongCount']
            and block.count(row['luaCorrectCleanup']) == row['luaCorrectCount']
            and block.endswith(row['luaNormalCleanup']),
            'lifted movie scope changed')
    return source[:begin] + block.replace(row['luaWrongCleanup'], row['luaCorrectCleanup']) + source[end:]
