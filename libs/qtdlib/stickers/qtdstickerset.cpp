#include "qtdstickerset.h"
#include "client/qtdclient.h"

QTdStickerSet::QTdStickerSet(QObject *parent)
    : QTdObject(parent)
    , m_id(0)
    , m_title("")
    , m_name("")
    , m_thumbnail(new QTdFile(this))
    , m_detailsLoaded(false)
{
    m_stickers = new QQmlObjectListModel<QTdSticker>(this, "", "id");
    connect(QTdClient::instance(), &QTdClient::stickerSet, this, &QTdStickerSet::handleDetails);
}

QString QTdStickerSet::title() const
{
    return m_title;
}

QString QTdStickerSet::name() const
{
    return m_name;
}

QTdFile *QTdStickerSet::thumbnail() const
{
    return m_thumbnail.data();
}

QObject *QTdStickerSet::qmlModel() const
{
    return m_stickers;
}

void QTdStickerSet::loadDetails() const
{
    QTdClient::instance()->send(QJsonObject{
            { "@type", "getStickerSet" },
            { "set_id", m_id.toJsonValue() },
    });
}

void QTdStickerSet::handleDetails(const QJsonObject &json)
{
    if (m_detailsLoaded) {
        return;
    }
    const QJsonValue value = json["id"];
    QTdInt64 tdId;
    tdId = value;
    if (tdId.value() == m_id.value()) {
        this->unmarshalJson(json);
        m_detailsLoaded = true;
    }
}

void QTdStickerSet::unmarshalJson(const QJsonObject &json)
{
    m_id = json["id"];
    m_title = json["title"].toString();
    m_name = json["name"].toString();
    if (json.contains("covers")) {
        auto coversArray = json["covers"].toArray();
        if (!coversArray.isEmpty()) {
            auto coverSticker = coversArray[0].toObject();
            m_thumbnail->unmarshalJson(coverSticker["sticker"].toObject());
        }
    }
    if (json.contains("stickers")) {
        auto stickersArray = json["stickers"].toArray();
        for (auto stickerJSON : stickersArray) {
            QTdSticker *sticker = new QTdSticker(this);
            sticker->unmarshalJson(stickerJSON.toObject());
            m_stickers->append(sticker);
        }
    }
    emit stickerSetChanged();
}
