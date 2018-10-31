#include "qtdmessagecontent.h"

QTdMessageContent::QTdMessageContent(QObject *parent) : QTdObject(parent)
{
}

QTdMessageText::QTdMessageText(QObject *parent) : QTdMessageContent(parent),
    m_text(new QTdFormattedText), m_webPage(new QTdWebPage), m_hasWebPage(false)
{
    setType(MESSAGE_TEXT);
}

QTdFormattedText *QTdMessageText::text() const
{
    return m_text.data();
}

QTdWebPage *QTdMessageText::webPage() const
{
    if (!m_hasWebPage) {
        return Q_NULLPTR;
    }
    return m_webPage.data();
}

void QTdMessageText::unmarshalJson(const QJsonObject &json)
{
    m_hasWebPage = false;
    m_text->unmarshalJson(json["text"].toObject());
    if (json.contains("web_page")) {
        m_webPage->unmarshalJson(json["web_page"].toObject());
        m_hasWebPage = true;
    }
    emit dataChanged();
}

QTdMessageAction::QTdMessageAction(QObject *parent) : QTdMessageContent(parent)
{
  setType(MESSAGE_ACTION);
}

QTdMessageHidden::QTdMessageHidden(QObject *parent) : QTdMessageContent(parent)
{
  setType(MESSAGE_HIDDEN);
}

QTdMessageSticker::QTdMessageSticker(QObject *parent) : QTdMessageContent(parent),
    m_sticker(new QTdSticker)
{
    setType(MESSAGE_STICKER);
}

QTdSticker *QTdMessageSticker::sticker() const
{
    return m_sticker.data();
}

void QTdMessageSticker::unmarshalJson(const QJsonObject &json)
{
    m_sticker->unmarshalJson(json["sticker"].toObject());
}

QTdMessagePhoto::QTdMessagePhoto(QObject *parent) : QTdMessageContent(parent),
    m_photo(new QTdPhotos), m_caption(new QTdFormattedText)
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
QTdMessageAnimation::QTdMessageAnimation(QObject *parent) : QTdMessageContent(parent),
    m_animation(new QTdAnimation), m_caption(new QTdFormattedText)
{
    setType(MESSAGE_ANIMATION);
}

QTdAnimation *QTdMessageAnimation::animation() const
{
    return m_animation.data();
}
QTdFormattedText *QTdMessageAnimation::caption() const
{
  return m_caption.data();
}
void QTdMessageAnimation::unmarshalJson(const QJsonObject &json)
{
    m_animation->unmarshalJson(json["animation"].toObject());
    m_caption->unmarshalJson(json["caption"].toObject());
}
