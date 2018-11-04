#include "qtdproxy.h"
#include <td/telegram/td_json_client.h>
#include <td/telegram/td_log.h>
#include <QString>
#include <QDebug>

/**
 * @brief Proxy class, used to access tdlib directly by calling its c functions.
 *
 */
class QTdProxyDirect final: public QTdProxy
{
public:
    QTdProxyDirect() {
        m_tdlibHandle = td_json_client_create();
        td_set_log_verbosity_level(2);
    }

    void send(const char *request) const override {
        td_json_client_send(m_tdlibHandle, request);
    }

    const char * receive(double timeout) const override {
        return td_json_client_receive(m_tdlibHandle, timeout);
    }

    const char * execute(const char *request) const override {
        return td_json_client_execute(m_tdlibHandle, request);
    }

    ~QTdProxyDirect() {
        qWarning() << "Destroying \"direct\" proxy";
        td_json_client_destroy(m_tdlibHandle);
    }
    
private:
    void *m_tdlibHandle;

};

class QTdProxyDirectProvider: public QTdProxyProvider
{
public:
    QTdProxyDirectProvider() : QTdProxyProvider("direct") {}
    virtual ~QTdProxyDirectProvider() {}
protected:
    QTdProxy * createProxy() const override{
        return new QTdProxyDirect();
    }
};

static QTdProxyDirectProvider direct;

