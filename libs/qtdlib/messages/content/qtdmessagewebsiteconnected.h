#ifndef QTDMESSAGEWEBSITECONNECTED_H
#define QTDMESSAGEWEBSITECONNECTED_H

#include <QObject>
#include <QScopedPointer>
#include "messages/qtdmessagecontent.h"

class QTdMessageWebsiteConnected : public QTdMessageContent
{
    Q_OBJECT
public:
    explicit QTdMessageWebsiteConnected(QObject *parent = nullptr);
    QString domainName() const;
    void unmarshalJson(const QJsonObject &json);

private:
    Q_DISABLE_COPY(QTdMessageWebsiteConnected)
    Q_PROPERTY(QString domainName READ domainName NOTIFY domainNameChanged)

signals:
    void domainNameChanged();

private:
    QString m_domain_name;
};

#endif // QTDMESSAGEWEBSITECONNECTED_H
