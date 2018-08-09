/********************************************************************************
** Form generated from reading UI file 'terrainpalettewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERRAINPALETTEWIDGET_H
#define UI_TERRAINPALETTEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TerrainPaletteWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *textures_group_box;
    QGridLayout *gridLayout;
    QGroupBox *deform_group_box;
    QHBoxLayout *horizontalLayout;
    QPushButton *raise_button;
    QPushButton *lower_button;

    void setupUi(QWidget *TerrainPaletteWidget)
    {
        if (TerrainPaletteWidget->objectName().isEmpty())
            TerrainPaletteWidget->setObjectName(QStringLiteral("TerrainPaletteWidget"));
        TerrainPaletteWidget->resize(232, 225);
        verticalLayout = new QVBoxLayout(TerrainPaletteWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textures_group_box = new QGroupBox(TerrainPaletteWidget);
        textures_group_box->setObjectName(QStringLiteral("textures_group_box"));
        textures_group_box->setMaximumSize(QSize(214, 16777215));
        gridLayout = new QGridLayout(textures_group_box);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        verticalLayout->addWidget(textures_group_box);

        deform_group_box = new QGroupBox(TerrainPaletteWidget);
        deform_group_box->setObjectName(QStringLiteral("deform_group_box"));
        deform_group_box->setMaximumSize(QSize(214, 16777215));
        horizontalLayout = new QHBoxLayout(deform_group_box);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        raise_button = new QPushButton(deform_group_box);
        raise_button->setObjectName(QStringLiteral("raise_button"));
        raise_button->setMinimumSize(QSize(40, 40));
        raise_button->setMaximumSize(QSize(40, 40));
        QIcon icon;
        icon.addFile(QStringLiteral("../icons/raise.png"), QSize(), QIcon::Normal, QIcon::Off);
        raise_button->setIcon(icon);
        raise_button->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(raise_button);

        lower_button = new QPushButton(deform_group_box);
        lower_button->setObjectName(QStringLiteral("lower_button"));
        lower_button->setMinimumSize(QSize(40, 40));
        lower_button->setMaximumSize(QSize(40, 40));
        QIcon icon1;
        icon1.addFile(QStringLiteral("../icons/lower.png"), QSize(), QIcon::Normal, QIcon::Off);
        lower_button->setIcon(icon1);
        lower_button->setIconSize(QSize(32, 32));
        lower_button->setCheckable(false);
        lower_button->setChecked(false);

        horizontalLayout->addWidget(lower_button);


        verticalLayout->addWidget(deform_group_box);


        retranslateUi(TerrainPaletteWidget);

        QMetaObject::connectSlotsByName(TerrainPaletteWidget);
    } // setupUi

    void retranslateUi(QWidget *TerrainPaletteWidget)
    {
        TerrainPaletteWidget->setWindowTitle(QApplication::translate("TerrainPaletteWidget", "TerrainPaletteWidget", nullptr));
        textures_group_box->setTitle(QApplication::translate("TerrainPaletteWidget", "Textures", nullptr));
        deform_group_box->setTitle(QApplication::translate("TerrainPaletteWidget", "Deform", nullptr));
        raise_button->setText(QString());
        lower_button->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TerrainPaletteWidget: public Ui_TerrainPaletteWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERRAINPALETTEWIDGET_H
