#ifndef TOPGAMESLISTMODEL_HH
#define TOPGAMESLISTMODEL_HH
#include "utilityprograms.hh"
#include <QAbstractListModel>

class TopGamesListModel : public QAbstractListModel {
    Q_OBJECT

    public:
        TopGamesListModel(const std::vector<my_program::Game> &games,
                                   QObject *parent = 0);

        // Header:
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;

        // Basic functionality:
        int rowCount(const QModelIndex &parent = QModelIndex()) const;

        QVariant data(const QModelIndex &index,
                      int role /* = Qt::DisplayRole */) const;
        QModelIndex parent(const QModelIndex &child) const;

    private:
        std::vector<my_program::Game> games_;
};

#endif // TOPGAMESLISTMODEL_HH
