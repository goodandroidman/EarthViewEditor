#ifndef  PROPERTYDOCKWIDGET_H
#define PROPERTYDOCKWIDGET_H

#include "../Dependence/Common.h"

#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include <QtGui/QSpinBox>
#include <QtGui/QTreeWidgetItem>
#include <string>

class PropertyDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	PropertyDockWidget(QWidget *parent = 0);
	void parameterUpdate(NodeObject* object);

	protected slots:
		void positionChanged();			// 坐标发生改变
		void oritentationChanged();	// 方位发生改变
		void scaleChanged();				// 缩放发生改变

private:
	void setupUI(QDockWidget *propertyDockWidget);
	void addTipRow(int row, const QFont &font, const QBrush &brush);
	void addSpinBox(int row, const char* signal, const char *slot, float step = 1.0f, 
		float rangeBegin = -1000, float rangeEnd = 1000, int ecimal = 6, bool enable = true);
	void retranslateUi(QDockWidget *PropertyDockWidget);
	void updateParameter(bool enable);

private:
	QWidget           *mPropertyWidget;
	QGridLayout     *mGridLayout;
	QTableWidget *mPropertyTable;
	QTextEdit          *mPropertyTip;

	std::map<int, std::string> mTips;
	std::string		mCurrentEntityName;

	NodeObject     *mObject;
};

#endif //PROPERTYDOCKWIDGET_H