#include "PropertyDockWidget.h"
#include "../Ogre/OgreApp.h"

#include <map>
#include <sstream>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QCommandLinkButton>

PropertyDockWidget::PropertyDockWidget(QWidget *parent) : QDockWidget(parent),
mObject(0)
{
	setupUI(this);
}

//----------------------------------------------------------------------------------------

void PropertyDockWidget::setupUI(QDockWidget *propertyDockWidget)
{
	propertyDockWidget->resize(225, 500);
	propertyDockWidget->setMinimumSize(QSize(200, 100));

	mPropertyWidget = new QWidget();
	QFont font;
	font.setFamily(tr("幼圆"));
	mPropertyWidget->setFont(font);
	mGridLayout = new QGridLayout(mPropertyWidget);
	mGridLayout->setMargin(2);

	mPropertyTable = new QTableWidget(mPropertyWidget);
	//设置列信息
	mPropertyTable->setColumnCount(2);
	mPropertyTable->setHorizontalHeaderItem(0, new QTableWidgetItem());
	mPropertyTable->setHorizontalHeaderItem(1, new QTableWidgetItem());
	//设置行信息
	mPropertyTable->setRowCount(14);
	for(int i = 0; i < mPropertyTable->rowCount(); i++) {
		QTableWidgetItem *widgetitem = new QTableWidgetItem();
		widgetitem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		widgetitem->setTextAlignment(Qt::AlignCenter);
		mPropertyTable->setItem(i, 0, widgetitem);
	}

	//设置提示粗体字和背景
	QFont myFont;
	myFont.setBold(true);
	myFont.setWeight(75);
	QBrush myBrush(QColor(236, 245, 189, 255));
	myBrush.setStyle(Qt::SolidPattern);

	addTipRow(1, myFont, myBrush);
	addTipRow(5, myFont, myBrush);
	addTipRow(10, myFont, myBrush);

	addSpinBox(2, SIGNAL(valueChanged(double)), SLOT(positionChanged()), 0.2f); 
	addSpinBox(3, SIGNAL(valueChanged(double)), SLOT(positionChanged()), 0.2f);
	addSpinBox(4, SIGNAL(valueChanged(double)), SLOT(positionChanged()), 0.2f); 
	addSpinBox(6, SIGNAL(valueChanged(double)), SLOT(oritentationChanged()), 1.0f, -1000.0f, 1000.0f, 6, false); 
	addSpinBox(7, SIGNAL(valueChanged(double)), SLOT(oritentationChanged()), 1.0f, -1000.0f, 1000.0f, 6, false); 
	addSpinBox(8, SIGNAL(valueChanged(double)), SLOT(oritentationChanged()), 1.0f, -1000.0f, 1000.0f, 6, false); 
	addSpinBox(9, SIGNAL(valueChanged(double)), SLOT(oritentationChanged()), 1.0f, -1000.0f, 1000.0f, 6, false); 
	addSpinBox(11, SIGNAL(valueChanged(double)), SLOT(scaleChanged()), 0.5f, -50, 50); 
	addSpinBox(12,SIGNAL(valueChanged(double)), SLOT(scaleChanged()), 0.5f, -50, 50); 
	addSpinBox(13, SIGNAL(valueChanged(double)), SLOT(scaleChanged()), 0.5f, -50, 50);


	// 添加entity名称的文本编辑
	QLineEdit *entityName = new QLineEdit(mPropertyTable);
	entityName->setAlignment(Qt::AlignCenter);
	entityName->setReadOnly(true);
	entityName->setFrame(false);
	mPropertyTable->setCellWidget(0, 1, entityName);

	// --------------------------------------------------------------------------------------------------

	mPropertyTable->setSelectionMode(QAbstractItemView::SingleSelection);
	mPropertyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	mPropertyTable->horizontalHeader()->setDefaultSectionSize(95);
	mPropertyTable->horizontalHeader()->setMinimumSectionSize(20);
	mPropertyTable->verticalHeader()->setVisible(false);
	mPropertyTable->verticalHeader()->setDefaultSectionSize(20);
	mPropertyTable->verticalHeader()->setMinimumSectionSize(20);

	mGridLayout->addWidget(mPropertyTable, 0, 0, 1, 1);

	mGridLayout->setRowStretch(0, 6);
	mGridLayout->setRowStretch(1, 1);

	propertyDockWidget->setWidget(mPropertyWidget);	
	propertyDockWidget->setWindowTitle(tr("属性"));

	retranslateUi(propertyDockWidget);

	updateParameter(false);
}

//----------------------------------------------------------------------------------------

void PropertyDockWidget::addTipRow(int row, const QFont &font, const QBrush &brush)
{
	mPropertyTable->item(row, 0)->setFont(font);
	mPropertyTable->item(row, 0)->setBackground(brush);
	mPropertyTable->item(row, 0)->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);

	QTableWidgetItem *widgetitem = new QTableWidgetItem();
	widgetitem->setBackground(brush);
	widgetitem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
	mPropertyTable->setItem(row, 1, widgetitem);
}

//----------------------------------------------------------------------------------------

void PropertyDockWidget::addSpinBox(int row, const char* signal, const char* slot, float step, 
									float rangeBegin, float rangeEnd, int ecimal, bool enable)
{
	QDoubleSpinBox *sb = new QDoubleSpinBox(mPropertyTable);
	sb->setRange(rangeBegin, rangeEnd);
	sb->setSingleStep(step);
	sb->setEnabled(enable);
	sb->setFrame(false);
	sb->setDecimals(ecimal);
	sb->setAlignment(Qt::AlignCenter);

	mPropertyTable->setCellWidget(row, 1, sb);
	QObject::connect(sb, signal, this, slot);
}

//----------------------------------------------------------------------------------------

void PropertyDockWidget::retranslateUi(QDockWidget *PropertyDockWidget)
{
	mPropertyTable->horizontalHeaderItem(0)->setText(tr("属性"));
	mPropertyTable->horizontalHeaderItem(1)->setText(tr("值"));

	mPropertyTable->setSortingEnabled(false);

	mPropertyTable->item(0, 0)->setText(tr("名称"));
	mPropertyTable->item(1, 0)->setText(tr("位置"));
	mPropertyTable->item(2, 0)->setText("X");
	mPropertyTable->item(3, 0)->setText("Y");
	mPropertyTable->item(4, 0)->setText("Z");
	mPropertyTable->item(5, 0)->setText(tr("旋转"));
	mPropertyTable->item(6, 0)->setText("X");
	mPropertyTable->item(7, 0)->setText("Y");
	mPropertyTable->item(8, 0)->setText("Z");
	mPropertyTable->item(9, 0)->setText("W");
	mPropertyTable->item(10, 0)->setText(tr("缩放"));
	mPropertyTable->item(11, 0)->setText("X");
	mPropertyTable->item(12, 0)->setText("Y");
	mPropertyTable->item(13, 0)->setText("Z");
}

//----------------------------------------------------------------------------------------

void PropertyDockWidget::positionChanged()
{
	QDoubleSpinBox *sb = static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(2, 1));
	float x =  sb->value();
	sb = static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(3, 1));
	float y = sb->value();
	sb = static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(4, 1));
	float z = sb->value();

	mObject->mNode->setPosition(x, y, z);
}

//----------------------------------------------------------------------------------------

void PropertyDockWidget::oritentationChanged()
{

}

//----------------------------------------------------------------------------------------

void PropertyDockWidget::scaleChanged()
{
	QDoubleSpinBox *sb = static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(11, 1));
	float x =  sb->value();
	sb = static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(12, 1));
	float y = sb->value();
	sb = static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(13, 1));
	float z = sb->value();

	mObject->mNode->setScale(x, y, z);
}

//----------------------------------------------------------------------------------------

void PropertyDockWidget::updateParameter(bool enable)
{
	mPropertyTable->cellWidget(0, 1)->setEnabled(enable);
	mPropertyTable->cellWidget(2, 1)->setEnabled(enable);
	mPropertyTable->cellWidget(3, 1)->setEnabled(enable);
	mPropertyTable->cellWidget(4, 1)->setEnabled(enable);

	mPropertyTable->cellWidget(11, 1)->setEnabled(enable);
	mPropertyTable->cellWidget(12, 1)->setEnabled(enable);
	mPropertyTable->cellWidget(13, 1)->setEnabled(enable);
}



//----------------------------------------------------------------------------------------

void PropertyDockWidget::parameterUpdate(NodeObject *object)
{
	mObject = object;
	if (!mObject)
	{
		updateParameter(false);
		return;
	}

	updateParameter(true);
	static_cast<QLineEdit *>(mPropertyTable->cellWidget(0, 1))->setText(mObject->mName.c_str());

	Ogre::Vector3 pos = mObject->mNode->getPosition();
	static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(2, 1))->setValue(pos.x);
	static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(3, 1))->setValue(pos.y);
	static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(4, 1))->setValue(pos.z);

	Ogre::Quaternion quat = mObject->mNode->getOrientation();
	static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(6, 1))->setValue(quat.x);
	static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(7, 1))->setValue(quat.y);
	static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(8, 1))->setValue(quat.z);
	static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(9, 1))->setValue(quat.w);

	Ogre::Vector3 scale = mObject->mNode->getScale();
	static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(11, 1))->setValue(scale.x);
	static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(12, 1))->setValue(scale.y);
	static_cast<QDoubleSpinBox *>(mPropertyTable->cellWidget(13, 1))->setValue(scale.z);
}

//----------------------------------------------------------------------------------------
