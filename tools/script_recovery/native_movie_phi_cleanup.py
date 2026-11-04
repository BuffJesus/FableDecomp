import re


def fold(text, call_labels):
    """Release known movie handles before a shared base-destructor join."""
    releases=[name for name,target in call_labels.items() if target in (0x6e7ab0,0xce1000)]
    bases=[name for name,target in call_labels.items() if target==0x99a430]
    if not releases or not bases:return text
    release='(?:'+'|'.join(re.escape(n).replace('::',r'::\s*') for n in releases)+')'
    base='(?:'+'|'.join(re.escape(n).replace('::',r'::\s*') for n in bases)+')'
    pattern=re.compile(r'^(?P<ind>[ \t]*)'+release+r'\s*\(&(?P<member>\w+)\);[ \t]*\r?\n'
        r'[ \t]*(?P<obj>\w+)(?:\[0\]|\._0_4_)? = &PTR_[A-Za-z_]*_0126008c;[ \t]*\r?\n'
        r'[ \t]*(?P<phi>\w+) = \(\w+ \*\)&?(?P=obj);[ \t]*\r?\n',re.M)
    groups={}
    for m in pattern.finditer(text):groups.setdefault(m['phi'],[]).append(m)
    edits=[]
    for phi,matches in groups.items():
        uses=list(re.finditer(r'\b'+re.escape(phi)+r'\b',text))
        destructors=list(re.finditer(r'^[ \t]*'+base+r'\('+re.escape(phi)+r'\);[ \t]*\r?\n',text,re.M))
        declarations=list(re.finditer(r'^[ \t]*\w+ \*'+re.escape(phi)+r';[ \t]*\r?\n',text,re.M))
        if len(declarations)!=1 or not destructors or len(uses)!=len(matches)+len(destructors)+1:continue
        valid=True
        for m in matches:
            obj,member=m['obj'],m['member']
            a=re.fullmatch(r'(?:[A-Za-z]+Stack_|local_)([0-9a-f]+)',obj)
            b=re.fullmatch(r'(?:[A-Za-z]+Stack_|local_)([0-9a-f]+)',member)
            if not a or not b or int(a[1],16)-int(b[1],16)!=8:
                valid=False;break
            # The common base vtable alone cannot distinguish a resource/movie.
            ctor=re.compile(r'^[ \t]*'+re.escape(obj)+r'(?:\[0\]|\._0_4_)? = &PTR_[A-Za-z_]*_01260ef4;',re.M)
            if not ctor.search(text,0,m.start()):valid=False;break
        if not valid:continue
        for m in matches:edits.append((m.start(),m.end(),m['ind']+'RESOURCE_DestroyMovie('+m['obj']+');\n'))
        edits.extend((m.start(),m.end(),'') for m in destructors)
    for begin,end,value in sorted(edits,reverse=True):text=text[:begin]+value+text[end:]
    return text
