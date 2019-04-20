#include "NewMapWindow.h"
#include "../Dependence/Common.h"
#include "../Ogre/OgreApp.h"
#include "MeshDockWidget.h"

#include <string>

#define TR(a)  QString::fromLocal8Bit(a)
#define Std(a) std::string((const char *)a.toLocal8Bit())

//----------------------------------------------------------------------------------------

NewMapWindow::NewMapWindow(MeshDockWidget *mesh, QWidget *parent) : 
QDialog(parent), mMeshDockWidget(mesh)
{
	setupUi(this);
}

//----------------------------------------------------------------------------------------

void NewMapWindow::setupUi(QDialog *newMapWindow)
{
	newMapWindow->resize(321, 233);
	gridLayout_2 = new QGridLayout(newMapWindow);
	gridLayout_2->setSpacing(6);
	gridLayout_2->setContentsMargins(11, 11, 11, 11);
	gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
	grB_TerrInfo = new QGroupBox(newMapWindow);
	grB_TerrInfo->setObjectName(QString::fromUtf8("grB_TerrInfo"));
	gridLayout = new QGridLayout(grB_TerrInfo);
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(11, 11, 11, 11);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

	lbl_VertexNum = new QLabel(grB_TerrInfo);
	lbl_VertexNum->setObjectName(QString::fromUtf8("lbl_VertexNum"));
	gridLayout->addWidget(lbl_VertexNum, 0, 0, 1, 1);

	cmb_VertexNum = new QComboBox(grB_TerrInfo);
	cmb_VertexNum->setObjectName(QString::fromUtf8("cmb_VertexNum"));
	gridLayout->addWidget(cmb_VertexNum, 0, 1, 1, 1);

	lbl_WorldSize = new QLabel(grB_TerrInfo);
	lbl_WorldSize->setObjectName(QString::fromUtf8("lbl_WorldSize"));
	gridLayout->addWidget(lbl_WorldSize, 1, 0, 1, 1);

	cmb_WorldSize = new QComboBox(grB_TerrInfo);
	cmb_WorldSize->setObjectName(QString::fromUtf8("cmb_WorldSize"));
	gridLayout->addWidget(cmb_WorldSize, 1, 1, 1, 1);

	lbl_WroldHeight = new QLabel(grB_TerrInfo);
	lbl_WroldHeight->setObjectName(QString::fromUtf8("lbl_WroldHeight"));
	gridLayout->addWidget(lbl_WroldHeight, 2, 0, 1, 1);

	cmb_WorldHeight = new QComboBox(grB_TerrInfo);
	cmb_WorldHeight->setObjectName(QString::fromUtf8("cmb_WorldHeight"));
	gridLayout->addWidget(cmb_WorldHeight, 2, 1, 1, 1);

	lbl_Height = new QLabel(grB_TerrInfo);
	lbl_Height->setObjectName(QString::fromUtf8("lbl_Height"));
	gridLayout->addWidget(lbl_Height, 3, 0, 1, 1);

	cmb_Height = new QComboBox(grB_TerrInfo);
	cmb_Height->setObjectName(QString::fromUtf8("cmb_Height"));
	gridLayout->addWidget(cmb_Height, 3, 1, 1, 1);

	lbl_Diffuse = new QLabel(grB_TerrInfo);
	lbl_Diffuse->setObjectName(QString::fromUtf8("lbl_Diffuse"));
	gridLayout->addWidget(lbl_Diffuse, 4, 0, 1, 1);

	cmb_Diffuse = new QComboBox(grB_TerrInfo);
	cmb_Diffuse->setObjectName(QString::fromUtf8("cmb_Diffuse"));
	gridLayout->addWidget(cmb_Diffuse, 4, 1, 1, 1);

	lbl_Alpha = new QLabel(grB_TerrInfo);
	lbl_Alpha->setObjectName(QString::fromUtf8("lbl_Alpha"));
	gridLayout->addWidget(lbl_Alpha, 5, 0, 1, 1);

	cmb_Alpha = new QComboBox(grB_TerrInfo);
	cmb_Alpha->setObjectName(QString::fromUtf8("cmb_Alpha"));
	gridLayout->addWidget(cmb_Alpha, 5, 1, 1, 1);
	gridLayout_2->addWidget(grB_TerrInfo, 0, 0, 1, 2);

	buttonBox = new QDialogButtonBox(newMapWindow);
	buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
	gridLayout_2->addWidget(buttonBox, 1, 1, 1, 1);

	retranslateUi(newMapWindow);	

	QObject::connect(buttonBox, SIGNAL(accepted()), newMapWindow, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), newMapWindow, SLOT(reject()));
}

//----------------------------------------------------------------------------------------

void NewMapWindow::retranslateUi(QDialog *newMapWindow)
{
	newMapWindow->setWindowTitle("新建");
	grB_TerrInfo->setTitle("地形信息");
	lbl_VertexNum->setText("顶点数目：");
	cmb_VertexNum->clear();
	cmb_VertexNum->insertItems(0, QStringList()
		<< "513"
		);
	lbl_WorldSize->setText("地形大小：");
	cmb_WorldSize->clear();
	cmb_WorldSize->insertItems(0, QStringList()
		<< "1000"
		<< "2000"
		<< "3000"
		);
	lbl_WroldHeight->setText("地形高度：");
	cmb_WorldHeight->clear();
	cmb_WorldHeight->insertItems(0, QStringList()
		<< "100"
		<< "150"
		<< "200"
		<< "250"
		<< "300"
		<< "350"
		<< "400"
		);
	lbl_Height->setText("高度图：");
	cmb_Height->clear();
	cmb_Height->insertItems(0, QStringList()
		<< "terrain.png"
		);
	lbl_Diffuse->setText("纹理：");
	cmb_Diffuse->clear();
	cmb_Diffuse->insertItems(0, QStringList()
		<< "terr_dirt-grass.jpg"
		);
	lbl_Alpha->setText("Alpha：");
	cmb_Alpha->clear();
	cmb_Alpha->insertItems(0, QStringList()
		<< "terrain_detail.jpg"
		);
}

//----------------------------------------------------------------------------------------

void NewMapWindow::accept()
{
	MapInfo info;
	info.TERRAINSIZE = cmb_VertexNum->currentText().toUInt();
	info.HEIGHTMAP = Std(cmb_Height->currentText());
	info.DIFFUSE = Std(cmb_Diffuse->currentText());
	info.SPECIAL= Std(cmb_Alpha->currentText());
	info.WORLDHEIGHT = cmb_WorldHeight->currentText().toUInt();
	info.WORLDSIZE = cmb_WorldSize->currentText().toUInt();

	OgreApp::getSingletonPtr()->createNewScene(info);

	mMeshDockWidget->setEnabled(true);

	QDialog::accept();
}
