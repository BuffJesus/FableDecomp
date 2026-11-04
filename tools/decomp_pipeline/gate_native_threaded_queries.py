"""Const UID lookup and manager/file existence/finished acceptance."""
import hashlib,json
import tempfile
from pathlib import Path
from threaded_query_oracle import compare_queries,R
import check_one as V
def const_contract():
 source=R/'rebuild/tests/integration/NativeThreadedConstQueries_compile.cpp'
 pin=hashlib.sha256(source.read_bytes()).hexdigest();rows=[]
 with tempfile.TemporaryDirectory(prefix='threaded-const-contract-') as tmp:
  for name,macro in (('positive',None),('const_write','FABLE_NEGATIVE_CONST_WRITE'),('const_conversion','FABLE_NEGATIVE_CONST_CONVERSION')):
   flags=['/nologo','/c','/O2','/MT','/Fo'+str(Path(tmp)/(name+'.obj'))]
   if macro:flags.append('/D'+macro)
   result=V.cl([*flags,str(source)],V.env(),Path(tmp));diagnostic=result.stdout+result.stderr
   if macro:assert result.returncode and 'error C' in diagnostic and 'const' in diagnostic.lower(),diagnostic
   else:assert result.returncode==0,diagnostic
   rows.append(dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
 assert hashlib.sha256(source.read_bytes()).hexdigest()==pin
 return dict(status='PASS',checks=rows,source=str(source),source_sha256=pin)
def main():
 report=compare_queries()
 assert report['providers']==report['missing_provider_checks']==5
 contract=const_contract();report['const_contract']=contract
 report['input_sha256'][contract['source']]=contract['source_sha256']
 report['input_sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 (R/'rebuild/compile-gate/native-threaded-queries.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_QUERIES_PASS cases=%d roots=5 omissions=5'%report['cases'])
if __name__=='__main__':main()
