#include "qtdauthlogoutresponse.h"

QTdAuthLogOutResponse::QTdAuthLogOutResponse(QObject *parent) : QTdRequest(parent)
{
}

QJsonObject QTdAuthLogOutResponse::marshalJson()
{
    return QJsonObject {
        {"@type", "logOut"},
    };
}
