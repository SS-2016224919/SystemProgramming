/* sigdemo1.c - shows how a signal handler works.
 *            - run this and press Ctrl-C a few times
 */

#include        <stdio.h>
#include        <signal.h>
#include	<termios.h>

main()
{
        void    f(int);                 /* declare the handler  */
        int     i;
	set_cr_noecho_mode();
        signal( SIGINT, f );            /* install the handler  */
        for(i=0; i<5; i++ ){            /* do something else    */
                printf("Hello\n");
                sleep(1);
        }
}


void f(int signum)                      /* this function is called */
{
        char cmd = 'n'; /* set default value */
        printf("Interrupted! OK to quit(y/n)?");
        cmd = getchar();
	printf("\n");
        if(cmd == 'y'){
                exit(1);
        }
}
set_cr_noecho_mode()
/* 
 * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 *  method: use bits in termios
 */
{
	struct	termios	ttystate;

	tcgetattr( 0, &ttystate);		/* read curr. setting	*/
	ttystate.c_lflag    	&= ~ICANON;	/* no buffering		*/
	ttystate.c_lflag    	&= ~ECHO;	/* no echo either	*/
	ttystate.c_cc[VMIN]  	=  1;		/* get 1 char at a time	*/
	tcsetattr( 0 , TCSANOW, &ttystate);	/* install settings	*/
}
