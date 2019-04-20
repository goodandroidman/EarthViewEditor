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
* ������ʾ��ͼ���еĽ������б�
**/
class MapEditor;

class ListDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	ListDockWidget(PropertyDockWidget *propertyDock, QWidget *parent);
	void addNode(const std::string &name, OBJECT type);  //type��ʾ�������ͣ�0��ʾ��Դ��1��ʾģ��
	void selectItem(const std::string &name);                //���б���ѡ��ĳ��

	void clear();                                                                       //���

	private slots:
		void showPopMenu(QTreeWidgetItem *, int);			// ��ʾ�����˵�
		void deleteItem();	      // ɾ��ĳ��

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