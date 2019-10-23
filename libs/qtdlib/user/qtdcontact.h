#ifndef QTDCONTACT_H
#define QTDCONTACT_H

#include <QObject>
#include <QPointer>
#include "common/qabstracttdobject.h"

class QTdContact: public QTdObject {
    Q_OBJECT
    Q_PROPERTY(QString phone_number READ phone_number WRITE set_phone_number NOTIFY dataChanged)
    Q_PROPERTY(QString first_name READ first_name WRITE set_first_name NOTIFY dataChanged)
    Q_PROPERTY(QString last_name READ last_name WRITE set_last_name NOTIFY dataChanged)
    Q_PROPERTY(QString vcard READ vcard WRITE set_vcard NOTIFY dataChanged)
    Q_PROPERTY(qint32 user_id READ user_id WRITE set_user_id NOTIFY dataChanged)

public:
    explicit QTdContact(QObject *parent = nullptr);

    QString phone_number() const;
    QString first_name() const;
    QString last_name() const;
    QString vcard() const;
    qint32 user_id() const;

    void set_phone_number(QString value);
    void set_first_name(QString value);
    void set_last_name(QString value);
    void set_vcard(QString value);
    void set_user_id(qint32 value);

    void unmarshalJson(const QJsonObject &json) Q_DECL_FINAL;
    QJsonObject marshalJson();

signals:
    void dataChanged();

private:
    Q_DISABLE_COPY(QTdContact)
    QString m_phone_number;
    QString m_first_name;
    QString m_last_name;
    QString m_vcard;
    qint32 m_user_id;
};

#endif // QTDCONTACT_H
