#include "OgreTerrain.h"

OgreTerrain::OgreTerrain(Ogre::SceneManager *sceneMgr,
						 Ogre::Root *root, Ogre::Camera *camera, MapInfo info) : 
mSceneManager(sceneMgr), mRoot(root), mCamera(camera), mMapInfo(info)
{
}

//----------------------------------------------------------------------------------------

OgreTerrain::~OgreTerrain()
{
}

//----------------------------------------------------------------------------------------

void OgreTerrain::createTerrain()
{
	mCamera->setPosition(Ogre::Vector3(0, mMapInfo.WORLDHEIGHT, 0));
	mCamera->lookAt(Ogre::Vector3(100, mMapInfo.WORLDHEIGHT, 0));
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(12500);

	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	{
		mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
	}

	Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
	lightdir.normalise();

	Ogre::Light* light = mSceneManager->createLight("TerrainLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(lightdir);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

	mSceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneManager, Ogre::Terrain::ALIGN_X_Z, 
		mMapInfo.TERRAINSIZE, mMapInfo.WORLDSIZE);
	mTerrainGroup->setFilenameConvention(Ogre::String("Editor-Terrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	configureTerrainDefaults(light);

	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);

	// sync load since we want everything in place when we start
	mTerrainGroup->loadAllTerrains(true);
}

//----------------------------------------------------------------------------------------

void OgreTerrain::defineTerrain(long x, long y)
{
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
	{
		mTerrainGroup->defineTerrain(x, y);
	}
	else
	{
		Ogre::Image img;
		getTerrainImage(x % 2 != 0, y % 2 != 0, img);
		mTerrainGroup->defineTerrain(x, y, &img);
	}
}

//----------------------------------------------------------------------------------------

void OgreTerrain::getTerrainImage(bool flipX, bool flipY, Ogre::Image &img)
{
	img.load(mMapInfo.HEIGHTMAP, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();
}

//----------------------------------------------------------------------------------------

void OgreTerrain::configureTerrainDefaults(Ogre::Light *light)
{
	mTerrainGlobals->setMaxPixelError(8);
	mTerrainGlobals->setCompositeMapDistance(1000);

	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneManager->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.terrainSize = mMapInfo.TERRAINSIZE;
	defaultimp.worldSize = mMapInfo.WORLDSIZE;
	defaultimp.inputScale = mMapInfo.WORLDHEIGHT;
	defaultimp.minBatchSize = 33;
	defaultimp.maxBatchSize = 65;

	//纹理
	defaultimp.layerList.resize(1);
	defaultimp.layerList[0].worldSize = 100;
	//diffuse texture with a specular map in the alpha channel
	//漫反射纹理
	defaultimp.layerList[0].textureNames.push_back(mMapInfo.DIFFUSE);
	//normal map with a height map in the alpha channe
	//ALPHA通道
	defaultimp.layerList[0].textureNames.push_back(mMapInfo.SPECIAL);
}

//----------------------------------------------------------------------------------------

Ogre::Vector3 OgreTerrain::getPosition(Ogre::Ray ray)
{
	Ogre::TerrainGroup::RayResult rayResult = mTerrainGroup->rayIntersects(ray);
	Ogre::Vector3 pos = rayResult.position;
	return pos;
}