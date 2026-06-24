# The Lonely Blacksmith

Jeu de crafting en mode console développé en C++ dans le cadre d'un test technique.

## Lancer le jeu

Téléchargez et exécutez directement `TheLonelyBlackSmith.exe` se trouvant dans le dossier Executable/ à la racine du projet.

## Démarche et étapes

### 1. Structures de base (`core structures`)
Mise en place des enums et types fondamentaux : `RessourceType`, `ItemType`, `ItemCategory`, `StructureType`, `LootRange`, `Recipe`. Ces structures portent toute la logique de données du jeu.

**Estimation : ~50 min**

### 2. Fichier de configuration (`configuration file and initial structures`)
Centralisation de toutes les valeurs configurables dans un struct `Config` : nombre de tours, ressources de départ, recettes, plages de loot, noms. L'objectif était de rendre le jeu facilement paramétrable sans toucher à la logique.

**Estimation : ~45 min**

### 3. Classe Player (`player class`)
Mise en place du joueur comme conteneur de ses composants : `RessourcesManager` et `ItemsManager`, ainsi que son score.

**Estimation : ~20 min**

### 4. Composants du joueur (`player's components`)
Implémentation de `RessourcesManager` (ajout, retrait, vérification des ressources) et `ItemsManager` (possession d'outils, récupération du meilleur outil par type).

**Estimation : ~10 min**

### 5. Boucle de jeu principale (`main game loop`)
Mise en place de `Game` avec la machine à états (`MENU`, `GAME`, `GAMEOVER`, `QUIT`) et les interfaces console correspondantes.

**Estimation : ~1h**

### 6. Système de collecte (`gather ressources system`)
Implémentation de `GatherRessourcesSystem` : sélection automatique du meilleur outil possédé, calcul du loot aléatoire selon les plages définies dans la config.

**Estimation : ~50 min**

### 7. Système de craft (`crafting system`)
Implémentation de `CraftSystem` : vérification des prérequis, vérification des ressources, fabrication d'objets et de structures, affichage des recettes avec disponibilité en temps réel.

**Estimation : ~1h10**

### 8. Corrections d'affichage (`display of items, structures`)
Correction de bugs liés à l'affichage des outils possédés et à la consommation des ressources pour les structures.

**Estimation : ~1h**

### 9. Traduction de l'interface (`translated user interfaces`)
Traduction de tous les textes affichés en console vers le français conformément aux spécifications, avec gestion de l'encodage UTF-8 pour les accents.

**Estimation : ~15 min**

### 10. Gestion des tours restants (`checking for remaining turns`)
Ajout d'une vérification pour que le joueur ne puisse pas dépasser le nombre maximum de tours, déclenchant correctement le game over.

**Estimation : ~15 min**

---

**Temps total estimé : ~7h**