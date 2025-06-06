/*

* Copyright (C) 2016 The Qt Company Ltd.
* SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

*/

/********************************************************************************
** Form generated from reading UI file 'qtgradientviewdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGRADIENTVIEWDIALOG_H
#define UI_QTGRADIENTVIEWDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>
#include "qtgradientview_p.h"

QT_BEGIN_NAMESPACE

class Ui_QtGradientViewDialog
{
public:
    QVBoxLayout *vboxLayout;
    QtGradientView *gradientView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *QtGradientViewDialog)
    {
        if (QtGradientViewDialog->objectName().isEmpty())
            QtGradientViewDialog->setObjectName("QtGradientViewDialog");
        QtGradientViewDialog->resize(178, 72);
        vboxLayout = new QVBoxLayout(QtGradientViewDialog);
        vboxLayout->setObjectName("vboxLayout");
        gradientView = new QtGradientView(QtGradientViewDialog);
        gradientView->setObjectName("gradientView");
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gradientView->sizePolicy().hasHeightForWidth());
        gradientView->setSizePolicy(sizePolicy);

        vboxLayout->addWidget(gradientView);

        buttonBox = new QDialogButtonBox(QtGradientViewDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(QtGradientViewDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, QtGradientViewDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, QtGradientViewDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(QtGradientViewDialog);
    } // setupUi

    void retranslateUi(QDialog *QtGradientViewDialog)
    {
        QtGradientViewDialog->setWindowTitle(QCoreApplication::translate("QtGradientViewDialog", "Select Gradient", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtGradientViewDialog: public Ui_QtGradientViewDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGRADIENTVIEWDIALOG_H
