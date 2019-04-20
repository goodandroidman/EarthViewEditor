#include "OgreWidget.h"
#include "MainWindow.h"
#include <QtGui/QApplication>
#include <QtGui/QPainter>

OgreWidget::OgreWidget(MapEditor* mainWindow, QWidget *parent) : 
QWidget(parent,Qt::WFlags(Qt::MSWindowsOwnDC)), mOgreApp(0), 
mMainWindow(mainWindow), mCurrentObject(0)
{
	// 设置这些属性来防止闪烁
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoBackground);

	// 需要设置鼠标追踪
	setMouseTracking(true);
	setFocusPolicy(Qt::WheelFocus);

	// 接收拖拽操作
	setAcceptDrops(true);
	setContextMenuPolicy( Qt::PreventContextMenu );

	setMinimumSize(600,400);

	update();
}

//----------------------------------------------------------------------------------------

OgreWidget::~OgreWidget(void)
{    
}

// 初始化Ogre显示, 在QtOgreCanvase构造之后调用
void OgreWidget::initOgre()
{
	if (!mOgreApp)
	{
		// RenderWindow必须在这里进行初始化, 否则会有问题
		mOgreApp = OgreApp::getSingletonPtr();
		mOgreApp->initRenderWindow((HWND)winId(), width(), height());
		mOgreApp->start();
	}

	// 设置定时器, 定时刷新
	startTimer(2);
}

//----------------------------------------------------------------------------------------

void OgreWidget::timerEvent (QTimerEvent *event)
{
	mOgreApp->update();
	update();
}

//----------------------------------------------------------------------------------------

void OgreWidget::paintEvent (QPaintEvent *event)
{
	mOgreApp->update();
}

//----------------------------------------------------------------------------------------

void OgreWidget::resizeEvent(QResizeEvent *event)
{
	mOgreApp->resize(width(), height());
	update();
}

//----------------------------------------------------------------------------------------

void OgreWidget::drawGrid()
{
	mOgreApp->drawGrid();
}

//----------------------------------------------------------------------------------------

bool OgreWidget::loadScene(const QString &filename)
{
	std::string fileName = filename.toStdString();
	if(mOgreApp->loadScene(fileName))
		return true;
	return false;
}

//----------------------------------------------------------------------------------------

void  OgreWidget::keyPressEvent(QKeyEvent*  evt)
{
	if (!mCurrentObject)
		mOgreApp->keyPressEvent(evt);
	switch (((QKeyEvent*)evt)->key()) {
		case Qt::Key_Delete :
			mOgreApp->destroyObject(mCurrentObject->mName);
	}
	QWidget::keyPressEvent(evt);
}

//----------------------------------------------------------------------------------------

void  OgreWidget::keyReleaseEvent(QKeyEvent*  evt)
{
	if (!mCurrentObject)
		mOgreApp->keyReleaseEvent(evt);
	QWidget::keyReleaseEvent(evt);
}

//----------------------------------------------------------------------------------------

void  OgreWidget::mousePressEvent(QMouseEvent*  evt)
{
	if (!mCurrentObject)
		mOgreApp->mousePressEvent(evt);
	QWidget::mousePressEvent(evt);
}

//----------------------------------------------------------------------------------------

void  OgreWidget::mouseReleaseEvent(QMouseEvent*  evt)
{
	if (!mCurrentObject)
		mOgreApp->mouseReleaseEvent(evt);
	QWidget::mouseReleaseEvent(evt);
}

//----------------------------------------------------------------------------------------

void  OgreWidget::mouseMoveEvent(QMouseEvent*  evt)
{
	if (!mCurrentObject)
		mOgreApp->mouseMoveEvent(evt);
	QWidget::mouseMoveEvent(evt);
}

//----------------------------------------------------------------------------------------

void  OgreWidget::wheelEvent(QWheelEvent*  evt)
{
	mOgreApp->wheelEvent(evt);
	QWidget::wheelEvent(evt);
}

//----------------------------------------------------------------------------------------

void OgreWidget::dragEnterEvent(QDragEnterEvent *event)
{
	const QMimeData *data = event->mimeData();
	QVariant type = data->property("TYPE");
	if(type == QVariant("MODELS")) {
		if(!mCurrentObject) {
			std::string meshName = data->text().toStdString();
			mCurrentObject = OgreApp::getSingletonPtr()->createObject(meshName);
		}
		event->accept();
	}
	else
		event->ignore();
}

//----------------------------------------------------------------------------------------

void OgreWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
}

//----------------------------------------------------------------------------------------

void OgreWidget::dragMoveEvent(QDragMoveEvent *event)
{
	QPoint pos = event->pos();
	{
		mCurrentObject->mNode->setPosition(mOgreApp->getPositionFromScreen(pos.x(), pos.y()));
	}

	event->accept();
}

//----------------------------------------------------------------------------------------

void OgreWidget::dropEvent(QDropEvent *event)
{
	mCurrentObject = 0;
	event->accept();
}

//----------------------------------------------------------------------------------------

void OgreWidget::setPropertyWindow(PropertyDockWidget *pd)
{
	mOgreApp->setPropertyWindow(pd);
}

//----------------------------------------------------------------------------------------

void OgreWidget::setListWindow(ListDockWidget* ld)
{
	mOgreApp->setListWindow(ld);
}

//----------------------------------------------------------------------------------------

bool OgreWidget::saveScene(const QString &filename)
{
	std::string fileName = filename.toStdString();
	if(mOgreApp->saveScene(fileName))
		return true;
	return false;
}