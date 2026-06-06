---
layout: default
nav_order: 3
title: Objectifs du projet
---

# Introduction

Le projet Otto-MKS s'inscrit dans le cadre des projets de première année du MakerSpace d'UniLaSalle Amiens. Il s'agit de concevoir, fabriquer et programmer un robot humanoïde bipède basé sur le projet open-source Otto DIY, dans une version adaptée au MakerSpace.

## Contexte du Projet

Otto est un petit robot humanoïde open-source initialement conçu pour initier les jeunes aux STEM (Sciences, Technologie, Ingénierie, Mathématiques). Facile à assembler et à personnaliser, il est utilisé dans les écoles et Makerspaces du monde entier.

Le MakerSpace d'UniLaSalle Amiens a développé sa propre version — l'**Otto-MKS** — intégrant une carte électronique sur mesure avec un microcontrôleur **ESP32**, une connectivité **Bluetooth/WiFi**, et des pièces imprimées en 3D. Chaque groupe de 3 étudiants fabrique son propre Otto-MKS, le personnalise, et le programme pour participer aux **Ottolympiades** en fin d'année.

## Objectifs du Projet

- Assembler un robot Otto-MKS fonctionnel à partir des composants fournis
- Modifier la conception 3D sur Onshape pour personnaliser notre robot
- Programmer des comportements autonomes (marche, évitement d'obstacles…)
- Mettre en place un contrôle à distance via l'application RemoteXY (Bluetooth/WiFi)
- Participer aux Ottolympiades avec un robot compétitif

# Existant

Le projet s'appuie sur la base Otto-MKS fournie par le MakerSpace :

| Composant | Description |
|-----------|-------------|
| Carte Otto-MKS | ESP32, LED, interrupteur, connecteurs servomoteurs, emplacement HC-SR04 |
| Servomoteurs | 4 servomoteurs pour les mouvements de marche (hanches + pieds) |
| Capteur ultrason | HC-SR04 — détection d'obstacles |
| Batterie | 9V rechargeable en USB-C |
| Pièces 3D | Fichiers Onshape de base à modifier et imprimer |

# Cahier des Charges

- Le robot doit marcher de manière autonome
- Il doit détecter et éviter les obstacles (capteur HC-SR04)
- Les pièces doivent être modifiées sur Onshape et imprimées en 3D
- Le code doit être développé sur Arduino IDE (ESP32)
- Le robot doit pouvoir être contrôlé à distance via RemoteXY
- Le robot doit être prêt pour les 4 épreuves des Ottolympiades :

| Épreuve | Description |
|---------|-------------|
| ⏱️ Chrono challenge | Ligne droite, meilleur temps |
| 🚧 Course d'obstacles | Parcours semé d'obstacles |
| ⚔️ Otto Sumo | Duel — un seul reste debout |
| 💪 Tir à la corde | Deux équipes de 3 robots s'affrontent |
