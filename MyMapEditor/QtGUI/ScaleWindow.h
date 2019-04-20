#ifndef SCALEWINDOW_H
#define SCALEWINDOW_H

#include <QtGui/QDialog>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

class ScaleWindow : public QDialog
{
	Q_OBJECT
public:
	ScaleWindow(QWidget*parent);

	private slots:
		void scaleX();
		void scaleY();
		void scaleZ();
		void setVisible(bool visible);

private:
	void scale();
	void setupUi(QWidget *scaleWindow);
	void retranslateUi(QWidget *scaleWindow);

private:
	QGridLayout *gridLayout;
	QGroupBox *groupBox;
	QGridLayout *gridLayout_3;
	QGridLayout *gridLayout_2;
	QCheckBox *mRatio;
	QLabel *XLabel;
	QDoubleSpinBox *mX;
	QLabel *YLabel;
	QDoubleSpinBox *mY;
	QLabel *ZLabel;
	QDoubleSpinBox *mZ;

	QDoubleSpinBox *mCurrent;
};

#endif // SCALEWINDOW_H
