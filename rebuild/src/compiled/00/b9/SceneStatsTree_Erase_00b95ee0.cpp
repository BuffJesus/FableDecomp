#include "engine/CEngineScene.h"
namespace _STL {
typedef map<EEngineRenderLayer,CEngineSceneRenderStats>::value_type SceneStatsEntry;
typedef _Rb_tree<EEngineRenderLayer,SceneStatsEntry,_Select1st<SceneStatsEntry>,less<EEngineRenderLayer>,allocator<SceneStatsEntry> > SceneStatsTree;
template<> void SceneStatsTree::_M_erase(_Rb_tree_node<SceneStatsEntry>* node)
{
    while(node){
        _M_erase(static_cast<_Rb_tree_node<SceneStatsEntry>*>(node->_M_right));
        _Rb_tree_node<SceneStatsEntry>* left=static_cast<_Rb_tree_node<SceneStatsEntry>*>(node->_M_left);
        _Destroy(&node->_M_value_field);
        free(node);
        node=left;
    }
}
}
