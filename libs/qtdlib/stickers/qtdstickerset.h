#ifndef QTDSTICKERSET_H
#define QTDSTICKERSET_H

#include <QObject>
#include <QPointer>
#include "models/QmlObjectListModel.h"
#include "common/qabstracttdobject.h"
#include "files/qtdthumbnail.h"
#include "files/qtdsticker.h"

class QTdStickerSet : public QTdObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY stickerSetChanged)
    Q_PROPERTY(QString title READ title NOTIFY stickerSetChanged)
    Q_PROPERTY(QTdThumbnail *thumbnail READ thumbnail NOTIFY stickerSetChanged)
    Q_PROPERTY(bool hasThumbnail READ hasThumbnail NOTIFY stickerSetChanged)
    Q_PROPERTY(bool isAnimated READ isAnimated NOTIFY stickerSetChanged)
    Q_PROPERTY(QObject *stickers READ qmlModel NOTIFY stickerSetChanged)
public:
    explicit QTdStickerSet(QObject *parent = nullptr);

    QString title() const;
    QString name() const;
    QTdThumbnail *thumbnail() const;
    bool hasThumbnail() const;
    bool isAnimated() const;

    QObject *qmlModel() const;

    Q_INVOKABLE void loadDetails() const;

    void handleDetails(const QJsonObject &json);

    void unmarshalJson(const QJsonObject &json) Q_DECL_FINAL;

signals:
    void stickerSetChanged();

private:
    QPointer<QQmlObjectListModel<QTdSticker>> m_stickers;
    QTdInt64 m_id;
    QString m_title;
    QString m_name;
    QPointer<QTdThumbnail> m_thumbnail;
    bool m_hasThumbnail;
    bool m_isAnimated;
    bool m_detailsLoaded;
};

#endif // QTDSTICKERSET_H
