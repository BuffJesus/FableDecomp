"""Certify only horizontal whitespace between adjacent C call parentheses.

Quoted literals and comments are opaque. This accepts no token, operator,
literal or call-inventory changes and supplies no native behavior proof.
"""
import hashlib,re
from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile,bind_st0_results,join_wrapped_statements

RAW=re.compile(r'(?:u8|u|U|L)?R"')

def canonical_call_spacing(text):
    out=[];plain=[];i=0;changes=0
    def flush():
        nonlocal changes
        s=''.join(plain);s,n=re.subn(r'(?<=\))[\t ]+(?=\()', '',s);changes+=n;out.append(s);plain.clear()
    while i<len(text):
        if RAW.match(text,i):raise ValueError('wrapped call correspondence: raw literal unsupported')
        if text[i] in '\"\'':
            flush();quote=text[i];start=i;i+=1
            while i<len(text):
                if text[i]=='\\':i+=2;continue
                if text[i]==quote:i+=1;break
                if text[i] in '\r\n':raise ValueError('wrapped call correspondence: multiline literal')
                i+=1
            else:raise ValueError('wrapped call correspondence: unterminated literal')
            out.append(text[start:i]);continue
        if text.startswith('//',i):
            flush();end=text.find('\n',i);end=len(text) if end<0 else end;out.append(text[i:end]);i=end;continue
        if text.startswith('/*',i):
            flush();end=text.find('*/',i+2)
            if end<0:raise ValueError('wrapped call correspondence: unterminated comment')
            end+=2;out.append(text[i:end]);i=end;continue
        plain.append(text[i]);i+=1
    flush();return ''.join(out),changes

def prove(fn,normalized):
    def require(ok,why):
        if not ok:raise ValueError('wrapped call correspondence: '+why)
    from tools.script_recovery.convert_quest_unit import _text_order_sites
    raw=fn['decompile'];expected=bind_st0_results(join_wrapped_statements(raw));require(normalized==expected,'joined binding input')
    left=bind_st0_results(normalise_typed_decompile(normalized));right=bind_st0_results(normalise_typed_decompile(raw));a,na=canonical_call_spacing(left);b,nb=canonical_call_spacing(right);require(a==b,'difference exceeds adjacent-parenthesis spacing')
    original=_text_order_sites(raw,fn);joined=_text_order_sites(normalized,fn);require(original is not None and joined is not None,'original call alignment');require([s[3] for s in original]==[s[3] for s in joined],'original call inventory')
    sha=lambda s:hashlib.sha256(s.encode()).hexdigest()
    return dict(kind='horizontal-call-parenthesis-spacing-only',originalSha256=sha(raw),joinedSha256=sha(normalized),normalizedJoinedSha256=sha(left),normalizedOriginalSha256=sha(right),canonicalSha256=sha(a),joinedSpacingChanges=na,originalSpacingChanges=nb,originalCallCount=len(original),allOriginalCallMetadataExact=True,literalsAndCommentsOpaque=True,limits='Whitespace/source call correspondence only; no frame, ABI, native behavior or lifetime proof.')
