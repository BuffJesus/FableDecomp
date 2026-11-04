"""A branch may skip a string copy; later source writes cannot change its value."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter,load_manifest
from tools.script_recovery.test_lift_native_lua import NoRData

@pytest.mark.parametrize('choice',[-2147483648,-1,0,1,2,2147483647])
@pytest.mark.parametrize('construction',['empty','literal','default'])
@pytest.mark.parametrize('change_source',[False,True])
@pytest.mark.parametrize('first',['FIRST',''])
@pytest.mark.parametrize('copy_constructor',[False,True])
def test_branch_skipped_copy_retains_constructed_value(choice,construction,change_source,first,copy_constructor):
 initial='INITIAL_VALUE' if construction=='literal' else ''
 constructor='CCharString::CCharString(&xStack_8);' if construction=='default' else 'CCharString::CCharString(&xStack_8,"'+initial+'",-1);'
 source='''{
 @CONSTRUCT@
 iVar1 = QUESTSTATE_GetInt("Choice");
 if (iVar1 == 0) {
  pOther = "@FIRST@";
 }
 else {
  if (iVar1 == 1) {
   pOther = "SECOND";
  }
  else {
   goto LAB_00001000;
  }
 }
 CCharString::operator=((CCharString *)&xStack_8,pOther);
 @CHANGE@
 LAB_00001000:
 @CONSUME@
}'''.replace('@CONSTRUCT@',constructor).replace('@FIRST@',first).replace('@CHANGE@','pOther = "AFTER_COPY";' if change_source else '')
 source=source.replace('@CONSUME@','CCharString::CCharString(&xStack_10,&xStack_8);\n GSI->ObserveString(xStack_10);' if copy_constructor else 'GSI->ObserveString(xStack_8);')
 manifest=dict(load_manifest())
 manifest['ObserveString']={'scope':'Quest','returnType':'void','parameters':[{'name':'value','type':'const std::string&'}]}
 lifter=Lifter(manifest,{},'quest',False,'Pkg',NoRData(),native_gotos=True)
 lifter.accessor_kinds=True
 body='\n'.join(lifter.lift('Main',source))
 assert not lifter.todo,lifter.todo
 # Predeclare recovered locals just as the converter's function scope does.
 lua=LuaRuntime(unpack_returned_tuples=True)
 lua.globals().QUESTSTATE_GetInt=lambda name:choice
 runner=lua.execute('return function(quest) local xStack_8,pOther,iVar1,pCVar5\n'+body+'\nend')
 q=lua.execute('''return {
  GetStateInt=function(self,key)assert(key=='Choice');return self.choice end,
  ObserveString=function(self,text)self.text=text;self.calls=(self.calls or 0)+1 end
 }''')
 q['choice']=choice;runner(q)
 assert q['calls']==1
 assert q['text']==(first if choice==0 else 'SECOND' if choice==1 else initial)

@pytest.mark.parametrize('kind',['string','number','thing'])
@pytest.mark.parametrize('rewrite',[False,True])
def test_unmaterialized_copy_constructor_stays_explicit(kind,rewrite):
 manifest={
  'TestValue':{'scope':'Quest','returnType':{'string':'std::string','number':'int','thing':'std::shared_ptr<CScriptThing>'}[kind],'parameters':[]},
  'UseValue':{'scope':'Quest','returnType':'void','parameters':[{'name':'value','type':'sol::object'}]},
  'ObserveString':{'scope':'Quest','returnType':'void','parameters':[{'name':'value','type':'const std::string&'}]}}
 source='''{
 pCVar1 = GSI->TestValue();
 GSI->UseValue(pCVar1);
 CCharString::CCharString(&xStack_8,&pCVar1);
 @REWRITE@
 GSI->ObserveString(xStack_8);
}'''.replace('@REWRITE@','pCVar1 = "AFTER_COPY";' if rewrite else '')
 lifter=Lifter(manifest,{},'quest',False,'Pkg',NoRData(),native_gotos=True);lifter.accessor_kinds=True
 body='\n'.join(lifter.lift('Main',source))
 # Preserve existing recovery boundaries for other local values. Only a
 # modeled string slot changed by the conditional-copy fix gets the new path.
 assert any('CCharString::CCharString' in todo for todo in lifter.todo)
