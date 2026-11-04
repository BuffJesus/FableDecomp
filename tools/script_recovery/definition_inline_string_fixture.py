"""Stock inline CCharString vectors; separate from CDefString conversion tokens."""
import struct
VECTOR='Vector_VCCharString__'
LIMIT=4096

def decode_items(raw):
    if not isinstance(raw,bytes) or len(raw)<4 or len(raw)>4*1024*1024:
        raise ValueError('invalid inline-string vector payload')
    count=struct.unpack_from('<I',raw)[0]
    if count>LIMIT:raise ValueError('inline-string vector count exceeds fixture bound')
    cursor=4;items=[]
    for _ in range(count):
        end=raw.find(b'\0',cursor)
        if end<0:raise ValueError('unterminated inline string')
        if end-cursor>16384:raise ValueError('inline string exceeds fixture bound')
        try:value=raw[cursor:end].decode('utf-8')
        except UnicodeDecodeError as exc:raise ValueError('invalid UTF-8 inline string') from exc
        cursor=end+1;items.append(value)
    if cursor!=len(raw):raise ValueError('trailing inline-string vector bytes')
    return items

def validate(groups,metadata):
    if not isinstance(groups,list):raise ValueError('inline-string groups must be a list')
    occupied=set();members={int(key,0) for key in metadata}
    for group in groups:
        if not isinstance(group,dict):raise ValueError('inline-string group must be an object')
        offset,items=group.get('offset'),group.get('items')
        if type(offset) is not int or offset<4 or offset%4 or offset+12>0x1088:
            raise ValueError('invalid inline-string member offset')
        field=metadata.get(hex(offset),{})
        if (field.get('defType')!=VECTOR or field.get('name')!=group.get('name')
                or field.get('verified') is False or not field.get('retailTransferCall')):
            raise ValueError('inline-string member does not match native metadata')
        words={offset,offset+4,offset+8}
        if words&occupied or members&{offset+4,offset+8}:raise ValueError('overlapping inline-string members')
        occupied|=words
        if (not isinstance(items,list) or len(items)>LIMIT
                or any(not isinstance(value,str) or '\0' in value or len(value.encode('utf-8'))>16384 for value in items)):
            raise ValueError('invalid inline-string item list')
    return occupied

def decode_vectors(metadata):
    groups=[]
    for key,field in metadata.items():
        if field.get('defType')!=VECTOR:continue
        try:raw=bytes.fromhex(field['value'])
        except (KeyError,TypeError,ValueError) as exc:raise ValueError('invalid inline-string vector hex') from exc
        groups.append(dict(offset=int(key,0),name=field.get('name'),items=decode_items(raw)))
    validate(groups,metadata)
    return groups
