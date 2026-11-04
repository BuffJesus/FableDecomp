"""Model read-only pointers to evidenced scalar CString-array elements as keys.

Keep the field read at the CString consumer, including joins reached from several
array bases. A pointer is never treated as an early snapshot or an integer.
"""
import re

PARENT=r'\*\(int \*\)\(this \+ 0x14\)'
INDEX=r'(?:[A-Za-z_]\w*|\([A-Za-z_]\w* % (?:0x[0-9a-f]+|[1-9]\d*)\))'

def _mask(text):
    token=re.compile(r'/\*[\s\S]*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')
    return token.sub(lambda m:''.join('\n' if c=='\n' else ' ' for c in m[0]),text)

def lower_member_string_refs(text,spec):
    if not getattr(spec,'entity',False):
        families=[('QUEST',r'this',getattr(spec,'self_arrays',[]))]
    else:
        families=[('QUEST',PARENT,getattr(spec,'parent_arrays',[])),('ENTITY',r'this',getattr(spec,'self_arrays',[]))]
    arrays=[]
    for tag,base,items in families:
        for item in items:
            if item.get('element')!='CCharString' or item.get('stride')!=4 or item.get('members')!={0:('','String')} or item.get('count',0)<=0:
                continue
            off=item['base'];offset=f'(?:{hex(off)}|{off})'
            index=f'(?P<index>{INDEX})'
            shapes=[base+' \\+ '+offset+' \\+ '+index+' \\* 4',index+' \\* 4 \\+ '+offset+' \\+ '+base,base+' \\+ '+index+' \\* 4 \\+ '+offset]
            for shape in shapes:
                pattern=re.compile(r'^(?P<indent>[ \t]*)(?P<var>\w+) = \(CCharString(?:_bv)? \*\)\('+shape+r'\);[ \t]*$',re.M)
                for match in pattern.finditer(text):
                    arrays.append((match,tag,item['name'],match['index']))
    grouped={}
    for entry in arrays:grouped.setdefault(entry[0]['var'],[]).append(entry)
    edits=[];temporary=0;masked=_mask(text)
    for var,entries in grouped.items():
        # Ambiguous metadata or mixed state ownership cannot form one reference.
        spans=[entry[0].span() for entry in entries]
        if len(set(spans))!=len(spans) or len({entry[1] for entry in entries})!=1:continue
        escaped=re.escape(var)
        consumer=re.compile(r'^(?P<indent>[ \t]*)CCharString::operator=\((?P<dest>(?:\(CCharString \*\))?&?\w+),'+escaped+r'\);[ \t]*$',re.M)
        reads=list(consumer.finditer(text))
        if not reads:continue
        declarations=list(re.finditer(r'^[ \t]*CCharString(?:_bv)? \*'+escaped+r';[ \t]*$',masked,re.M))
        # Every occurrence must be a recognized definition, read, or declaration.
        if len(re.findall(r'\b'+escaped+r'\b',masked))!=len(entries)+len(reads)+len(declarations):continue
        for match,tag,name,index in entries:
            value=f'ENGINE_Concat("{name}_",ENGINE_IntToString({index}))'
            edits.append((match.start(),match.end(),f'{match["indent"]}{var} = {value};'))
        tag=entries[0][1]
        for match in reads:
            while True:
                temporary+=1;name=f'native_arg_member_string_value_{temporary}'
                if not re.search(r'\b'+name+r'\b',text):break
            indent=match['indent']
            replacement=f'{indent}{name} = {tag}STATE_GetString({var});\n{indent}CCharString::operator=({match["dest"]},{name});'
            edits.append((match.start(),match.end(),replacement))
    for start,end,replacement in sorted(edits,reverse=True):text=text[:start]+replacement+text[end:]
    return text
