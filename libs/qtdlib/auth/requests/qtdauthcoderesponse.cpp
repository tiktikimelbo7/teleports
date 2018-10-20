#include "qtdauthcoderesponse.h"

QTdAuthCodeResponse::QTdAuthCodeResponse(QObject *parent) : QTdRequest(parent)
{

}

void QTdAuthCodeResponse::setCode(const QString &code)
{
    m_code = code;
}

void QTdAuthCodeResponse::setFirstName(const QString &firstname)
{
    m_firstname = firstname;
}

void QTdAuthCodeResponse::setLastName(const QString &lastname)
{
    m_lastname = lastname;
}

QJsonObject QTdAuthCodeResponse::marshalJson()
{
    return QJsonObject{
        {"@type", "checkAuthenticationCode"},
        {"code", m_code},
        {"first_name", m_firstname},
        {"last_name", m_lastname},
    };
}
