#include "qtdcreatesecretchatrequest.h"

QTdCreateSecretChatRequest::QTdCreateSecretChatRequest(QObject *parent) : QTdRequest(parent),
    m_userId(0)
{
}

void QTdCreateSecretChatRequest::setUserId(const qint32 &id)
{
    m_userId = id;
}

QJsonObject QTdCreateSecretChatRequest::marshalJson()
{
    return QJsonObject{
        {"@type", "createSecretChat"},
        {"user_id", m_userId},
    };
}
