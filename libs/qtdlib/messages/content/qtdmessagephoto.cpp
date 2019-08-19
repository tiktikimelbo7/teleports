#include "qtdmessagephoto.h"

QTdMessagePhoto::QTdMessagePhoto(QObject *parent)
    : QTdMessageContent(parent)
    , m_photo(new QTdPhotos)
    , m_caption(new QTdFormattedText)
{
    setType(MESSAGE_PHOTO);
}

QTdPhotos *QTdMessagePhoto::photo() const
{
    return m_photo.data();
}
QTdFormattedText *QTdMessagePhoto::caption() const
{
    return m_caption.data();
}
void QTdMessagePhoto::unmarshalJson(const QJsonObject &json)
{
    m_photo->unmarshalJson(json["photo"].toObject());
    m_caption->unmarshalJson(json["caption"].toObject());
}
