#include "MeshDockWidget.h"

MeshDockWidget::MeshDockWidget(QWidget *parent) : QDockWidget(parent)
{
	setupUI(this);
}

void MeshDockWidget::setupUI(QDockWidget *meshDockWidget)
{
	meshDockWidget->resize(225, 500);
	meshDockWidget->setMinimumSize(QSize(200, 100));
	mMeshWidget = new QWidget();
	QFont font;
	font.setFamily(tr("Ó×Ô²"));
	mMeshWidget->setFont(font);
	
	mGridLayout = new QGridLayout(mMeshWidget);
	mGridLayout->setMargin(2);	
	
	mMeshView = new EntityViewWidget(mMeshWidget);
	mGridLayout->addWidget(mMeshView, 0, 0, 1, 1);

	meshDockWidget->setWidget(mMeshWidget);
	meshDockWidget->setWindowTitle(tr("Ä£ĞÍ"));
}

//----------------------------------------------------------------------------------------

void MeshDockWidget::setEnabled(bool enable)
{
	mMeshView->setEnabled(enable);
}

//----------------------------------------------------------------------------------------
