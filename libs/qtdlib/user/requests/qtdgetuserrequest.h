#ifndef QTDGETUSERREQUEST_H
#define QTDGETUSERREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"
#include "common/qtdint.h"

/**
 * @brief The QTdGetUserRequest class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1get_user.html
 */
class QTdGetUserRequest : public QTdRequest
{
    Q_OBJECT
public:
    explicit QTdGetUserRequest(QObject *parent = nullptr);

    void setUserId(const QString &id);
    void setUserId(const qint32 &id);

    QJsonObject marshalJson() Q_DECL_FINAL;

    QFuture<QTdResponse> sendAsync() Q_DECL_FINAL;

private:
    QTdInt32 m_uid;
};

#endif // QTDGETUSERREQUEST_H
