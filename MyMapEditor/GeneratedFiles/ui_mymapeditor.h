/********************************************************************************
** Form generated from reading UI file 'mymapeditor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYMAPEDITOR_H
#define UI_MYMAPEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyMapEditorClass
{
public:
    QAction *mActionNew;
    QAction *mActionOpen;
    QAction *mActionSave;
    QAction *mActionExit;
    QAction *mActionSelect;
    QAction *mActionMove;
    QAction *mActionScale;
    QAction *mActionRotate;
    QAction *mActionUp;
    QAction *mActionNext;
    QAction *mActionList;
    QAction *mActionResource;
    QAction *mActionProperty;
    QAction *mActionLog;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MyMapEditorClass)
    {
        if (MyMapEditorClass->objectName().isEmpty())
            MyMapEditorClass->setObjectName(QString::fromUtf8("MyMapEditorClass"));
        MyMapEditorClass->resize(600, 400);
        mActionNew = new QAction(MyMapEditorClass);
        mActionNew->setObjectName(QString::fromUtf8("mActionNew"));
        mActionNew->setCheckable(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/File/filenew.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionNew->setIcon(icon);
        mActionOpen = new QAction(MyMapEditorClass);
        mActionOpen->setObjectName(QString::fromUtf8("mActionOpen"));
        mActionOpen->setCheckable(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/File/fileopen.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionOpen->setIcon(icon1);
        mActionSave = new QAction(MyMapEditorClass);
        mActionSave->setObjectName(QString::fromUtf8("mActionSave"));
        mActionSave->setCheckable(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/File/filesave.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionSave->setIcon(icon2);
        mActionExit = new QAction(MyMapEditorClass);
        mActionExit->setObjectName(QString::fromUtf8("mActionExit"));
        mActionExit->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/File/exit.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionExit->setIcon(icon3);
        mActionSelect = new QAction(MyMapEditorClass);
        mActionSelect->setObjectName(QString::fromUtf8("mActionSelect"));
        mActionSelect->setCheckable(true);
        mActionSelect->setChecked(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Tool/select.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionSelect->setIcon(icon4);
        mActionMove = new QAction(MyMapEditorClass);
        mActionMove->setObjectName(QString::fromUtf8("mActionMove"));
        mActionMove->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Tool/move.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionMove->setIcon(icon5);
        mActionScale = new QAction(MyMapEditorClass);
        mActionScale->setObjectName(QString::fromUtf8("mActionScale"));
        mActionScale->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/Tool/scale.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionScale->setIcon(icon6);
        mActionRotate = new QAction(MyMapEditorClass);
        mActionRotate->setObjectName(QString::fromUtf8("mActionRotate"));
        mActionRotate->setCheckable(true);
        mActionRotate->setChecked(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Tool/rotate.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionRotate->setIcon(icon7);
        mActionUp = new QAction(MyMapEditorClass);
        mActionUp->setObjectName(QString::fromUtf8("mActionUp"));
        mActionUp->setCheckable(false);
        mActionNext = new QAction(MyMapEditorClass);
        mActionNext->setObjectName(QString::fromUtf8("mActionNext"));
        mActionList = new QAction(MyMapEditorClass);
        mActionList->setObjectName(QString::fromUtf8("mActionList"));
        mActionList->setCheckable(true);
        mActionList->setChecked(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/View/scene.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionList->setIcon(icon8);
        mActionResource = new QAction(MyMapEditorClass);
        mActionResource->setObjectName(QString::fromUtf8("mActionResource"));
        mActionResource->setCheckable(true);
        mActionResource->setChecked(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/View/entity.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionResource->setIcon(icon9);
        mActionProperty = new QAction(MyMapEditorClass);
        mActionProperty->setObjectName(QString::fromUtf8("mActionProperty"));
        mActionProperty->setCheckable(true);
        mActionProperty->setChecked(true);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/View/properties.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionProperty->setIcon(icon10);
        mActionLog = new QAction(MyMapEditorClass);
        mActionLog->setObjectName(QString::fromUtf8("mActionLog"));
        mActionLog->setCheckable(true);
        mActionLog->setChecked(true);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/View/info.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mActionLog->setIcon(icon11);
        centralWidget = new QWidget(MyMapEditorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MyMapEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MyMapEditorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        MyMapEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MyMapEditorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setMovable(false);
        MyMapEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MyMapEditorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MyMapEditorClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(mActionNew);
        menu->addSeparator();
        menu->addAction(mActionOpen);
        menu->addAction(mActionSave);
        menu->addSeparator();
        menu->addAction(mActionExit);
        menu_2->addAction(mActionSelect);
        menu_2->addAction(mActionMove);
        menu_2->addAction(mActionScale);
        menu_2->addAction(mActionRotate);
        menu_2->addSeparator();
        menu_2->addAction(mActionUp);
        menu_2->addAction(mActionNext);
        menu_3->addAction(mActionList);
        menu_3->addSeparator();
        menu_3->addAction(mActionResource);
        menu_3->addAction(mActionProperty);
        menu_3->addSeparator();
        menu_3->addAction(mActionLog);
        mainToolBar->addAction(mActionNew);
        mainToolBar->addAction(mActionOpen);
        mainToolBar->addAction(mActionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(mActionSelect);
        mainToolBar->addAction(mActionMove);
        mainToolBar->addAction(mActionScale);
        mainToolBar->addAction(mActionRotate);
        mainToolBar->addSeparator();
        mainToolBar->addAction(mActionList);
        mainToolBar->addAction(mActionResource);
        mainToolBar->addAction(mActionProperty);
        mainToolBar->addAction(mActionLog);

        retranslateUi(MyMapEditorClass);

        QMetaObject::connectSlotsByName(MyMapEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *MyMapEditorClass)
    {
        MyMapEditorClass->setWindowTitle(QApplication::translate("MyMapEditorClass", "\345\234\260\345\233\276\347\274\226\350\276\221\345\231\250", 0, QApplication::UnicodeUTF8));
        mActionNew->setText(QApplication::translate("MyMapEditorClass", "\346\226\260\345\273\272", 0, QApplication::UnicodeUTF8));
        mActionOpen->setText(QApplication::translate("MyMapEditorClass", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
        mActionSave->setText(QApplication::translate("MyMapEditorClass", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
        mActionExit->setText(QApplication::translate("MyMapEditorClass", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        mActionSelect->setText(QApplication::translate("MyMapEditorClass", "\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        mActionMove->setText(QApplication::translate("MyMapEditorClass", "\347\247\273\345\212\250", 0, QApplication::UnicodeUTF8));
        mActionScale->setText(QApplication::translate("MyMapEditorClass", "\347\274\251\346\224\276", 0, QApplication::UnicodeUTF8));
        mActionRotate->setText(QApplication::translate("MyMapEditorClass", "\346\227\213\350\275\254", 0, QApplication::UnicodeUTF8));
        mActionUp->setText(QApplication::translate("MyMapEditorClass", "\344\270\212\344\270\200\346\255\245", 0, QApplication::UnicodeUTF8));
        mActionNext->setText(QApplication::translate("MyMapEditorClass", "\344\270\213\344\270\200\346\255\245", 0, QApplication::UnicodeUTF8));
        mActionList->setText(QApplication::translate("MyMapEditorClass", "\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        mActionResource->setText(QApplication::translate("MyMapEditorClass", "\350\265\204\346\272\220", 0, QApplication::UnicodeUTF8));
        mActionProperty->setText(QApplication::translate("MyMapEditorClass", "\345\261\236\346\200\247", 0, QApplication::UnicodeUTF8));
        mActionLog->setText(QApplication::translate("MyMapEditorClass", "\346\227\245\345\277\227", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("MyMapEditorClass", "\350\217\234\345\215\225", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("MyMapEditorClass", "\345\267\245\345\205\267", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("MyMapEditorClass", "\350\247\206\345\233\276", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MyMapEditorClass: public Ui_MyMapEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYMAPEDITOR_H
