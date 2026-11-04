"""Classify a reviewed native scratch-byte spill while preserving all Lua tokens."""
import hashlib,re
from tools.script_recovery.readable_lua import tokens


def classify(source,todo,fn,row,image,unit,owner,summarize,prove):
    if row is None:return source,list(todo),[]
    def require(ok,why):
        if not ok:raise ValueError('reviewed native scratch-byte spill '+fn['address']+': '+why)
    sha=lambda data:hashlib.sha256(data).hexdigest()
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=image.bytes_at(start,end-start)
    require(raw is not None and sha(raw)==row['nativeSha256'],'native extent changed')
    require(sha(fn['decompile'].encode())==row['sourceSha256'],'exported native source changed')
    require(sha(source.encode())==row['luaPhaseSha256'],'lifted source phase changed')
    require(owner==row['owner'] and owner in unit.get('entities',{}),'owner identity changed')
    entity=unit['entities'][owner];metadata=row['helperMetadata']
    require(int(entity['vtable'],16)==int(metadata['entityVtable'],16),'owner vtable changed')
    require(int(entity['functions']['GetParentScript']['address'],16)==int(metadata['getter']['address'],16),
            'registered parent getter changed')
    summary=summarize(image,fn,metadata)
    require(summary['nativeProofs']==row['helperNativeProofs'],'helper native identities changed')
    reads=row['reads'];require(reads and len({read['nativeStore'] for read in reads})==len(reads),'empty/duplicate stores')
    replacements=[];reviews=[];remaining=list(todo)
    for read in reads:
        site=int(read['nativeStore'],16)
        witness=bytes.fromhex(read['nativeStoreBytes'])
        require(image.bytes_at(site,len(witness))==witness,'byte-store witness changed')
        proof=prove(raw,start,site,summary['contracts'],nonstack_registers=summary['nonstackRegisters'])
        require(proof['slotOffset']==read['entryEspOffset'] and proof['width']==4,'scratch slot identity changed')
        fragment=read['nativeSourceFragment']
        require(fragment and fn['decompile'].count(fragment)==1,'ambiguous native source fragment')
        diagnostic=read['diagnostic'];alias=read['alias'];marker=read['marker']
        require(re.fullmatch('[A-Za-z_]\\w*',alias) is not None and diagnostic.startswith('unlifted: '+alias+' = '),
                'diagnostic identity changed')
        require(marker.startswith(diagnostic[10:]) and marker.startswith(alias+' = '),'truncated diagnostic/marker mismatch')
        require(remaining.count(diagnostic)==1,'diagnostic multiplicity changed')
        pattern=re.compile(r'(?m)^(?P<indent>[ \t]*)-- TODO\(native\): '+re.escape(marker)+r'$')
        matches=list(pattern.finditer(source));require(len(matches)==1,'ambiguous Lua marker')
        match=matches[0]
        replacement=match['indent']+'-- reviewed native scratch-byte spill: '+marker
        replacements.append((match.start(),match.end(),replacement));remaining.remove(diagnostic)
        reviews.append(dict(nativeStore=read['nativeStore'],diagnostic=diagnostic,proof=proof,
                            executableLuaUnchanged=True))
    require(all(a[1]<=b[0] for a,b in zip(sorted(replacements),sorted(replacements)[1:])),'overlapping markers')
    out=source
    for begin,end,replacement in sorted(replacements,reverse=True):out=out[:begin]+replacement+out[end:]
    significant=lambda text:[(token.lastgroup,token[0]) for token in tokens(text)
        if token.lastgroup not in ('space','comment','longcomment')]
    require(significant(out)==significant(source),'executable Lua changed')
    return out,remaining,[dict(reads=reviews,helperAccess=summary,executableLuaUnchanged=True)]
