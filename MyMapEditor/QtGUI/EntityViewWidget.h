#ifndef  ENTITYVIEWWIDGET_H
#define ENTITYVIEWWIDGET_H

#include "DragListWidget.h"
#include "../Dependence/Common.h"

#include <QtGui/QGridLayout>
#include <QtGui/QWidget>
#include <QtGui/QListWidget>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtGui/QLabel>
#include <QtCore/QStringList>
#include <Ogre.h>
#include <map>

/**
	����ģ�͵�����ͼ����.
*/

class EntityViewWidget : public QWidget
{
public :
	EntityViewWidget(QWidget *parent);

	void prepareEntityView();
	void clear();

private:
	void setupUi(QWidget *entityView);

private:
	QGridLayout *gridLayout;
	DragListWidget *mEntityView;

	ImageMap	mImageData;
};


#endif //ENTITYVIEWWIDGET_H