import json, csv, sys
base=r'C:/Users/Cornelio/AppData/Local/Temp/claude/D--Documents-FableTLC/60e53032-daf0-4b7f-b8fd-c49d9c4b10b6'
scr=base+'/scratchpad'
taskid=sys.argv[1]; prefix=sys.argv[2]
d=json.load(open(base+'/tasks/%s.output'%taskid))
orc={r['address'].lower():r for r in csv.DictReader(open(scr+'/%s_oracle.tsv'%prefix,encoding='utf-8-sig'),delimiter='\t')}
authored=[]; rows=[]; tried=[]
for c in d['result']['candidates']:
    a=c['address'].lower().replace('0x','')
    tried.append(a)
    if c['status']=='WIN' and a in orc:
        o=orc[a]; rows.append((a,o['name'],o['length'],o['bytes']))
        authored.append({"address":a,"name":c['name'],"module":c['module'],
            "source_cpp":c['source_cpp'],"test_cpp":c['test_cpp'],"pass_pattern":c['pass_pattern']})
json.dump({"result":{"authored":authored}}, open(scr+'/%s_land.json'%prefix,'w'))
with open(scr+'/%s_land_oracle.tsv'%prefix,'w',newline='',encoding='utf-8') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n'); w.writerow(['address','name','length','bytes'])
    for r in rows: w.writerow(r)
# update tried ledger (all attempted, win or not)
tp=scr+'/gen_tried.txt'
import os
ex=set(open(tp).read().split()) if os.path.exists(tp) else set()
ex.update(tried); open(tp,'w').write(' '.join(sorted(ex))+'\n')
print('land candidates:',len(authored),' tried-ledger:',len(ex))
