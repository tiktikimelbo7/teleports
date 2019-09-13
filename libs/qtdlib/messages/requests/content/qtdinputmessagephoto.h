#ifndef QTDINPUTMESSAGEPHOTO_H
#define QTDINPUTMESSAGEPHOTO_H

#include <QObject>
#include <QJsonArray>
#include <QString>
#include <QDebug>

#include "imessageattachmentcontent.h"
#include "imessagecaptioncontent.h"
#include "../qtdinputmessagecontent.h"

class QTdInputMessagePhoto : public QTdInputMessageContent, public IMessageCaptionContent, public IMessageAttachmentContent
{
    Q_OBJECT
    Q_INTERFACES(IMessageCaptionContent IMessageAttachmentContent)

public:
    explicit QTdInputMessagePhoto(QObject *parent = nullptr);
    QJsonObject marshalJson() Q_DECL_FINAL;
    void setAttachmentPath(const QString &url);
    void setCaption(const QString &text);
    void setCaptionEntities(const QJsonArray &entities);

private:
    Q_DISABLE_COPY(QTdInputMessagePhoto)
    QString m_photo;
    // QScopedPointer<QTdFormattedText> m_caption;
    QString m_caption;
    QJsonArray m_captionEntities;
};

#endif // QTDINPUTMESSAGEPHOTO_H
