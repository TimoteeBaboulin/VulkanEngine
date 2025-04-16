/********************************************************************************
** Form generated from reading UI file 'qtresourceeditordialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTRESOURCEEDITORDIALOG_H
#define UI_QTRESOURCEEDITORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtResourceEditorDialog
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QWidget *qrcLayoutWidget;
    QGridLayout *qrcLayout;
    QListWidget *qrcFileList;
    QToolButton *newQrcButton;
    QToolButton *removeQrcButton;
    QSpacerItem *spacerItem;
    QToolButton *importQrcButton;
    QWidget *resourceLayoutWidget;
    QGridLayout *resourceLayout;
    QTreeView *resourceTreeView;
    QToolButton *newResourceButton;
    QToolButton *addResourceButton;
    QToolButton *removeResourceButton;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *QtResourceEditorDialog)
    {
        if (QtResourceEditorDialog->objectName().isEmpty())
            QtResourceEditorDialog->setObjectName("QtResourceEditorDialog");
        QtResourceEditorDialog->resize(469, 317);
        verticalLayout = new QVBoxLayout(QtResourceEditorDialog);
        verticalLayout->setObjectName("verticalLayout");
        splitter = new QSplitter(QtResourceEditorDialog);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        splitter->setChildrenCollapsible(false);
        qrcLayoutWidget = new QWidget(splitter);
        qrcLayoutWidget->setObjectName("qrcLayoutWidget");
        qrcLayout = new QGridLayout(qrcLayoutWidget);
        qrcLayout->setObjectName("qrcLayout");
        qrcLayout->setContentsMargins(0, 0, 0, 0);
        qrcFileList = new QListWidget(qrcLayoutWidget);
        qrcFileList->setObjectName("qrcFileList");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(qrcFileList->sizePolicy().hasHeightForWidth());
        qrcFileList->setSizePolicy(sizePolicy);

        qrcLayout->addWidget(qrcFileList, 0, 0, 1, 4);

        newQrcButton = new QToolButton(qrcLayoutWidget);
        newQrcButton->setObjectName("newQrcButton");

        qrcLayout->addWidget(newQrcButton, 1, 0, 1, 1);

        removeQrcButton = new QToolButton(qrcLayoutWidget);
        removeQrcButton->setObjectName("removeQrcButton");

        qrcLayout->addWidget(removeQrcButton, 1, 2, 1, 1);

        spacerItem = new QSpacerItem(21, 20, QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Minimum);

        qrcLayout->addItem(spacerItem, 1, 3, 1, 1);

        importQrcButton = new QToolButton(qrcLayoutWidget);
        importQrcButton->setObjectName("importQrcButton");

        qrcLayout->addWidget(importQrcButton, 1, 1, 1, 1);

        splitter->addWidget(qrcLayoutWidget);
        resourceLayoutWidget = new QWidget(splitter);
        resourceLayoutWidget->setObjectName("resourceLayoutWidget");
        resourceLayout = new QGridLayout(resourceLayoutWidget);
        resourceLayout->setObjectName("resourceLayout");
        resourceLayout->setContentsMargins(0, 0, 0, 0);
        resourceTreeView = new QTreeView(resourceLayoutWidget);
        resourceTreeView->setObjectName("resourceTreeView");

        resourceLayout->addWidget(resourceTreeView, 0, 0, 1, 4);

        newResourceButton = new QToolButton(resourceLayoutWidget);
        newResourceButton->setObjectName("newResourceButton");

        resourceLayout->addWidget(newResourceButton, 1, 0, 1, 1);

        addResourceButton = new QToolButton(resourceLayoutWidget);
        addResourceButton->setObjectName("addResourceButton");

        resourceLayout->addWidget(addResourceButton, 1, 1, 1, 1);

        removeResourceButton = new QToolButton(resourceLayoutWidget);
        removeResourceButton->setObjectName("removeResourceButton");

        resourceLayout->addWidget(removeResourceButton, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        resourceLayout->addItem(horizontalSpacer, 1, 3, 1, 1);

        splitter->addWidget(resourceLayoutWidget);

        verticalLayout->addWidget(splitter);

        buttonBox = new QDialogButtonBox(QtResourceEditorDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(QtResourceEditorDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, QtResourceEditorDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, QtResourceEditorDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(QtResourceEditorDialog);
    } // setupUi

    void retranslateUi(QDialog *QtResourceEditorDialog)
    {
        QtResourceEditorDialog->setWindowTitle(QCoreApplication::translate("QtResourceEditorDialog", "Dialog", nullptr));
#if QT_CONFIG(tooltip)
        newQrcButton->setToolTip(QCoreApplication::translate("QtResourceEditorDialog", "New File", nullptr));
#endif // QT_CONFIG(tooltip)
        newQrcButton->setText(QCoreApplication::translate("QtResourceEditorDialog", "N", nullptr));
#if QT_CONFIG(tooltip)
        removeQrcButton->setToolTip(QCoreApplication::translate("QtResourceEditorDialog", "Remove File", nullptr));
#endif // QT_CONFIG(tooltip)
        removeQrcButton->setText(QCoreApplication::translate("QtResourceEditorDialog", "R", nullptr));
        importQrcButton->setText(QCoreApplication::translate("QtResourceEditorDialog", "I", nullptr));
#if QT_CONFIG(tooltip)
        newResourceButton->setToolTip(QCoreApplication::translate("QtResourceEditorDialog", "New Resource", nullptr));
#endif // QT_CONFIG(tooltip)
        newResourceButton->setText(QCoreApplication::translate("QtResourceEditorDialog", "N", nullptr));
        addResourceButton->setText(QCoreApplication::translate("QtResourceEditorDialog", "A", nullptr));
#if QT_CONFIG(tooltip)
        removeResourceButton->setToolTip(QCoreApplication::translate("QtResourceEditorDialog", "Remove Resource or File", nullptr));
#endif // QT_CONFIG(tooltip)
        removeResourceButton->setText(QCoreApplication::translate("QtResourceEditorDialog", "R", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtResourceEditorDialog: public Ui_QtResourceEditorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTRESOURCEEDITORDIALOG_H
