import sys, struct, re
path=sys.argv[1]
data=open(path,'rb').read()
assert data[:4]==b'XBEH', "not an XBE"
base=struct.unpack_from('<I',data,0x104)[0]
entry_enc=struct.unpack_from('<I',data,0x128)[0]
nsec=struct.unpack_from('<I',data,0x11C)[0]
sec_addr=struct.unpack_from('<I',data,0x120)[0]
# XBE entry XOR keys: retail 0xA8FC57AB, debug 0x94859D4B
for k,name in [(0xA8FC57AB,'retail'),(0x94859D4B,'debug'),(0xE682F45B,'chihiro')]:
    ep=entry_enc^k
    if base<=ep<base+0x2000000:
        print(f"BaseAddr=0x{base:08X}  EntryPoint=0x{ep:08X} ({name} key)")
        break
print(f"Sections: {nsec}")
sec_off=sec_addr-base
for i in range(nsec):
    o=sec_off+i*0x38
    flags,va,vsz,raw,rawsz=struct.unpack_from('<IIIII',data,o)
    nameaddr=struct.unpack_from('<I',data,o+0x14)[0]
    noff=nameaddr-base
    nm=data[noff:noff+20].split(b'\x00')[0].decode('latin-1','replace')
    print(f"  {nm:16s} VA=0x{va:08X} vsz=0x{vsz:06X} raw=0x{raw:08X} rawsz=0x{rawsz:06X} flags=0x{flags:X}")
print("="*50)
# strings scan
kw=[b'joystick',b'EGameAction',b'GameAction',b'GAME_ACTION',b'ACTION_',b'Controller',b'JoyPad',b'joypad',b'Gamepad',b'button',b'Button',b'RedefineKeys',b'Redefine',b'Rumble',b'Trigger',b'ThumbStick',b'DPad',b'.ini',b'BindAxis',b'InputMap',b'KeyMap',b'CInput']
strs=re.findall(rb'[\x20-\x7e]{4,}', data)
seen=set()
for s in strs:
    for k in kw:
        if k in s and s not in seen:
            seen.add(s)
            print(s.decode('latin-1'))
            break
