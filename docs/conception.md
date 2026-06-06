---
layout: default
nav_order: 5
title: Conception et prototypage
---

# Conception et prototypage

## Modélisation 3D

Nous avons travaillé sur **Onshape** à partir des fichiers de base Otto-MKS pour modifier et personnaliser les pièces de notre robot.

[Voir notre modèle Onshape](https://cad.onshape.com/documents/dc2aca01fe742e8085016c10/w/add9737cc5fec3410f20bf62/e/b48b7d90dc361639b8b7c192){: .btn .btn-primary }

## Impression 3D

Les pièces ont été imprimées au MakerSpace d'UniLaSalle Amiens en **PLA**.

{: .note }
> Section à compléter avec les photos d'impression et les paramètres retenus.

## Assemblage

L'assemblage suit les étapes des tutoriels Otto-MKS :
1. Fixation des servomoteurs dans les pièces imprimées
2. Montage de la carte électronique ESP32
3. Câblage des servomoteurs et du capteur ultrason
4. Installation de la batterie 9V USB-C

{: .note }
> Section à compléter avec les photos d'assemblage.

## Programmation

Le code est développé avec l'**IDE Arduino** en C++ pour l'ESP32. Il gère :
- Les mouvements de marche (oscillations des 4 servomoteurs)
- La détection d'obstacles (HC-SR04)
- Le contrôle à distance (RemoteXY via Bluetooth/WiFi)

{: .note }
> Section à compléter avec le code final et les explications.
