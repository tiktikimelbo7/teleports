#ifndef QTDINPUTMESSAGEDOCUMENT_H
#define QTDINPUTMESSAGEDOCUMENT_H

#include <QObject>
#include <QJsonArray>
#include <QString>
#include <QDebug>

#include "imessageattachmentcontent.h"
#include "imessagecaptioncontent.h"
#include "../qtdinputmessagecontent.h"

class QTdInputMessageDocument : public QTdInputMessageContent, public IMessageCaptionContent, public IMessageAttachmentContent
{
    Q_OBJECT
    Q_INTERFACES(IMessageCaptionContent IMessageAttachmentContent)

public:
    explicit QTdInputMessageDocument(QObject *parent = nullptr);
    QJsonObject marshalJson() Q_DECL_FINAL;
    void setAttachmentPath(const QString &url);
    void setCaption(const QString &text);
    void setCaptionEntities(const QJsonArray &entities);

private:
    Q_DISABLE_COPY(QTdInputMessageDocument)
    QString m_document;
    QString m_caption;
    QJsonArray m_captionEntities;
};

#endif // QTDINPUTMESSAGEDOCUMENT_H
