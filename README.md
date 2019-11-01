# Gnuvario_for_TTGO-T5
# GNUVARIO-E     Stable version 0.3
# GNUVARIO-E     Developement version 0.6 beta 4
# PCB            version 2.00
# 3D CASE        version 2.00


 v 0.1                              beta 1 version                     
 v 0.2     beta 1      23/06/19     debug VarioScreen                        
 v 0.3     beta 1      25/06/19     correction mesure tension          
                                    correction mesure de vitesse       
 v 0.3     beta 2      26/06/19     correction save IGC                                                 
 v 0.4     beta 1      03/07/19     ajout la coupure du son            
 v 0.4     beta 2      06/07/19     ajout statistique                  
 v 0.4     beta 3      18/07/19     Correction gestion Eeprom          
                                    Correction tache affichage          
                                    Ajout delete task display          
                                    Correction affichage statistique   
 v 0.4      beta 4      22/07/19    Correction effacement screendigit                               
                                    Correction affichage duree du vol  
                                    Correction affichage statistique   
                                    Modification FlightHistory         
                                    Ajout parametre flighthistory dans 
                                    SETTINGS.TXT                       
                                    Enregistrement des statistique     
                                    toutes les 60sec - reduction des   
                                    cycle d'ecriture dans la mémoire   
                                    flash de l'ESP32                   
 v 0.4     beta 5    24/07/19       Ajout TrendDigit                   
                                    Modification TrendLevel            
                                    Modification screendigit           
                                    Correction démarrage du vol    
 v 0.4     beta 6    25/07/19       Ajout NO_RECORD                                                                                          
                                    Correction TWOWIRESCHEDULER        																		
 
 v 0.4     beta 7    05/08/19       Désactivation enregistrement stat  
                                    pour eviter le bug du plantage      
                                    Ajout paramettre wifi              
                                    Modification SETTINGS.TXT v1.2     
 v 0.4     beta 8    06/08/19       Ajout icone Norecord               
                                    Correction bug statistique         
                                    Raffraichissement de l'ensemle de  
                                    l'écran toutes les 30sec           
 v 0.5     beta 1    10/08/19       Ajout Gestionnaire d'erreur ESP32  
                                    Mise à jour via SDCARD /update.bin 
                                    Ajout Serveur Web update/download  
                                    Ajout HAVE_WIFI                    
                                    Ajout BT                           
 v 0.5     beta 2    20/08/19       Ajout dans hardwareConfig32 de la  
                                    version du PCB et de la TTGO       
                                    Ajout lecture de la temperature    
                                    MAJ OTA Web serveur                
                                    Ajout changement de page           
                                    Ajout 2ème ecran                   

 v 0.5     beta 3  25/08/19         Ajout LOW/HIGH level cmd ampli     
                                    Ajout ecran reglage volume du son  
                                    Correction Bug bouton              

 v 0.5     beta 4  30/08/19         Ajout écran stat 
                                    Modification librairie ToneHAL 
																		Correction bug d'affichage mineurs

 v 0.5     beta 5  31/08/19         Correction bg reglage volume       
                                    Ajout commande ampli dans loop     
                                    pour test                          
 
 v 0.5     beta 6  04/09/19         Changement librairies MS5611 et MPU9250                                                             
                                    Modification de la calibration de l'altitude par le GPS                  
                                    Ajout d'un coeficiant de compensation de temperature             
                                    Modification de la séquence de démarrage de l'enregistrement   
																		
 v 0.5     beta 7  10/09/19         Ajout choix des pin SDA, SCL                                                                                                       
                                    Modification des librairies du MPU9250 / ajout fonctions de                                                                                   
                                    Calibration                                                      
                                    Ajout Son de monté variable                                      
                                    Modification de la sequence de démarrage -                       
                                    allongement du temps de l'écran de stat à 6 sec                  
                                    init MS5611 avant ecran stat, ajout acquisition durant ecran     
                                    stat et init kalman après                                        
                                    Ajout d'un paramettre de nombre d'acquisition du GPS avant        
                                    la mise à jour de Altitude Barometrique                          
                                    Modification librairie EEPROM                                    
 v 0.5   beta 8  22/09/19           Modification de l'affichage de Vbat                              
                                    Ajout d'un filtrage de la mesure de Vbat      

 v 0.5   beta 9  23/09/19           Correction affichage ecran Stat                                                                      
                                    Ajout possibilité d'avoir plusieurs tailles d'écran              
                                    Deep-sleep sur bouton central (3 sec)                            
 v 0.6   beta 1  26/09/19           Ajout mise en sommeil                                                                               
                                    Flash via USB en utilisant flash_download_tools                  
                                    Ajout ecran calibration                                          
                                    Modification SETTING.TXT (Version 1.3)                           
                                    Correction HAVE_SDCARD                                           
                                    Changement de la librairie webserver                             
 v 0.6   beta 2  06/10/19           Ajout varioscreen pour l'écran 2.9''                             
                                    Ajout serveur Web sur sdcard                                     
                                    Correction du bug d'affichage des fleches                        
                                    Ajout du nom des champs de données sur l'écran                   
                                    Couper le SSID à 12 caractères                                                        
                                    Modification du fichier SETTINGS.TXT v1.4                                                                                                                            
                                    Ajout paramettre d'activation du BT dans le fichier SETTINGS.TXT 
                                    Modification de varioscreen pour gérer les lignes et les cercles 
                                    ainsi que les titres des champs de donnée                        
                                    Modification de l'arborescence de la carte SD                               
                                    Enregistrement des fichiers .IGC (traces) dans le dossier 'vols' 
                                    Mise à jour Ecran 2,9''                                          
                                    Modifier l'affichage des titres pour que la position             
                                    soit liée au digit                                               
 v 0.6   beta 3  20/10/19           Correction bug enregistrement sur la SD                          
                                    recodage de l'affichage des satellites                                  
                                    Ajout desactivation bip au demarrage                              
                                    Ajout écran 2.13''                                               
                                    Ajout compensation de température et correction altitude GPS     
                                    dans le fichier SETTINGS.TXT                                     
 v 0.6   beta 4  27/10/19           Correction gestion zone horaire                                 
                                    Changement de librairie MS5611 et MPU                            
                                    Ajout compensation alti gps                                      
                                    Ajout d'un bip lors du reglage du volume                         
                                    Amélioration gestion des boutons et des écrans au boot           
                                    Correction gestion du volume                                     
                                    Correction Calibration enableAmp                                 
                                    Correction du bug d'affichage de l'écran de stat                 
                                    Correction du bug de reboot / blocage si #define DEBUG_PRO est         
                                    commenté                                                         																		
																		
                   Developpement à venir                               
                                                                                                                                    
 V0.4                                                                                                                                          																												
 bug affichage finesse  
 
 V0.5                                                                  
 voir réactivité des données GPS                                                                     
 Probleme consommasion - SDcard - deep slepp                                                         
                                                                                                     
 v0.6                                                                                                   
 AJOUT - Calibration MPU                                                                                                              
 AJOUT - page web de configuration du vario / Page Web SDCARD                                        
 MODIF - Refaire gestion Eeprom avec preference                                                      
 AJOUT - gestion du MPU par Interruption                                                             
 BUG   - affichage figé / problème d'init du MPU et d'attente de la première lecture                 
 BUG   - reboot à l'init du MPU                                                                      
                                                                                                                                                           
 VX.X                                                                                                
 Paramètrage des écrans                                                                              
 Gérer le son via le DAC                                                                             
 revoir volume du son ToneESP32                                                                      
 Refaire gestion du son (parametrage via xctracer)                                                   
 Ecran position afficher les coordonées GPS, la boussole, et l'altitude                                                                                                     *
 Création dynamique des objets screen                                                                
 Créer une bibliothèque de log (debug)                                                               
 Sens et vitesse du vent                                                                             
 Carnet de vol (10 derniers vols)                                                                    
     10 zones d'eeprom - reduit le nombre d'écriture et économise la mémoire flash                   
 verifier fonctionnement BT                                                                          
 Recupération vol via USB                                                                                                                                                                                                                                  

                    Fonctionalitées   
		
  Version 0		
    Vario précis avec capteur de pression et accéleromètres
    GPS
    Enregistrement des vols sur carte SD
    Affichage sur ecran E-Ink
    Affichage du vario, de l'altitude, de la vitesse sol,
    de l'heure, du temps de vol, ...
                                                                       
  Version 0.3                                                                                                                               
    Parametre utilisateur dans fichier SETTINGS.TXT                    
    Coupure du son via le bouton central (en vol)                      
                                                                       
  Version 0.4                                                          
    Statistiques de Vol                                                
    taux de chute et finesse                                           
    indicateur de monte/descente 
    Désactiver l'enregistrement des vols		
                                                                       
  Version 0.5                                                            
    Mise à jour via la carte SD (update.bin)                          
    Récupération des vol via Wifi                                      
    Mise à jour via Wifi                                               
    Upload des fichiers de configuration via Wifi                      
    Ajout Bluetooth                                                    
    Multi-écran (ajout 2ème page / gestion des bouton droit et gauche) 
    Affichage de la température     
		Page de configuration du volume sonore
    Page de statistique accessible via les boutons	
    Mise en veille automatique en cas de batterie trop faible          
                                                                      
  Version 0.6                                                          
    Page de calibration                                                 
    Mise en veille prolongée                                           
    Ajout gestion des écran 2.9'' et 2.13''                            
    Ajout du serveur Web sur SDCARD                                    
    Activation du Bluetooth en parametre dans le fichier SETTINGS.TXT  
    Ajout de titre au dessus des champs de données                     
    Ajout de nouveau objects screen - ligne - rose des vents           
    Modification de l'organisation des fichiers sur la carte SD        
    Ajout compensation de température dans fichier SETTINGS.TXT        
    Ajout correction d'altitude GPS dans SETTINGS.TXT                  		