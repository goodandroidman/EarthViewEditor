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

	void initOgre();                                                               // ��ʼ��Ogre��ʾ, ��OgreWidget����֮�����
	void drawGrid();                                                            //��������
	bool loadScene(const QString &filename = QString()); //���볡��
	bool saveScene(const QString &filename = QString()); //���泡��
	void setPropertyWindow(PropertyDockWidget* pd);
	void setListWindow(ListDockWidget* ld);

private:
	void timerEvent (QTimerEvent *event);
	void paintEvent (QPaintEvent *event);
	QPaintEngine *paintEngine() const { return 0;}
	void resizeEvent(QResizeEvent *event);

	// ��д�������¼�
	void  keyPressEvent(QKeyEvent*  evt);
	void  keyReleaseEvent(QKeyEvent*  evt);
	void  mousePressEvent(QMouseEvent*  evt);
	void  mouseReleaseEvent(QMouseEvent*  evt);
	void  mouseMoveEvent(QMouseEvent*  evt);
	void  wheelEvent(QWheelEvent*  evt);


	void dragEnterEvent(QDragEnterEvent *event);		                 // ��ק���볡��
	void dragLeaveEvent(QDragLeaveEvent *event);		                // ��ק�뿪����
	void dragMoveEvent(QDragMoveEvent *event);	                    // ��ק�������ڳ������ƶ�
	void dropEvent(QDropEvent *event);		                                    // �ͷ�����


private:
	OgreApp        *mOgreApp;
	MapEditor      *mMainWindow;
	NodeObject   *mCurrentObject;
};

#endif //OGREWIDGET_H