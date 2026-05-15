#pragma once
#include <qpalette.h>

inline QPalette getDarkPalette()
{
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,          QColor(0x19, 0x23, 0x2D));
    darkPalette.setColor(QPalette::WindowText,      QColor(0xDF, 0xE1, 0xE2));
    darkPalette.setColor(QPalette::Base,            QColor(0x0E, 0x14, 0x1A));
    darkPalette.setColor(QPalette::AlternateBase,   QColor(0x19, 0x23, 0x2D));
    darkPalette.setColor(QPalette::ToolTipBase,     QColor(0x19, 0x23, 0x2D));
    darkPalette.setColor(QPalette::ToolTipText,     QColor(0xDF, 0xE1, 0xE2));
    darkPalette.setColor(QPalette::Text,            QColor(0xDF, 0xE1, 0xE2));
    darkPalette.setColor(QPalette::Button,          QColor(0x19, 0x23, 0x2D));
    darkPalette.setColor(QPalette::ButtonText,      QColor(0xDF, 0xE1, 0xE2));
    darkPalette.setColor(QPalette::BrightText,      QColor(0xFF, 0x00, 0x00));
    darkPalette.setColor(QPalette::Link,            QColor(0x1A, 0x72, 0xBB));
    darkPalette.setColor(QPalette::Highlight,       QColor(0x1A, 0x72, 0xBB));
    darkPalette.setColor(QPalette::HighlightedText, QColor(0xDF, 0xE1, 0xE2));
    darkPalette.setColor(QPalette::Dark,            QColor(0x0E, 0x14, 0x1A));

    return darkPalette;
}
