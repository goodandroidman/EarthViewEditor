#include "LogDockWidget.h"
#include <QtCore/QDateTime>

LogDockWidget::LogDockWidget(QWidget *parent) : QDockWidget(parent)
{
	setupUI(this);
}

//----------------------------------------------------------------------------------------

void LogDockWidget::setupUI(QDockWidget *logDockWidget)
{
	logDockWidget->resize(400, 100);
	logDockWidget->setMinimumSize(QSize(100, 90));
	mLogWidget = new QWidget();
	QFont font;
	font.setFamily(tr("��Բ"));
	logDockWidget->setFont(font);

	mGridLayout = new QGridLayout(mLogWidget);
	mGridLayout->setMargin(2);

	QFont sizeFont;
	sizeFont.setPointSize(10);
	mLogText = new QTextEdit(mLogWidget);
	mLogText->setMinimumSize(QSize(100, 85));
	mLogText->setReadOnly(true);
	mLogText->setFont(sizeFont);

	mGridLayout->addWidget(mLogText, 0, 0, 1, 1);

	logDockWidget->setWidget(mLogWidget);
	logDockWidget->setWindowTitle(tr("��־��Ϣ"));

	//OgreApp::getSingletonPtr()->getLog()->addLogWidget(this);

}

//----------------------------------------------------------------------------------------

void LogDockWidget::messageLog(const std::string &msg, LOGTYPE type, bool first)
{
	 QDateTime time;
	 QString str = time.currentDateTime().toString(" yyyy.MM.dd.    HH:mm:ss        ");
	 //ȡ��ϵͳ��ǰʱ�䣬����ָ����ʽ����
	 str.append(QString(QString::fromLocal8Bit(msg.c_str())));
	 std::string MSG = std::string((const char *)str.toLocal8Bit());

	if (!first)
		mLogText->append(std::string(formatLog(MSG, type) + "\n").c_str());
	else
		mLogText->append(msg.c_str());
}