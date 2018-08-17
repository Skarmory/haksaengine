/********************************************************************************
** Form generated from reading UI file 'mapeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPEDITOR_H
#define UI_MAPEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "customopenglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_mapeditorClass
{
public:
    QWidget *centralWidget;
    CustomOpenGLWidget *openGLWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *mapeditorClass)
    {
        if (mapeditorClass->objectName().isEmpty())
            mapeditorClass->setObjectName(QStringLiteral("mapeditorClass"));
        mapeditorClass->resize(821, 657);
        centralWidget = new QWidget(mapeditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        openGLWidget = new CustomOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(10, 10, 800, 600));
        mapeditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mapeditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 821, 21));
        mapeditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(mapeditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mapeditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(mapeditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        mapeditorClass->setStatusBar(statusBar);

        retranslateUi(mapeditorClass);

        QMetaObject::connectSlotsByName(mapeditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *mapeditorClass)
    {
        mapeditorClass->setWindowTitle(QApplication::translate("mapeditorClass", "mapeditor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mapeditorClass: public Ui_mapeditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPEDITOR_H
