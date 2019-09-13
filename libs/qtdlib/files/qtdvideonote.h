#ifndef QTDVIDEONOTE_H
#define QTDVIDEONOTE_H

#include <QObject>
#include <QScopedPointer>
#include "common/qabstracttdobject.h"
#include "common/qtdint.h"
#include "qtdphotosize.h"
#include "qtdfile.h"

class QTdVideoNote : public QTdObject
{
    Q_OBJECT
    Q_PROPERTY(qint32 duration READ duration NOTIFY videoChanged)
    Q_PROPERTY(qint32 length READ length NOTIFY videoChanged)
    Q_PROPERTY(QTdPhotoSize *thumbnail READ thumbnail NOTIFY videoChanged)
    Q_PROPERTY(QTdFile *video READ video NOTIFY videoChanged)
public:
    explicit QTdVideoNote(QObject *parent = nullptr);

    qint32 duration() const;

    qint32 length() const;

    QTdPhotoSize *thumbnail() const;

    QTdFile *video() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void videoChanged();

private:
    qint32 m_duration;
    qint32 m_length;
    QScopedPointer<QTdPhotoSize> m_thumbnail;
    QScopedPointer<QTdFile> m_video;
};

#endif // QTDVIDEONOTE_H
