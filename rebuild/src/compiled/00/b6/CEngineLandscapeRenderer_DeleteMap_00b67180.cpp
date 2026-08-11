struct CMapResource {
    virtual void* Destroy(unsigned int flags);
};

struct CEngineMap {
    CMapResource* res;
};

struct CEngineLandscapeRenderer {
    void DeleteMap(CEngineMap* map);
};

void CEngineLandscapeRenderer::DeleteMap(CEngineMap* map)
{
    CMapResource* r = map->res;
    if (r != 0)
        r->Destroy(1);
    map->res = 0;
}