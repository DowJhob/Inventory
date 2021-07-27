#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include <QTableWidgetItem>

// Qt::UserRole - определяет тип

enum class itemType
{
    black_hole,
    apple,
    space_cucumber,
    double_space_cucumber
};


class inventoryItem: public QTableWidgetItem
{
public:
    inventoryItem(itemType it = itemType::black_hole, int count = 1)
    {
        QString ssh;
        switch (it) {
        case itemType::apple                 : ssh = ":/apple.png"; break;
        case itemType::space_cucumber        : ssh = ":/space_cucumber.jpg"; break;
        case itemType::double_space_cucumber : ssh = ":/double_space_cucumber.jpg"; break;
        default                              : ssh = ":/nothing.jpg";
        }
        setIcon(QIcon(ssh));
        setTextAlignment( Qt::AlignRight|Qt::AlignBottom );

        setData(Qt::UserRole, (int)it);
        setData(Qt::DisplayRole, count);
    }

private:

};

#endif // INVENTORYITEM_H
