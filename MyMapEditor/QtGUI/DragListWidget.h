#ifndef  DRAGLISTWIDGET_H
#define DRAGLISTWIDGET_H

#include <Ogre.h>
#include <QtCore/QStringList>
#include <QtGui/QListWidget>

/**
* ģ���б�
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

// ����mesh��ͼ������
void createImages(ImageMap& retlist, Ogre::StringVectorPtr pList);

// ��createImages������ͼ�����ݵ��뵽QListWidget��
// �˺�������֮ǰ�������createImages
void prepareEntityView(QListWidget *listView, ImageMap	&imageData);

#endif //DRAGLISTWIDGET_H