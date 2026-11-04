"""Exact write-open leaf with independently controlled dependency contracts."""
import native_exact_roots
from threaded_file_open_write_oracle import ROOTS,ADDRESS,compare_write_open
def main():
 behavior=compare_write_open();assert behavior['cases']==864
 native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-file-open-write.json',[behavior],
  'Exact original write-open and shared sort counter; strings and OS boundaries controlled.')
 print('NATIVE_THREADED_FILE_OPEN_WRITE_PASS cases=864 roots=1 omissions=1 controls=2 grade=MATCH')
if __name__=='__main__':main()
