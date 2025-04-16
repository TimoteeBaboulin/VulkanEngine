/*

* Copyright (C) 2016 The Qt Company Ltd.
* SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

*/

/********************************************************************************
** Form generated from reading UI file 'treewidgeteditor.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TREEWIDGETEDITOR_H
#define UI_TREEWIDGETEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Ui_TreeWidgetEditor
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *itemsTab;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *treeWidget;
    QHBoxLayout *buttonsLayout;
    QToolButton *newItemButton;
    QToolButton *newSubItemButton;
    QToolButton *deleteItemButton;
    QSpacerItem *spacerItem;
    QToolButton *moveItemLeftButton;
    QToolButton *moveItemRightButton;
    QToolButton *moveItemUpButton;
    QToolButton *moveItemDownButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *showPropertiesButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *qdesigner_internal__TreeWidgetEditor)
    {
        if (qdesigner_internal__TreeWidgetEditor->objectName().isEmpty())
            qdesigner_internal__TreeWidgetEditor->setObjectName("qdesigner_internal__TreeWidgetEditor");
        qdesigner_internal__TreeWidgetEditor->resize(700, 360);
        verticalLayout = new QVBoxLayout(qdesigner_internal__TreeWidgetEditor);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(qdesigner_internal__TreeWidgetEditor);
        tabWidget->setObjectName("tabWidget");
        itemsTab = new QWidget();
        itemsTab->setObjectName("itemsTab");
        verticalLayout_3 = new QVBoxLayout(itemsTab);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(9, 9, 9, -1);
        widget = new QWidget(itemsTab);
        widget->setObjectName("widget");
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        treeWidget = new QTreeWidget(widget);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setFocusPolicy(Qt::WheelFocus);

        verticalLayout_2->addWidget(treeWidget);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setObjectName("buttonsLayout");
        newItemButton = new QToolButton(widget);
        newItemButton->setObjectName("newItemButton");

        buttonsLayout->addWidget(newItemButton);

        newSubItemButton = new QToolButton(widget);
        newSubItemButton->setObjectName("newSubItemButton");

        buttonsLayout->addWidget(newSubItemButton);

        deleteItemButton = new QToolButton(widget);
        deleteItemButton->setObjectName("deleteItemButton");

        buttonsLayout->addWidget(deleteItemButton);

        spacerItem = new QSpacerItem(28, 23, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(spacerItem);

        moveItemLeftButton = new QToolButton(widget);
        moveItemLeftButton->setObjectName("moveItemLeftButton");

        buttonsLayout->addWidget(moveItemLeftButton);

        moveItemRightButton = new QToolButton(widget);
        moveItemRightButton->setObjectName("moveItemRightButton");

        buttonsLayout->addWidget(moveItemRightButton);

        moveItemUpButton = new QToolButton(widget);
        moveItemUpButton->setObjectName("moveItemUpButton");

        buttonsLayout->addWidget(moveItemUpButton);

        moveItemDownButton = new QToolButton(widget);
        moveItemDownButton->setObjectName("moveItemDownButton");

        buttonsLayout->addWidget(moveItemDownButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(horizontalSpacer);

        showPropertiesButton = new QPushButton(widget);
        showPropertiesButton->setObjectName("showPropertiesButton");

        buttonsLayout->addWidget(showPropertiesButton);


        verticalLayout_2->addLayout(buttonsLayout);


        verticalLayout_3->addWidget(widget);

        tabWidget->addTab(itemsTab, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(qdesigner_internal__TreeWidgetEditor);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(qdesigner_internal__TreeWidgetEditor);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, qdesigner_internal__TreeWidgetEditor, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, qdesigner_internal__TreeWidgetEditor, qOverload<>(&QDialog::reject));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(qdesigner_internal__TreeWidgetEditor);
    } // setupUi

    void retranslateUi(QDialog *qdesigner_internal__TreeWidgetEditor)
    {
        qdesigner_internal__TreeWidgetEditor->setWindowTitle(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "Edit Tree Widget", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "1", nullptr));
#if QT_CONFIG(tooltip)
        treeWidget->setToolTip(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "Tree Items", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        newItemButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "New Item", nullptr));
#endif // QT_CONFIG(tooltip)
        newItemButton->setText(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "&New", nullptr));
#if QT_CONFIG(tooltip)
        newSubItemButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "New Subitem", nullptr));
#endif // QT_CONFIG(tooltip)
        newSubItemButton->setText(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "New &Subitem", nullptr));
#if QT_CONFIG(tooltip)
        deleteItemButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "Delete Item", nullptr));
#endif // QT_CONFIG(tooltip)
        deleteItemButton->setText(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "&Delete", nullptr));
#if QT_CONFIG(tooltip)
        moveItemLeftButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "Move Item Left (before Parent Item)", nullptr));
#endif // QT_CONFIG(tooltip)
        moveItemLeftButton->setText(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "L", nullptr));
#if QT_CONFIG(tooltip)
        moveItemRightButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "Move Item Right (as a First Subitem of the Next Sibling Item)", nullptr));
#endif // QT_CONFIG(tooltip)
        moveItemRightButton->setText(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "R", nullptr));
#if QT_CONFIG(tooltip)
        moveItemUpButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "Move Item Up", nullptr));
#endif // QT_CONFIG(tooltip)
        moveItemUpButton->setText(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "U", nullptr));
#if QT_CONFIG(tooltip)
        moveItemDownButton->setToolTip(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "Move Item Down", nullptr));
#endif // QT_CONFIG(tooltip)
        moveItemDownButton->setText(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "D", nullptr));
        showPropertiesButton->setText(QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "Properties &>>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(itemsTab), QCoreApplication::translate("qdesigner_internal::TreeWidgetEditor", "&Items", nullptr));
    } // retranslateUi

};

} // namespace qdesigner_internal

namespace qdesigner_internal {
namespace Ui {
    class TreeWidgetEditor: public Ui_TreeWidgetEditor {};
} // namespace Ui
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // UI_TREEWIDGETEDITOR_H
