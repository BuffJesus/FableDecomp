#!/usr/bin/env python3
"""Score ONE authored candidate against the retail oracle (byte-match + behaviour).
Isolated per-call workdir so parallel cl.exe invocations don't collide.
Usage: python check_one.py <candidate.json> <oracle.tsv> <workdir>
  candidate.json = {"address","name","module","source_cpp","test_cpp","pass_pattern"}
Prints one line:  <addr> <parity> <behav>  and a final RESULT: WIN|NOWIN <parity> <behav>.
Parity WIN = MATCH*/RELOCATION_MATCH* ; behav WIN = PASS. Both required for a land."""
import csv, json, re, subprocess, os, html, sys
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
VC = Path(r"D:\Tools\vc71")
OBJDUMP = r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe"
DS = re.compile(r"^\s*[0-9a-fA-F]+\s+<(.+)>:$"); DB = re.compile(r"^\s*[0-9a-fA-F]+:\s+((?:[0-9a-fA-F]{2}\s+)+)")
RL = re.compile(r"^([0-9a-fA-F]{8})\s+\S+\s+.+$")
PRAGMAS = ["", '#pragma optimize("s",on)', '#pragma optimize("t",on)', '#pragma optimize("g",on)']

def vc71(s):
    s = html.unescape(s).replace("__thiscall", "__fastcall")
    return "\n".join(l for l in s.splitlines() if "static_assert" not in l)

def obj_text(path, leaf):
    pr = subprocess.run([OBJDUMP,"-d",str(path)],capture_output=True,text=True,check=True)
    fns,sec,cur=[],-1,None
    for line in pr.stdout.splitlines():
        if line.startswith("Disassembly of section .text:"): sec+=1;cur=None;continue
        s=DS.match(line)
        if s: cur={"symbol":s.group(1),"section":sec,"bytes":bytearray()};fns.append(cur);continue
        e=DB.match(line)
        if e and cur is not None: cur["bytes"].extend(bytes.fromhex(e.group(1)))
    if not fns: raise RuntimeError("no funcs")
    named=[f for f in fns if leaf and leaf in str(f["symbol"])]
    sel=max(named or fns,key=lambda f:len(f["bytes"]))
    return bytes(sel["bytes"]),int(sel["section"]),str(sel["symbol"])

def obj_relocs(path, sel):
    hp=subprocess.run([OBJDUMP,"-h",str(path)],capture_output=True,text=True,check=True)
    flags=[];hl=hp.stdout.splitlines()
    for i,l in enumerate(hl):
        if re.match(r"^\s*\d+\s+\.text\s+",l): flags.append("RELOC" in (hl[i+1] if i+1<len(hl) else ""))
    if sel>=len(flags) or not flags[sel]: return []
    bi=sum(flags[:sel+1])-1
    rp=subprocess.run([OBJDUMP,"-r",str(path)],capture_output=True,text=True,check=True)
    blocks=[];intext=False;ts=-1
    for l in rp.stdout.splitlines():
        if l.startswith("RELOCATION RECORDS FOR ["):
            intext="[.text]" in l
            if intext: ts+=1;blocks.append([])
            continue
        if intext:
            m=RL.match(l.strip())
            if m: blocks[ts].append(int(m.group(1),16))
    return blocks[bi] if bi<len(blocks) else []

def mask(p,offs):
    r=bytearray(p)
    for o in offs:
        for i in range(o,min(o+4,len(r))): r[i]=0
    return bytes(r)

def env():
    e=dict(os.environ);e["PATH"]=str(VC/"bin")+";"+e["PATH"]
    e["INCLUDE"]=f"{VC/'include'};{ROOT/'rebuild'/'include'}";e["LIB"]=str(VC/"lib");return e

def cl(args,e,cwd): return subprocess.run([str(VC/"bin"/"cl.exe")]+args,capture_output=True,text=True,env=e,cwd=str(cwd))

def main():
    candf=Path(sys.argv[1]); oraclef=Path(sys.argv[2]); WORK=Path(sys.argv[3]); WORK.mkdir(parents=True,exist_ok=True)
    c=json.loads(candf.read_text(encoding="utf-8"))
    orc={r["address"].lower():r for r in csv.DictReader(open(oraclef,encoding="utf-8-sig"),delimiter="\t")}
    e=env()
    addr=c["address"].lower().replace("0x","")
    name=c["name"]; leaf=name.rsplit("::",1)[-1]
    o=orc.get(addr)
    if not o: print(f"{addr} NO_ORACLE -"); print("RESULT: NOWIN NO_ORACLE -"); return
    retail=bytes.fromhex(o["bytes"])
    src0=vc71(c["source_cpp"]); tst=vc71(c["test_cpp"]); patt=c["pass_pattern"]
    def parity_of(srctext):
        sp=WORK/f"{addr}.cpp"; sp.write_text(srctext,encoding="utf-8")
        obj=WORK/f"{addr}.obj"; obj.unlink(missing_ok=True)
        cp=cl(["/nologo","/c","/O2","/Oy","/W3",f"/Fo{obj}",str(sp)],e,WORK)
        if cp.returncode or not obj.exists(): return "SRC_FAIL",cp.stdout+cp.stderr
        try: built,sec,_=obj_text(obj,leaf)
        except Exception as ex: return "OBJDUMP_ERR",str(ex)
        rel=obj_relocs(obj,sec)
        if retail==built: return "MATCH",""
        if mask(retail,rel)==mask(built,rel): return "RELOCATION_MATCH",""
        return f"DIFFER({len(built)}v{len(retail)})",""
    st,diag=parity_of(src0)
    if not st.startswith(("MATCH","RELOCATION")):
        for P in PRAGMAS[1:]:
            st2,_=parity_of(P+"\n"+src0)
            if st2.startswith(("MATCH","RELOCATION")): st=st2+f"[{P.split('(')[1].split(',')[0]}]"; break
    # behaviour
    tp=WORK/f"{addr}.test.cpp"; tp.write_text(tst,encoding="utf-8")
    tobj=WORK/f"{addr}.t.obj"; exe=WORK/f"{addr}.exe"; tobj.unlink(missing_ok=True); exe.unlink(missing_ok=True)
    beh="TCC_FAIL"; bdiag=""
    tc=cl(["/nologo","/c","/Od","/W3",f"/Fo{tobj}",str(tp)],e,WORK)
    if tc.returncode==0 and tobj.exists():
        lk=subprocess.run([str(VC/"bin"/"link.exe"),"/nologo",f"/out:{exe}",str(tobj)],capture_output=True,text=True,env=e,cwd=str(WORK))
        if lk.returncode==0 and exe.exists():
            rn=subprocess.run([str(exe)],capture_output=True,text=True,env=e,cwd=str(WORK))
            beh="PASS" if (rn.returncode==0 and re.search(re.escape(patt),rn.stdout or "")) else "FAIL"
        else: beh="LINK_FAIL"; bdiag=lk.stdout+lk.stderr
    else: bdiag=tc.stdout+tc.stderr
    print(f"{addr} {st} {beh}")
    win = st.startswith(("MATCH","RELOCATION_MATCH")) and beh=="PASS"
    print(f"RESULT: {'WIN' if win else 'NOWIN'} {st} {beh}")
    if not win and (diag or bdiag): print("DIAG:", (diag or bdiag)[:1500])

if __name__=="__main__": main()
