#!/usr/bin/env python3
"""Verify authored candidates (byte-match + behaviour) and auto-land the wins.
Usage: python verify_and_land.py <workflow_output.json> <oracle.tsv>
       [--land] [--prune-outside-manifest]
Without --land: dry-run report only. With --land: writes src/tests/catalog/oracle for wins.
Candidates without an authoritative manifest function start are always rejected. The prune
flag removes already-landed outside-manifest rows from this specific authoring payload."""
import csv, json, re, subprocess, os, html, sys
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
VC = Path(r"D:\Tools\vc71")
SP = ROOT / "rebuild" / "build"
WORK_ROOT = SP / "landverify"; WORK_ROOT.mkdir(parents=True, exist_ok=True)
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from artifact_layout import shard_directory

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

def make_base(module, leaf, addr):
    """Generate a Windows-safe, address-unique candidate basename.

    Some STL/template module names are hundreds of characters after
    sanitization. NTFS path components are capped at 255 chars, so cap the
    descriptive portion and rely on the address suffix for uniqueness.
    """
    m = sanitize(module or "_global")
    l = sanitize(leaf or "function")
    base = f"{m[:80]}_{l[:48]}_{addr}"
    return base[:180]

def cataloged_addresses(catp):
    if not catp.exists():
        return set()
    return {m.group(1).lower() for m in re.finditer(r"Address\s*=\s*'([0-9a-fA-F]{8})'", catp.read_text(encoding="utf-8"))}

def insert_catalog_entries(text, entries):
    """Append generated blocks to $catalog without touching later arrays."""
    catalog_start = text.index("$catalog = @(")
    close = text.index("\n)", catalog_start)
    return text[:close] + "\n" + "\n".join(entries) + text[close:]

def manifest_addresses():
    path = ROOT / "rebuild" / "manifest" / "functions.tsv"
    return {
        row["address"].lower().replace("0x", "")
        for row in csv.DictReader(open(path, encoding="utf-8-sig"), delimiter="\t")
        if row.get("address")
    }

def prune_catalog_blocks(text, addresses):
    """Remove simple top-level catalog object blocks for the specified addresses."""
    lines = text.splitlines(keepends=True)
    output = []
    removed = 0
    index = 0
    while index < len(lines):
        if "[pscustomobject]@{" not in lines[index]:
            output.append(lines[index])
            index += 1
            continue
        block = [lines[index]]
        index += 1
        while index < len(lines):
            block.append(lines[index])
            index += 1
            if re.match(r"^\s*}\s*,?\s*$", block[-1]):
                break
        match = re.search(r"Address\s*=\s*'([0-9a-fA-F]{8})'", "".join(block))
        if match and match.group(1).lower() in addresses:
            removed += 1
        else:
            output.extend(block)
    return "".join(output), removed

def prune_outside_manifest(addresses, catp):
    """Undo candidates from one authoring payload that lack a catalog function start."""
    addresses = {address.lower().replace("0x", "") for address in addresses}
    if not addresses:
        return
    catalog, removed_blocks = prune_catalog_blocks(catp.read_text(encoding="utf-8"), addresses)
    catp.write_text(catalog, encoding="utf-8")

    oracle_path = ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv"
    oracle_rows = list(csv.DictReader(open(oracle_path, encoding="utf-8-sig"), delimiter="\t"))
    kept = [row for row in oracle_rows if row["address"].lower().replace("0x", "") not in addresses]
    with open(oracle_path, "w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(
            stream,
            delimiter="\t",
            fieldnames=["address", "name", "length", "bytes"],
            lineterminator="\n",
        )
        writer.writeheader()
        writer.writerows(kept)

    removed_files = 0
    for address in addresses:
        patterns = (
            (ROOT / "rebuild" / "src" / "compiled", f"*_{address}.cpp"),
            (ROOT / "rebuild" / "tests", f"*_{address}_test.cpp"),
        )
        for directory, pattern in patterns:
            for path in directory.rglob(pattern):
                path.unlink()
                removed_files += 1
    print(
        f"PRUNED OUTSIDE_MANIFEST addresses={len(addresses)} "
        f"catalog_blocks={removed_blocks} files={removed_files}"
    )

def main():
    outf=Path(sys.argv[1]); oraclef=Path(sys.argv[2]); land="--land" in sys.argv
    prune="--prune-outside-manifest" in sys.argv
    data=json.loads(outf.read_text(encoding="utf-8"))["result"]["authored"]
    orc={r["address"].lower():r for r in csv.DictReader(open(oraclef,encoding="utf-8-sig"),delimiter="\t")}
    e=env(); wins=[]
    PRAGMAS=["", '#pragma optimize("s",on)', '#pragma optimize("t",on)', '#pragma optimize("g",on)']
    catp = ROOT/"rebuild"/"build_candidates.ps1"
    known_manifest = manifest_addresses()
    outside_manifest = {
        c["address"].lower().replace("0x", "")
        for c in data
        if c["address"].lower().replace("0x", "") not in known_manifest
    }
    if prune:
        prune_outside_manifest(outside_manifest, catp)
    landed_addrs = cataloged_addresses(catp)
    BASE_FLAGS=["/O2","/Oy","/W3"]
    # Extra flag variants tried when the base flags miss. Retail Fable.exe was built
    # by VC7.1 QFE 4035, whose /GS stack-cookie and /Oa (assume-no-aliasing) codegen
    # differ from our RTM 3077. Several functions only reach byte-parity with these;
    # BASE_FLAGS is always tried first so a win here can only upgrade a DIFFER, never
    # regress an existing match. The winning extra flags are recorded per-catalog-entry
    # (CompilerFlags) so the real build reproduces the exact retail bytes.
    EXTRA_FLAGSETS=[[], ["/GS"], ["/Oa"], ["/GS","/Oa"]]
    def parity_of(srctext, addr, leaf, retail, work, extra=()):
        sp=work/f"{addr}.cpp"; sp.write_text(srctext,encoding="utf-8")
        obj=work/f"{addr}.obj"; obj.unlink(missing_ok=True)
        cp=cl(["/nologo","/c"]+BASE_FLAGS+list(extra)+[f"/Fo{obj}",str(sp)],e)
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
        name=c.get("name") or (orc.get(addr,{}).get("name")) or f"sub_{addr}"; leaf=name.rsplit("::",1)[-1]
        if addr in outside_manifest:
            print(f"{addr:10} {'OUTSIDE_MANIFEST':16} {'-':6} {name}")
            continue
        if addr in landed_addrs: continue  # skip already-landed
        o=orc.get(addr)
        if not o: print(f"{addr:10} {'NO_ORACLE':16} {'-':6} {name}"); continue
        work=shard_directory(WORK_ROOT, addr, leaf=True); work.mkdir(parents=True, exist_ok=True)
        src0=vc71(c["source_cpp"]); tst=vc71(c["test_cpp"]); patt=c["pass_pattern"]; mod=c.get("module","_global")
        tp=work/f"{addr}.test.cpp"; tp.write_text(tst,encoding="utf-8")
        retail=bytes.fromhex(o["bytes"])
        # try base source/flags, then a pragma x extra-flag sweep (permuter integrated).
        src=src0; winflags=[]; st,_,_=parity_of(src, addr, leaf, retail, work)
        if not st.startswith(("MATCH","RELOCATION")):
            done=False
            for xf in EXTRA_FLAGSETS:
                for P in PRAGMAS:
                    if not xf and not P: continue  # already tried base above
                    cand=(P+"\n"+src0) if P else src0
                    st2,_,_=parity_of(cand, addr, leaf, retail, work, extra=xf)
                    if st2.startswith(("MATCH","RELOCATION")):
                        src=cand; winflags=xf
                        tag=(P.split('(')[1].split(',')[0] if P else "")
                        if xf: tag=(tag+" "+" ".join(xf)).strip()
                        st=st2+(f"[{tag}]" if tag else ""); done=True; break
                if done: break
        # recompile the winning (src, winflags) last so sobj holds the winning object
        if st.startswith(("MATCH","RELOCATION")):
            parity_of(src, addr, leaf, retail, work, extra=winflags)
        tobj=work/f"{addr}.t.obj"; exe=work/f"{addr}.exe"; tobj.unlink(missing_ok=True); exe.unlink(missing_ok=True)
        sobj=work/f"{addr}.obj"  # source object from the last parity_of(src) compile
        beh="TCC_FAIL"
        tc=cl(["/nologo","/c","/Od","/W3",f"/Fo{tobj}",str(tp)],e)
        if tc.returncode==0 and tobj.exists():
            def _run_exe():
                rn=subprocess.run([str(exe)],capture_output=True,text=True,env=e)
                return "PASS" if (rn.returncode==0 and re.search(re.escape(patt),rn.stdout or "")) else "FAIL"
            # 1) test-only link (self-contained test)
            lk=subprocess.run([str(VC/"bin"/"link.exe"),"/nologo",f"/out:{exe}",str(tobj)],capture_output=True,text=True,env=e)
            if lk.returncode==0 and exe.exists():
                beh=_run_exe()
            else:
                # 2) fallback: link source.obj + test.obj together (thunk/accessor where the
                #    tested fn lives in source and the test provides its _impl mocks). Duplicate
                #    symbols just fail again -> no regression vs the test-only LINK_FAIL.
                beh="LINK_FAIL"
                if sobj.exists():
                    exe.unlink(missing_ok=True)
                    lk2=subprocess.run([str(VC/"bin"/"link.exe"),"/nologo",f"/out:{exe}",str(sobj),str(tobj)],capture_output=True,text=True,env=e)
                    if lk2.returncode==0 and exe.exists():
                        beh=_run_exe()
        print(f"{addr:10} {st:16} {beh:6} {name}")
        if st.startswith(("MATCH","RELOCATION_MATCH")) and beh=="PASS":
            base=make_base(mod, leaf, addr)
            wins.append({"addr":addr,"name":name,"leaf":leaf,"module":mod,"status":st,"pass":patt,"base":base,
                         "src":src,"test":tst,"oracle_row":o,"flags":list(winflags)})
    print(f"\nWINS: {len(wins)}")
    for w in wins: print(f"  {w['addr']} {w['status']} {w['module']}::{w['leaf']}")
    if land and wins:
        srcdir=ROOT/"rebuild"/"src"/"compiled"; tdir=ROOT/"rebuild"/"tests"
        entries=[]
        for w in wins:
            src_shard=shard_directory(srcdir, w["addr"]); src_shard.mkdir(parents=True, exist_ok=True)
            test_shard=shard_directory(tdir, w["addr"]); test_shard.mkdir(parents=True, exist_ok=True)
            source_rel=f"{w['addr'][:2]}/{w['addr'][2:4]}/{w['base']}.cpp"
            test_rel=f"{w['addr'][:2]}/{w['addr'][2:4]}/{w['base']}_test.cpp"
            (srcdir/source_rel).write_text(w["src"],encoding="utf-8")
            (tdir/test_rel).write_text(w["test"],encoding="utf-8")
            flagline=""
            if w.get("flags"):
                flagline=f"        CompilerFlags = '{' '.join(BASE_FLAGS+w['flags'])}'\n"
            entries.append("    [pscustomobject]@{\n"
                f"        Address = '{w['addr']}'\n        Module = '{w['module']}'\n"
                f"        Source = '{source_rel}'\n        TestSource = '{test_rel}'\n"
                f"{flagline}        PassPattern = '{w['pass']}'\n    }}")
        # Insert into the $catalog array itself.  The build driver declares
        # other arrays after the catalog, so searching backwards from a later
        # marker can accidentally splice entries into $requestedAddresses.
        text=catp.read_text(encoding="utf-8")
        text=insert_catalog_entries(text, entries)
        catp.write_text(text,encoding="utf-8")
        # append oracle rows
        oracle_catalog = ROOT/"rebuild"/"oracles"/"auto-re-candidates.tsv"
        existing_oracle_addresses = {
            row["address"].lower().replace("0x", "")
            for row in csv.DictReader(open(oracle_catalog, encoding="utf-8-sig"), delimiter="\t")
        }
        with open(oracle_catalog,"a",encoding="utf-8",newline="") as f:
            for w in wins:
                if w["addr"] not in existing_oracle_addresses:
                    r=w["oracle_row"]; f.write(f"{w['addr']}\t{r['name']}\t{r['length']}\t{r['bytes']}\n")
                    existing_oracle_addresses.add(w["addr"])
        print(f"\nLANDED {len(wins)} wins into src/compiled + tests + catalog + oracle")
        for w in wins: print(f"  {w['base']}")

if __name__=="__main__": main()
