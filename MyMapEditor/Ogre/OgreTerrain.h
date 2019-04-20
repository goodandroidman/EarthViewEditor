#ifndef  OGRETERRAIN_H
#define OGRETERRAIN_H

#include "../Dependence/Common.h"

#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>

//用于绘制地形
class OgreTerrain
{
public:
	OgreTerrain(	Ogre::SceneManager *sceneMgr,
		Ogre::Root *root, Ogre::Camera *camera, MapInfo info);
	~OgreTerrain();

	void createTerrain();
	Ogre::Vector3 getPosition(Ogre::Ray ray);
	MapInfo getTerrainInfo() { return mMapInfo; }

private:

	void defineTerrain(long x, long y);
	void configureTerrainDefaults(Ogre::Light* light);
	void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);

private:
	Ogre::Root                                  *mRoot;    
	Ogre::SceneManager	               *mSceneManager;
	Ogre::Camera                             *mCamera;    
	Ogre::TerrainGlobalOptions    *mTerrainGlobals;
	Ogre::TerrainGroup                   *mTerrainGroup;

	MapInfo                                         mMapInfo;
};

#endif //OGRETERRAIN_H