// CLandscapeBackgroundLoadTask::ReleaseFileBlockLoad  @ 0x00bdc000

struct CFileBlockManager
{
	void Release(int handle);
};

struct CLandscapeBackgroundLoadTask
{
	char pad00[0x20];
	CFileBlockManager* manager;   // +0x20
	char pad24[0x30 - 0x24];
	int  fileHandle;              // +0x30
	char pad34[0x5e - 0x34];
	char loaded;                  // +0x5e

	void ReleaseFileBlockLoad();
};

void CLandscapeBackgroundLoadTask::ReleaseFileBlockLoad()
{
	if (this->loaded)
	{
		this->manager->Release(this->fileHandle);
		this->loaded = 0;
	}
}