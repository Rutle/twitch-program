#include "topgameslistmodel.hh"
#include <QDebug>

TopGamesListModel::TopGamesListModel(const std::vector<my_program::Game> &games,
                                     QObject *parent)
    : QAbstractListModel(parent) {
    games_ = games;
}

QVariant TopGamesListModel::headerData(int section,
                                       Qt::Orientation orientation,
                                       int role) const {
    if ( role != Qt::DisplayRole ) {
        return QVariant();
    }
    if ( orientation == Qt::Horizontal ) {
        return QString("Column %1").arg(section);
    } else {
        return QString("Row %1").arg(section);
    }
}

int TopGamesListModel::rowCount(const QModelIndex &parent) const {
    return int(games_.size());
}

QVariant TopGamesListModel::data(const QModelIndex &index, int role) const {
    // qDebug() << "Came to TopGamesListModel::data, index.row: " << index.row();
    if ( !index.isValid() ) {
        //qDebug() << "Data, Index: Invalid";
        return QVariant();
    }
    if ( index.row() < 0 || index.row() >= int(games_.size()) ) {
        //qDebug() << "Too high row number.";
        return QVariant();
    }
    if ( role == Qt::DisplayRole ) {
        // Can return QStringList with name, viewers and channels.
        //qDebug() << "DisplayRole: " << role;
        return QVariant(games_.at(index.row()).name);
    }
    if ( role == Qt::DecorationRole ) {
        //qDebug() << "DecorationRole: " << role;
        return QVariant(games_.at(index.row()).logo);
    }
    /*
    if ( role == Qt::DecorationRole ) {
        return QVariant(games_.at(index.row()).logo);
    }
    */
    return QVariant();
}

QModelIndex TopGamesListModel::parent(const QModelIndex &child) const {
    return QModelIndex();

}

