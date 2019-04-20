#include "ObjectFactory.h"
#include "OgreApp.h"
#include "Loader.h"
#include "Saver.h"
#include <iostream>


ObjectFactory::ObjectFactory() : mId(0)
{
	mSceneManager = OgreApp::getSingletonPtr()->getSceneManager();
}

//----------------------------------------------------------------------------------------

ObjectFactory::~ObjectFactory()
{
	clear();
}

//----------------------------------------------------------------------------------------

void ObjectFactory::clear()
{
	for(std::map<std::string, NodeObject*>::iterator it = mNodeMap.begin();
		it != mNodeMap.end(); it++) {
			mSceneManager->destroyEntity(it->second->mEntity);
			mSceneManager->destroySceneNode(it->second->mNode);
	}
	for(std::map<std::string, LightObject*>::iterator it = mLightMap.begin();
		it != mLightMap.end(); it++)
		mSceneManager->destroyLight(it->second->mName);
	mNodeMap.clear();
	mLightMap.clear();
}

//----------------------------------------------------------------------------------------

NodeObject* ObjectFactory::createObject(const std::string &meshName)
{
	std::string name = meshName + "#" + Ogre::StringConverter::toString(mId++);
	return createObject(meshName, name);
}

//----------------------------------------------------------------------------------------

NodeObject* ObjectFactory::createObject(const std::string &meshName, const std::string &name)
{
	NodeObject* object = new NodeObject;
	try {
		object->mMeshName = meshName;
		object->mName = name;
		object->mEntity = mSceneManager->createEntity(object->mName, meshName + ".mesh");
		object->mEntity->setQueryFlags(QUERY_MASK);
		object->mNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
		object->mNode->attachObject(object->mEntity);
	} catch (...) {
		OgreApp::getSingletonPtr()->log("OGRE EXCEPTION : Create Object '" + object->mName + "' Fail");
		return 0;
	}

	OgreApp::getSingletonPtr()->log("OGRE Create Object ' " + object->mName + "'");
	mNodeMap.insert(std::make_pair(object->mName, object));

	return object;
}

//----------------------------------------------------------------------------------------

LightObject* ObjectFactory::createLight(const std::string &lightName, const std::string &type)
{
	LightObject* object = new LightObject;
	try {
		object->mName = lightName;
		object->type = type;
	} catch (...) {
		OgreApp::getSingletonPtr()->log("OGRE EXCEPTION : Create Lights '" + object->mName + "' Fail");
		return 0;
	}

	OgreApp::getSingletonPtr()->log("OGRE Create Lights ' " + object->mName + "'");
	mLightMap.insert(std::make_pair(object->mName, object));
	return object;
}

//----------------------------------------------------------------------------------------

bool ObjectFactory::destroyObject(const std::string &name)
{
	if(mNodeMap.count(name) > 0) {
		std::map<std::string, NodeObject*>::iterator itr = mNodeMap.find(name);
		NodeObject* object= itr->second;
		OgreApp::getSingletonPtr()->log("OGRE Delete Object ' " + object->mName + "'");
		mSceneManager->destroyEntity(object->mEntity);
		mSceneManager->destroySceneNode(object->mNode);
		object = 0;
		mNodeMap.erase(itr);
		mId--;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------

NodeObject* ObjectFactory::getObjectByName(const std::string &name)
{
	std::map<std::string, NodeObject*>::iterator itr = mNodeMap.find(name);
	if(itr != mNodeMap.end())
		return itr->second;
	return 0;
}

//----------------------------------------------------------------------------------------

bool ObjectFactory::loadMap(const std::string &filename)
{
		Loader loader(filename);
		if (!loader.readFile())
			return false;

		return true;
}

//----------------------------------------------------------------------------------------

bool ObjectFactory::saveMap(const std::string &filename, MapInfo info)
{
		Saver saver(filename, info, this);
		if (!saver.writeFile())
			return false;

		return true;
}