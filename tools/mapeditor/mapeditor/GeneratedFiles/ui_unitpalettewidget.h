/********************************************************************************
** Form generated from reading UI file 'unitpalettewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNITPALETTEWIDGET_H
#define UI_UNITPALETTEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UnitPaletteWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *units_group_box;
    QGridLayout *gridLayout;
    QGroupBox *objects_group_box;
    QGridLayout *gridLayout_2;

    void setupUi(QWidget *UnitPaletteWidget)
    {
        if (UnitPaletteWidget->objectName().isEmpty())
            UnitPaletteWidget->setObjectName(QStringLiteral("UnitPaletteWidget"));
        UnitPaletteWidget->resize(241, 224);
        verticalLayout = new QVBoxLayout(UnitPaletteWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        units_group_box = new QGroupBox(UnitPaletteWidget);
        units_group_box->setObjectName(QStringLiteral("units_group_box"));
        gridLayout = new QGridLayout(units_group_box);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        verticalLayout->addWidget(units_group_box);

        objects_group_box = new QGroupBox(UnitPaletteWidget);
        objects_group_box->setObjectName(QStringLiteral("objects_group_box"));
        gridLayout_2 = new QGridLayout(objects_group_box);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));

        verticalLayout->addWidget(objects_group_box);


        retranslateUi(UnitPaletteWidget);

        QMetaObject::connectSlotsByName(UnitPaletteWidget);
    } // setupUi

    void retranslateUi(QWidget *UnitPaletteWidget)
    {
        UnitPaletteWidget->setWindowTitle(QApplication::translate("UnitPaletteWidget", "UnitPaletteWidget", nullptr));
        units_group_box->setTitle(QApplication::translate("UnitPaletteWidget", "Units", nullptr));
        objects_group_box->setTitle(QApplication::translate("UnitPaletteWidget", "Objects", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UnitPaletteWidget: public Ui_UnitPaletteWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNITPALETTEWIDGET_H
