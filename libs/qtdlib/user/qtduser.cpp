#include "qtduser.h"
#include "qtduserstatusfactory.h"
#include "common/qtdhelpers.h"
#include "client/qtdclient.h"
#include "requests/qtdgetuserfullinforequest.h"
#include "utils/await.h"

QTdUser::QTdUser(QObject *parent)
    : QAbstractInt32Id(parent)
    , m_fullInfo(new QTdUserFullInfo)
    , m_status(Q_NULLPTR)
    , m_profilePhoto(new QTdProfilePhoto)
    , m_isVerified(false)
    , m_userType(Q_NULLPTR)
{
    setType(USER);
    m_my_id = qint32(QTdClient::instance()->getOption("my_id").toInt());
}

void QTdUser::unmarshalJson(const QJsonObject &json)
{
    setFirstName(json["first_name"].toString());
    setLastName(json["last_name"].toString());
    setUsername(json["username"].toString());
    setPhoneNumber(json["phone_number"].toString());
    setStatus(QTdUserStatusFactory::create(json["status"].toObject(), this));
    m_isVerified = json["is_verified"].toBool();
    emit isVerifiedChanged(m_isVerified);
    m_restrictionReason = json["restriction_reason"].toString();
    emit restrictionReasonChanged(m_restrictionReason);
    m_languageCode = json["language_code"].toString();
    emit languageCodeChanged(m_languageCode);

    if (m_userType) {
        delete m_userType;
        m_userType = nullptr;
    }
    const QJsonObject typeObj = json["type"].toObject();
    const QString type = typeObj["@type"].toString();
    if (type == "userTypeRegular") {
        setType(USER_TYPE_REGULAR);
        m_userType = new QTdUserTypeRegular(this);
    } else if (type == "userTypeBot") {
        m_userType = new QTdUserTypeBot(this);
    } else if (type == "userTypeDeleted") {
        m_userType = new QTdUserTypeDeleted(this);
    } else if (type == "userTypeUnknown") {
        m_userType = new QTdUserTypeUnknown(this);
    }
    if (m_userType) {
        m_userType->unmarshalJson(typeObj);
    }

    m_profilePhoto->unmarshalJson(json["profile_photo"].toObject());
    emit profilePhotoChanged(m_profilePhoto.data());
    const bool hasProfilePhoto = m_profilePhoto->small()->id() > 0;
    const bool needsDownload = m_profilePhoto->small()->local()->path().isEmpty();
    if (hasProfilePhoto && needsDownload) {
        m_profilePhoto->small()->downloadFile();
    }

    QAbstractInt32Id::unmarshalJson(json);
}

QTdUserType *QTdUser::userType() const
{
    return m_userType;
}

QString QTdUser::restrictionReason() const
{
    return m_restrictionReason;
}

QString QTdUser::languageCode() const
{
    return m_languageCode;
}

bool QTdUser::isVerified() const
{
    return m_isVerified;
}

bool QTdUser::isMyself()
{
    if (m_my_id == 0) {
        m_my_id = qint32(QTdClient::instance()->getOption("my_id").toInt());
    }
    return id() == m_my_id;
}

QTdProfilePhoto *QTdUser::profilePhoto() const
{
    return m_profilePhoto.data();
}

QTdUserFullInfo *QTdUser::fullInfo()
{
    return m_fullInfo.data();
}

QTdUserStatus *QTdUser::status() const
{
    return m_status;
}

void QTdUser::setFirstName(QString firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit nameChanged(m_firstName);
}

void QTdUser::setLastName(QString lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit nameChanged(m_lastName);
}

void QTdUser::setUsername(QString username)
{
    if (m_username == username)
        return;

    m_username = username;
    emit nameChanged(m_username);
}

void QTdUser::setPhoneNumber(QString phoneNumber)
{
    if (m_phoneNumber == phoneNumber)
        return;

    m_phoneNumber = phoneNumber;
    emit phoneNumberChanged(m_phoneNumber);
}

void QTdUser::setFullInfo(QTdUserFullInfo *fullInfo)
{
    m_fullInfo.reset(fullInfo);
    emit fullInfoChanged(m_fullInfo.data());
}

void QTdUser::setStatus(QTdUserStatus *status)
{
    QString oldStatusString = "";

    if (m_status) {
        oldStatusString = m_status->toString();
        delete m_status;
        m_status = nullptr;
    }

    m_status = status;
    emit statusChanged(m_status);

    if (m_status->toString() != oldStatusString) {
        emit m_status->statusStringChanged(m_status->toString());
    }
}

QString QTdUser::firstName() const
{
    return m_firstName;
}

QString QTdUser::lastName() const
{
    return m_lastName;
}

QString QTdUser::username() const
{
    return m_username;
}

QString QTdUser::fullName() const
{
    if (m_firstName != "") {
        auto fullName = m_firstName;
        if (m_lastName != "")
            fullName = fullName + " " + m_lastName;
        return fullName;
    } else {
        return m_username;
    }
}

QString QTdUser::phoneNumber() const
{
    return m_phoneNumber;
}

QString QTdUser::initials() const
{
    if (m_firstName != "") {
        if (m_lastName != "") {
            return (m_firstName.trimmed().left(1) + m_lastName.trimmed().left(1)).toUpper();
        }
        return m_firstName.trimmed().left(2).toUpper();
    }
    if (m_lastName != "") {
        return m_lastName.trimmed().left(2).toUpper();
    }
    if (m_username != "") {
        return m_username.trimmed().left(2).toUpper();
    }
    return "N/A";
}

QString QTdUser::avatarColor(unsigned int userId)
{
    return QTdHelpers::avatarColor(userId);
}
