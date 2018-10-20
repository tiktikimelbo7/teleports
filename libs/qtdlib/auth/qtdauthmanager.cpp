#include "qtdauthmanager.h"
#include <QDebug>
#include "client/qtdclient.h"
#include "requests/qtdauthencryptionkeyresponse.h"
#include "requests/qtdauthparametersresponse.h"
#include "requests/qtdauthphonenumberresponse.h"
#include "requests/qtdauthcoderesponse.h"
#include "requests/qtdauthpasswordresponse.h"

QTdAuthManager::QTdAuthManager(QObject *parent) : QObject(parent),
    m_state(Invalid),
    m_params(0)
{
    connect(QTdClient::instance(),
            &QTdClient::authStateChanged,
            this,
            &QTdAuthManager::handleAuthStateChanged);

    // Manually run the update now as the client may already be initialized
    // so we want the *current* state to set us up correctly.
    if (QTdClient::instance()->authState()) {
        handleAuthStateChanged(QTdClient::instance()->authState());
    }
}

QTdAuthManager::State QTdAuthManager::state() const
{
    return m_state;
}

QTdAuthState *QTdAuthManager::type() const
{
    return QTdClient::instance()->authState();
}

QTdAuthParams *QTdAuthManager::params() const
{
    return m_params;
}

void QTdAuthManager::setParams(QTdAuthParams *params)
{
    if (m_params == params)
        return;

    m_params = params;
    emit paramsChanged(m_params);
}

void QTdAuthManager::sendParams()
{
    if (!m_params) {
        qWarning() << "Missing auth params. aborting...";
        return;
    }

    if (m_state != WaitTdParams) {
        qWarning() << "TDLib isn't waiting for this so not sending!";
        return;
    }
    auto *resp = new QTdAuthParametersResponse;
    resp->setParameters(m_params);
    QTdClient::instance()->send(resp);
}

void QTdAuthManager::setEncryptionKey(const QString &key)
{
    if (m_state != WaitEncryptionKey) {
        qWarning() << "TDLib isn't waiting for the encryption key";
        return;
    }
    auto *resp = new QTdAuthEncryptionKeyResponse;
    resp->setKey(key);
    QTdClient::instance()->send(resp);
}

void QTdAuthManager::sendPhoneNumber(const QString &number)
{
    if (m_state != WaitPhoneNumber) {
        qWarning() << "TDLib isn't waiting for the phone number";
        return;
    }
    auto *resp = new QTdAuthPhoneNumberResponse;
    resp->setPhoneNumber(number);
    QTdClient::instance()->send(resp);
}

void QTdAuthManager::sendCode(const QString &code, const QString &firstname, const QString &lastname)
{
    if (m_state != WaitCode) {
        qWarning() << "TDLib isn't waiting for a code";
        return;
    }
    auto *resp = new QTdAuthCodeResponse;
    resp->setCode(code);
    resp->setFirstName(firstname);
    resp->setLastName(lastname);
    QTdClient::instance()->send(resp);
}

void QTdAuthManager::sendPassword(const QString &password)
{
    if (m_state != WaitPassword) {
        qWarning() << "TDLib isn't waiting for a password";
        return;
    }
    auto *resp = new QTdAuthPasswordResponse;
    resp->setPassword(password);
    QTdClient::instance()->send(resp);
}

void QTdAuthManager::handleAuthStateChanged(QTdAuthState *state)
{
    switch (state->type()) {
    case QTdAuthState::Type::AUTHORIZATION_STATE_WAIT_TDLIB_PARAMETERS:
    {
        m_state = WaitTdParams;
        emit waitingForTdParams();
        break;
    }
    case QTdAuthState::Type::AUTHORIZATION_STATE_WAIT_ENCRYPTION_KEY:
    {
        m_state = WaitEncryptionKey;
        emit waitingForEncryptionKey();
        break;
    }
    case QTdAuthState::Type::AUTHORIZATION_STATE_WAIT_PHONE_NUMBER:
    {
        m_state = WaitPhoneNumber;
        emit waitingForPhoneNumber();
        break;
    }
    case QTdAuthState::Type::AUTHORIZATION_STATE_WAIT_CODE:
    {
        auto currentState = (QTdAuthStateWaitCode*) state;
        if (currentState->isRegistered())
        {
            emit waitingForCode();
        }
        else
        {
            emit waitingForUserProfile();
        }
        m_state = WaitCode;
        break;
    }
    case QTdAuthState::Type::AUTHORIZATION_STATE_WAIT_PASSWORD:
    {
        m_state = WaitPassword;
        emit waitingForPassword();
        break;
    }
    case QTdAuthState::Type::AUTHORIZATION_STATE_READY:
    {
        m_state = Ready;
        emit ready();
        break;
    }
    case QTdAuthState::Type::AUTHORIZATION_STATE_LOGGING_OUT:
    {
        m_state = LoggingOut;
        emit loggingOut();
        break;
    }
    case QTdAuthState::Type::AUTHORIZATION_STATE_CLOSING:
    {
        m_state = Closing;
        emit closing();
        break;
    }
    case QTdAuthState::Type::AUTHORIZATION_STATE_CLOSED:
    {
        m_state = Closed;
        emit closed();
        break;
    }
    default:
        return;
    }
    emit stateChanged(m_state);
}
