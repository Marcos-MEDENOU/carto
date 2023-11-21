#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QtSql>
#include "PointRepere.h"
#include "Equipe.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouterPointRepereButton_clicked();
    void on_calculerDistanceButton_clicked();
    void on_retirerPointRepereButton_clicked();
    void on_mettreAJourPointRepereButton_clicked();
    void updatePointsListWidget();
    void insertPointIntoDatabase(const PointRepere &point);
    void updatePointInDatabase(const PointRepere &point);
    void deletePointFromDatabase(const QString& pointName);
private:
    Ui::MainWindow *ui;
    QList<PointRepere> pointsDeRepere;
    QList<Equipe> equipes;

    // Ajout d'un objet QSqlDatabase pour la gestion de la base de données
    QSqlDatabase database;

    // Fonctions pour initialiser et manipuler la base de données
    void initializeDatabase();
    void createTables();
};

#endif // MAINWINDOW_H
