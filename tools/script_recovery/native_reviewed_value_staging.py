"""Classify witnessed pointer staging only after proving its Lua value unused."""
import hashlib
import re
from collections import Counter
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.readable_lua import tokens


def classify(source, todo, fn, row, rdata):
    if row is None:
        return source, list(todo), []
    def require(ok, why):
        if not ok: raise ValueError(f'reviewed value staging {fn["address"]}: {why}')
    digest = lambda b: hashlib.sha256(b).hexdigest()
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end-start)
    require(raw is not None and digest(raw) == row['nativeSha256'], 'native body changed')
    require(digest(fn['decompile'].encode()) == row['sourceSha256'], 'exported source changed')
    require(digest(source.encode()) == row['luaBodySha256'], 'lifted source phase changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32); decoder.detail = True
    instructions = list(decoder.disasm(raw, start)); by = {i.address: i for i in instructions}
    require([(i.mnemonic, i.op_str) for i in instructions[:7]] == [('sub','esp, 0xd4'),('push','ebx'),('push','ebp'),('push','esi'),('mov','ebp, ecx'),('push','edi'),('lea','edi, [ebp + 8]')], 'script frame/receiver origin changed')
    require(row['frameBytes'] == 228 and row['headerFrameOffset'] == 24 and row['headerEntryOffset'] == -204, 'CString frame identity changed')
    require(all(re.fullmatch('[0-9a-f]{64}', row[key]) for key in ['acceptedDispatchSealSha256','acceptedSdkSealSha256']), 'missing accepted contract evidence')
    require('unverified' in row['scopeLimit'] and 'ABI' in row['scopeLimit'], 'missing proof limits')
    from tools.script_recovery.convert_quest_unit import callee_stack_words
    expected_helpers = {'compare':(0x411570,0,40),'chars':(0x99e4c0,0,15),'resourceCtor':(0x7e72a0,0,26),'actorCtor':(0x4abe90,1,33),'gsiCtor':(0x6e7740,5,173)}
    helper_shapes = {}
    for name, (address, words, extent) in expected_helpers.items():
        helper = row['helpers'][name]; body = bytes.fromhex(helper['bytes'])
        require(int(helper['address'],16) == address and len(body) == extent and rdata.bytes_at(address,len(body)) == body and callee_stack_words(address) == words, 'helper identity/ABI changed: '+name)
        decoded = list(decoder.disasm(body,address))
        require(sum(i.size for i in decoded) == extent, 'incomplete helper instructions: '+name)
        helper_shapes[name] = [(i.mnemonic,i.op_str) for i in decoded]
    for name, setup, write in [
        ('resourceCtor',('mov','esi, ecx'),('mov','dword ptr [esi], 0x127094c')),
        ('actorCtor',('mov','eax, ecx'),('mov','dword ptr [eax], 0x1238c8c')),
        ('gsiCtor',('mov','esi, ecx'),('mov','dword ptr [esi], 0x1260f0c')),
    ]:
        require(setup in helper_shapes[name] and write in helper_shapes[name], 'canonical receiver constructor changed: '+name)
    require(helper_shapes['chars'] == [('mov','eax, dword ptr [ecx]'),('test','eax, eax'),('jne','0x99e4cc'),('mov','eax, 0x129aaf4'),('ret',''),('mov','eax, dword ptr [eax]'),('ret','')], 'CString text accessor changed')
    require(helper_shapes['compare'] == [('push','ebx'),('mov','al, byte ptr [ecx]'),('test','al, al'),('jne','0x41157c'),('cmp','byte ptr [edx], 0'),('je','0x411588'),('mov','bl, byte ptr [edx]'),('cmp','al, bl'),('jl','0x41158c'),('jg','0x411591'),('inc','ecx'),('inc','edx'),('jmp','0x411571'),('xor','eax, eax'),('pop','ebx'),('ret',''),('or','eax, 0xffffffff'),('pop','ebx'),('ret',''),('mov','eax, 1'),('pop','ebx'),('ret','')], 'signed-byte comparison helper changed')
    def sequence(witness):
        result = []
        for item in witness:
            site = int(item['site'],16); i = by.get(site)
            require(i is not None and i.bytes.hex() == item['bytes'], 'native witness changed')
            result.append(i)
        require(result and all(a.address+a.size == b.address for a,b in zip(result,result[1:])), 'noncontiguous native sequence')
        return result
    def shape(seq, template):
        require([(i.mnemonic,i.op_str) for i in seq] == template, 'native staging operands changed')
    def dispatch(site, table, slot, words):
        call = by[int(site,16)]; op = call.operands[0]
        require(call.mnemonic == 'call' and op.type == x86.X86_OP_MEM and op.mem.disp == slot, 'dispatch slot changed')
        metadata = [c for c in fn['indirectCalls'] if int(c['site'],16) == call.address]
        require(len(metadata) == 1 and int(metadata[0]['slot'],16) == slot, 'dispatch metadata changed')
        target = int.from_bytes(rdata.bytes_at(table+slot,4),'little')
        require(callee_stack_words(target) == words, 'dispatch return ABI changed')
    parts = row['reads']; expected = Counter(p['diagnostic'] for p in parts)
    require(parts and all(todo.count(d) == n for d,n in expected.items()), 'diagnostic multiplicity changed')
    require(len({p['nativeSite'] for p in parts}) == len(parts), 'duplicate native staging site')
    edits, reviews = [], []
    for part in parts:
        seq = sequence(part['sequence']); site = int(part['nativeSite'],16)
        require(site in {i.address for i in seq}, 'staging site outside sequence')
        require(part['nativeSourceFragment'] in fn['decompile'], 'native source fragment changed')
        region = part['luaRegion']; require(source.count(region) == 1, 'ambiguous lifted staging region')
        diagnostic = part['diagnostic']; require(diagnostic.startswith('unlifted: '), 'not an unlifted diagnostic')
        alias = part['alias']; require(diagnostic[10:].startswith(alias+' = '), 'diagnostic target changed')
        pattern = re.compile(r'(?m)^[ \t]*-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n[ \t]*(?P<assignment>'+re.escape(alias)+r' = nil) --\[\[unresolved native value\]\]\n')
        matches = list(pattern.finditer(region)); require(len(matches) == 1, 'staging comment/assignment changed')
        match = matches[0]; offset = source.index(region)+match.start('assignment')
        liveness = prove_unused_staging(source, offset, alias)
        family = part['family']
        if family == 'cstring-text':
            literal = part['literal']; address = int(part['literalAddress'],16)
            require(rdata.string_at(address) == literal, 'comparison literal changed')
            shape(seq,[('mov','esi, dword ptr [eax]'),('mov','edx, '+hex(address)),('mov','ecx, esi'),('call','0x411570')])
            guard = sequence(part['guard'])
            shape(guard,[('mov','eax, dword ptr [esp + 0x18]'),('test','eax, eax'),('jne',hex(site))])
            require(site == seq[0].address and '('+part['luaStringValue']+' == "'+literal+'")' in region, 'restored Lua comparison changed')
        elif family == 'gsi-pointer' and part['method'] == 'GetHero':
            shape(seq,[('mov','ecx, dword ptr [ebp + 4]'),('mov','edi, dword ptr [ebx]'),('push','0'),('push','1'),('push','0'),('mov','dword ptr [esp + 0x38], ecx'),('push','0'),('lea','ecx, [esp + 0x30]'),('call','0x99e4c0'),('mov','ecx, dword ptr [esp + 0x3c]'),('mov','edx, dword ptr [ecx]'),('push','eax'),('call','dword ptr [edx + 0x118]')])
            require(site == seq[0].address and 'quest:GetHero()' in region, 'restored getter changed'); dispatch(part['nativeCall'],0x1260f0c,0x118,0)
        elif family == 'gsi-pointer' and part['method'] == 'PauseAllNonScriptedEntities':
            shape(seq,[('mov','esi, dword ptr [ebp + 4]'),('mov','edx, dword ptr [esi]'),('push','1'),('mov','ecx, esi'),('mov','dword ptr [esp + 0x30], esi'),('call','dword ptr [edx + 0x5ec]')])
            require(site == seq[0].address and 'quest:PauseAllNonScriptedEntities(true)' in region, 'restored true pause changed'); dispatch(part['nativeCall'],0x1260f0c,0x5ec,1)
        elif family == 'gsi-vtable':
            tail = sequence(part['tail']); shape(tail,[('push','0'),('call','dword ptr [eax + 0x5ec]')])
            if part['receiver'] == 'register':
                shape(seq,[('mov','eax, dword ptr [esi]'),('mov','ecx, esi'),('jmp',hex(tail[0].address))])
                require(site == seq[0].address and 'goto '+part['luaCleanupLabel'] in region, 'restored cleanup jump changed')
                cleanup = part['luaCleanupRegion']; require(source.count(cleanup) == 1 and cleanup.startswith('::'+part['luaCleanupLabel']+'::') and 'quest:PauseAllNonScriptedEntities(false)' in cleanup, 'restored shared false pause changed')
            else:
                require(part['receiver'] == 'saved-frame', 'unsupported receiver')
                shape(seq,[('mov','ecx, dword ptr [esp + 0x2c]'),('mov','eax, dword ptr [ecx]'),('push','0'),('call','dword ptr [eax + 0x5ec]')])
                require(site == seq[1].address and 'quest:PauseAllNonScriptedEntities(false)' in region, 'restored saved-frame false pause changed')
            dispatch(part['nativeCall'],0x1260f0c,0x5ec,1)
        elif family == 'resource-vtable':
            address = int(part['literalAddress'],16); require(rdata.string_at(address) == part['literal'], 'speech literal changed')
            shape(seq,[('mov','ecx, dword ptr [ebp + 4]'),('mov','edx, dword ptr [ecx]'),('mov','edi, dword ptr [ebx]'),('push','0'),('push','1'),('push','0'),('push','0'),('push',hex(address)),('call','dword ptr [edx + 0x118]'),('push','eax'),('mov','ecx, ebx'),('call','dword ptr [edi + 0x34]')])
            require(site == seq[2].address and 'me:Speak(uVar7, "'+part['literal']+'", 0, false, true, false)' in region, 'restored resource speech changed')
            dispatch(part['nativeCall'],0x127094c,0x34,6)
        else: require(False, 'unsupported staging family')
        begin = source.index(region)+match.start(); limit = source.index(region)+match.end()
        replacement = source[begin:limit]
        # Preserve the annotation boundary used by the readable pass while
        # recording that this retained nil value has a completed dead-use proof.
        replacement = replacement[replacement.index('\n')+1:].replace('--[[unresolved native value]]','--[[reviewed native staging]]')
        edits.append((begin,limit,replacement)); reviews.append(dict(nativeSite=part['nativeSite'],family=family,diagnostic=diagnostic,liveness=liveness,executableLuaUnchanged=True))
    require(all(a[1] <= b[0] for a,b in zip(sorted(edits),sorted(edits)[1:])), 'overlapping staging edits')
    out = source
    for begin,limit,replacement in sorted(edits,reverse=True): out = out[:begin]+replacement+out[limit:]
    significant = lambda s: [(t.lastgroup,t[0]) for t in tokens(s) if t.lastgroup not in ['space','comment','longcomment']]
    require(significant(out) == significant(source), 'classification changed executable Lua')
    remaining = list(todo)
    for part in parts: remaining.remove(part['diagnostic'])
    return out, remaining, [dict(reads=reviews,acceptedDispatchSealSha256=row['acceptedDispatchSealSha256'],acceptedSdkSealSha256=row['acceptedSdkSealSha256'],scopeLimit=row['scopeLimit'],executableLuaUnchanged=True)]
