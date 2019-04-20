#ifndef  SAVER_H
#define SAVER_H

#include <QXmlStreamWriter>
#include "ObjectFactory.h"

class Saver
{
	friend class ObjectFactory;
public:
	Saver(const std::string &filename, MapInfo info, ObjectFactory* objectFactory);
	bool writeFile();

	std::string getFileName() const { return mFilename; }
	bool hasError() const { return mHasError; }
	std::string getErrorMessage() const { return mErrorMessage; }

private:
	void writeTerrain();
	void writeLights(std::map<std::string, LightObject*>& objects);
	void writeNodes(std::map<std::string, NodeObject*>& objects); 

private:
	bool						mHasError;
	std::string				mFilename;
	std::string				mErrorMessage;

	QXmlStreamWriter mXmlWriter;
	ObjectFactory*		   mObjectFactory;
	MapInfo                 mMapInfo;
};

#endif //SAVER_H