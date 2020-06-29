#include "qtdchatlistsortfiltermodel.h"
#include <QDebug>
#include "client/qtdclient.h"
#include "models/QmlObjectListModel.h"
#include "qtdbasicgroupchat.h"
#include "qtdsupergroupchat.h"

QTdChatListSortFilterModel::QTdChatListSortFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_chatList(0)
    , m_chatFilters(CurrentChats)
    , m_chatList_type(Main)
{
}

QTdChatListModel *QTdChatListSortFilterModel::model() const
{
    return m_chatList;
}

void QTdChatListSortFilterModel::setModel(QTdChatListModel *model)
{
    m_chatList = model;
    setSourceModel(static_cast<QAbstractItemModel *>(m_chatList->model()));
    connect(model, &QTdChatListModel::chatStatusChanged, [=]() {
        invalidateFilter();
    });
    emit modelChanged();
    setSortRole(static_cast<QQmlObjectListModel<QTdChat> *>(m_chatList->model())->roleForName("lastMessage"));
    setFilterRole(static_cast<QQmlObjectListModel<QTdChat> *>(m_chatList->model())->roleForName("chatType"));
    setDynamicSortFilter(true);
    sort(0, Qt::DescendingOrder);
}

int QTdChatListSortFilterModel::chatFilters() const
{
    return m_chatFilters;
}

int QTdChatListSortFilterModel::chatList() const
{
    return m_chatList_type;
}

void QTdChatListSortFilterModel::setChatFilters(int chatFilters)
{
    if (m_chatFilters == chatFilters)
        return;

    m_chatFilters = chatFilters;
    emit chatFiltersChanged(m_chatFilters);
    invalidateFilter();
}

void QTdChatListSortFilterModel::setChatList(int chatList)
{
    if (m_chatList_type == chatList)
        return;

    m_chatList_type = chatList;
    emit chatListChanged(m_chatList_type);
    invalidateFilter();
}

bool QTdChatListSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // First check for Everything flag as we just want to
    // show well... everything! even if other flags are set
    if (m_chatFilters & ChatFilters::Everything) {
        return true;
    }

    // Ok so the filters want something a little more fine grained.
    // So first we need to get rid of the chats with status banned or left
    QQmlObjectListModel<QTdChat> *model = static_cast<QQmlObjectListModel<QTdChat> *>(sourceModel());
    QTdChat *chat = model->at(source_row);
    if (!chat) {
        return false;
    }
    if (chat->chatList() != Q_NULLPTR) {
        int chatList_type;
        if (chat->chatList()->type() == QTdChatList::Type::CHAT_LIST_MAIN) {
            chatList_type = 1;
        } else if (chat->chatList()->type() == QTdChatList::Type::CHAT_LIST_ARCHIVE) {
            chatList_type = 2;
        }

        if (m_chatList_type != chatList_type) {
            return false;
        }
    }

    switch (chat->chatType()->type()) {
    case QTdChatType::Type::CHAT_TYPE_BASIC_GROUP: {
        QTdBasicGroupChat *gc = static_cast<QTdBasicGroupChat *>(chat);
        if (!gc->status()) {
            return false;
        }
        switch (gc->status()->type()) {
        case QTdChatMemberStatus::Type::CHAT_MEMBER_STATUS_LEFT:
        case QTdChatMemberStatus::Type::CHAT_MEMBER_STATUS_BANNED: {
            return false;
        }
        default:
            break;
        }
        break;
    }
    case QTdChatType::Type::CHAT_TYPE_SUPERGROUP: {
        QTdSuperGroupChat *gc = static_cast<QTdSuperGroupChat *>(chat);
        if (!gc->status()) {
            return false;
        }
        switch (gc->status()->type()) {
        case QTdChatMemberStatus::Type::CHAT_MEMBER_STATUS_LEFT:
        case QTdChatMemberStatus::Type::CHAT_MEMBER_STATUS_BANNED: {
            return false;
        }
        default:
            break;
        }
        break;
    }
    default:
        // Secret and Private groups get their order set to 0 after leaving
        // a chat
        // Archived chats have order set to 0
        if (!chat->order() && m_chatList_type == ChatList::Main) {
            return false;
        }
    }

    // If current chats is defined we are just going to show all the remaining chats
    // after the filtering above has been applied. Otherwise filter on a per chat
    // basis filtering on the remaining filters.
    bool allow = false;
    if (m_chatFilters & ChatFilters::CurrentChats) {
        allow = true;
    } else if (m_chatFilters & ChatFilters::PrivateChats) {
        allow = chat->chatType()->type() == QTdChatType::Type::CHAT_TYPE_PRIVATE;
    } else if (m_chatFilters & ChatFilters::SecretChats) {
        allow = chat->chatType()->type() == QTdChatType::Type::CHAT_TYPE_SECRET;
    } else if (m_chatFilters & ChatFilters::SuperGroups) {
        allow = chat->chatType()->type() == QTdChatType::Type::CHAT_TYPE_SUPERGROUP;
    } else if (m_chatFilters & ChatFilters::BasicGroups) {
        allow = chat->chatType()->type() == QTdChatType::Type::CHAT_TYPE_BASIC_GROUP;
    }

    // Finally if PinnedChats is set then only allow pinned chats
    // This allows us to only show pinned chats for each type above
    if (m_chatFilters & ChatFilters::PinnedChats) {
        allow = chat->isPinned();
    }

    return allow;
}

bool QTdChatListSortFilterModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    /**
     * TDlib suggests to use QTdChat::order() for ordering of the chat list
     *
     * Note this requires calling getBasicGroupData and getSuperGroupData to get the QTdChat::order
     * to even update so we have to wait longer for them to arrive compared to a basic chat or channel.
     */
    QQmlObjectListModel<QTdChat> *model = static_cast<QQmlObjectListModel<QTdChat> *>(sourceModel());
    QTdChat *left = model->at(source_left.row());
    QTdChat *right = model->at(source_right.row());

    /*
    This is how the sorting rule works:
        IF left and right sort orders are <>0 AND left order is smaller than right order, return true
        OR IF both sort orders are equal AND left id is smaller than right id, return true
        OR IF left OR right order is 0 AND left last message date is smaller then right last message date
            AND left is not pinned, return true
        ELSE return false

        The first part is the rule from tdlib, sort by order, but if equal order is given, sort by id
        The second part is a safeguard against sort order being 0, which happens with "Saved Messages" chat a lot
     */
    auto result = left->order() != 0 && right->order() != 0 && left->order() < right->order()
            || left->order() == right->order() && left->id() < right->id()
            || (left->order() == 0 || right->order() == 0)
                    && left->lastMessage() && right->lastMessage()
                    && left->lastMessage()->date() < right->lastMessage()->date() && !left->isPinned();
    return result;
}
