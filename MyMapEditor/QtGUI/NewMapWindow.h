#ifndef  NEWMAPWINDOW_H
#define NEWMAPWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QDialogButtonBox>

class MeshDockWidget;

class NewMapWindow  : public QDialog
{
	Q_OBJECT
public:
	NewMapWindow(MeshDockWidget *mesh, QWidget *parent = 0);

private:
	void setupUi(QDialog *newMapWindow);
	void retranslateUi(QDialog *newMapWindow);

	private slots:
		void accept();
private:
	QGridLayout *gridLayout_2;
	QGroupBox *grB_TerrInfo;
	QGridLayout *gridLayout;
	QLabel *lbl_VertexNum;
	QComboBox *cmb_VertexNum;
	QLabel *lbl_WorldSize;
	QComboBox *cmb_WorldSize;
	QLabel *lbl_WroldHeight;
	QComboBox *cmb_WorldHeight;
	QLabel *lbl_Height;
	QComboBox *cmb_Height;
	QLabel *lbl_Diffuse;
	QComboBox *cmb_Diffuse;
	QLabel *lbl_Alpha;
	QComboBox *cmb_Alpha;
	QDialogButtonBox *buttonBox;

	MeshDockWidget  *mMeshDockWidget;
};

#endif //NEWMAPWINDOW_H