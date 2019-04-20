#include "Util.h"
#include <Ogre.h>

void Util::loadResouse(const std::string &path, const std::string &type, 
					   const std::string &groupName, bool load , bool recursive)
{
	//�ֶ�������Դ
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path,type,groupName,recursive);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(groupName);

	if(load)	// ��Ҫֱ��ȫ������
		Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(groupName);
}

//---------------------------------------------------------------------------------------------------------

// ж����Դ
void Util::unloadResouse(const std::string &groupName)
{
	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(groupName);
}
