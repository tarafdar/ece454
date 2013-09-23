#!/usr/bin/perl
$NUM_REP=5;

print "SEQUENTIAL COMPILE TIMES\n";
@opt = ('gprof', 'gcov');
@opt = ('gprof', 'gcov', 'g', 'O2', 'O3', 'Os');
foreach(@opt){
	system("rm -f prog.$_.time");
	system("rm -f prog_run.$_.time");
	for($i=0; $i<$NUM_REP; $i=$i+1)
	{
	   system("make clean -f Makefile.$_ > out");
	   system("/usr/bin/time -f \t%U make -f Makefile.$_ 1>>prog.stdout 2>>prog.$_.time");
	}
	$total_compile=0.0;
	$total_run=0.0;
	system("/usr/bin/time -f \t%U ./vpr iir1.map4.latren.net k4-n10.xml place.out route.out -nodisp -place_only -seed 0 > out 2>>prog_run.$_.time");
	$ls_out = `ls -l vpr`;
	$ls_print = $ls_print . "$_ $ls_out\n";	
	open( my $fh, "<", "prog.$_.time") or die "Could not open prog.$_time\n";

	print "Average Compilation time(USER) for  $_ using $NUM_REP repetions avg: ";
	$run_out = $run_out . "Average Run time(USER) for $_ using $NUM_REP repetitions avg: ";
	while(my $line = <$fh>){
		$total_compile += $line;
	}
	$avg=$total_compile /$NUM_REP;
	print "$avg\n";

	close($fh);
        	
	open( my $fh, "<", "prog_run.$_.time") or die "Could not open prog_run.$_time\n";

	while(my $line_run = <$fh>){
		$total_run += $line_run;
	}
	$avg_run=$total_run/$NUM_REP;
	$run_out = $run_out . " $avg_run\n";
	close($fh);
	
}

print "\n\nPARALLEL COMPILE TIMES\n";
for($i=1;$i<16;$i=$i*2){
	system("rm -f prog.parallel.time");
	for($j=0; $j<$NUM_REP; $j=$j+1)
	{
	   system("make clean -f Makefile.O3 > out");
	   system("/usr/bin/time -f \t%E make -f Makefile.O3 -j $i 1>>prog.stdout 2>>prog.parallel.time");
	}
	$total_parallel=0.0;
	open( my $fh, "<", "prog.parallel.time") or die "Could not open prog.parallel.time\n";
	while(my $line_parallel = <$fh>){
		my @line_col = split(':', $line_parallel);
		$total_parallel += $line_col[1];
	}
	$avg_parallel=$total_parallel/$NUM_REP;
	$avg_print = $avg_print . "Average time{ELAPSED) for  parallel compilation with $i threads using $NUM_REP repetions avg: $avg_parallel\n";
	close($fh);
}

print $avg_print;
print "\n\n";

print "PROGRAM SIZES\n";
print $ls_print;

print "PROGRAM RUN TIMES\n";
print $run_out;


#Profiling
@prof = ('gprof', 'gprofO2', 'gprofO3');
system("rm -rf profile_output");
system("mkdir profile_output");
foreach(@prof){
	system("make clean -f Makefile.$_ > out");
	system("make -f Makefile.$_ > out");
	system("./vpr iir1.map4.latren.net k4-n10.xml place.out route.out -nodisp -place_only -seed 0 >out");	
	system("gprof vpr gmon.out > profile_output/$_.out");
}



system("rm -f out");
