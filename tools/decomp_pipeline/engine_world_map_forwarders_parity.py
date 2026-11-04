"""Native engine entry and donor-identified world-map boundary identities."""
SPECS=[
 ('CloseStaticMapFile','XZ',0xb23db0,'CloseStaticMapFile','XZ',0xb40000,'',''),
 ('SetStaticMapFileForUse','ABVCWideString@@@Z',0xb23dc0,'SetStaticMapFileForUse','ABVCWideString@@@Z',0xb428e0,'const CWideString& path','path'),
 ('SetWorld','PBVCIDrawWorld@@@Z',0xb23e00,'SetWorld','PBVCIDrawWorld@@@Z',0xb42ed0,'const CIDrawWorld* world','world'),
 ('AddMap','JJ@Z',0xb23e10,'AddMap','JJ@Z',0xb42d10,'long x,long y','x,y'),
 ('RemoveMap','J@Z',0xb23e20,'DeleteMap','J@Z',0xb42b10,'long id','id'),
 ('SetMapAsLoaded','J@Z',0xb23e30,'OnGameMapLoaded','J@Z',0xb401e0,'long id','id'),
 ('SetMapVisibility','J_N@Z',0xb23e40,'SetMapVisibility','J_N@Z',0xb42390,'long id,bool visible','id,visible'),
 ('SetMapVisibilityAllMaps','_N@Z',0xb23e50,'SetMapVisibilityAllMaps','_N@Z',0xb423b0,'bool visible','visible'),
 ('SetMapAsUnloaded','J@Z',0xb23e60,'OnGameMapUnloaded','J@Z',0xb40230,'long id','id')]
GLOBAL='?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A'
def parent(s):return '?'+s[0]+'@CEngine@@UAEX'+s[1]
def owner(s):return '?'+s[3]+'@CEngineWorldMap@@QAEX'+s[4]
