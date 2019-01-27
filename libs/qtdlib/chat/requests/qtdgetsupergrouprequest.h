#ifndef QTDGETSUPERGROUPREQUEST_H
#define QTDGETSUPERGROUPREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"

/**
 * @brief The QTdGetSuperGroupRequest class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1get_supergroup.html
 */
class QTdGetSuperGroupRequest : public QTdRequest
{
    Q_OBJECT
    qint32 m_sgId;
public:
    explicit QTdGetSuperGroupRequest(QObject *parent = nullptr);
    void setSuperGroupId(const qint32 &id);
    QJsonObject marshalJson() Q_DECL_FINAL;
    QFuture<QTdResponse> sendAsync() Q_DECL_FINAL;
};

#endif // QTDGETSUPERGROUPREQUEST_H
