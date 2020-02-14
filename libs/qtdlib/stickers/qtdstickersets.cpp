#include "qtdstickersets.h"
#include "client/qtdclient.h"
#include <QJsonObject>
#include <QJsonArray>

QTdStickerSets::QTdStickerSets(QObject *parent)
    : QTdObject(parent)
{
    m_model = new QQmlObjectListModel<QTdStickerSet>(this, "", "id");
    connect(QTdClient::instance(), &QTdClient::stickerSets, this, &QTdStickerSets::handleStickerSets);
}

QObject *QTdStickerSets::qmlModel() const
{
    return m_model;
}

void QTdStickerSets::loadStickerSets()
{
    QTdClient::instance()->send(QJsonObject{
            { "@type", "getInstalledStickerSets" },
            { "is_masks", false },
    });
}

void QTdStickerSets::handleStickerSets(const QJsonObject &json)
{
    m_model->clear();
    auto array = json["sets"].toArray();
    for (auto value : array) {
        auto newSet = new QTdStickerSet(this);
        newSet->unmarshalJson(value.toObject());
        m_model->append(newSet);
    }
}
