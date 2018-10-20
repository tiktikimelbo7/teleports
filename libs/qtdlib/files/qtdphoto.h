#ifndef QTDPHOTO_H
#define QTDPHOTO_H

#include <QObject>
#include <QScopedPointer>
#include "common/qabstractint64id.h"
#include "qtdfile.h"

class QTdPhoto : public QTdObject
{
    Q_OBJECT
    Q_PROPERTY(QTdFile* small READ small NOTIFY smallChanged)
    Q_PROPERTY(QTdFile* big READ big NOTIFY bigChanged)
public:
    explicit QTdPhoto(QObject *parent = nullptr);

    /**
     * @brief A small (160x160) user profile photo
     */
    QTdFile* small() const;
    /**
     * @brief A big (640x640) user profile photo
     */
    QTdFile* big() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void smallChanged(QTdFile* small);
    void bigChanged(QTdFile* big);

private:
    QScopedPointer<QTdFile> m_small;
    QScopedPointer<QTdFile> m_big;
};

#endif // QTDPHOTO_H
