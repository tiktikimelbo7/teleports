#ifndef QTDUSERS_H
#define QTDUSERS_H

#include <QObject>
#include <QJsonObject>
#include <QSortFilterProxyModel>
#include "user/qtduser.h"
#include "models/QmlObjectListModel.h"

class QTdUsers : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* model READ qmlModel NOTIFY modelChanged)
    Q_PROPERTY(QTdUser* me READ meMyself NOTIFY meMyselfChanged)
    Q_PROPERTY(QTdUser* currentUser READ currentUser WRITE setCurrentUser NOTIFY currentUserChanged)

public:
    explicit QTdUsers(QObject *parent = nullptr);
    ~QTdUsers();

    static QTdUsers  *instance();

    QObject* qmlModel() const;
    QTdUser* meMyself() const;
    QTdUser* currentUser() const;

    QQmlObjectListModel<QTdUser> *model() const;
public slots:
    void setCurrentUser(QTdUser* currentUser);
    void clearCurrentUser();

signals:
    void modelChanged(QObject* model);
    void userCreated(qint32 id);

    void currentUserChanged(QTdUser* currentUser);
    void meMyselfChanged(QTdUser *meMyself);


private slots:
    void handleUpdateUser(const QJsonObject &user);
    void handleUpdateUserStatus(const QString &userId, const QJsonObject &status);

private:
    Q_DISABLE_COPY(QTdUsers)
    QQmlObjectListModel<QTdUser> *m_model;
    QPointer<QTdUser> m_meMyself;
    QPointer<QTdUser> m_currentUser;

};

/**
 * @brief The QTdUsersSortFilterModel class
 *
 * Filters users to only show the allowed user ids
 *
 * This makes it easier to reuse the global user instance
 * to just show a subset of user.
 */
class QTdUsersSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_DISABLE_COPY(QTdUsersSortFilterModel)
    Q_PROPERTY(int count READ rowCount NOTIFY rowCountChanged)
public:
    explicit QTdUsersSortFilterModel(QObject *parent = nullptr);
    Q_INVOKABLE QTdUser *get(const int &row);

public slots:
    void setAllowedUsers(QList<qint32> user_ids);

signals:
    void countChanged();
    void allowedUsersChanged();
    void rowCountChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    QList<qint32> m_uids;
};

#endif // QTDUSERS_H
