_monitor(philosophes)
  type etatPossible = enum (pense, afaim, mange)
  const yo := 777
_body(philosophes)
  var etat[5]: etatPossible
  /*_proc(  )*/
_monitor_end


resource hello()
	import philosophes
	write("Yo", yo)
end
