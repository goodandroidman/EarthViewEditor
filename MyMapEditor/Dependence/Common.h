#ifndef  COMMON_H
#define COMMON_H

#include <string>
#include <Ogre.h>

// ���״̬
enum MOUSE_STATE {
	LEFT_BUTTON_DOWN,			// ����������
	MIDDLE_BUTTON_DOWN,   	// ����н�����
	RIGHT_BUTTON_DOWN,		// ����Ҽ�����
	BUTTON_UP,						    	// �����ͷ�
};

// ��ǰ����������
enum OPERATOR_STATE
{
	OPE_SELECT,			// ѡ��״̬
	OPE_MOVE,			// �ƶ�״̬
	OPE_SCALE,			// ����״̬
	OPE_ROTATE,		// ��ת״̬
};

//��־����
enum LOGTYPE {
	LOGTYPE_NORMAL = 0,
	LOGTYPE_CREATING,
	LOGTYPE_ADDING,
	LOGTYPE_PARSING,
	LOGTYPE_WARNING,
	LOGTYPE_EXCEPTION,
};

/**
* ģ�͵Ĳ�ѯ����, ���ڳ����ڵ�ȫ��ģ����˵,��Ϊ����, һ���ǿ�ʰȡ��, ����ľ, ǽ��ʵ��,
* ��һ���ǲ���ʰȡ�ľ�̬ģ��, �����ģ��.
**/
enum OBJECT_MASK
{
	QUERY_MASK = 1<<0,		// ��ʰȡ��ѯ����
	HIDE_MASK = 1<<1			// ����ʰȡ��ѯ����
};

/**
* ��ʵ����з�װ
**/
struct NodeObject
{
	Ogre::Entity             *mEntity;
	Ogre::SceneNode  *mNode;
	std::string                  mName;
	std::string                  mMeshName;
};

/**
* �Եƹ���з�װ
**/
struct LightObject
{
	std::string                  mName;
	std::string                  type;                         //�ƹ������
};

enum OBJECT {
	NODE_OBJECT = 0,
	LIGHT_OBJECT,
};

struct MapInfo {
	unsigned int TERRAINSIZE;              //���ζ�����Ŀ��2^n + 1)
	unsigned int WORLDSIZE;                //���δ�С
	unsigned int WORLDHEIGHT;         //���θ߶�
	std::string HEIGHTMAP;                   //�߶�ͼ
	std::string DIFFUSE;                           //������
	std::string SPECIAL;                           //ALPHAͨ��
};


//ģ����Դ�ļ���Դ��
static std::string 	RESOURCE_GROUP_MESH = "GROUP_MESH";


// ����־���ַ������и�ʽ��
std::string formatLog(const std::string &msg, LOGTYPE type);

#endif //COMMON_H