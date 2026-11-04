import sys,tempfile,subprocess,json,argparse,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
import gate_zlib_deflate_core as core
ap=argparse.ArgumentParser();ap.add_argument('--active-list',action='store_true');args=ap.parse_args();args.active_list=True
w=Path(tempfile.mkdtemp(prefix='installer-compressor-lifetime-'));env=check_one.env();objects=[]
sources=[ROOT/'rebuild/src/compiled/00/a3/CCompressorZlib_CCompressorZlib_00a3acc0.cpp',core.SOURCE,core.TREES]+[ROOT/'rebuild/src/compiled'/p for p in core.SUPPORT]+[ROOT/'rebuild/src/compiled/00/c0'/p for p in ['Zlib_zinflateInit_00c02310.cpp','Zlib_zinflateInit2_00c02240.cpp','Zlib_zinflateReset_00c021f0.cpp','Zlib_zinflateEnd_00c038d0.cpp']]+[ROOT/'rebuild/tests/integration/InstallerCompressorLifetime_test.cpp']
if args.active_list:
 sources[-1]=ROOT/'rebuild/tests/integration/InstallerActiveFileLifetime_test.cpp'
 sources.insert(-1,ROOT/'rebuild/src/compiled/00/9c/InstallerActiveFileList_Clear_009c71a0.cpp')
for i,source in enumerate(sources):
 obj=w/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,w);assert r.returncode==0,(source,r.stdout,r.stderr);objects.append(obj)
blob=objects[0].read_bytes()
for old,new in [('??1CBaseClass@@UAE@XZ','@BDtor@4')]:
 needle=old.encode()+b'\0';value=new.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
objects[0].write_bytes(blob)
exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
assert ('INSTALLER_ACTIVE_LIFETIME_OK cases=144 nodes=288 objects=144'if args.active_list else 'INSTALLER_COMPRESSOR_LIFETIME_OK cases=64') in r.stdout
if args.active_list:
 r2=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=len(objects)-2],env=env,capture_output=True,text=True);assert r2.returncode!=0
report=dict(cases=144 if args.active_list else 64,omission_checks=2 if args.active_list else 1,boundary='Actual recovered compressor destructor and zlib deflate/inflate init/end providers; allocation callbacks use calloc/free and track live allocations. Base destructor redirected only in test object; uncalled virtual slots abort; raw storage bypasses constructor.')
if args.active_list:report.update(nodes=288,objects=144,composition='Actual active-list cleanup, unredirected CRT nested/outer releases, shared counted-control new/delete, fixture object destroyed exactly once after all streams released. Empty/uninitialized, deflate, inflate modes; repeated empty Clear. Production CFileInfo object body and full constructor remain deferred.')
report['providers']=[dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest())for source in sources]
(ROOT/'rebuild/compile-gate'/('active-file-list-lifetime.json'if args.active_list else 'compressor-lifetime.json')).write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip())
