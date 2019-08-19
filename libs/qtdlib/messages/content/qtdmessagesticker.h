#ifndef QTDMESSAGESTICKER_H
#define QTDMESSAGESTICKER_H

#include <QObject>
#include <QScopedPointer>
#include "messages/qtdmessagecontent.h"
#include "files/qtdsticker.h"

class QTdMessageSticker : public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdSticker *sticker READ sticker NOTIFY dataChanged)
public:
    explicit QTdMessageSticker(QObject *parent = nullptr);

    QTdSticker *sticker() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void dataChanged();

private:
    Q_DISABLE_COPY(QTdMessageSticker)
    QScopedPointer<QTdSticker> m_sticker;
};

#endif // QTDMESSAGESTICKER_H
