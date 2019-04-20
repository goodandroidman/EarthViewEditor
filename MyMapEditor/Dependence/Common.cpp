#include "Common.h"

std::string formatLog(const std::string &msg, LOGTYPE type)
{
	std::string formattedMsg = msg;
	switch(type) {
	case LOGTYPE_NORMAL:
		formattedMsg = std::string("<font color='black'>") + formattedMsg + "</font>";
		break;
	case LOGTYPE_CREATING:
		formattedMsg = std::string("<font color='darkBlue'>") + formattedMsg + "</font>";
		break;
	case LOGTYPE_ADDING:
		formattedMsg = std::string("<font color='blue'>") + formattedMsg + "</font>";
		break;
	case LOGTYPE_PARSING:
		formattedMsg = std::string("<font color='darkGreen'>") + formattedMsg + "</font>";
		break;
	case LOGTYPE_WARNING:
		formattedMsg = std::string("<b><font color=#ded30c>") + formattedMsg + "</font></b>";
		break;
	case LOGTYPE_EXCEPTION:
		formattedMsg = std::string("<b><font color='red'>") + formattedMsg + "</font></b>";
		break;
	}
	return formattedMsg;
}