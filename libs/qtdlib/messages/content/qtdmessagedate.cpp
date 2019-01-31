#include "qtdmessagedate.h"

QTdMessageDate::QTdMessageDate(QObject *parent) : QTdMessageContent(parent),
    m_date(0)
{
    setType(MESSAGE_DATE);
}

QDateTime QTdMessageDate::date() const
{
    return QDateTime::fromTime_t(m_date);
}

QString QTdMessageDate::dateString() const
{
    return date().toString("MMMM dd");
}

void QTdMessageDate::setDate(const qint32 &date)
{
    if (date == m_date) {
        return;
    }
    m_date = date;
    emit dataChanged();
}
