#ifndef QTDMESSAGEVIDEONOTE_H
#define QTDMESSAGEVIDEONOTE_H

#include <QObject>
#include <QScopedPointer>
#include "messages/qtdmessagecontent.h"
#include "files/qtdvideonote.h"

class QTdMessageVideoNote: public QTdMessageContent
{
    Q_OBJECT
    Q_PROPERTY(QTdVideoNote* videoNote READ videoNote NOTIFY dataChanged)
    Q_PROPERTY(bool isViewed READ isViewed NOTIFY dataChanged)
    Q_PROPERTY(bool isSecret READ isSecret NOTIFY dataChanged)

public:
    explicit QTdMessageVideoNote(QObject *parent = nullptr);

    QTdVideoNote *videoNote() const;
    bool isViewed() const;
    bool isSecret() const;

    void unmarshalJson(const QJsonObject &json);

signals:
    void dataChanged();

private:
    Q_DISABLE_COPY(QTdMessageVideoNote)
    QScopedPointer<QTdVideoNote> m_videoNote;
    bool m_isViewed;
    bool m_isSecret;
};

#endif // QTDMESSAGEVIDEONOTE_H
