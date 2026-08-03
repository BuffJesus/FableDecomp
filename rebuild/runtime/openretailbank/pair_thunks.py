import re
cpp=open(r'D:/Documents/FableTLC/rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp').read()
syms=re.findall(r'__asm call (FableOpenRetail\w+)', cpp)
line=open(r'C:/Users/Cornelio/AppData/Local/Temp/oab_anchor_oracle.tsv',encoding='utf-8-sig').read().splitlines()[1].split('\t')
va=int(line[0],16); b=bytes.fromhex(line[2])
i=0; calls=[]
while i < len(b)-4:
    if b[i]==0xE8:
        rel=int.from_bytes(b[i+1:i+5],'little',signed=True); calls.append((va+i+5+rel)&0xffffffff); i+=5; continue
    i+=1
print('cpp __asm call count:',len(syms),' retail E8 count:',len(calls))
assert len(syms)==len(calls), "MISMATCH — cannot pair"
m={}
for s,t in zip(syms,calls):
    a='%08x'%t
    if s in m and m[s]!=a: print('CONFLICT',s,m[s],a)
    m[s]=a
print('distinct thunks:',len(m))
for s,a in sorted(m.items(),key=lambda x:x[1]): print(f'{a}  {s}')
import json
json.dump(m, open(r'C:/Users/Cornelio/AppData/Local/Temp/oab_thunk_map.json','w'))
