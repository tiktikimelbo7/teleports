#ifndef QTDMESSAGECONTENT_H
#define QTDMESSAGECONTENT_H

#include <QObject>
#include <QDateTime>
#include <QScopedPointer>
#include "common/qabstracttdobject.h"
#include "qtdformattedtext.h"
#include "qtdwebpage.h"
#include "files/qtdaudio.h"
#include "files/qtddocument.h"
#include "files/qtdphoto.h"
#include "files/qtdsticker.h"
#include "files/qtdvideo.h"

class QTdMessageContent : public QTdObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QTdMessageContent)
    Q_PROPERTY(QString infoText READ infoText NOTIFY dataChanged)
    Q_PROPERTY(QUrl infoImageUrl READ infoImageUrl NOTIFY dataChanged)
public:
    explicit QTdMessageContent(QObject *parent = nullptr);
    // infoText and infoImageUrl are helper methods and not part of of TDLib::message_content
    // They are used for displaying limited information about a message when editing or replying.
    QString infoText() const;
    QUrl infoImageUrl() const;
signals:
    void dataChanged();
protected:
    QString m_infoText;
    QUrl m_infoImageUrl;
};

class QTdMessageDate : public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QDateTime date READ date NOTIFY dataChanged)
    Q_PROPERTY(QString dateString READ dateString NOTIFY dataChanged)
public:
    explicit QTdMessageDate(QObject *parent = nullptr);

    QDateTime date() const;
    QString dateString() const;

    void setDate(const qint32 &date);

signals:
    void dataChanged();
private:
    qint32 m_date;
};

class QTdMessageText : public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdFormattedText* text READ text NOTIFY dataChanged)
    Q_PROPERTY(QTdWebPage* webPage READ webPage NOTIFY dataChanged)
public:
    explicit QTdMessageText(QObject *parent = nullptr);

    QTdFormattedText *text() const;
    QTdWebPage *webPage() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void dataChanged();
private:
    Q_DISABLE_COPY(QTdMessageText)
    QScopedPointer<QTdFormattedText> m_text;
    QScopedPointer<QTdWebPage> m_webPage;
    bool m_hasWebPage;
};
class QTdMessageAction : public QTdMessageContent
{
    Q_OBJECT
public:
    explicit QTdMessageAction(QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(QTdMessageAction)
};

class QTdMessageHidden : public QTdMessageContent
{
    Q_OBJECT
public:
    explicit QTdMessageHidden(QObject *parent = nullptr);
private:
    Q_DISABLE_COPY(QTdMessageHidden)
};

class QTdMessageSticker: public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdSticker* sticker READ sticker NOTIFY dataChanged)
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
class QTdMessagePhoto: public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdPhotos* photo READ photo NOTIFY dataChanged)
    Q_PROPERTY(QTdFormattedText* caption READ caption NOTIFY dataChanged)
public:
    explicit QTdMessagePhoto(QObject *parent = nullptr);

    QTdPhotos *photo() const;
    QTdFormattedText *caption() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void dataChanged();

private:
    Q_DISABLE_COPY(QTdMessagePhoto)
    QScopedPointer<QTdPhotos> m_photo;
    QScopedPointer<QTdFormattedText> m_caption;

};
class QTdMessageAnimation: public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdAnimation* animation READ animation NOTIFY dataChanged)
    Q_PROPERTY(QTdFormattedText* caption READ caption NOTIFY dataChanged)
public:
    explicit QTdMessageAnimation(QObject *parent = nullptr);

    QTdAnimation *animation() const;
    QTdFormattedText *caption() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void dataChanged();

private:
    Q_DISABLE_COPY(QTdMessageAnimation)
    QScopedPointer<QTdAnimation > m_animation;
    QScopedPointer<QTdFormattedText> m_caption;

};
class QTdMessageVideo: public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdVideo* video READ video NOTIFY dataChanged)
    Q_PROPERTY(QTdFormattedText* caption READ caption NOTIFY dataChanged)
public:
    explicit QTdMessageVideo(QObject *parent = nullptr);

    QTdVideo *video() const;
    QTdFormattedText *caption() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void dataChanged();

private:
    Q_DISABLE_COPY(QTdMessageVideo)
    QScopedPointer<QTdVideo> m_video;
    QScopedPointer<QTdFormattedText> m_caption;

};
class QTdMessageAudio: public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdAudio* audio READ audio NOTIFY dataChanged)
    Q_PROPERTY(QTdFormattedText* caption READ caption NOTIFY dataChanged)
public:
    explicit QTdMessageAudio(QObject *parent = nullptr);

    QTdAudio *audio() const;
    QTdFormattedText *caption() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void dataChanged();

private:
    Q_DISABLE_COPY(QTdMessageAudio)
    QScopedPointer<QTdAudio> m_audio;
    QScopedPointer<QTdFormattedText> m_caption;

};
class QTdMessageDocument: public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdDocument* document READ document NOTIFY dataChanged)
    Q_PROPERTY(QTdFormattedText* caption READ caption NOTIFY dataChanged)
public:
    explicit QTdMessageDocument(QObject *parent = nullptr);

    QTdDocument *document() const;
    QTdFormattedText *caption() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void dataChanged();

private:
    Q_DISABLE_COPY(QTdMessageDocument)
    QScopedPointer<QTdDocument> m_document;
    QScopedPointer<QTdFormattedText> m_caption;

};
#endif // QTDMESSAGECONTENT_H
