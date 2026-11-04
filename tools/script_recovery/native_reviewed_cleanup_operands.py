"""Generic, fail-closed restoration of reviewed cleanup operands."""
import hashlib,re

def restore(source,fn,row,rd):
    if row is None:return source
    def require(ok,why):
        if not ok:raise ValueError(f'reviewed cleanup operands {fn["address"]}: {why}')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=rd.bytes_at(start,end-start)
    require(raw is not None and hashlib.sha256(raw).hexdigest()==row['nativeSha256'],'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest()==row['sourceSha256'],'exported source changed')
    require(hashlib.sha256(source.encode()).hexdigest()==row['luaSha256'],'lifted source phase changed')
    for witness in row['nativeWitnesses']:
        body=rd.bytes_at(int(witness['address'],16),witness['size'])
        require(body is not None and hashlib.sha256(body).hexdigest()==witness['sha256'],'native cleanup witness changed')
    begin,finish=row['spanBegin'],row['spanEnd']
    require(source.count(begin)==source.count(finish)==1,'bounded source labels changed')
    a=source.index(begin);b=source.index(finish,a)
    require(a<b,'source span reversed')
    span=source[a:b]
    for replacement in row['replacements']:
        old,new,count=(replacement[k] for k in ['old','new','count'])
        require(isinstance(count,int) and count>0 and old!=new,'invalid replacement')
        require(span.count(old)==count,'cleanup operand inventory changed')
        span=span.replace(old,new)
    return source[:a]+span+source[b:]
