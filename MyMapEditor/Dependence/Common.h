#ifndef  COMMON_H
#define COMMON_H

#include <string>
#include <Ogre.h>

// 鼠标状态
enum MOUSE_STATE {
	LEFT_BUTTON_DOWN,			// 鼠标左键按下
	MIDDLE_BUTTON_DOWN,   	// 鼠标中建按下
	RIGHT_BUTTON_DOWN,		// 鼠标右键按下
	BUTTON_UP,						    	// 鼠标键释放
};

// 当前操作的类型
enum OPERATOR_STATE
{
	OPE_SELECT,			// 选择状态
	OPE_MOVE,			// 移动状态
	OPE_SCALE,			// 缩放状态
	OPE_ROTATE,		// 旋转状态
};

//日志类型
enum LOGTYPE {
	LOGTYPE_NORMAL = 0,
	LOGTYPE_CREATING,
	LOGTYPE_ADDING,
	LOGTYPE_PARSING,
	LOGTYPE_WARNING,
	LOGTYPE_EXCEPTION,
};

/**
* 模型的查询遮罩, 对于场景内的全部模型来说,分为两种, 一种是可拾取的, 如树木, 墙等实体,
* 另一种是不可拾取的静态模型, 如地形模型.
**/
enum OBJECT_MASK
{
	QUERY_MASK = 1<<0,		// 可拾取查询遮罩
	HIDE_MASK = 1<<1			// 不可拾取查询遮罩
};

/**
* 对实体进行封装
**/
struct NodeObject
{
	Ogre::Entity             *mEntity;
	Ogre::SceneNode  *mNode;
	std::string                  mName;
	std::string                  mMeshName;
};

/**
* 对灯光进行封装
**/
struct LightObject
{
	std::string                  mName;
	std::string                  type;                         //灯光的类型
};

enum OBJECT {
	NODE_OBJECT = 0,
	LIGHT_OBJECT,
};

struct MapInfo {
	unsigned int TERRAINSIZE;              //地形顶点数目（2^n + 1)
	unsigned int WORLDSIZE;                //地形大小
	unsigned int WORLDHEIGHT;         //地形高度
	std::string HEIGHTMAP;                   //高度图
	std::string DIFFUSE;                           //漫反射
	std::string SPECIAL;                           //ALPHA通道
};


//模型资源文件资源组
static std::string 	RESOURCE_GROUP_MESH = "GROUP_MESH";


// 对日志的字符串进行格式化
std::string formatLog(const std::string &msg, LOGTYPE type);

#endif //COMMON_H