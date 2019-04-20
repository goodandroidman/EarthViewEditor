#include "ListDockWidget.h"
#include "../Ogre/OgreApp.h"

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

ListDockWidget::ListDockWidget(PropertyDockWidget *propertyDock, QWidget *parent)
: QDockWidget(parent), mPropertyDock(propertyDock)
{
	setupUI(this);
}

void ListDockWidget::setupUI(QDockWidget *listDockWidget)
{
	listDockWidget->resize(250, 400);
	listDockWidget->setMinimumSize(QSize(200, 200));
	mListWidget = new QWidget();
	QFont font;
	font.setFamily(tr("幼圆"));
	mListWidget->setFont(font);

	mGridLayout = new QGridLayout(mListWidget);
	mGridLayout->setMargin(2);
	mGridLayout->setHorizontalSpacing(2);
	mGridLayout->setVerticalSpacing(0);

	mTreeWidget = new QTreeWidget(listDockWidget);
	mTreeWidget->setSelectionBehavior(QAbstractItemView::SelectItems);

	mTreeWidget->setAnimated(true);
	mTreeWidget->sortItems(0, Qt::AscendingOrder);
	mTreeWidget->setSortingEnabled(true);
	mTreeWidget->header()->setVisible(false);

	addNodes();
	addPopMenu();	

	mGridLayout->addWidget(mTreeWidget, 0, 0, 1, 1);

	listDockWidget->setWidget(mListWidget);
	listDockWidget->setWindowTitle(tr("列表"));
}

//----------------------------------------------------------------------------------------

void ListDockWidget::addNodes()
{
	QFont boldFont;
	boldFont.setBold(true);
	boldFont.setWeight(75);

	// root 项
	QTreeWidgetItem *rootItem = new QTreeWidgetItem(mTreeWidget);
	rootItem->setText(0, tr("Scene"));
	rootItem->setFont(0, boldFont);
	rootItem->setIcon(0, QIcon(":/List/scene.svg"));

	// 光源项
	mLightItem = new QTreeWidgetItem(rootItem);
	mLightItem->setText(0, tr("Lights"));
	mLightItem->setFont(0, boldFont);
	mLightItem->setIcon(0, QIcon(":/List/splatgrass.svg"));

	// 模型项
	mModelItem = new QTreeWidgetItem(rootItem);
	mModelItem->setText(0, tr("Model"));
	mModelItem->setFont(0, boldFont);
	mModelItem->setIcon(0, QIcon(":/List/relight.svg"));
}

//----------------------------------------------------------------------------------------
//type表示结点的类型，0表示光源，1表示模型
void ListDockWidget::addNode(const std::string &name, OBJECT type)
{
	QTreeWidgetItem *item = 0;
	if(type == LIGHT_OBJECT) {
		item = new QTreeWidgetItem(mLightItem);

	} else {
		item = new QTreeWidgetItem(mModelItem);
	}

	item->setIcon(0, QIcon(":/View/entity.svg"));
	item->setText(0, name.c_str());
	mTreeWidget->setCurrentItem(item);
}

//----------------------------------------------------------------------------------------

void ListDockWidget::selectItem(const std::string &name)
{
	for (int i = 0; i < mModelItem->childCount(); i++)
		if (mModelItem->child(i)->text(0) == QString(name.c_str()))
		{
			mTreeWidget->setCurrentItem(mModelItem->child(i));
			return;
		}

		for (int i = 0; i < mLightItem->childCount(); i++)
			if (mLightItem->child(i)->text(0) == QString(name.c_str()))
			{
				mTreeWidget->setCurrentItem(mLightItem->child(i));
				return;
			}
}

//----------------------------------------------------------------------------------------

void ListDockWidget::addPopMenu()
{
	mPopMenu = new QMenu(mTreeWidget);
	deleteAction = mPopMenu->addAction(QIcon(":/File/delete.svg"), tr("删除"));

	QObject::connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteItem()));
	QObject::connect(mTreeWidget, SIGNAL(itemPressed(QTreeWidgetItem *, int)), 
		this, SLOT(showPopMenu(QTreeWidgetItem *, int)));
}

//----------------------------------------------------------------------------------------

void ListDockWidget::deleteItem()
{
	QMessageBox::StandardButton bt = QMessageBox::warning(this, tr("警告"), tr("是否确定删除此模型?"), 
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	if (bt == QMessageBox::No)
		return;

	QTreeWidgetItem* cur = mTreeWidget->currentItem();
	// 删除模型
	OgreApp::getSingletonPtr()->destroyObject(cur->text(0).toStdString());
	// 删除当前项
	cur->parent()->removeChild(cur);
}

//----------------------------------------------------------------------------------------

void ListDockWidget::showPopMenu(QTreeWidgetItem *item, int col)
{
	if (item->parent() && item != mModelItem && item != mLightItem)
	{
		if (qApp->mouseButtons() == Qt::RightButton)
		{
			mPopMenu->exec(QCursor::pos());
		}
		else
			NodeObject* object = OgreApp::getSingletonPtr()->selectObject(item->text(0).toStdString());
	}
}
