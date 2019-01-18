import QtQuick 2.4
import QuickFlux 1.1
import Qt.labs.settings 1.0
import "../actions"

Store {
    readonly property alias theme: s.theme
    readonly property alias uitkTheme: s.uitkTheme

    Settings {
        id: s
        property int theme: 0 // 0 = Suru.Light, 1 = Suru.Dark, 2 = Suru.System
        property var uitkTheme: "Ubuntu.Components.Themes.Ambiance"
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
}
