#ifndef QTDUSER_H
#define QTDUSER_H

#include <QObject>
#include <QPointer>
#include <QScopedPointer>
#include <QList>
#include "common/qabstractint32id.h"
#include "qtduserfullinfo.h"
#include "qtduserstatus.h"
#include "qtdprofilephoto.h"
#include "qtdusertype.h"

/**
 * @brief The QTdUser class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1user.html
 */
class QTdUser : public QAbstractInt32Id
{
    Q_OBJECT
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY nameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY nameChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY nameChanged)
    Q_PROPERTY(QString fullName READ fullName NOTIFY nameChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(QString initials READ initials NOTIFY initialsChanged)
    Q_PROPERTY(QTdUserFullInfo *fullInfo READ fullInfo WRITE setFullInfo NOTIFY fullInfoChanged)
    Q_PROPERTY(QTdUserStatus *status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QTdProfilePhoto *profilePhoto READ profilePhoto NOTIFY profilePhotoChanged)
    Q_PROPERTY(bool isVerified READ isVerified NOTIFY isVerifiedChanged)
    Q_PROPERTY(bool isMyself READ isMyself NOTIFY isMyselfChanged)
    Q_PROPERTY(QString restrictionReason READ restrictionReason NOTIFY restrictionReasonChanged)
    Q_PROPERTY(QTdUserType *userType READ userType NOTIFY userTypeChanged)
    Q_PROPERTY(QString languageCode READ languageCode NOTIFY languageCodeChanged)
public:
    explicit QTdUser(QObject *parent = nullptr);

    QString firstName() const;
    QString lastName() const;
    QString username() const;
    QString fullName() const;
    QString phoneNumber() const;
    QString initials() const;
    Q_INVOKABLE QString avatarColor(unsigned int userId);
    QTdUserFullInfo *fullInfo();
    QTdUserStatus *status() const;
    QTdProfilePhoto *profilePhoto() const;
    bool isVerified() const;
    bool isMyself();
    QString restrictionReason() const;
    QString languageCode() const;
    QTdUserType *userType() const;

    void unmarshalJson(const QJsonObject &json) Q_DECL_FINAL;

signals:
    void nameChanged(QString name);
    void phoneNumberChanged(QString phoneNumber);
    void initialsChanged(QString initials);
    void fullInfoChanged(QTdUserFullInfo *fullInfo);
    void statusChanged(QTdUserStatus *status);
    void profilePhotoChanged(QTdProfilePhoto *profilePhoto);
    void isVerifiedChanged(bool isVerified);
    void isMyselfChanged(bool isMyself);
    void restrictionReasonChanged(QString restrictionReason);
    void languageCodeChanged(QString languageCode);

    void userTypeChanged(QTdUserType *userType);

public slots:
    void setFirstName(QString firstName);
    void setLastName(QString lastName);
    void setUsername(QString username);
    void setPhoneNumber(QString phoneNumber);
    void setFullInfo(QTdUserFullInfo *fullInfo);
    void setStatus(QTdUserStatus *status);

private:
    Q_DISABLE_COPY(QTdUser)
    QString m_firstName;
    QString m_lastName;
    QString m_username;
    QString m_phoneNumber;
    qint32 m_my_id;
    QScopedPointer<QTdUserFullInfo> m_fullInfo;
    QPointer<QTdUserStatus> m_status;
    QScopedPointer<QTdProfilePhoto> m_profilePhoto;
    bool m_isVerified;
    QString m_restrictionReason;
    QString m_languageCode;
    QPointer<QTdUserType> m_userType;
};

#endif // QTDUSER_H
