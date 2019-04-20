#ifndef  LISTDOCKWIDGET_H
#define LISTDOCKWIDGET_H

#include "../Dependence/Common.h"
#include "PropertyDockWidget.h"

#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>
#include <QtGui/QMenu>
#include <QtGui/QCheckBox>

/**
* 用于显示地图已有的建筑的列表
**/
class MapEditor;

class ListDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	ListDockWidget(PropertyDockWidget *propertyDock, QWidget *parent);
	void addNode(const std::string &name, OBJECT type);  //type表示结点的类型，0表示光源，1表示模型
	void selectItem(const std::string &name);                //在列表中选定某项

	void clear();                                                                       //清除

	private slots:
		void showPopMenu(QTreeWidgetItem *, int);			// 显示弹出菜单
		void deleteItem();	      // 删除某项

private:
	void setupUI(QDockWidget *listDockWidget);
	void addNodes();
	void addPopMenu();

private:
	QWidget        *mListWidget;
	QGridLayout  *mGridLayout;
	QTreeWidget		*mTreeWidget;
	QTreeWidgetItem	*mModelItem;
	QTreeWidgetItem	*mLightItem;

	QMenu				*mPopMenu;
	QAction				*deleteAction;

	PropertyDockWidget		*mPropertyDock;


};

#endif //LISTDOCKWIDGET_H