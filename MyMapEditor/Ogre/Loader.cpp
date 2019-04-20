#include "Loader.h"
#include "OgreApp.h"

#include <QtXml>

#define toStd(a) std::string((const char *)a.toLocal8Bit());

Loader::Loader(const std::string &filename) :mHasError(false), mFilename(filename)
{
}

//-------------------------------------------------------------------------------

bool Loader::readFile()
{
	QFile file(QString::fromLocal8Bit(mFilename.c_str()));
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		mHasError = true;
		mErrorMessage = "Error: Cannot read file \n";
		return false;
	}

	QString errorStr;
	int errorLine;
	int errorColumn;

	QDomDocument doc;
	if (!doc.setContent(&file, false, &errorStr, &errorLine,
		&errorColumn)) {
			mErrorMessage = "Error: Parse line error \n";
			return false;
	}

	QDomElement root = doc.documentElement();
	if (root.tagName() != "MyMapScene") {
		mErrorMessage = "Error: Not a Scene file \n";
		return false;
	}

	parseScene(root);

	return true;
}

//-------------------------------------------------------------------------------

void Loader::parseScene(const QDomElement &element)
{
	QDomNode child = element.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "Map")
			parseMap(child.toElement());
		else if (child.toElement().tagName() == "Lights")
			parseLights(child.toElement());
		else if (child.toElement().tagName() == "Nodes")
			parseNodes(child.toElement());
		child = child.nextSibling();
	}
}

//-------------------------------------------------------------------------------

void Loader::parseMap(const QDomElement &element)
{
	QDomNode child = element.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "map") {
			MapInfo info;
			info.HEIGHTMAP = toStd(child.toElement().attribute("heightmap"));
			info.DIFFUSE = toStd(child.toElement().attribute("Diffuse"));
			info.SPECIAL = toStd(child.toElement().attribute("Special"));
			info.WORLDHEIGHT = child.toElement().attribute("WorldHeight").toDouble();
			info.TERRAINSIZE = child.toElement().attribute("VertexNum").toDouble();
			info.WORLDSIZE = child.toElement().attribute("WorldSize").toDouble();
			
			OgreApp::getSingletonPtr()->createNewScene(info);
		}
		child = child.nextSibling();
	}
}

//-------------------------------------------------------------------------------

void Loader::parseLights(const QDomElement &element)
{
	QDomNode child = element.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "light") {
		}
		child = child.nextSibling();
	}
}

//-------------------------------------------------------------------------------

void Loader::parseNodes(const QDomElement &element)
{
	QDomNode child = element.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "node") {
			std::string name = toStd(child.toElement().attribute("name"));
			std::string meshName = toStd(child.toElement().attribute("meshFile"));

			//各种参数
			Ogre::Vector3 pos;
			Ogre::Vector3 scale;
			Ogre::Quaternion quat;

			QDomNode param = child.firstChild();
			if(param.toElement().tagName() == "position") {
				pos.x = param.toElement().attribute("x").toDouble();
				pos.y = param.toElement().attribute("y").toDouble();
				pos.z = param.toElement().attribute("z").toDouble();
			}

			param = param.nextSibling();
			if(param.toElement().tagName() == "rotation") {
				quat.x = param.toElement().attribute("qx").toDouble();
				quat.y = param.toElement().attribute("qy").toDouble();
				quat.z = param.toElement().attribute("qy").toDouble();
				quat.w = param.toElement().attribute("qw").toDouble();
			}

			param = param.nextSibling();
			if(param.toElement().tagName() == "scale") {
				scale.x = param.toElement().attribute("x").toDouble();
				scale.y = param.toElement().attribute("y").toDouble();
				scale.z = param.toElement().attribute("z").toDouble();
			}

			NodeObject* obj = OgreApp::getSingletonPtr()->createObject(meshName, name);
			{
				obj->mNode->setPosition(pos);
				obj->mNode->setScale(scale);
				obj->mNode->setOrientation(quat);
			}
		}
		child = child.nextSibling();
	}
}