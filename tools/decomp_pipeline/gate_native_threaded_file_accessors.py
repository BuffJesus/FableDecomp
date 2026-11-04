"""Fresh VC71 exact parity and behavior for four public threaded-file leaves."""
import native_exact_roots
from threaded_file_accessors_oracle import ROOTS,ADDRESS,compare_accessors
def main():
 behavior=compare_accessors()
 assert behavior['cases']==870 and behavior['missing_provider_checks']==4
 native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-file-accessors.json',[behavior],
  'Original and rebuilt public field/wake leaves; host APIs controlled, no engine execution.')
 print('NATIVE_THREADED_FILE_ACCESSORS_PASS cases=870 roots=4 omissions=4')
if __name__=='__main__':main()
