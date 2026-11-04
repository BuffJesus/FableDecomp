from pathlib import Path
import sys,json,subprocess,hashlib
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'));import check_one as V
b=Path(sys.argv[1]).resolve();report=json.loads((b/'build-report.json').read_text());proof=json.loads((b/'allocation-retail-proof.json').read_text());assert proof['status']=='PASS' and proof['exact_bodies']==28
root=next(row for row in proof['proof'] if row['entry']=='0x9ceae0');n=root['provider'];source=(b/('root%d.cpp'%n)).read_text();objects=[Path(p) for p in report['object_sha256']]
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include')
controls=[('bank-size','Size = size;','Size = size+1;'),('symbol-array','if (OpenFlags & 8)','if (OpenFlags & 0)'),('checksum-array','if (OpenFlags & 16)','if (OpenFlags & 0)'),('entry-update-array','if (OpenFlags & 32)','if (OpenFlags & 0)'),('runtime-valid','empty.Valid=false;','empty.Valid=true;')]
results=[]
for i,(name,old,new) in enumerate(controls):
 assert source.count(old)==1;src=b/('allocation-control-%d.cpp'%i);assert not src.exists();src.write_text(source.replace(old,new));obj=src.with_suffix('.obj')
 run=V.cl(['/nologo','/c','/O2','/MT','/Gy','/Dmalloc=FixtureVectorMalloc','/Dfree=FixtureVectorFree','/I'+str(b/'include'),'/Fo'+str(obj),str(src)],env,b)
 (b/('allocation-control-%d.compile.json'%i)).write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 exe=src.with_suffix('.exe');cmd=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*[str(obj) if p==b/('root%d.obj'%n) else str(p) for p in objects]]
 run=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True)
 (b/('allocation-control-%d.link.json'%i)).write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 run=subprocess.run([str(exe),str(b/'cases.bin')],cwd=b,capture_output=True,text=True,timeout=30)
 row=dict(name=name,returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)
 (b/('allocation-control-%d.run.json'%i)).write_text(json.dumps(row));assert run.returncode==80 and report.get('failure_marker','BANK_ENTRY_ALLOCATED_FAILURE') in run.stderr,row;results.append(row)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for p,h in report['input_sha256'].items():assert sha(Path(p))==h,('input',p)
for p,h in report['object_sha256'].items():assert sha(Path(p))==h,('object',p)
assert sha(b/'probe.exe')==report['executable_sha256'] and sha(b/'original-root.exe')==report['original_executable_sha256']
(b/'allocation-controls.json').write_text(json.dumps(dict(status='PASS',controls=results,additional_controls=len(results),input_pins=len(report['input_sha256']),objects=len(objects)),indent=2));print('ALLOCATION_CONTROLS_PASS',len(results))
