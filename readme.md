# PURR BROS - Jeu de plateforme 2D

![PURR BROS](assets/environment/background.png)

## Description
PURR BROS est un jeu de plateforme 2D développé en C++ avec Qt. Le joueur contrôle un personnage qui doit naviguer à travers différentes plateformes pour atteindre un objectif final, tout en collectant des pièces. Le jeu enregistre le meilleur temps réalisé et permet de recommencer une partie à tout moment.

## Fonctionnalités
- Contrôle du personnage avec les touches du clavier (Z/Q/S/D ou flèches directionnelles)
- Système de saut et gravité réaliste
- Plateformes mobiles et statiques
- Collecte de pièces
- Chronomètre et enregistrement du meilleur temps
- Menu avec options "About" et "Restart"
- Vue centrée sur le joueur qui suit ses déplacements

## Installation
### Prérequis
- Qt5 (Core, Gui, Widgets)
- Compilateur C++ compatible C++11
- CMake (version 3.25 ou supérieure)

### Compilation
1. Clonez le dépôt ou téléchargez les fichiers sources
2. Créez un répertoire de build :
   ```bash
   mkdir build && cd build
3. cmake ..
4. cmake --build .


##Structure du projet
.
├── CMakeLists.txt          # Fichier de configuration CMake
├── main.cpp                # Point d'entrée de l'application
├── MainWindow.h/cpp        # Gestion de la fenêtre principale
├── MyScene.h/cpp           # Gestion de la scène de jeu
└── assets/                 # Dossier contenant les ressources graphiques
    ├── environment/        # Arrière-plans et plateformes
    ├── player/             # Sprites du joueur
    └── ...


##Licence

Ce README fournit toutes les informations essentielles sur votre projet, y compris comment le compiler, l'exécuter et les contrôles. Vous pouvez le modifier selon vos besoins, par exemple pour ajouter des captures d'écran ou des informations supplémentaires sur les mécanismes de jeu.