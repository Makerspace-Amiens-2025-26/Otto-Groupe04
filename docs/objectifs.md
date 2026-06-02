---
layout: default
nav_order: 3
title: Objectifs du projet
---

# Introduction

Le projet Otto-MKS s'inscrit dans le cadre des projets de première année du MakerSpace d'UniLaSalle Amiens. Il s'agit de concevoir, fabriquer et programmer un robot humanoïde bipède basé sur le projet open-source Otto DIY, dans une version adaptée au MakerSpace.

## Contexte du Projet

Otto est un petit robot humanoïde open-source initialement conçu pour initier les jeunes aux STEM. Le MakerSpace d'UniLaSalle Amiens a développé sa propre version — l'Otto-MKS — intégrant une carte électronique sur mesure avec un microcontrôleur ESP32, une connectivité Bluetooth/WiFi, et des pièces imprimées en 3D.

Chaque groupe de 3 étudiants doit fabriquer son propre Otto-MKS en suivant les tutoriels du site, puis le personnaliser et le programmer pour participer aux **Ottolympiades** en fin d'année.

## Objectifs du Projet

- Assembler un robot Otto-MKS fonctionnel à partir des composants fournis
- Modifier la conception 3D pour personnaliser notre robot
- Programmer des comportements autonomes (marche, évitement d'obstacles…)
- Participer aux Ottolympiades avec un robot compétitif

# Existant

Le projet s'appuie sur la base Otto-MKS fournie par le MakerSpace, qui comprend :
- Une carte électronique avec ESP32, LED, interrupteur et connecteurs servomoteurs
- 4 servomoteurs pour les mouvements de marche
- Un capteur ultrason HC-SR04
- Une batterie 9V rechargeable en USB-C
- Des fichiers de pièces 3D de base à imprimer et modifier

# Cahier des Charges

- Le robot doit marcher de manière autonome
- Il doit détecter et éviter les obstacles grâce au capteur ultrason
- Les pièces doivent être imprimées en 3D et modifiées sur Onshape
- Le code doit être développé sur Arduino (ESP32)
- Le robot doit être prêt pour les épreuves des Ottolympiades :
  - Chrono challenge (ligne droite)
  - Course d'obstacles
  - Otto Sumo
  - Tir à la corde
