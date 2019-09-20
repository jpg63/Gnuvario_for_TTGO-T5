# Gnuvario_for_TTGO-T5
# GNUVARIO-E     Stable version 0.3
# GNUVARIO-E     Developement version 0.5 beta 6
# PCB            version 1.12
# 3D CASE        version 1.05


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
																		
                   Developpement à venir                               
                                                                                                                                    
 V0.4                                                                                                                                          																												
 bug affichage finesse  
 
 V0.5                                                                  
 Recupération vol via USB                                                                                         
 Mise à jour ESP32 via USB sans IDE  
 Calibration du MPU																												
 revoir volume du son ToneESP32                                        
 verifier effacement du m (altitude)                                   
 bug d'affichage des fleches  
 Ajout d'un ecran d'arrêt suivi de l'écran de stat si appuie 3 sec sur bouton au centre                  
 Ajout page web de reglage des paramettres
                                                                                                                                             
 VX.X                                                                  
 Refaire gestion du son                                                                              
 Paramètrage des écrans                                                                              
 Gérer le son via le DAC                                                                             
 Afficher la boussole                                                                                
 Sens et vitesse du vent                                                                             
 Carnet de vol (10 derniers vols)                                                                    

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
		Son de monté variable