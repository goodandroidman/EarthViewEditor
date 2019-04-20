#ifndef  OGREGRID_H
#define OGREGRID_H

#include "../Dependence/Common.h"

//���ڻ��Ƶر�������
class OgreGrid
{
public:
	// gridX X���򷽸����
	// gridZ Z���򷽸����
	// blockSize ������
	OgreGrid(Ogre::SceneManager *mSceneMgr, int gridX, int gridZ,
		int blockSize, const Ogre::ColourValue &gridColor);
	~OgreGrid();

	void setPosition(const Ogre::Vector3 &pos);
	void setVisible(bool visible);
	bool isVisible();

	// ��ȡ����İ�Χ��, ���߼��ʹ��
	Ogre::AxisAlignedBox getBoundingbox();

private:
	Ogre::SceneManager	    *mSceneManager;
	Ogre::ManualObject		*mGridMesh;
	Ogre::SceneNode			*mNode;

	bool								mVisible;
};

#endif //OGREGRID_H