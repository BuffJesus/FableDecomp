from pathlib import Path
import sys,json
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent
from tools.script_recovery import native_reviewed_register_resource_health_lifetimes as M
from tools.script_recovery.test_native_reviewed_mans_resource_health_lifetimes import inputs,repin
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
LINE='            if cVar3 then local_presentedItemName = _G.g_PresentedItemName end'
BLOCK='            if cVar3 then\n                local_presentedItemName = _G.g_PresentedItemName\n            end'
def analysis(monkeypatch):
 captured=[];original=M.build_cfg
 def record(source):
  captured.append(source);return original(source)
 monkeypatch.setattr(M,'build_cfg',record)
 data=inputs();out,*_=M.recover(**data)
 assert LINE in out and BLOCK not in out
 assert len(captured)==1 and BLOCK in captured[0] and LINE not in captured[0]
 return captured[0]
def test_only_analysis_changes_and_offsets_remain_valid(monkeypatch):analysis(monkeypatch)
@pytest.mark.parametrize('condition',['false','nil','true','0',"'yes'"])
@pytest.mark.parametrize('value',['nil','false','0',"'LETTER'"])
def test_conditional_global_getter_trace(monkeypatch,condition,value):
 analysis(monkeypatch)
 results=[]
 for block in [LINE,BLOCK]:
  lua=LuaRuntime()
  code="local hits=0;local _G=setmetatable({}, {__index=function(_,key) assert(key=='g_PresentedItemName');hits=hits+1;return "+value+" end});local cVar3="+condition+";local local_presentedItemName='old';\n"+block+"\nreturn hits,local_presentedItemName"
  results.append(lua.execute(code))
 assert results[0]==results[1]
 assert results[0][0]==(0 if condition in ['false','nil'] else 1)
@pytest.mark.parametrize('fault',['indexed','call','multiple','expression','different_count'])
def test_unreviewed_inline_forms_reject(monkeypatch,fault):
 data=inputs()
 replacement={'indexed':LINE.replace('_G.g_PresentedItemName',"_G['g_PresentedItemName']"),'call':LINE.replace('_G.g_PresentedItemName','quest:GetName()'),'multiple':LINE.replace(' end','; other = 2 end'),'expression':LINE.replace('if cVar3','if cVar3 and other'),'different_count':LINE}[fault]
 data['source']=data['source'].replace(LINE,replacement)
 if fault=='different_count':data['row']['analysisInlineGlobalReads']=0
 repin(data)
 with pytest.raises(ValueError,match='analysis inline global reads'):M.recover(**data)
def test_false_branch_keeps_old_alias_live():
 source='function f(cVar3)\n local local_presentedItemName\n local_presentedItemName = nil\n'+BLOCK+'\n quest:Observe(local_presentedItemName)\nend'
 at=source.index('local_presentedItemName = nil')
 with pytest.raises(ValueError,match='live staging value read/escape'):prove_unused_staging(source,at,'local_presentedItemName')
