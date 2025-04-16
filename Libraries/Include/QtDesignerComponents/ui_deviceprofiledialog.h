/********************************************************************************
** Form generated from reading UI file 'deviceprofiledialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICEPROFILEDIALOG_H
#define UI_DEVICEPROFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "dpi_chooser.h"

QT_BEGIN_NAMESPACE

class Ui_DeviceProfileDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *SystemSettingsWidget;
    QFormLayout *formLayout;
    QLabel *m_systemFontFamilyLabel;
    QFontComboBox *m_systemFontComboBox;
    QLabel *m_systemFontSizeLabel;
    QComboBox *m_systemFontSizeCombo;
    QLabel *m_styleLabel;
    QComboBox *m_styleCombo;
    QLabel *m_systemDPILabel;
    qdesigner_internal::DPI_Chooser *m_dpiChooser;
    QLabel *m_nameLabel;
    QLineEdit *m_nameLineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DeviceProfileDialog)
    {
        if (DeviceProfileDialog->objectName().isEmpty())
            DeviceProfileDialog->setObjectName("DeviceProfileDialog");
        DeviceProfileDialog->resize(348, 209);
        verticalLayout = new QVBoxLayout(DeviceProfileDialog);
        verticalLayout->setObjectName("verticalLayout");
        SystemSettingsWidget = new QWidget(DeviceProfileDialog);
        SystemSettingsWidget->setObjectName("SystemSettingsWidget");
        formLayout = new QFormLayout(SystemSettingsWidget);
        formLayout->setObjectName("formLayout");
        m_systemFontFamilyLabel = new QLabel(SystemSettingsWidget);
        m_systemFontFamilyLabel->setObjectName("m_systemFontFamilyLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, m_systemFontFamilyLabel);

        m_systemFontComboBox = new QFontComboBox(SystemSettingsWidget);
        m_systemFontComboBox->setObjectName("m_systemFontComboBox");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, m_systemFontComboBox);

        m_systemFontSizeLabel = new QLabel(SystemSettingsWidget);
        m_systemFontSizeLabel->setObjectName("m_systemFontSizeLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, m_systemFontSizeLabel);

        m_systemFontSizeCombo = new QComboBox(SystemSettingsWidget);
        m_systemFontSizeCombo->setObjectName("m_systemFontSizeCombo");
        m_systemFontSizeCombo->setEditable(true);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, m_systemFontSizeCombo);

        m_styleLabel = new QLabel(SystemSettingsWidget);
        m_styleLabel->setObjectName("m_styleLabel");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, m_styleLabel);

        m_styleCombo = new QComboBox(SystemSettingsWidget);
        m_styleCombo->setObjectName("m_styleCombo");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, m_styleCombo);

        m_systemDPILabel = new QLabel(SystemSettingsWidget);
        m_systemDPILabel->setObjectName("m_systemDPILabel");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, m_systemDPILabel);

        m_dpiChooser = new qdesigner_internal::DPI_Chooser(SystemSettingsWidget);
        m_dpiChooser->setObjectName("m_dpiChooser");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, m_dpiChooser);

        m_nameLabel = new QLabel(SystemSettingsWidget);
        m_nameLabel->setObjectName("m_nameLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, m_nameLabel);

        m_nameLineEdit = new QLineEdit(SystemSettingsWidget);
        m_nameLineEdit->setObjectName("m_nameLineEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, m_nameLineEdit);


        verticalLayout->addWidget(SystemSettingsWidget);

        buttonBox = new QDialogButtonBox(DeviceProfileDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::Open|QDialogButtonBox::Save);

        verticalLayout->addWidget(buttonBox);

#if QT_CONFIG(shortcut)
        m_systemFontFamilyLabel->setBuddy(m_systemFontComboBox);
        m_systemFontSizeLabel->setBuddy(m_systemFontSizeCombo);
        m_styleLabel->setBuddy(m_styleCombo);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(m_nameLineEdit, m_systemFontComboBox);
        QWidget::setTabOrder(m_systemFontComboBox, m_systemFontSizeCombo);
        QWidget::setTabOrder(m_systemFontSizeCombo, m_styleCombo);
        QWidget::setTabOrder(m_styleCombo, buttonBox);

        retranslateUi(DeviceProfileDialog);

        QMetaObject::connectSlotsByName(DeviceProfileDialog);
    } // setupUi

    void retranslateUi(QDialog *DeviceProfileDialog)
    {
        m_systemFontFamilyLabel->setText(QCoreApplication::translate("DeviceProfileDialog", "&Family", nullptr));
        m_systemFontSizeLabel->setText(QCoreApplication::translate("DeviceProfileDialog", "&Point Size", nullptr));
        m_styleLabel->setText(QCoreApplication::translate("DeviceProfileDialog", "Style", nullptr));
        m_systemDPILabel->setText(QCoreApplication::translate("DeviceProfileDialog", "Device DPI", nullptr));
        m_nameLabel->setText(QCoreApplication::translate("DeviceProfileDialog", "Name", nullptr));
        (void)DeviceProfileDialog;
    } // retranslateUi

};

namespace Ui {
    class DeviceProfileDialog: public Ui_DeviceProfileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICEPROFILEDIALOG_H
