#ifndef  UTIL_H
#define UTIL_H

#include <string>

//辅助工具类
class Util
{
public:
	//手动加载资源
	static void loadResouse(const std::string &path,const std::string &type,
		const std::string &groupName, bool load = false, bool recursive = false);
	// 卸载资源
	static void unloadResouse(const std::string &groupName);
};

#endif //UTIL_H