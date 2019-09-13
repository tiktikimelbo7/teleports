#ifndef QTDGETBASICGROUPREQUEST_H
#define QTDGETBASICGROUPREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"

/**
 * @brief The QTdGetBasicGroupRequest class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1get_basic_group.html
 */
class QTdGetBasicGroupRequest : public QTdRequest
{
    Q_OBJECT
public:
    explicit QTdGetBasicGroupRequest(QObject *parent = nullptr);

    void setGroupId(const qint32 groupId);

    QJsonObject marshalJson() Q_DECL_FINAL;

    QFuture<QTdResponse> sendAsync() Q_DECL_FINAL;

private:
    Q_DISABLE_COPY(QTdGetBasicGroupRequest)
    qint32 m_groupId;
};

#endif // QTDGETBASICGROUPREQUEST_H
