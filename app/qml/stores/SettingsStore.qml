import QtQuick 2.4
import QuickFlux 1.1
import Qt.labs.settings 1.0
import "../actions"

Store {
    readonly property alias theme: s.theme
    readonly property alias uitkTheme: s.uitkTheme
    readonly property alias u1DialogShown: s.onceDialog
    readonly property alias indicators: s.indicators

    Settings {
        id: s
        property int theme: 0 // 0 = Suru.Light, 1 = Suru.Dark, 2 = Suru.System
        property var uitkTheme: "Ubuntu.Components.Themes.Ambiance"
        // Dialog shown for U1 account needed for push notifications
        property bool onceDialog: false
        property bool indicators: false
    }

    Filter {
        type: SettingsKey.setTheme
        onDispatched: {
            s.theme = message.theme != s.theme ? message.theme : s.theme
            switch(theme) {
                case 0: s.uitkTheme = "Ubuntu.Components.Themes.Ambiance"; break;
                case 1: s.uitkTheme = "Ubuntu.Components.Themes.SuruDark"; break;
            }
        }
    }

    Filter {
        type: SettingsKey.u1dialogShown
        onDispatched: {
            if (!s.onceDialog) {
                s.onceDialog = true
            }
        }
    }

    Filter {
        type: SettingsKey.toggleIndicators
        onDispatched: {
            switch(message.indicators) {
                case true: s.indicators = true; break;
                case false: s.indicators = false; break;
                default: s.indicators = !s.indicators;
            }
        }
    }
}
