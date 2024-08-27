#include "pb_model.h"

#include <QDebug>

PropertyBrowserModel::PropertyBrowserModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    setup();
}

QVariant PropertyBrowserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() < 0 || index.row() > max_row_ || index.column() < 0 ||
        index.column() > max_col_)
    {
        return QVariant();
    }

    if (role == value)
    {
        return data_[index.row()][index.column()].value;
    }
    else if (role == type)
    {
        return data_[index.row()][index.column()].type;
    }
    else if (role == append)
    {
        return data_[index.row()][index.column()].append;
    }
    else
    {
        qWarning() << "未使用的role";
        return QVariant();
    }
}

bool PropertyBrowserModel::setData(const QModelIndex &index,
                                   const QVariant &value, int role)
{
    if (!index.isValid())
    {
        return false;
    }

    if (index.row() < 0 || index.row() > max_row_ || index.column() < 0 ||
        index.column() > max_col_)
    {
        return false;
    }

    if (role == ModelRoles::value)
    {
        data_[index.row()][index.column()].value = value.toString();
    }

    saveParam();

    return true;
}

Qt::ItemFlags PropertyBrowserModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

int PropertyBrowserModel::rowCount(const QModelIndex &parent) const
{
    return max_row_;
}

int PropertyBrowserModel::columnCount(const QModelIndex &parent) const
{
    return max_col_;
}

QHash<int, QByteArray> PropertyBrowserModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[value] = "value";
    roles[type] = "type";
    roles[append] = "append";
    return roles;
}

void PropertyBrowserModel::setup()
{
    order_.append(ItemInfo("string", "string", String));
    order_.append(ItemInfo("int", "int", Int));
    order_.append(ItemInfo("double", "double", Double));
    order_.append(ItemInfo("bool", "bool", Bool));
    order_.append(ItemInfo("enum", "enum", Enum, "enum1,enum2"));
}

void PropertyBrowserModel::loadParam()
{
    QMap<QString, QVariant> dict = {
        {"string", "this is str"},
        {"int", 1},
        {"double", 3.14},
        {"bool", true},
        {"enum", 1},
    };

    for (int i = 0; i < order_.size(); ++i)
    {
        auto &order = order_[i];
        if (dict.contains(order.save) == false)
        {
            dict[order.save] = 0;
        }
        QList<Item> tmp;
        tmp.append(Item(order.display, order.save, None));
        if (order.type == Enum)
        {
            tmp.append(Item(dict.value(order.save), order.save, order.type,
                            order.append));
        }
        else
        {
            tmp.append(Item(dict.value(order.save), order.save, order.type));
        }
        data_.append(tmp);
    }

    max_col_ = 2;
    max_row_ = data_.size();

    return;
}

void PropertyBrowserModel::saveParam() {}
