#include "qtdsortedchats.h"
#include "qtdchats.h"
#include <QDebug>
#include "client/qtdclient.h"
#include "models/QmlObjectListModel.h"
#include "qtdbasicgroupchat.h"
#include "qtdsupergroupchat.h"

QTdSortedChats::QTdSortedChats(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_chatFilters(Everything)
{
}

int QTdSortedChats::chatFilters() const
{
    return m_chatFilters;
}

void QTdSortedChats::setChatFilters(int chatFilters)
{
    if (m_chatFilters == chatFilters)
        return;

    m_chatFilters = chatFilters;
    emit chatFiltersChanged(m_chatFilters);
    invalidateFilter();
}

void QTdSortedChats::setChatNameFilter(const QString &chatNameFilter)
{
    m_chatNameFilter = chatNameFilter;
    invalidateFilter();
}

bool QTdSortedChats::filterBarVisible() const
{
    return m_filterBarVisible;
}

void QTdSortedChats::toggleFilterBar(const bool &value)
{
    m_filterBarVisible = value;
    if (!m_filterBarVisible) {
        m_chatFilters |= ChatFilters::Everything;
    } else {
        m_chatFilters &= ~ChatFilters::Everything;
    }
    invalidateFilter();
}

void QTdSortedChats::setChatListFilter(const int &value)
{

    switch(value) {
        case 0:
            m_chatFilters = ChatFilters::Everything;
            break;
        case 1:
            m_chatFilters = ChatFilters::Personal;
            break;
        case 2:
            m_chatFilters = ChatFilters::Unread;
            break;
        case 3:
            m_chatFilters = ChatFilters::Archived;
            break;
    }
    invalidateFilter();
}

bool QTdSortedChats::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QQmlObjectListModel<QTdChat> *model = static_cast<QQmlObjectListModel<QTdChat> *>(sourceModel());
    QTdChat *chat = model->at(source_row);

    if (!chat || !chat->order()) {
        return false;
    }

    bool showRow = true;
    bool archived = chat->chatList() && chat->chatList()->typeString() == "chatListArchive";

    //Apply chat title filter, if set
    if (!chat->title().contains(m_chatNameFilter, Qt::CaseInsensitive)) {
        showRow = false;
    }

    if (m_chatFilters & ChatFilters::Everything) {
        return showRow & !archived;
    }

    if (m_chatFilters & ChatFilters::Unread && !(chat->hasUnreadMessages() || chat->hasUnreadMentions())) {
        showRow = false;
    }

    if (m_chatFilters & ChatFilters::Personal && !(chat->isPrivate() || chat->isSecret())) {
        showRow = false;
    }

    if (chat->chatList()) {
        if (m_chatFilters & ChatFilters::Archived) {
            showRow = showRow & archived;
        } else {
            showRow = showRow & !archived;
        }
    }

    return showRow;
}

bool QTdSortedChats::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
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
