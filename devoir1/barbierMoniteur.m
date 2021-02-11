_monitor(boutique)
  type etat = enum (DORT, ATTEND, COUPE, DEPLACEMENT, PORTE)
  const NBPLACES:int := 5
  const TOTALCLIENT:int := 10
  var nbClients:int := 0
  op appelerClient(), ouvrirPorte(), fermerPorte(), deplacer(i:int), arreter(i:int; activite:etat), clientArrive() returns attente:bool, attendreTour(), signalerPret(), attendrePorte(), sortir()
_body(boutique)
  _condvar(reveil)
  _condvar(clientPret)
  _condvar(deplacement)
  _condvar(porteSortie)
  _condvar(salleAttente)
  _condvar(clientSorti)

  var etatBarbier:etat := DORT
  var etatClient[TOTALCLIENT]:etat := ([TOTALCLIENT] DORT)

  _proc( deplacer(i) )
    var deplacementEnCours:bool := false
    fa k := 1 to TOTALCLIENT ->
      if (etatClient[k] = DEPLACEMENT) -> deplacementEnCours := true fi
    af
    if (etatBarbier = DEPLACEMENT) -> deplacementEnCours := true fi
    if (deplacementEnCours) -> _wait(deplacement) fi
    if (i = 0) -> etatBarbier := DEPLACEMENT
    []else -> etatClient[i] := DEPLACEMENT
    fi
  _proc_end

  _proc( arreter(i, activite) )
    if (i = 0) -> etatBarbier := activite
    []else -> etatClient[i] := activite
    fi
    _signal(deplacement)
  _proc_end

  _proc( appelerClient() )
    if (nbClients = 0) ->
      write("Le barbier s'endort")
      etatBarbier := DORT
      _wait(reveil)
      write("Le barbier se reveille")
    []else ->
      _signal(salleAttente)
    fi
    etatBarbier := ATTEND
    _wait(clientPret)
    etatBarbier := COUPE
  _proc_end

  _proc( ouvrirPorte() )
    _signal(porteSortie)
  _proc_end

  _proc( fermerPorte() )
    _wait(clientSorti)
  _proc_end

  _proc( sortir() )
    _signal(clientSorti)
  _proc_end

  _proc( clientArrive() returns attente )
    attente := true
    if (etatBarbier = DORT) ->
      _signal(reveil)
      attente := false
    fi
  _proc_end

  _proc( attendreTour() )
    _wait(salleAttente)
  _proc_end

  _proc( signalerPret() )
    _signal(clientPret)
  _proc_end

  _proc( attendrePorte() )
    _wait(porteSortie)
  _proc_end

_monitor_end


resource boutiqueBarbier()
	import boutique

  process barbier
     do true ->
      boutique.appelerClient()
      write("Barbier travaille")
      nap(int(random(1)*400))

      boutique.deplacer(0)
      write("Le barbier va ouvrir la porte")
      nap(5)
      boutique.arreter(0, boutique.PORTE)
      boutique.ouvrirPorte()
      write("La porte de sortie est ouverte")
      boutique.fermerPorte()
      write("La porte de sortie est ferme")
      boutique.deplacer(0)
      write("Le barbier retourne devant sa chaise")
      nap(5)
      boutique.arreter(0, boutique.ATTEND)

     od
  end

  process client(i := 1 to boutique.TOTALCLIENT)
     var j := 0
     do j < 10 ->
       nap(int(random(1)*500))
       boutique.nbClients++
       write("Le client", i, "arrive")
       if (boutique.nbClients > boutique.NBPLACES +1) ->
         write("La boutique est pleine, le client", i, "part")
         boutique.nbClients--
         next
       [](boutique.clientArrive()) ->
         boutique.deplacer(i)
         write("Le client", i, "se deplace vers une chaise de la salle d'attente")
         nap(5)
         boutique.arreter(i, boutique.ATTEND)
         write("Le client", i, "attend son tour sur une chaise dans la salle d'attente")
         boutique.attendreTour()
       fi
       boutique.deplacer(i)
       write("Le client", i, "se deplace vers la chaise du barbier")
       nap(5)
       boutique.arreter(i, boutique.COUPE)
       boutique.signalerPret()
       write("Le client", i, "est assis sur la chaise du barbier")

       boutique.attendrePorte()
       boutique.deplacer(i)
       write("Le client", i, "sort de la boutique")
       nap(5)
       boutique.arreter(i, boutique.DORT)
       boutique.nbClients--
       boutique.sortir()

       j++
     od
  end
end
