bool CNetworkClient::IsFreeToRender() const
{
    if (LocalGame != false)
    {
        return !GameComponent->IsTimeForServerUpdate(LocalFrame);
    }

    return true;
}