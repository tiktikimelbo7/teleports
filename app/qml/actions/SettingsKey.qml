pragma Singleton
import QtQuick 2.4
import QuickFlux 1.1

KeyTable {
    property string setTheme

    /**
     * Callback key to dispatch after prompting
     * about No U1 account during push registration
     */
    property string u1dialogShown

    property string viewConnectivity
    property string viewUserList
}
