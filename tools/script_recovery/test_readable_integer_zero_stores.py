from copy import deepcopy
import json
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import readable_integer_zero_stores as P
from tools.script_recovery import build_readable_unit as B
from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery.readable_style import state_writers
W=Path(__file__).resolve().parent
ROW=json.loads((W/'testdata/beardy_integer_zero_store_review.json').read_text())
UNIT=json.loads((R/'refs/script_recovery/beardy_baldy/units/V_BeardyBaldy.json').read_text())
UNIT['reviewedReadableIntegerZeroStores']={ROW['address']:deepcopy(ROW)}
MANIFEST=(R/'refs/fse_api_manifest.json').read_bytes()
DRAFT=R/'refs/script_recovery/lifted/BeardyBaldy/draft'
SOURCES={p.relative_to(DRAFT).as_posix():p.read_text(encoding='utf-8') for p in DRAFT.rglob('*.lua')}
PHASE_FILE,_=B.readable_file(SOURCES[ROW['file']],rel=ROW['file'],writers=state_writers(SOURCES))
SOURCE=next(c for c in B.FUNCTION_SPLIT.split(PHASE_FILE) if c.startswith('function Main('))

def kwargs():
    return dict(review=deepcopy(ROW),registration=deepcopy(UNIT),manifest_bytes=MANIFEST,rel=ROW['file'],phase=P.PHASE)

def test_exact_proof():
    after,proof=P.prune(SOURCE,**kwargs())
    assert proof['status']=='proved_under_fixed_normal_host_integer_contract'
    assert len(proof['zeroStores'])==24
    lines=SOURCE.splitlines(True)
    removed={r['line']-1 for r in proof['zeroStores']}
    assert after==''.join(l for i,l in enumerate(lines) if i not in removed)
    assert len(proof['analysisProjections'])==1
    assert proof['analysisProjections'][0]['original'] in after
    assert after.count('quest:AddNewConversation(')==1
    assert after.count('addNewConversation')==4
    LuaRuntime().compile(after)

def test_default_preserves():
    assert P.prune(SOURCE)==(SOURCE,None)

@pytest.mark.parametrize('key,value',[
    ('returnType','CScriptThing*'),('returnType','bool'),('returnType','float'),
    ('scope','Entity'),('blocking',True),('name','ForgedIntegerApi')])
def test_forged_contract(key,value):
    kw=kwargs();kw['review']['hostContract'][key]=value
    assert P.prune(SOURCE,**kw)[1]['reason']=='untrusted review'

@pytest.mark.parametrize('key,value',[
    ('phaseSha256','0'*64),('variable','resource'),('hostReceiver','me'),
    ('address','0x00e50fd1'),('owner','Other'),('function','Init'),
    ('expectedZeroStores',23),('expectedReads',1),
    ('nativeImplementationVerified',True),('fixedNormalHostIntegerReturnRequired',False)])
def test_forged_review(key,value):
    kw=kwargs();kw['review'][key]=value
    assert P.prune(SOURCE,**kw)[0]==SOURCE
    assert P.prune(SOURCE,**kw)[1]['reason']=='untrusted review'

@pytest.mark.parametrize('which',[
    'file','phase','manifest_missing','manifest_changed','registration_missing',
    'owner','address','row','row_missing'])
def test_stale_identity(which):
    kw=kwargs()
    if which=='file':kw['rel']='other.lua'
    if which=='phase':kw['phase']='before-naming'
    if which=='manifest_missing':kw['manifest_bytes']=None
    if which=='manifest_changed':kw['manifest_bytes']=MANIFEST+b' '
    if which=='registration_missing':kw['registration']=None
    if which=='owner':del kw['registration']['entities'][ROW['owner']]
    if which=='address':kw['registration']['entities'][ROW['owner']]['functions']['Main']['address']='0x1234'
    if which=='row':kw['registration']['reviewedReadableIntegerZeroStores'][ROW['address']]['expectedReads']=9
    if which=='row_missing':del kw['registration']['reviewedReadableIntegerZeroStores']
    out,proof=P.prune(SOURCE,**kw)
    assert out==SOURCE and proof['status']=='preserved'

@pytest.mark.parametrize('old,new',[
    ('addNewConversation = 0','addNewConversation = {}'),
    ('addNewConversation = 0','addNewConversation = resource'),
    ('addNewConversation = 0','addNewConversation = quest:NewResource()'),
    ('addNewConversation = 0','addNewConversation = nil'),
    ('addNewConversation = 0','addNewConversation = false'),
    ('addNewConversation = 0','addNewConversation = 0\n                quest:Use(addNewConversation)'),
    ('scratchValue = quest:IsActiveThreadTerminating(); goto continue_3', 'scratchValue = quest:Use(addNewConversation); goto continue_3'),
    ('scratchValue = quest:IsActiveThreadTerminating(); goto continue_3', 'addNewConversation = quest:IsActiveThreadTerminating(); goto continue_3'),
    ('scratchValue = quest:IsActiveThreadTerminating(); goto continue_3', 'unproven = quest:IsActiveThreadTerminating(); goto continue_3'),
    ('scratchValue = quest:IsActiveThreadTerminating(); goto continue_3', 'quest = quest:IsActiveThreadTerminating(); goto continue_3'),
    ('goto continue_3 end','goto missing_label end'),
    ('    scratchValue = false','    quest = other'),
    ('    scratchValue = false','    local quest = other'),
    ('    scratchValue = false','    local function capture()\n        return addNewConversation\n    end'),
    ('    scratchValue = false','    local addNewConversation = 0'),
    ('    scratchValue = false','    debug.getlocal(1, 1)'),
    ('    scratchValue = false','    local _ENV = other'),
    ('    scratchValue = false','    load("return 1")()'),
    ('    scratchValue = false','    getfenv()'),
    ('    scratchValue = false','    local text = [[a\nb]]'),
    ('    scratchValue = false','    addNewConversation, scratchValue = 0, false'),
    ('    scratchValue = false','    quest.addNewConversation = 1'),
    ('    scratchValue = false','    for i=1,2 do\n    end'),
])
def test_structural_adverse_controls(old,new):
    assert old in SOURCE
    source=SOURCE.replace(old,new,1)
    assert P.prune(source,**kwargs())[0]==source
    with pytest.raises(P.Refused):P.analyze(source,ROW)

@pytest.mark.parametrize('body',[
    '    x = 0\n    quest:Use(x)\n',
    '    repeat\n        x = 0\n        if quest:Again() then goto read end\n        x = quest:AddNewConversation(me, false, false)\n        ::read::\n        quest:Use(x)\n    until quest:Done()\n',
    '    x = 0\n    if quest:Choose() then\n        x = quest:AddNewConversation(me, false, false)\n    end\n    quest:Use(x)\n'])
def test_cfg_live_zero_rejects(body):
    source='function Main(quest, me)\n    local x\n'+body+'end\n'
    row=dict(ROW,variable='x',expectedZeroStores=1,expectedReads=1,
        expectedHostDefinitions=body.count('x = quest:'),expectedTargetFreeCompoundJumps=0)
    with pytest.raises(P.Refused,match='live zero store'):P.analyze(source,row)

def test_scalar_paths_preserve_observations():
    source='''function Main(quest, me)
    local x
    x = quest:AddNewConversation(me, false, false)
    quest:Use(x)
    x = 0
    if quest:Choose() then
        x = quest:AddNewConversation(me, false, false)
        quest:Use(x)
    end
end
'''
    row=dict(ROW,variable='x',expectedZeroStores=1,expectedReads=2,expectedHostDefinitions=2,expectedTargetFreeCompoundJumps=0)
    after,proof=P.analyze(source,row)
    vm=LuaRuntime(unpack_returned_tuples=True)
    run=vm.eval('''function(source, n, choice)
        local events={}
        local q={AddNewConversation=function() events[#events+1]="create"; return n end,
                 Use=function(_,x) events[#events+1]=tostring(x) end,
                 Choose=function() events[#events+1]="choose"; return choice end}
        local env={}; setmetatable(env,{__index=_G})
        assert(load(source,"candidate","t",env))()
        env.Main(q,{})
        return table.concat(events,",")
    end''')
    for n in [-2147483648,-1,0,1,2147483647]:
        for choice in [True,False]:assert run(source,n,choice)==run(after,n,choice)

def test_builder_default_and_style_off():
    from tools.script_recovery.readable_style import state_writers
    draft=R/'refs/script_recovery/lifted/BeardyBaldy/draft'
    sources={p.relative_to(draft).as_posix():p.read_text(encoding='utf-8') for p in draft.rglob('*.lua')}
    rel=ROW['file'];kw=dict(rel=rel,writers=state_writers(sources))
    default,_=B.readable_file(sources[rel],**kw)
    assert default==PHASE_FILE
    base,_=B.readable_file(sources[rel],style=False,**kw)
    off,_=B.readable_file(sources[rel],style=False,integer_registrations=[UNIT],integer_manifest_bytes=MANIFEST,**kw)
    assert base==off

@pytest.mark.parametrize('key,value',[
    ('script','OtherScript'),('package','OtherPackage'),('nativeClass','OtherClass'),
    ('allocator','0x1234'),('vtable','0x5678'),('schema','stale'),
    ('unreviewedExtraField',True)])
def test_complete_registration_pin(key,value):
    kw=kwargs();kw['registration'][key]=value
    out,proof=P.prune(SOURCE,**kw)
    assert out==SOURCE
    assert proof['reason']=='stale complete registration'
