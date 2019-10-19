
#include "avatarmaptable.h"
#include "auxdb.h"

#include <QtSql>

AvatarMapTable::AvatarMapTable(AuxDatabase *db, QObject *parent)
    : QObject(parent)
    , m_db(db)
{
}

QString AvatarMapTable::getAvatarPathbyId(qint64 id)
{
    QString path = "";
    if (!m_db->getDB()) {
        return path;
    }
    QSqlQuery query(*m_db->getDB());
    query.prepare("SELECT path FROM chatlist_map WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec())
        m_db->logSqlError(query);
    if (query.next()) {
        path = query.value(0).toString();
    }
    return path;
}

void AvatarMapTable::setMapEntry(qint64 id, QString path)
{
    QSqlQuery query(*m_db->getDB());
    query.prepare("INSERT OR REPLACE INTO chatlist_map(id, path, unread_messages) VALUES(:id, :path, (select unread_messages from chatlist_map where id = :id))");
    query.bindValue(":id", id);
    query.bindValue(":path", path);
    if (!query.exec())
        m_db->logSqlError(query);
}

void AvatarMapTable::setUnreadMapEntry(qint64 id, qint32 unread_messages)
{
    QSqlQuery query(*m_db->getDB());
    query.prepare("INSERT OR REPLACE INTO chatlist_map(id, path, unread_messages) VALUES(:id, COALESCE((select path from chatlist_map where id = :id), \"\"), :unread_messages)");
    query.bindValue(":id", id);
    query.bindValue(":unread_messages", unread_messages);
    if (!query.exec())
        m_db->logSqlError(query);
}

qint32 AvatarMapTable::getTotalUnread() {
    qint32 totalCount = 0;
    if (!m_db->getDB()) {
        return totalCount;
    }
    QSqlQuery query(*m_db->getDB());
    query.prepare("SELECT SUM(unread_messages) FROM chatlist_map");
    if (!query.exec())
        m_db->logSqlError(query);
    if (query.next()) {
        totalCount = query.value(0).toInt();
    }
    return totalCount;
}
