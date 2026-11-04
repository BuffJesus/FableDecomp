"""Generated BIGB font bank (schema 105) in the layout of native_text_retail_schema_fixture.create_archive:
two type-0 entries (indices 1 and 2) whose payloads use the static-font entry format the accepted upload
fixture feeds CStaticFontBank::CStaticFontBank (name, metrics, glyph banks of 22-byte records, A8R8G8B8 pixels)."""
from pathlib import Path
import struct,hashlib
FONTS=[dict(name=b'UIFontSmall',height=12,max_height=14,width=64,tex_height=32),dict(name=b'UIFontLarge',height=18,max_height=20,width=128,tex_height=64)]
def font_payload(n,f):
    u=lambda *v:struct.pack('<'+'I'*len(v),*v)
    b=f['name']+b'\0'+u(f['height'],400)+b'\0'+u(f['max_height'],f['width'],f['tex_height'],0,65535,2)
    for bank in range(2):
        b+=u(bank,0x10000,0x10000+64)
        for i in range(64):
            left=(i%7)*0.1;top=(i%5)*0.1
            b+=struct.pack('<4f',left,top,left+0.07,top+0.07)+struct.pack('<3h',(i%5)-2,i%11+n,(i%13)-3)
    pixels=f['width']*f['tex_height']*4
    b+=u(pixels+18)+b'\xf1'*18+bytes((n*7+k*13)&255 for k in range(pixels))
    return b
def create_font_archive(destination,alignment=1):
    assert alignment in (1,4,16,2048)
    u=lambda *v:struct.pack('<'+'I'*len(v),*v);blob=lambda b:u(len(b))+b
    b=bytearray(16);entries=[]
    for n,f in enumerate(FONTS):
        payload=font_payload(n,f);b+=bytes((-len(b))%alignment)
        entries.append(dict(index=n+1,type=0,offset=len(b),size=len(payload),symbol=b'FONT_'+f['name'].upper()));b+=payload
    b+=bytes((-len(b))%alignment)
    at=len(b);toc=u(1,0,len(entries))
    for e in reversed(entries):toc+=u(42,e['index'],e['type'],e['size'],e['offset'],0)+blob(e['symbol'])+u(0,0)+blob(b'')
    b+=toc;footer=len(b);b+=u(1)+b'FONT_TEST\0'+u(105,len(entries),at,len(toc),alignment)
    struct.pack_into('<4sIII',b,0,b'BIGB',100,footer,len(b)-footer)
    Path(destination).write_bytes(b)
    return dict(entries=[{k:(v.decode() if isinstance(v,bytes) else v) for k,v in e.items()} for e in entries],alignment=alignment,sha256=hashlib.sha256(b).hexdigest())
if __name__=='__main__':
    import sys,json;print(json.dumps(create_font_archive(sys.argv[1]),indent=1))
