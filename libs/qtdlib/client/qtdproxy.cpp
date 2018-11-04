#include "qtdproxy.h"
#include <QDebug>

QTdProxyProvider::QTdProxyProvider(QString type): m_type(type) {
    if (!s_registry) {
        s_registry = new QVector<QPointer<QTdProxyProvider>>();
        qDebug() << "QTdProxyProvider: creating registry";
    }

    qDebug() << "QTdProxyProvider: push_back proxy provider for type " << type;
    s_registry->push_back(QPointer<QTdProxyProvider>(this));
}

QTdProxyProvider::~QTdProxyProvider() {
    for (int i=0; i < s_registry->size(); i++) {
        if (s_registry->at(i)->m_type == m_type) {
            qDebug() << "QTdProxyProvider: removing proxy provider for type " << m_type;
            s_registry->remove(i);
            break;
        }
    }

    if (s_registry->isEmpty()) {
        delete s_registry;
    }
}

QVector<QPointer<QTdProxyProvider>> * QTdProxyProvider::s_registry = nullptr;

QSharedPointer<QTdProxy> QTdProxyProvider::instance(QString proxyType) {
    if (!s_registry) {
        qDebug() << "QTdProxyProvider: Error: QTdProxyProvider::s_registry == NULL";
        return nullptr;
    }

    for (int i=0; i < s_registry->size(); i++) {
        QPointer<QTdProxyProvider> provider = s_registry->at(i);
        if (provider->m_type == proxyType) {
            return provider->instance();
        }
    }

    qDebug() << "QTdProxyProvider: Error: No provider found for proxy type: " << proxyType;
    return nullptr;
}

QSharedPointer<QTdProxy> QTdProxyProvider::instance() {
    QSharedPointer<QTdProxy> proxy = m_proxy.toStrongRef();
    if (proxy.isNull()) {
        qDebug() << "QTdProxyProvider: creating shared pointer for proxy of type " << m_type;
        proxy = QSharedPointer<QTdProxy>(createProxy(), deleteProxy);
        m_proxy = proxy;
    }

    return m_proxy.toStrongRef();
}

void QTdProxyProvider::deleteProxy(QTdProxy * proxy) {
    delete proxy;
    proxy = Q_NULLPTR;
}


