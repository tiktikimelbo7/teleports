import QtQuick 2.9
import QtTest 1.0
import QuickFlux 1.1
import Teleports.Testing 1.0
import actions 0.1

Item {
    id: root
    width: 400
    height: width

    AppListener{id: listener}

    TestCase {
        name: "AuthAction"

        function init() {
            TestRunner.init()
        }

        function test_set_code() {
            var fname = ""
            var lname = ""
            var code = ""
            listener.on(AuthKey.setCode, function (message){
                fname = message.firstname
                lname = message.lastname
                code = message.code
            })
            AppActions.auth.setCode("12345", "first", "last")
            compare(fname, "first")
            compare(lname, "last")
            compare(code, "12345")
        }

        function test_auth_code_accepted() {
            var accepted = false
            listener.on(AuthKey.authCodeAccepted, function(message){
                accepted = true
            })
            AppActions.auth.authCodeAccepted()
            compare(accepted, true)
        }

        function test_auth_code_error() {
            var error = ""
            listener.on(AuthKey.authCodeError, function (message) {
                error = message.error
            })
            AppActions.auth.authCodeError("error")
            compare(error, "error")
        }

        function test_set_password() {
            var password = ""
            listener.on(AuthKey.setPassword, function (message) {
                password = message.password
            })
            AppActions.auth.setPassword("pwd")
            compare(password, "pwd")
        }

        function test_auth_password_accepted() {
            var accepted = false
            listener.on(AuthKey.authPasswordAccepted, function(message){
                accepted = true
            })
            AppActions.auth.authPasswordAccepted()
            compare(accepted, true)
        }

        function test_auth_password_error() {
            var error = ""
            listener.on(AuthKey.authPasswordError, function (message) {
                error = message.error
            })
            AppActions.auth.authPasswordError("error")
            compare(error, "error")
        }

        function test_logout() {
            var seen = false
            listener.on(AuthKey.logOut, function(message){
                seen = true
            })
            AppActions.auth.logOut()
            compare(seen, true)
        }

        function test_delete_account() {
            var seen = false
            listener.on(AuthKey.deleteAccount, function(message){
                seen = true
            })
            AppActions.auth.deleteAccount()
            compare(seen, true)
        }
    }
}
