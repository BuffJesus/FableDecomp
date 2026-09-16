"""Remove literal argument scaffolding from the two verified woman speech sites."""
import hashlib
import re
from tools.script_recovery.readable_lua import tokens

def readable_woman_final(source):
    original=source
    protected=[(t.start(),t.end()) for t in tokens(source) if t.lastgroup in ('comment','longcomment','string','longstring')]
    pattern=r'(?m)^(?P<indent> *)speechResult(?P<suffix>2?) = resources:Speak\(woman_resource, hero(?P=suffix), \("TEXT_QST_048_AFFAIRWOMAN_(?P<key>ON_HIT|BUSY)"\), \(0x0\), \(\(0x0\) ~= 0\), \(\(0x1\) ~= 0\), \(false\)\)$'
    matches=[m for m in re.finditer(pattern,source) if not any(a<=m.start()<b for a,b in protected)]
    if len(matches)!=2 or {m['key'] for m in matches}!={'ON_HIT','BUSY'}:
        raise ValueError('Woman verified speech literal sites changed')
    for m in reversed(matches):
        replacement=(m['indent']+'speechResult'+m['suffix']+' = resources:Speak(woman_resource, hero'+m['suffix']+', '
                     +'"TEXT_QST_048_AFFAIRWOMAN_'+m['key']+'", 0, false, true, false)')
        source=source[:m.start()]+replacement+source[m.end():]
    return source,{'speechSites':2,'literalBooleanComparisons':4,
        'beforeSha256':hashlib.sha256(original.encode()).hexdigest(),
        'afterSha256':hashlib.sha256(source.encode()).hexdigest(),
        'limits':'Only literal speech arguments change presentation. Call/result storage, helpers, cleanup and diagnostics stay unchanged.'}

if __name__=='__main__':
    import json
    from tools.script_recovery.generate_affair_woman_resource_candidate import generate
    from tools.script_recovery.readable_affair_woman import readable_woman_source
    from tools.script_recovery.structure_affair_woman_lua import structure_woman
    from tools.script_recovery.lift_native_lua import ROOT
    candidate,_=generate();readable,_=readable_woman_source(candidate)
    structured,_=structure_woman(readable);output,report=readable_woman_final(structured)
    folder=ROOT/'work/woman_readability_20260913';folder.mkdir(exist_ok=True)
    (folder/'NOVI_AffairWoman.lua').write_text(output)
    (folder/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))
