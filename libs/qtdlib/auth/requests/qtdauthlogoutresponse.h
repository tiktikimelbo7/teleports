#ifndef QTDAUTHLOGOUTRESPONSE_H
#define QTDAUTHLOGOUTRESPONSE_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdAuthLogOutResponse : public QTdRequest
{
    Q_OBJECT
public:
    explicit QTdAuthLogOutResponse(QObject *parent = nullptr);

    QJsonObject marshalJson();
private:
    QString m_number;
};

#endif // QTDAUTHLOGOUTRESPONSE_H
