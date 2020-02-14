#ifndef QTDINPUTMESSAGESTICKER_H
#define QTDINPUTMESSAGESTICKER_H

#include <QObject>
#include <QString>
#include <QDebug>

#include "../qtdinputmessagecontent.h"
#include "files/qtdsticker.h"

class QTdInputMessageSticker : public QTdInputMessageContent
{
    Q_OBJECT

public:
    explicit QTdInputMessageSticker(QObject *parent = nullptr);

    void setSticker(QTdSticker *sticker);

    QJsonObject marshalJson() Q_DECL_FINAL;

private:
    QTdSticker *m_sticker;
    Q_DISABLE_COPY(QTdInputMessageSticker)
};

#endif // QTDINPUTMESSAGESTICKER_H
