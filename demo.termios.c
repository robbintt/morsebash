#include <termios.h>
#include <stdio.h>


struct termios info;

/* termios - from: https://stackoverflow.com/a/21101030
 * see: `man termios` */
void setup_termios()
{
  tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
  info.c_lflag &= ~ICANON;      /* disable canonical mode */
  info.c_cc[VMIN] = 0;          /* don't even wait, just read from stdin constantly */
  // info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
  info.c_cc[VTIME] = 0;         /* no timeout */

  /* from `man termios`
  * see k&r p.49 this sets all the bits of echo to zero by 
  * setting echo off */
  info.c_lflag &= ~ECHO;  /* maybe turn echo off... */

  /* where info is consumed */
  tcsetattr(0, TCSANOW, &info); /* set immediately */
}

/* clean up after termios */
void teardown_termios()
{
  tcgetattr(0, &info);
  info.c_lflag |= ICANON;
  info.c_lflag |= ECHO;
  tcsetattr(0, TCSANOW, &info);
}

void termios_demo()
{
  int ch;
  while((ch = getchar()) != 27 /* ascii ESC */) {
		if (ch < 0) {
			if (ferror(stdin)) { /* there was an error... */ }
			clearerr(stdin);
			/* do other stuff */
		} else {
			/* some key OTHER than ESC was hit, do something about it? */
		}
  }
}

int main()
{
  setup_termios();

	termios_demo();

  teardown_termios();

  return 0;
}



