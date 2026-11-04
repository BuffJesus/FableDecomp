import json
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import readable_compound_layout as P
from tools.script_recovery import build_readable_unit as B
from tools.script_recovery.readable_style import Flow,state_writers
W=Path(__file__).resolve().parent
from tools.script_recovery.script_units import ROOT as R

@pytest.mark.parametrize('condition',['quest:Ready()','not quest:Ready()','quest:Choose("then ; end")','quest:A() and quest:B()'])
@pytest.mark.parametrize('tail',['',' -- keep this ; goto text'])
def test_expands_exact_tokens(condition,tail):
    source='function Main(quest, me)\n    local v\n    if '+condition+' then quest:Frame(me); v = quest:Terminating(); goto again end'+tail+'\n    ::again::\nend\n'
    after,proof=P.expand(source)
    assert len(proof)==1 and proof[0]['tokensExact'] and proof[0]['semicolonsRetained']
    assert P.code_tokens(source)==P.code_tokens(after)
    assert after.count(';')==source.count(';')
    assert Flow(after.splitlines(True)).ok
    LuaRuntime().compile(after)

@pytest.mark.parametrize('source',[
    'if ready then q:Frame(); v = q:End(); goto again end',
    '-- if ready then q:Frame(); v = q:End(); goto again end\n',
    'local s = "if ready then q:Frame(); v = q:End(); goto again end"\n',
    'local s = [[if ready then q:Frame(); v = q:End(); goto again end]]\n',
    'local s = [[a\nb]]\nif ready then q:Frame(); v = q:End(); goto again end\n',
    'if ready then q:Frame(); v = q:End(); other(); goto again end\n',
    'if ready then q:Frame(); v, other = q:End(); goto again end\n',
    'if ready then q:Frame(); local v = q:End(); goto again end\n',
    'if ready then q:Frame(); q.v = q:End(); goto again end\n',
    'if ready then q:Frame(); v = 0; goto again end\n',
    'if ready then q:Frame(); v = q:End(); return end\n',
    'if ready then q:Frame(); -- comment hides rest v = q:End(); goto again end\n',
    'if ready then q:Frame(); v = q:End(); goto again end; q:After()\n',
])
def test_unsupported_preserves(source):
    assert P.expand(source)==(source,[])

def test_literal_arguments_and_comments_exact():
    source='if q:Check("a; then", [[x]]) then q:Frame("; goto end"); value = q:Check("; then"); goto again end -- c\n'
    after,proof=P.expand(source)
    assert len(proof)==1
    assert [t[0] for t in P.tokens(source) if t.lastgroup!='space']==[t[0] for t in P.tokens(after) if t.lastgroup!='space']

def test_semicolon_retention_blocks_call_chaining():
    source='function Main(q, me)\n    local v\n    if q:Check() then q:Frame(); v = q:Get(); goto again end\n    ::again::\n    return v\nend\n'
    after,_=P.expand(source)
    assert 'q:Frame();\n' in after and 'v = q:Get();\n' in after
    run=LuaRuntime().eval('''function(s, choice, fail)
        local events={};local env={};setmetatable(env,{__index=_G})
        assert(load(s,"layout","t",env))()
        local q={Check=function() events[#events+1]="check";return choice end,
                 Frame=function() events[#events+1]="frame";if fail then error("frame stopped",0) end end,
                 Get=function() events[#events+1]="get";return 71 end}
        local ok,result=pcall(env.Main,q,{})
        return tostring(ok)..":"..tostring(result)..":"..table.concat(events,",")
    end''')
    for choice in [False,True]:
        for fail in [False,True]:assert run(source,choice,fail)==run(after,choice,fail)

def test_beardy_supported_graph_without_grammar_change():
    draft=R/'refs/script_recovery/lifted/BeardyBaldy/draft'
    sources={p.relative_to(draft).as_posix():p.read_text() for p in draft.rglob('*.lua')}
    rel='FSE/V_BeardyBaldy/Entities/BB_BeardyBaldyMan.lua'
    source,_=B.readable_file(sources[rel],rel=rel,writers=state_writers(sources))
    before=next(c for c in B.FUNCTION_SPLIT.split(source) if c.startswith('function Main('))
    assert not Flow(before.splitlines(True)).ok
    after,proof=P.expand(before)
    assert len(proof)==1 and Flow(after.splitlines(True)).ok
    assert P.code_tokens(before)==P.code_tokens(after)
    assert after.count('addNewConversation = 0')==before.count('addNewConversation = 0')==24

def test_builder_guard_survives_and_style_off_preserves():
    draft=R/'refs/script_recovery/lifted/BeardyBaldy/draft'
    sources={p.relative_to(draft).as_posix():p.read_text() for p in draft.rglob('*.lua')}
    registration=json.loads((R/'refs/script_recovery/beardy_baldy/units/V_BeardyBaldy.json').read_text())
    rel='FSE/V_BeardyBaldy/Entities/BB_BeardyBaldyMan.lua'
    kw=dict(rel=rel,writers=state_writers(sources),integer_registrations=[registration],integer_manifest_bytes=(R/'refs/fse_api_manifest.json').read_bytes())
    after,report=B.readable_file(sources[rel],**kw)
    assert report['reviewedIntegerZeroStores']['Main']['status']=='proved_under_fixed_normal_host_integer_contract'
    assert len(P.expand(after)[1])==4
    assert 'expandedCompoundBranches' not in report
    assert 'addNewConversation = 0' not in after
    off,report=B.readable_file(sources[rel],style=False,**kw)
    assert 'expandedCompoundBranches' not in report

@pytest.mark.parametrize('hazard',sorted(P.HAZARDS))
def test_introspection_and_environment_preserve(hazard):
    source='function Main(q, me)\n    '+hazard+'()\n    if q:Check() then q:Frame(); v = q:Get(); goto again end\n    ::again::\nend\n'
    assert P.expand(source)==(source,[])

@pytest.mark.parametrize('expression',['',' ""',' 71',' (q:Get("then ; end"))'])
def test_terminal_prefix_keeps_explicit_return_edge(expression):
    source='function Main(q, me)\n    if q:Check() then do return'+expression+' end; goto again end\n    ::again::\n    return 99\nend\n'
    after,proof=P.expand(source)
    assert len(proof)==1 and proof[0]['kind']=='return-jump'
    assert P.code_tokens(source)==P.code_tokens(after)
    flow=Flow(after.splitlines(True));assert flow.ok
    at=next(i for i,l in enumerate(flow.structure) if l.strip().startswith('do return'))
    assert flow.graph[at]==set()
    LuaRuntime().compile(after)
