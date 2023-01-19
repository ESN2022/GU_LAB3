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
![image](https://github.com/ESN2022/GU_LAB3/blob/main/qsys_1.png)
![image](https://github.com/ESN2022/GU_LAB3/blob/main/qsys_2.png)

# Avancement et Résultats
Le programme se compose principalement du module de lecture-écriture I2C, du module int vers BCD, du module d'interruption de temps et du module d'interruption de bouton. Les données de l'axe XYZ doivent être lues dans des registres ayant des adresses différentes. Il convient de noter que la mesure ne peut être activée qu'en écrivant dans les deux registres de contrôle aux adresses 0X27 et 0X2D et que la résolution des données est spécifiée dans le registre DATA_FORMAT.


Enfin, nous pouvons voir que le DE10-Lite affiche l'accélération de chaque axe sur l'affichage de 7 segments et met à jour les valeurs d'accélération une fois par seconde, le bouton peut également  modifier pour afficher les valeurs d'accélération sur les différents axes.


https://user-images.githubusercontent.com/90764364/213461092-8e3481bf-ecf0-4405-9617-c6a2b43d9c23.mp4


# Conclusion
Ce TP a été la première fois que j'ai utilisé la co-conception dans le développement d'un produit réel, et j'ai essayé d'ajouter des IPs qui n'étaient pas dans le système, et j'ai amélioré ma compréhension et mon application du protocole de communication I²C. La communication entre le capteur et la carte a été le point le plus difficile dans ce TP, et aussi le domaine où j'ai passé le plus de temps. Enfin, en raison des délais limités, je n'ai pas très bien calibré mes capteurs et je n'ai donc pas obtenu de résultats très précis, sur lequel je dois encore avancer.
