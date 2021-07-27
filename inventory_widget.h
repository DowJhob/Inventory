#ifndef INVENTORY_WIDGET_H
#define INVENTORY_WIDGET_H

#include <QTableWidget>
#include <QObject>
#include <QDebug>

#include <QMouseEvent>
#include "inventoryitem.h"
#include <QSound>

class inventoryWidget : public QTableWidget
{
    Q_OBJECT
public:
    inventoryWidget(QTableWidget *parent = nullptr)
    {
        setParent( parent);
        setEnabled(false);
    }

    void mousePressEvent(QMouseEvent * event)
    {
        if(event->button() == Qt::RightButton)
        {
            QModelIndex selected_model_index = indexAt(event->pos());
            QAbstractItemModel *_model = qobject_cast<QAbstractItemModel*>(model());
            if(_model) {
                if(item(selected_model_index.row(), selected_model_index.column()) != nullptr)
                {
                    int count = selected_model_index.data(Qt::DisplayRole).toInt();
                    itemType type = (itemType)selected_model_index.data(Qt::UserRole).toInt();
                    if(count <= 1)
                        takeItem(selected_model_index.row(), selected_model_index.column());
                    else
                        _model->setData( selected_model_index,  --count, Qt::DisplayRole);
                    emit itemDestroy_sig(type);
                }
            }
            return;                                     // Это что бы не подтупливать
        }
        QTableWidget::mousePressEvent(event);
    }
    void mouseReleaseEvent (QMouseEvent * event )
    {
        QTableWidget::mouseReleaseEvent(event);
    }
    void dragEnterEvent(QDragEnterEvent *event)
    {
        local_selected_index = indexAt(event->pos());
        if (event->source() == this) {
            setDragDropMode(QAbstractItemView::InternalMove);
        } else {
            setDragDropMode(QAbstractItemView::DragDrop);
        }
        QAbstractItemView::dragEnterEvent(event);
    }
    void dropEvent(QDropEvent *event)
    {
        QModelIndex target_model_index = indexAt(event->pos());
        QAbstractItemModel *_model = qobject_cast<QAbstractItemModel*>(model());
        int type;
        int count;
        if(_model)
        {
            if( item(target_model_index.row(), target_model_index.column()) != nullptr )
            {
                type = target_model_index.data(Qt::UserRole).toInt();
                if ( local_selected_index.data(Qt::UserRole).toInt() == type )
                {
                    count = target_model_index.data(Qt::DisplayRole).toInt();  // Сохраним сумму
                    QTableWidget::dropEvent(event);                             // Пусть за нас все скопирует кьют
                    _model->setData( target_model_index, target_model_index.data(Qt::DisplayRole).toInt() + count, Qt::DisplayRole); //поправим сумму
                }
            }
            else
                QTableWidget::dropEvent(event);
        }
    }

    QByteArray serializeToByteArray()
    {
        QByteArray a;
        char type;
        int count;
        for(int x = 0; x < columnCount(); ++x)
            for(int y = 0; y < rowCount(); ++y)
            {
                QTableWidgetItem *it = item(y, x);
                if(it != nullptr)
                {
                    type = it->data(Qt::UserRole).toInt();
                    count = it->data(Qt::DisplayRole).toInt();
                }
                else
                {
                    type = (int)itemType::black_hole;
                    count = 0;
                }
                a.append(type);
                a.append(count);
            }

        return a;
    }

    void deserializeFromByteArray(QByteArray inv)
    {
        clear();
        int it_it = 0;
        for(int x = 0; x < columnCount(); ++x)
            for(int y = 0; y < rowCount(); ++y)
            {
                if( (int)inv[it_it] != (int)itemType::black_hole )
                {
                    inventoryItem *it = new inventoryItem( (itemType)(int)inv[it_it++], (int)inv[it_it++] );
                    setItem(y, x, it);
                }
                else
                    it_it += 2;
            }
    }

public slots:

private:
    QModelIndex local_selected_index;

signals:
    void itemDestroy_sig(itemType);
};

#endif // INVENTORY_WIDGET_H
