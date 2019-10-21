#ifndef QTDINPUTMESSAGEVIDEO_H
#define QTDINPUTMESSAGEVIDEO_H

#include <QObject>
#include <QJsonArray>
#include <QString>
#include <QDebug>

#include "imessageattachmentcontent.h"
#include "imessagecaptioncontent.h"
#include "../qtdinputmessagecontent.h"

class QTdInputMessageVideo : public QTdInputMessageContent, public IMessageCaptionContent, public IMessageAttachmentContent
{
    Q_OBJECT
    Q_INTERFACES(IMessageCaptionContent IMessageAttachmentContent)

public:
    explicit QTdInputMessageVideo(QObject *parent = nullptr);
    QJsonObject marshalJson() Q_DECL_FINAL;
    void setAttachmentPath(const QString &url);
    void setCaption(const QString &text);
    void setCaptionEntities(const QJsonArray &entities);

private:
    Q_DISABLE_COPY(QTdInputMessageVideo)
    QString m_video;
    QString m_caption;
    QJsonArray m_captionEntities;
};

#endif // QTDINPUTMESSAGEVIDEO_H
