#ifndef  MESHDOCKWIDGET_H
#define MESHDOCKWIDGET_H

#include "EntityViewWidget.h"

#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QWidget>

/**
* 用于显示模型列表
**/

class MeshDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	MeshDockWidget(QWidget *parent = 0);
	void setEnabled(bool enable);

private:
	void setupUI(QDockWidget *meshDockWidget);

private:
	QWidget                        *mMeshWidget;
	QGridLayout                 *mGridLayout;
	EntityViewWidget        *mMeshView;
};

#endif //MESHDOCKWIDGET_H