#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PointRepere.h"
#include "Equipe.h"
#include <QListWidgetItem>
#include <QInputDialog>
#include <QtMath>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Initialiser la base de données
    initializeDatabase();


    // Exemple d'ajout de points de repère
    PointRepere point1 = {"Point 1", 45.0, -75.0, "Description 1", "Categorie 1"};
    PointRepere point2 = {"Point 2", 46.0, -76.0, "Description 2", "Categorie 2"};
    pointsDeRepere.append(point1);
    pointsDeRepere.append(point2);

    // Créer les tables de base de données
    createTables();

    // Exemple d'ajout d'une équipe
    Equipe equipe1 = {"Equipe 1", {"Participant 1", "Participant 2"}};
    equipes.append(equipe1);

    // Remplir la liste des points de repère
    for (const auto& point : pointsDeRepere) {
        QString itemText = point.nom + " (" + QString::number(point.latitude) + ", " + QString::number(point.longitude) + ")";
        ui->pointsListWidget->addItem(itemText);
    }

    // Connecter le bouton à la fonction d'ajout de point de repère
    connect(ui->ajouterPointRepereButton, &QPushButton::clicked, this, &MainWindow::on_ajouterPointRepereButton_clicked);

    // Connecter le bouton à la fonction de calcul de distance
    connect(ui->calculerDistanceButton, &QPushButton::clicked, this, &MainWindow::on_calculerDistanceButton_clicked);

    // Connecte les boutons retirerPointRepereButton et mettreAJourPointRepereButton à leurs fonctions respectives dans votre constructeur MainWindow
    connect(ui->retirerPointRepereButton, &QPushButton::clicked, this, &MainWindow::on_retirerPointRepereButton_clicked);
    connect(ui->mettreAJourPointRepereButton, &QPushButton::clicked, this, &MainWindow::on_mettreAJourPointRepereButton_clicked);

}

MainWindow::~MainWindow() {

    // Fermer la connexion à la base de données avant de détruire l'objet MainWindow
    database.close();

    delete ui;
}

void MainWindow::on_ajouterPointRepereButton_clicked() {
    // Utiliser un QDialog pour obtenir les informations nécessaires de l'utilisateur
    QString nom = QInputDialog::getText(this, "Ajouter un Point de Repère", "Nom du Point de Repère:");
    double latitude = QInputDialog::getDouble(this, "Ajouter un Point de Repère", "Latitude:");
    double longitude = QInputDialog::getDouble(this, "Ajouter un Point de Repère", "Longitude:");
    QString description = QInputDialog::getText(this, "Ajouter un Point de Repère", "Description:");
    QString categorie = QInputDialog::getText(this, "Ajouter un Point de Repère", "Catégorie:");

    // Ajouter le nouveau point de repère
    PointRepere nouveauPoint = {nom, latitude, longitude, description, categorie};
    pointsDeRepere.append(nouveauPoint);

    // Ajouter le point de repère à la base de données
    insertPointIntoDatabase(nouveauPoint);

    // Mettre à jour l'affichage
    QString itemText = nouveauPoint.nom + " (" + QString::number(nouveauPoint.latitude) + ", " + QString::number(nouveauPoint.longitude) + ")";
    ui->pointsListWidget->addItem(itemText);
}

void MainWindow::on_calculerDistanceButton_clicked() {
    // Utilisez un QDialog pour obtenir les noms des deux points de repère entre lesquels calculer la distance
    QString point1Nom = QInputDialog::getText(this, "Calcul de Distance", "Nom du premier Point de Repère:");
    QString point2Nom = QInputDialog::getText(this, "Calcul de Distance", "Nom du deuxième Point de Repère:");

    // Recherchez les points de repère correspondants aux noms
    PointRepere point1, point2;
    bool foundPoint1 = false, foundPoint2 = false;

    for (const auto& point : pointsDeRepere) {
        if (point.nom == point1Nom) {
            point1 = point;
            foundPoint1 = true;
        } else if (point.nom == point2Nom) {
            point2 = point;
            foundPoint2 = true;
        }

        if (foundPoint1 && foundPoint2) {
            break;
        }
    }

    // Vérifiez si les deux points ont été trouvés
    if (foundPoint1 && foundPoint2) {
        // Calculez la distance entre les deux points (utilisation de la formule de Haversine pour une sphère)
        double dLat = qDegreesToRadians(point2.latitude - point1.latitude);
        double dLon = qDegreesToRadians(point2.longitude - point1.longitude);
        double lat1 = qDegreesToRadians(point1.latitude);
        double lat2 = qDegreesToRadians(point2.latitude);

        double a = qSin(dLat / 2) * qSin(dLat / 2) +
                   qSin(dLon / 2) * qSin(dLon / 2) * qCos(lat1) * qCos(lat2);
        double c = 2 * qAtan2(qSqrt(a), qSqrt(1 - a));
        double distance = 6371000 * c;  // Rayon moyen de la Terre en mètres

        // Affichez la distance
        QString distanceText = "Distance entre " + point1.nom + " et " + point2.nom + ": " + QString::number(distance) + " mètres";
                               QMessageBox::information(this, "Calcul de Distance", distanceText);
        } else {
            // Affichez un message d'erreur si l'un des points n'a pas été trouvé
            QMessageBox::warning(this, "Erreur", "Au moins l'un des points de repère n'a pas été trouvé.");
        }
}



// Ajoutez ces définitions de fonction à votre classe MainWindow dans le fichier mainwindow.cpp

void MainWindow::on_retirerPointRepereButton_clicked() {
    // Utiliser un QDialog pour obtenir le nom du point de repère à retirer
    QString pointNom = QInputDialog::getText(this, "Retirer un Point de Repère", "Nom du Point de Repère à retirer:");

    // Rechercher le point de repère correspondant au nom
    for (auto it = pointsDeRepere.begin(); it != pointsDeRepere.end(); ++it) {
        if (it->nom == pointNom) {
            // Retirez le point de repère de la liste
            it = pointsDeRepere.erase(it);

            //supprimer de la base de donner
            deletePointFromDatabase(pointNom);

            // Mettez à jour l'affichage
            updatePointsListWidget();
            return;
        }
    }

    // Afficher un message d'erreur si le point de repère n'a pas été trouvé
    QMessageBox::warning(this, "Erreur", "Le point de repère spécifié n'a pas été trouvé.");
}

void MainWindow::on_mettreAJourPointRepereButton_clicked() {
    // Utiliser un QDialog pour obtenir le nom du point de repère à mettre à jour
    QString pointNom = QInputDialog::getText(this, "Mettre à Jour un Point de Repère", "Nom du Point de Repère à mettre à jour:");

    // Rechercher le point de repère correspondant au nom
    for (auto& point : pointsDeRepere) {
        if (point.nom == pointNom) {
            // Utiliser un QDialog pour obtenir les nouvelles informations du point de repère
            QString nouveauNom = QInputDialog::getText(this, "Mettre à Jour un Point de Repère", "Nouveau Nom du Point de Repère:", QLineEdit::Normal, point.nom);
            double nouvelleLatitude = QInputDialog::getDouble(this, "Mettre à Jour un Point de Repère", "Nouvelle Latitude:", point.latitude);
            double nouvelleLongitude = QInputDialog::getDouble(this, "Mettre à Jour un Point de Repère", "Nouvelle Longitude:", point.longitude);
            QString nouvelleDescription = QInputDialog::getText(this, "Mettre à Jour un Point de Repère", "Nouvelle Description:", QLineEdit::Normal, point.description);
            QString nouvelleCategorie = QInputDialog::getText(this, "Mettre à Jour un Point de Repère", "Nouvelle Catégorie:", QLineEdit::Normal, point.categorie);

            // Metter à jour les informations du point de repère
            point.nom = nouveauNom;
            point.latitude = nouvelleLatitude;
            point.longitude = nouvelleLongitude;
            point.description = nouvelleDescription;
            point.categorie = nouvelleCategorie;

            updatePointInDatabase(point);

            // Metter à jour l'affichage
            updatePointsListWidget();
            return;
        }
    }

    // Afficher un message d'erreur si le point de repère n'a pas été trouvé
    QMessageBox::warning(this, "Erreur", "Le point de repère spécifié n'a pas été trouvé.");
}

void MainWindow::updatePointsListWidget() {
    // Effacer la liste actuelle des points de repère
    ui->pointsListWidget->clear();

    // Remplir la liste des points de repère mise à jour
    for (const auto& point : pointsDeRepere) {
        QString itemText = point.nom + " (" + QString::number(point.latitude) + ", " + QString::number(point.longitude) + ")";
        ui->pointsListWidget->addItem(itemText);
    }
}

void MainWindow::initializeDatabase() {
    // Initialiser la base de données
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(QCoreApplication::applicationDirPath() +"cartographie_exercice.db"); // Remplacez par le nom de votre base de données

    // Ouvrir la connexion à la base de données
    if (!database.open()) {
        qDebug() << "Impossible d'ouvrir la base de données.";
    }
}

void MainWindow::createTables() {
    // Créer la table pour les points de repère
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS PointsDeRepere ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "nom TEXT,"
               "latitude REAL,"
               "longitude REAL,"
               "description TEXT,"
               "categorie TEXT"
               ");");

    // Créer la table pour les équipes (ajoutez d'autres champs au besoin)
    query.exec("CREATE TABLE IF NOT EXISTS Equipes ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "nom TEXT,"
               "participants TEXT"
               ");");
}

void MainWindow::insertPointIntoDatabase(const PointRepere &point) {
    QSqlQuery query;
    query.prepare("INSERT INTO PointsDeRepere (nom, latitude, longitude, description, categorie) "
                  "VALUES (:nom, :latitude, :longitude, :description, :categorie)");
    query.bindValue(":nom", point.nom);
    query.bindValue(":latitude", point.latitude);
    query.bindValue(":longitude", point.longitude);
    query.bindValue(":description", point.description);
    query.bindValue(":categorie", point.categorie);

    if (query.exec()) {
        qDebug() << "Point inséré dans la base de données avec succès.";
    } else {
        qDebug() << "Erreur lors de l'insertion du point dans la base de données:" << query.lastError().text();
    }
}



void MainWindow::updatePointInDatabase(const PointRepere &point) {
    QSqlQuery query;
    query.prepare("UPDATE Points SET nom = ?, latitude = ?, longitude = ?, description = ?, categorie = ? WHERE nom = ?");
    query.bindValue(0, point.nom);
    query.bindValue(1, point.latitude);
    query.bindValue(2, point.longitude);
    query.bindValue(3, point.description);
    query.bindValue(4, point.categorie);
    query.bindValue(5, point.nom);

    if (query.exec()) {
        qDebug() << "Point mis à jour avec succès dans la base de données.";
    } else {
        qDebug() << "Erreur lors de la mise à jour du point :" << query.lastError().text();
    }
}

void MainWindow::deletePointFromDatabase(const QString& pointName) {
    QSqlQuery query;
    query.prepare("DELETE FROM Points WHERE nom = ?");
    query.bindValue(0, pointName);

    if (query.exec()) {
        qDebug() << "Point supprimé avec succès de la base de données.";
    } else {
        qDebug() << "Erreur lors de la suppression du point :" << query.lastError().text();
    }
}
