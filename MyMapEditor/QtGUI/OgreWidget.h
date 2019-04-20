#ifndef  OGREWIDGET_H
#define OGREWIDGET_H

#include <QtGui>
#include "../Ogre/OgreApp.h"

class MapEditor;
class PropertyDockWidget;
class ListDockWidget;

class OgreWidget : public QWidget
{
	Q_OBJECT

public:
	OgreWidget(MapEditor *mainWindow, QWidget *parent = 0);
	~OgreWidget(void);

	void initOgre();                                                               // 初始化Ogre显示, 在OgreWidget构造之后调用
	void drawGrid();                                                            //绘制网格
	bool loadScene(const QString &filename = QString()); //载入场景
	bool saveScene(const QString &filename = QString()); //保存场景
	void setPropertyWindow(PropertyDockWidget* pd);
	void setListWindow(ListDockWidget* ld);

private:
	void timerEvent (QTimerEvent *event);
	void paintEvent (QPaintEvent *event);
	QPaintEngine *paintEngine() const { return 0;}
	void resizeEvent(QResizeEvent *event);

	// 重写鼠标键盘事件
	void  keyPressEvent(QKeyEvent*  evt);
	void  keyReleaseEvent(QKeyEvent*  evt);
	void  mousePressEvent(QMouseEvent*  evt);
	void  mouseReleaseEvent(QMouseEvent*  evt);
	void  mouseMoveEvent(QMouseEvent*  evt);
	void  wheelEvent(QWheelEvent*  evt);


	void dragEnterEvent(QDragEnterEvent *event);		                 // 拖拽进入场景
	void dragLeaveEvent(QDragLeaveEvent *event);		                // 拖拽离开场景
	void dragMoveEvent(QDragMoveEvent *event);	                    // 拖拽这物体在场景内移动
	void dropEvent(QDropEvent *event);		                                    // 释放物体


private:
	OgreApp        *mOgreApp;
	MapEditor      *mMainWindow;
	NodeObject   *mCurrentObject;
};

#endif //OGREWIDGET_H