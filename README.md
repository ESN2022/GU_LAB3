# Lab 3 : Display the accelerometer data on the 7 segments
# Introduction
Il s'agit du dernier TP de Co-conception, dans lequel nous devons lire les valeurs en mg de l'accéléromètre intégré dans le DE-10 Lite 
et les afficher sur un afficheur à 7 segments, et nous devons également utiliser les boutons pour afficher l'accélération dans les différents axes, 
il faut noter que les valeurs d'accélération doivent être obtenues via le protocole de communication I²C.

# Architecture du système
J'ai d'abord construit un système de base avec **un noyau logiciel NIOS II**, **un module d'horloge**, **un module de mémoire**, **un module JTAG** pour la communication avec le DE-10 Lite.
Ensuite, j'ai également dû ajouter différents **6 modules PIO** pour gérer les segment, **1 module PIO** pour gérer le bouton, ainsi que **le module timer** et, surtout, **le module I²C open source**.
L'architecture du système se présente comme suit :
![image](https://github.com/ESN2022/GU_LAB3/blob/main/LAB3.jpg)

Le **QSYS** est le suivant :





# Avancement et Résultats

# Conclusion
