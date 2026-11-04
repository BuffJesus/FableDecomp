"""Generated streaming-font glyph data for the glyph fixture (layouts from Ego_r.pdb; not a retail font):
two fonts, glyph flags, 64-character glyph banks (odd bank indices LZO1X-1 compressed behind a 16-bit header,
bit 15 set), a glyph-data table in LZO-compressed chunks of 0x40 CStreamingGlyphData records, and a glyph
adjustment table.

Each font is also emitted as the bank-entry record CStreamingFontBank's constructor reads: name, FontHeight,
Weight, Italics (byte), MaxHeight, 1025 bank offsets, then the stored banks back to back.

File layout (little endian): 'SGLY', perChunk, numChunks, numAdjustments, dataBlobSize, numGlyphs, then chunk offsets
(numChunks+1 u32), data blob, adjustments (2 bytes each), then per font: FontHeight, fontIndex,
GlyphFlags[2048] u32, bank offsets[1024] u32 (0xFFFFFFFF = none) into the font's bank blob, bank blob size, blob, record size, record."""
import struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from lionhead_lz_compress import compress
from lionhead_lz import decompress
PER_CHUNK=0x40
def chars_for(font):
    c=set(range(0x20,0x7f))|{0xa0,0xe9,0x152,0x3a9}
    c|={0x41+0x100*k for k in range(1,11)}        # eleven characters share bucket 0x41 with 'A'
    c|={0x62+0x100*k for k in range(2,6)}
    if font==0:c|={0x25a1}
    else:c|={0x4e2d,0x4e00}
    return sorted(c)
def build():
    fonts=[];glyph_data=[];adjust=[(100,0),(87,-2),(125,3),(255,-128),(1,127),(50,5)]
    for f in range(2):
        chars=chars_for(f);index={}
        for ch in chars:
            glyph_data.append(ch*2654435761+f*97)
            index[ch]=len(glyph_data)               # 1-based glyph index
        fonts.append((chars,index))
    recs=b''
    for k,seed in enumerate(glyph_data):
        s=seed&0xffffffff
        ox,oy,w,h=(s>>0)%17-8,(s>>5)%17-8,(s>>9)%40,(s>>13)%40
        mem=(s>>3)&0x7fffff;adv=(s>>11)&0xff;big=(k%3==0)
        recs+=struct.pack('<4bI',ox,oy,w,h,mem|(adv<<23)|(big<<31))
    recs+=b'\0'*((-len(recs))%(PER_CHUNK*8))
    chunks=[recs[i:i+PER_CHUNK*8] for i in range(0,len(recs),PER_CHUNK*8)]
    blob=b'';offsets=[]
    for c in chunks:
        offsets.append(len(blob));z=compress(c);assert decompress(z,len(c))==c;blob+=z
    offsets.append(len(blob))
    out=b'SGLY'+struct.pack('<5I',PER_CHUNK,len(chunks),len(adjust),len(blob),len(glyph_data))+struct.pack('<%dI'%len(offsets),*offsets)+blob
    out+=b''.join(struct.pack('<Bb',s,b) for s,b in adjust)
    for f,(chars,index) in enumerate(fonts):
        flags=[0]*2048
        for ch in chars:flags[ch>>5]|=1<<(ch&31)
        banks={}
        for ch in chars:banks.setdefault(ch>>6,[]).append(ch)
        bank_blob=b'';bank_off=[0xffffffff]*1024;record_banks=b'';record_off=[]
        for b,members in sorted(banks.items()):
            first=min(c&63 for c in members);last=max(c&63 for c in members)
            glyphs=[0]*64
            for ch in members:glyphs[(ch&63)-first]=index[ch]|(((ch*7+f)%len(adjust))<<24)
            body=struct.pack('<HH',first,last-first+1)+struct.pack('<64I',*glyphs)
            assert len(body)==260
            if b%2:
                z=compress(body);assert len(z)<0x8000 and decompress(z,260)==body;stored=struct.pack('<H',0x8000|len(z))+z
            else:stored=struct.pack('<H',len(body))+body
            bank_off[b]=len(bank_blob);bank_blob+=stored+b'\0'*((-len(stored))%4)
            banks[b]=stored
        for b in range(1024):
            record_off.append(len(record_banks))
            if isinstance(banks.get(b),bytes):record_banks+=banks[b]
        record_off.append(len(record_banks))
        name=(b'StreamSmall',b'StreamLarge')[f]
        record=name+b'\0'+struct.pack('<2iBI',(12,18)[f],(400,700)[f],f,(14,21)[f])+struct.pack('<1025I',*record_off)+record_banks
        out+=struct.pack('<2I',(12,18)[f],f+1)+struct.pack('<2048I',*flags)+struct.pack('<1024I',*bank_off)+struct.pack('<I',len(bank_blob))+bank_blob+struct.pack('<I',len(record))+record
    return out
if __name__=='__main__':
    data=build();Path(sys.argv[1]).write_bytes(data);print(len(data))
