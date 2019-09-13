#ifndef QTDINPUTMESSAGETEXT_H
#define QTDINPUTMESSAGETEXT_H

#include <QObject>
#include <QJsonArray>
#include <QString>
#include "../qtdinputmessagecontent.h"
// #include "qtdinputformattedtext.h"

class QTdInputMessageText : public QTdInputMessageContent
{
    Q_OBJECT
    // Q_DISABLE_COPY(QTdInputMessageText)
    // QTdFormattedText m_text; <- TODO: that's the way to go
    QString m_text;
    QJsonArray m_entities;
    bool m_disable_web_page_preview;
    bool m_clear_draft;

public:
    explicit QTdInputMessageText(QObject *parent = nullptr);
    QJsonObject marshalJson();
    void setText(const QString &text);
    void setEntities(const QJsonArray &entities);

private:
    Q_DISABLE_COPY(QTdInputMessageText)
};

#endif // QTDINPUTMESSAGETEXT_H
