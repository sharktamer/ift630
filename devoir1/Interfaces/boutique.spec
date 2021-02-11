# 0 /home/yo/Desktop/uni/ift630/devoir1/barbierMoniteur.sr 9+

global boutique
 type etat = enum ( DORT , ATTEND , COUPE , DEPLACEMENT , PORTE )
 const NBPLACES : int := 5
 const TOTALCLIENT : int := 10
 var nbClients : int := 0
 op appelerClient ( ) , ouvrirPorte ( ) , fermerPorte ( ) , deplacer ( i : int ) , arreter ( i : int ; activite : etat ) , clientArrive ( ) returns attente : bool , attendreTour ( ) , signalerPret ( ) , attendrePorte ( ) , sortir ( )
 body boutique ; ; end ;
