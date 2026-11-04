"""Lower reviewed definition-token vector conversions into the existing ABI binding."""
import hashlib,re
HELPER=(0x415d70,25,'5d25bf8d028f3d3177c51c19da040833f927308f2fb913e47b0b59ef34ae1a66')
def restore(source,fn,row,rdata):
    if row is None:return source
    def require(ok,why):
        if not ok:raise ValueError(f'reviewed definition vector {fn["address"]}: {why}')
    start=int(fn['address'],16);end=int(fn['bodyEndExclusive'],16)
    require(hashlib.sha256(rdata.bytes_at(start,end-start) or b'').hexdigest()==row['nativeSha256'],'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest()==row['sourceSha256'],'exported source changed')
    require(hashlib.sha256(rdata.bytes_at(HELPER[0],HELPER[1]) or b'').hexdigest()==HELPER[2],'conversion helper changed')
    text=source.replace('\r\n','\n');edits=[]
    require(bool(row['sites']),'empty conversion inventory')
    for site in row['sites']:
        # Exact reviewed window proves global first/last loads, stride-four
        # unsigned remainder, conversion hidden output and temporary destructor.
        a=int(site['start'],16);b=int(site['end'],16)
        require(start<=a<b<=end,'window outside function')
        require((rdata.bytes_at(a,b-a) or b'').hex()==site['nativeBytes'],'native conversion window changed')
        anchor=site['sourceAnchor'].replace('\r\n','\n')
        require(text.count(anchor)==1,'source conversion missing or ambiguous')
        pattern=r'(?P<last>\w+) = ENGINE_GlobalGameData\((?P<lastoff>0x[0-9a-f]+)\);\n\s*(?P<first>\w+) = ENGINE_GlobalGameData\((?P<off>0x[0-9a-f]+)\);\n\s*(?P<rng>\w+) = ENGINE_Rand\(\);\n\s*(?P<conversion>CDefString::operator_class_CCharString\(\(CDefString \*\)\(ENGINE_GlobalGameData\((?P=off)\) \+ \((?P=rng) % \(uint\)\((?P=last) - (?P=first) >> 2\)\) \* 4\),\(int\)&(?P<out>\w+)\);)'
        m=re.search(pattern,anchor)
        require(m is not None,'unsupported vector expression')
        offset=int(m['off'],16)
        require(offset==site['offset'] and int(m['lastoff'],16)==offset+4,'vector bounds disagree')
        require(m['out']==site['output'],'hidden output disagrees')
        conversion=m['conversion']
        replacement=f'{m["out"]} = ENGINE_GlobalGameDataDefStringAt({m["off"]}, {m["rng"]} % (({m["last"]} - {m["first"]}) >> 2));'
        at=text.index(anchor)+m.start('conversion');edits.append((at,at+len(conversion),replacement))
    edits.sort();require(all(a[1]<=b[0] for a,b in zip(edits,edits[1:])),'overlapping conversion sites')
    for a,b,value in reversed(edits):text=text[:a]+value+text[b:]
    return text.replace('\n','\r\n' if '\r\n' in source else '\n')
