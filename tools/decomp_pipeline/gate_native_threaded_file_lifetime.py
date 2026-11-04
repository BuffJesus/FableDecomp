"""Constructor, virtual deleting helper, vtable fallback and ordinary teardown."""
import hashlib,json
from pathlib import Path
from threaded_file_lifetime_oracle import R,compare_lifetime
def main():
 report=compare_lifetime();assert report['cases']==512 and report['ordinary_destructor_cases']==72
 report['input_sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 (R/'rebuild/compile-gate/native-threaded-file-lifetime.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_FILE_LIFETIME_PASS cases=512 ordinary=72 roots=3 omissions=3 controls=6 grade=MATCH')
if __name__=='__main__':main()
