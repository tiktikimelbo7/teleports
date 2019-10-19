#ifndef AVATARMAP_H
#define AVATARMAP_H

#include <QList>
#include <QObject>

class AuxDatabase;

class AvatarMapTable : public QObject
{
    Q_OBJECT

public:
    explicit AvatarMapTable(AuxDatabase *db, QObject *parent = 0);

    QString getAvatarPathbyId(qint64 id);
    void setMapEntry(qint64 id, QString path);
    void setUnreadMapEntry(qint64 id, qint32 unread_count);
    qint32 getTotalUnread();

private:
    AuxDatabase *m_db;
};

#endif // AVATARMAP_H
