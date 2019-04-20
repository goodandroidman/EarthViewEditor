#include "EntityViewWidget.h"

EntityViewWidget::EntityViewWidget(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	prepareEntityView();
}

//----------------------------------------------------------------------------------------

void EntityViewWidget::prepareEntityView()
{
	clear();
	mImageData.clear();

	//添加模型图标
	Ogre::StringVectorPtr pList = Ogre::ResourceGroupManager::getSingletonPtr()
		->findResourceNames(RESOURCE_GROUP_MESH, "*.mesh", false);
	createImages(mImageData, pList);
	::prepareEntityView(mEntityView, mImageData);
}

//----------------------------------------------------------------------------------------

void EntityViewWidget::clear()
{
	mEntityView->clear();
}

//----------------------------------------------------------------------------------------

void EntityViewWidget::setupUi(QWidget *entityView)
{
	entityView->resize(280, 500);
	gridLayout = new QGridLayout(entityView);
	gridLayout->setSpacing(0);
	gridLayout->setMargin(2);

	mEntityView = new DragListWidget(entityView);
	gridLayout->addWidget(mEntityView, 0, 0, 1, 1);

	setAcceptDrops(true);
}

