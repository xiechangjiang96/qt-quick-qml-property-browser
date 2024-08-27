#ifndef PB_MODEL_H
#define PB_MODEL_H
#include <QAbstractTableModel>
#include <QObject>
enum ItemType
{
    String = 0,
    Int,
    Double,
    Bool,
    Enum,
    None
};

struct Item
{
    QVariant value;
    QString save;
    ItemType type;
    QString append;
    Item(QVariant v, QString s, ItemType t)
    {
        value = v;
        save = s;
        type = t;
    }
    Item(QVariant v, QString s, ItemType t, QString a)
    {
        value = v;
        save = s;
        type = t;
        append = a;
    }
};

struct ItemInfo
{
    QString display;
    QString save;
    ItemType type = String;
    QString append;
    ItemInfo(QString d, QString s, ItemType t)
    {
        display = d;
        save = s;
        type = t;
    }
    ItemInfo(QString d, QString s, ItemType t, QString a)
    {
        display = d;
        save = s;
        type = t;
        append = a;
    }
};

enum ModelRoles
{
    value = Qt::UserRole + 1,
    type,
    append
};

class PropertyBrowserModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PropertyBrowserModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

public:
    /*
     * @brief 编写浏览器的项，以及顺序
     */
    void setup();

    void loadParam();

    void saveParam();

protected:
    int max_row_ = 0;
    int max_col_ = 0;
    QList<QList<Item>> data_;  ///< row col，表格数据
    QList<ItemInfo> order_;    ///< 从字典里加载参数的顺序
};

#endif  // PB_MODEL_H
