import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    PropertyBrowser {
        id: property_browser
        objectName: "property_browser"
        anchors.fill: parent
    }
}
