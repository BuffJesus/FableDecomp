import sys, os, struct
SECTOR=2048; MAGIC=b"MICROSOFT*XBOX*MEDIA"
iso=sys.argv[1]; wanted=sys.argv[2]; outdir=sys.argv[3]
os.makedirs(outdir, exist_ok=True)
def find_base(f,size):
    f.seek(32*SECTOR)
    if f.read(20)==MAGIC: return 0
    return 0
with open(iso,'rb') as f:
    base=find_base(f,os.path.getsize(iso))
    f.seek(base+32*SECTOR+20); root_sec,root_size=struct.unpack('<II',f.read(8))
    def read_dir(sec,dsize,path,acc):
        f.seek(base+sec*SECTOR); table=f.read(dsize)
        def walk(pos):
            if pos*4+14>len(table): return
            left,right=struct.unpack_from('<HH',table,pos*4)
            ssec,fsize,attr,nlen=struct.unpack_from('<IIBB',table,pos*4+4)
            name=table[pos*4+14:pos*4+14+nlen].decode('latin-1','replace')
            if left: walk(left)
            full=path+'/'+name; isd=bool(attr&0x10)
            acc.append((full,isd,fsize,ssec))
            if isd and fsize and ssec: read_dir(ssec,fsize,full,acc)
            if right: walk(right)
        if dsize: walk(0)
    acc=[]; read_dir(root_sec,root_size,'',acc)
    for full,isd,fsize,ssec in acc:
        if isd: continue
        if wanted.lower() in full.lower():
            f.seek(base+ssec*SECTOR); data=f.read(fsize)
            out=os.path.join(outdir, full.strip('/').replace('/','__'))
            open(out,'wb').write(data)
            print(f"extracted {full} -> {out} ({fsize} bytes)")
