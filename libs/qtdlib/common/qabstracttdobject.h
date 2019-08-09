#ifndef QABSTRACTTDOBJECT_H
#define QABSTRACTTDOBJECT_H

#include <QObject>
#include <QJsonObject>

/**
 * @brief The QTdObject class
 *
 * Abstract base class all tdlib objects should inherit.
 *
 * All subclasses should call setType() during construction to indicate
 * the message type the class represents.
 */
class QTdObject : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QTdObject)
    Q_PROPERTY(Type type READ type NOTIFY typeChanged)
public:
    explicit QTdObject(QObject *parent = nullptr);

    /**
     * @brief Telegram Message Types
     */
    enum Type {
        UNKNOWN = -1, // Uninitialized state
        ANIMATION,
        AUDIO,
        AUTHENTICATION_CODE_INFO,
        AUTHENTICATION_CODE_TYPE_SMS,
        AUTHENTICATION_CODE_TYPE_TELEGRAM_MESSAGE,
        AUTHENTICATION_CODE_TYPE_CALL,
        AUTHENTICATION_CODE_TYPE_FLASH_CALL,
        AUTHORIZATION_STATE_CLOSED,
        AUTHORIZATION_STATE_CLOSING,
        AUTHORIZATION_STATE_LOGGING_OUT,
        AUTHORIZATION_STATE_READY,
        AUTHORIZATION_STATE_WAIT_CODE,
        AUTHORIZATION_STATE_WAIT_ENCRYPTION_KEY,
        AUTHORIZATION_STATE_WAIT_PASSWORD,
        AUTHORIZATION_STATE_WAIT_PHONE_NUMBER,
        AUTHORIZATION_STATE_WAIT_TDLIB_PARAMETERS,
        CALL,
        CALL_DISCARD_REASON,
        CALL_DISCARD_REASON_DECLINED,
        CALL_DISCARD_REASON_DISCONNECTED,
        CALL_DISCARD_REASON_EMPTY,
        CALL_DISCARD_REASON_HUNG_UP,
        CALL_DISCARD_REASON_MISSED,
        CHAT,
        CHAT_ACTION_CANCEL,
        CHAT_ACTION_CHOOSING_CONTACT,
        CHAT_ACTION_CHOOSING_LOCATION,
        CHAT_ACTION_RECORDING_VIDEO,
        CHAT_ACTION_RECORDING_VIDEO_NOTE,
        CHAT_ACTION_RECORDING_VOICE_NOTE,
        CHAT_ACTION_START_PLAYING_GAME,
        CHAT_ACTION_TYPING,
        CHAT_ACTION_UPLOADING_DOCUMENT,
        CHAT_ACTION_UPLOADING_PHOTO,
        CHAT_ACTION_UPLOADING_VIDEO,
        CHAT_ACTION_UPLOADING_VIDEO_NOTE,
        CHAT_ACTION_UPLOADING_VOICE_NOTE,
        CHAT_MEMBER,
        CHAT_MEMBER_STATUS_ADMIN,
        CHAT_MEMBER_STATUS_BANNED,
        CHAT_MEMBER_STATUS_CREATOR,
        CHAT_MEMBER_STATUS_LEFT,
        CHAT_MEMBER_STATUS_MEMBER,
        CHAT_MEMBER_STATUS_RESTRICTED,
        CHAT_PHOTO,
        CHAT_TYPE_PRIVATE,
        CHAT_TYPE_BASIC_GROUP,
        CHAT_TYPE_SUPERGROUP,
        CHAT_TYPE_SECRET,
        CONNECTION_STATE_CONNECTING,
        CONNECTION_STATE_CONNECTING_TO_PROXY,
        CONNECTION_STATE_READY,
        CONNECTION_STATE_UPDATING,
        CONNECTION_STATE_WAITING_FOR_NETWORK,
        DOCUMENT,
        FILE,
        FORMATTED_TEXT,
        INPUT_MESSAGE,
        LINK_STATE_IS_CONTACT,
        LINK_STATE_KNOWS_PHONE_NUMBER,
        LINK_STATE_NONE,
        LOCAL_FILE,
        MESSAGE,
        MESSAGE_ACTION,
        MESSAGE_ANIMATION,
        MESSAGE_UNREAD_LABEL,
        MESSAGE_AUDIO,
        MESSAGE_BASIC_GROUP_CHAT_CREATE,
        MESSAGE_CALL,
        MESSAGE_CHAT_ADD_MEMBERS,
        MESSAGE_CHAT_CHANGE_PHOTO,
        MESSAGE_CHAT_CHANGE_TITLE,
        MESSAGE_CHAT_DELETE_MEMBER,
        MESSAGE_CHAT_DELETE_PHOTO,
        MESSAGE_CHAT_JOIN_BY_LINK,
        MESSAGE_CHAT_SET_TTL,
        MESSAGE_CHAT_UPGRADE_FROM,
        MESSAGE_CHAT_UPGRADE_TO,
        MESSAGE_CUSTOM_SERVICE_ACTION,
        MESSAGE_DATE,
        MESSAGE_DOCUMENT,
        MESSAGE_HIDDEN,
        MESSAGE_LOCATION,
        MESSAGE_TEXT,
        MESSAGE_PHOTO,
        MESSAGE_STICKER,
        MESSAGE_VIDEO,
        MESSAGE_VIDEO_NOTE,
        MESSAGE_VOICE_NOTE,
        MESSAGE_SENDING_STATE_FAILED,
        MESSAGE_SENDING_STATE_PENDING,
        NETWORK_TYPE_MOBILE,
        NETWORK_TYPE_MOBILE_ROAMING,
        NETWORK_TYPE_NONE,
        NETWORK_TYPE_OTHER,
        NETWORK_TYPE_WIFI,
        NOTIFICATION_SETTINGS,
        PHOTO,
        PHOTO_SIZE,
        PROFILE_PHOTO,
        REMOTE_FILE,
        STICKER,
        STICKER_SET_INFO,
        TEXT_ENTITY,
        TEXT_ENTITY_TYPE_MENTION,
        TEXT_ENTITY_TYPE_HASHTAG,
        TEXT_ENTITY_TYPE_BOT_COMMAND,
        TEXT_ENTITY_TYPE_URL,
        TEXT_ENTITY_TYPE_EMAIL_ADDRESS,
        TEXT_ENTITY_TYPE_BOLD,
        TEXT_ENTITY_TYPE_ITALIC,
        TEXT_ENTITY_TYPE_CODE,
        TEXT_ENTITY_TYPE_PRE,
        TEXT_ENTITY_TYPE_PRE_CODE,
        TEXT_ENTITY_TYPE_TEXT_URL,
        TEXT_ENTITY_TYPE_MENTION_NAME,
        TEXT_ENTITY_TYPE_PHONE_NUMBER,
        TEXT_ENTITY_TYPE_CASHTAG,
        USER,
        USER_STATUS_EMPTY,
        USER_STATUS_LAST_MONTH,
        USER_STATUS_LAST_WEEK,
        USER_STATUS_OFFLINE,
        USER_STATUS_ONLINE,
        USER_STATUS_RECENTLY,
        USER_TYPE_BOT,
        USER_TYPE_DELETED,
        USER_TYPE_REGULAR,
        USER_TYPE_UNKNOWN,
        VIDEO,
        VIDEO_NOTE,
        VOICE_NOTE,
        WEB_PAGE,
        REPLY_MARKUP_FORCE_REPLY,
        REPLY_MARKUP_INLINE_KEYBOARD,
        REPLY_MARKUP_REMOVE_KEYBOARD,
        REPLY_MARKUP_SHOW_KEYBOARD,
        MESSAGE_FORWARDED_FROM_USER,
        MESSAGE_FORWARDED_POST,
    };
    Q_ENUM(Type)

    Type type() const;
    QString typeString() const;
    
    virtual void unmarshalJson(const QJsonObject &json);
    virtual QJsonObject marshalJson();

signals:
    void typeChanged(Type type);

protected:
    void setType(const Type type);
    Type m_type;
    QString m_typeString;
};

#endif // QABSTRACTTDOBJECT_H
