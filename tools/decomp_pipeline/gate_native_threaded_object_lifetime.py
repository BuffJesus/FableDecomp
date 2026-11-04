"""Exact request lifetime roots and unchanged existing manager deletion callers."""
import native_exact_roots
import hashlib,tempfile
from pathlib import Path
import check_one as V
from threaded_object_lifetime_oracle import ROOTS,ADDRESS,compare_lifetime
import gate_native_threaded_file_manager_remove as manager
import gate_native_threaded_worker_exit as worker
def main():
 behavior=compare_lifetime();assert behavior['cases']==280
 from threaded_object_lifetime_oracle import R
 source=R/'rebuild/tests/integration/NativeThreadedObjectLifetime_compile.cpp'
 pin=hashlib.sha256(source.read_bytes()).hexdigest();checks=[]
 with tempfile.TemporaryDirectory(prefix='request-owner-contract-') as tmp:
  for name,macro in (('positive',None),('copy','FABLE_NEGATIVE_REQUEST_COPY'),('assignment','FABLE_NEGATIVE_REQUEST_ASSIGN')):
   flags=['/nologo','/c','/O2','/MT','/Fo'+str(Path(tmp)/(name+'.obj'))]
   if macro:flags.append('/D'+macro)
   result=V.cl([*flags,str(source)],V.env(),Path(tmp));text=result.stdout+result.stderr
   if macro:assert result.returncode and 'error C2248' in text and 'private' in text,text
   else:assert result.returncode==0,text
   checks.append(dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
 assert hashlib.sha256(source.read_bytes()).hexdigest()==pin
 behavior['copy_contract']=checks;behavior['input_sha256'][str(source)]=pin
 callers=[r for r in manager.ROOTS if r['name'] in ('remove_object','on_finish_object')]+worker.ROOTS
 targets=dict(manager.ADDRESS);targets.update(worker.ADDRESS);targets.update(ADDRESS)
 native_exact_roots.run(__file__,ROOTS+callers,targets,'native-threaded-object-lifetime.json',[behavior],
  'Three new lifetime roots; existing remove/finish/worker callers remain exact with ownership centralized in the true destructor.')
 print('NATIVE_THREADED_OBJECT_LIFETIME_PASS cases=280 roots=3 callers=3 omissions=3')
if __name__=='__main__':main()
