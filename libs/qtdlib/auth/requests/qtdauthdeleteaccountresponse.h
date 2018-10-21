#ifndef QTDAUTHDELETEACCOUNTRESPONSE_H
#define QTDAUTHDELETEACCOUNTRESPONSE_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdAuthDeleteAccountResponse : public QTdRequest
{
    Q_OBJECT
public:
    explicit QTdAuthDeleteAccountResponse(QObject *parent = nullptr);

    void setReason(const QString &reason);

    QJsonObject marshalJson();

private:
    QString m_reason;
};

#endif // QTDAUTHDELETEACCOUNTRESPONSE_H
