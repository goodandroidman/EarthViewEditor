#ifndef  UTIL_H
#define UTIL_H

#include <string>

//����������
class Util
{
public:
	//�ֶ�������Դ
	static void loadResouse(const std::string &path,const std::string &type,
		const std::string &groupName, bool load = false, bool recursive = false);
	// ж����Դ
	static void unloadResouse(const std::string &groupName);
};

#endif //UTIL_H