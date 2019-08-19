#include "qtdregisterdevicerequest.h"

QTdRegisterDeviceRequest::QTdRegisterDeviceRequest(QObject *parent)
    : QTdOkRequest(parent)
{
}

void QTdRegisterDeviceRequest::setToken(const QString &token)
{
    m_token = token;
}

QJsonObject QTdRegisterDeviceRequest::marshalJson()
{
    return QJsonObject{
        { "@type", "registerDevice" },
        { "device_token", QJsonObject{
                                  { "@type", "deviceTokenUbuntuPush" },
                                  { "token", m_token } } }
    };
}
