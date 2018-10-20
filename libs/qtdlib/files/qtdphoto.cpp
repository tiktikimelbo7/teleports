#include "qtdphoto.h"
#include <QJsonArray>
#include <QDebug>


QTdPhoto::QTdPhoto(QObject *parent) : QAbstractInt64Id(parent),
    m_hasStickers(false), m_sizes(Q_NULLPTR)
{
    m_sizes = new QQmlObjectListModel<QTdPhotoSize>(this, "", "type");
}

QTdPhoto::~QTdPhoto()
{
    m_sizes->clear();
}

void QTdPhoto::unmarshalJson(const QJsonObject &json)
{
    m_hasSitckers = json["has_stickers"].toBool();
    const QJsonArray sizes = json["sizes"].toArray();
    for (const QJsonValue &size : sizes) {
        auto *s = new QTdPhotoSize();
        s->unmarshalJson(size.toObject());
        m_sizes->append(s);
    }
    QAbstractInt64Id::unmarshalJson(json);
    emit photosChanged();
}

bool QTdPhoto::hasStickers() const
{
    return m_hasSitckers;
}

QObject *QTdPhoto::qmlSizes() const
{
    return m_sizes;
}

QQmlObjectListModel<QTdPhotoSize> *QTdPhoto::sizes() const
{
    return m_sizes;
}
