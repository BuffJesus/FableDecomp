"""Native shader layout pointer-vector symbols and complete retail spans."""
vector='?$vector@PBVCVertexShaderConstantLayout@@V?$allocator@PBVCVertexShaderConstantLayout@@@_STL@@@_STL@@'
resize='?resize@'+vector+'QAEXIABQBVCVertexShaderConstantLayout@@@Z'
insert='?_M_fill_insert@'+vector+'QAEXPAPBVCVertexShaderConstantLayout@@IABQBV3@@Z'
overflow='?_M_insert_overflow@'+vector+'QAEXPAPBVCVertexShaderConstantLayout@@ABQBV3@ABU__true_type@2@I_N@Z'
fill='??$fill@PAPBVCVertexShaderConstantLayout@@PBV1@@_STL@@YIXPAPBVCVertexShaderConstantLayout@@0ABQBV1@@Z'
targets={resize:0x98d870,insert:0x98d000,overflow:0x98cde0,fill:0x98c120,
         '_memmove':0xbfeae6,'_malloc':0xbfea0e,'_free':0xbfea14}
specs=[('RegisterVertexShaderConstantLayout','?RegisterVertexShaderConstantLayout@CShaderRenderManager@@QAEXJPBVCVertexShaderConstantLayout@@@Z',0x98b230,73,1),
       ('UnregisterVertexShaderConstantLayouts','?UnregisterVertexShaderConstantLayouts@CShaderRenderManager@@QAEXXZ',0x98b280,51,1),
       ('layout_resize',resize,0x98d870,87,2),('layout_insert',insert,0x98d000,276,6),
       ('layout_insert',overflow,0x98cde0,216,4),('layout_insert',fill,0x98c120,31,0)]
sources={'RegisterVertexShaderConstantLayout':'00/98/CShaderRenderManager_RegisterVertexShaderConstantLayout_0098b230.cpp',
         'UnregisterVertexShaderConstantLayouts':'00/98/CShaderRenderManager_UnregisterVertexShaderConstantLayouts_0098b280.cpp',
         'layout_resize':'00/98/STLShaderLayoutPointerVector_Resize_0098d870.cpp',
         'layout_insert':'00/98/STLShaderLayoutPointerVector_FillInsert_0098d000.cpp'}
