"""Preserve explicit float narrowing of scalar arithmetic expressions."""
import re
def narrow_arithmetic(text):
    # Walk inner casts first; a cast of a plain already-float snapshot is left
    # to the existing type erasure. Pointer reinterpretations are excluded.
    starts=list(re.finditer(r'\(float\)\s*(?=\()',text))
    for match in reversed(starts):
        opening=match.end();depth=0;end=None
        for i in range(opening,len(text)):
            if text[i]=='(':depth+=1
            elif text[i]==')':
                depth-=1
                if depth==0:end=i+1;break
        if end is None:continue
        body=text[opening:end]
        uncast=re.sub(r'\((?:float10|float)\)','',body)
        if re.search(r'\((?:int|uint|byte|bool|char|short|long|undefined\d*)\)',uncast):continue
        # Deliberately limit this first recovery to scalar add/subtract.
        # Calls, pointers, boolean expressions and integer conversions need
        # their own operand/precision evidence.
        if not re.fullmatch(r'[\w\s().+\-]+',uncast):continue
        if re.search(r'\w\s*\(',uncast):continue
        if not re.search(r'\w\s*[+\-]\s*\w|\)\s*[+\-]\s*\(',uncast):continue
        text=text[:match.start()]+'(string.unpack("<f", string.pack("<f", '+body+')))'+text[end:]
    return text
