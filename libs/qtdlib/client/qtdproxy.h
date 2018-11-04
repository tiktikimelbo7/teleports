#ifndef QTDPROXY_H
#define QTDPROXY_H

#include <QObject>
#include <QPointer>
#include <QWeakPointer>
#include <QSharedPointer>
#include <QVector>
#include <QString>

/**
 * @brief Proxy class, abstracting access to tdlib
 *
 * Abstracts the access to the td_json_client 
 *
 * handle so it can
 * be stored in a QSharedPointer allowing sharing
 * of the instance across threads
 */
class QTdProxy: public QObject
{
public:
    virtual void send(const char *request) const = 0;
    virtual const char * receive(double timeout) const = 0;
    virtual const char * execute(const char *request) const = 0;
    virtual ~QTdProxy() {}
};

class QTdProxyProvider: public QObject
{
public:
    QTdProxyProvider(QString type);
    QSharedPointer<QTdProxy> instance();
    virtual ~QTdProxyProvider();

    static void deleteProxy(QTdProxy * proxy);
    static QSharedPointer<QTdProxy> instance(QString proxyType);

protected:
    virtual QTdProxy * createProxy() const = 0;

private:
    QString m_type;
    QWeakPointer<QTdProxy> m_proxy;
    static QVector<QPointer<QTdProxyProvider>> *s_registry;
};

#endif // QTDPROXY_H
