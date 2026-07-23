#!/usr/bin/env python3
"""Definitive typed HEADER decoder using the CONFIRMED name/type schema (seed-0 tags).
Reads [tag][value] per field in call order and prints value across saves."""
import savecrc, struct

# (name, kind) — kind: L=long4, B=bool1, F=float4, S=asciicstr, W=widecstr, R=CFloatRange(12b:3 floats)
SCHEMA = [
 ("WorldName","S"),
 ("WorldFrame","L"),
 ("TeleportingEnabled","B"),
 ("SavingEnabled","B"),
 ("ExperienceSpendingEnabled","B"),
 ("CreatureGenerationEnabled","B"),
 ("CreatureGenerationDisabledGroups","L"),
 ("HeroSleepingEnabled","B"),
 ("MapTableShowQuestCardsOnUsed","B"),
 ("MiniMapEnabled","B"),
 ("MiniMapActiveBeforeDisabled","B"),
 ("GuildMasterMessagesEnabled","B"),
 ("SummonerDeathExplosionAffectsHero","B"),
 ("MostRecentSaveType","L"),
 ("MostRecentSaveTypeBeforeManualSave","L"),
 ("MostRecentManualSaveName","W"),
 ("SaveGameMarkerPos","R"),
 ("SaveGameMarkerAngleXY","F"),
 ("GuildSealRecallPos","R"),
 ("GuildSealRecallAngleXY","F"),
 ("CurrentRegionName","S"),
 ("CurrentRegionMinimapGraphicName","S"),
 ("TotalTimePlayed","F"),
]
def cstr(d,o):
    e=d.index(b'\0',o); return d[o:e].decode('latin1'), e+1
def wstr(d,o):
    r=[]
    while o+1<len(d):
        w=struct.unpack_from('<H',d,o)[0]; o+=2
        if w==0: break
        r.append(chr(w) if 32<=w<127 else '?')
    return ''.join(r), o
def decode(fn):
    d=open(fn,'rb').read(); e=d.index(b'\0'); o=e+1
    seclen=struct.unpack_from('<I',d,o)[0]; o+=4
    res=[]; ok=True
    for name,k in SCHEMA:
        exp=savecrc.getcrc(name,0)
        if o+4>len(d): res.append((name,k,'<EOF>','tagmiss')); ok=False; break
        tag=struct.unpack_from('<I',d,o)[0]
        match = (tag==exp)
        o+=4
        if k=='S': v,o=cstr(d,o)
        elif k=='W': v,o=wstr(d,o)
        elif k=='L': v=struct.unpack_from('<I',d,o)[0]; o+=4
        elif k=='F': v=round(struct.unpack_from('<f',d,o)[0],4); o+=4
        elif k=='B': v=d[o]; o+=1
        elif k=='R':
            v=tuple(round(x,3) for x in struct.unpack_from('<3f',d,o)); o+=12
        res.append((name,k,v,'OK' if match else 'TAGMISMATCH(%08x!=%08x)'%(tag,exp)))
        if not match: ok=False
    return res, seclen, len(d), o, ok

FILES=['test_auto.header.bin','default_auto.header.bin','corn_manual.header.bin','corn_autoqs.header.bin']
D={fn:decode(fn) for fn in FILES}
labels=['test','default','manual','autoqs']
print("%-34s %-3s | %-16s %-16s %-16s %-16s"%('field','ty',*labels))
for i in range(len(SCHEMA)):
    row=[]; name=SCHEMA[i][0]; k=SCHEMA[i][1]
    for fn in FILES:
        res=D[fn][0]
        if i<len(res):
            _,_,v,st=res[i]
            s=str(v)
            if 'MISMATCH' in st: s='!'+s
            row.append(s[:16])
        else: row.append('-')
    print("%-34s %-3s | %-16s %-16s %-16s %-16s"%(name[:34],k,*row))
print()
for fn in FILES:
    _,sl,tot,off,ok=D[fn]
    print("%-24s seclen=%d total=%d parsed_to=%d rem=%d  ALL_TAGS_OK=%s"%(fn,sl,tot,off,tot-off,ok))
