# Écoconception - Passerelle LoRa ↔ GSM pour Ruche Connectée

## 1. Description du projet
Ce projet consiste en la conception et la réalisation de la chaîne de communication d'une **ruche connectée**. 
Le système a pour but de collecter des données environnementales à distance via un réseau local de très faible consommation (**LoRa**) et de les retransmettre à un utilisateur distant en utilisant le réseau mobile cellulaire (**GSM / LTE** via le module SIM7000G). 

Les contraintes principales du projet reposent sur la gestion drastique de l'énergie. Le système est conçu pour basculer régulièrement en mode veille (*Deep Sleep* / *Light Sleep*) entre les réceptions d'informations des capteurs et les émissions afin de minimiser au maximum sa consommation électrique. 

Les modules/composants matériels principaux intervenant dans cette passerelle sont :
- Le microcontrôleur principal **ESP32 WROOM** (qui orchestre la réception et l'envoi, et gère les mises en sommeil).
- Le module de communication mobile **SIM7000G** (qui transfère les données vers le serveur / utilisateur).
- Le module de réception longue portée **LoRa LLCC68** (qui reçoit les données émises par des nœuds capteurs sur la ruche).


## 2. Définition de l'unité fonctionnelle
L'unité fonctionnelle (UF) d'un produit caractérise de manière quantifiable le service rendu par ce produit. Elle sert de référence (quantité de référence) dans toute étude d'Analyse du Cycle de Vie (ACV) ou démarche d'écoconception.

Pour cette passerelle LoRa/GSM de ruche connectée, une définition pertinente et complète de l'unité fonctionnelle peut s'énoncer de la manière suivante :

> **« Assurer la collecte locale (via LoRa) et la transmission distante (via GSM/LTE) des paramètres environnementaux d'une ruche, à une fréquence d'un envoi toutes les X heures, dans un environnement rural, pour une durée de vie anticipée de 5 ans. »**

Les grandeurs clés de cette unité fonctionnelle sont :
- **L'action technique (Quoi ?)** : Collecte de petits paquets de données et ré-émission sur le réseau mobile.
- **Le contexte (Où ?) / La portée** : Communication radio LoRa pour un capteur éloigné de l'ESP32, puis liaison data via les antennes relais GSM de l'opérateur local.
- **La performance / La durée (Combien de temps ?)** : La durée de vie d'usage estimée (ex: 5 ans) est une donnée vitale car elle permet de répartir les impacts de fabrication sur le service rendu et détermine si un renouvellement de la batterie ou du matériel sera à intégrer dans le calcul global.
