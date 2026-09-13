"""Prepare review-only metadata/package manifests; never apply or deploy them."""
import argparse
import copy
import hashlib
import json
from pathlib import Path

try:
    from .validate_meet_sister_package import validate
except ImportError:
    from validate_meet_sister_package import validate

ROOT = Path(__file__).resolve().parents[2]


def hash_file(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def parameter(name, type_name, optional=False):
    return {'name': name, 'type': type_name, 'optional': optional}


def prepare(forge, output):
    package = ROOT / 'refs/script_recovery/reconstructed/MeetSister/FSE'
    validation = validate(package, forge)
    if validation['errors']:
        raise ValueError(validation['errors'])
    baseline = ROOT / 'refs/fse_api_manifest.json'
    current = json.loads(baseline.read_text(encoding='utf-8-sig'))
    binding = next(f for f in current['functions'] if f['name'] == 'AddEntityBinding' and f['scope'] == 'Quest')
    replacement = copy.deepcopy(binding)
    replacement['parameters'] = binding['parameters'][:2] + [parameter('flags', 'int', True)]
    replacement['description'] = 'Registers an entity binding. Omitted or nil flags default to 0; native binding flags propagate to the active script.'
    functions = []
    def function(name, scope, result, params, blocking, description):
        functions.append({'name': name, 'scope': scope, 'returnType': result, 'parameters': params,
                          'blocking': blocking, 'category': scope + ' API', 'description': description})
    function('WithRetailResources', 'Quest', 'void', [parameter('callback', 'sol::protected_function')], True,
             'Calls callback(resources) synchronously; closes resources at callback exit/error. Retained scopes reject later calls.')
    for name, result, args, blocking, description in [
        ('NewResource','unsigned',[],False,'Creates a distinct empty scripted resource local.'),
        ('PrepareResource','void',[('id','unsigned')],False,'Runs native preparation helpers for this resource.'),
        ('TryAcquire','bool',[('id','unsigned'),('actor','CScriptThing*'),('priority','int')],False,'Makes one native acquisition attempt; does not retry, yield or infer termination.'),
        ('ReleaseResource','void',[('id','unsigned')],False,'Destroys this local reference now; actor-map copies may retain ownership.'),
        ('Frame','void',[],True,'Performs one native script frame without an implicit termination query.'),
        ('NewActorMap','unsigned',[],False,'Creates a caller-owned actor map.'),
        ('SetActor','void',[('map','unsigned'),('name','const std::string&'),('resource','unsigned')],False,'Copies the resource into the map, including an empty resource.'),
        ('DestroyActorMap','void',[('id','unsigned')],False,'Destroys the map and its retained resource references now.'),
        ('RunMacro','void',[('name','const std::string&'),('map','unsigned'),('setup','bool'),('skippable','bool')],True,'Runs the macro with nil flag/input maps and the caller-owned actor map; no implicit acquisition, movie, camera or termination policy.'),
        ('StartMovie','unsigned',[('name','const std::string&')],False,'Creates and starts one movie local. A second live movie in the same scope is rejected.'),
        ('DestroyMovie','void',[('id','unsigned')],False,'Destroys the movie now; reviewed code explicitly unpauses first.'),
        ('Pause','void',[('paused','bool')],False,'Forwards the native non-scripted-entity pause operation; Lua booleans are required.'),
    ]:
        function(name,'RetailResources',result,[parameter(*arg) for arg in args],blocking,description)
    proposal = {'status': 'proposal-only-not-applied', 'baseline_manifest_sha256': hash_file(baseline),
                'replace_by_identity': [{'scope':'Quest','name':'AddEntityBinding','before':binding,'after':replacement}],
                'add_functions':functions,
                'integration_requirements': ['Coordinate with converter/metadata owner before applying.',
                    'Teach metadata consumers the RetailResources receiver scope; do not classify it as Entity.',
                    'Review callback type support before generating SDK overlays.',
                    'Do not change manifest/build version fields without a coordinated release.']}
    evidence = ROOT / 'refs/script_recovery/meet_sister/runtime_evidence'
    dependencies = ['installed-marker-camera-dependencies-20260912.json', 'installed-dialogue-dependencies-20260912.json',
                    'native-scheduler-entry-20260912.json', 'native-lua-flow-comparison-20260912.json']
    source = forge / 'FableScriptExtender'
    manifest = {'status':'disabled-source-package-not-deployable-binary-bundle', 'native_name':'QS_MeetSister',
                'validation':validation, 'files':[{'path':p.relative_to(package).as_posix(),'sha256':hash_file(p)} for p in sorted(package.rglob('*')) if p.is_file()],
                'runtime_sources':[{'path':str(p),'sha256':hash_file(p)} for p in sorted(source.iterdir()) if p.suffix in ('.h','.cpp')],
                'evidence':[{'path':str(evidence/name),'sha256':hash_file(evidence/name)} for name in dependencies],
                'baseline_install_manifest_sha256':hash_file(ROOT/'refs/script_recovery/new_oakvale_intro/runtime_evidence/deployment-v23-20260912.json'),
                'binary':None, 'activation':'disabled; no legacy registration',
                'prerequisites':['Build and identify a Release x86 DLL containing the explicit resource API.',
                                 'Apply coordinated API metadata changes.',
                                 'Use a separate backed-up disposable profile and preserve New Oakvale configuration.',
                                 'Verify current installed assets against reviewed dependency evidence.',
                                 'Perform live scheduler, resource lifecycle and normal/skip playback acceptance.']}
    output.mkdir(parents=True, exist_ok=False)
    for name, data in [('api-proposal.json',proposal),('package-manifest.json',manifest)]:
        (output/name).write_text(json.dumps(data,indent=2)+'\n',encoding='utf-8')
    print(f'Prepared {len(manifest["files"])} package files and {len(functions)} proposed API entries: {output}')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--forge-root',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    prepare(args.forge_root.resolve(),args.output.resolve())


if __name__=='__main__':
    main()
