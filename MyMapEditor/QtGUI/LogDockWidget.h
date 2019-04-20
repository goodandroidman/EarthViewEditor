#ifndef  LOGDOCKWIDGET_H
#define LOGDOCKWIDGET_H

#include "../Dependence/Common.h"
#include "../Ogre/OgreLog.h"
#include "../Ogre/OgreApp.h"

#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class LogDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	LogDockWidget(QWidget *parent = 0);
	void messageLog(const std::string &msg, LOGTYPE type, bool first = false);

private:
	void setupUI(QDockWidget *logDockWidget);

private:
	QWidget        *mLogWidget;
	QGridLayout  *mGridLayout;
	QTextEdit       *mLogText;
};

#endif //LOGDOCKWIDGET_H