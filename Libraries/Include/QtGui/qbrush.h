// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QBRUSH_H
#define QBRUSH_H

#include <QtGui/qtguiglobal.h>
#include <QtCore/qlist.h>
#include <QtCore/qpoint.h>
#include <QtCore/qscopedpointer.h>
#include <QtGui/qcolor.h>
#include <QtGui/qimage.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qtransform.h>

QT_BEGIN_NAMESPACE


struct QBrushData;
class QPixmap;
class QGradient;
class QVariant;
struct QBrushDataPointerDeleter
{
    void operator()(QBrushData *d) const noexcept;
};

class Q_GUI_EXPORT QBrush
{
public:
    QBrush();
    QBrush(Qt::BrushStyle bs);
    QBrush(const QColor &color, Qt::BrushStyle bs=Qt::SolidPattern);
    QBrush(Qt::GlobalColor color, Qt::BrushStyle bs=Qt::SolidPattern);

    QBrush(const QColor &color, const QPixmap &pixmap);
    QBrush(Qt::GlobalColor color, const QPixmap &pixmap);
    QBrush(const QPixmap &pixmap);
    QBrush(const QImage &image);

    QBrush(const QBrush &brush);

    QBrush(const QGradient &gradient);

    ~QBrush();
    QBrush &operator=(const QBrush &brush);
    QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QBrush)
    inline void swap(QBrush &other) noexcept
    { d.swap(other.d); }

    QBrush &operator=(Qt::BrushStyle style);
    QBrush &operator=(QColor color);
    QBrush &operator=(Qt::GlobalColor color) { return operator=(QColor(color)); }

    operator QVariant() const;

    inline Qt::BrushStyle style() const;
    void setStyle(Qt::BrushStyle);

    inline QTransform transform() const;
    void setTransform(const QTransform &);

    QPixmap texture() const;
    void setTexture(const QPixmap &pixmap);

    QImage textureImage() const;
    void setTextureImage(const QImage &image);

    inline const QColor &color() const;
    void setColor(const QColor &color);
    inline void setColor(Qt::GlobalColor color);

    const QGradient *gradient() const;

    bool isOpaque() const;

    bool operator==(const QBrush &b) const;
    inline bool operator!=(const QBrush &b) const { return !(operator==(b)); }

    using DataPtr = std::unique_ptr<QBrushData, QBrushDataPointerDeleter>;

private:
    friend class QRasterPaintEngine;
    friend class QRasterPaintEnginePrivate;
    friend struct QSpanData;
    friend class QPainter;
    friend bool Q_GUI_EXPORT qHasPixmapTexture(const QBrush& brush);

    bool doCompareEqualColor(QColor rhs) const noexcept;
    friend bool comparesEqual(const QBrush &lhs, QColor rhs) noexcept
    {
        return lhs.doCompareEqualColor(rhs);
    }
    Q_DECLARE_EQUALITY_COMPARABLE(QBrush, QColor)
    Q_DECLARE_EQUALITY_COMPARABLE(QBrush, Qt::GlobalColor)

    bool doCompareEqualStyle(Qt::BrushStyle rhs) const noexcept;
    friend bool comparesEqual(const QBrush &lhs, Qt::BrushStyle rhs) noexcept
    {
        return lhs.doCompareEqualStyle(rhs);
    }
    Q_DECLARE_EQUALITY_COMPARABLE(QBrush, Qt::BrushStyle)

    void detach(Qt::BrushStyle newStyle);
    void init(const QColor &color, Qt::BrushStyle bs);
    DataPtr d;

public:
    inline bool isDetached() const;
    inline DataPtr &data_ptr() { return d; }
};

inline void QBrush::setColor(Qt::GlobalColor acolor)
{ setColor(QColor(acolor)); }

Q_DECLARE_SHARED(QBrush)

/*****************************************************************************
  QBrush stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QBrush &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QBrush &);
#endif

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QBrush &);
#endif

struct QBrushData
{
    QAtomicInt ref;
    Qt::BrushStyle style;
    QColor color;
    QTransform transform;
};

inline Qt::BrushStyle QBrush::style() const { return d->style; }
inline const QColor &QBrush::color() const { return d->color; }
inline QTransform QBrush::transform() const { return d->transform; }
inline bool QBrush::isDetached() const { return d->ref.loadRelaxed() == 1; }


/*******************************************************************************
 * QGradients
 */
class QGradientPrivate;

typedef std::pair<qreal, QColor> QGradientStop;
typedef QList<QGradientStop> QGradientStops;

class Q_GUI_EXPORT QGradient
{
    Q_GADGET
public:
    enum Type {
        LinearGradient,
        RadialGradient,
        ConicalGradient,
        NoGradient
    };
    Q_ENUM(Type)

    enum Spread {
        PadSpread,
        ReflectSpread,
        RepeatSpread
    };
    Q_ENUM(Spread)

    enum CoordinateMode {
        LogicalMode,
        StretchToDeviceMode,
        ObjectBoundingMode,
        ObjectMode
    };
    Q_ENUM(CoordinateMode)

    enum InterpolationMode {
        ColorInterpolation,
        ComponentInterpolation
    };

    enum Preset {
        WarmFlame = 1,
        NightFade = 2,
        SpringWarmth = 3,
        JuicyPeach = 4,
        YoungPassion = 5,
        LadyLips = 6,
        SunnyMorning = 7,
        RainyAshville = 8,
        FrozenDreams = 9,
        WinterNeva = 10,
        DustyGrass = 11,
        TemptingAzure = 12,
        HeavyRain = 13,
        AmyCrisp = 14,
        MeanFruit = 15,
        DeepBlue = 16,
        RipeMalinka = 17,
        CloudyKnoxville = 18,
        MalibuBeach = 19,
        NewLife = 20,
        TrueSunset = 21,
        MorpheusDen = 22,
        RareWind = 23,
        NearMoon = 24,
        WildApple = 25,
        SaintPetersburg = 26,
        PlumPlate = 28,
        EverlastingSky = 29,
        HappyFisher = 30,
        Blessing = 31,
        SharpeyeEagle = 32,
        LadogaBottom = 33,
        LemonGate = 34,
        ItmeoBranding = 35,
        ZeusMiracle = 36,
        OldHat = 37,
        StarWine = 38,
        HappyAcid = 41,
        AwesomePine = 42,
        NewYork = 43,
        ShyRainbow = 44,
        MixedHopes = 46,
        FlyHigh = 47,
        StrongBliss = 48,
        FreshMilk = 49,
        SnowAgain = 50,
        FebruaryInk = 51,
        KindSteel = 52,
        SoftGrass = 53,
        GrownEarly = 54,
        SharpBlues = 55,
        ShadyWater = 56,
        DirtyBeauty = 57,
        GreatWhale = 58,
        TeenNotebook = 59,
        PoliteRumors = 60,
        SweetPeriod = 61,
        WideMatrix = 62,
        SoftCherish = 63,
        RedSalvation = 64,
        BurningSpring = 65,
        NightParty = 66,
        SkyGlider = 67,
        HeavenPeach = 68,
        PurpleDivision = 69,
        AquaSplash = 70,
        SpikyNaga = 72,
        LoveKiss = 73,
        CleanMirror = 75,
        PremiumDark = 76,
        ColdEvening = 77,
        CochitiLake = 78,
        SummerGames = 79,
        PassionateBed = 80,
        MountainRock = 81,
        DesertHump = 82,
        JungleDay = 83,
        PhoenixStart = 84,
        OctoberSilence = 85,
        FarawayRiver = 86,
        AlchemistLab = 87,
        OverSun = 88,
        PremiumWhite = 89,
        MarsParty = 90,
        EternalConstance = 91,
        JapanBlush = 92,
        SmilingRain = 93,
        CloudyApple = 94,
        BigMango = 95,
        HealthyWater = 96,
        AmourAmour = 97,
        RiskyConcrete = 98,
        StrongStick = 99,
        ViciousStance = 100,
        PaloAlto = 101,
        HappyMemories = 102,
        MidnightBloom = 103,
        Crystalline = 104,
        PartyBliss = 106,
        ConfidentCloud = 107,
        LeCocktail = 108,
        RiverCity = 109,
        FrozenBerry = 110,
        ChildCare = 112,
        FlyingLemon = 113,
        NewRetrowave = 114,
        HiddenJaguar = 115,
        AboveTheSky = 116,
        Nega = 117,
        DenseWater = 118,
        Seashore = 120,
        MarbleWall = 121,
        CheerfulCaramel = 122,
        NightSky = 123,
        MagicLake = 124,
        YoungGrass = 125,
        ColorfulPeach = 126,
        GentleCare = 127,
        PlumBath = 128,
        HappyUnicorn = 129,
        AfricanField = 131,
        SolidStone = 132,
        OrangeJuice = 133,
        GlassWater = 134,
        NorthMiracle = 136,
        FruitBlend = 137,
        MillenniumPine = 138,
        HighFlight = 139,
        MoleHall = 140,
        SpaceShift = 142,
        ForestInei = 143,
        RoyalGarden = 144,
        RichMetal = 145,
        JuicyCake = 146,
        SmartIndigo = 147,
        SandStrike = 148,
        NorseBeauty = 149,
        AquaGuidance = 150,
        SunVeggie = 151,
        SeaLord = 152,
        BlackSea = 153,
        GrassShampoo = 154,
        LandingAircraft = 155,
        WitchDance = 156,
        SleeplessNight = 157,
        AngelCare = 158,
        CrystalRiver = 159,
        SoftLipstick = 160,
        SaltMountain = 161,
        PerfectWhite = 162,
        FreshOasis = 163,
        StrictNovember = 164,
        MorningSalad = 165,
        DeepRelief = 166,
        SeaStrike = 167,
        NightCall = 168,
        SupremeSky = 169,
        LightBlue = 170,
        MindCrawl = 171,
        LilyMeadow = 172,
        SugarLollipop = 173,
        SweetDessert = 174,
        MagicRay = 175,
        TeenParty = 176,
        FrozenHeat = 177,
        GagarinView = 178,
        FabledSunset = 179,
        PerfectBlue = 180,

        NumPresets
    };
    Q_ENUM(Preset)

    QGradient();
    QGradient(Preset);
    ~QGradient();

    Type type() const { return m_type; }

    inline void setSpread(Spread spread);
    Spread spread() const { return m_spread; }

    void setColorAt(qreal pos, const QColor &color);

    void setStops(const QGradientStops &stops);
    QGradientStops stops() const;

    CoordinateMode coordinateMode() const;
    void setCoordinateMode(CoordinateMode mode);

    InterpolationMode interpolationMode() const;
    void setInterpolationMode(InterpolationMode mode);

    bool operator==(const QGradient &gradient) const;
    inline bool operator!=(const QGradient &other) const
    { return !operator==(other); }

    union QGradientData {
        struct {
            qreal x1, y1, x2, y2;
        } linear;
        struct {
            qreal cx, cy, fx, fy, cradius, fradius;
        } radial;
        struct {
            qreal cx, cy, angle;
        } conical;
    };

private:
    friend class QLinearGradient;
    friend class QRadialGradient;
    friend class QConicalGradient;
    friend class QBrush;

    Type m_type = NoGradient;
    Spread m_spread = PadSpread;
    QGradientStops m_stops;
    QGradientData m_data;
    CoordinateMode m_coordinateMode = LogicalMode;
    InterpolationMode m_interpolationMode = ColorInterpolation;
};

inline void QGradient::setSpread(Spread aspread)
{ m_spread = aspread; }

class Q_GUI_EXPORT QLinearGradient : public QGradient
{
public:
    QLinearGradient();
    QLinearGradient(const QPointF &start, const QPointF &finalStop);
    QLinearGradient(qreal xStart, qreal yStart, qreal xFinalStop, qreal yFinalStop);
    ~QLinearGradient();

    QPointF start() const;
    void setStart(const QPointF &start);
    inline void setStart(qreal x, qreal y) { setStart(QPointF(x, y)); }

    QPointF finalStop() const;
    void setFinalStop(const QPointF &stop);
    inline void setFinalStop(qreal x, qreal y) { setFinalStop(QPointF(x, y)); }
};


class Q_GUI_EXPORT QRadialGradient : public QGradient
{
public:
    QRadialGradient();
    QRadialGradient(const QPointF &center, qreal radius, const QPointF &focalPoint);
    QRadialGradient(qreal cx, qreal cy, qreal radius, qreal fx, qreal fy);

    QRadialGradient(const QPointF &center, qreal radius);
    QRadialGradient(qreal cx, qreal cy, qreal radius);

    QRadialGradient(const QPointF &center, qreal centerRadius, const QPointF &focalPoint, qreal focalRadius);
    QRadialGradient(qreal cx, qreal cy, qreal centerRadius, qreal fx, qreal fy, qreal focalRadius);

    ~QRadialGradient();

    QPointF center() const;
    void setCenter(const QPointF &center);
    inline void setCenter(qreal x, qreal y) { setCenter(QPointF(x, y)); }

    QPointF focalPoint() const;
    void setFocalPoint(const QPointF &focalPoint);
    inline void setFocalPoint(qreal x, qreal y) { setFocalPoint(QPointF(x, y)); }

    qreal radius() const;
    void setRadius(qreal radius);

    qreal centerRadius() const;
    void setCenterRadius(qreal radius);

    qreal focalRadius() const;
    void setFocalRadius(qreal radius);
};


class Q_GUI_EXPORT QConicalGradient : public QGradient
{
public:
    QConicalGradient();
    QConicalGradient(const QPointF &center, qreal startAngle);
    QConicalGradient(qreal cx, qreal cy, qreal startAngle);
    ~QConicalGradient();

    QPointF center() const;
    void setCenter(const QPointF &center);
    inline void setCenter(qreal x, qreal y) { setCenter(QPointF(x, y)); }

    qreal angle() const;
    void setAngle(qreal angle);
};

QT_END_NAMESPACE

#endif // QBRUSH_H
