#ifndef QTDAUTHCODERESPONSE_H
#define QTDAUTHCODERESPONSE_H

#include <QObject>
#include "common/qtdrequest.h"

/**
 * @brief The QTdAuthCodeResponse class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1check_authentication_code.html
 */
class QTdAuthCodeResponse : public QTdOkRequest
{
    Q_OBJECT
    Q_DISABLE_COPY(QTdAuthCodeResponse)
public:
    explicit QTdAuthCodeResponse(QObject *parent = nullptr);

    /**
     * @brief setCode
     * @param code
     *
     * The verification code received via SMS, Telegram or call
     */
    void setCode(const QString &code);

    /**
     * @brief setFirstName
     * @param firstname
     *
     * If user is not registered; the first name of the user
     *
     * no more than 255 characters
     */
    void setFirstName(const QString &firstname);

    /**
     * @brief setLastName
     * @param lastname
     *
     * If user is not registered; the last name of the user
     *
     * no more than 255 characters
     */
    void setLastName(const QString &lastname);

    /**
     * @brief marshalJson
     * @return QJsonObject of the request suitable for sending to tdlib
     */
    QJsonObject marshalJson() Q_DECL_FINAL;

private:
    QString m_code;
    QString m_firstname;
    QString m_lastname;
};

#endif // QTDAUTHCODERESPONSE_H
