#include "Saver.h"
#include "OgreApp.h"
#include "OgreTerrain.h"

#include <string>
#include <QtCore/QFile>

#define TR(a) QString(QString::fromLocal8Bit(a.c_str()))

Saver::Saver(const std::string &filename, MapInfo info, ObjectFactory *objectFactory) :
mHasError(false), mFilename(filename), mObjectFactory(objectFactory),
mMapInfo(info)
{
	mXmlWriter.setAutoFormatting(true);
}

//----------------------------------------------------------------------------------------

bool Saver::writeFile()
{
	QFile file(TR(mFilename));
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		return false;
	}

	mXmlWriter.setDevice(&file);

	mXmlWriter.writeStartDocument();
	mXmlWriter.writeStartElement("MyMapScene");

	writeTerrain();
	writeLights(mObjectFactory->getLightMap());
	writeNodes(mObjectFactory->getNodeMap());

	mXmlWriter.writeEndDocument();
	return true;
}

//----------------------------------------------------------------------------------------

void Saver::writeTerrain()
{
	mXmlWriter.writeStartElement("Map");

	mXmlWriter.writeStartElement("map");

	mXmlWriter.writeAttribute("heightmap", TR(mMapInfo.HEIGHTMAP));
	mXmlWriter.writeAttribute("VertexNum",  QString::number(mMapInfo.TERRAINSIZE));
	mXmlWriter.writeAttribute("WorldSize",  QString::number(mMapInfo.WORLDSIZE));
	mXmlWriter.writeAttribute("WorldHeight",  QString::number(mMapInfo.WORLDHEIGHT));
	mXmlWriter.writeAttribute("Diffuse", TR(mMapInfo.DIFFUSE));
	mXmlWriter.writeAttribute("Special", TR(mMapInfo.SPECIAL));

	mXmlWriter.writeEndElement();

	mXmlWriter.writeEndElement();
}

//----------------------------------------------------------------------------------------

void Saver::writeLights(std::map<std::string,LightObject*> &objects)
{
	mXmlWriter.writeStartElement("Lights");

	for(std::map<std::string, LightObject*>::iterator it = objects.begin();
		it != objects.end(); it++) {
			LightObject* obj = it->second;
			mXmlWriter.writeStartElement("light");
			mXmlWriter.writeAttribute("name", TR(obj->mName));
			mXmlWriter.writeAttribute("type", TR(obj->type));
			mXmlWriter.writeEndElement();
	}	
	mXmlWriter.writeEndElement();
}

//----------------------------------------------------------------------------------------

void Saver::writeNodes(std::map<std::string,NodeObject*> &objects)
{
	mXmlWriter.writeStartElement("Nodes");
	Ogre::Vector3 value = Ogre::Vector3::ZERO;
	Ogre::Quaternion quat = Ogre::Quaternion::ZERO;

	for(std::map<std::string, NodeObject*>::iterator it = objects.begin();
		it != objects.end(); it++) {
			NodeObject* obj = it->second;
			mXmlWriter.writeStartElement("node");
			mXmlWriter.writeAttribute("name", TR(obj->mName));
			mXmlWriter.writeAttribute("meshFile", TR(obj->mMeshName));

			value = obj->mNode->getPosition();
			mXmlWriter.writeStartElement("position");
			mXmlWriter.writeAttribute("x", QString::number(value.x));
			mXmlWriter.writeAttribute("y", QString::number(value.y));
			mXmlWriter.writeAttribute("z", QString::number(value.z));
			mXmlWriter.writeEndElement();

			quat = obj->mNode->getOrientation();
			mXmlWriter.writeStartElement("rotation");
			mXmlWriter.writeAttribute("qx", QString::number(quat.x));
			mXmlWriter.writeAttribute("qy", QString::number(quat.y));
			mXmlWriter.writeAttribute("qz", QString::number(quat.z));
			mXmlWriter.writeAttribute("qw", QString::number(quat.w));
			mXmlWriter.writeEndElement();

			value = obj->mNode->getScale();
			mXmlWriter.writeStartElement("scale");
			mXmlWriter.writeAttribute("x", QString::number(value.x));
			mXmlWriter.writeAttribute("y", QString::number(value.y));
			mXmlWriter.writeAttribute("z", QString::number(value.z));
			mXmlWriter.writeEndElement();

			mXmlWriter.writeEndElement();

	}	
	mXmlWriter.writeEndElement();
}
