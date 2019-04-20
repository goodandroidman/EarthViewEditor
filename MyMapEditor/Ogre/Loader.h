#ifndef  LOADER_H
#define LOADER_H

#include <iostream>

class QDomElement;
class QString;

class Loader
{
	friend class ObjectFactory;
public:
	Loader(const std::string &filename);
	bool readFile();

private:
	void parseScene(const QDomElement &element);
	void parseMap(const QDomElement &element);
	void parseLights(const QDomElement &element);
	void parseNodes(const QDomElement &element);

private:
	bool						mHasError;
	std::string				mFilename;
	std::string				mErrorMessage;
};

#endif
