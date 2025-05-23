// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCOMBOBOX_P_H
#define QCOMBOBOX_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtWidgets/private/qtwidgetsglobal_p.h>
#include "QtWidgets/qcombobox.h"

#include "QtWidgets/qabstractslider.h"
#include "QtWidgets/qapplication.h"
#include "QtWidgets/qstyleditemdelegate.h"
#include "QtGui/qstandarditemmodel.h"
#include "QtWidgets/qlineedit.h"
#include "QtWidgets/qlistview.h"
#include "QtGui/qpainter.h"
#include "QtWidgets/qstyle.h"
#include "QtWidgets/qstyleoption.h"
#include "QtCore/qtimer.h"
#include "private/qwidget_p.h"
#include "QtCore/qpointer.h"
#if QT_CONFIG(completer)
#include "QtWidgets/qcompleter.h"
#endif
#include "QtGui/qevent.h"

#include <limits.h>

QT_REQUIRE_CONFIG(combobox);

QT_BEGIN_NAMESPACE

class QPlatformMenu;

class QComboBoxListView : public QListView
{
    Q_OBJECT
public:
    QComboBoxListView(QComboBox *cmb = nullptr) : combo(cmb)
    {
        if (cmb)
            setScreen(cmb->screen());
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        resizeContents(viewport()->width(), contentsSize().height());
        QListView::resizeEvent(event);
    }

    void initViewItemOption(QStyleOptionViewItem *option) const override
    {
        QListView::initViewItemOption(option);
        option->showDecorationSelected = true;
        if (combo)
            option->font = combo->font();
    }

    void paintEvent(QPaintEvent *e) override
    {
        if (combo) {
            QStyleOptionComboBox opt;
            opt.initFrom(combo);
            opt.editable = combo->isEditable();
            if (combo->style()->styleHint(QStyle::SH_ComboBox_Popup, &opt, combo)) {
                //we paint the empty menu area to avoid having blank space that can happen when scrolling
                QStyleOptionMenuItem menuOpt;
                menuOpt.initFrom(this);
                menuOpt.palette = palette();
                menuOpt.state = QStyle::State_None;
                menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
                menuOpt.menuRect = e->rect();
                menuOpt.maxIconWidth = 0;
                menuOpt.reservedShortcutWidth = 0;
                QPainter p(viewport());
                combo->style()->drawControl(QStyle::CE_MenuEmptyArea, &menuOpt, &p, this);
            }
        }
        QListView::paintEvent(e);
    }

private:
    QComboBox *combo;
};

class Q_AUTOTEST_EXPORT QComboBoxPrivateScroller : public QWidget
{
    Q_OBJECT

public:
    QComboBoxPrivateScroller(QAbstractSlider::SliderAction action, QWidget *parent)
        : QWidget(parent), sliderAction(action)
    {
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        setAttribute(Qt::WA_NoMousePropagation);
    }
    QSize sizeHint() const override {
        return QSize(20, style()->pixelMetric(QStyle::PM_MenuScrollerHeight, nullptr, this));
    }

protected:
    inline void stopTimer() {
        timer.stop();
    }

    inline void startTimer() {
        timer.start(100, this);
        fast = false;
    }

    void enterEvent(QEnterEvent *) override {
        startTimer();
    }

    void leaveEvent(QEvent *) override {
        stopTimer();
    }
    void timerEvent(QTimerEvent *e) override {
        if (e->timerId() == timer.timerId()) {
            emit doScroll(sliderAction);
            if (fast) {
                emit doScroll(sliderAction);
                emit doScroll(sliderAction);
            }
        }
    }
    void hideEvent(QHideEvent *) override {
        stopTimer();
    }

    void mouseMoveEvent(QMouseEvent *e) override
    {
        // Enable fast scrolling if the cursor is directly above or below the popup.
        const int mouseX = e->position().toPoint().x();
        const int mouseY = e->position().toPoint().y();
        const bool horizontallyInside = pos().x() < mouseX && mouseX < rect().right() + 1;
        const bool verticallyOutside = (sliderAction == QAbstractSlider::SliderSingleStepAdd) ?
                                        rect().bottom() + 1 < mouseY : mouseY < pos().y();

        fast = horizontallyInside && verticallyOutside;
    }

    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        QStyleOptionMenuItem menuOpt;
        menuOpt.initFrom(this);
        menuOpt.checkType = QStyleOptionMenuItem::NotCheckable;
        menuOpt.menuRect = rect();
        menuOpt.maxIconWidth = 0;
        menuOpt.reservedShortcutWidth = 0;
        menuOpt.menuItemType = QStyleOptionMenuItem::Scroller;
        if (sliderAction == QAbstractSlider::SliderSingleStepAdd)
            menuOpt.state |= QStyle::State_DownArrow;
        p.eraseRect(rect());
        style()->drawControl(QStyle::CE_MenuScroller, &menuOpt, &p);
    }

Q_SIGNALS:
    void doScroll(int action);

private:
    QAbstractSlider::SliderAction sliderAction;
    QBasicTimer timer;
    bool fast = false;
};

class Q_WIDGETS_EXPORT QComboBoxPrivateContainer : public QFrame
{
    Q_OBJECT

public:
    QComboBoxPrivateContainer(QAbstractItemView *itemView, QComboBox *parent);
    ~QComboBoxPrivateContainer();
    QAbstractItemView *itemView() const;
    void setItemView(QAbstractItemView *itemView);
    int spacing() const;
    int topMargin() const;
    int bottomMargin() const { return topMargin(); }
    void updateTopBottomMargin();
    void updateStyleSettings();

    QTimer blockMouseReleaseTimer;
    QBasicTimer adjustSizeTimer;
    QPoint initialClickPosition;

public Q_SLOTS:
    void scrollItemView(int action);
    void hideScrollers();
    void updateScrollers();
    void viewDestroyed();

protected:
    void changeEvent(QEvent *e) override;
    bool eventFilter(QObject *o, QEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;
    void timerEvent(QTimerEvent *timerEvent) override;
    void resizeEvent(QResizeEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    QStyleOptionComboBox comboStyleOption() const;

Q_SIGNALS:
    void itemSelected(const QModelIndex &);
    void resetButton();

private:
    QComboBox *combo;
    QAbstractItemView *view = nullptr;
    QComboBoxPrivateScroller *top = nullptr;
    QComboBoxPrivateScroller *bottom = nullptr;
    QElapsedTimer popupTimer;
    bool maybeIgnoreMouseButtonRelease = false;

    friend class QComboBox;
    friend class QComboBoxPrivate;
};

class Q_AUTOTEST_EXPORT QComboMenuDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    QComboMenuDelegate(QObject *parent, QComboBox *cmb)
    : QAbstractItemDelegate(parent), mCombo(cmb), pressedIndex(-1)
    {}

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override {
        QStyleOptionMenuItem opt = getStyleOption(option, index);
        painter->fillRect(option.rect, opt.palette.window());
        mCombo->style()->drawControl(QStyle::CE_MenuItem, &opt, painter, mCombo);
    }
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override {
        QStyleOptionMenuItem opt = getStyleOption(option, index);
        return mCombo->style()->sizeFromContents(
            QStyle::CT_MenuItem, &opt, option.rect.size(), mCombo);
    }
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    QStyleOptionMenuItem getStyleOption(const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const;
    QComboBox *mCombo;
    int pressedIndex;
};

class Q_AUTOTEST_EXPORT QComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QComboBoxDelegate(QObject *parent, QComboBox *cmb)
        : QStyledItemDelegate(parent), mCombo(cmb)
    {}

    static bool isSeparator(const QModelIndex &index) {
        return index.data(Qt::AccessibleDescriptionRole).toString()
                == QLatin1StringView("separator");
    }
    static void setSeparator(QAbstractItemModel *model, const QModelIndex &index) {
        model->setData(index, QString::fromLatin1("separator"), Qt::AccessibleDescriptionRole);
        if (QStandardItemModel *m = qobject_cast<QStandardItemModel*>(model))
            if (QStandardItem *item = m->itemFromIndex(index))
                item->setFlags(item->flags() & ~(Qt::ItemIsSelectable|Qt::ItemIsEnabled));
    }

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override {
        if (isSeparator(index)) {
            QRect rect = option.rect;
            if (const QAbstractItemView *view = qobject_cast<const QAbstractItemView*>(option.widget))
                rect.setWidth(view->viewport()->width());
            QStyleOption opt;
            opt.rect = rect;
            mCombo->style()->drawPrimitive(QStyle::PE_IndicatorToolBarSeparator, &opt, painter, mCombo);
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override {
        if (isSeparator(index)) {
            int pm = mCombo->style()->pixelMetric(QStyle::PM_DefaultFrameWidth, nullptr, mCombo);
            return QSize(pm, pm);
        }
        return QStyledItemDelegate::sizeHint(option, index);
    }
private:
    QComboBox *mCombo;
};

class Q_AUTOTEST_EXPORT QComboBoxPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QComboBox)
public:
    QComboBoxPrivate();
    ~QComboBoxPrivate();
    void init();
    QComboBoxPrivateContainer* viewContainer();
    void updateLineEditGeometry();
    Qt::MatchFlags matchFlags() const;
    void editingFinished();
    void returnPressed();
    void complete();
    void itemSelected(const QModelIndex &item);
    bool contains(const QString &text, int role);
    void emitActivated(const QModelIndex &index);
    void emitHighlighted(const QModelIndex &index);
    void emitCurrentIndexChanged(const QModelIndex &index);
    void modelDestroyed();
    void modelReset();
    void updateMicroFocus() { q_func()->updateMicroFocus(); } // PMF connect doesn't handle default args
#if QT_CONFIG(completer)
    void completerActivated(const QModelIndex &index);
#endif
    void resetButton();
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void updateIndexBeforeChange();
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsRemoved(const QModelIndex &parent, int start, int end);
    void updateArrow(QStyle::StateFlag state);
    bool updateHoverControl(const QPoint &pos);
    void trySetValidIndex();
    QRect popupGeometry(const QPoint &globalPos) const;
    QStyle::SubControl newHoverControl(const QPoint &pos);
    int computeWidthHint() const;
    QSize recomputeSizeHint(QSize &sh) const;
    void adjustComboBoxSize();
    QString itemText(const QModelIndex &index) const;
    QIcon itemIcon(const QModelIndex &index) const;
    int itemRole() const;
    void updateLayoutDirection();
    void setCurrentIndex(const QModelIndex &index);
    void updateDelegate(bool force = false);
    void initViewItemOption(QStyleOptionViewItem *option) const;
    void keyboardSearchString(const QString &text);
    void modelChanged();
    void updateViewContainerPaletteAndOpacity();
    void updateFocusPolicy();
    void showPopupFromMouseEvent(QMouseEvent *e);
    void doHidePopup();
    void updateCurrentText(const QString &text);
    void connectModel();
    void disconnectModel();

#ifdef Q_OS_MAC
    void cleanupNativePopup();
    bool showNativePopup();
    struct IndexSetter {
        int index;
        QComboBox *cb;

        void operator()(void)
        {
            cb->setCurrentIndex(index);
            cb->d_func()->emitActivated(cb->d_func()->currentIndex);
        }
    };
#endif

    std::array<QMetaObject::Connection, 8> modelConnections;
    QAbstractItemModel *model = nullptr;
    QLineEdit *lineEdit = nullptr;
    QPointer<QComboBoxPrivateContainer> container;
#ifdef Q_OS_MAC
    QPlatformMenu *m_platformMenu = nullptr;
#endif
    QPersistentModelIndex currentIndex;
    QPersistentModelIndex root;
    QString placeholderText;
    QString currentText;
    QRect hoverRect;
    QSize iconSize;
    mutable QSize minimumSizeHint;
    mutable QSize sizeHint;
    QComboBox::InsertPolicy insertPolicy = QComboBox::InsertAtBottom;
    QComboBox::SizeAdjustPolicy sizeAdjustPolicy = QComboBox::AdjustToContentsOnFirstShow;
    QStyle::StateFlag arrowState = QStyle::State_None;
    QStyle::SubControl hoverControl = QStyle::SC_None;
    QComboBox::LabelDrawingMode labelDrawingMode = QComboBox::LabelDrawingMode::UseStyle;
    int minimumContentsLength = 0;
    int indexBeforeChange = -1;
    int maxVisibleItems = 10;
    int maxCount = (std::numeric_limits<int>::max)();
    int modelColumn = 0;
    int placeholderIndex = -1;
    bool shownOnce : 1;
    bool duplicatesEnabled : 1;
    bool frame : 1;
    bool inserting : 1;
    bool hidingPopup : 1;
};

QT_END_NAMESPACE

#endif // QCOMBOBOX_P_H
