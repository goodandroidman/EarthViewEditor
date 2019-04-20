#ifndef  OGRELOG_H
#define OGRELOG_H

#include "../Dependence/Common.h"
#include <sstream>

class LogDockWidget;

/**
* �Զ�����־�������, �������½���־��λ��ָ���ؼ���ʾ.
**/

class OgreLog : public Ogre::LogListener
{
public:
	OgreLog();

	void addLogWidget(LogDockWidget* log);                  //����־�ļ��󶨵�Ogre�ؼ�
	void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, 
		bool maskDebug, const Ogre::String &logName); //��¼��־����
	LOGTYPE getLogType(const Ogre::String &msg);        //��ȡ��־����

private:
	LogDockWidget *mLogWidget;
	std::stringstream  mBack;
};


#endif //OGRELOG_H