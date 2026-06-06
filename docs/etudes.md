---
layout: default
nav_order: 4
title: Études et choix techniques
---

# Études et choix techniques

## Microcontrôleur — ESP32

La carte Otto-MKS est équipée d'un **ESP32**, choisi pour :
- Sa connectivité **WiFi et Bluetooth** intégrée (contrôle à distance via RemoteXY)
- Sa compatibilité avec l'environnement Arduino
- Sa puissance suffisante pour gérer 4 servomoteurs en temps réel

## Servomoteurs

4 servomoteurs assurent les mouvements de marche :
- **2 servos "hanches"** — rotation latérale du corps
- **2 servos "pieds"** — inclinaison des pieds

Les oscillations combinées de ces 4 servos génèrent la démarche bipède d'Otto.

## Capteur ultrason — HC-SR04

Le HC-SR04 mesure la distance par émission/réception d'ultrasons. Il est positionné à l'avant du robot pour détecter les obstacles et déclencher un comportement d'évitement automatique.

| Caractéristique | Valeur |
|-----------------|--------|
| Portée | 2 cm – 400 cm |
| Précision | ~3 mm |
| Tension | 5V |

## Contrôle à distance — RemoteXY

L'application **RemoteXY** permet de créer une interface de contrôle sur smartphone connectée à l'ESP32 via Bluetooth ou WiFi, sans infrastructure réseau.

## Conception 3D — Onshape

Les pièces sont modélisées sur **Onshape** et imprimées en 3D (PLA) au MakerSpace. Onshape est un outil de CAO collaboratif accessible directement depuis le navigateur.

[Voir notre modèle Onshape](https://cad.onshape.com/documents/dc2aca01fe742e8085016c10/w/add9737cc5fec3410f20bf62/e/b48b7d90dc361639b8b7c192){: .btn .btn-primary }
