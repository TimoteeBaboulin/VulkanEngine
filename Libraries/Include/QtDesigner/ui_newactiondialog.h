/*

* Copyright (C) 2016 The Qt Company Ltd.
* SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

*/

/********************************************************************************
** Form generated from reading UI file 'newactiondialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWACTIONDIALOG_H
#define UI_NEWACTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QKeySequenceEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include "iconselector_p.h"
#include "textpropertyeditor_p.h"

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Ui_NewActionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *textLabel;
    QLineEdit *editActionText;
    QLabel *objectNameLabel;
    QLineEdit *editObjectName;
    QLabel *toolTipLabel;
    QHBoxLayout *toolTipLayout;
    TextPropertyEditor *tooltipEditor;
    QToolButton *toolTipToolButton;
    QLabel *iconThemeLabel;
    qdesigner_internal::IconThemeEditor *iconThemeEditor;
    QLabel *iconLabel;
    QHBoxLayout *horizontalLayout;
    qdesigner_internal::IconSelector *iconSelector;
    QSpacerItem *spacerItem;
    QCheckBox *checkableCheckBox;
    QLabel *checkableLabel;
    QLabel *shortcutLabel;
    QHBoxLayout *keysequenceLayout;
    QKeySequenceEdit *keySequenceEdit;
    QToolButton *keysequenceResetToolButton;
    QLabel *label;
    QComboBox *menuRole;
    QLabel *iconThemeEnumLabel;
    qdesigner_internal::IconThemeEnumEditor *iconThemeEnumEditor;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *qdesigner_internal__NewActionDialog)
    {
        if (qdesigner_internal__NewActionDialog->objectName().isEmpty())
            qdesigner_internal__NewActionDialog->setObjectName("qdesigner_internal__NewActionDialog");
        qdesigner_internal__NewActionDialog->resize(381, 291);
        verticalLayout = new QVBoxLayout(qdesigner_internal__NewActionDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        textLabel = new QLabel(qdesigner_internal__NewActionDialog);
        textLabel->setObjectName("textLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, textLabel);

        editActionText = new QLineEdit(qdesigner_internal__NewActionDialog);
        editActionText->setObjectName("editActionText");
        editActionText->setMinimumSize(QSize(255, 0));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, editActionText);

        objectNameLabel = new QLabel(qdesigner_internal__NewActionDialog);
        objectNameLabel->setObjectName("objectNameLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, objectNameLabel);

        editObjectName = new QLineEdit(qdesigner_internal__NewActionDialog);
        editObjectName->setObjectName("editObjectName");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, editObjectName);

        toolTipLabel = new QLabel(qdesigner_internal__NewActionDialog);
        toolTipLabel->setObjectName("toolTipLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, toolTipLabel);

        toolTipLayout = new QHBoxLayout();
        toolTipLayout->setObjectName("toolTipLayout");
        tooltipEditor = new TextPropertyEditor(qdesigner_internal__NewActionDialog);
        tooltipEditor->setObjectName("tooltipEditor");
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tooltipEditor->sizePolicy().hasHeightForWidth());
        tooltipEditor->setSizePolicy(sizePolicy);

        toolTipLayout->addWidget(tooltipEditor);

        toolTipToolButton = new QToolButton(qdesigner_internal__NewActionDialog);
        toolTipToolButton->setObjectName("toolTipToolButton");

        toolTipLayout->addWidget(toolTipToolButton);


        formLayout->setLayout(2, QFormLayout::ItemRole::FieldRole, toolTipLayout);

        iconThemeLabel = new QLabel(qdesigner_internal__NewActionDialog);
        iconThemeLabel->setObjectName("iconThemeLabel");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, iconThemeLabel);

        iconThemeEditor = new qdesigner_internal::IconThemeEditor(qdesigner_internal__NewActionDialog);
        iconThemeEditor->setObjectName("iconThemeEditor");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, iconThemeEditor);

        iconLabel = new QLabel(qdesigner_internal__NewActionDialog);
        iconLabel->setObjectName("iconLabel");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, iconLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        iconSelector = new qdesigner_internal::IconSelector(qdesigner_internal__NewActionDialog);
        iconSelector->setObjectName("iconSelector");

        horizontalLayout->addWidget(iconSelector);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(spacerItem);


        formLayout->setLayout(5, QFormLayout::ItemRole::FieldRole, horizontalLayout);

        checkableCheckBox = new QCheckBox(qdesigner_internal__NewActionDialog);
        checkableCheckBox->setObjectName("checkableCheckBox");

        formLayout->setWidget(6, QFormLayout::ItemRole::FieldRole, checkableCheckBox);

        checkableLabel = new QLabel(qdesigner_internal__NewActionDialog);
        checkableLabel->setObjectName("checkableLabel");

        formLayout->setWidget(6, QFormLayout::ItemRole::LabelRole, checkableLabel);

        shortcutLabel = new QLabel(qdesigner_internal__NewActionDialog);
        shortcutLabel->setObjectName("shortcutLabel");

        formLayout->setWidget(7, QFormLayout::ItemRole::LabelRole, shortcutLabel);

        keysequenceLayout = new QHBoxLayout();
        keysequenceLayout->setObjectName("keysequenceLayout");
        keySequenceEdit = new QKeySequenceEdit(qdesigner_internal__NewActionDialog);
        keySequenceEdit->setObjectName("keySequenceEdit");
        sizePolicy.setHeightForWidth(keySequenceEdit->sizePolicy().hasHeightForWidth());
        keySequenceEdit->setSizePolicy(sizePolicy);

        keysequenceLayout->addWidget(keySequenceEdit);

        keysequenceResetToolButton = new QToolButton(qdesigner_internal__NewActionDialog);
        keysequenceResetToolButton->setObjectName("keysequenceResetToolButton");

        keysequenceLayout->addWidget(keysequenceResetToolButton);


        formLayout->setLayout(7, QFormLayout::ItemRole::FieldRole, keysequenceLayout);

        label = new QLabel(qdesigner_internal__NewActionDialog);
        label->setObjectName("label");

        formLayout->setWidget(8, QFormLayout::ItemRole::LabelRole, label);

        menuRole = new QComboBox(qdesigner_internal__NewActionDialog);
        menuRole->setObjectName("menuRole");

        formLayout->setWidget(8, QFormLayout::ItemRole::FieldRole, menuRole);

        iconThemeEnumLabel = new QLabel(qdesigner_internal__NewActionDialog);
        iconThemeEnumLabel->setObjectName("iconThemeEnumLabel");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, iconThemeEnumLabel);

        iconThemeEnumEditor = new qdesigner_internal::IconThemeEnumEditor(qdesigner_internal__NewActionDialog);
        iconThemeEnumEditor->setObjectName("iconThemeEnumEditor");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, iconThemeEnumEditor);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        line = new QFrame(qdesigner_internal__NewActionDialog);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        verticalLayout->addWidget(line);

        buttonBox = new QDialogButtonBox(qdesigner_internal__NewActionDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

#if QT_CONFIG(shortcut)
        textLabel->setBuddy(editActionText);
        objectNameLabel->setBuddy(editObjectName);
        toolTipLabel->setBuddy(tooltipEditor);
        iconThemeLabel->setBuddy(iconThemeEditor);
        iconLabel->setBuddy(iconSelector);
        checkableLabel->setBuddy(checkableCheckBox);
        shortcutLabel->setBuddy(keySequenceEdit);
        label->setBuddy(menuRole);
        iconThemeEnumLabel->setBuddy(iconThemeEnumEditor);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(editActionText, editObjectName);

        retranslateUi(qdesigner_internal__NewActionDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, qdesigner_internal__NewActionDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, qdesigner_internal__NewActionDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(qdesigner_internal__NewActionDialog);
    } // setupUi

    void retranslateUi(QDialog *qdesigner_internal__NewActionDialog)
    {
        qdesigner_internal__NewActionDialog->setWindowTitle(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "New Action...", nullptr));
        textLabel->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "&Text:", nullptr));
        objectNameLabel->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "Object &name:", nullptr));
        toolTipLabel->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "T&oolTip:", nullptr));
        toolTipToolButton->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "...", nullptr));
        iconThemeLabel->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "Icon &XDG theme:", nullptr));
        iconLabel->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "&Icon:", nullptr));
        checkableCheckBox->setText(QString());
        checkableLabel->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "&Checkable:", nullptr));
        shortcutLabel->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "&Shortcut:", nullptr));
        keysequenceResetToolButton->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "...", nullptr));
        label->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "&Menu role:", nullptr));
        iconThemeEnumLabel->setText(QCoreApplication::translate("qdesigner_internal::NewActionDialog", "Icon &theme:", nullptr));
    } // retranslateUi

};

} // namespace qdesigner_internal

namespace qdesigner_internal {
namespace Ui {
    class NewActionDialog: public Ui_NewActionDialog {};
} // namespace Ui
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // UI_NEWACTIONDIALOG_H
