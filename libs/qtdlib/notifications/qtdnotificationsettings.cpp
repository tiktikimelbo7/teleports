#include "qtdnotificationsettings.h"

QTdNotificationSettings::QTdNotificationSettings(QObject *parent)
    : QTdObject(parent)
    , m_muteFor(0)
    , m_showPreview(false)
{
    setType(NOTIFICATION_SETTINGS);
}

qint32 QTdNotificationSettings::muteFor() const
{
    return m_muteFor;
}

bool QTdNotificationSettings::showPreview() const
{
    return m_showPreview;
}

void QTdNotificationSettings::unmarshalJson(const QJsonObject &json)
{
    auto muteFor = json["mute_for"].toInt();
    if (m_muteFor != muteFor) {
        m_muteFor = muteFor;
        emit muteForChanged(m_muteFor);
    }

    auto showPreview = json["show_preview"].toBool();
    if (m_showPreview != showPreview) {
        m_showPreview = showPreview;
        emit showPreviewChanged(m_showPreview);
    }

    QTdObject::unmarshalJson(json);
}
