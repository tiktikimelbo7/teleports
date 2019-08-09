#include "qtdmessagevoicenote.h"

QTdMessageVoiceNote::QTdMessageVoiceNote(QObject *parent) : QTdMessageContent(parent),
    m_voiceNote(new QTdVoiceNote), m_caption(new QTdFormattedText), m_isListened(new bool)
{
    setType(MESSAGE_VOICE_NOTE);
}

QTdVoiceNote *QTdMessageVoiceNote::voiceNote() const
{
    return m_voiceNote.data();
}
QTdFormattedText *QTdMessageVoiceNote::caption() const
{
  return m_caption.data();
}
bool QTdMessageVoiceNote::isListened() const
{
  return m_isListened;
}
void QTdMessageVoiceNote::unmarshalJson(const QJsonObject &json)
{
    m_voiceNote->unmarshalJson(json["voice_note"].toObject());
    m_caption->unmarshalJson(json["caption"].toObject());
    m_isListened = json["is_listened"].toBool();
}
