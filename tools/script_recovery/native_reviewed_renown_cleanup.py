"""Remove C++ temporary-string teardown from the renown Lua body."""
import hashlib
import re

LOOP=re.compile(
    r'(?m)^(?P<indent> +)repeat\n'
    r'(?P=indent)    register0x00000010 = \(register0x00000010 \+ -4\)\n'
    r'(?P=indent)    iVar2 = iVar2 \+ -1\n'
    r'(?P=indent)until not \(iVar2 ~= 0\)\n')
TODO='-- TODO(native): _vector_constructor_iterator_((int)xStack_20,4,8,0x99eae0);'
DIAGNOSTIC='unlifted: _vector_constructor_iterator_((int)xStack_20,4,8,0x99eae0);'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo
    def require(ok, reason):
        if not ok:
            raise ValueError('renown string cleanup: '+reason)
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest()==row['sourceSha256'],'source changed')
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    require(hashlib.sha256(rdata.bytes_at(start,end-start)).hexdigest()==row['nativeSha256'],'native body changed')
    require(row['temporaryCount']==8 and row['destructorAddress']=='0x0099eae0' and
            fn['decompile'].count('_vector_constructor_iterator_((int)local_20,4,8,0x99eae0);')==3 and
            fn['decompile'].count('iVar2 = 8;')==3,
            'native temporary teardown changed')
    require(len(LOOP.findall(source))==4 and source.count('iVar2 = 8\n')==3 and
            source.count(TODO)==3 and todo.count(DIAGNOSTIC)==3 and
            source.count('local iVar2, register0x00000010')==1,
            'lifted cleanup shape changed')
    source=LOOP.sub('',source)
    source=re.sub(r'(?m)^ +iVar2 = 8\n','',source)
    source=re.sub(r'(?m)^ +'+re.escape(TODO)+r'\n','',source)
    source=source.replace('local iVar2, register0x00000010','local iVar2',1)
    todo=[item for item in todo if item!=DIAGNOSTIC]
    return source,todo
