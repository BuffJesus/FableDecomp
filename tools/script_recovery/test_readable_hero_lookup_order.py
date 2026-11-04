from pathlib import Path
import importlib.util
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent
if (W/'candidate_bootstrap.py').exists():
 spec=importlib.util.spec_from_file_location('candidate_style',W/'readable_style.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
else:
 from tools.script_recovery import readable_style as m
@pytest.mark.parametrize('body',[
 'local a=q:GetHero(); q:NewScriptFrame(); local b=q:GetHero(); return a,b',
 'q:NewScriptFrame(); local a=q:GetHero(); q:Pause(1); local b=q:GetHero(); return a,b',
 'if q.enabled then local a=q:GetHero(); local b=q:GetHero(); return a,b end; return 0,0',
 'local a=q:GetHero(); q:NewScriptFrame(); if q.enabled then return a,q:GetHero() end; return a,0',
 'local unused=q:GetHero(); q:NewScriptFrame(); return 1,1',
])
@pytest.mark.parametrize('enabled',[True,False])
@pytest.mark.parametrize('initial',[0,1])
def test_lookup_timing_and_result(body,enabled,initial):
 source='function Main(quest)\n'+body.replace('q:','quest:').replace('q.enabled','quest.enabled').replace(';','\n')+'\nend\n'
 out,stats=m.style_function(source,set(),frame_returns_alive=False)
 def run(text):
  lua=LuaRuntime(unpack_returned_tuples=True);lua.execute('quest={calls=0,epoch='+str(initial)+',enabled='+str(enabled).lower()+'};function quest:GetHero()self.calls=self.calls+1;if self.epoch==0 then return nil end;return self.epoch end;function quest:NewScriptFrame()self.epoch=self.epoch+1 end;function quest:Pause(_)self.epoch=self.epoch+1 end');lua.execute(text);a,b=lua.globals().Main(lua.globals().quest);return a,b,lua.globals().quest['calls']
 assert run(out)==run(source)
