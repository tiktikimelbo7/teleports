#include "qtdvideonote.h"

QTdVideoNote::QTdVideoNote(QObject *parent)
    : QTdObject(parent)
    , m_duration(0)
    , m_length(0)
    , m_thumbnail(new QTdPhotoSize)
    , m_video(new QTdFile)
{
    setType(VIDEO_NOTE);
}

qint32 QTdVideoNote::duration() const
{
    return m_duration;
}

qint32 QTdVideoNote::length() const
{
    return m_duration;
}

QTdPhotoSize *QTdVideoNote::thumbnail() const
{
    return m_thumbnail.data();
}

QTdFile *QTdVideoNote::video() const
{
    return m_video.data();
}

void QTdVideoNote::unmarshalJson(const QJsonObject &json)
{
    m_duration = qint32(json["duration"].toInt());
    m_length = qint32(json["length"].toInt());
    // TODO: Repair this after upgrade to tdlib 1.7.9
    // if (json.contains("thumbnail")) {
    //     m_thumbnail->unmarshalJson(json["thumbnail"].toObject());
    // }
    m_video->unmarshalJson(json["video"].toObject());
    emit videoChanged();
}
