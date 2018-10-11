#ifndef QTDUSER_H
#define QTDUSER_H

#include <QObject>
#include <QPointer>
#include <QScopedPointer>
#include <QList>
#include "common/qabstractint32id.h"
#include "qtduserstatus.h"
#include "qtdprofilephoto.h"
#include "qtdlinkstate.h"
#include "qtdusertype.h"


/**
 * @brief The QTdUser class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1user.html
 */
class QTdUser : public QAbstractInt32Id
{
    Q_OBJECT
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(QTdUserStatus* status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QTdProfilePhoto* profilePhoto READ profilePhoto NOTIFY profilePhotoChanged)
    Q_PROPERTY(QTdLinkState* outgoingLink READ outgoingLink NOTIFY outgoingLinkChanged)
    Q_PROPERTY(QTdLinkState* incomingLink READ incomingLink NOTIFY incomingLinkChanged)
    Q_PROPERTY(bool isVerified READ isVerified NOTIFY isVerifiedChanged)
    Q_PROPERTY(QString restrictionReason READ restrictionReason NOTIFY restrictionReasonChanged)
    Q_PROPERTY(QTdUserType* userType READ userType NOTIFY userTypeChanged)
    Q_PROPERTY(QString languageCode READ languageCode NOTIFY languageCodeChanged)
public:
    explicit QTdUser(QObject *parent = nullptr);

    QString firstName() const;
    QString lastName() const;
    QString username() const;
    QString phoneNumber() const;
    QTdUserStatus* status() const;
    QTdProfilePhoto* profilePhoto() const;
    QTdLinkState* outgoingLink() const;
    QTdLinkState* incomingLink() const;
    bool isVerified() const;
    QString restrictionReason() const;
    QString languageCode() const;
    QTdUserType *userType() const;

    void unmarshalJson(const QJsonObject &json) Q_DECL_FINAL;

signals:
    void firstNameChanged(QString firstName);
    void lastNameChanged(QString lastName);
    void usernameChanged(QString username);
    void phoneNumberChanged(QString phoneNumber);
    void statusChanged(QTdUserStatus* status);
    void profilePhotoChanged(QTdProfilePhoto* profilePhoto);
    void outgoingLinkChanged(QTdLinkState* outgoingLink);
    void incomingLinkChanged(QTdLinkState* incomingLink);
    void isVerifiedChanged(bool isVerified);
    void restrictionReasonChanged(QString restrictionReason);
    void languageCodeChanged(QString languageCode);

    void userTypeChanged(QTdUserType* userType);

public slots:
    void setFirstName(QString firstName);
    void setLastName(QString lastName);
    void setUsername(QString username);
    void setPhoneNumber(QString phoneNumber);
    void setStatus(QTdUserStatus* status);

private:
    Q_DISABLE_COPY(QTdUser)
    QString m_firstName;
    QString m_lastName;
    QString m_username;
    QString m_phoneNumber;
    QPointer<QTdUserStatus> m_status;
    QScopedPointer<QTdProfilePhoto> m_profilePhoto;
    QPointer<QTdLinkState> m_outgoingLink;
    QPointer<QTdLinkState> m_incomingLink;
    bool m_isVerified;
    QString m_restrictionReason;
    QString m_languageCode;
    QPointer<QTdUserType> m_userType;
};

#endif // QTDUSER_H
