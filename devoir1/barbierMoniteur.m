_monitor(philosophes)
  type etatPossible = enum (PENSE, AFAIM, MANGE)
  op prendre(i:int), deposer(i:int)
_body(philosophes)
  op test(k:int) returns libre: bool
  var etat[5]: etatPossible := ([5] PENSE)
  _condvar1(attente,5)

  _proc( prendre(i) )
    etat[i+1] := AFAIM;
    test(i)
    if etat[i+1] != MANGE -> _wait(attente[i+1]) fi
  _proc_end

  _proc( deposer(i) )
    etat[i+1] := PENSE;
    var k := (i+1) mod 5
    if (test(k)) -> _signal(attente[k+1]) fi
    k := (i-1) mod 5
    if (test(k)) -> _signal(attente[k+1]) fi
  _proc_end

  proc test(k) returns libre
    libre := false
    if (etat[(k+1) mod 5 +1] != MANGE and (etat[k+1] = AFAIM) and (etat[(k-1) mod 5 +1] != MANGE)) ->
      etat[k+1] := MANGE
      libre := true
    fi
  end
_monitor_end


resource philo()
	import philosophes
  process phil(i := 0 to 4)
     var j := 0
     do j < 10 ->
       philosophes.prendre(i)
       write("Philosophe", i+1, "mange")
       nap(int(random(1)*100))
       philosophes.deposer(i)
       write("Philosophe", i+1, "pense")
       nap(int(random(1)*300))
       j++
     od
  end
end
