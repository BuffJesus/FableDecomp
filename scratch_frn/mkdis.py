import struct,subprocess
def text(obj):
    data=open(obj,'rb').read()
    nsec=struct.unpack('<H',data[2:4])[0];opt=struct.unpack('<H',data[16:18])[0];off=20+opt
    for i in range(nsec):
        e=data[off+i*40:off+i*40+40]
        if e[:8].rstrip(b'\x00')==b'.text':
            size=struct.unpack('<I',e[16:20])[0];praw=struct.unpack('<I',e[20:24])[0]
            return data[praw:praw+size]
def dis(raw,tag):
    open(tag+'.bin','wb').write(raw)
    out=subprocess.run(['objdump','-D','-b','binary','-m','i386','-M','intel',tag+'.bin'],capture_output=True,text=True).stdout
    print('===',tag,len(raw),'===')
    started=False
    for l in out.splitlines():
        if '<.data>:' in l: started=True; continue
        if started and ':\t' in l: print(l)
retail=bytes.fromhex('558bec83ec14e8bbc310008b80dc000000d98080000000d8153cba2901dfe0f6c4417541db0550553701c745f40000003fd8c9d80dfcf22401d95df0ddd8d9e8d95df8d945f0d845f4db5dfcdb45fcd865f8d95dec8b45f03b45ec7503ff4dfc8b45fcc9c3a150553701ddd869c088090000c9c3')
dis(retail,'retail')
dis(text('src.obj'),'mine')
