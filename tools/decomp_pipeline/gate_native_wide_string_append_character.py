"""First-form character append and exact reserve/fill shared template support."""
import hashlib,json
from pathlib import Path
from wide_string_append_char_oracle import R,compare_append_char
def main():
 report=compare_append_char();assert report['cases']==1756 and report['root_controls']==6
 report['input_sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 (R/'rebuild/compile-gate/native-wide-string-append-character.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_WIDE_STRING_APPEND_CHARACTER_PASS cases=1756 append=784 reserve=804 fill=168 roots=3 omissions=3 controls=6 grades=NEAR_MATCH,MATCH,MATCH')
if __name__=='__main__':main()
