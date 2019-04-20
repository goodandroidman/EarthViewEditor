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
* ���ڳ�ʼ��Ogre�Լ�ÿ֡�ĸ��£��������
**/

class OgreApp : public SingletonT<OgreApp>
{
	SINGLETON_DEFINE(OgreApp)

		friend class OgreWidget;
public:
	~OgreApp();

	// ��ȡOgre::RenderWindow
	Ogre::RenderWindow* getRenderWindow() { return mRenderWindow; }
	Ogre::SceneManager*  getSceneManager() { return mSceneManager; }	
	OgreLog*                         getLog() { return mLog; }	

	void start();                                                //��ʼ����
	void drawGrid();
	bool loadScene(const std::string &fileName);
	bool saveScene(const std::string &fileName);
	void clearScene();                                     //��ճ�������
	void createNewScene(MapInfo info);                          //�����³���
	void log(const Ogre::String &msg);
	void setPropertyWindow(PropertyDockWidget* pd);
	void setListWindow(ListDockWidget* ld);
	Ogre::Vector3 getPositionFromScreen(float screenX, float screenY);    // ��Ļ����ת������������
	Ogre::Vector2 getRenderWindowSize();                                                      // ��ȡ��ǰ��Ⱦ����RenderWindow���ڴ�С
	void setOperationState(OPERATOR_STATE op);


	NodeObject* createObject(const std::string &meshName);				  // ����һ��ʵ��, ÿ��ObjectWapper����һ�������е�ģ��ʵ��,
	NodeObject* createObject(const std::string &name,
		const std::string &meshName);														      // ����һ��ʵ��
	bool destroyObject(const std::string& name);									      // ����һ��ʵ��
	NodeObject* queryScene(float screenX, float screenY);						  //  ��ѯ����, ��ȡģ��ʵ��
	NodeObject* selectObject(const std::string &objectName, 
		bool boundingCamera = true);																  // ��ĳһģ������Ϊѡ��״̬
	NodeObject* getCurrentSelectObject();												  // ��ȡ��ǰѡ��ģ��ʵ��

private:
	OgreApp();
	OgreApp(const OgreApp&);
	OgreApp& operator=(const OgreApp&);	

	// ��ʼ��Ogre��ʾ, ��ʼ��Qt Widgt֮�����, ֻ�ڵ�һ�ε�����Ч.
	// hwnd ��ʾ���ڵĴ��ھ��
	// width height ��ʾ���ڵĿ�͸�
	void initRenderWindow(HWND hwnd, int width, int height);	
	void update();                                              // ����
	bool configure(void);	                               // ��ȡOgre����, �ɹ�����true
	void setupResources(void);	                   // ������Դ
	void createSceneManager();                   // ��������������
	void resize(int width, int height);           // ������ʾ���ڴ�С

	// ��д�������¼�
	void  keyPressEvent(QKeyEvent*  evt);
	void  keyReleaseEvent(QKeyEvent*  evt);
	void  mousePressEvent(QMouseEvent*  evt);
	void  mouseReleaseEvent(QMouseEvent*  evt);
	void  mouseMoveEvent(QMouseEvent*  evt);
	void  wheelEvent(QWheelEvent*  evt);

	void createCamera();
	void updateCamera();

	void showCursor(bool show);	                // ��ʾ/����ϵͳ���
	void setupLog();                                           //������־ϵͳ
	void selectObjectChanged(NodeObject* obj);      //ѡ��Ķ���ı�ʱ����������

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
