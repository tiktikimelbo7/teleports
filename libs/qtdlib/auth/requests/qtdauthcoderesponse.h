#ifndef QTDAUTHCODERESPONSE_H
#define QTDAUTHCODERESPONSE_H

#include <QObject>
#include "common/qtdrequest.h"

class QTdAuthCodeResponse : public QTdRequest
{
    Q_OBJECT
public:
    explicit QTdAuthCodeResponse(QObject *parent = nullptr);

    void setCode(const QString &code);
    void setFirstName(const QString &firstname);
    void setLastName(const QString &lastname);

    QJsonObject marshalJson();

private:
    QString m_code;
    QString m_firstname;
    QString m_lastname;
};

#endif // QTDAUTHCODERESPONSE_H
