#include "qtdauthpasswordresponse.h"

QTdAuthPasswordResponse::QTdAuthPasswordResponse(QObject *parent) : QTdRequest(parent)
{

}

void QTdAuthPasswordResponse::setPassword(const QString &password)
{
    m_password = password;
}

QJsonObject QTdAuthPasswordResponse::marshalJson()
{
    return QJsonObject{
        {"@type", "checkAuthenticationPassword"},
        {"password", m_password},
    };
}
