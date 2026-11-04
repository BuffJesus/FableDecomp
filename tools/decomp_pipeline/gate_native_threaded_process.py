"""Worker scheduling/submission near-match acceptance gate."""
import json,hashlib
from pathlib import Path
from threaded_process_oracle import compare_process,R

def main():
 report=compare_process()
 assert report['cases']==960 and report['root_controls']==2
 report['input_sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 (R/'rebuild/compile-gate/native-threaded-process.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_PROCESS_PASS cases=960 root_controls=2 grade=NEAR_MATCH')

if __name__=='__main__':main()
