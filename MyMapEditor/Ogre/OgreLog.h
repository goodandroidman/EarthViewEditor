#ifndef  OGRELOG_H
#define OGRELOG_H

#include "../Dependence/Common.h"
#include <sstream>

class LogDockWidget;

/**
* 自定义日志类监听器, 用于重新将日志定位到指定控件显示.
**/

class OgreLog : public Ogre::LogListener
{
public:
	OgreLog();

	void addLogWidget(LogDockWidget* log);                  //将日志文件绑定到Ogre控件
	void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, 
		bool maskDebug, const Ogre::String &logName); //记录日志过滤
	LOGTYPE getLogType(const Ogre::String &msg);        //获取日志类型

private:
	LogDockWidget *mLogWidget;
	std::stringstream  mBack;
};


#endif //OGRELOG_H