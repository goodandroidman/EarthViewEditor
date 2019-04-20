#ifndef  DRAGLISTWIDGET_H
#define DRAGLISTWIDGET_H

#include <Ogre.h>
#include <QtCore/QStringList>
#include <QtGui/QListWidget>

/**
* 模型列表
**/

typedef std::map<QString, unsigned char *> ImageMap;
typedef std::map<Ogre::String, Ogre::String> EntityMap;

class DragListWidget : public QListWidget
{
	Q_OBJECT
public:
	DragListWidget(QWidget *parent = 0);

protected:
	void startDrag(Qt::DropActions supportedActions);
};

// 创建mesh的图标数据
void createImages(ImageMap& retlist, Ogre::StringVectorPtr pList);

// 将createImages创建的图表数据导入到QListWidget里
// 此函数调用之前必须调用createImages
void prepareEntityView(QListWidget *listView, ImageMap	&imageData);

#endif //DRAGLISTWIDGET_H