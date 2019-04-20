#include "RotateWindow.h"
#include "../Ogre/OgreApp.h"

#define TR(a) QString::fromLocal8Bit(a)

//----------------------------------------------------------------------------------------

RotateWindow::RotateWindow(QWidget*parent) : QDialog(parent), mCurrentValue(0)
{
	setupUi(this);

	QObject::connect(mX, SIGNAL(valueChanged(double)), this, SLOT(rotateX()));
	QObject::connect(mY, SIGNAL(valueChanged(double)), this, SLOT(rotateY()));
	QObject::connect(mZ, SIGNAL(valueChanged(double)), this, SLOT(rotateZ()));
	QObject::connect(mResetButton, SIGNAL(clicked()), this, SLOT(resetRotate()));
}

//----------------------------------------------------------------------------------------

void RotateWindow::setupUi(QWidget *rotateWindow)
{
	rotateWindow->setFixedSize(190, 125);
	rotateWindow->setWindowOpacity(0.9);

	QFont font;
	font.setFamily(TR("Î¢ÈíÑÅºÚ"));
	font.setPointSize(10);
	rotateWindow->setFont(font);
	gridLayout = new QGridLayout(rotateWindow);
	gridLayout->setSpacing(2);
	gridLayout->setMargin(1);
	groupBox = new QGroupBox(rotateWindow);
	gridLayout_3 = new QGridLayout(groupBox);
	gridLayout_3->setSpacing(2);
	gridLayout_3->setMargin(2);
	gridLayout_2 = new QGridLayout();
	gridLayout_2->setSpacing(3);
	XLabel = new QLabel(groupBox);
	XLabel->setAlignment(Qt::AlignCenter);

	gridLayout_2->addWidget(XLabel, 0, 0, 1, 1);

	mX = new QDoubleSpinBox(groupBox);
	mX->setAlignment(Qt::AlignCenter);
	mX->setSingleStep(0.5);
	mX->setDecimals(5);
	mX->setRange(-360, 360);

	gridLayout_2->addWidget(mX, 0, 1, 1, 1);

	YLabel = new QLabel(groupBox);
	YLabel->setAlignment(Qt::AlignCenter);

	gridLayout_2->addWidget(YLabel, 1, 0, 1, 1);

	mY = new QDoubleSpinBox(groupBox);
	mY->setAlignment(Qt::AlignCenter);
	mY->setSingleStep(0.5);
	mY->setDecimals(5);
	mY->setRange(-360, 360);
	gridLayout_2->addWidget(mY, 1, 1, 1, 1);

	ZLabel = new QLabel(groupBox);
	ZLabel->setAlignment(Qt::AlignCenter);
	gridLayout_2->addWidget(ZLabel, 2, 0, 1, 1);

	mZ = new QDoubleSpinBox(groupBox);
	mZ->setObjectName(QString::fromUtf8("mZ"));
	mZ->setAlignment(Qt::AlignCenter);
	mZ->setSingleStep(0.5);
	mZ->setDecimals(5);
	mZ->setRange(-360, 360);

	gridLayout_2->addWidget(mZ, 2, 1, 1, 1);
	gridLayout_2->setColumnStretch(0, 1);
	gridLayout_2->setColumnStretch(1, 4);
	gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 2);

	horizontalSpacer = new QSpacerItem(122, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	gridLayout_3->addItem(horizontalSpacer, 1, 0, 1, 1);

	mResetButton = new QPushButton(groupBox);
	gridLayout_3->addWidget(mResetButton, 1, 1, 1, 1);
	mResetButton->setAutoDefault(false);

	gridLayout_3->setRowStretch(0, 7);
	gridLayout_3->setRowStretch(1, 1);
	gridLayout_3->setColumnStretch(0, 7);
	gridLayout_3->setColumnStretch(1, 1);
	gridLayout->addWidget(groupBox, 1, 0, 1, 1);

	gridLayout->setRowStretch(0, 7);
	gridLayout->setRowStretch(1, 1);

	retranslateUi(rotateWindow);
} // setupUi

//----------------------------------------------------------------------------------------

void RotateWindow::retranslateUi(QWidget *rotateWindow)
{
	rotateWindow->setWindowTitle(TR("Ðý×ª"));
	groupBox->setTitle(QString());
	XLabel->setText("X ");
	YLabel->setText("Y");
	ZLabel->setText("Z");
	mResetButton->setText(TR("ÖØÖÃ"));
} // retranslateUi

//----------------------------------------------------------------------------------------

void RotateWindow::rotate()
{
	static float lastX = 0;
	static float lastY = 0;
	static float lastZ = 0;

	NodeObject* mCurrentObj = OgreApp::getSingletonPtr()->getCurrentSelectObject();
	if(!mCurrentObj) 
		return ;

	if (mCurrentValue == mX)
	{
		mCurrentObj->mNode->pitch(Ogre::Degree(mX->value() - lastX));
		lastX = mX->value();
	}
	else if (mCurrentValue == mY)
	{
		mCurrentObj->mNode->yaw(Ogre::Degree(mY->value() - lastY));
		lastY = mY->value();
	}
	else
	{
		mCurrentObj->mNode->roll(Ogre::Degree(mZ->value() - lastZ));
		lastZ = mZ->value();
	}
}

//----------------------------------------------------------------------------------------

void RotateWindow::rotateX()
{
	mCurrentValue = mX;
	rotate();
}

//----------------------------------------------------------------------------------------

void RotateWindow::rotateY()
{
	mCurrentValue = mY;
	rotate();
}

//----------------------------------------------------------------------------------------

void RotateWindow::rotateZ()
{
	mCurrentValue = mZ;
	rotate();
}

//----------------------------------------------------------------------------------------

void RotateWindow::resetRotate()
{
	NodeObject* mCurrentObj = OgreApp::getSingletonPtr()->getCurrentSelectObject();
	if(!mCurrentObj) 
		return ;
	mCurrentObj->mNode->resetOrientation();

	mX->setValue(0);
	mY->setValue(0);
	mZ->setValue(0);
}

//----------------------------------------------------------------------------------------

void RotateWindow::updateParameter(bool enable)
{
	mX->setEnabled(enable);
	mY->setEnabled(enable);
	mZ->setEnabled(enable);

	mX->setValue(0);
	mY->setValue(0);
	mZ->setValue(0);
}

//----------------------------------------------------------------------------------------

void RotateWindow::setVisible(bool visible)
{
	NodeObject* mCurrentObj = OgreApp::getSingletonPtr()->getCurrentSelectObject();

	mX->setEnabled(mCurrentObj != 0);
	mY->setEnabled(mCurrentObj != 0);
	mZ->setEnabled(mCurrentObj != 0);

	QDialog::setVisible(visible);
}