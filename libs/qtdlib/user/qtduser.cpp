#include "qtduser.h"
#include "qtduserstatusfactory.h"
#include "qtdlinkstatefactory.h"
#include "common/qtdhelpers.h"
#include "client/qtdclient.h"

QTdUser::QTdUser(QObject *parent)
    : QAbstractInt32Id(parent)
    , m_status(Q_NULLPTR)
    , m_profilePhoto(new QTdProfilePhoto)
    , m_outgoingLink(Q_NULLPTR)
    , m_incomingLink(Q_NULLPTR)
    , m_isVerified(false)
    , m_userType(Q_NULLPTR)
{
    setType(USER);
    m_my_id = QTdClient::instance()->getOption("my_id").toInt();
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

    if (m_outgoingLink) {
        delete m_outgoingLink;
        m_outgoingLink = nullptr;
    }
    m_outgoingLink = QTdLinkStateFactory::create(json["outgoing_link"].toObject(), this);
    emit outgoingLinkChanged(m_outgoingLink);

    if (m_incomingLink) {
        delete m_incomingLink;
        m_incomingLink = nullptr;
    }
    m_incomingLink = QTdLinkStateFactory::create(json["incoming_link"].toObject(), this);
    emit incomingLinkChanged(m_incomingLink);

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

bool QTdUser::isMyself() const
{
    return id() == m_my_id;
}

QTdLinkState *QTdUser::outgoingLink() const
{
    return m_outgoingLink;
}

QTdLinkState *QTdUser::incomingLink() const
{
    return m_incomingLink;
}

QTdProfilePhoto *QTdUser::profilePhoto() const
{
    return m_profilePhoto.data();
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
    emit firstNameChanged(m_firstName);
}

void QTdUser::setLastName(QString lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged(m_lastName);
}

void QTdUser::setUsername(QString username)
{
    if (m_username == username)
        return;

    m_username = username;
    emit usernameChanged(m_username);
}

void QTdUser::setPhoneNumber(QString phoneNumber)
{
    if (m_phoneNumber == phoneNumber)
        return;

    m_phoneNumber = phoneNumber;
    emit phoneNumberChanged(m_phoneNumber);
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
