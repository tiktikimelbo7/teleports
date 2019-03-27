#ifndef QTDKEYBOARDBUTTONTYPE_H
#define QTDKEYBOARDBUTTONTYPE_H

#include "common/qabstracttdobject.h"

/**
 * @brief The QTdKeyboardButtonType class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1_keyboard_button_type.html
 */
class QTdKeyboardButtonType : public QTdObject
{
    Q_OBJECT
public:
    explicit QTdKeyboardButtonType(QObject *parent = nullptr);
};

/**
 * @brief The QTdKeyboardButtonTypeRequestLocation class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1keyboard_button_type_request_location.html
 */
class QTdKeyboardButtonTypeRequestLocation : public QTdKeyboardButtonType
{
    Q_OBJECT
public:
    explicit QTdKeyboardButtonTypeRequestLocation(QObject *parent = nullptr);
};

/**
 * @brief The QTdKeyboardButtonTypeRequestPhoneNumber class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1keyboard_button_type_request_phone_number.html
 */
class QTdKeyboardButtonTypeRequestPhoneNumber : public QTdKeyboardButtonType
{
    Q_OBJECT
public:
    explicit QTdKeyboardButtonTypeRequestPhoneNumber(QObject *parent = nullptr);
};

/**
 * @brief The QTdKeyboardButtonTypeText class
 *
 * https://core.telegram.org/tdlib/docs/classtd_1_1td__api_1_1keyboard_button_type_text.html
 */
class QTdKeyboardButtonTypeText : public QTdKeyboardButtonType
{
    Q_OBJECT
public:
    explicit QTdKeyboardButtonTypeText(QObject *parent = nullptr);
};

#endif // QTDKEYBOARDBUTTONTYPE_H
