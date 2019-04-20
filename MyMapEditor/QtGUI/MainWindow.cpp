#include "MainWindow.h"

MapEditor::MapEditor(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setupWindows(this);

	connect();
	mOgreWidget->setPropertyWindow(mPropertyWidget);
	mOgreWidget->setListWindow(mListWidget);
}

//----------------------------------------------------------------------------------------

MapEditor::~MapEditor()
{

}

//----------------------------------------------------------------------------------------

void MapEditor::setupWindows(QMainWindow *parent)
{
	parent->resize(1366, 700);

	QGridLayout *gridLayout = new QGridLayout(ui.centralWidget);
	gridLayout->setSpacing(6);
	gridLayout->setMargin(10);
	gridLayout->setContentsMargins(2, 4, 2, 2);

	QActionGroup *group = new QActionGroup(this);
	ui.mActionSelect->setActionGroup(group);
	ui.mActionMove->setActionGroup(group);
	ui.mActionRotate->setActionGroup(group);
	ui.mActionScale->setActionGroup(group);

	//------------------------------------------------------------------

	QTabWidget *mTabWidget = new QTabWidget(ui.centralWidget);
	QWidget *mRenderTab = new QWidget();
	mTabWidget->addTab(mRenderTab, QString());
	mTabWidget->setTabText(0, "渲染窗口");
	gridLayout->addWidget(mTabWidget, 0, 0, 1, 1);

	QGridLayout *tmpLayout = new QGridLayout(mRenderTab);
	tmpLayout->setSpacing(6);
	tmpLayout->setMargin(1);

	//------------------------------------------------------------------

	mOgreWidget = new OgreWidget(this, mRenderTab) ;
	mOgreWidget->initOgre();
	tmpLayout->addWidget(mOgreWidget, 0, 0, 1, 1);
	mOgreWidget->drawGrid();

	//------------------------------------------------------------------

	mLogDockWidget = new LogDockWidget(parent);
	parent->addDockWidget(static_cast<Qt::DockWidgetArea>(8), mLogDockWidget);

	//------------------------------------------------------------------

	mMeshWidget = new MeshDockWidget(parent);
	parent->addDockWidget(static_cast<Qt::DockWidgetArea>(2), mMeshWidget);
	mMeshWidget->setEnabled(false);

	//------------------------------------------------------------------

	mPropertyWidget = new PropertyDockWidget(parent);
	parent->addDockWidget(static_cast<Qt::DockWidgetArea>(2), mPropertyWidget);

	//------------------------------------------------------------------

	mListWidget = new ListDockWidget(mPropertyWidget, parent);
	parent->addDockWidget(static_cast<Qt::DockWidgetArea>(1), mListWidget);

	//------------------------------------------------------------------

	mScaleWindow = new ScaleWindow(this);
	mRotateWindow = new RotateWindow(this);
	mNewMapWindow = new NewMapWindow(mMeshWidget, this);
}

//----------------------------------------------------------------------------------------

void MapEditor::connect()
{
	QObject::connect(ui.mActionLog, SIGNAL(toggled(bool)), mLogDockWidget, SLOT(setVisible(bool)));
	QObject::connect(ui.mActionResource, SIGNAL(toggled(bool)), mMeshWidget, SLOT(setVisible(bool)));
	QObject::connect(ui.mActionProperty, SIGNAL(toggled(bool)), mPropertyWidget, SLOT(setVisible(bool)));
	QObject::connect(ui.mActionList, SIGNAL(toggled(bool)), mListWidget, SLOT(setVisible(bool)));	
	QObject::connect(ui.mActionScale, SIGNAL(toggled(bool)), mScaleWindow, SLOT(setVisible(bool)));
	QObject::connect(ui.mActionRotate, SIGNAL(toggled(bool)), mRotateWindow, SLOT(setVisible(bool)));
	QObject::connect(ui.mActionNew, SIGNAL(triggered()), mNewMapWindow, SLOT(show()));

	QObject::connect(ui.mActionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
	QObject::connect(ui.mActionSave, SIGNAL(triggered()), this, SLOT(saveMap()));
	QObject::connect(ui.mActionSelect, SIGNAL(toggled(bool)), this, SLOT(setSelectState()));
	QObject::connect(ui.mActionMove, SIGNAL(toggled(bool)), this, SLOT(setMoveState()));

	QObject::connect(mScaleWindow, SIGNAL(finished(int)), ui.mActionSelect, SLOT(toggle()));
	QObject::connect(mRotateWindow, SIGNAL(finished(int)), ui.mActionSelect, SLOT(toggle()));
}

//----------------------------------------------------------------------------------------

void MapEditor::openFile(const QString &path)
{
	QString fileName = path;

	if(fileName.isNull()) {
		fileName = QFileDialog::getOpenFileName(this, tr("打开场景文件"),
			"", "Scene Files (*.xml)");
	}
	if(!fileName.isEmpty()) {
		if(mOgreWidget->loadScene(fileName))
			mMeshWidget->setEnabled(true);
	}
}

//----------------------------------------------------------------------------------------

void MapEditor::saveMap()
{
	QString fileName =
		QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), tr("XML Files (*.xml)"));
	if (!fileName.isEmpty())
		if(mOgreWidget->saveScene(fileName)) {
			QMessageBox msgBox;
			msgBox.setText("保存成功!");
			msgBox.setIcon(QMessageBox::Information);
			msgBox.exec();
		}
}

//----------------------------------------------------------------------------------------

void MapEditor::setMoveState()
{
	OgreApp::getSingletonPtr()->setOperationState(OPE_MOVE);
	if(ui.mActionSelect->isChecked())
		ui.mActionSelect->setChecked(false);
}

//----------------------------------------------------------------------------------------

void MapEditor::setSelectState()
{
	OgreApp::getSingletonPtr()->setOperationState(OPE_SELECT);
	if(ui.mActionMove->isChecked())
		ui.mActionMove->setChecked(false);
}