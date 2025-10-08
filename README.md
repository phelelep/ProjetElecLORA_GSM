# 🐝 Ruche Connectée – Passerelle LoRa ↔ GSM

Projet réalisé dans le cadre du **cours de Projet d'Électronique** à **Polytech Nice Sophia Antipolis**,  
encadré par **M. Christian Peter** (*Christian.PETER@univ-cotedazur.fr*).

**Groupe :**  
- CEBAN Daniel — [ceban.daniel@etu.univ-cotedazur.fr] 
- MIRRIONE Xavier  — [mirrione.xavier@etu.univ-cotedazur.fr]

---

## 🎯 Objectif du projet

Ce projet vise à concevoir une **ruche connectée** capable de :

- Collecter et transmettre à distance des données environnementales ;  
- Communiquer via une **passerelle LoRa ↔ GSM** ;  
- Intégrer des fonctions avancées comme **l’élimination des parasites par laser**.

Cette partie du projet se concentre sur **la communication à distance** des données.

---

## 🧩 Technologies utilisées

- **ESP32** – Microcontrôleur principal du système.  
- **SIM7000G** – Module GSM / NB-IoT / LTE pour la communication mobile.  
- **LoRa** – Communication longue portée et faible consommation.  
- **TinyGSM** – Bibliothèque facilitant la communication série avec le SIM7000G.

---

## ⚙️ Fonctionnement général

1. Initialisation et configuration du modem GSM (SIM7000G).  
2. Connexion au réseau mobile via **APN SFR** (`sl2sfr`).  
3. Transmission ou test d’envoi de données / SMS.  
4. Préparation à l’intégration de la communication **LoRa** pour la collecte distante.  

---

## 🚧 Prochaines étapes

- Finaliser le **débogage de la carte** pour une communication stable entre les composants.  
- **Intégrer le module LoRa** à la passerelle GSM.  
- **Optimiser la consommation énergétique** pour un fonctionnement autonome sur batterie.

---

## 📁 Structure du dépôt




## 💡 Référence du projet précédent

Ce projet est basé sur le travail de la promotion précédente :  
**Oumar Sadjo Barry** & **Mor Dieng** – *RucherLoRa*  
- Guilherme Ventapane Rodrigues — [guilherme@ventapane.eu](mailto:guilherme@ventapane.eu)  
- Jarod Lecoeuvre — [jarod.lecoeuvre@etu.univ-cotedazur.fr](mailto:jarod.lecoeuvre@etu.univ-cotedazur.fr)



---

## 🧠 Encadrant

**M. Christian Peter**  
_Enseignant – Département Électronique & Informatique Industrielle_  
[Christian.PETER@univ-cotedazur.fr](mailto:Christian.PETER@univ-cotedazur.fr)
