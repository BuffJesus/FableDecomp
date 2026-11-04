"""Exact manager callback policy, original fastcall and zero error-callback state."""
from threaded_error_policy_oracle import compare_policy,audit_error_callback_data,ROOTS,ADDRESS
import native_exact_roots
def main():
 behavior=compare_policy();data=audit_error_callback_data()
 assert behavior['cases']==196 and behavior['missing_provider_checks']==6
 native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-error-policy.json',[behavior,data],
  'Six policy leaves and their real fastcall/dispatch instructions; error callbacks controlled, no IO or engine owner.')
 print('NATIVE_THREADED_ERROR_POLICY_PASS cases=196 roots=6 data=1 omissions=6')
if __name__=='__main__':main()
