#include	<stdio.h>
#include	<unistd.h>

#define	oops(m,x)	{ perror(m); exit(x); }

main(int ac, char **av)
{
	int	thepipe1[2], pid1;
	int 	thepipe2[2], pid2;
	if ( ac != 4 ){
		fprintf(stderr, "usage: pipe cmd1 cmd2 cmd3\n");
		exit(1);
	}
	if ( pipe( thepipe1 ) == -1 )
		oops("Cannot get a pipe1", 1);

	if( (pid1 = fork()) == -1 )
		oops("Cannot fork1", 2);

	if ( pid1 > 0 ){					
		close(thepipe1[1]);	

		if ( dup2(thepipe1[0], 0) == -1 )
			oops("could not redirect stdin 1",3);

		close(thepipe1[0]);	
		execlp( av[3], av[3], NULL);
	}else{
		if( pipe(thepipe2) == -1 )
			oops("Cannot get a pipe2", 1);
		if ( (pid2 = fork()) == -1 )
			oops("Cannot fork2", 2);
		if ( pid2 > 0 ){	
			close(thepipe1[0]);
			if( dup2(thepipe1[1], 1) == -1 )
				oops("Could not redirect stdin 2", 3);
			close(thepipe1[1]);
			
			close(thepipe2[1]);
			if( dup2(thepipe2[0], 0) == -1)
				oops("Could not redirect stdin 3", 3);
			close(thepipe2[0]);
			execlp(av[2], av[2], NULL);
		}else{
			close(thepipe2[0]);
			if( dup2(thepipe2[1], 1) == -1 )
				oops("Could not redirect stdin 4", 3);
			close(thepipe2[1]);
			execlp(av[1], av[1], NULL);
		}
	}
	return 0;
}
