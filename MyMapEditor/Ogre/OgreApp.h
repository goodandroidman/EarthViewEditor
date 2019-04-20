#ifndef  OGREAPP_H
#define OGREAPP_H
#include "../Dependence/SingletonT.h"
#include "../Dependence/Common.h"
#include "../QtGUI/PropertyDockWidget.h"
#include "../QtGUI/ListDockWidget.h"
#include "ObjectFactory.h"
#include "OgreLog.h"
#include "OgreGrid.h"
#include "OgreTerrain.h"
#include "Util.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QApplication>
#include <QtGui/QCursor>

/**
* 用于初始化Ogre以及每帧的更新，输入输出
**/

class OgreApp : public SingletonT<OgreApp>
{
	SINGLETON_DEFINE(OgreApp)

		friend class OgreWidget;
public:
	~OgreApp();

	// 获取Ogre::RenderWindow
	Ogre::RenderWindow* getRenderWindow() { return mRenderWindow; }
	Ogre::SceneManager*  getSceneManager() { return mSceneManager; }	
	OgreLog*                         getLog() { return mLog; }	

	void start();                                                //开始场景
	void drawGrid();
	bool loadScene(const std::string &fileName);
	bool saveScene(const std::string &fileName);
	void clearScene();                                     //清空场景内容
	void createNewScene(MapInfo info);                          //创建新场景
	void log(const Ogre::String &msg);
	void setPropertyWindow(PropertyDockWidget* pd);
	void setListWindow(ListDockWidget* ld);
	Ogre::Vector3 getPositionFromScreen(float screenX, float screenY);    // 屏幕坐标转换到场景坐标
	Ogre::Vector2 getRenderWindowSize();                                                      // 获取当前渲染窗口RenderWindow窗口大小
	void setOperationState(OPERATOR_STATE op);


	NodeObject* createObject(const std::string &meshName);				  // 创建一个实体, 每个ObjectWapper代表一个场景中的模型实体,
	NodeObject* createObject(const std::string &name,
		const std::string &meshName);														      // 创建一个实体
	bool destroyObject(const std::string& name);									      // 销毁一个实体
	NodeObject* queryScene(float screenX, float screenY);						  //  查询场景, 获取模型实体
	NodeObject* selectObject(const std::string &objectName, 
		bool boundingCamera = true);																  // 将某一模型设置为选中状态
	NodeObject* getCurrentSelectObject();												  // 获取当前选中模型实体

private:
	OgreApp();
	OgreApp(const OgreApp&);
	OgreApp& operator=(const OgreApp&);	

	// 初始化Ogre显示, 初始化Qt Widgt之后调用, 只在第一次调用有效.
	// hwnd 显示窗口的窗口句柄
	// width height 显示窗口的宽和高
	void initRenderWindow(HWND hwnd, int width, int height);	
	void update();                                              // 更新
	bool configure(void);	                               // 获取Ogre配置, 成功返回true
	void setupResources(void);	                   // 加载资源
	void createSceneManager();                   // 创建场景管理器
	void resize(int width, int height);           // 更改显示窗口大小

	// 重写鼠标键盘事件
	void  keyPressEvent(QKeyEvent*  evt);
	void  keyReleaseEvent(QKeyEvent*  evt);
	void  mousePressEvent(QMouseEvent*  evt);
	void  mouseReleaseEvent(QMouseEvent*  evt);
	void  mouseMoveEvent(QMouseEvent*  evt);
	void  wheelEvent(QWheelEvent*  evt);

	void createCamera();
	void updateCamera();

	void showCursor(bool show);	                // 显示/隐藏系统光标
	void setupLog();                                           //设置日志系统
	void selectObjectChanged(NodeObject* obj);      //选择的对象改变时更新属性栏

private:
	Ogre::Root					   *mRoot;
	Ogre::RenderWindow   *mRenderWindow;
	Ogre::SceneManager	   *mSceneManager;
	Ogre::Camera                  *mCamera;    
	Ogre::Viewport               *mViewport;   
	Ogre::RaySceneQuery   *mRayQuery;

	OgreTerrain                     *mTerrain;
	OgreLog                           *mLog;
	OgreGrid                          *mGrid;
	NodeObject                    *mCurrentObject;

	Ogre::Vector3                  mMoveVector;
	float                                    mMoveSpeed;
	float                                    mRotateSpeed;
	MOUSE_STATE				     mMouseButtonState;
	QPoint							     mLastFrameMousePos;
	QPoint							     mCurrentMousePos;
	QPoint                                 mCursorLastPos;       

	OPERATOR_STATE          mOperationState;

	PropertyDockWidget     *mPropertyWindows;
	ListDockWidget               *mListWindows;
};

#endif //OGREAPP_H
