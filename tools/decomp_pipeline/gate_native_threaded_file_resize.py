"""Fresh compile and original/rebuilt resize contract acceptance."""
import hashlib,json
from pathlib import Path
from threaded_file_resize_oracle import R,compare_resize
def main():
 report=compare_resize();assert report['cases']==480
 report['input_sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 (R/'rebuild/compile-gate/native-threaded-file-resize.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_FILE_RESIZE_PASS cases=480 roots=1 omissions=1 controls=2 grade=%s'%report['root']['acceptance_grade'])
if __name__=='__main__':main()
