#include "DragListWidget.h"
#include <QtGui/QDrag>
#include <QtCore/QMimeData>

DragListWidget::DragListWidget(QWidget *parent) : QListWidget(parent)
{
	setViewMode(QListView::IconMode);
	setGridSize(QSize(64,64));
	setFlow(QListView::LeftToRight);
	setIconSize(QSize(48,48));
	setDragEnabled(true);
	setDragDropMode(QAbstractItemView::DragOnly);
	setWordWrap(true);
	setDropIndicatorShown(true);
}

//----------------------------------------------------------------------------------------

void DragListWidget::startDrag(Qt::DropActions supportedActions)
{
	QListWidgetItem *item = currentItem();
	QMimeData *mimeData = new QMimeData;
	mimeData->setText(currentItem()->text());
	// 设置拖拽物体为模型
	mimeData->setProperty("TYPE", QVariant("MODELS"));

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->exec(Qt::MoveAction);
}

//----------------------------------------------------------------------------------------

// 创建mesh的图标数据
void createImages(ImageMap& retlist, Ogre::StringVectorPtr pList)
{
	retlist.clear();
	//使用RTT渲染模型截图
	Ogre::TexturePtr texture = Ogre::TextureManager::getSingletonPtr()->createManual(
		"Tex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		Ogre::TEX_TYPE_2D, 256, 256, 0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET);

	Ogre::RenderTexture *rttTex = texture->getBuffer()->getRenderTarget();
	Ogre::SceneManager *mSceneManager = Ogre::Root::getSingletonPtr()->
		createSceneManager(Ogre::ST_EXTERIOR_CLOSE, "TextureMgr");

	Ogre::Light *mLight = mSceneManager->createLight("DisplayLight");
	mLight->setDirection(-1, -1, -1);
	mLight->setDiffuseColour(1, 1, 1);
	mLight->setType(Ogre::Light::LT_DIRECTIONAL);

	Ogre::Camera* rttCam = mSceneManager->createCamera("TextureCam");
	rttCam->setNearClipDistance(0.01F);
	rttCam->setFarClipDistance(0);
	rttCam->setAspectRatio(1);
	rttCam->setFOVy(Ogre::Degree(90));
	rttCam->setPosition(0,0,1);
	rttCam->lookAt(0,0,0);

	Ogre::Viewport *mVp = rttTex->addViewport(rttCam);
	mVp->setClearEveryFrame(true);
	mVp->setBackgroundColour(Ogre::ColourValue(0, 0, 0, 0));

	//获取模型
	Ogre::Entity *mEntity;
	unsigned char dataptr[300 * 300 * 6];
	unsigned char *dataptr2;
	Ogre::PixelBox pb(256, 256, 1, Ogre::PF_A8R8G8B8, dataptr);

	//获取模型名称
	EntityMap mEntityName;
	for(unsigned int i = 0; i < pList->size(); i++) {
		Ogre::String addstr = (*pList)[i];
		//没有找到该名称是加入
		if(mEntityName.find(addstr) == mEntityName.end())
			mEntityName.insert(EntityMap::value_type(addstr, addstr));
	}
	EntityMap::const_iterator it = mEntityName.begin();

	while(it != mEntityName.end()) {
		Ogre::String addstr = it->first;
		// 首先创建一个模型, 渲染, 并保存渲染图像, 之后将其删除
		// 模型初始位置在原点
		mEntity = mSceneManager->createEntity("srcEntity", addstr);
		Ogre::SceneNode *node = mSceneManager->getRootSceneNode();
		node->attachObject(mEntity);

		Ogre::Vector3 vSize = mEntity->getBoundingBox().getHalfSize();
		Ogre::Vector3 vCenter = mEntity->getBoundingBox().getCenter();

		vSize += Ogre::Vector3(vSize.z, vSize.z, vSize.z);
		float maxsize = std::max(std::max(vSize.x,vSize.y),vSize.z);
		vSize = Ogre::Vector3(0, 0, maxsize * 1.1f) + vCenter;
		// 修正摄像机位置和视点位置
		rttCam->setPosition(vSize);
		rttCam->lookAt(vCenter);

		try {
			rttTex->update();;
			//将摄像机内容拷贝到PixelBox里
			rttTex->copyContentsToMemory(pb, Ogre::RenderTarget::FB_FRONT);
			dataptr2 = new unsigned char[96 * 96 * 4];
			Ogre::PixelBox pb2(96,96,1,Ogre::PF_A8R8G8B8, dataptr2);
			Ogre::Image::scale(pb,pb2);

			addstr.erase(addstr.length() - 5, 5);
			retlist.insert(ImageMap::value_type(addstr.c_str(), dataptr2));
		}
		catch(...) {
		}
		node->detachObject(mEntity);
		mSceneManager->destroyEntity(mEntity);

		it++;
	}
	rttTex->removeAllViewports();
	Ogre::Root::getSingletonPtr()->destroySceneManager(mSceneManager);
	Ogre::TextureManager::getSingletonPtr()->unload(texture->getName());
	Ogre::TextureManager::getSingletonPtr()->remove(texture->getName());

	pList.setNull();
}

//----------------------------------------------------------------------------------------

// 将createImages创建的图表数据导入到QListWidget里
// 此函数调用之前必须调用createImages
void prepareEntityView(QListWidget *listView, ImageMap	&imageData)
{
	Ogre::String filename;
	Ogre::String itemname;
	ImageMap::iterator it = imageData.begin();

	while(it != imageData.end()) {
		QImage pImg(it->second, 96, 96, QImage::Format_ARGB32);
		QPixmap pMap = QPixmap::fromImage(pImg);
		QListWidgetItem *item = new QListWidgetItem(QIcon(pMap), it->first, listView);
		item->setToolTip(it->first);
		listView->addItem(item);
		delete [] it->second;
		it++;
	}
	imageData.clear();
}