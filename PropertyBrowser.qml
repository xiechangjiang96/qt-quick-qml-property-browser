import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels 1.0

TableView {
    id: table_view
    objectName: "table_view"
    anchors.fill: parent
    columnWidthProvider: function (column) {
        if (parent == null)
            return 200
        else
            return parent.width / 2
    }
    rowHeightProvider: function (row) {
        return 30
    }
    clip: true
    delegate: item
    topMargin: header.height
    rowSpacing: 4

    onWidthChanged: {
        this.forceLayout()
    }

    enum ItemType {
        String = 0,
        Int,
        Double,
        Bool,
        Enum,
        None
    }

    Row {
        y: table_view.contentY
        z: 2
        anchors {
            left: parent.left
            right: parent.right
        }

        id: header
        Text {
            width: table_view.columnWidthProvider(0)
            text: "属性"
            font.family: "微软雅黑"
            font.pointSize: 10
        }
        Text {
            width: table_view.columnWidthProvider(1)
            text: "值"
            font.family: "微软雅黑"
            font.pointSize: 10
        }
    }

    Component {
        id: item
        Loader {
            id: loader
            sourceComponent: {
                switch (model.type) {
                case PropertyBrowser.ItemType.String:
                    return item_string
                case PropertyBrowser.ItemType.Int:
                    return item_int
                case PropertyBrowser.ItemType.Double:
                    return item_double
                case PropertyBrowser.ItemType.Bool:
                    return item_bool
                case PropertyBrowser.ItemType.Enum:
                    return item_enum
                default:
                    return item_none
                }
            }
            onLoaded: {
                if (model.type == PropertyBrowser.ItemType.Enum) {
                    var str_arr = model.append.split(",")
                    for (var i = 0; i < str_arr.length; i++) {
                        item.model.append({
                                              "text": str_arr[i]
                                          })
                    }
                }
            }

            Component {
                id: item_none
                TextArea {
                    id: item_none_display
                    // model.value 只能在component对象内才能访问
                    text: model.value
                    font.family: "微软雅黑"
                    font.pointSize: 10
                    readOnly: true
                    background: Rectangle {
                        color: item_none_display.hovered ? "#bcbaba" : "transparent"
                    }
                }
            }

            Component {
                id: item_string
                TextField {
                    id: item_string_field
                    text: model.value
                    font.family: "微软雅黑"
                    font.pointSize: 10
                    // 调试时回触发2次，1次enter，1次因进入ide页面失去焦点
                    onEditingFinished: {
                        model.value = text
                    }
                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 40
                        color: item_string_field.hovered ? "#bcbaba" : "transparent"
                        border.color: "black"
                    }
                }
            }

            Component {
                id: item_int
                TextField {
                    id: item_int_field
                    text: model.value
                    font.family: "微软雅黑"
                    font.pointSize: 10
                    validator: IntValidator {}
                    // 只有acceptable才会触发
                    onEditingFinished: {
                        model.value = text
                    }
                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 40
                        color: item_int_field.hovered ? "#bcbaba" : "transparent"
                        border.color: "black"
                    }
                }
            }

            Component {
                id: item_double
                TextField {
                    id: item_double_field
                    text: model.value
                    font.family: "微软雅黑"
                    font.pointSize: 10
                    validator: DoubleValidator {}
                    // 只有acceptable才会触发
                    onEditingFinished: {
                        model.value = text
                    }
                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 40
                        color: item_double_field.hovered ? "#bcbaba" : "transparent"
                        border.color: "black"
                    }
                }
            }

            Component {
                id: item_bool
                CheckBox {
                    id: item_bool_checkbox
                    checked: {
                        if (model.value != false)
                            return true
                        else
                            return false
                    }
                    onClicked: {
                        model.value = checked
                    }
                    indicator: Rectangle {
                        implicitWidth: 26
                        implicitHeight: 26
                        anchors.centerIn: parent
                        radius: 3
                        color: item_bool_checkbox.hovered ? "#bcbaba" : "transparent"
                        border.color: "black"

                        Rectangle {
                            width: 14
                            height: 14
                            x: 6
                            y: 6
                            radius: 2
                            color: "black"
                            visible: item_bool_checkbox.checked
                        }
                    }
                }
            }

            Component {
                id: item_enum
                ComboBox {
                    model: ListModel {
                        id: item_enum_combo
                    }
                    currentIndex: value
                    onActivated: {
                        value = index
                    }
                }
            }
        }
    }
}
