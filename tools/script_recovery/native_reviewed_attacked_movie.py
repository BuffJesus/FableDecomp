"""Restore Magicman's attacked movie cleanup after reviewed native lowering."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed attacked movie {fn["address"]}: {why}')

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
    require(len(row['cleanupSites']) == 3, 'native cleanup coverage changed')
    for entry in row['cleanupSites']:
        call = direct.get(entry['site'].lower())
        require(call is not None and call['target'].lower() == entry['target'].lower()
                and call['ecxStack'] == entry['stack'],
                f'native cleanup changed at {entry["site"]}')

    obj, member = row['rawObject'], row['rawMember']
    require(int(obj.rsplit('_', 1)[1], 16) - int(member.rsplit('_', 1)[1], 16) == 8,
            'raw movie member layout changed')
    constructor = obj + '[0] = &PTR__scalar_deleting_destructor__01260ef4;'
    normal = obj + '[0] = &PTR__vector_deleting_destructor__0126008c;'
    require(decompile.count(constructor) == 1 and decompile.count(normal) == 1,
            'raw movie anchors changed')
    begin = decompile.index(constructor)
    end = decompile.index(normal, begin) + len(normal)
    scope = decompile[begin:end]
    require(all(scope.count(label) == 1 for label in row['rawLabels'])
            and scope.count('((CScriptGameResourceObjectMovieBase *)' + obj + ');') == 2
            and scope.count('(&' + member + ');') == 1,
            'raw movie lifetime changed')

    lua_start = row['luaStart']
    lua_end = row['luaNormalCleanup']
    require(source.count(lua_start) == 1 and source.count(lua_end) == 1,
            'lifted movie anchors changed')
    begin = source.index(lua_start)
    end = source.index(lua_end, begin) + len(lua_end)
    block = source[begin:end]
    require(block.count('resources:StartMovie(') == 1
            and block.count(row['luaWrongCleanup']) == row['luaWrongCount']
            and block.count(lua_end) == 1,
            'lifted movie scope changed')
    fixed = block.replace(row['luaWrongCleanup'], lua_end)
    return source[:begin] + fixed + source[end:]
