struct COptimisedFace {
    unsigned long VertexIndices[3];
    void RemapIndex(unsigned long oldIndex, unsigned long newIndex);
};
void COptimisedFace::RemapIndex(unsigned long oldIndex, unsigned long newIndex)
{
    if (VertexIndices[0] == oldIndex) VertexIndices[0] = newIndex;
    if (VertexIndices[1] == oldIndex) VertexIndices[1] = newIndex;
    if (VertexIndices[2] == oldIndex) VertexIndices[2] = newIndex;
}