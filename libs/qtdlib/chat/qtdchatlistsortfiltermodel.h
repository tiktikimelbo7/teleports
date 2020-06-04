#ifndef QTDCHATLISTSORTFILTERMODEL_H
#define QTDCHATLISTSORTFILTERMODEL_H

#include <QObject>
#include <QPointer>
#include <QFlags>
#include <QSortFilterProxyModel>
#include "qtdchatlistmodel.h"

/**
 * @brief The QTdChatListSortFilterModel class
 *
 * Sorts chat list in descending order based on QTdChat::order()
 *
 * Filters can be applied to show certain QTdChatType's
 */
class QTdChatListSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QTdChatListModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int chatFilters READ chatFilters WRITE setChatFilters NOTIFY chatFiltersChanged)
    Q_PROPERTY(int chatList READ chatList WRITE setChatList NOTIFY chatListChanged)
public:
    explicit QTdChatListSortFilterModel(QObject *parent = nullptr);

    enum ChatFilters {
        Everything = 1, // this includes banned or left chats
        PrivateChats = 2, // QTdPrivate chats
        BasicGroups = 4, // QTDBasicGroup chats
        SuperGroups = 8, // QTdSuperGroup chats
        SecretChats = 16, // QTdSecret chats
        CurrentChats = 32, // All accessible chats
        PinnedChats = 64 // All pinned chats - should be used to limit the above flags to just pinned chats of each type.
    };
    Q_ENUM(ChatFilters)
    enum ChatList {
        Main = 1,
        Archive = 2,
    };
    Q_ENUM(ChatList)

    QTdChatListModel *model() const;
    void setModel(QTdChatListModel *model);
    int chatFilters() const;
    int chatList() const;

public slots:
    void setChatFilters(int chatFilters);
    void setChatList(int chatList);

signals:
    void modelChanged();
    void countChanged();
    void chatFiltersChanged(int chatFilters);
    void chatListChanged(int chatList);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

private:
    Q_DISABLE_COPY(QTdChatListSortFilterModel)
    QPointer<QTdChatListModel> m_chatList;
    int m_chatFilters;
    int m_chatList_type;
};

#endif // QTDCHATLISTSORTFILTERMODEL_H
