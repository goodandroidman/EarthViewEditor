#include "Util.h"
#include <Ogre.h>

void Util::loadResouse(const std::string &path, const std::string &type, 
					   const std::string &groupName, bool load , bool recursive)
{
	//手动加载资源
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path,type,groupName,recursive);
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(groupName);

	if(load)	// 需要直接全部加载
		Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(groupName);
}

//---------------------------------------------------------------------------------------------------------

// 卸载资源
void Util::unloadResouse(const std::string &groupName)
{
	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(groupName);
}
