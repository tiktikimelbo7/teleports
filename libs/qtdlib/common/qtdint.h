#ifndef QTDINT_H
#define QTDINT_H

#include <QJsonValue>
#include <QDebug>

class QTdInt32
{
    qint32 _value;
public:
    QTdInt32() : _value(0) {}
    QTdInt32(const qint32 &v) : _value(v) {}
    QJsonValue toJsonValue() const;
    QString toQmlValue() const;
    qint32 value() const;

    bool isValid() const {
        return _value > 0;
    }

    void operator=(const qint32 &v) {
        _value = v;
    }

    void operator=(const QJsonValue &v) {
        _value = qint32(v.toInt());
    }

    void operator=(const QString &v) {
        _value = qint32(v.toInt());
    }
};

class QTdInt53
{
    qint64 _value;
public:
    QTdInt53() : _value(0) {}
    QTdInt53(const qint64 &v) : _value(v) {}
    QJsonValue toJsonValue() const;
    QString toQmlValue() const;
    qint64 value() const;
    bool isValid() const {
        return _value > 0;
    }

    void operator=(const qint64 &v) {
        _value = v;
    }

    void operator=(const QJsonValue &v) {
        _value = qint64(v.toDouble());
    }

    void operator=(const QString &v) {
        _value = qint64(v.toLongLong());
    }
};

class QTdInt64
{
    qint64 _value;
public:
    QTdInt64() : _value(0) {}
    QTdInt64(const qint64 &v) : _value(v) {}
    QJsonValue toJsonValue() const;
    QString toQmlValue() const;
    qint64 value() const;
    bool isValid() const {
        return _value > 0;
    }

    void operator=(const qint64 &v) {
        _value = v;
    }

    void operator=(const QJsonValue &v) {
        QVariant res = v.toVariant();
        bool ok = false;
        _value = res.toLongLong(&ok);
        if (!ok) {
            qWarning() << "Failed parsing qint64 from json value";
        }
    }

    void operator=(const QString &v) {
        _value = qint64(v.toLongLong());
    }
};

#endif // QTDINT_H
