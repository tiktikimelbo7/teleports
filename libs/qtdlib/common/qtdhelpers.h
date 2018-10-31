#include <QDateTime>
#include <QDebug> 

class QTdHelpers {

public:
    static QString formatDate(const QDateTime &dt);
    static QString avatarColor(unsigned int userId);
    static QString selfColor();
};

