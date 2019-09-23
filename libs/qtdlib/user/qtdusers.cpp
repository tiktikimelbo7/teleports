#include "qtdusers.h"
#include <QPointer>
#include <QDebug>
#include "client/qtdclient.h"
#include "common/qtdint.h"
#include "user/qtduserstatusfactory.h"

QTdUsers::QTdUsers(QObject *parent)
    : QObject(parent)
    , m_model(Q_NULLPTR)
    , m_meMyself(Q_NULLPTR)
{
    m_model = new QQmlObjectListModel<QTdUser>(this, "", "id");
    m_meMyself = new QTdUser(this);
    connect(QTdClient::instance(), &QTdClient::updateUser, this, &QTdUsers::handleUpdateUser);
    connect(QTdClient::instance(), &QTdClient::updateUserStatus, this, &QTdUsers::handleUpdateUserStatus);
}

QTdUsers::~QTdUsers()
{
    m_model->clear();
}

QObject *QTdUsers::qmlModel() const
{
    return m_model;
}

QQmlObjectListModel<QTdUser> *QTdUsers::model() const
{
    return m_model;
}

static QPointer<QTdUsers> s_users;
QTdUsers *QTdUsers::instance()
{
    if (s_users.isNull()) {
        s_users = new QTdUsers();
    }
    return s_users;
}

QTdUser *QTdUsers::meMyself() const
{
    return m_meMyself;
}

void QTdUsers::handleUpdateUser(const QJsonObject &user)
{
    const qint32 uid = qint32(user["id"].toInt());
    // Need to remember the model actually indexes on the qmlId variant which is a QString
    QTdUser *tduser = m_model->getByUid(QString::number(uid));
    if (!tduser) {
        tduser = new QTdUser();
        tduser->unmarshalJson(user);
        m_model->append(tduser);
        emit userCreated(uid);
    } else {
        tduser->unmarshalJson(user);
    }

    const qint32 myId = qint32(QTdClient::instance()->getOption("my_id").toInt());
    if (uid == myId) {
        m_meMyself->unmarshalJson(user);
        emit meMyselfChanged(m_meMyself);
    }
}

void QTdUsers::handleUpdateUserStatus(const QString &userId, const QJsonObject &status)
{
    QTdUser *tduser = m_model->getByUid(userId);
    if (tduser) {
        tduser->setStatus(QTdUserStatusFactory::create(status, tduser));
    }

    const qint32 uid = qint32(userId.toInt());
    const qint32 myId = qint32(QTdClient::instance()->getOption("my_id").toInt());
    if (uid == myId) {
        m_meMyself->setStatus(QTdUserStatusFactory::create(status, m_meMyself));
    }
}

QTdUsersSortFilterModel::QTdUsersSortFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    connect(this, &QTdUsersSortFilterModel::countChanged, this, &QTdUsersSortFilterModel::rowCountChanged);
}

QTdUser *QTdUsersSortFilterModel::get(const int &row)
{
    QModelIndex idx = index(row, 0);
    if (idx.isValid()) {
        QModelIndex srcIdx = mapToSource(idx);
        if (srcIdx.isValid()) {
            QQmlObjectListModel<QTdUser> *model = static_cast<QQmlObjectListModel<QTdUser> *>(sourceModel());
            return model->at(srcIdx.row());
        }
    }
    return nullptr;
}

void QTdUsersSortFilterModel::setAllowedUsers(QList<qint32> user_ids)
{
    if (user_ids == m_uids) {
        return;
    }
    m_uids = user_ids;
    emit allowedUsersChanged();
    invalidateFilter();
}

bool QTdUsersSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QQmlObjectListModel<QTdUser> *model = static_cast<QQmlObjectListModel<QTdUser> *>(sourceModel());
    QTdUser *user = model->at(source_row);
    if (!user) {
        return false;
    }
    return m_uids.contains(user->id());
}
