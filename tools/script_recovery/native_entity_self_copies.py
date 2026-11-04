"""Preserve a retained entity-self copy before vtable-only constructor folding."""
import re


COPY = re.compile(
    r'^(?P<ind>[ \t]*)(?P<obj>\w+)\._4_4_ = \*\(undefined4 \*\)\(this \+ 0xc\);\s*'
    r'(?P<info>\w+) = \*\(int \*\*\)\(this \+ 0x10\);\s*'
    r'(?P<flags>(?:\w+ = (?:true|false);\s*){0,3})'
    r'(?P=obj)\._0_4_ = &PTR_[A-Za-z_]*_01238c8c;\s*'
    r'if \((?P=info) != \(int \*\)0x0\) \{\s*'
    r'\*(?P=info) = \*(?P=info) \+ 1;\s*\}', re.M)


def fold(text):
    def replace(match):
        obj, info = match['obj'], match['info']
        offsets = [re.search(r'(?:Stack_|local_)([0-9a-f]+)$', name) for name in (obj,info)]
        if not all(offsets) or int(offsets[0][1],16)-int(offsets[1][1],16) != 8:
            return match[0]
        if not re.search(r'\(CScriptThing(?:_bv)? \*\)&?'+re.escape(obj)+r'\b',text):
            return match[0]
        if re.search(r'\b(?:'+re.escape(obj)+'|'+re.escape(info)+r')\s*=',match['flags']):
            return match[0]
        tail=text[match.end():]
        next_store=re.search(r'^[ \t]*'+re.escape(info)+r'\s*=(?!=)',tail,re.M)
        if re.search(r'\b'+re.escape(info)+r'\b',tail[:next_store.start()] if next_store else tail):
            return match[0]
        flags=''.join(match['ind']+line.strip()+'\n' for line in match['flags'].splitlines() if line.strip())
        return f'{match["ind"]}{obj} = (CScriptThing *)(this + 8);\n'+flags.rstrip('\n')
    return COPY.sub(replace,text)
