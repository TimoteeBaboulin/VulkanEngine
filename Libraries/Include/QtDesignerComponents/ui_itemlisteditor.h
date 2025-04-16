/*

* Copyright (C) 2016 The Qt Company Ltd.
* SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

*/

/********************************************************************************
** Form generated from reading UI file 'itemlisteditor.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ITEMLISTEDITOR_H
#define UI_ITEMLISTEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Ui_ItemListEditor
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *buttonsLayout;
    QToolButton *newListItemButton;
    QToolButton *deleteListItemButton;
    QSpacerItem *spacerItem;
    QToolButton *moveListItemUpButton;
    QToolButton *moveListItemDownButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *showPropertiesButton;

    void setupUi(QWidget *qdesigner_internal__ItemListEditor)
    {
        if (qdesigner_internal__ItemListEditor->objectName().isEmpty())
            qdesigner_internal__ItemListEditor->setObjectName("qdesigner_internal__ItemListEditor");
        qdesigner_internal__ItemListEditor->resize(550, 360);
        verticalLayout_2 = new QVBoxLayout(qdesigner_internal__ItemListEditor);
        verticalLayout_2->setObjectName("verticalLayout_2");
        widget = new QWidget(qdesigner_internal__ItemListEditor);
        widget->setObjectName("widget");
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName("verticalLayout");
        listWidget = new QListWidget(widget);
        listWidget->setObjectName("listWidget");
        listWidget->setEnabled(true);

        verticalLayout->addWidget(listWidget);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setObjectName("buttonsLayout");
        newListItemButton = new QToolButton(widget);
        newListItemButton->setObjectName("newListItemButton");

        buttonsLayout->addWidget(newListItemButton);

        deleteListItemButton = new QToolButton(widget);
        deleteListItemButton->setObjectName("deleteListItemButton");

        buttonsLayout->addWidget(deleteListItemButton);

        spacerItem = new QSpacerItem(16, 10, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(spacerItem);

        moveListItemUpButton = new QToolButton(widget);
        moveListItemUpButton->setObjectName("moveListItemUpButton");

        buttonsLayout->addWidget(moveListItemUpButton);

        moveListItemDownButton = new QToolButton(widget);
        moveListItemDownButton->setObjectName("moveListItemDownButton");

        buttonsLayout->addWidget(moveListItemDownButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(horizontalSpacer);

        showPropertiesButton = new QPushButton(widget);
        showPropertiesButton->setObjectName("showPropertiesButton");

        buttonsLayout->addWidget(showPropertiesButton);


        verticalLayout->addLayout(buttonsLayout);


        verticalLayout_2->addWidget(widget);


        retranslateUi(qdesigner_internal__ItemListEditor);

        QMetaObject::connectSlotsByName(qdesigner_internal__ItemListEditor);
    } // setupUi

    void retranslateUi(QWidget *qdesigner_internal__ItemListEditor)
    {
        qdesigner_internal__ItemListEditor->setWindowTitle(QString());
#if QT_CONFIG(tooltip)
        listWidget->setToolTip(QCoreApplication::translate("qdesigner_internal::ItemListEditor", "Items List", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        newListItemButton->setToolTip(QCoreApplication::translate("qdesigner_internal::ItemListEditor", "New Item", nullptr));
#endif // QT_CONFIG(tooltip)
        newListItemButton->setText(QCoreApplication::translate("qdesigner_internal::ItemListEditor", "&New", nullptr));
#if QT_CONFIG(tooltip)
        deleteListItemButton->setToolTip(QCoreApplication::translate("qdesigner_internal::ItemListEditor", "Delete Item", nullptr));
#endif // QT_CONFIG(tooltip)
        deleteListItemButton->setText(QCoreApplication::translate("qdesigner_internal::ItemListEditor", "&Delete", nullptr));
#if QT_CONFIG(tooltip)
        moveListItemUpButton->setToolTip(QCoreApplication::translate("qdesigner_internal::ItemListEditor", "Move Item Up", nullptr));
#endif // QT_CONFIG(tooltip)
        moveListItemUpButton->setText(QCoreApplication::translate("qdesigner_internal::ItemListEditor", "U", nullptr));
#if QT_CONFIG(tooltip)
        moveListItemDownButton->setToolTip(QCoreApplication::translate("qdesigner_internal::ItemListEditor", "Move Item Down", nullptr));
#endif // QT_CONFIG(tooltip)
        moveListItemDownButton->setText(QCoreApplication::translate("qdesigner_internal::ItemListEditor", "D", nullptr));
        showPropertiesButton->setText(QCoreApplication::translate("qdesigner_internal::ItemListEditor", "Properties &>>", nullptr));
    } // retranslateUi

};

} // namespace qdesigner_internal

namespace qdesigner_internal {
namespace Ui {
    class ItemListEditor: public Ui_ItemListEditor {};
} // namespace Ui
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // UI_ITEMLISTEDITOR_H
