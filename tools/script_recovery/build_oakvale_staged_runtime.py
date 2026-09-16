"""Build the full DLL in an isolated copy of the current runtime plus proposal."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_oakvale_entity_capabilities import OUTPUT
from tools.script_recovery.run_meet_sister_runtime_checks import find_vcvars

OUT=ROOT/'work/oakvale_runtime_build'


def run(stage=OUTPUT):
    stage=Path(stage);proposal=json.loads((stage/'proposal.json').read_text())
    runtime=Path(proposal['source']).parent.parent
    copied=OUT/'runtime';copied.mkdir(parents=True,exist_ok=True)
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    records=dict(proposal['additionalSources'])
    records['LuaRetailResources.h']=dict(source=proposal['source'],sourceSha256=proposal['sourceSha256'],candidateSha256=proposal['candidateSha256'])
    for name,record in records.items():
        if digest(Path(record['source']))!=record['sourceSha256'] or digest(stage/name)!=record['candidateSha256']:
            raise ValueError('Full runtime input changed: '+name)
    for name,expected in proposal['helpers'].items():
        if digest(stage/name)!=expected:raise ValueError('Full runtime helper changed: '+name)
    inputs={}
    for folder in ('FableScriptExtender','Vendor'):
        for path in (runtime/folder).rglob('*'):
            if not path.is_file() or any(part in ('Release','Debug','.vs','x64','ipch') for part in path.relative_to(runtime/folder).parts):continue
            relative=path.relative_to(runtime);destination=copied/relative;destination.parent.mkdir(parents=True,exist_ok=True)
            shutil.copyfile(path,destination);inputs[str(path)]=digest(path)
    for name in (*records,*proposal['helpers']):shutil.copyfile(stage/name,copied/'FableScriptExtender'/name)
    staged_inputs={str(p.relative_to(copied)):digest(p) for folder in ('FableScriptExtender','Vendor') for p in (copied/folder).rglob('*') if p.is_file() and not any(x in ('Release','Debug') for x in p.relative_to(copied).parts)}
    msbuild=Path(find_vcvars()).parents[3]/'MSBuild/Current/Bin/MSBuild.exe'
    command=[str(msbuild),str(copied/'FableScriptExtender/FableScriptExtender.vcxproj'),
        '/t:Build','/m:2','/nologo','/v:minimal','/p:Configuration=Release','/p:Platform=Win32',
        '/p:SolutionDir='+str(copied)+'\\','/p:OutDir='+str(OUT/'bin')+'\\',
        '/p:IntDir='+str(OUT/'obj')+'\\']
    with (OUT/'build.log').open('w') as log:result=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT)
    changed=[path for path,expected in inputs.items() if digest(Path(path))!=expected]
    report=dict(status='passed' if result.returncode==0 and not changed else 'failed',exitCode=result.returncode,
                command=command,sourceInputs=inputs,stagedInputs=staged_inputs,changedInputs=changed,
                limits='Full x86 DLL build only; no installation, activation, scheduler or gameplay validation.')
    dll=OUT/'bin/FableScriptExtender.dll'
    if result.returncode==0:
        body=dll.read_bytes();pe=int.from_bytes(body[0x3c:0x40],'little')
        report.update(dll=str(dll),dllSha256=digest(dll),machine=int.from_bytes(body[pe+4:pe+6],'little'))
        if report['machine']!=0x14c:report['status']='failed'
    (OUT/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    if report['status']!='passed':raise RuntimeError('Full staged DLL build failed; see '+str(OUT/'build.log'))
    return report


if __name__=='__main__':
    report=run();print(json.dumps({k:report[k] for k in ('status','exitCode','dll','dllSha256','machine')},indent=2))
