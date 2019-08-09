#ifndef QTDMESSAGESCREENSHOTTAKEN_H
#define QTDMESSAGESCREENSHOTTAKEN_H

#include <QObject>
#include <QScopedPointer>
#include "messages/qtdmessagecontent.h"

class QTdMessageScreenshotTaken: public QTdMessageContent
{
    Q_OBJECT
public:
    explicit QTdMessageScreenshotTaken(QObject *parent = nullptr);

    void unmarshalJson(const QJsonObject &json);

private:
    Q_DISABLE_COPY(QTdMessageScreenshotTaken)

};

#endif // QTDMESSAGESCREENSHOTTAKEN_H
