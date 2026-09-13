"""Build staged-port/resource checks from source in a new directory (Windows x86).

Requires MSVC and the runtime checkout's vendor Lua/sol sources; no prebuilt
objects, installed game changes, or engine execution. The engine is mocked.
"""
import argparse
import hashlib
import json
import os
import re
import shutil
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
CHECKS = Path(__file__).with_name('runtime_checks')
FACADE = r'''
local quest = {}
function quest:WithRetailResources(f) with_resources(f) end
function quest:IsRegionLoaded(name) return region(name) end
function quest:IsActiveThreadTerminating() return terminating() end
function quest:GetHero() return hero() end
function quest:RemoveThing(actor, a, b)
    assert(actor == me)
    record('remove|bound-instance-not-a-name-lookup|' .. tostring(a) .. '|' .. tostring(b))
end
function quest:GetActiveQuestName() record('active'); return 'QS_MeetSister' end
function quest:SetQuestAsCompleted(name,a,b,c)
    record('complete|' .. name .. '|' .. tostring(a) .. '|' .. tostring(b) .. '|' .. tostring(c))
end
function quest:DeactivateQuestLater(name,delay) record('deactivate|' .. name .. '|' .. tostring(delay)) end
function quest:FadeScreenIn() record('fade.in') end
'''


def replay_source(cases, package):
    lines = [f'package.path = {json.dumps(package.as_posix() + "/?.lua;")} .. package.path', FACADE]
    for case in cases:
        options = ','.join(k + '=' + json.dumps(v) for k, v in case['options'].items())
        path = package / 'MeetSister/Entities' / ('MeetSister' + case['entry'].title() + '.lua')
        expected = ','.join(json.dumps('|'.join(row)) for row in case['events'])
        lines += ['reset_case({' + options + '})', f'dofile({json.dumps(path.as_posix())})',
                  'Init(quest, me)', 'Main(quest, me)',
                  f'verify({json.dumps(case["entry"] + "/" + case["scenario"])}, {{{expected}}})']
    return '\n'.join(lines) + '\n'


def compiler_environment(vcvars):
    # Only this fixed environment setup uses cmd. Compiler/test invocations use
    # argument lists directly; no user-controlled shell command concatenation.
    path = str(vcvars.resolve())
    if any(char in path for char in '%!^&|<>"\r\n'):
        raise ValueError('Unsupported shell metacharacter in vcvars path')
    # cmd uses its own quotation rules; list2cmdline would introduce literal
    # backslashes before the embedded quotes in this /c command.
    command = f'cmd.exe /d /s /c "call "{path}" >nul && set"'
    result = subprocess.run(command,
                            capture_output=True, text=True, check=True)
    env = {key.upper(): value for key, value in os.environ.items()}
    for line in result.stdout.splitlines():
        if '=' in line and not line.startswith('='):
            key, value = line.split('=', 1)
            env[key.upper()] = value
    return env


def find_vcvars():
    vswhere = Path(os.environ.get('ProgramFiles(x86)', 'C:/Program Files (x86)')) / (
        'Microsoft Visual Studio/Installer/vswhere.exe')
    result = subprocess.run([str(vswhere), '-latest', '-products', '*', '-requires',
                             'Microsoft.VisualStudio.Component.VC.Tools.x86.x64',
                             '-property', 'installationPath'], capture_output=True,
                            text=True, check=True)
    if not result.stdout.strip():
        raise RuntimeError('No MSVC x86 installation found; specify --vcvars')
    return Path(result.stdout.strip()) / 'VC/Auxiliary/Build/vcvars32.bat'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def lua_arguments(text):
    """Split this inventory's one-line calls without splitting nested calls/strings."""
    parts=[];start=0;depth=0;quote=None;escaped=False
    for i,char in enumerate(text):
        if quote:
            if escaped:escaped=False
            elif char=='\\':escaped=True
            elif char==quote:quote=None
        elif char in ('"',"'"):quote=char
        elif char in '({[':depth+=1
        elif char in ')}]':depth-=1
        elif char==',' and depth==0:parts.append(text[start:i].strip());start=i+1
    if quote or depth:raise ValueError('Unbalanced conversation expression')
    parts.append(text[start:].strip())
    return parts


def run(args):
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)  # Never reuse/delete prior products.
    report = {'passed': False, 'scope': __doc__, 'commands': [], 'inputs': []}
    try:
        forge = args.forge_root.resolve()
        vendor = forge / 'Vendor'
        lua = vendor / 'lua'
        runtime = forge / 'FableScriptExtender'
        quest_cpp = runtime / 'LuaQuestState.cpp'
        quest_source = quest_cpp.read_text(encoding='utf-8-sig')
        start = quest_source.index('void LuaQuestState::SetThingAsUsable(')
        end = quest_source.index('\n}', start) + 2
        (output / 'usable-method.inc').write_text(quest_source[start:end] + '\n', encoding='utf-8')
        for prefix,name in [('static CScriptThing* GetScriptThingFromSolObject(','conversation-helper.inc'),
                            ('void LuaQuestState::AddLineToConversation(','conversation-method.inc')]:
            start=quest_source.index(prefix);end=quest_source.index('\n}',start)+2
            (output/name).write_text(quest_source[start:end]+'\n',encoding='utf-8')
        start=quest_source.index('bool LuaQuestState::PersistTransferBool(')
        end=quest_source.index('\n}',start)+2
        (output/'persist-method.inc').write_text(quest_source[start:end]+'\n',encoding='utf-8')
        start=quest_source.index('void LuaQuestState::AddEntityBinding(')
        end=quest_source.index('\n}',start)+2
        (output/'binding-method.inc').write_text(quest_source[start:end]+'\n',encoding='utf-8')
        maze_package=ROOT/'refs/script_recovery/reconstructed/MazeResearch/FSE'
        persist_proof=ROOT/'refs/script_recovery/maze_research/runtime_evidence/native-port-persistence-20260912.json'
        persist_cases=json.loads(persist_proof.read_text())['cases']
        def lua_bool(value):return 'true' if value else 'false'
        def lua_fields(fields):return '{'+','.join('['+json.dumps(k)+']='+lua_bool(v) for k,v in fields.items())+'}'
        persist_lines=[f'package.path={json.dumps(maze_package.as_posix()+"/?.lua;")}..package.path',
                       f'dofile({json.dumps((maze_package/"MazeResearch/MazeResearch.lua").as_posix())})']
        for case in persist_cases:
            if case['entry']!='0xea80a0':continue
            a,b=map(lua_bool,case['initial']);x,y=map(lua_bool,case['final'])
            persist_lines += [f'reset_case({a},{b},{lua_bool(case["mode"]=="read")},{lua_fields(case["stored"])})',
                              'OnPersist(quest,context)',f'verify_case({x},{y},{lua_fields(case["storage"])})']
        guardian_package=ROOT/'refs/script_recovery/reconstructed/GuardianTrophyDealerInfo/FSE'
        guardian_persist_proof=ROOT/'refs/script_recovery/guardian_trophy_dealer_info/runtime_evidence/native-persistence-cases-20260912.json'
        guardian_persist_cases=json.loads(guardian_persist_proof.read_text())['cases']
        if len(guardian_persist_cases)!=8:raise ValueError('Review changed Guardian persistence inventory')
        persist_lines += [f'package.path={json.dumps(guardian_package.as_posix()+"/?.lua;")}..package.path',
                          f'dofile({json.dumps((guardian_package/"GuardianTrophyDealerInfo/GuardianTrophyDealerInfo.lua").as_posix())})']
        for case in guardian_persist_cases:
            persist_lines += [f'reset_guardian({lua_bool(case["initial"][0])},{lua_bool(case["mode"]=="read")},{lua_fields(case["stored"])})',
                              'OnPersist(quest,context)',f'verify_guardian({lua_bool(case["final"][0])},{lua_fields(case["storage"])})']
        persist_lines += ['''local rootquest={}
function rootquest:AddEntityBinding(...) quest:AddEntityBinding(...) end
function rootquest:FinalizeEntityBindings() quest:FinalizeEntityBindings() end
function rootquest:CreateThread(name,options) assert(name=='WaitForPieceOver' and options.region=='') end
function rootquest:GetActiveQuestName() return 'QS_GuardianTrophyDealerInfo' end
function rootquest:SetQuestCardObjective(name,key,first,second)
    assert(name=='QS_GuardianTrophyDealerInfo' and key=='TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01')
    assert(first=='HeroGuildComplexInside' and second=='HeroGuildComplexInside')
end
Main(rootquest)
''']
        for name,active,bindings,objective in [
            ('MazeResearch','V_MazeResearch',[('EmptyGrave',0),('HistoryBookcase',0)],
             ['TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01','HeroGuildComplexInside','']),
            ('MeetSister','QS_MeetSister',[('MeetSisterMessenger',1),('MeetSisterSister',0)],
             ['TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY','HauntedHouse','BarrowFields'])]:
            root_package=ROOT/f'refs/script_recovery/reconstructed/{name}/FSE'
            rows=','.join('{'+json.dumps(entity)+','+json.dumps(f'{name}/Entities/{entity}')+','+str(flags)+'}' for entity,flags in bindings)
            persist_lines += [f'package.path={json.dumps(root_package.as_posix()+"/?.lua;")}..package.path',
                f'dofile({json.dumps((root_package/name/(name+".lua")).as_posix())})',
                f'expect_bindings({{{rows}}})',
                f'function rootquest:GetActiveQuestName() return {json.dumps(active)} end',
                'function rootquest:SetQuestCardObjective(name,key,first,second) assert('+ ' and '.join(
                    variable+'=='+json.dumps(value) for variable,value in zip(('name','key','first','second'),[active,*objective]))+') end',
                'Main(rootquest)']
        (output/'port-persistence.lua').write_text('\n'.join(persist_lines)+'\n')
        manager_cpp = runtime / 'LuaManager.cpp'
        manager_source = manager_cpp.read_text(encoding='utf-8-sig')
        registration=next(line for line in manager_source.splitlines() if 'questState_type["AddEntityBinding"] =' in line)
        (output/'binding-registration.inc').write_text(registration+'\n')
        start = manager_source.index('    questState_type["RegisterBoundAliveCondition"] =')
        end = manager_source.index('\n    };', start) + len('\n    };')
        (output/'alive-registration.inc').write_text(manager_source[start:end]+'\n',encoding='utf-8')
        sys.path.insert(0,str(ROOT))
        from tools.script_recovery.guardian_runtime_cases import replay_source as guardian_replay_source, watcher_source
        guardian_helper=ROOT/'tools/script_recovery/guardian_runtime_cases.py'
        guardian_reference=ROOT/'refs/script_recovery/guardian_trophy_dealer_info/reference'
        guardian_proof=guardian_reference.parent/'runtime_evidence/native-entity-cases-20260912.json'
        guardian_cases=json.loads(guardian_proof.read_text())['cases']
        if len(guardian_cases)!=28:raise ValueError('Review changed Guardian inventory before integration')
        (output/'guardian-cases.lua').write_text(guardian_replay_source(guardian_cases,guardian_reference,guardian_package),encoding='utf-8')
        guardian_waiter_proof=guardian_reference.parent/'runtime_evidence/native-waiter-cases-20260912.json'
        guardian_waiter_cases=json.loads(guardian_waiter_proof.read_text())['cases']
        if len(guardian_waiter_cases)!=10:raise ValueError('Review changed Guardian watcher inventory')
        (output/'guardian-watcher.lua').write_text(watcher_source(guardian_waiter_cases,guardian_package),encoding='utf-8')
        start = manager_source.index('    cscriptThing_type["Speak"] = sol::overload(')
        end = manager_source.index('    cscriptThing_type["Converse"]',start)
        (output/'speech-registration.inc').write_text(manager_source[start:end],encoding='utf-8')
        start=manager_source.index('    questState_type["AddLineToConversation"] = sol::overload(')
        end=manager_source.index('    questState_type["IsConversationActive"]',start)
        (output/'conversation-registration.inc').write_text(manager_source[start:end],encoding='utf-8')
        registration=next(line for line in manager_source.splitlines() if 'questState_type["PersistTransferBool"] =' in line)
        (output/'persist-registration.inc').write_text(registration+'\n')
        package = ROOT / 'refs/script_recovery/reconstructed/MeetSister/FSE'
        proof = ROOT / 'refs/script_recovery/meet_sister/runtime_evidence/native-lua-flow-comparison-20260912.json'
        native = json.loads(proof.read_text(encoding='utf-8'))
        cases = [c for c in native['cases'] if c['entry'] != 'main']
        if len(cases) != 14 or any(c['entry'] not in ('messenger', 'sister') for c in cases):
            raise ValueError('Unexpected native scenario inventory; review before accepting')
        (output / 'cases.lua').write_text(replay_source(cases, package), encoding='utf-8')
        oakvale_sources = sorted((ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE').rglob('*.lua'))
        bool_checks = []
        speech_checks = []
        line_checks = []
        for path in oakvale_sources:
            source = path.read_text(encoding='utf-8-sig')
            constants = re.findall(r'local\s+(SPE(?:AK|ECH)\w*)\s*=\s*(\d+)',source)
            for line in source.splitlines():
                line_match=re.search(r'quest:AddLineToConversation\((.*)\)',line.split('--',1)[0])
                if line_match:
                    line_args=lua_arguments(line_match.group(1))
                    if len(line_args) not in (4,5):raise ValueError('Unexpected conversation-line arity')
                    line_checks += [f'expect({line_args[1]}, {line_args[2]}, {line_args[3]}, false)',line_match.group(0),'verify()']
                match = re.search(r'me:Speak\((.*)\)',line.split('--',1)[0])
                if not match: continue
                expression=match.group(0)
                method_names=[name for name,value in constants if re.search(r'\b'+name+r'\b',expression)]
                if len(method_names)!=1: raise ValueError('Unreviewed Speak method expression: '+str(path))
                default=dict(constants)[method_names[0]]
                variants=[('nil','nil',default)]
                if 'method or ' in expression: variants.append(('2','nil','2'))
                if 'selection or ' in expression: variants.append(('nil','2','2'))
                for method,selection,expected_method in variants:
                    speech_checks += ['do',*[f'local {name}={value}' for name,value in constants],
                        f'local method,selection={method},{selection}', 'local key="OPERAND_PROBE"',
                        expression, f'assert(last()=="{expected_method}:010", {json.dumps(path.name)})','end']
            for number, line in enumerate(path.read_text(encoding='utf-8-sig').splitlines(), 1):
                code = line.split('--', 1)[0]
                if ':AddNewConversation(' not in code:
                    continue
                match = re.search(r':AddNewConversation\(me,\s*([^,]+),\s*([^\)]+)\)', code)
                if not match:
                    raise ValueError(f'Unreviewed conversation operand form: {path}:{number}')
                operands = ','.join(match.groups())
                label = json.dumps(f'{path.name}:{number}')
                bool_checks.append(f'assert(operands({operands}) == "00", {label})')
        if len(bool_checks) != 17:
            raise ValueError('Conversation call-site inventory changed; review the native operands')
        (output / 'conversation-bools.lua').write_text('\n'.join(bool_checks) + '\n', encoding='utf-8')
        if speech_checks.count('do') != 15: raise ValueError('Speak call-site inventory changed')
        (output/'speech-operands.lua').write_text('\n'.join(speech_checks)+'\n',encoding='utf-8')
        if line_checks.count('verify()')!=27:raise ValueError('Conversation line inventory changed')
        (output/'conversation-lines.lua').write_text('\n'.join(line_checks)+'\n',encoding='utf-8')
        sources = sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c', 'luac.c', 'onelua.c'))
        if not sources:
            raise ValueError('Vendor Lua sources are missing')
        vcvars = args.vcvars or find_vcvars()
        env = compiler_environment(vcvars)
        compiler = shutil.which('cl.exe', path=env.get('PATH', ''))
        if not compiler:
            raise RuntimeError('vcvars did not provide an accessible cl.exe')
        report['compiler'] = {'path': compiler, 'sha256': sha(Path(compiler))}
        # Hash the header trees too: the actual sol/template code is compiled here.
        inputs = {Path(__file__), proof, vcvars, quest_cpp, manager_cpp, *sources, *CHECKS.glob('*.cpp'),
                  guardian_helper, guardian_proof, guardian_waiter_proof, guardian_persist_proof,
                  *guardian_package.rglob('*.lua'), *guardian_reference.glob('*.lua'),
                  *package.rglob('*.lua'), *maze_package.rglob('*.lua'), persist_proof, *oakvale_sources, *runtime.glob('*.h'),
                  *vendor.rglob('*.h'), *vendor.rglob('*.hpp')}
        report['inputs'] = [{'path': str(p.resolve()), 'sha256': sha(p)} for p in sorted(inputs)]
        report['native_executable_sha256'] = native['exeSha256']

        def execute(name, command):
            result = subprocess.run(command, cwd=output, env=env, stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT, text=True, errors='replace')
            (output / (name + '.log')).write_text(result.stdout, encoding='utf-8')
            report['commands'].append({'name': name, 'argv': command, 'exit_code': result.returncode})
            if result.returncode:
                raise RuntimeError(f'{name} failed ({result.returncode}); see {name}.log')
            return result.stdout

        execute('lua-build', [compiler, '/nologo', '/Bv', '/c', '/TC', '/MD', '/O2',
                              '/I' + str(lua), *map(str, sources)])
        objects = [str(output / (p.stem + '.obj')) for p in sources]
        for name, source in [('resource-smoke', 'retail_resources_smoke.cpp'),
                             ('thing-condition', 'retail_thing_condition.cpp'),
                             ('guardian-integration', 'guardian_cpp_integration.cpp'),
                             ('staged-integration', 'meet_sister_cpp_integration.cpp'),
                             ('maze-shared-state', 'maze_shared_state.cpp'),
                             ('speech-operands', 'speech_operands.cpp'),
                             ('conversation-lines', 'conversation_lines.cpp'),
                             ('port-persistence', 'port_persistence.cpp'),
                             ('conversation-bools', 'conversation_bool_operands.cpp')]:
            execute(name + '-build', [compiler, '/nologo', '/Bv', '/EHsc', '/std:c++17', '/MD', '/O2',
                                     '/I' + str(runtime), '/I' + str(vendor), '/I' + str(lua), '/I' + str(output),
                                     str(CHECKS / source), *objects, '/Fe:' + name + '.exe'])
        persistence = execute('port-persistence', [str(output/'port-persistence.exe'),'port-persistence.lua'])
        if not persistence.startswith('PASS:'):raise RuntimeError('Persistence check missing success record')
        smoke = execute('resource-smoke', [str(output / 'resource-smoke.exe')])
        thing_condition = execute('thing-condition', [str(output / 'thing-condition.exe')])
        if not thing_condition.startswith('PASS:'):raise RuntimeError('Thing/condition check missing success record')
        guardian=execute('guardian-integration',[str(output/'guardian-integration.exe'),'guardian-cases.lua'])
        guardian_expected=[f"PASS {c['name']} {len(c['events'])} events" for c in guardian_cases]
        if guardian.splitlines()!=guardian_expected:raise RuntimeError('Guardian native adapter event inventory differs')
        watcher=execute('guardian-watcher',[str(output/'guardian-integration.exe'),'guardian-watcher.lua'])
        watcher_expected=[f"PASS {c['name']} {len(c['events'])} events" for c in guardian_waiter_cases]+['PASS root_init_main 6 events']
        if watcher.splitlines()!=watcher_expected:raise RuntimeError('Guardian watcher/root event inventory differs')
        maze = execute('maze-shared-state', [str(output / 'maze-shared-state.exe')])
        speech = execute('speech-operands', [str(output/'speech-operands.exe'),'speech-operands.lua'])
        lines = execute('conversation-lines', [str(output/'conversation-lines.exe'),'conversation-lines.lua'])
        if not lines.startswith('PASS:'):
            raise RuntimeError('Conversation-line check exited without its success record')
        if not speech.startswith('PASS:'):
            raise RuntimeError('Speech operands exited without its success record')
        if not maze.startswith('PASS:'):
            raise RuntimeError('Maze shared-state check exited without its success record')
        if not smoke.startswith('PASS:'):
            raise RuntimeError('Resource smoke exited without its success record')
        replay = execute('staged-integration', [str(output / 'staged-integration.exe'), 'cases.lua'])
        expected = [f"PASS {c['entry']}/{c['scenario']} {len(c['events'])} events" for c in cases]
        if replay.splitlines() != expected:
            raise RuntimeError('Integration output differs from complete native scenario inventory')
        boolean_result = execute('conversation-bools', [str(output / 'conversation-bools.exe'), 'conversation-bools.lua'])
        if not boolean_result.startswith('PASS:'):
            raise RuntimeError('Boolean operand check exited without its success record')
        # Shared workspace: detect source changes during compilation/testing.
        for item in report['inputs']:
            if sha(Path(item['path'])) != item['sha256']:
                raise RuntimeError('Input changed during run: ' + item['path'])
        report.update(passed=True, entity_cases=len(cases), matching_events=sum(len(c['events']) for c in cases),
                      guardian_entity_cases=len(guardian_cases), guardian_entity_events=sum(len(c['events']) for c in guardian_cases),
                      guardian_watcher_cases=len(guardian_waiter_cases), guardian_persistence_cases=len(guardian_persist_cases),
                      guardian_root_contract_cases=1,
                      staged_root_binding_cases=3,
                      conversation_bool_sites=len(bool_checks))
    except Exception as exc:
        report['error'] = str(exc)
    finally:
        report['outputs'] = [{'path': p.name, 'sha256': sha(p)} for p in sorted(output.iterdir()) if p.is_file()]
        (output / 'result.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ('passed', 'entity_cases', 'matching_events', 'error') if k in report}))
    print('Report:', output / 'result.json')
    return 0 if report['passed'] else 1


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--forge-root', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True, help='New directory; existing paths are rejected')
    parser.add_argument('--vcvars', type=Path, help='Optional explicit vcvars32.bat')
    args = parser.parse_args()
    try:
        return run(args)
    except Exception as exc:
        print(str(exc), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
