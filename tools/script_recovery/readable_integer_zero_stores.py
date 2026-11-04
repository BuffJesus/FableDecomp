"""Opt-in dead integer stores, requiring an independently pinned review.

This models normal host integer returns, not current DLL or gameplay behavior.
The default is preservation. The compound-jump projection is analysis only;
the production Flow grammar and executable prefix statements are untouched.
"""
from collections import Counter
import hashlib
import json
import re
from tools.script_recovery import readable_style as S
from tools.script_recovery.lua_binding_guard import ambiguous_bindings
from tools.script_recovery.readable_lua import tokens

REVIEW_SHA256='68465a4a30072506ab93e67e4f5655bb37b2a3f9ca65b23bdd3590cf148c7123'
MANIFEST_SHA256='8ac14d1e83ad940ddc174ef80ba62d100c9a332e2876c190e0d2b76889f8dfdd'
REGISTRATION_SHA256='84f36267d691c56cc445c5b046dab24c6a4f29bca1f8b904d072ee4a13cf0303'
PHASE='after-final-naming-wrap-and-nil-only-pruning'
HAZARDS={'_ENV','debug','getfenv','setfenv','load','loadstring','getlocal','setlocal','getupvalue','setupvalue','upvaluejoin'}
COMPOUND=re.compile(r'(?P<indent>\s*)if (?P<condition>.+) then (?P<call>\w+(?:[.:]\w+)+\([^;]*\)); (?P<other>\w+) = (?P<value>\w+(?:[.:]\w+)+\([^;]*\)); goto (?P<label>\w+) end\s*')


class Refused(ValueError):
    pass


def require(condition,reason):
    if not condition:raise Refused(reason)


def digest(row):
    return hashlib.sha256(json.dumps(row,sort_keys=True,separators=(',',':')).encode()).hexdigest()


def methods(value,name):
    if isinstance(value,dict):
        if value.get('name')==name and 'returnType' in value:yield value
        for v in value.values():yield from methods(v,name)
    elif isinstance(value,list):
        for v in value:yield from methods(v,name)


def analyze(source,row):
    """Structural proof after trust gates; separately exercised with adverse Lua.

    Reject any unaccounted target token, closure, rebind or non-scalar definition.
    Only literal zero stores with no reachable read before redefinition qualify.
    """
    stream=list(tokens(source))
    ids=Counter(t[0] for t in stream if t.lastgroup=='identifier')
    require(not (ids.keys() & HAZARDS),'environment or introspection')
    require(not any(t.lastgroup in {'string','longstring','longcomment'} and '\n' in t[0] for t in stream),'multiline text')
    require(not ambiguous_bindings(source),'ambiguous bindings')
    require(ids['function']==1,'closure or nested function')
    lines=source.splitlines(True)
    require(lines and lines[0].strip()=='function '+row['function']+'(quest, me)','function header')
    original=S.Flow(lines)
    require(hasattr(original,'pinned'),'opaque structure')
    target=row['variable'];receiver=row['hostReceiver']
    declarations=Counter(S._declared_temporaries(original.structure))
    require(declarations[target]==1,'target declaration')
    require(declarations[receiver]==0,'receiver shadow')
    require(not S._reads_and_defs(original.structure,receiver)[0] and receiver not in original.pinned,'receiver rebind')
    require(target not in original.pinned,'captured or compound target')
    defs,reads=S._reads_and_defs(original.structure,target)
    require(ids[target]==1+len(defs)+len(reads),'unaccounted target token')
    require(len(reads)==row['expectedReads'],'read count')
    zero=[];host=[]
    for d in defs:
        line=original.structure[d]
        if re.fullmatch(r'\s*'+re.escape(target)+r' = 0\s*',line):zero.append(d)
        else:
            require(re.fullmatch(r'\s*'+re.escape(target)+r' = '+re.escape(receiver)+':'+re.escape(row['hostMethod'])+r'\(me, false, false\)\s*',line),'non-integer definition')
            host.append(d)
    require(len(zero)==row['expectedZeroStores'],'zero count')
    require(len(host)==row['expectedHostDefinitions'],'host count')
    view=list(lines);projections=[]
    for i,line in enumerate(original.structure):
        m=COMPOUND.fullmatch(line)
        if not m:continue
        require(target not in {t[0] for t in tokens(line) if t.lastgroup=='identifier'},'target in compound branch')
        require(m['other']!=receiver and declarations[m['other']]==1 and m['other'] in original.pinned,'unproven prefix destination')
        view[i]=m['indent']+'if '+m['condition']+' then goto '+m['label']+' end\n'
        projections.append(dict(line=i+1,original=lines[i].rstrip('\n'),analysisOnly=view[i].rstrip('\n'),pinnedLocal=m['other']))
    require(len(projections)==row['expectedTargetFreeCompoundJumps'],'projection count')
    flow=S.Flow(view)
    require(flow.ok,'unsupported control flow')
    require(S._reads_and_defs(flow.structure,target)==(defs,reads),'projection changed target uses')
    # Graph construction must resolve each jump to one unique original label.
    labels=[m[1] for line in flow.structure if (m:=re.fullmatch(r'\s*::(\w+)::\s*',line))]
    require(len(labels)==len(set(labels)),'duplicate labels')
    proofs=[]
    for d in zero:
        require(d not in reads and not S._read_reachable(flow,target,flow.graph.get(d,()),set(defs)),'live zero store')
        proofs.append(dict(line=d+1,statement=lines[d].strip(),unreadBeforeRedefinition=True))
    # Whole-function scalar-domain accounting: entry nil, literal zero or one
    # fixed normal host int result. There is no populated userdata to retire.
    after=list(lines)
    for d in zero:after[d]=''
    return ''.join(after),dict(variable=target,zeroStores=proofs,hostDefinitions=[d+1 for d in host],reads=[d+1 for d in reads],analysisProjections=projections,scalarDomain='entry nil, literal zero, fixed normal host integer return',userdataRetirement=False)


def prune(source,*,review=None,registration=None,manifest_bytes=None,rel=None,phase=None):
    """Only the pinned review/phase/registration/manifest can opt in."""
    if review is None:return source,None
    try:
        require(digest(review)==REVIEW_SHA256,'untrusted review')
        require(phase==PHASE==review['phase'] and rel==review['file'],'phase or file identity')
        require(hashlib.sha256(source.encode()).hexdigest()==review['phaseSha256'],'stale phase')
        require(manifest_bytes is not None and hashlib.sha256(manifest_bytes).hexdigest()==MANIFEST_SHA256,'stale manifest')
        manifest=json.loads(manifest_bytes)
        require(list(methods(manifest,review['hostMethod']))==[review['hostContract']],'host contract')
        require(review['fixedNormalHostIntegerReturnRequired'] is True and review['hostContract']['returnType']=='int','integer contract')
        require(registration is not None,'missing registration')
        require(digest(registration)==REGISTRATION_SHA256,'stale complete registration')
        entity=registration['entities'][review['owner']]
        require(entity['functions'][review['function']]['address'].lower()==review['address'],'native registration identity')
        require(registration['reviewedReadableIntegerZeroStores'][review['address']]==review,'registration review mismatch')
        result,proof=analyze(source,review)
        proof.update(status='proved_under_fixed_normal_host_integer_contract',phaseSha256=review['phaseSha256'],reviewSha256=REVIEW_SHA256,nativeImplementationVerified=False)
        return result,proof
    except (Refused,KeyError,TypeError,ValueError) as exc:
        return source,dict(status='preserved',reason=str(exc))
