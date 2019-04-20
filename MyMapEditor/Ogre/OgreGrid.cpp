#include "OgreGrid.h"
#include "OgreApp.h"

OgreGrid::OgreGrid(Ogre::SceneManager *mSceneMgr, int gridX, int gridZ, int blockSize, 
				   const Ogre::ColourValue &gridColor) : mSceneManager(mSceneMgr), mVisible(true)
{
	mGridMesh = mSceneManager->createManualObject("Grid");
	mGridMesh->setQueryFlags(HIDE_MASK);
	mNode = mSceneManager->getRootSceneNode()->createChildSceneNode();

	Ogre::MaterialManager &matMgr = Ogre::MaterialManager::getSingleton();
	Ogre::MaterialPtr pMaterial = matMgr.create("GridMat", 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	pMaterial->setLightingEnabled(false);
	pMaterial->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

	mGridMesh->begin("GridMat", Ogre::RenderOperation::OT_LINE_LIST);
	float maxXSize = gridX * blockSize;
	float maxZSize = gridZ * blockSize;
	// ÁÐ
	for (float x = -maxXSize / 2; x <= maxXSize / 2; x += blockSize)
	{
		mGridMesh->position(x, 0, maxZSize / 2);
		mGridMesh->colour(gridColor);
		mGridMesh->position(x, 0, -maxZSize / 2);
		mGridMesh->colour(gridColor);
		
	}

	// ÐÐ
	for (float z = -maxZSize / 2; z <= maxZSize / 2; z += blockSize)
	{
		mGridMesh->position(maxXSize / 2, 0, z);
		mGridMesh->colour(gridColor);
		mGridMesh->position(-maxXSize / 2, 0, z);
		mGridMesh->colour(gridColor);
	}

	mGridMesh->end();

	mNode->attachObject(mGridMesh);
}

//----------------------------------------------------------------------------------------

OgreGrid::~OgreGrid()
{
	mNode->detachObject(mGridMesh);
	mSceneManager->destroyManualObject(mGridMesh);
	mSceneManager->destroySceneNode(mNode);

	mGridMesh = 0;
	mNode = 0;
}

//----------------------------------------------------------------------------------------

void OgreGrid::setPosition(const Ogre::Vector3 &pos)
{
	mNode->setPosition(pos);
}

//----------------------------------------------------------------------------------------

void OgreGrid::setVisible(bool visible)
{
	mVisible = visible;
	mNode->setVisible(visible);
}

//----------------------------------------------------------------------------------------

bool OgreGrid::isVisible()
{
	return mVisible;
}

//----------------------------------------------------------------------------------------

Ogre::AxisAlignedBox OgreGrid::getBoundingbox()
{
	return mGridMesh->getBoundingBox();
}