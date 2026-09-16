"""Bounded wife presentation pass after verified cleanup structuring.

Uses Lua numeric comparison semantics, never Lua truthiness of zero. Native
operand verification stays in the candidate generator. No calls move or vanish.
"""
import hashlib
import re
from tools.script_recovery.readable_lua import tokens

def readable_wife_final(source):
    original=source
    callback_pattern=r'(?m)^(?P<i> *)local continueArgument = resources:WithArgumentKey[^\n]+\n[\s\S]*?^(?P=i)end\)\n'
    callback=re.search(callback_pattern,source)
    if callback is None:raise ValueError('Wife argument callback missing')
    callback_text=callback[0]
    stream=tokens(source)
    scratch=[t for t in stream if t.lastgroup=='identifier' and t[0]=='scratchValue']
    staged=re.search(r'(?m)^( *)scratchValue = wifeAnimationRemainder == 0\n\1if scratchValue then\n',source)
    declaration=re.search(r'(?m)^ *local [^\n]*\bscratchValue, ',source)
    if len(scratch)!=3 or staged is None or declaration is None:
        raise ValueError('wife scratch comparison definition/use changed')
    # Exactly one declaration, one definition and one immediate read. Its value
    # cannot escape or feed another branch; the comparison itself stays in place.
    source=source[:staged.start()]+staged[1]+'if wifeAnimationRemainder == 0 then\n'+source[staged.end():]
    declaration=re.search(r'(?m)^ *local [^\n]*\bscratchValue, ',source)
    source=source[:declaration.start()]+declaration[0].replace('scratchValue, ','')+source[declaration.end():]
    # Match only executable tokens. Do not fold a lookalike in diagnostics,
    # strings, or a comparison of an unknown numeric value.
    stream=tokens(source)
    protected=[(t.start(),t.end()) for t in stream if t.lastgroup in ('comment','longcomment','string','longstring')]
    changes=[]
    for match in re.finditer(r'\(\(0x([01])\) ~= 0\)',source):
        if not any(a<match.end() and match.start()<b for a,b in protected):
            changes.append((match.start(),match.end(),'true' if match[1]=='1' else 'false'))
    if not changes:raise ValueError('wife literal boolean staging missing')
    for start,end,value in reversed(changes):source=source[:start]+value+source[end:]
    after_callback=re.search(callback_pattern,source)
    if after_callback is None or after_callback[0]!=callback_text:
        raise ValueError('Wife argument callback changed during presentation')
    return source,{'callbackPreserved':True,'literalBooleanComparisons':len(changes),'removedScratchComparisons':1,
        'beforeSha256':hashlib.sha256(original.encode()).hexdigest(),
        'afterSha256':hashlib.sha256(source.encode()).hexdigest(),
        'limits':'Presentation only; native evidence and unresolved runtime/operand diagnostics remain authoritative.'}

if __name__=='__main__':
    import json
    from tools.script_recovery.generate_affair_wife_resource_candidate import generate
    from tools.script_recovery.structure_affair_wife_lua import structure_wife
    from tools.script_recovery.readable_affair_wife import readable_wife_source
    from tools.script_recovery.lift_native_lua import ROOT
    candidate,_=generate();structured,_=structure_wife(candidate)
    readable,_=readable_wife_source(structured);output,report=readable_wife_final(readable)
    folder=ROOT/'work/wife_readability_20260913';folder.mkdir(exist_ok=True)
    (folder/'NOVI_AffairWife.lua').write_text(output)
    (folder/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))

