#include "qtdchatposition.h"
#include "qtdchattypefactory.h"

QTdChatPosition::QTdChatPosition(QObject *parent)
    : QObject(parent)
    , m_list(Q_NULLPTR)
    , m_order(0)
    , m_isPinned(false)
{
}
QTdChatList *QTdChatPosition::list() const
{
    return m_list;
}

QString QTdChatPosition::qmlOrder() const
{
    return m_order.toQmlValue();
}

qint64 QTdChatPosition::order() const
{
    return m_order.value();
}
bool QTdChatPosition::isPinned() const
{
    return m_isPinned;
}

void QTdChatPosition::unmarshalJson(const QJsonObject &json)
{
    m_order = json["order"].toVariant().toLongLong();
    m_isPinned = json["is_pinned"].toBool();
    if (m_list) {
        delete m_list;
    }
    m_list = QTdChatFactory::createList(json["list"].toObject(), this);
    m_list->unmarshalJson(json);
}
