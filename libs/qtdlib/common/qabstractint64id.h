#ifndef QABSTRACTINT64ID_H
#define QABSTRACTINT64ID_H

#include <QObject>
#include "qabstracttdobject.h"
#include "qtdint.h"

class QAbstractInt64Id : public QTdObject
{
    Q_OBJECT
    // We expose all ID's as strings to qml to make sure
    // all id bit lengths are passable. tdlib returns either
    // 32, 53 and 64 bit id's
    Q_PROPERTY(QString id READ qmlId NOTIFY idChanged)
public:
    explicit QAbstractInt64Id(QObject *parent = nullptr);

    qint64 id() const;
    QString qmlId() const;
    QJsonValue jsonId() const;

    // QAbstractTdObject interface
    virtual void unmarshalJson(const QJsonObject &json);

    static QAbstractInt64Id *create(const QJsonObject &json, QObject *parent);

signals:
    void idChanged(int id);

private:
    Q_DISABLE_COPY(QAbstractInt64Id)
    QTdInt64 m_id;
};

#endif // QABSTRACTINT64ID_H
