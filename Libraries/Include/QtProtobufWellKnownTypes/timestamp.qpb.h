/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef TIMESTAMP_QPB_H
#define TIMESTAMP_QPB_H

#include "protobufwellknowntypes_exports.qpb.h"

#include <QtProtobuf/qprotobuflazymessagepointer.h>
#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qtprotobuftypes.h>

#include <QtCore/QDateTime>
#include <QtCore/qbytearray.h>
#include <QtCore/qlist.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>

namespace google::protobuf {
class Timestamp;
namespace Timestamp_QtProtobufNested {
enum class QtProtobufFieldEnum;
} // namespace Timestamp_QtProtobufNested

#ifdef QT_USE_PROTOBUF_LIST_ALIASES
using TimestampRepeated = QList<Timestamp>;
#endif // QT_USE_PROTOBUF_LIST_ALIASES

class Timestamp_QtProtobufData;
class Timestamp : public QProtobufMessage
{
    Q_PROTOBUF_OBJECT_EXPORT(QPB_PROTOBUFWELLKNOWNTYPES_EXPORT)
    Q_PROPERTY(QtProtobuf::int64 seconds READ seconds WRITE setSeconds SCRIPTABLE false)
    Q_PROPERTY(QtProtobuf::int32 nanos READ nanos WRITE setNanos SCRIPTABLE true)

public:
    using QtProtobufFieldEnum = Timestamp_QtProtobufNested::QtProtobufFieldEnum;
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT Timestamp();
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT ~Timestamp();
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT Timestamp(const Timestamp &other);
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT Timestamp &operator =(const Timestamp &other);
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT Timestamp(Timestamp &&other) noexcept;
    Timestamp &operator =(Timestamp &&other) noexcept
    {
        swap(other);
        return *this;
    }
    void swap(Timestamp &other) noexcept
    {
        QProtobufMessage::swap(other);
        dptr.swap(other.dptr);
    }
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT Q_IMPLICIT operator QVariant() const;

    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT QtProtobuf::int64 seconds() const;

    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT QtProtobuf::int32 nanos() const;
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT void setSeconds(QtProtobuf::int64 seconds);
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT void setNanos(QtProtobuf::int32 nanos);
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT static void registerTypes();
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT static Timestamp fromDateTime(const QDateTime &dateTime);
    QPB_PROTOBUFWELLKNOWNTYPES_EXPORT QDateTime toDateTime() const;

private:
    friend QPB_PROTOBUFWELLKNOWNTYPES_EXPORT bool comparesEqual(const Timestamp &lhs, const Timestamp &rhs) noexcept;
    friend bool operator==(const Timestamp &lhs, const Timestamp &rhs) noexcept
    {
        return comparesEqual(lhs, rhs);
    }
    friend bool operator!=(const Timestamp &lhs, const Timestamp &rhs) noexcept
    {
        return !comparesEqual(lhs, rhs);
    }
    QExplicitlySharedDataPointer<Timestamp_QtProtobufData> dptr;
};
namespace Timestamp_QtProtobufNested {
Q_NAMESPACE_EXPORT(QPB_PROTOBUFWELLKNOWNTYPES_EXPORT)

enum class QtProtobufFieldEnum {
    SecondsProtoFieldNumber = 1,
    NanosProtoFieldNumber = 2,
};
Q_ENUM_NS(QtProtobufFieldEnum)

} // namespace Timestamp_QtProtobufNested
} // namespace google::protobuf

#endif // TIMESTAMP_QPB_H
