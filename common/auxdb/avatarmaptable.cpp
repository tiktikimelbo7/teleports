
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
    query.prepare("SELECT id, path FROM avatarmap WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec())
        m_db->logSqlError(query);
    if(query.next()) {
        path = query.value(1).toString();
    }
    return path;
}

void AvatarMapTable::setMapEntry(qint64 id, QString path)
{
    QSqlQuery query(*m_db->getDB());
    query.prepare("INSERT OR REPLACE INTO avatarmap(id, path) VALUES(:id, :path)");
    query.bindValue(":id", id);
    query.bindValue(":path", path);
    if (!query.exec())
        m_db->logSqlError(query);
}

