from pathlib import Path
import importlib.util,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent;from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_style as S
BODY='''    if condition then goto finish end
    literal_text()
    ::finish::
'''
BODY2='''    if condition then
        literal_then()
        goto finish
    end
    literal_else()
    ::finish::
'''

def value(source):
 vm=LuaRuntime();env=vm.table()
 vm.eval('function(s,e) setmetatable(e,{__index=_G});assert(load(s,"@literal","t",e))() end')(source,env)
 return env.F()

@pytest.mark.parametrize('eq',['','=', '=='])
@pytest.mark.parametrize('body',[BODY,BODY2])
def test_long_string_control_text_stays_literal_in_direct_and_full_pipeline(eq,body):
 source='function F()\n    local text = ['+eq+'[\n'+body+'    ]'+eq+']\n    return text\nend\n'
 lines=source.splitlines(True);assert S.fold_goto_else(lines)==(lines,0)
 after,stats=S.style_function(source,set())
 assert value(source)==value(after)
 assert stats.get('gotoElse',0)==0
 assert '['+eq+'[\n'+body+'    ]'+eq+']' in after

@pytest.mark.parametrize('eq',['','=', '=='])
@pytest.mark.parametrize('body',[BODY,BODY2])
def test_multiline_comment_control_text_and_annotations_stay_exact(eq,body):
 comment='--['+eq+'[\n'+body+'TODO preserve native evidence\n]'+eq+']'
 source='function F()\n    '+comment+'\n    return "unchanged"\nend\n'
 lines=source.splitlines(True);assert S.fold_goto_else(lines)==(lines,0)
 after,stats=S.style_function(source,set())
 assert comment in after and value(after)==value(source)=='unchanged'
 assert stats.get('gotoElse',0)==0

def test_escaped_physical_newline_string_stays_exact():
 literal='"start\\\n    if condition then goto finish end\\\n    ::finish::"'
 source='function F()\n    local text = '+literal+'\n    return text\nend\n'
 lines=source.splitlines(True);assert S.fold_goto_else(lines)==(lines,0)
 after,stats=S.style_function(source,set());assert value(after)==value(source)

@pytest.mark.parametrize('condition',[False,True,0,''])
def test_supported_single_line_control_flow_still_folds(condition):
 source='''function F(q)
    if q:Cond() then goto finish end
    q:Step()
    ::finish::
end
'''
 after,count=S.fold_goto_else(source.splitlines(True));after=''.join(after)
 assert count==1 and 'goto ' not in after
 vm=LuaRuntime(unpack_returned_tuples=True)
 run=vm.eval('''function(s,v)
  local env=setmetatable({},{__index=_G});assert(load(s,"@normal","t",env))()
  local calls=0;env.F({Cond=function() return v end,Step=function() calls=calls+1 end})
  return calls
 end''')
 assert run(source,condition)==run(after,condition)
