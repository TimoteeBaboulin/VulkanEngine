/********************************************************************************
** Form generated from reading UI file 'templateoptionspage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEMPLATEOPTIONSPAGE_H
#define UI_TEMPLATEOPTIONSPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class Ui_TemplateOptionsWidget
{
public:
    QGridLayout *gridLayout;
    QGroupBox *m_templatePathGroupBox;
    QGridLayout *gridLayout1;
    QListWidget *m_templatePathListWidget;
    QToolButton *m_addTemplatePathButton;
    QToolButton *m_removeTemplatePathButton;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *qdesigner_internal__TemplateOptionsWidget)
    {
        if (qdesigner_internal__TemplateOptionsWidget->objectName().isEmpty())
            qdesigner_internal__TemplateOptionsWidget->setObjectName("qdesigner_internal__TemplateOptionsWidget");
        qdesigner_internal__TemplateOptionsWidget->resize(376, 387);
        gridLayout = new QGridLayout(qdesigner_internal__TemplateOptionsWidget);
        gridLayout->setObjectName("gridLayout");
        m_templatePathGroupBox = new QGroupBox(qdesigner_internal__TemplateOptionsWidget);
        m_templatePathGroupBox->setObjectName("m_templatePathGroupBox");
        gridLayout1 = new QGridLayout(m_templatePathGroupBox);
        gridLayout1->setObjectName("gridLayout1");
        m_templatePathListWidget = new QListWidget(m_templatePathGroupBox);
        m_templatePathListWidget->setObjectName("m_templatePathListWidget");

        gridLayout1->addWidget(m_templatePathListWidget, 0, 0, 1, 3);

        m_addTemplatePathButton = new QToolButton(m_templatePathGroupBox);
        m_addTemplatePathButton->setObjectName("m_addTemplatePathButton");

        gridLayout1->addWidget(m_addTemplatePathButton, 1, 0, 1, 1);

        m_removeTemplatePathButton = new QToolButton(m_templatePathGroupBox);
        m_removeTemplatePathButton->setObjectName("m_removeTemplatePathButton");

        gridLayout1->addWidget(m_removeTemplatePathButton, 1, 1, 1, 1);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout1->addItem(spacerItem, 1, 2, 1, 1);


        gridLayout->addWidget(m_templatePathGroupBox, 0, 0, 1, 1);


        retranslateUi(qdesigner_internal__TemplateOptionsWidget);

        QMetaObject::connectSlotsByName(qdesigner_internal__TemplateOptionsWidget);
    } // setupUi

    void retranslateUi(QWidget *qdesigner_internal__TemplateOptionsWidget)
    {
        qdesigner_internal__TemplateOptionsWidget->setWindowTitle(QCoreApplication::translate("qdesigner_internal::TemplateOptionsWidget", "Form", nullptr));
        m_templatePathGroupBox->setTitle(QCoreApplication::translate("qdesigner_internal::TemplateOptionsWidget", "Additional Template Paths", nullptr));
        m_addTemplatePathButton->setText(QCoreApplication::translate("qdesigner_internal::TemplateOptionsWidget", "...", nullptr));
        m_removeTemplatePathButton->setText(QCoreApplication::translate("qdesigner_internal::TemplateOptionsWidget", "...", nullptr));
    } // retranslateUi

};

} // namespace qdesigner_internal

namespace qdesigner_internal {
namespace Ui {
    class TemplateOptionsWidget: public Ui_TemplateOptionsWidget {};
} // namespace Ui
} // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // UI_TEMPLATEOPTIONSPAGE_H
