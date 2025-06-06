// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef QCUSTOM3DVOLUME_P_H
#define QCUSTOM3DVOLUME_P_H

#include "qcustom3ditem_p.h"
#include "qcustom3dvolume.h"

QT_BEGIN_NAMESPACE

struct QCustomVolumeDirtyBitField
{
    bool textureDimensionsDirty : 1;
    bool slicesDirty : 1;
    bool colorTableDirty : 1;
    bool textureDataDirty : 1;
    bool textureFormatDirty : 1;
    bool alphaDirty : 1;
    bool shaderDirty : 1;

    QCustomVolumeDirtyBitField()
        : textureDimensionsDirty(false)
        , slicesDirty(false)
        , colorTableDirty(false)
        , textureDataDirty(false)
        , textureFormatDirty(false)
        , alphaDirty(false)
        , shaderDirty(false)
    {}
};

class QCustom3DVolumePrivate : public QCustom3DItemPrivate
{
    Q_DECLARE_PUBLIC(QCustom3DVolume)

public:
    QCustom3DVolumePrivate();
    QCustom3DVolumePrivate(QVector3D position,
                           QVector3D scaling,
                           const QQuaternion &rotation,
                           int textureWidth,
                           int textureHeight,
                           int textureDepth,
                           QList<uchar> *textureData,
                           QImage::Format textureFormat,
                           const QList<QRgb> &colorTable);
    ~QCustom3DVolumePrivate() override;

    void resetDirtyBits();
    QImage renderSlice(Qt::Axis axis, int index);

public:
    int m_textureWidth;
    int m_textureHeight;
    int m_textureDepth;
    int m_sliceIndexX;
    int m_sliceIndexY;
    int m_sliceIndexZ;

    QImage::Format m_textureFormat;
    QList<QRgb> m_colorTable;
    QList<uchar> *m_textureData;

    float m_alphaMultiplier;
    bool m_preserveOpacity;
    bool m_useHighDefShader;

    bool m_drawSlices;
    bool m_drawSliceFrames;
    QColor m_sliceFrameColor;
    QVector3D m_sliceFrameWidths;
    QVector3D m_sliceFrameGaps;
    QVector3D m_sliceFrameThicknesses;

    QCustomVolumeDirtyBitField m_dirtyBitsVolume;

private:
    int multipliedAlphaValue(int alpha);
};

QT_END_NAMESPACE

#endif
