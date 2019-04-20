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
	NodeObject* createObject(const std::string &meshName, const std::string &name);   //����һ��ʵ��
	LightObject*  createLight(const std::string &lightName, const std::string &type);                      //����һ���ƹ�

	std::map<std::string, NodeObject*> getNodeMap() const { return mNodeMap; }
	std::map<std::string, LightObject*> getLightMap() const { return mLightMap; }

	bool destroyObject(const std::string &name);                                                                        //����һ��ʵ��
	NodeObject* getObjectByName(const std::string &name);                                                 //�������ƻ�ȡʵ��

	void clear();                                                  //���ȫ��ģ�ͣ����ݣ��½�����ʱ�����
	bool loadMap(const std::string &filename);			// ���ص�ͼ
	bool saveMap(const std::string &filename, MapInfo info);			// ���ص�ͼ
private:
	ObjectFactory();
	ObjectFactory(const ObjectFactory&);
	ObjectFactory& operator=(const ObjectFactory&);

	std::map<std::string, NodeObject*>                                mNodeMap;          //�洢�ڵ���Ϣ
	std::map<std::string, LightObject*>                                 mLightMap;           //�洢�ƹ���Ϣ

	int                                                            mId;

	Ogre::SceneManager							*mSceneManager;
};

#endif //OBJECTFACTORY_H