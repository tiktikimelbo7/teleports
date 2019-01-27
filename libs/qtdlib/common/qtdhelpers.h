#include <QDateTime>
#include <QDebug> 
#include "messages/qtdtextentity.h"

class QTdHelpers {

    static QRegExp rxEntity;
    static QRegExp rxLinebreaks;

public:
    static QString formatDate(const QDateTime &dt);
    static QString avatarColor(unsigned int userId);
    static QString selfColor();
    static void getEntitiesFromMessage(const QString &messageText,
                                       QString &plainText,
                                       QJsonArray &entities);
    static QJsonArray formatPlainTextMessage(const QString &messageText,
                                             QString &plainText);
};
