import json,re,glob,os
SC="C:/Users/Cornelio/AppData/Local/Temp/claude/D--Documents-FableTLC/c1506a19-6449-4e31-9da7-38998d0c1b33/scratchpad"
ours=json.load(open(SC+"/transfer_orders.json"))
DEFDIR="C:/Users/Cornelio/Documents/EgoCoreInspect/fable-defs/packages/defs/src/def"
fd={}
for p in glob.glob(DEFDIR+"/*.rs"):
    txt=open(p,encoding="utf-8").read()
    sm=re.search(r"pub struct (\w+Def)\b",txt)
    if not sm: continue
    cls="C"+sm.group(1)
    order=re.findall(r'#\[def\("([^"]+)"',txt)
    fd[cls]=order
def subseq(short,long):
    it=iter(long)
    return all(any(x==y for y in it) for x in short)  # order-preserving containment
matched=0; exact_sub=0; missing_cls=[]; deltas=[]
for cls,order in fd.items():
    if cls not in ours: missing_cls.append(cls); continue
    matched+=1
    ourn=[fl["name"] for fl in ours[cls]["fields"] if fl["name"]]
    if subseq(order,ourn): exact_sub+=1
    else:
        only_fd=[x for x in order if x not in ourn]
        deltas.append((cls,only_fd,order,ourn))
print("fable-defs def structs:",len(fd))
print("present in our extraction:",matched,"| fable-defs-only (we lack Transfer):",len(missing_cls))
print("order-preserving subsequence match:",exact_sub,"/",matched)
print("\nfable-defs classes we DON'T have:",sorted(missing_cls)[:20])
print("\n-- deltas (fable-defs field names not found in our order) --")
for cls,only,o,n in deltas[:25]:
    print("  %-30s fd-only=%s"%(cls,only))
