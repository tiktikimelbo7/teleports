import QtQuick 2.4
import QuickFlux 1.1
import QTelegram 1.0
import "../actions"

Store {

    property alias type: authState.type
    property alias state: authState.state
    property alias params: authParams

    AuthState {
        id: authState
        params: AuthParams {
            id: authParams
            apiID: 202381
            apiHash: "02c503ab3bdaf7b912d0640d4c6b1faf"
            useFileDatabase: true
            useChatInfoDatabase: true
            useMessageDatabase: true
            useSecretChats: true
            useStorageOptimizer: true
            useTestDC: false
        }

        onWaitingForTdParams: {
            sendParams()
        }

        onWaitingForEncryptionKey: {
            // NOTE: Once a key has been set it should NEVER change
            // as it is used to encrypt the local DB. We could possibly
            // use QtKeyChain to store/retrieve from gnome-keyring.
            // AuthState does not store the key nor keep it in memory
            // longer than the length of the response cycle.
            // So finding something safer than QSettings is a must.
            setEncryptionKey("")
        }

        onWaitingForPhoneNumber: {
            AppDispatcher.dispatch("replaceOnStack", {view: "qrc:/pages/WaitPhoneNumberPage.qml"})
        }

        onWaitingForPassword: {
            AppDispatcher.dispatch("replaceOnStack", {view: "qrc:/pages/WaitPasswordPage.qml", properties: {"hint": passwordHint}})
        }

        onWaitingForCode: {
            AppDispatcher.dispatch("replaceOnStack", {view: "qrc:/pages/WaitCodePage.qml"})
        }
        onWaitingForRegistration: {
            AppDispatcher.dispatch("replaceOnStack", {view: "qrc:/pages/WaitRegistrationPage.qml"})
        }

        onReady: {
            AppDispatcher.dispatch("replaceOnStack", {view: "qrc:/pages/ChatListPage.qml"})
        }
        onClosed: {
              Qt.quit();
        }
        onPhoneNumberError: {
            if (message == "PHONE_NUMBER_INVALID") {
                AppActions.auth.authPhoneNumberError(i18n.tr("Invalid phone number!"));
            } else {
                AppActions.auth.authPhoneNumberError(message);
            }
        }
        onCodeError: {
            if (message == "PHONE_CODE_INVALID") {
                AppActions.auth.authCodeError(i18n.tr("Invalid code!"), false);
            } else {
                AppActions.auth.authCodeError(message, false);
            }
        }
        onPasswordError: {
            if (message == "PASSWORD_HASH_INVALID") {
                AppActions.auth.authPasswordError(i18n.tr("Invalid password!"));
            } else {
                AppActions.auth.authPasswordError(message);
            }
        }
    }

    Filter {
        type: AuthKey.setPhoneNumber
        onDispatched: {
            var number = message.dialcode.trim() + message.phonenumber.trim()

            authState.sendPhoneNumber(number)
        }
    }

    Filter {
        type: AuthKey.setCode
        onDispatched: {
            if (authState.state !== AuthState.WaitCode) {
                AppActions.auth.authCodeError(i18n.tr('Auth code not expected right now'), false)
                console.log("setCode: Auth code not expected right now")
                return
            }
            var info = authState.type.info;
            if (!info) {
                AppActions.auth.authCodeError(i18n.tr('Oops! Internal error.'), false)
                console.log("setCode: Oops! Internal error.")
                return
            }

/*             if (message.code.length !== parseInt(info.type.length)) {
                AppActions.auth.authCodeError(i18n.tr('Incorrect auth code length.'), true);
                console.log("setCode: Incorrect auth code length.")
                return;
            } */
            authState.sendCode(message.code)
            AppActions.auth.authCodeAccepted()
        }
    }

    Filter {
        type: AuthKey.registerUser
        onDispatched: {
            if (authState.state !== AuthState.WaitRegistration) {
                AppActions.auth.authCodeError(i18n.tr('Registration not expected right now'), false)
                console.log("registerUser: Registration not expected right now")
                return
            }
            // var info = authState.type.info;
            // if (!info) {
            //     AppActions.auth.authCodeError(i18n.tr('Oops! Internal error.'), false)
            //     console.log("setCode: Oops! Internal error.")
            //     return
            // }
            authState.registerUser(message.firstname, message.lastname)
            AppActions.auth.authRegistrationAccepted()
        }
    }

    Filter {
        type: AuthKey.setPassword
        onDispatched: {
            if (authState.state !== AuthState.WaitPassword) {
                AppActions.auth.authPasswordError("Password not expected right now")
                return
            }
            authState.sendPassword(message.password)
        }
    }

    Filter {
        type: AuthKey.logOut
        onDispatched: {
            authState.logOut()
        }
    }

    Filter {
        type: AuthKey.deleteAccount

        onDispatched: {
            // TODO: Ask for reason - optional
            authState.deleteAccount("");
        }
    }
}
