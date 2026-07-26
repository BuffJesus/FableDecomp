void CLandscapeBackgroundTreeNode::UpdateTesselation()
{
    if (!m_bTesselationUpdated)
    {
        if (m_pLoadTask != nullptr)
        {
            m_pLoadTask->Abort();
        }

        m_bTesselationUpdated = true;
    }
}