#!/usr/bin/env python3
"""Verify authored candidates (byte-match + behaviour) and auto-land the wins.
Usage: python verify_and_land.py <workflow_output.json> <oracle.tsv> [--land]
Without --land: dry-run report only. With --land: writes src/tests/catalog/oracle for wins."""
import csv, json, re, subprocess, os, html, sys
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
VC = Path(r"D:\Tools\vc71")
SP = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\1d84be3f-a25d-4888-923b-bcd7fa732dfb\scratchpad")
WORK = SP / "landverify"; WORK.mkdir(exist_ok=True)
OBJDUMP = r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe"
DS = re.compile(r"^\s*[0-9a-fA-F]+\s+<(.+)>:$"); DB = re.compile(r"^\s*[0-9a-fA-F]+:\s+((?:[0-9a-fA-F]{2}\s+)+)")
RL = re.compile(r"^([0-9a-fA-F]{8})\s+\S+\s+.+$")

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

def cl(args,e): return subprocess.run([str(VC/"bin"/"cl.exe")]+args,capture_output=True,text=True,env=e)

def sanitize(s): return re.sub(r"[^A-Za-z0-9]","",s)

def main():
    outf=Path(sys.argv[1]); oraclef=Path(sys.argv[2]); land="--land" in sys.argv
    data=json.loads(outf.read_text(encoding="utf-8"))["result"]["authored"]
    orc={r["address"].lower():r for r in csv.DictReader(open(oraclef,encoding="utf-8-sig"),delimiter="\t")}
    e=env(); wins=[]
    PRAGMAS=["", '#pragma optimize("s",on)', '#pragma optimize("t",on)', '#pragma optimize("g",on)']
    landed_addrs={p.stem.split("_")[-1].lower() for p in (ROOT/"rebuild"/"src"/"compiled").glob("*.cpp")}
    def parity_of(srctext, addr, leaf, retail):
        sp=WORK/f"{addr}.cpp"; sp.write_text(srctext,encoding="utf-8")
        obj=WORK/f"{addr}.obj"; obj.unlink(missing_ok=True)
        cp=cl(["/nologo","/c","/O2","/Oy","/W3",f"/Fo{obj}",str(sp)],e)
        if cp.returncode or not obj.exists(): return "SRC_FAIL",None,None
        try: built,sec,_=obj_text(obj,leaf)
        except Exception: return "OBJDUMP_ERR",None,None
        rel=obj_relocs(obj,sec)
        if retail==built: return "MATCH",built,sec
        if mask(retail,rel)==mask(built,rel): return "RELOCATION_MATCH",built,sec
        return f"DIFFER({len(built)}v{len(retail)})",built,sec
    print(f"{'addr':10} {'parity':16} {'behav':6} name")
    for c in data:
        addr=c["address"].lower().replace("0x","")
        name=c["name"]; leaf=name.rsplit("::",1)[-1]
        if addr in landed_addrs: continue  # skip already-landed
        o=orc.get(addr)
        if not o: print(f"{addr:10} {'NO_ORACLE':16} {'-':6} {name}"); continue
        src0=vc71(c["source_cpp"]); tst=vc71(c["test_cpp"]); patt=c["pass_pattern"]; mod=c.get("module","_global")
        tp=WORK/f"{addr}.test.cpp"; tp.write_text(tst,encoding="utf-8")
        retail=bytes.fromhex(o["bytes"])
        # try base source, then a pragma sweep (permuter flag-sweep integrated)
        src=src0; st,_,_=parity_of(src, addr, leaf, retail)
        if not st.startswith(("MATCH","RELOCATION")):
            for P in PRAGMAS[1:]:
                cand=P+"\n"+src0
                st2,_,_=parity_of(cand, addr, leaf, retail)
                if st2.startswith(("MATCH","RELOCATION")):
                    src=cand; st=st2+f"[{P.split('(')[1].split(',')[0]}]"; break
        tobj=WORK/f"{addr}.t.obj"; exe=WORK/f"{addr}.exe"; tobj.unlink(missing_ok=True); exe.unlink(missing_ok=True)
        beh="TCC_FAIL"
        tc=cl(["/nologo","/c","/Od","/W3",f"/Fo{tobj}",str(tp)],e)
        if tc.returncode==0 and tobj.exists():
            lk=subprocess.run([str(VC/"bin"/"link.exe"),"/nologo",f"/out:{exe}",str(tobj)],capture_output=True,text=True,env=e)
            if lk.returncode==0 and exe.exists():
                rn=subprocess.run([str(exe)],capture_output=True,text=True,env=e)
                beh="PASS" if (rn.returncode==0 and re.search(re.escape(patt),rn.stdout or "")) else "FAIL"
            else: beh="LINK_FAIL"
        print(f"{addr:10} {st:16} {beh:6} {name}")
        if st.startswith(("MATCH","RELOCATION_MATCH")) and beh=="PASS":
            base=f"{sanitize(mod)}_{sanitize(leaf)[:36]}_{addr}"
            wins.append({"addr":addr,"name":name,"leaf":leaf,"module":mod,"status":st,"pass":patt,"base":base,
                         "src":src,"test":tst,"oracle_row":o})
    print(f"\nWINS: {len(wins)}")
    for w in wins: print(f"  {w['addr']} {w['status']} {w['module']}::{w['leaf']}")
    if land and wins:
        srcdir=ROOT/"rebuild"/"src"/"compiled"; tdir=ROOT/"rebuild"/"tests"
        entries=[]
        for w in wins:
            (srcdir/f"{w['base']}.cpp").write_text(w["src"],encoding="utf-8")
            (tdir/f"{w['base']}_test.cpp").write_text(w["test"],encoding="utf-8")
            entries.append("    [pscustomobject]@{\n"
                f"        Address = '{w['addr']}'\n        Module = '{w['module']}'\n"
                f"        Source = '{w['base']}.cpp'\n        TestSource = '{w['base']}_test.cpp'\n"
                f"        PassPattern = '{w['pass']}'\n    }}")
        # insert into catalog before closing ')'
        catp=ROOT/"rebuild"/"build_candidates.ps1"; text=catp.read_text(encoding="utf-8")
        marker="$oldPath = $env:PATH"; idx=text.index(marker); close=text.rindex("\n)",0,idx)
        text=text[:close]+"\n"+"\n".join(entries)+text[close:]
        catp.write_text(text,encoding="utf-8")
        # append oracle rows
        with open(ROOT/"rebuild"/"oracles"/"auto-re-candidates.tsv","a",encoding="utf-8",newline="") as f:
            for w in wins:
                r=w["oracle_row"]; f.write(f"{w['addr']}\t{r['name']}\t{r['length']}\t{r['bytes']}\n")
        print(f"\nLANDED {len(wins)} wins into src/compiled + tests + catalog + oracle")
        for w in wins: print(f"  {w['base']}")

if __name__=="__main__": main()
