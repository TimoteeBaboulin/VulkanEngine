/*

* Copyright (C) 2016 The Qt Company Ltd.
* SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

*/

/********************************************************************************
** Form generated from reading UI file 'formwindowsettings.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMWINDOWSETTINGS_H
#define UI_FORMWINDOWSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <gridpanel_p.h>

QT_BEGIN_NAMESPACE

class Ui_FormWindowSettings
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    qdesigner_internal::GridPanel *gridPanel;
    QGroupBox *includeHintsGroupBox;
    QVBoxLayout *vboxLayout;
    QTextEdit *includeHintsTextEdit;
    QGroupBox *translationsGroupBox;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *idBasedTranslationsCheckBox;
    QHBoxLayout *hboxLayout;
    QGroupBox *pixmapFunctionGroupBox;
    QVBoxLayout *vboxLayout1;
    QLineEdit *pixmapFunctionLineEdit;
    QFrame *line;
    QGroupBox *pixmapFunctionGroupBox_2;
    QVBoxLayout *vboxLayout2;
    QLineEdit *authorLineEdit;
    QSpacerItem *spacerItem;
    QGroupBox *embeddedGroupBox;
    QVBoxLayout *verticalLayout;
    QLabel *deviceProfileLabel;
    QHBoxLayout *hboxLayout1;
    QGroupBox *layoutDefaultGroupBox;
    QGridLayout *gridLayout1;
    QLabel *label_2;
    QLabel *label;
    QSpinBox *defaultSpacingSpinBox;
    QSpinBox *defaultMarginSpinBox;
    QGroupBox *layoutFunctionGroupBox;
    QGridLayout *gridLayout2;
    QLineEdit *spacingFunctionLineEdit;
    QLineEdit *marginFunctionLineEdit;
    QLabel *label_3;
    QLabel *label_3_2;
    QGroupBox *connectionsGroupBox;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *connectSlotsByNameCheckBox;

    void setupUi(QDialog *FormWindowSettings)
    {
        if (FormWindowSettings->objectName().isEmpty())
            FormWindowSettings->setObjectName("FormWindowSettings");
        FormWindowSettings->resize(463, 654);
        gridLayout = new QGridLayout(FormWindowSettings);
        gridLayout->setObjectName("gridLayout");
        buttonBox = new QDialogButtonBox(FormWindowSettings);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 9, 0, 1, 2);

        gridPanel = new qdesigner_internal::GridPanel(FormWindowSettings);
        gridPanel->setObjectName("gridPanel");

        gridLayout->addWidget(gridPanel, 2, 0, 1, 2);

        includeHintsGroupBox = new QGroupBox(FormWindowSettings);
        includeHintsGroupBox->setObjectName("includeHintsGroupBox");
        vboxLayout = new QVBoxLayout(includeHintsGroupBox);
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName("vboxLayout");
        vboxLayout->setContentsMargins(8, 8, 8, 8);
        includeHintsTextEdit = new QTextEdit(includeHintsGroupBox);
        includeHintsTextEdit->setObjectName("includeHintsTextEdit");

        vboxLayout->addWidget(includeHintsTextEdit);


        gridLayout->addWidget(includeHintsGroupBox, 5, 0, 3, 1);

        translationsGroupBox = new QGroupBox(FormWindowSettings);
        translationsGroupBox->setObjectName("translationsGroupBox");
        verticalLayout_2 = new QVBoxLayout(translationsGroupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        idBasedTranslationsCheckBox = new QCheckBox(translationsGroupBox);
        idBasedTranslationsCheckBox->setObjectName("idBasedTranslationsCheckBox");

        verticalLayout_2->addWidget(idBasedTranslationsCheckBox);


        gridLayout->addWidget(translationsGroupBox, 6, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName("hboxLayout");
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        pixmapFunctionGroupBox = new QGroupBox(FormWindowSettings);
        pixmapFunctionGroupBox->setObjectName("pixmapFunctionGroupBox");
        pixmapFunctionGroupBox->setCheckable(true);
        vboxLayout1 = new QVBoxLayout(pixmapFunctionGroupBox);
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName("vboxLayout1");
        vboxLayout1->setContentsMargins(8, 8, 8, 8);
        pixmapFunctionLineEdit = new QLineEdit(pixmapFunctionGroupBox);
        pixmapFunctionLineEdit->setObjectName("pixmapFunctionLineEdit");

        vboxLayout1->addWidget(pixmapFunctionLineEdit);


        hboxLayout->addWidget(pixmapFunctionGroupBox);


        gridLayout->addLayout(hboxLayout, 5, 1, 1, 1);

        line = new QFrame(FormWindowSettings);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line, 8, 0, 1, 2);

        pixmapFunctionGroupBox_2 = new QGroupBox(FormWindowSettings);
        pixmapFunctionGroupBox_2->setObjectName("pixmapFunctionGroupBox_2");
        vboxLayout2 = new QVBoxLayout(pixmapFunctionGroupBox_2);
        vboxLayout2->setSpacing(6);
        vboxLayout2->setObjectName("vboxLayout2");
        vboxLayout2->setContentsMargins(8, 8, 8, 8);
        authorLineEdit = new QLineEdit(pixmapFunctionGroupBox_2);
        authorLineEdit->setObjectName("authorLineEdit");

        vboxLayout2->addWidget(authorLineEdit);


        gridLayout->addWidget(pixmapFunctionGroupBox_2, 1, 0, 1, 2);

        spacerItem = new QSpacerItem(111, 115, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(spacerItem, 7, 1, 1, 1);

        embeddedGroupBox = new QGroupBox(FormWindowSettings);
        embeddedGroupBox->setObjectName("embeddedGroupBox");
        verticalLayout = new QVBoxLayout(embeddedGroupBox);
        verticalLayout->setObjectName("verticalLayout");
        deviceProfileLabel = new QLabel(embeddedGroupBox);
        deviceProfileLabel->setObjectName("deviceProfileLabel");
        deviceProfileLabel->setText(QString::fromUtf8("TextLabel"));

        verticalLayout->addWidget(deviceProfileLabel);


        gridLayout->addWidget(embeddedGroupBox, 0, 0, 1, 2);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName("hboxLayout1");
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        layoutDefaultGroupBox = new QGroupBox(FormWindowSettings);
        layoutDefaultGroupBox->setObjectName("layoutDefaultGroupBox");
        layoutDefaultGroupBox->setCheckable(true);
        gridLayout1 = new QGridLayout(layoutDefaultGroupBox);
        gridLayout1->setSpacing(6);
        gridLayout1->setObjectName("gridLayout1");
        gridLayout1->setContentsMargins(8, 8, 8, 8);
        label_2 = new QLabel(layoutDefaultGroupBox);
        label_2->setObjectName("label_2");

        gridLayout1->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(layoutDefaultGroupBox);
        label->setObjectName("label");

        gridLayout1->addWidget(label, 0, 0, 1, 1);

        defaultSpacingSpinBox = new QSpinBox(layoutDefaultGroupBox);
        defaultSpacingSpinBox->setObjectName("defaultSpacingSpinBox");

        gridLayout1->addWidget(defaultSpacingSpinBox, 1, 1, 1, 1);

        defaultMarginSpinBox = new QSpinBox(layoutDefaultGroupBox);
        defaultMarginSpinBox->setObjectName("defaultMarginSpinBox");

        gridLayout1->addWidget(defaultMarginSpinBox, 0, 1, 1, 1);


        hboxLayout1->addWidget(layoutDefaultGroupBox);

        layoutFunctionGroupBox = new QGroupBox(FormWindowSettings);
        layoutFunctionGroupBox->setObjectName("layoutFunctionGroupBox");
        layoutFunctionGroupBox->setCheckable(true);
        gridLayout2 = new QGridLayout(layoutFunctionGroupBox);
        gridLayout2->setSpacing(6);
        gridLayout2->setObjectName("gridLayout2");
        gridLayout2->setContentsMargins(8, 8, 8, 8);
        spacingFunctionLineEdit = new QLineEdit(layoutFunctionGroupBox);
        spacingFunctionLineEdit->setObjectName("spacingFunctionLineEdit");

        gridLayout2->addWidget(spacingFunctionLineEdit, 1, 1, 1, 1);

        marginFunctionLineEdit = new QLineEdit(layoutFunctionGroupBox);
        marginFunctionLineEdit->setObjectName("marginFunctionLineEdit");

        gridLayout2->addWidget(marginFunctionLineEdit, 0, 1, 1, 1);

        label_3 = new QLabel(layoutFunctionGroupBox);
        label_3->setObjectName("label_3");

        gridLayout2->addWidget(label_3, 0, 0, 1, 1);

        label_3_2 = new QLabel(layoutFunctionGroupBox);
        label_3_2->setObjectName("label_3_2");

        gridLayout2->addWidget(label_3_2, 1, 0, 1, 1);


        hboxLayout1->addWidget(layoutFunctionGroupBox);


        gridLayout->addLayout(hboxLayout1, 3, 0, 1, 2);

        connectionsGroupBox = new QGroupBox(FormWindowSettings);
        connectionsGroupBox->setObjectName("connectionsGroupBox");
        verticalLayout_3 = new QVBoxLayout(connectionsGroupBox);
        verticalLayout_3->setObjectName("verticalLayout_3");
        connectSlotsByNameCheckBox = new QCheckBox(connectionsGroupBox);
        connectSlotsByNameCheckBox->setObjectName("connectSlotsByNameCheckBox");

        verticalLayout_3->addWidget(connectSlotsByNameCheckBox);


        gridLayout->addWidget(connectionsGroupBox, 4, 0, 1, 2);

#if QT_CONFIG(shortcut)
        label_2->setBuddy(defaultSpacingSpinBox);
        label->setBuddy(defaultMarginSpinBox);
        label_3->setBuddy(marginFunctionLineEdit);
        label_3_2->setBuddy(spacingFunctionLineEdit);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(authorLineEdit, defaultMarginSpinBox);
        QWidget::setTabOrder(defaultMarginSpinBox, defaultSpacingSpinBox);
        QWidget::setTabOrder(defaultSpacingSpinBox, marginFunctionLineEdit);
        QWidget::setTabOrder(marginFunctionLineEdit, spacingFunctionLineEdit);
        QWidget::setTabOrder(spacingFunctionLineEdit, pixmapFunctionLineEdit);

        retranslateUi(FormWindowSettings);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, FormWindowSettings, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, FormWindowSettings, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(FormWindowSettings);
    } // setupUi

    void retranslateUi(QDialog *FormWindowSettings)
    {
        FormWindowSettings->setWindowTitle(QCoreApplication::translate("FormWindowSettings", "Form Settings", nullptr));
        gridPanel->setTitle(QCoreApplication::translate("FormWindowSettings", "Grid", nullptr));
        includeHintsGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "&Include Hints", nullptr));
        translationsGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "Translations", nullptr));
        idBasedTranslationsCheckBox->setText(QCoreApplication::translate("FormWindowSettings", "ID-based", nullptr));
        pixmapFunctionGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "&Pixmap Function", nullptr));
        pixmapFunctionGroupBox_2->setTitle(QCoreApplication::translate("FormWindowSettings", "&Author", nullptr));
        embeddedGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "Embedded Design", nullptr));
        layoutDefaultGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "Layout &Default", nullptr));
        label_2->setText(QCoreApplication::translate("FormWindowSettings", "&Spacing:", nullptr));
        label->setText(QCoreApplication::translate("FormWindowSettings", "&Margin:", nullptr));
        layoutFunctionGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "&Layout Function", nullptr));
        label_3->setText(QCoreApplication::translate("FormWindowSettings", "Ma&rgin:", nullptr));
        label_3_2->setText(QCoreApplication::translate("FormWindowSettings", "Spa&cing:", nullptr));
        connectionsGroupBox->setTitle(QCoreApplication::translate("FormWindowSettings", "Connections", nullptr));
        connectSlotsByNameCheckBox->setText(QCoreApplication::translate("FormWindowSettings", "Connect slots by name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormWindowSettings: public Ui_FormWindowSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMWINDOWSETTINGS_H
