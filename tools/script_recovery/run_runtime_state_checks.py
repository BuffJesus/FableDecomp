"""Fresh x86 actual-code VM/state checks; optional ASan, no game execution."""
import argparse,hashlib,json,os,re,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def method(source,prefix):
    start=source.index(prefix);opening=source.index('{',start);depth=0
    for token in re.finditer(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|[{}]',source[opening:]):
        if token.group()=='{':depth+=1
        elif token.group()=='}':
            depth-=1
            if depth==0:return source[start:opening+token.end()]+'\n'
    raise ValueError('Unclosed method: '+prefix)

def run(args):
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    runtime=args.forge_root.resolve()/'FableScriptExtender';vendor=runtime.parent/'Vendor';lua=vendor/'lua'
    report={'passed':False,'scope':__doc__,'asan':args.asan,'commands':[]}
    try:
        manager=runtime/'LuaManager.cpp';source=manager.read_text(encoding='utf-8-sig')
        methods=[method(source,p) for p in ['void LuaManager::Initialize(', 'void LuaManager::Shutdown(',
            'void LuaManager::Reinitialize(', 'void LuaManager::ClearAllEntityData(', 'LuaManager& LuaManager::GetInstance(',
            'void LuaManager::RegisterEntityScriptData(', 'void LuaManager::UnregisterEntityScriptData(',
            'EntityScriptData* LuaManager::GetEntityScriptData(']]
        (out/'entity-manager-methods.inc').write_text('\n'.join(methods),encoding='utf-8')
        quest_cpp=runtime/'LuaQuestState.cpp';host_cpp=runtime/'LuaQuestHost.cpp'
        quest_source=quest_cpp.read_text(encoding='utf-8-sig');host_source=host_cpp.read_text(encoding='utf-8-sig')
        state_methods=[method(quest_source,p) for p in ['std::string LuaQuestState::GetNamespacedKey(',
            'void LuaQuestState::SetStateBool(', 'bool LuaQuestState::GetStateBool(', 'bool LuaQuestState::PersistTransferBool(']]
        state_methods += [method(source,p) for p in ['void LuaManager::SetGlobalStateBool(',
            'bool LuaManager::GetGlobalStateBool(', 'void LuaManager::SetGlobalStateInt(']]
        state_methods += [method(host_source,'const std::string& LuaQuestHost::GetScriptName(')]
        (out/'quest-state-methods.inc').write_text('\n'.join(state_methods),encoding='utf-8')
        registrations=[next(line for line in source.splitlines() if f'questState_type["{name}"] =' in line)
            for name in ('GetStateBool','SetStateBool','PersistTransferBool')]
        (out/'quest-state-registration.inc').write_text('\n'.join(registrations)+'\n',encoding='utf-8')
        for prefix,name in [('int LuaQuestState::StartAmbientConversation(','ambient-method.inc'),
            ('static CScriptThing* GetScriptThingFromSolObject(','conversation-helper.inc'),
            ('void LuaQuestState::AddLineToConversation(','conversation-method.inc')]:
            (out/name).write_text(method(quest_source,prefix),encoding='utf-8')
        ambient=next(line for line in source.splitlines() if 'questState_type["StartAmbientConversation"] =' in line)
        (out/'ambient-registration.inc').write_text(ambient+'\n',encoding='utf-8')
        first=source.index('    questState_type["AddLineToConversation"] = sol::overload(')
        last=source.index('    questState_type["IsConversationActive"]',first)
        (out/'conversation-registration.inc').write_text(source[first:last],encoding='utf-8')
        checks=Path(__file__).with_name('runtime_checks')
        sources=[p for p in sorted(lua.glob('*.c')) if p.name not in ('lua.c','luac.c')]
        package=ROOT/'refs/script_recovery/reconstructed/GuardianTrophyDealerInfo/FSE'
        files={Path(__file__),manager,quest_cpp,host_cpp,*runtime.glob('*.h'),*vendor.rglob('*.h'),*vendor.rglob('*.hpp'),
               checks/'entity_vm_lifetime.cpp',checks/'quest_state_isolation.cpp',checks/'retail_resources_smoke.cpp',
               checks/'ambient_conversation.cpp',checks/'conversation_lines.cpp',
               Path(__file__).with_name('run_meet_sister_runtime_checks.py'),*sources,
               *package.rglob('*.lua')}
        report['inputs']=[{'path':str(p.resolve()),'sha256':sha(p)} for p in sorted(files)]
        if args.conversation_cases:
            report['inputs'].append({'path':str(args.conversation_cases.resolve()),'sha256':sha(args.conversation_cases)})
        env=compiler_environment(find_vcvars())
        import shutil
        compiler=shutil.which('cl.exe',path=env['PATH']);assert compiler
        flags=['/fsanitize=address'] if args.asan else []
        def execute(name,argv,check=True):
            r=subprocess.run(argv,cwd=out,env=env,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True,errors='replace')
            (out/(name+'.log')).write_text(r.stdout,encoding='utf-8')
            report['commands'].append({'name':name,'argv':argv,'exit_code':r.returncode})
            if check and r.returncode:raise RuntimeError(name+' failed; see log')
            return r
        execute('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2',*flags,'/I'+str(lua),*map(str,sources)])
        objects=[str(out/(p.stem+'.obj')) for p in sources]
        execute('entity-vm-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',*flags,
            '/I'+str(runtime),'/I'+str(vendor),'/I'+str(lua),'/I'+str(out),
            str(checks/'entity_vm_lifetime.cpp'),*objects,'/Fe:entity-vm.exe'])
        outcomes=[]
        for scenario in ('unregister','clear','shutdown','reinitialize','replace','process_exit'):
            r=execute(scenario,[str(out/'entity-vm.exe'),scenario],False)
            outcomes.append(r.returncode==0 and 'PASS '+scenario in r.stdout)
        report['scenarios']=len(outcomes);report['passed']=all(outcomes)
        execute('quest-state-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',*flags,
            '/I'+str(runtime),'/I'+str(vendor),'/I'+str(lua),'/I'+str(out),
            str(checks/'quest_state_isolation.cpp'),*objects,'/Fe:quest-state.exe'])
        state=execute('quest-state',[str(out/'quest-state.exe'),str(package)],False)
        report['state_contract_groups']=state.stdout.count('PASS ')
        report['passed']=report['passed'] and state.returncode==0 and report['state_contract_groups']==3
        execute('ambient-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',*flags,
            '/I'+str(runtime),'/I'+str(vendor),'/I'+str(lua),'/I'+str(out),
            str(checks/'ambient_conversation.cpp'),*objects,'/Fe:ambient.exe'])
        ambient_result=execute('ambient',[str(out/'ambient.exe')],False)
        report['ambient_contract_groups']=ambient_result.stdout.count('PASS ')
        report['passed']=report['passed'] and ambient_result.returncode==0 and report['ambient_contract_groups']==3
        if args.conversation_cases:
            execute('conversation-lines-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',*flags,
                '/I'+str(runtime),'/I'+str(vendor),'/I'+str(lua),'/I'+str(out),
                str(checks/'conversation_lines.cpp'),*objects,'/Fe:conversation-lines.exe'])
            lines=execute('conversation-lines',[str(out/'conversation-lines.exe'),str(args.conversation_cases.resolve())],False)
            report['conversation_lines_passed']=lines.returncode==0 and lines.stdout.startswith('PASS:')
            report['passed']=report['passed'] and report['conversation_lines_passed']
        if any(sha(Path(p['path']))!=p['sha256'] for p in report['inputs']):raise RuntimeError('input drift during checks')
    except Exception as e:report.update(passed=False,error=str(e))
    report['outputs']=[{'path':p.name,'sha256':sha(p)} for p in sorted(out.iterdir()) if p.is_file()]
    (out/'result.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({k:report[k] for k in ('passed','asan','scenarios','error') if k in report}))
    return 0 if report['passed'] else 1

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--forge-root',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--asan',action='store_true')
    p.add_argument('--conversation-cases',type=Path,help='Generated existing conversation-line calls to run under this build')
    raise SystemExit(run(p.parse_args()))
