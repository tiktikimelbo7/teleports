import QtQuick 2.9
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Content 1.1
import "../actions"

Page {

    id: picker

    Component {
        id: resultComponent
        ContentItem {}
    }
    header:UITK.PageHeader{
        title: i18n.tr("Content Picker")
        leadingActionBar.actions:[
            BackAction {
                id: backAction
                enableShortcut: pageStack.currentItem == picker
                onTriggered:{
                    AppActions.view.popFromStack()
                }
            }
        ]
    }

    visible: false
    property var url
    property var handler
    property var contentType
    property var curTransfer

    function __exportItems(url) {
        if (picker.curTransfer.state === ContentTransfer.InProgress)
        {
            picker.curTransfer.items = [ resultComponent.createObject(parent, {"url": url}) ];
            picker.curTransfer.state = ContentTransfer.Charged;
        }
    }

    ContentPeerPicker {
        visible: parent.visible
        contentType: picker.contentType
        handler: picker.handler
        showTitle: false
        onPeerSelected: {
            picker.curTransfer = peer.request();
            if (picker.curTransfer.state === ContentTransfer.InProgress)
                picker.__exportItems(picker.url);
            AppActions.view.popFromStack()
        }
        onCancelPressed: {
            AppActions.view.popFromStack()
        }
    }

    Connections {
        target: picker.curTransfer
        onStateChanged: {
            console.log("curTransfer StateChanged: " + picker.curTransfer.state);
            if (picker.curTransfer.state === ContentTransfer.InProgress) {
                picker.__exportItems(picker.url);
            }
        }
    }
}
