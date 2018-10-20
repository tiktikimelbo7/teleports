#ifndef QTDAUTHPASSWORDRESPONSE_H
#define QTDAUTHPASSWORDRESPONSE_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdAuthPasswordResponse : public QTdRequest
{
    Q_OBJECT
public:
    explicit QTdAuthPasswordResponse(QObject *parent = nullptr);

    void setPassword(const QString &password);

    QJsonObject marshalJson();

private:
    QString m_password;
};

#endif // QTDAUTHPASSWORDRESPONSE_H
