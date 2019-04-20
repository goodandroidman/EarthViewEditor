#include "OgreLog.h"
#include "../QtGUI/LogDockWidget.h"

OgreLog::OgreLog() : mLogWidget(0)
{
}

//----------------------------------------------------------------------------------------

void OgreLog::addLogWidget(LogDockWidget *log)
{
	mLogWidget = log;
	mLogWidget->messageLog(mBack.str(), LOGTYPE_NORMAL, true);
	mBack.clear();
}

//----------------------------------------------------------------------------------------

void OgreLog::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, 
							bool maskDebug, const Ogre::String &logName)
{
	std::string m = message.c_str();
	if (mLogWidget)
		mLogWidget->messageLog(m, getLogType(m));
	else
		mBack << formatLog(m, getLogType(m)) << "<br>";
}

//----------------------------------------------------------------------------------------

LOGTYPE OgreLog::getLogType(const Ogre::String &msg)
{
	if (msg.find("Added") != std::string::npos)
		return LOGTYPE_ADDING;
	else if (msg.find("Creating") != std::string::npos)
		return LOGTYPE_CREATING;
	else if (msg.find("Parsing") != std::string::npos)
		return LOGTYPE_PARSING;
	else if (msg.find("WARNING") != std::string::npos)
		return LOGTYPE_WARNING;
	else if (msg.find("EXCEPTION") != std::string::npos || msg.find("Create Object") != std::string::npos ||
		msg.find("Delete Object") != std::string::npos)
		return LOGTYPE_EXCEPTION;
	else 
		return LOGTYPE_NORMAL;
}
