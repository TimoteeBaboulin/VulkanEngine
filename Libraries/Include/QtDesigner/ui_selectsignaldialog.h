/********************************************************************************
** Form generated from reading UI file 'selectsignaldialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTSIGNALDIALOG_H
#define UI_SELECTSIGNALDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SelectSignalDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QTreeView *signalList;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SelectSignalDialog)
    {
        if (SelectSignalDialog->objectName().isEmpty())
            SelectSignalDialog->setObjectName("SelectSignalDialog");
        SelectSignalDialog->resize(514, 183);
        verticalLayout = new QVBoxLayout(SelectSignalDialog);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(SelectSignalDialog);
        groupBox->setObjectName("groupBox");
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        signalList = new QTreeView(groupBox);
        signalList->setObjectName("signalList");
        signalList->setSortingEnabled(false);
        signalList->header()->setVisible(false);

        verticalLayout_2->addWidget(signalList);


        verticalLayout->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(SelectSignalDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(SelectSignalDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, SelectSignalDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, SelectSignalDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(SelectSignalDialog);
    } // setupUi

    void retranslateUi(QDialog *SelectSignalDialog)
    {
        SelectSignalDialog->setWindowTitle(QCoreApplication::translate("SelectSignalDialog", "Go to slot", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SelectSignalDialog", "Select signal", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectSignalDialog: public Ui_SelectSignalDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTSIGNALDIALOG_H
