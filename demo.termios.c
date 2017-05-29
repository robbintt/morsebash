#include <termios.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h> /* for gettimeofday() */


struct termios info;

int keydown = 0;
struct timeval t1, t2, t_debounce;

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

// compute and print the elapsed time in millisec
double yield_time()
{
  double elapsedTime;
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  printf("%f ms.\n", elapsedTime);

  return elapsedTime;
}

void termios_demo()
{

  int ch;
  while((ch = getchar()) != 27 /* ascii ESC */) {
		if (ch < 0) { if (ferror(stdin)) { /* there was an error... */ }

      // i want to test debounce here and yield the time if debounce state is false
      // same code as if you hit another key...
      if (keydown == 1) {
        gettimeofday(&t2, NULL);
        gettimeofday(&t_debounce, NULL);

        if (yield_time() > 2500) {
          keydown = 0; /* set keyup */
        }
      }

			clearerr(stdin);
			/* do other stuff */
    } else if (ch == 'f') {
      keydown = 1; /* set keydown */
      gettimeofday(&t1, NULL);
      printf("%c\n", ch);
		} else {
			/* some key OTHER than ESC was hit, do something about it? */
      
      if (keydown == 1) {
        keydown = 0; /* set keyup */
        gettimeofday(&t2, NULL);

        yield_time();
      }
      // give difftime of keydown and keyup
      // how will this work given VMIN is always returning an error... 
      // i guess in error we will reset the flag too
      // maybe the debounce goes in error
      
		}
  }
  printf("\n");
}

int main()
{
  setup_termios();

	termios_demo();

  teardown_termios();

  return 0;
}



