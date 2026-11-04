"""Decode explicit stock conversation values for smoke fixtures only.

Serialized fields are tagged in transfer order. Native object offsets are not
used here; the value snapshot exposes Lines and four independent string lists.
"""
import struct
from tools.decode_game_bin_field import field_tag

VECTOR='vector<CConversation>'
STRINGS=('Speaker','Dialogue','Animation','AnimLoop')
FIELDS=('Lines',*STRINGS)
LIMIT=4096

def decode_records(raw):
    if not isinstance(raw,bytes) or len(raw)>4*1024*1024:
        raise ValueError('invalid serialized conversation payload')
    cursor=0
    def word(signed=False):
        nonlocal cursor
        if cursor+4>len(raw):raise ValueError('truncated conversation word')
        value=struct.unpack_from('<i' if signed else '<I',raw,cursor)[0];cursor+=4
        return value
    def count():
        value=word()
        if value>LIMIT:raise ValueError('conversation count exceeds fixture bound')
        return value
    records=[]
    for _ in range(count()):
        record={}
        for name in FIELDS:
            if word()!=field_tag(name):raise ValueError('conversation field tag/order mismatch: '+name)
            if name=='Lines':
                value=word(True)
                if not 0<=value<=LIMIT:raise ValueError('conversation Lines exceeds fixture bound')
                record[name]=value
            else:
                values=[]
                for _ in range(count()):
                    end=raw.find(b'\0',cursor)
                    if end<0:raise ValueError('unterminated conversation string')
                    if end-cursor>16384:raise ValueError('conversation string exceeds fixture bound')
                    try:value=raw[cursor:end].decode('utf-8')
                    except UnicodeDecodeError as exc:raise ValueError('invalid UTF-8 conversation string') from exc
                    cursor=end+1;values.append(value)
                record[name]=values
        records.append(record)
    if cursor!=len(raw):raise ValueError('trailing conversation bytes')
    return records

def validate(groups,metadata):
    if not isinstance(groups,list):raise ValueError('conversation groups must be a list')
    occupied=set();members={int(key,0) for key in metadata}
    for group in groups:
        if not isinstance(group,dict):raise ValueError('conversation group must be an object')
        offset,records=group.get('offset'),group.get('records')
        if type(offset) is not int or offset<4 or offset%4 or offset+12>0x1088:
            raise ValueError('invalid conversation member offset')
        field=metadata.get(hex(offset),{})
        if (field.get('defType')!=VECTOR or field.get('name')!=group.get('name')
                or field.get('verified') is False or not field.get('retailTransferCall')):
            raise ValueError('conversation member does not match native metadata')
        words={offset,offset+4,offset+8}
        if words&occupied or members&{offset+4,offset+8}:raise ValueError('overlapping conversation members')
        occupied|=words
        if not isinstance(records,list) or len(records)>LIMIT:raise ValueError('conversation records must be bounded')
        for row in records:
            if not isinstance(row,dict) or set(row)!=set(FIELDS):raise ValueError('conversation record fields mismatch')
            if type(row['Lines']) is not int or not 0<=row['Lines']<=LIMIT:raise ValueError('invalid conversation Lines')
            for name in STRINGS:
                items=row[name]
                if (not isinstance(items,list) or len(items)>LIMIT
                        or any(not isinstance(value,str) or '\0' in value or len(value.encode('utf-8'))>16384 for value in items)):
                    raise ValueError('invalid conversation string vector')
    return occupied

def decode_conversations(metadata):
    groups=[]
    for key,field in metadata.items():
        if field.get('defType')!=VECTOR:continue
        try:raw=bytes.fromhex(field['value'])
        except (KeyError,TypeError,ValueError) as exc:raise ValueError('invalid serialized conversation vector') from exc
        groups.append(dict(offset=int(key,0),name=field.get('name'),records=decode_records(raw)))
    validate(groups,metadata)
    return groups

def to_lua(lua,groups,metadata):
    validate(groups,metadata)
    out=lua.table()
    for group in groups:
        rows=lua.table()
        for index,row in enumerate(group['records'],1):
            item=lua.table(Lines=row['Lines'])
            for name in STRINGS:item[name]=lua.table_from(row[name])
            rows[index]=item
        out[group['offset']]=rows
    return out
