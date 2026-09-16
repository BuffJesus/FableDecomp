"""Collapse the verified intermittent-line predicate without changing polling."""
import hashlib
import re
from tools.script_recovery.readable_lua import tokens

CONDITION='''            timeRemaining = quest:GetTimer(quest:GetStateInt("TalkIntermittentTimer"))
            __native_condition_1 = timeRemaining == 0
            if __native_condition_1 then
                randomChoice = quest:RetailRandModulo(200)
                __native_condition_1 = randomChoice == 0
            end
            if __native_condition_1 then
'''
DIRECT='''            if quest:GetTimer(quest:GetStateInt("TalkIntermittentTimer")) == 0
                and quest:RetailRandModulo(200) == 0 then
'''

def readable_book_conditions(source):
    original=source
    names={'timeRemaining':3,'randomChoice':3,'__native_condition_1':5}
    if source.count(CONDITION)!=1:raise ValueError('BookTrader intermittent predicate changed')
    for name,count in names.items():
        if sum(t.lastgroup=='identifier' and t[0]==name for t in tokens(source))!=count:
            raise ValueError('BookTrader predicate temporary has additional uses: '+name)
        declaration=re.search(r'(?m)^ *local [^\n]*\b'+name+r'\b[^\n]*$',source)
        if declaration is None:raise ValueError('BookTrader predicate local missing')
    source=source.replace(CONDITION,DIRECT)
    for name in names:
        declaration=re.search(r'(?m)^ *local [^\n]*\b'+name+r'\b[^\n]*$',source)
        text=declaration[0]
        if name+', ' in text:text=text.replace(name+', ','')
        else:text=text.replace(', '+name,'')
        source=source[:declaration.start()]+text+source[declaration.end():]
    protected=[(t.start(),t.end()) for t in tokens(source) if t.lastgroup in ('comment','longcomment','string','longstring')]
    edits=[]
    for match in re.finditer(r'\(\(0x([01])\) ~= 0\)',source):
        if not any(a<match.end() and match.start()<b for a,b in protected):
            edits.append((match.start(),match.end(),'true' if match[1]=='1' else 'false'))
    for start,end,value in reversed(edits):source=source[:start]+value+source[end:]
    return source,{'removedPredicateLocals':list(names),'literalBooleanComparisons':len(edits),
        'beforeSha256':hashlib.sha256(original.encode()).hexdigest(),
        'afterSha256':hashlib.sha256(source.encode()).hexdigest(),
        'semantics':'Timer queried once; random queried only for exactly zero timer; cleanup and cancellation unchanged.',
        'limits':'Readable presentation of verified candidate; no runtime or operand gaps resolved.'}

if __name__=='__main__':
    import json
    from tools.script_recovery.generate_book_trader_resource_candidate import generate
    from tools.script_recovery.readable_book_trader import readable_book_source
    from tools.script_recovery.structure_book_trader_lua import structure_book
    from tools.script_recovery.lift_native_lua import ROOT
    candidate,_=generate();readable,_=readable_book_source(candidate)
    structured,_=structure_book(readable);output,report=readable_book_conditions(structured)
    folder=ROOT/'work/book_readability_20260913';folder.mkdir(exist_ok=True)
    (folder/'NOVI_BookTrader.lua').write_text(output)
    (folder/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))
