_monitor(attraction)
  const C := 5
  type etatPossible = enum (VIDE, PLEIN, ATTENTE)
  op demarrer(), terminer(), embarquer(), debarquer()
_body(attraction)
  var nbPassagers := 0
  var nbAttentes := 0
  var etat:etatPossible := VIDE
  _condvar(plein)
  _condvar(vide)
  _condvar(embarquement)
  _condvar(debarquement)

  _proc( demarrer() )
    fa i := 1 to C -> _signal(embarquement) af
    _wait(plein)
  _proc_end

  _proc( terminer() )
    fa i := 1 to C -> _signal(debarquement) af
    _wait(vide)
  _proc_end

  _proc( embarquer() )
    if (nbAttentes = 0 and etat != PLEIN) ->
      nbPassagers++
    []else ->
      nbAttentes++
      _wait(embarquement)
      nbAttentes--
      nbPassagers++
      etat := ATTENTE
    fi
    if (nbPassagers = C) ->
      etat := PLEIN
      _signal(plein)
    fi
  _proc_end

  _proc( debarquer() )
    _wait(debarquement)
    nbPassagers--
    if(nbPassagers = 0) ->
      etat := VIDE
      _signal(vide)
    fi
  _proc_end

_monitor_end


resource parc()
	import attraction
  const N := 16
  process voiture
    do true ->
      write("Voiture prete pour l'embarquement")
      attraction.demarrer()
      write("Debut des 5 tours de piste")
      fa i := 1 to 5 -> nap(100) af
      write("Fin des tours")
      attraction.terminer()
    od
  end

  process passager(i := 1 to N)
     var j := 0
     do j < 10 ->
       write("Passager", i, "attend pour embarquer")
       attraction.embarquer()
       write("Passager", i, "dans la voiture")
       attraction.debarquer()
       j++
     od
  end
end
