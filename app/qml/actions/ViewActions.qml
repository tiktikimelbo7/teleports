import QtQuick 2.9
import QuickFlux 1.1
import "./"

ActionCreator {
    /**
     * Show error in popup dialog
     *
     * title: title of dialog
     * description: description of error
     * callback: the ActionKey to call on closing of the dialog
     */
    signal showError(string title, string description, string callback)
}
