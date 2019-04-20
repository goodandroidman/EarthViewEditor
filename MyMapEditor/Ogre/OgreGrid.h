#ifndef  OGREGRID_H
#define OGREGRID_H

#include "../Dependence/Common.h"

//用于绘制地表辅助网格
class OgreGrid
{
public:
	// gridX X方向方格个数
	// gridZ Z方向方格个数
	// blockSize 方格宽度
	OgreGrid(Ogre::SceneManager *mSceneMgr, int gridX, int gridZ,
		int blockSize, const Ogre::ColourValue &gridColor);
	~OgreGrid();

	void setPosition(const Ogre::Vector3 &pos);
	void setVisible(bool visible);
	bool isVisible();

	// 获取网格的包围盒, 射线检测使用
	Ogre::AxisAlignedBox getBoundingbox();

private:
	Ogre::SceneManager	    *mSceneManager;
	Ogre::ManualObject		*mGridMesh;
	Ogre::SceneNode			*mNode;

	bool								mVisible;
};

#endif //OGREGRID_H