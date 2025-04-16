/********************************************************************************
** Form generated from reading UI file 'formlayoutrowdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMLAYOUTROWDIALOG_H
#define UI_FORMLAYOUTROWDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FormLayoutRowDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelTextLabel;
    QLineEdit *labelTextLineEdit;
    QLineEdit *labelNameLineEdit;
    QLabel *fieldClassLabel;
    QComboBox *fieldClassComboBox;
    QLabel *fieldNameLabel;
    QLabel *buddyLabel;
    QCheckBox *buddyCheckBox;
    QLabel *rowLabel;
    QSpinBox *rowSpinBox;
    QLineEdit *fieldNameLineEdit;
    QLabel *labelNameLabel;
    QFrame *line;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *FormLayoutRowDialog)
    {
        if (FormLayoutRowDialog->objectName().isEmpty())
            FormLayoutRowDialog->setObjectName("FormLayoutRowDialog");
        verticalLayout = new QVBoxLayout(FormLayoutRowDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        labelTextLabel = new QLabel(FormLayoutRowDialog);
        labelTextLabel->setObjectName("labelTextLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, labelTextLabel);

        labelTextLineEdit = new QLineEdit(FormLayoutRowDialog);
        labelTextLineEdit->setObjectName("labelTextLineEdit");
        labelTextLineEdit->setMinimumSize(QSize(180, 0));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, labelTextLineEdit);

        labelNameLineEdit = new QLineEdit(FormLayoutRowDialog);
        labelNameLineEdit->setObjectName("labelNameLineEdit");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, labelNameLineEdit);

        fieldClassLabel = new QLabel(FormLayoutRowDialog);
        fieldClassLabel->setObjectName("fieldClassLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, fieldClassLabel);

        fieldClassComboBox = new QComboBox(FormLayoutRowDialog);
        fieldClassComboBox->setObjectName("fieldClassComboBox");
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(fieldClassComboBox->sizePolicy().hasHeightForWidth());
        fieldClassComboBox->setSizePolicy(sizePolicy);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, fieldClassComboBox);

        fieldNameLabel = new QLabel(FormLayoutRowDialog);
        fieldNameLabel->setObjectName("fieldNameLabel");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, fieldNameLabel);

        buddyLabel = new QLabel(FormLayoutRowDialog);
        buddyLabel->setObjectName("buddyLabel");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, buddyLabel);

        buddyCheckBox = new QCheckBox(FormLayoutRowDialog);
        buddyCheckBox->setObjectName("buddyCheckBox");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, buddyCheckBox);

        rowLabel = new QLabel(FormLayoutRowDialog);
        rowLabel->setObjectName("rowLabel");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, rowLabel);

        rowSpinBox = new QSpinBox(FormLayoutRowDialog);
        rowSpinBox->setObjectName("rowSpinBox");

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, rowSpinBox);

        fieldNameLineEdit = new QLineEdit(FormLayoutRowDialog);
        fieldNameLineEdit->setObjectName("fieldNameLineEdit");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, fieldNameLineEdit);

        labelNameLabel = new QLabel(FormLayoutRowDialog);
        labelNameLabel->setObjectName("labelNameLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, labelNameLabel);


        verticalLayout->addLayout(formLayout);

        line = new QFrame(FormLayoutRowDialog);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        verticalLayout->addWidget(line);

        buttonBox = new QDialogButtonBox(FormLayoutRowDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

#if QT_CONFIG(shortcut)
        labelTextLabel->setBuddy(labelTextLineEdit);
        fieldClassLabel->setBuddy(fieldClassComboBox);
        fieldNameLabel->setBuddy(fieldNameLineEdit);
        buddyLabel->setBuddy(buddyCheckBox);
        rowLabel->setBuddy(rowSpinBox);
        labelNameLabel->setBuddy(labelNameLineEdit);
#endif // QT_CONFIG(shortcut)

        retranslateUi(FormLayoutRowDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, FormLayoutRowDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, FormLayoutRowDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(FormLayoutRowDialog);
    } // setupUi

    void retranslateUi(QDialog *FormLayoutRowDialog)
    {
        FormLayoutRowDialog->setWindowTitle(QCoreApplication::translate("FormLayoutRowDialog", "Add Form Layout Row", nullptr));
        labelTextLabel->setText(QCoreApplication::translate("FormLayoutRowDialog", "&Label text:", nullptr));
        fieldClassLabel->setText(QCoreApplication::translate("FormLayoutRowDialog", "Field &type:", nullptr));
        fieldNameLabel->setText(QCoreApplication::translate("FormLayoutRowDialog", "&Field name:", nullptr));
        buddyLabel->setText(QCoreApplication::translate("FormLayoutRowDialog", "&Buddy:", nullptr));
        buddyCheckBox->setText(QString());
        rowLabel->setText(QCoreApplication::translate("FormLayoutRowDialog", "&Row:", nullptr));
        labelNameLabel->setText(QCoreApplication::translate("FormLayoutRowDialog", "Label &name:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormLayoutRowDialog: public Ui_FormLayoutRowDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMLAYOUTROWDIALOG_H
