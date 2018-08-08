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
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TerrainPaletteWidget
{
public:
    QFormLayout *formLayout;
    QGroupBox *textures_group_box;
    QGridLayout *gridLayout;
    QWidget *widget_4;
    QWidget *widget_2;
    QWidget *widget;
    QWidget *widget_3;
    QWidget *widget_5;
    QWidget *widget_6;
    QWidget *widget_7;
    QWidget *widget_8;
    QGroupBox *deform_group_box;
    QHBoxLayout *horizontalLayout;
    QPushButton *raise_button;
    QPushButton *lower_button;

    void setupUi(QWidget *TerrainPaletteWidget)
    {
        if (TerrainPaletteWidget->objectName().isEmpty())
            TerrainPaletteWidget->setObjectName(QStringLiteral("TerrainPaletteWidget"));
        TerrainPaletteWidget->resize(232, 225);
        formLayout = new QFormLayout(TerrainPaletteWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        textures_group_box = new QGroupBox(TerrainPaletteWidget);
        textures_group_box->setObjectName(QStringLiteral("textures_group_box"));
        textures_group_box->setMaximumSize(QSize(214, 16777215));
        gridLayout = new QGridLayout(textures_group_box);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget_4 = new QWidget(textures_group_box);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setMinimumSize(QSize(40, 40));
        widget_4->setMaximumSize(QSize(40, 40));

        gridLayout->addWidget(widget_4, 1, 3, 1, 1);

        widget_2 = new QWidget(textures_group_box);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMinimumSize(QSize(40, 40));
        widget_2->setMaximumSize(QSize(40, 40));

        gridLayout->addWidget(widget_2, 1, 1, 1, 1);

        widget = new QWidget(textures_group_box);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(40, 40));
        widget->setMaximumSize(QSize(40, 40));

        gridLayout->addWidget(widget, 1, 0, 1, 1);

        widget_3 = new QWidget(textures_group_box);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMinimumSize(QSize(40, 40));
        widget_3->setMaximumSize(QSize(40, 40));

        gridLayout->addWidget(widget_3, 1, 2, 1, 1);

        widget_5 = new QWidget(textures_group_box);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        widget_5->setMinimumSize(QSize(40, 40));
        widget_5->setMaximumSize(QSize(40, 40));

        gridLayout->addWidget(widget_5, 2, 0, 1, 1);

        widget_6 = new QWidget(textures_group_box);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        widget_6->setMinimumSize(QSize(40, 40));
        widget_6->setMaximumSize(QSize(40, 40));

        gridLayout->addWidget(widget_6, 2, 1, 1, 1);

        widget_7 = new QWidget(textures_group_box);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        widget_7->setMinimumSize(QSize(40, 40));
        widget_7->setMaximumSize(QSize(40, 40));

        gridLayout->addWidget(widget_7, 2, 2, 1, 1);

        widget_8 = new QWidget(textures_group_box);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        widget_8->setMinimumSize(QSize(40, 40));
        widget_8->setMaximumSize(QSize(40, 40));

        gridLayout->addWidget(widget_8, 2, 3, 1, 1);


        formLayout->setWidget(0, QFormLayout::SpanningRole, textures_group_box);

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


        formLayout->setWidget(2, QFormLayout::SpanningRole, deform_group_box);


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
