"""Bound returned resource callbacks and preserve their frame/store semantics."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import build_readable_unit as readable
from tools.script_recovery import readable_callback_scopes as callbacks

@pytest.mark.parametrize('change',['extra_outer_statement','additional_parameter','same_parameter','generated_receiver','generated_parameter','nested_callback','environment','multiline_string','prefix_call','prefix_mutable','renamed_header'])
def test_unsupported_wrappers_or_generated_output_are_preserved(change):
 source='function Main(quest)\n    return quest:WithRetailResources(function(resources)\n    local x = 1\n    return x\n    end)\nend\n'
 convert=lambda s,**_: (s,{})
 if change=='extra_outer_statement':source=source.replace('    return quest:', '    quest:Mark()\n    return quest:')
 elif change=='additional_parameter':source=source.replace('Main(quest)','Main(quest, other)')
 elif change=='same_parameter':source=source.replace('function(resources)','function(quest)')
 elif change=='generated_receiver':source=source.replace('quest','r1')
 elif change=='generated_parameter':source=source.replace('resources','r2')
 elif change=='nested_callback':source=source.replace('local x = 1','local x = run(function() return 1 end)')
 elif change=='environment':source=source.replace('local x = 1','local _ENV = environment\n    local x = 1')
 elif change=='multiline_string':source=source.replace('local x = 1','local x = [[one\ntwo]]')
 elif change=='inline_store':source=source.replace('return x','record(x); x = 0; return x')
 elif change=='prefix_call':convert=lambda s,**_: ('record()\n'+s,{})
 elif change=='prefix_mutable':convert=lambda s,**_: ('local value = get()\n'+s,{})
 else:convert=lambda s,**_: (s.replace('function Main(quest, resources)','function Other(quest, resources)'),{})
 assert callbacks.rewrite(source,convert)==(source,{})

@pytest.mark.parametrize('error',[False,True])
def test_callback_execution_stays_at_call_site_with_outer_capture_and_multiple_results(error):
 source='function Main(quest)\n    return quest:WithRetailResources(function(resources)\n    quest:Record(resources)\n    if quest.failed then error("callback") end\n    return 17, false, quest.value\n    end)\nend\n'
 # The named function exists for analysis only. It must never be emitted or
 # called separately, and the callback receives exactly its original argument.
 result,reports=callbacks.rewrite(source,lambda s,**_: (s,{}))
 assert list(reports)==['Main']
 assert 'function Main(quest, resources)'not in result
 def run(text):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(text)
  return lua.execute('local seen={};local q={failed=...,value=23};function q:Record(r)assert(r=="owned");seen[#seen+1]="body"end;function q:WithRetailResources(f)seen[#seen+1]="scope";local out=table.pack(pcall(f,"owned"));seen[#seen+1]="close";if not out[1]then error(out[2])end;return table.unpack(out,2,out.n)end;local ok,a,b,c=pcall(Main,q);return ok,ok and a or tostring(a):match("callback")~=nil,b,c,table.concat(seen,",")',error)
 assert run(source)==run(result)


@pytest.mark.parametrize('body',[
 'local a=q:GetHero(); q:NewScriptFrame(); local b=q:GetHero(); return a,b',
 'q:NewScriptFrame(); local a=q:GetHero(); q:Pause(1); local b=q:GetHero(); return a,b',
 'if q.enabled then local a=q:GetHero(); local b=q:GetHero(); return a,b end; return 0,0',
 'local a=q:GetHero(); q:NewScriptFrame(); if q.enabled then return a,q:GetHero() end; return a,0',
 'local unused=q:GetHero(); q:NewScriptFrame(); return 1,1',
])
@pytest.mark.parametrize('enabled',[True,False])
@pytest.mark.parametrize('initial',[0,1])
def test_scoped_changing_hero_lookups_stay_at_original_time(body,enabled,initial):
 body='\n'.join('    '+line.strip()for line in body.replace(';','\n').splitlines())
 source='function Main(q)\n    return q:WithRetailResources(function(resources)\n'+body+'\n    end)\nend\n'
 after,report=readable.readable_file(source,frame_returns_alive=False)
 def run(text):
  lua=LuaRuntime(unpack_returned_tuples=True)
  lua.execute('q={calls=0,epoch='+str(initial)+',enabled='+str(enabled).lower()+'};function q:GetHero()self.calls=self.calls+1;if self.epoch==0 then return nil end;return self.epoch end;function q:NewScriptFrame()self.epoch=self.epoch+1 end;function q:Pause(_)self.epoch=self.epoch+1 end;function q:WithRetailResources(body)return body({})end')
  lua.execute(text);a,b=lua.globals().Main(lua.globals().q);return a,b,lua.globals().q['calls'],lua.globals().q['epoch']
 assert run(source)==run(after)

def test_false_frame_contract_does_not_invent_termination():
 source='function Main(q)\n    return q:WithRetailResources(function(resources)\n    q:NewScriptFrame()\n    local stopped = q:IsActiveThreadTerminating()\n    if stopped then return 0 end\n    return 7\n    end)\nend\n'
 after,_=readable.readable_file(source,frame_returns_alive=False)
 def run(text):
  lua=LuaRuntime();lua.execute(text)
  return lua.execute('local q={};function q:WithRetailResources(f)return f({})end;function q:NewScriptFrame()return false end;function q:IsActiveThreadTerminating()return false end;return Main(q)')
 assert run(source)==run(after)==7


def run(source,flag):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source)
 return lua.execute('local q={flag=...,events={}};function q:Record(x)self.events[#self.events+1]=tostring(x)end;function q:WithRetailResources(f)self.events[#self.events+1]="scope";local a,b=f({});self.events[#self.events+1]="close";return a,b end;local a,b=Main(q);return a,b,table.concat(q.events,",")',flag)

@pytest.mark.parametrize('parent_alive',[False,True])
@pytest.mark.parametrize('statement',[
 'q:Record(value); value = 1',
 'q:Record(value); value = 1; q:Record(value)',
 'value = 2; value = 1',
 'q:Record(value); value, other = 1, 2',
 'if q.flag then value = 1; return value, other end',
 'if q.flag then q:Record(value); value = 1; return value, other end',
])
def test_supported_compound_callback_stores_remain_executable(statement,parent_alive):
 source='function Main(q)\n    return q:WithRetailResources(function(resources)\n    local value, other\n    value = 0\n    '+statement+'\n    return value, other\n    end)\nend\n'
 after,report=readable.readable_file(source,frame_returns_alive=parent_alive)
 assert 'Main' in report['callbackScopes']
 for flag in [False,True]:assert run(source,flag)==run(after,flag)

@pytest.mark.parametrize('parent_alive',[False,True])
@pytest.mark.parametrize('frame_return',['nil','false','true'])
@pytest.mark.parametrize('stopping',[False,True])
def test_explicit_frame_check_survives_both_bridge_contracts(parent_alive,frame_return,stopping):
 source='function Main(q)\n    return q:WithRetailResources(function(resources)\n    q:NewScriptFrame()\n    local stopped = q:IsActiveThreadTerminating()\n    if stopped then return 0 end\n    return 7\n    end)\nend\n'
 after,report=readable.readable_file(source,frame_returns_alive=parent_alive)
 assert 'Main' in report['callbackScopes']
 assert ':IsActiveThreadTerminating()' in after and 'if not q:NewScriptFrame()'not in after
 def execute(text):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(text)
  return lua.execute('local q={frames=0,checks=0};function q:WithRetailResources(f)return f({})end;function q:NewScriptFrame()self.frames=self.frames+1;return '+frame_return+' end;function q:IsActiveThreadTerminating()self.checks=self.checks+1;return '+str(stopping).lower()+' end;local out=Main(q);return out,q.frames,q.checks')
 assert execute(source)==execute(after)==(0 if stopping else 7,1,1)

def test_callback_analysis_requests_conservative_frame_contract():
 from tools.script_recovery import readable_callback_scopes as callbacks
 seen=[]
 source='function Main(q)\n    return q:WithRetailResources(function(resources)\n    return 7\n    end)\nend\n'
 def convert(text,**options):seen.append(options['frame_returns_alive']);return text,{}
 output,report=callbacks.rewrite(source,convert,frame_returns_alive=True)
 assert seen==[False] and list(report)==['Main']

@pytest.mark.parametrize('parent_alive',[False,True])
@pytest.mark.parametrize('callback_first',[False,True])
def test_callback_policy_does_not_change_unrelated_function(parent_alive,callback_first):
 callback='function Main(q)\n    return q:WithRetailResources(function(resources)\n    q:NewScriptFrame()\n    if q:IsActiveThreadTerminating() then return end\n    q:Record(7)\n    end)\nend\n'
 ordinary='function Watch(q)\n    q:NewScriptFrame()\n    if q:IsActiveThreadTerminating() then return end\n    q:Record(8)\nend\n'
 source=callback+ordinary if callback_first else ordinary+callback
 after,report=readable.readable_file(source,frame_returns_alive=parent_alive)
 assert 'Main' in report['callbackScopes']
 expected,_=readable.readable_file(ordinary,frame_returns_alive=parent_alive)
 watch=after[after.index('function Watch('):]
 if not callback_first:watch=watch[:watch.index('function Main(')]
 assert watch.strip()==expected.strip()
 main=after[after.index('function Main('):]
 if callback_first:main=main[:main.index('function Watch(')]
 assert ':IsActiveThreadTerminating()' in main
 assert 'if not q:NewScriptFrame()' not in main
