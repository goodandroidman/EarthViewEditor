#include "ScaleWindow.h"
#include "../Ogre/OgreApp.h"

#define TR(a) QString::fromLocal8Bit(a)

//----------------------------------------------------------------------------------------

ScaleWindow::ScaleWindow(QWidget*parent) : QDialog(parent)
{
	setupUi(this);

	QObject::connect(mX, SIGNAL(valueChanged(double)), this, SLOT(scaleX()));
	QObject::connect(mY, SIGNAL(valueChanged(double)), this, SLOT(scaleY()));
	QObject::connect(mZ, SIGNAL(valueChanged(double)), this, SLOT(scaleZ()));
}

//----------------------------------------------------------------------------------------

void ScaleWindow::setupUi(QWidget *scaleWindow)
{
	scaleWindow->setWindowOpacity(0.9);
	scaleWindow->setFixedSize(190, 120);

	QFont font;
	font.setFamily(TR("微软雅黑"));
	font.setPointSize(10);
	scaleWindow->setFont(font);
	gridLayout = new QGridLayout(scaleWindow);
	gridLayout->setMargin(2);
	groupBox = new QGroupBox(scaleWindow);
	gridLayout_3 = new QGridLayout(groupBox);
	gridLayout_3->setSpacing(3);
	gridLayout_3->setMargin(2);
	gridLayout_2 = new QGridLayout();
	gridLayout_2->setSpacing(3);
	mRatio = new QCheckBox(groupBox);

	QFont font1;
	font1.setPointSize(9);
	mRatio->setFont(font1);
	mRatio->setTristate(false);
	gridLayout_2->addWidget(mRatio, 0, 0, 1, 2);

	XLabel = new QLabel(groupBox);
	XLabel->setAlignment(Qt::AlignCenter);
	gridLayout_2->addWidget(XLabel, 1, 0, 1, 1);

	mX = new QDoubleSpinBox(groupBox);
	mX->setAlignment(Qt::AlignCenter);
	mX->setSingleStep(0.5);
	mX->setValue(1);
	mX->setDecimals(5);
	mX->setRange(-10, 10);
	gridLayout_2->addWidget(mX, 1, 1, 1, 1);

	YLabel = new QLabel(groupBox);
	YLabel->setAlignment(Qt::AlignCenter);
	gridLayout_2->addWidget(YLabel, 2, 0, 1, 1);

	mY = new QDoubleSpinBox(groupBox);
	mY->setAlignment(Qt::AlignCenter);
	mY->setSingleStep(0.5);
	mY->setRange(-10, 10);
	mY->setValue(1);
	mY->setDecimals(5);
	gridLayout_2->addWidget(mY, 2, 1, 1, 1);

	ZLabel = new QLabel(groupBox);
	ZLabel->setAlignment(Qt::AlignCenter);
	gridLayout_2->addWidget(ZLabel, 3, 0, 1, 1);

	mZ = new QDoubleSpinBox(groupBox);
	mZ->setAlignment(Qt::AlignCenter);
	mZ->setValue(1);
	mZ->setSingleStep(0.5);
	mZ->setDecimals(5);
	mZ->setRange(-10, 10);

	gridLayout_2->addWidget(mZ, 3, 1, 1, 1);
	gridLayout_2->setColumnStretch(0, 1);
	gridLayout_2->setColumnStretch(1, 6);

	gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);

	gridLayout->addWidget(groupBox, 0, 0, 1, 1);

	retranslateUi(scaleWindow);

	QMetaObject::connectSlotsByName(scaleWindow);
} // setupUi

//----------------------------------------------------------------------------------------

void ScaleWindow::retranslateUi(QWidget *scaleWindow)
{
	scaleWindow->setWindowTitle(TR("缩放"));
	groupBox->setTitle(QString());
	mRatio->setText(TR("等比缩放"));
	XLabel->setText("X");
	YLabel->setText("Y");
	ZLabel->setText("Z");
} // retranslateUi

//----------------------------------------------------------------------------------------

void ScaleWindow::scale()
{
	NodeObject* mCurrentObj = OgreApp::getSingletonPtr()->getCurrentSelectObject();
	if(!mCurrentObj) 
		return ;

	if (mRatio->isChecked())
	{
		if (mCurrent == mX)
		{
			mY->setValue(mX->value());
			mZ->setValue(mX->value());
		}
		else if (mCurrent == mY)
		{
			mX->setValue(mY->value());
			mZ->setValue(mY->value());
		}
		else
		{
			mX->setValue(mZ->value());
			mY->setValue(mZ->value());
		}
	}

	// 更新entity;
	mCurrentObj->mNode->setScale(mX->value(), mY->value(), mZ->value());
}

//----------------------------------------------------------------------------------------

void ScaleWindow::setVisible(bool visible)
{
	NodeObject* mCurrentObj = OgreApp::getSingletonPtr()->getCurrentSelectObject();
	if (mCurrentObj)
	{
		mX->setEnabled(true);
		mY->setEnabled(true);
		mZ->setEnabled(true);

		Ogre::Vector3 sc = mCurrentObj->mNode->getScale();
		mX->setValue(sc.x);
		mY->setValue(sc.y);
		mZ->setValue(sc.z);
	}
	else
	{
		mX->setEnabled(false);
		mY->setEnabled(false);
		mZ->setEnabled(false);

		mX->setValue(1);
		mY->setValue(1);
		mZ->setValue(1);
	}

	QDialog::setVisible(visible);
}

//----------------------------------------------------------------------------------------

void ScaleWindow::scaleX()
{
	mCurrent = mX;
	scale();
}

//----------------------------------------------------------------------------------------

void ScaleWindow::scaleY()
{
	mCurrent = mY;
	scale();
}

//----------------------------------------------------------------------------------------

void ScaleWindow::scaleZ()
{
	mCurrent = mZ;
	scale();
}

//----------------------------------------------------------------------------------------
