/*

* Copyright (C) 2016 The Qt Company Ltd.
* SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

*/

/********************************************************************************
** Form generated from reading UI file 'newformwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWFORMWIDGET_H
#define UI_NEWFORMWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Ui_NewFormWidget
{
public:
    QHBoxLayout *hboxLayout;
    QTreeWidget *treeWidget;
    QVBoxLayout *verticalLayout;
    QLabel *lblPreview;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QGroupBox *embeddedGroup;
    QGridLayout *gridLayout;
    QComboBox *profileComboBox;
    QComboBox *sizeComboBox;
    QLabel *label;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *qdesigner_internal__NewFormWidget)
    {
        if (qdesigner_internal__NewFormWidget->objectName().isEmpty())
            qdesigner_internal__NewFormWidget->setObjectName("qdesigner_internal__NewFormWidget");
        qdesigner_internal__NewFormWidget->resize(480, 194);
        hboxLayout = new QHBoxLayout(qdesigner_internal__NewFormWidget);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName("hboxLayout");
        treeWidget = new QTreeWidget(qdesigner_internal__NewFormWidget);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setMinimumSize(QSize(200, 0));
        treeWidget->setIconSize(QSize(128, 128));
        treeWidget->setRootIsDecorated(false);
        treeWidget->setColumnCount(1);

        hboxLayout->addWidget(treeWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        lblPreview = new QLabel(qdesigner_internal__NewFormWidget);
        lblPreview->setObjectName("lblPreview");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblPreview->sizePolicy().hasHeightForWidth());
        lblPreview->setSizePolicy(sizePolicy);
        lblPreview->setLineWidth(1);
        lblPreview->setAlignment(Qt::AlignCenter);
        lblPreview->setMargin(5);

        verticalLayout->addWidget(lblPreview);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(7, 0, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        embeddedGroup = new QGroupBox(qdesigner_internal__NewFormWidget);
        embeddedGroup->setObjectName("embeddedGroup");
        gridLayout = new QGridLayout(embeddedGroup);
        gridLayout->setObjectName("gridLayout");
        profileComboBox = new QComboBox(embeddedGroup);
        profileComboBox->setObjectName("profileComboBox");

        gridLayout->addWidget(profileComboBox, 0, 1, 1, 1);

        sizeComboBox = new QComboBox(embeddedGroup);
        sizeComboBox->setObjectName("sizeComboBox");

        gridLayout->addWidget(sizeComboBox, 1, 1, 1, 1);

        label = new QLabel(embeddedGroup);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(embeddedGroup);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);


        horizontalLayout->addWidget(embeddedGroup);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);


        hboxLayout->addLayout(verticalLayout);


        retranslateUi(qdesigner_internal__NewFormWidget);

        QMetaObject::connectSlotsByName(qdesigner_internal__NewFormWidget);
    } // setupUi

    void retranslateUi(QWidget *qdesigner_internal__NewFormWidget)
    {
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("qdesigner_internal::NewFormWidget", "0", nullptr));
        lblPreview->setText(QCoreApplication::translate("qdesigner_internal::NewFormWidget", "Choose a template for a preview", nullptr));
        embeddedGroup->setTitle(QCoreApplication::translate("qdesigner_internal::NewFormWidget", "Embedded Design", nullptr));
        label->setText(QCoreApplication::translate("qdesigner_internal::NewFormWidget", "Device:", nullptr));
        label_2->setText(QCoreApplication::translate("qdesigner_internal::NewFormWidget", "Screen Size:", nullptr));
        (void)qdesigner_internal__NewFormWidget;
    } // retranslateUi

};

} // namespace qdesigner_internal

namespace qdesigner_internal {
namespace Ui {
    class NewFormWidget: public Ui_NewFormWidget {};
} // namespace Ui
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // UI_NEWFORMWIDGET_H
