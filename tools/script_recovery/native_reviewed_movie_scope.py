"""Rejoin a reviewed movie scope after native lowering has resolved calls."""
import hashlib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed movie scope {fn["address"]}: {why}')

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
    constructor = obj + ' = &PTR__scalar_deleting_destructor__01260ef4;'
    destructor = 'CPhysicsMeshInfo::~CPhysicsMeshInfo((CPhysicsMeshInfo *)&' + obj + ');'
    require(decompile.count(constructor) == 2 and 'LAB_00e437b3:' in decompile
            and destructor in decompile, 'raw movie anchors changed')
    raw_start = decompile.index(constructor)
    raw_normal = decompile.index('LAB_00e437b3:', raw_start)
    raw_end = decompile.index(destructor, raw_normal) + len(destructor)
    scope = decompile[raw_start:raw_end]
    require(all(scope.count(label) == 1 for label in row['rawLabels'])
            and scope.count('((CScriptGameResourceObjectMovieBase *)&' + obj + ');') == 2
            and scope.count('(&' + member + ');') == 1
            and scope.count(obj + ' = &PTR__vector_deleting_destructor__0126008c;') == 1,
            'raw movie lifetime changed')

    require(source.count(row['luaStart']) == 1
            and '::LAB_00e437b3::' in source
            and row['luaNormalCleanup'] in source,
            'lifted movie anchors changed')
    begin = source.index(row['luaStart'])
    label = source.index('::LAB_00e437b3::', begin)
    end = source.index(row['luaNormalCleanup'], label) + len(row['luaNormalCleanup'])
    block = source[begin:end]
    require(block.count(row['luaStart']) == 1
            and block.count(row['luaSharedCleanup']) == row['luaSharedCount']
            and block.count(row['luaNormalCleanup']) == 1
            and block.count('resources:StartMovie(') == 1,
            'lifted movie scope changed')
    replacement = row['luaName']
    block = block.replace(row['luaStart'], replacement + ' = resources:StartMovie("")')
    block = block.replace(row['luaSharedCleanup'], 'resources:DestroyMovie(' + replacement + ')')
    block = block.replace(row['luaNormalCleanup'], 'resources:DestroyMovie(' + replacement + ')')
    source = source[:begin] + block + source[end:]
    local_resources = '    local resources = quest:RetailResources()\n'
    require(source.count(local_resources) == 1 and 'local ' + replacement not in source,
            'Lua movie local changed')
    return source.replace(local_resources, local_resources + '    local ' + replacement + '\n', 1)
