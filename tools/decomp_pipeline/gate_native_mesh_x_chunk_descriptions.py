"""Fresh VC71 parity gate: Mesh X chunk GetChunkDescription overrides 0x00AA7010..0x00AA7350 (TextureIndex, MappingInfo, AnimatedPrimitive, BoundingSphere, BoundingBox, VertexData, UVTangent), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??_C@_0BA@MLDIEJDD@Bounding?5Sphere?$AA@':0x129e2e8,
 '??_C@_0BD@BFHKAAIJ@UV?5Tangent?5Vectors?$AA@':0x129e3ec,
 '??_C@_0BD@HEDKHL@Vertex?5Buffer?5Data?$AA@':0x129e368,
 '??_C@_0BD@LKEEFPDG@Animated?5Primitive?$AA@':0x129e2a4,
 '??_C@_0N@GDCFAHKF@Bounding?5Box?$AA@':0x129e328,
 '??_C@_0N@ONIPDHHN@Mapping?5Info?$AA@':0x129e264,
 '??_C@_0O@OFOKEHKN@Texture?5Index?$AA@':0x129e224,
}
ROOTS=[
 dict(name='c3dmeshfilextextureindexchunk_getchunkdescription',source=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunkDescriptions_00aa7010.cpp',symbol='?GetChunkDescription@C3DMeshFileXTextureIndexChunk@@UBE?BVCCharString@@XZ',address=0xaa7010,size=25,instructions=9,relocations=2),
 dict(name='c3dmeshfilexmappinginfochunk_getchunkdescription',source=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunkDescriptions_00aa7010.cpp',symbol='?GetChunkDescription@C3DMeshFileXMappingInfoChunk@@UBE?BVCCharString@@XZ',address=0xaa7050,size=25,instructions=9,relocations=2),
 dict(name='c3dmeshfilexanimatedprimitivechunk_getchunkdescription',source=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunkDescriptions_00aa7010.cpp',symbol='?GetChunkDescription@C3DMeshFileXAnimatedPrimitiveChunk@@UBE?BVCCharString@@XZ',address=0xaa7090,size=25,instructions=9,relocations=2),
 dict(name='c3dmeshfilexboundingspherechunk_getchunkdescription',source=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunkDescriptions_00aa7010.cpp',symbol='?GetChunkDescription@C3DMeshFileXBoundingSphereChunk@@UBE?BVCCharString@@XZ',address=0xaa7100,size=25,instructions=9,relocations=2),
 dict(name='c3dmeshfilexboundingboxchunk_getchunkdescription',source=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunkDescriptions_00aa7010.cpp',symbol='?GetChunkDescription@C3DMeshFileXBoundingBoxChunk@@UBE?BVCCharString@@XZ',address=0xaa7140,size=25,instructions=9,relocations=2),
 dict(name='c3dmeshfilexvertexdatachunk_getchunkdescription',source=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunkDescriptions_00aa7010.cpp',symbol='?GetChunkDescription@C3DMeshFileXVertexDataChunk@@UBE?BVCCharString@@XZ',address=0xaa7230,size=25,instructions=9,relocations=2),
 dict(name='c3dmeshfilexuvtangentchunk_getchunkdescription',source=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunkDescriptions_00aa7010.cpp',symbol='?GetChunkDescription@C3DMeshFileXUVTangentChunk@@UBE?BVCCharString@@XZ',address=0xaa7350,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-x-chunk-descriptions.json',[],
  'Mesh X chunk GetChunkDescription overrides 0x00AA7010..0x00AA7350 (TextureIndex, MappingInfo, AnimatedPrimitive, Boundin')
 print('NATIVE_MESH_X_CHUNK_DESCRIPTIONS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
