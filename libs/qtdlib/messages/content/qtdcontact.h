#ifndef QTDCONTACT_H
#define QTDCONTACT_H

#include <QObject>
#include <QPointer>
#include "common/qabstracttdobject.h"
#include "user/qtduser.h"

class QTdContact: public QTdObject {
    Q_OBJECT
    Q_PROPERTY(QString phone_number READ phone_number NOTIFY dataChanged)
    Q_PROPERTY(QString first_name READ first_name NOTIFY dataChanged)
    Q_PROPERTY(QString last_name READ last_name NOTIFY dataChanged)
    Q_PROPERTY(QString vcard READ vcard NOTIFY dataChanged)
    Q_PROPERTY(QString user_id READ qmlUserId NOTIFY dataChanged)
    Q_PROPERTY(QTdUser *user READ user NOTIFY userChanged)

public:
    explicit QTdContact(QObject *parent = nullptr);

    QString phone_number() const;
    QString first_name() const;
    QString last_name() const;
    QString vcard() const;
    QString qmlUserId() const;
    qint32 user_id() const;
    QTdUser *user() const;

    void set_phone_number(QString value);
    void set_first_name(QString value);
    void set_last_name(QString value);
    void set_vcard(QString value);
    void set_user_id(QString value);

    void unmarshalJson(const QJsonObject &json) Q_DECL_FINAL;
    //QJsonObject marshalJson();

signals:
    void dataChanged();
    void userChanged();

private slots:
    void isUserAvailable(const qint32 &userId);

private:
    Q_DISABLE_COPY(QTdContact)
    QString m_phone_number;
    QString m_first_name;
    QString m_last_name;
    QString m_vcard;
    QTdInt32 m_user_id;
    bool m_waitingForUser;
};

#endif // QTDCONTACT_H
