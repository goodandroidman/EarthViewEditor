#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QGridLayout>
#include <QtGui/QAction>

#include "../Dependence/Common.h"
#include "../Ogre/OgreApp.h"
#include "ui_mymapeditor.h"
#include "OgreWidget.h"
#include "LogDockWidget.h"
#include "PropertyDockWidget.h"
#include "MeshDockWidget.h"
#include "ListDockWidget.h"
#include "ScaleWindow.h"
#include "RotateWindow.h"
#include "NewMapWindow.h"

class MapEditor : public QMainWindow
{	
	Q_OBJECT

public:
	MapEditor(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MapEditor();

private:
	void setupWindows(QMainWindow *parent);
	void connect();

	public slots:
		void saveMap();
		void openFile(const QString &path = QString());
		void setMoveState();
		void setSelectState();

private:
	Ui::MyMapEditorClass ui;
	LogDockWidget            *mLogDockWidget;
	PropertyDockWidget   *mPropertyWidget;
	MeshDockWidget         *mMeshWidget;
	ListDockWidget             *mListWidget;
	ScaleWindow                  *mScaleWindow;
	RotateWindow              *mRotateWindow;
	NewMapWindow           *mNewMapWindow;
	OgreWidget                   *mOgreWidget;
};

#endif // MAINWINDOW_H
