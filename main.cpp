/*
 * main.cpp
 *
 *  Created on: 23 oct. 2026
 *      Author: hP
 */
#include <iostream> 	// afficher à l'écran
#include <string> 		// texte (std::string)
#include <vector>		// liste dynamique
#include <algorithm>	// std::find
#include <stdexcept>	// exceptions

class Filiere;		// "Filiere existe, je la définis plus tard"

class Etudiant {
private:		 // données cachées
    std::string nom; // @suppress("Type cannot be resolved")
    std::string cin; // @suppress("Type cannot be resolved")
    double note1, note2, note3;
    Filiere* filiere;		  // l'étudiant connaît sa filière

public:
    Etudiant();		// constructeur sans paramètre
    Etudiant(std::string n, std::string c, double n1, double n2, double n3); 	// constructeur avec paramètres

    std::string getNom() const;
    std::string getCin() const;
    double getNote1() const;
    double getNote2() const;
    double getNote3() const;
    Filiere* getFiliere() const;

    void setNom(std::string n);
    void setCin(std::string c);
    void setNote1(double n);
    void setNote2(double n);
    void setNote3(double n);
    void setFiliere(Filiere* f);

    double calculerMoyenne() const;
    void changerDeFiliere(Filiere* nouvelleFiliere);
};

//  CLASSE FILIERE
class Filiere {
private:
    std::string nomFiliere;
    std::vector<Etudiant*> etudiants;  // la filière connaît ses étudiants

public:
    Filiere(std::string nom);
    ~Filiere();	 // destructeur

    std::string getNomFiliere() const;
    void inscrireEtudiant(Etudiant* e);
    void retirerEtudiant(Etudiant* e);
    int getNombreEtudiants() const;
    void afficherTous() const;
};

//IMPLEMENTATION ETUDIANT
Etudiant::Etudiant()
    : nom("Inconnu"), cin("000000000000000"),
      note1(0), note2(0), note3(0), filiere(nullptr) {}

Etudiant::Etudiant(std::string n, std::string c, double n1, double n2, double n3)
    : nom(n), cin(c), note1(0), note2(0), note3(0), filiere(nullptr) {
    setNote1(n1);   // on passe par les setters pour valider
    setNote2(n2);
    setNote3(n3);
}

std::string Etudiant::getNom() const { return nom; }
std::string Etudiant::getCin() const { return cin; }
double Etudiant::getNote1() const { return note1; }
double Etudiant::getNote2() const { return note2; }
double Etudiant::getNote3() const { return note3; }
Filiere* Etudiant::getFiliere() const { return filiere; }

void Etudiant::setNom(std::string n) { nom = n; }
void Etudiant::setCin(std::string c) { cin = c; }

void Etudiant::setNote1(double n) {
    if (n >= 0 && n <= 20) note1 = n;
    else std::cerr << "Note invalide : " << n << std::endl;
}
void Etudiant::setNote2(double n) {
    if (n >= 0 && n <= 20) note2 = n;
    else std::cerr << "Note invalide : " << n << std::endl;
}
void Etudiant::setNote3(double n) {
    if (n >= 0 && n <= 20) note3 = n;
    else std::cerr << "Note invalide : " << n << std::endl;
}

void Etudiant::setFiliere(Filiere* f) { filiere = f; }

double Etudiant::calculerMoyenne() const {
    return (note1 + note2 + note3) / 3.0;
}

void Etudiant::changerDeFiliere(Filiere* nouvelleFiliere) {
    if (nouvelleFiliere == nullptr) {
        std::cerr << "Nouvelle filiere invalide." << std::endl;
        return;
    }
    if (filiere != nullptr) {
        filiere->retirerEtudiant(this);   // 1. quitter l'ancienne
    }
    nouvelleFiliere->inscrireEtudiant(this); // 2. rejoindre la nouvelle
    std::cout << nom << " a change de filiere." << std::endl;
}

//IMPLEMENTATION FILIERE
Filiere::Filiere(std::string nom) : nomFiliere(nom) {}

Filiere::~Filiere() {
    for (Etudiant* e : etudiants) {
        delete e;
    }
    etudiants.clear();
}

std::string Filiere::getNomFiliere() const { return nomFiliere; }

void Filiere::inscrireEtudiant(Etudiant* e) {
    if (e == nullptr) {
        throw std::invalid_argument("Etudiant nul");
    }
    for (const Etudiant* et : etudiants) {
        if (et == e) {
            throw std::runtime_error("Etudiant deja inscrit dans cette filiere");
        }
    }
    if (e->getFiliere() != nullptr) {
        throw std::runtime_error("Etudiant deja inscrit dans une autre filiere");
    }
    etudiants.push_back(e);
    e->setFiliere(this);
}

void Filiere::retirerEtudiant(Etudiant* e) {
    auto it = std::find(etudiants.begin(), etudiants.end(), e);
    if (it != etudiants.end()) {
        (*it)->setFiliere(nullptr);
        etudiants.erase(it);
    }
}

int Filiere::getNombreEtudiants() const {
    return etudiants.size();
}

void Filiere::afficherTous() const {
    std::cout << "Etudiants de la filiere " << nomFiliere << " :" << std::endl;
    for (const Etudiant* e : etudiants) {
        std::cout << "  - " << e->getNom()
                  << " (Moyenne: " << e->calculerMoyenne() << ")" << std::endl;
    }
}

// MAIN
int main() {
    // --- Test de la classe Etudiant ---
    Etudiant e1;
    e1.setNom("Alice Dupont");
    e1.setCin("123456789012345");
    e1.setNote1(15.5);
    e1.setNote2(12.0);
    e1.setNote3(18.5);
    std::cout << "Etudiant 1 : " << e1.getNom() << std::endl;
    std::cout << "Moyenne : " << e1.calculerMoyenne() << std::endl;

    e1.setNote1(25.0); // doit afficher une erreur
    std::cout << "Note1 apres tentative d'erreur : " << e1.getNote1() << std::endl;

    Etudiant e2("Bob Martin", "987654321098765", 10.0, 14.0, 16.0);
    std::cout << "Etudiant 2 : " << e2.getNom() << std::endl;
    std::cout << "Moyenne : " << e2.calculerMoyenne() << std::endl;

    // --- Test des associations ---
    std::cout << "\n=== Associations ===" << std::endl;
    Filiere* info  = new Filiere("Informatique");
    Filiere* maths = new Filiere("Mathematiques");

    Etudiant* a = new Etudiant("Alice",   "123", 15, 16, 17);
    Etudiant* b = new Etudiant("Bob",     "456", 10, 12, 14);
    Etudiant* c = new Etudiant("Charlie", "789", 18, 19, 20);

    info->inscrireEtudiant(a);
    info->inscrireEtudiant(b);
    info->inscrireEtudiant(c);
    info->afficherTous();
    std::cout << "Nombre d'etudiants : " << info->getNombreEtudiants() << std::endl;

    info->retirerEtudiant(b);
    delete b; // b n'appartient plus a la filiere : on le supprime nous-memes
    std::cout << "\nApres retrait de Bob :" << std::endl;
    info->afficherTous();

    a->changerDeFiliere(maths);
    std::cout << std::endl;
    info->afficherTous();
    maths->afficherTous();

    // --- Test des exceptions ---
    std::cout << "\n=== Erreurs ===" << std::endl;
    try {
        maths->inscrireEtudiant(a); // doublon
    } catch (const std::exception& ex) {
        std::cerr << "Erreur : " << ex.what() << std::endl;
    }
    try {
        maths->inscrireEtudiant(nullptr);
    } catch (const std::exception& ex) {
        std::cerr << "Erreur : " << ex.what() << std::endl;
    }

    delete info;  // supprime Charlie
    delete maths; // supprime Alice
    return 0;
}



