#include "qtdmessagechatupgradeto.h"
#include "chat/requests/qtdgetsupergrouprequest.h"
#include "utils/await.h"

QTdMessageChatUpgradeTo::QTdMessageChatUpgradeTo(QObject *parent) : QTdMessageContent(parent)
    
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

void QTdMessageChatUpgradeTo::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    m_sgId = json["supergroup_id"];
    emit contentChanged();
}
