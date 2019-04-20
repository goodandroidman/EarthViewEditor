#ifndef  OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "../Dependence/Common.h"
#include "../Dependence/SingletonT.h"


#include <map>

class ObjectFactory : public SingletonT<ObjectFactory>
{
	SINGLETON_DEFINE(ObjectFactory)
public:
	~ObjectFactory();

	NodeObject* createObject(const std::string &meshName);
	NodeObject* createObject(const std::string &meshName, const std::string &name);   //创建一个实体
	LightObject*  createLight(const std::string &lightName, const std::string &type);                      //创建一个灯光

	std::map<std::string, NodeObject*> getNodeMap() const { return mNodeMap; }
	std::map<std::string, LightObject*> getLightMap() const { return mLightMap; }

	bool destroyObject(const std::string &name);                                                                        //销毁一个实体
	NodeObject* getObjectByName(const std::string &name);                                                 //根据名称获取实体

	void clear();                                                  //清楚全部模型，数据，新建场景时候调用
	bool loadMap(const std::string &filename);			// 加载地图
	bool saveMap(const std::string &filename, MapInfo info);			// 加载地图
private:
	ObjectFactory();
	ObjectFactory(const ObjectFactory&);
	ObjectFactory& operator=(const ObjectFactory&);

	std::map<std::string, NodeObject*>                                mNodeMap;          //存储节点信息
	std::map<std::string, LightObject*>                                 mLightMap;           //存储灯光信息

	int                                                            mId;

	Ogre::SceneManager							*mSceneManager;
};

#endif //OBJECTFACTORY_H