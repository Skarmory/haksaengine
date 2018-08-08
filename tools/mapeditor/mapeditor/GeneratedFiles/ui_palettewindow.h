/********************************************************************************
** Form generated from reading UI file 'palettewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PALETTEWINDOW_H
#define UI_PALETTEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PaletteWindow
{
public:
    QWidget *centralWidget;
    QComboBox *palette_combo_box;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PaletteWindow)
    {
        if (PaletteWindow->objectName().isEmpty())
            PaletteWindow->setObjectName(QStringLiteral("PaletteWindow"));
        PaletteWindow->resize(240, 485);
        centralWidget = new QWidget(PaletteWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        palette_combo_box = new QComboBox(centralWidget);
        palette_combo_box->addItem(QString());
        palette_combo_box->addItem(QString());
        palette_combo_box->setObjectName(QStringLiteral("palette_combo_box"));
        palette_combo_box->setGeometry(QRect(10, 10, 60, 20));
        PaletteWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PaletteWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 240, 21));
        PaletteWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PaletteWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        PaletteWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(PaletteWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PaletteWindow->setStatusBar(statusBar);

        retranslateUi(PaletteWindow);

        QMetaObject::connectSlotsByName(PaletteWindow);
    } // setupUi

    void retranslateUi(QMainWindow *PaletteWindow)
    {
        PaletteWindow->setWindowTitle(QApplication::translate("PaletteWindow", "PaletteWindow", nullptr));
        palette_combo_box->setItemText(0, QApplication::translate("PaletteWindow", "Terrain", nullptr));
        palette_combo_box->setItemText(1, QApplication::translate("PaletteWindow", "Unit", nullptr));

    } // retranslateUi

};

namespace Ui {
    class PaletteWindow: public Ui_PaletteWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PALETTEWINDOW_H
