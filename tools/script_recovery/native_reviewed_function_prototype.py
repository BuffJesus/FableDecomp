"""Apply native/source-pinned PDB void prototypes to registered helper signatures."""
import hashlib
import re
from pathlib import Path
from tools.script_recovery.native_function_parameters import function_parameters
ROOT=Path(__file__).resolve().parents[2]

def reviewed_function_parameters(fn, unit, rdata, *, root=ROOT):
    signature=function_parameters(fn['decompile'],member=True)
    row=unit.get('reviewedFunctionPrototypes',{}).get(str(fn['address']).lower())
    if row is None:return signature
    def require(ok,message):
        if not ok:raise ValueError('Reviewed function prototype: '+message)
    address=int(fn['address'],16);end=int(fn['bodyEndExclusive'],16)
    require(int(row['address'],16)==address and end-address==row['bodyExtent'],'native extent changed')
    require(hashlib.sha256(fn['decompile'].encode('utf-8')).hexdigest()==row['sourceSha256'],'source changed')
    require(hashlib.sha256(rdata.bytes_at(address,end-address) or b'').hexdigest()==row['nativeSha256'],'native body changed')
    oracle=(root/row['oraclePath']).resolve()
    require(oracle.is_relative_to(root.resolve()) and oracle.is_file(),'oracle unavailable')
    raw=oracle.read_bytes();require(hashlib.sha256(raw).hexdigest()==row['oracleSha256'],'oracle changed')
    require(row['oracleSignature'] in raw.decode('utf-8-sig'),'oracle signature absent')
    require(row['returnType']=='void' and re.fullmatch(r'void [A-Za-z_][\w:~<>]*\((?:long|int) [A-Za-z_]\w*\)',row['oracleSignature']) is not None,'unsupported prototype')
    parameters=[{'native':p['native'],'type':p['type']}for p in signature['parameters']]
    require(parameters==row['expectedParameters'] and signature['returnKind']==row['expectedReturnKind'],'exported signature changed')
    returns=re.findall(r'\breturn\s+([^;]+);',fn['decompile'])
    require(all(re.fullmatch(r'[A-Za-z_]\w*',value.strip()) for value in returns),'effectful return expression unsupported')
    result=dict(signature);result['returnKind']=None;result['reviewedVoid']=True
    result['returnEvidence']={'oracle':row['oraclePath'],'signature':row['oracleSignature'],'evidence':row['evidence']}
    return result
