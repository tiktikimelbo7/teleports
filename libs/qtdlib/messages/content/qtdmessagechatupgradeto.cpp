#include "qtdmessagechatupgradeto.h"
#include "chat/requests/qtdgetsupergrouprequest.h"
#include "utils/await.h"

QTdMessageChatUpgradeTo::QTdMessageChatUpgradeTo(QObject *parent) : QTdMessageContent(parent),
    m_sg(new QTdSuperGroupChat)
{
    setType(MESSAGE_CHAT_UPGRADE_TO);
}

QString QTdMessageChatUpgradeTo::qmlSuperGroupId() const
{
    return m_sgId.toQmlValue();
}

qint32 QTdMessageChatUpgradeTo::superGroupId() const
{
    return m_sgId.value();
}

QTdSuperGroupChat *QTdMessageChatUpgradeTo::superGroup() const
{
    return m_sg.data();
}

void QTdMessageChatUpgradeTo::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }

    m_sgId = json["supergroup_id"];
    QScopedPointer<QTdGetSuperGroupRequest> req(new QTdGetSuperGroupRequest);
    req->setSuperGroupId(m_sgId.value());
    QFuture<QTdResponse> resp = req->sendAsync();
    await(resp, 1000);
    if (resp.result().isError()) {
        qWarning() << "Failed fetching supergroup: " << resp.result().errorString();
        return;
    }
    m_sg->unmarshalJson(resp.result().json());
    emit contentChanged();
}
