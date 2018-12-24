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
QTdMessageVideo::QTdMessageVideo(QObject *parent) : QTdMessageContent(parent),
    m_video(new QTdVideo), m_caption(new QTdFormattedText)
{
    setType(MESSAGE_VIDEO);
}

QTdVideo *QTdMessageVideo::video() const
{
    return m_video.data();
}
QTdFormattedText *QTdMessageVideo::caption() const
{
  return m_caption.data();
}
void QTdMessageVideo::unmarshalJson(const QJsonObject &json)
{
    m_video->unmarshalJson(json["video"].toObject());
    m_caption->unmarshalJson(json["caption"].toObject());
}
QTdMessageAudio::QTdMessageAudio(QObject *parent) : QTdMessageContent(parent),
    m_audio(new QTdAudio), m_caption(new QTdFormattedText)
{
    setType(MESSAGE_AUDIO);
}

QTdAudio *QTdMessageAudio::audio() const
{
    return m_audio.data();
}
QTdFormattedText *QTdMessageAudio::caption() const
{
  return m_caption.data();
}
void QTdMessageAudio::unmarshalJson(const QJsonObject &json)
{
    m_audio->unmarshalJson(json["audio"].toObject());
    m_caption->unmarshalJson(json["caption"].toObject());
}
QTdMessageDocument::QTdMessageDocument(QObject *parent) : QTdMessageContent(parent),
    m_document(new QTdDocument), m_caption(new QTdFormattedText)
{
    setType(MESSAGE_DOCUMENT);
}

QTdDocument *QTdMessageDocument::document() const
{
    return m_document.data();
}
QTdFormattedText *QTdMessageDocument::caption() const
{
  return m_caption.data();
}
void QTdMessageDocument::unmarshalJson(const QJsonObject &json)
{
    m_document->unmarshalJson(json["document"].toObject());
    m_caption->unmarshalJson(json["caption"].toObject());
}

QTdMessageDate::QTdMessageDate(QObject *parent) : QTdMessageContent(parent),
    m_date(0)
{
    setType(MESSAGE_DATE);
}

QDateTime QTdMessageDate::date() const
{
    return QDateTime::fromTime_t(m_date);
}

QString QTdMessageDate::dateString() const
{
    return date().toString("MMMM dd");
}

void QTdMessageDate::setDate(const qint32 &date)
{
    if (date == m_date) {
        return;
    }
    m_date = date;
    emit dataChanged();
}
