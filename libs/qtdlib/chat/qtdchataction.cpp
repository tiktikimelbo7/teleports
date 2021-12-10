#include "qtdchataction.h"
#include "utils/i18n.h"

QTdChatAction::QTdChatAction(QObject *parent)
    : QTdObject(parent)
{
    m_singular_description = QString(gettext("is doing something..."));
    m_plural_description = QString(gettext("are doing something..."));
}

QString QTdChatAction::singular_description()
{
    return m_singular_description;
}

QString QTdChatAction::plural_description()
{
    return m_plural_description;
}

QTdChatActionCancel::QTdChatActionCancel(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_CANCEL);
}

QTdChatActionChoosingContact::QTdChatActionChoosingContact(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_CHOOSING_CONTACT);
    m_singular_description = QString(gettext("is choosing a contact..."));
    m_plural_description = QString(gettext("are choosing a contact..."));
}

QTdChatActionRecordingVideo::QTdChatActionRecordingVideo(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_RECORDING_VIDEO);
    m_singular_description = QString(gettext("is recording a video..."));
    m_plural_description = QString(gettext("are recording a video..."));
}

QTdChatActionChoosingLocation::QTdChatActionChoosingLocation(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_CHOOSING_LOCATION);
    m_singular_description = QString(gettext("is choosing a location..."));
    m_plural_description = QString(gettext("are choosing a location..."));
}

QTdChatActionRecordingVideoNote::QTdChatActionRecordingVideoNote(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_RECORDING_VIDEO_NOTE);
    m_singular_description = QString(gettext("is recording a video note..."));
    m_plural_description = QString(gettext("are recording a video note..."));
}

QTdChatActionRecordingVoiceNote::QTdChatActionRecordingVoiceNote(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_RECORDING_VOICE_NOTE);
    m_singular_description = QString(gettext("is recording a voice note..."));
    m_plural_description = QString(gettext("are recording a voice note..."));
}

QTdChatActionStartPlayingGame::QTdChatActionStartPlayingGame(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_START_PLAYING_GAME);
    m_singular_description = QString(gettext("is starting a game..."));
    m_plural_description = QString(gettext("are starting a game..."));
}

QTdChatActionTyping::QTdChatActionTyping(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_TYPING);
    m_singular_description = QString(gettext("is typing..."));
    m_plural_description = QString(gettext("are typing..."));
}

QTdChatActionUploadingDocument::QTdChatActionUploadingDocument(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_UPLOADING_DOCUMENT);
    m_singular_description = QString(gettext("is uploading a document..."));
    m_plural_description = QString(gettext("are uploading a document..."));
}

QTdChatActionUploadingPhoto::QTdChatActionUploadingPhoto(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_UPLOADING_PHOTO);
    m_singular_description = QString(gettext("is uploading a photo..."));
    m_plural_description = QString(gettext("are uploading a photo..."));
}

QTdChatActionUploadingVideo::QTdChatActionUploadingVideo(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_UPLOADING_VIDEO);
    m_singular_description = QString(gettext("is uploading a video..."));
    m_plural_description = QString(gettext("are uploading a video..."));
}

QTdChatActionUploadingVideoNote::QTdChatActionUploadingVideoNote(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_UPLOADING_VIDEO_NOTE);
    m_singular_description = QString(gettext("is uploading a video note..."));
    m_plural_description = QString(gettext("are uploading a video note..."));
}

QTdChatActionUploadingVoiceNote::QTdChatActionUploadingVoiceNote(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_UPLOADING_VOICE_NOTE);
    m_singular_description = QString(gettext("is uploading a voice note..."));
    m_plural_description = QString(gettext("are uploading a voice note..."));
}

QTdChatActionChoosingSticker::QTdChatActionChoosingSticker(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_CHOOSING_STICKER);
    m_singular_description = QString(gettext("is choosing a sticker..."));
    m_plural_description = QString(gettext("are choosing a sticker..."));
}

QTdChatActionRecordingAudio::QTdChatActionRecordingAudio(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_RECORDING_AUDIO);
    m_singular_description = QString(gettext("is recording audio..."));
    m_plural_description = QString(gettext("are recording audio..."));
}

QTdChatActionUploadingAudio::QTdChatActionUploadingAudio(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_UPLOADING_AUDIO);
    m_singular_description = QString(gettext("is uploading audio..."));
    m_plural_description = QString(gettext("are uploading audio..."));
}

QTdChatActionUnknown::QTdChatActionUnknown(QObject *parent)
    : QTdChatAction(parent)
{
    setType(CHAT_ACTION_UNKNOWN);
}
