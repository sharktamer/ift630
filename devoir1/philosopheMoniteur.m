_monitor(philosophes)
  type etatPossible = enum (PENSE, AFAIM, MANGE)
  op prendre(i:int), deposer(i:int), test(k:int)
_body(philosophes)
  var etat[5]: etatPossible := ([5] PENSE)
  _condvar1(attente,5)
  _proc( prendre(i) )
    etat[i+1] := AFAIM;
    /*test(i)*/
    var k := i
    if (etat[(k+1) mod 5 +1] != MANGE and (etat[k+1] = AFAIM) and (etat[(k-1) mod 5 +1] != MANGE)) ->
      etat[k+1] := MANGE
      _signal(attente[k+1])
    fi


    if etat[i+1] != MANGE -> _wait(attente[i+1]) fi
  _proc_end
  _proc( deposer(i) )
    etat[i+1] := PENSE;
    /*test((i-1) mod 5 +1)*/
    var k := (i+1) mod 5 +1
    if (etat[(k+1) mod 5 +1] != MANGE and (etat[k] = AFAIM) and (etat[(k-1) mod 5 +1] != MANGE)) ->
      etat[k] := MANGE
      _signal(attente[k])
    fi



    /*test((i+1) mod 5 +1)*/
    k := (i-1) mod 5 +1
    if (etat[(k+1) mod 5 +1] != MANGE and (etat[k] = AFAIM) and (etat[(k-1) mod 5 +1] != MANGE)) ->
      etat[k] := MANGE
      _signal(attente[k])
    fi

  _proc_end

  _proc( test(k) )
    if (etat[(k+1) mod 5 +1] != MANGE and (etat[k] = AFAIM) and (etat[(k-1) mod 5 +1] != MANGE)) ->
      etat[k] := MANGE
      _signal(attente[k])
    fi
  _proc_end

_monitor_end


resource philo()
	import philosophes
  process phil(i := 0 to 4)
     var j := 0
     do j < 20 ->
       philosophes.prendre(i)
       write("Philosophe", i+1, "mange")
       nap(100)
       philosophes.deposer(i)
       write("Philosophe", i+1, "pense")
       nap(300)
       j++
     od
  end
end
