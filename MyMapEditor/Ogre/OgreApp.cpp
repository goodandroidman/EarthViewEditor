#include "OgreApp.h"
#include <cassert>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

OgreApp::OgreApp() : mRenderWindow(0), mSceneManager(0), mMoveVector(Ogre::Vector3::ZERO),
mMoveSpeed(0.2f), mRotateSpeed(0.01f), mMouseButtonState(BUTTON_UP), mGrid(0)
{
	setupLog();

	Ogre::String  pluginsPath;
	Ogre::String  configPath;
	Ogre::String  resourcePath;
	pluginsPath = "E:/EarthViewEditor/bin/Debug/plugins_d.cfg"; 
	configPath = "E:/EarthViewEditor/bin/Debug/ogre.cfg";
	resourcePath = "Ogre.log";
	mRoot = new Ogre::Root(pluginsPath,configPath,resourcePath);

	if (!configure())
		throw "Ogre 初始化失败!";

	mRoot->initialise(false);

	setupResources();
	createSceneManager();

	mRayQuery = mSceneManager->createRayQuery(Ogre::Ray());
}

//----------------------------------------------------------------------------------------

OgreApp::~OgreApp()
{
	delete mGrid;
}

//----------------------------------------------------------------------------------------

void OgreApp::initRenderWindow(HWND hwnd, int width, int heigth)
{
	if(!mRenderWindow) {
		Ogre::NameValuePairList parms;
		parms["externalWindowHandle"] = Ogre::StringConverter::toString((long)hwnd);
		// 创建RenderWindow
		mRenderWindow = mRoot->createRenderWindow("Editor", width, heigth, false, &parms);

		// 设定为活动
		mRenderWindow->setActive(true);
		mRenderWindow->update();

		// 初始化资源组
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}
}

//----------------------------------------------------------------------------------------

void OgreApp::update()
{
	if(mRenderWindow) {
		mRoot->_fireFrameStarted();
		mRenderWindow->update();
		updateCamera();
		mRoot->_fireFrameEnded();
	}
}

//----------------------------------------------------------------------------------------

void OgreApp::updateCamera()
{	
	mCamera->moveRelative(mMoveVector);              //移动相机
	if (mMouseButtonState == RIGHT_BUTTON_DOWN) {
		QPoint mouseDelta = mCurrentMousePos - mLastFrameMousePos;
		mCamera->yaw(Ogre::Radian(-mouseDelta.x() * mRotateSpeed));
		mCamera->pitch(Ogre::Radian(-mouseDelta.y() * mRotateSpeed));
	}

	mLastFrameMousePos = mCurrentMousePos;
}

//----------------------------------------------------------------------------------------

// 更改显示窗口大小
void OgreApp::resize(int width, int height)
{
	// 这里需要对RenderWindow, Viewport和CEGUI进行更改
	mRenderWindow->resize(width, height);
	mRenderWindow->windowMovedOrResized();
}

//----------------------------------------------------------------------------------------

bool	OgreApp::configure(void)
{
	//#ifdef _DEBUG
	if (mRoot->restoreConfig() || mRoot->showConfigDialog())
		//#else
		//		if (mRoot->showConfigDialog())
		//#endif
		return true;
	else
		return false;
}

//----------------------------------------------------------------------------------------

void OgreApp::createSceneManager()
{
	mSceneManager = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE, "MapEditor");

	// 设置场景阴影参数
	mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	mSceneManager->setShadowFarDistance(100);
	mSceneManager->setShadowColour(Ogre::ColourValue(0.7, 0.73, 0.75));
	mSceneManager->setShadowTextureSize(512);
}

//----------------------------------------------------------------------------------------

// 加载资源
void	OgreApp::setupResources(void)
{
	Ogre::ConfigFile cf;
	cf.load("E://EarthViewEditor//bin//Debug//resources_d.cfg");

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements()) {
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i) {
			typeName = i->first;
			archName = i->second;

			Ogre::ResourceGroupManager::getSingleton().
				addResourceLocation(archName, typeName, secName);
		}

		Util::loadResouse("media/models/Tanks", "FileSystem", RESOURCE_GROUP_MESH, false);
		Util::loadResouse("media/models/Tank", "FileSystem", RESOURCE_GROUP_MESH, false);
		Util::loadResouse("media/models/Normal", "FileSystem", RESOURCE_GROUP_MESH, false);
	}
}

//----------------------------------------------------------------------------------------

void OgreApp::start()
{
	createCamera();
}

//----------------------------------------------------------------------------------------

void OgreApp::createCamera()
{
	assert(mRoot);
	assert(mSceneManager);
	assert(mRenderWindow); 

	mCamera = mSceneManager->createCamera("MyCamera");
	mCamera->setPosition(Ogre::Vector3(0, 15, 30));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(5);
	mViewport = mRenderWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0, 0.0, 0.0, 1));
	mCamera->setAspectRatio(
		Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

	// Make window active and post an update
	mRenderWindow->setActive(true);
	mRenderWindow->update();
}

//----------------------------------------------------------------------------------------

void OgreApp::drawGrid()
{
	clearScene();

	// 创建网格
	mGrid = new OgreGrid(mSceneManager, 30, 30, 20, Ogre::ColourValue(1.0f, 0.0f, 0.0f, 0.8f));
	mGrid->setPosition(Ogre::Vector3(0, 0.15f, 0));
}

//----------------------------------------------------------------------------------------

bool OgreApp::loadScene(const std::string &fileName)
{
	if(ObjectFactory::getSingletonPtr()->loadMap(fileName)) {
		return true;
	} else {
		QMessageBox msgBox;
		msgBox.setText("您所选择的文件无法打开!");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return false;
	}
}

//----------------------------------------------------------------------------------------

void  OgreApp::keyPressEvent(QKeyEvent*  evt)
{
	switch (evt->key()) {
	case Qt::Key_Up :
	case Qt::Key_W : mMoveVector.z -= mMoveSpeed; break;
	case Qt::Key_Down :
	case Qt::Key_S : mMoveVector.z += mMoveSpeed; break;
	case Qt::Key_Left :
	case Qt::Key_A : mMoveVector.x -= mMoveSpeed; break;
	case Qt::Key_Right :
	case Qt::Key_D : mMoveVector.x += mMoveSpeed; break;
	case Qt::Key_PageUp :
	case Qt::Key_Q : mMoveVector.y += mMoveSpeed; break;
	case Qt::Key_PageDown :
	case Qt::Key_E : mMoveVector.y -= mMoveSpeed; break;
	case Qt::Key_Space:	// 空格键释放摄像机绑定
		mCamera->setAutoTracking(false);
	}
}

//----------------------------------------------------------------------------------------

void  OgreApp::keyReleaseEvent(QKeyEvent*  evt)
{
	switch (evt->key()) {
		case Qt::Key_Up :
		case Qt::Key_W : mMoveVector.z += mMoveSpeed; break;
		case Qt::Key_Down :
		case Qt::Key_S : mMoveVector.z -= mMoveSpeed; break;
		case Qt::Key_Left :
		case Qt::Key_A : mMoveVector.x += mMoveSpeed; break;
		case Qt::Key_Right :
		case Qt::Key_D : mMoveVector.x -= mMoveSpeed; break;
		case Qt::Key_PageUp :
		case Qt::Key_Q : mMoveVector.y -= mMoveSpeed; break;
		case Qt::Key_PageDown :
		case Qt::Key_E : mMoveVector.y += mMoveSpeed; break;
	}
}

//----------------------------------------------------------------------------------------

void  OgreApp::mousePressEvent(QMouseEvent*  evt)
{
	if ( evt->button() == Qt::LeftButton ) {
		// 左键点击场景, 查询
		mMouseButtonState = LEFT_BUTTON_DOWN;
		QPoint pos = evt->pos();
		mCurrentObject = queryScene(pos.x(), pos.y());
		selectObjectChanged(mCurrentObject);
	}
	else {
		// 右键事件
		mMouseButtonState = RIGHT_BUTTON_DOWN;
		mCurrentMousePos = evt->pos();
		mLastFrameMousePos = mCurrentMousePos;
		showCursor(false);
	}
}

//----------------------------------------------------------------------------------------

void  OgreApp::mouseReleaseEvent(QMouseEvent*  evt)
{
	if (evt->button() == Qt::RightButton)	// 隐藏鼠标
		showCursor(true);

	mMouseButtonState = BUTTON_UP;
}

//----------------------------------------------------------------------------------------

void  OgreApp::mouseMoveEvent(QMouseEvent*  evt)
{
	if ( mMouseButtonState == RIGHT_BUTTON_DOWN ) {
		mCurrentMousePos = evt->pos();
	}
	else if ( mMouseButtonState == LEFT_BUTTON_DOWN ) {
		if(mCurrentObject) {
			QPoint pos = evt->pos();
			if(mOperationState == OPE_MOVE) {
				int y = mCurrentObject->mNode->getPosition().y;
				mCurrentObject->mNode->setPosition(getPositionFromScreen(pos.x(), pos.y()));
			}
		}
		selectObjectChanged(mCurrentObject);
	}
}

//----------------------------------------------------------------------------------------

void  OgreApp::wheelEvent(QWheelEvent*  evt)
{
	if ( mMouseButtonState == BUTTON_UP) {
		mMoveVector.z = -evt->delta() / 25;
		updateCamera();
		mMoveVector.z = 0;
	}
}

//----------------------------------------------------------------------------------------

void OgreApp::showCursor(bool show)
{
	if (!show) {
		qApp->setOverrideCursor( QCursor( Qt::BlankCursor ) );
		mCursorLastPos = QCursor::pos();		        // 记录鼠标位置
	}
	else {
		qApp->restoreOverrideCursor(); 
		QCursor::setPos(mCursorLastPos);             // 恢复鼠标位置
	}
}

//----------------------------------------------------------------------------------------

void OgreApp::clearScene()
{
	mSceneManager->clearScene();
}

//----------------------------------------------------------------------------------------

void OgreApp::setupLog()
{
	//预先创建定向Log,输出到控制台 
// 	mLog = new OgreLog();
// 	Ogre::LogManager *logManager = new Ogre::LogManager();
// 	Ogre::Log *log = logManager->createLog("MyEditorLog.log", true, true);
// 	log->addListener(mLog);
// 	log->logMessage("<b>----------------------- 编辑器日志信息 -----------------------</b>");
}

//----------------------------------------------------------------------------------------

void OgreApp::log(const Ogre::String &msg)
{
	Ogre::LogManager::getSingletonPtr()->logMessage(msg);
}

//----------------------------------------------------------------------------------------

Ogre::Vector3 OgreApp::getPositionFromScreen(float screenX, float screenY)
{
	static Ogre::Camera *cam = 0;
	cam = mSceneManager->getCameraIterator().getNext();
	Ogre::Vector2 windowSize = getRenderWindowSize();

	// 获取查询射线的起始坐标并向里移动20个单位
	Ogre::Ray ray = cam->getCameraToViewportRay(screenX / windowSize.x,
		screenY / windowSize.y);
	Ogre::Vector3 pos = mTerrain->getPosition(ray);

	pos.y += 5;

	return pos;
}

//----------------------------------------------------------------------------------------

Ogre::Vector2 OgreApp::getRenderWindowSize()
{
	return Ogre::Vector2(mRenderWindow->getWidth(), mRenderWindow->getHeight());
}

//----------------------------------------------------------------------------------------

void OgreApp::createNewScene(MapInfo info)
{
	clearScene();

	Ogre::Plane plane;	                                                               //天空面 
	plane.d = 200;	                                                                       //天空面的高度
	plane.normal = -Ogre::Vector3::UNIT_Y;	                           //法线向下
	mSceneManager->setSkyPlane(true, plane, "Sky");		   //设置天空面 

	mTerrain = new OgreTerrain(mSceneManager, mRoot, mCamera, info);
	mTerrain->createTerrain();
}

//----------------------------------------------------------------------------------------

NodeObject* OgreApp::queryScene(float screenX, float screenY)
{
	static Ogre::Camera *cam = mSceneManager->getCameraIterator().getNext();

	Ogre::Vector2 windowSize = getRenderWindowSize();
	Ogre::Ray mouseRay = cam->getCameraToViewportRay(
		screenX / windowSize.x, screenY / windowSize.y);

	mRayQuery->setRay(mouseRay);
	mRayQuery->setQueryMask(QUERY_MASK);

	Ogre::RaySceneQueryResult &result = mRayQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	if (itr != result.end() && itr->movable) {
		Ogre::MovableObject *obj = itr->movable;
		// 获取查询到的模型实体的信息
		NodeObject* object = ObjectFactory::getSingletonPtr()->getObjectByName(obj->getName().c_str());
		// 如果模型存在并且不是上一次查询的模型则更新
		if (object && object != mCurrentObject) {
			// 将上一次选中的物体包围盒去掉
			if (mCurrentObject) 
				mCurrentObject->mNode->showBoundingBox(false);

			mCurrentObject = object;
			mCurrentObject->mNode->showBoundingBox(true);

			return mCurrentObject;
		}
		//  两次选择了相同的模型
		if (object == mCurrentObject)
			return mCurrentObject;
	}
	// 如果什么也没选中则取消之前选中模型的包围盒
	if (mCurrentObject) {
		mCurrentObject->mNode->showBoundingBox(false);
		mCurrentObject = 0;
	}
	return 0;
}

//----------------------------------------------------------------------------------------

NodeObject* OgreApp::selectObject(const std::string &objectName, 
								  bool boundingCamera)
{
	Ogre::Camera *cam = mSceneManager->getCameraIterator().getNext();
	NodeObject *object = ObjectFactory::getSingletonPtr()->getObjectByName(objectName);
	// 隐藏之前选中模型的包围盒并显示当前模型的包围盒
	if (mCurrentObject)
		mCurrentObject->mNode->showBoundingBox(false);

	object->mNode->showBoundingBox(true);
	mCurrentObject = object;

	//if (boundingCamera) {
	//	// 修正摄像机位置
	//	cam->setPosition(mCurrentObject->mNode->getPosition() + Ogre::Vector3(15.0f, 5.0f, 15.0f));
	//	cam->setAutoTracking(true, object->mNode);
	//}
	return mCurrentObject;
}

//----------------------------------------------------------------------------------------

NodeObject* OgreApp::getCurrentSelectObject()
{
	return mCurrentObject;
}

//----------------------------------------------------------------------------------------

NodeObject* OgreApp::createObject(const std::string &meshName)
{
	NodeObject* obj = ObjectFactory::getSingletonPtr()->createObject(meshName);
	mListWindows->addNode(obj->mName, NODE_OBJECT);
	return obj;
}

//----------------------------------------------------------------------------------------

NodeObject* OgreApp::createObject(const std::string &meshName, const std::string &name)
{
	NodeObject* obj = ObjectFactory::getSingletonPtr()->createObject(meshName, name);
	mListWindows->addNode(obj->mName, NODE_OBJECT);
	return obj;
}

//----------------------------------------------------------------------------------------

bool OgreApp::destroyObject(const std::string& name)
{		
	// 如果当前选中模型是要删除的模型, 则指针设置为0
	if (mCurrentObject && (mCurrentObject->mName == name))
		mCurrentObject = 0;
	return ObjectFactory::getSingletonPtr()->destroyObject(name);
}

//----------------------------------------------------------------------------------------

void OgreApp::setOperationState(OPERATOR_STATE op)
{
	mOperationState = op;
}

//----------------------------------------------------------------------------------------

void OgreApp::selectObjectChanged(NodeObject* obj)
{
	mPropertyWindows->parameterUpdate(obj);
}

//----------------------------------------------------------------------------------------

void OgreApp::setPropertyWindow(PropertyDockWidget* pd)
{
	mPropertyWindows = pd;
}

//----------------------------------------------------------------------------------------

void OgreApp::setListWindow(ListDockWidget* ld)
{
	mListWindows = ld;
}

//----------------------------------------------------------------------------------------

bool OgreApp::saveScene(const std::string &fileName)
{
	if(ObjectFactory::getSingletonPtr()->saveMap(fileName, mTerrain->getTerrainInfo())) {
		return true;
	} else {
		QMessageBox msgBox;
		msgBox.setText("保存失败!");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		return false;
	}
}