#ifndef  ROTATEWINDOW_H
#define ROTATEWINDOW_H

#include <QtGui/QPushButton>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

class RotateWindow : public QDialog
{
	Q_OBJECT
public:
	RotateWindow(QWidget*parent);
	void updateParameter(bool enable);

	private slots:
		void rotateX();
		void rotateY();
		void rotateZ();

		void resetRotate();
		void setVisible(bool visible);

private:
	void setupUi(QWidget *rotaWindow);
	void retranslateUi(QWidget *rotaWindow);
	void rotate();

private:
	QGridLayout *gridLayout;
	QGroupBox *groupBox;
	QGridLayout *gridLayout_3;
	QGridLayout *gridLayout_2;
	QLabel *XLabel;
	QDoubleSpinBox *mX;
	QLabel *YLabel;
	QDoubleSpinBox *mY;
	QLabel *ZLabel;
	QDoubleSpinBox *mZ;
	QSpacerItem *horizontalSpacer;
	QPushButton *mResetButton;

	QDoubleSpinBox	*mCurrentValue;
};

#endif // ROTATEWINDOW_H
