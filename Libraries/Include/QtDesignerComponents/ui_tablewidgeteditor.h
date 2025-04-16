/*

* Copyright (C) 2016 The Qt Company Ltd.
* SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

*/

/********************************************************************************
** Form generated from reading UI file 'tablewidgeteditor.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABLEWIDGETEDITOR_H
#define UI_TABLEWIDGETEDITOR_H

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
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Ui_TableWidgetEditor
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *itemsTab;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableWidget;
    QHBoxLayout *buttonsLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *showPropertiesButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *qdesigner_internal__TableWidgetEditor)
    {
        if (qdesigner_internal__TableWidgetEditor->objectName().isEmpty())
            qdesigner_internal__TableWidgetEditor->setObjectName("qdesigner_internal__TableWidgetEditor");
        qdesigner_internal__TableWidgetEditor->resize(550, 360);
        verticalLayout = new QVBoxLayout(qdesigner_internal__TableWidgetEditor);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(qdesigner_internal__TableWidgetEditor);
        tabWidget->setObjectName("tabWidget");
        itemsTab = new QWidget();
        itemsTab->setObjectName("itemsTab");
        verticalLayout_2 = new QVBoxLayout(itemsTab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        widget = new QWidget(itemsTab);
        widget->setObjectName("widget");
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName("verticalLayout_3");
        tableWidget = new QTableWidget(widget);
        tableWidget->setObjectName("tableWidget");

        verticalLayout_3->addWidget(tableWidget);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setObjectName("buttonsLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(horizontalSpacer);

        showPropertiesButton = new QPushButton(widget);
        showPropertiesButton->setObjectName("showPropertiesButton");

        buttonsLayout->addWidget(showPropertiesButton);


        verticalLayout_3->addLayout(buttonsLayout);


        verticalLayout_2->addWidget(widget);

        tabWidget->addTab(itemsTab, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(qdesigner_internal__TableWidgetEditor);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(qdesigner_internal__TableWidgetEditor);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, qdesigner_internal__TableWidgetEditor, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, qdesigner_internal__TableWidgetEditor, qOverload<>(&QDialog::reject));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(qdesigner_internal__TableWidgetEditor);
    } // setupUi

    void retranslateUi(QDialog *qdesigner_internal__TableWidgetEditor)
    {
        qdesigner_internal__TableWidgetEditor->setWindowTitle(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Edit Table Widget", nullptr));
#if QT_CONFIG(tooltip)
        tableWidget->setToolTip(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Table Items", nullptr));
#endif // QT_CONFIG(tooltip)
        showPropertiesButton->setText(QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "Properties &>>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(itemsTab), QCoreApplication::translate("qdesigner_internal::TableWidgetEditor", "&Items", nullptr));
    } // retranslateUi

};

} // namespace qdesigner_internal

namespace qdesigner_internal {
namespace Ui {
    class TableWidgetEditor: public Ui_TableWidgetEditor {};
} // namespace Ui
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // UI_TABLEWIDGETEDITOR_H
