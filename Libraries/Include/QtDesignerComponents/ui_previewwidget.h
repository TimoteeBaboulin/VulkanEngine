/*

* Copyright (C) 2016 The Qt Company Ltd.
* SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

*/

/********************************************************************************
** Form generated from reading UI file 'previewwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREVIEWWIDGET_H
#define UI_PREVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Ui_PreviewWidget
{
public:
    QGridLayout *gridLayout;
    QGroupBox *buttonGroup;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *RadioButton1;
    QRadioButton *RadioButton2;
    QRadioButton *RadioButton3;
    QCheckBox *CheckBox1;
    QCheckBox *CheckBox2;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *PushButton1;
    QPushButton *toggleButton;
    QHBoxLayout *horizontalLayout;
    QToolButton *toolButton;
    QToolButton *menuToolButton;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *itemGroupBox;
    QVBoxLayout *verticalLayout_5;
    QTreeWidget *treeWidget;
    QGroupBox *simpleGroupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *LineEdit1;
    QComboBox *ComboBox1;
    QHBoxLayout *horizontalLayout_4;
    QSpinBox *SpinBox1;
    QScrollBar *ScrollBar1;
    QSlider *Slider1;
    QGroupBox *displayGroupBox;
    QVBoxLayout *verticalLayout_4;
    QProgressBar *ProgressBar1;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *qdesigner_internal__PreviewWidget)
    {
        if (qdesigner_internal__PreviewWidget->objectName().isEmpty())
            qdesigner_internal__PreviewWidget->setObjectName("qdesigner_internal__PreviewWidget");
        qdesigner_internal__PreviewWidget->resize(608, 367);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(qdesigner_internal__PreviewWidget->sizePolicy().hasHeightForWidth());
        qdesigner_internal__PreviewWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(qdesigner_internal__PreviewWidget);
        gridLayout->setObjectName("gridLayout");
        buttonGroup = new QGroupBox(qdesigner_internal__PreviewWidget);
        buttonGroup->setObjectName("buttonGroup");
        buttonGroup->setCheckable(true);
        horizontalLayout_2 = new QHBoxLayout(buttonGroup);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        RadioButton1 = new QRadioButton(buttonGroup);
        RadioButton1->setObjectName("RadioButton1");
        RadioButton1->setChecked(true);

        verticalLayout_2->addWidget(RadioButton1);

        RadioButton2 = new QRadioButton(buttonGroup);
        RadioButton2->setObjectName("RadioButton2");

        verticalLayout_2->addWidget(RadioButton2);

        RadioButton3 = new QRadioButton(buttonGroup);
        RadioButton3->setObjectName("RadioButton3");

        verticalLayout_2->addWidget(RadioButton3);

        CheckBox1 = new QCheckBox(buttonGroup);
        CheckBox1->setObjectName("CheckBox1");
        CheckBox1->setChecked(true);

        verticalLayout_2->addWidget(CheckBox1);

        CheckBox2 = new QCheckBox(buttonGroup);
        CheckBox2->setObjectName("CheckBox2");
        CheckBox2->setTristate(true);

        verticalLayout_2->addWidget(CheckBox2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        PushButton1 = new QPushButton(buttonGroup);
        PushButton1->setObjectName("PushButton1");

        verticalLayout->addWidget(PushButton1);

        toggleButton = new QPushButton(buttonGroup);
        toggleButton->setObjectName("toggleButton");
        toggleButton->setCheckable(true);
        toggleButton->setChecked(true);
        toggleButton->setFlat(false);

        verticalLayout->addWidget(toggleButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        toolButton = new QToolButton(buttonGroup);
        toolButton->setObjectName("toolButton");

        horizontalLayout->addWidget(toolButton);

        menuToolButton = new QToolButton(buttonGroup);
        menuToolButton->setObjectName("menuToolButton");

        horizontalLayout->addWidget(menuToolButton);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(verticalLayout);


        gridLayout->addWidget(buttonGroup, 0, 0, 1, 1);

        itemGroupBox = new QGroupBox(qdesigner_internal__PreviewWidget);
        itemGroupBox->setObjectName("itemGroupBox");
        itemGroupBox->setCheckable(true);
        verticalLayout_5 = new QVBoxLayout(itemGroupBox);
        verticalLayout_5->setObjectName("verticalLayout_5");
        treeWidget = new QTreeWidget(itemGroupBox);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setAlternatingRowColors(true);

        verticalLayout_5->addWidget(treeWidget);


        gridLayout->addWidget(itemGroupBox, 0, 1, 1, 1);

        simpleGroupBox = new QGroupBox(qdesigner_internal__PreviewWidget);
        simpleGroupBox->setObjectName("simpleGroupBox");
        simpleGroupBox->setCheckable(true);
        verticalLayout_3 = new QVBoxLayout(simpleGroupBox);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        LineEdit1 = new QLineEdit(simpleGroupBox);
        LineEdit1->setObjectName("LineEdit1");
        LineEdit1->setClearButtonEnabled(true);

        horizontalLayout_3->addWidget(LineEdit1);

        ComboBox1 = new QComboBox(simpleGroupBox);
        ComboBox1->addItem(QString());
        ComboBox1->addItem(QString());
        ComboBox1->addItem(QString());
        ComboBox1->setObjectName("ComboBox1");

        horizontalLayout_3->addWidget(ComboBox1);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        SpinBox1 = new QSpinBox(simpleGroupBox);
        SpinBox1->setObjectName("SpinBox1");

        horizontalLayout_4->addWidget(SpinBox1);

        ScrollBar1 = new QScrollBar(simpleGroupBox);
        ScrollBar1->setObjectName("ScrollBar1");
        ScrollBar1->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(ScrollBar1);

        Slider1 = new QSlider(simpleGroupBox);
        Slider1->setObjectName("Slider1");
        Slider1->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(Slider1);


        verticalLayout_3->addLayout(horizontalLayout_4);


        gridLayout->addWidget(simpleGroupBox, 1, 0, 1, 1);

        displayGroupBox = new QGroupBox(qdesigner_internal__PreviewWidget);
        displayGroupBox->setObjectName("displayGroupBox");
        verticalLayout_4 = new QVBoxLayout(displayGroupBox);
        verticalLayout_4->setObjectName("verticalLayout_4");
        ProgressBar1 = new QProgressBar(displayGroupBox);
        ProgressBar1->setObjectName("ProgressBar1");
        ProgressBar1->setValue(50);
        ProgressBar1->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(ProgressBar1);

        label = new QLabel(displayGroupBox);
        label->setObjectName("label");

        verticalLayout_4->addWidget(label);

        label_2 = new QLabel(displayGroupBox);
        label_2->setObjectName("label_2");
        label_2->setFrameShape(QFrame::StyledPanel);

        verticalLayout_4->addWidget(label_2);


        gridLayout->addWidget(displayGroupBox, 1, 1, 1, 1);


        retranslateUi(qdesigner_internal__PreviewWidget);

        QMetaObject::connectSlotsByName(qdesigner_internal__PreviewWidget);
    } // setupUi

    void retranslateUi(QWidget *qdesigner_internal__PreviewWidget)
    {
        qdesigner_internal__PreviewWidget->setWindowTitle(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Preview Window", nullptr));
        buttonGroup->setTitle(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Buttons", nullptr));
        RadioButton1->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "RadioButton1", nullptr));
        RadioButton2->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "RadioButton2", nullptr));
        RadioButton3->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "RadioButton3", nullptr));
        CheckBox1->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "CheckBox1", nullptr));
        CheckBox2->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Tristate CheckBox", nullptr));
        PushButton1->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "PushButton", nullptr));
        toggleButton->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "ToggleButton", nullptr));
        toolButton->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "ToolButton", nullptr));
        menuToolButton->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Menu", nullptr));
        itemGroupBox->setTitle(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Item Views", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Column 1", nullptr));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Top Level 1", nullptr));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(0, QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Nested Item 1", nullptr));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem1->child(1);
        ___qtreewidgetitem3->setText(0, QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Nested Item 2", nullptr));
        QTreeWidgetItem *___qtreewidgetitem4 = ___qtreewidgetitem1->child(2);
        ___qtreewidgetitem4->setText(0, QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Nested Item 3", nullptr));
        treeWidget->setSortingEnabled(__sortingEnabled);

        simpleGroupBox->setTitle(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Simple Input Widgets", nullptr));
        LineEdit1->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "LineEdit", nullptr));
        ComboBox1->setItemText(0, QCoreApplication::translate("qdesigner_internal::PreviewWidget", "ComboBox", nullptr));
        ComboBox1->setItemText(1, QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Item1", nullptr));
        ComboBox1->setItemText(2, QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Item2", nullptr));

        displayGroupBox->setTitle(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "Display Widgets", nullptr));
        label->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "QLabel", nullptr));
        label_2->setText(QCoreApplication::translate("qdesigner_internal::PreviewWidget", "QLabel with frame", nullptr));
    } // retranslateUi

};

} // namespace qdesigner_internal

namespace qdesigner_internal {
namespace Ui {
    class PreviewWidget: public Ui_PreviewWidget {};
} // namespace Ui
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // UI_PREVIEWWIDGET_H
