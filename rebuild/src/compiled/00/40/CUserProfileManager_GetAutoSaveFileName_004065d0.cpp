struct CWideString { int d; CWideString(const unsigned short* lit); };
struct CUserProfileManager { CWideString GetAutoSaveFileName(); };
CWideString CUserProfileManager::GetAutoSaveFileName()
{ return CWideString((const unsigned short*)0x122de80); }