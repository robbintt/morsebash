#include <stdio.h>
#include <time.h>
#include <sys/time.h> /* for gettimeofday() */

#include <termios.h>

#include <ncurses.h>


struct termios info;

int counter = 0;
int keydown = 0;
struct timeval t1, t2;

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

double elapsedTime;
// compute and print the elapsed time in millisec
double yield_time()
{
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

  return elapsedTime;
}
void keyup_event()
{
  printf("Event %i, at %f ms.\n", counter, elapsedTime);
  counter++;
}

void termios_demo()
{

  int ch;
  while((ch = getchar()) != 27 /* ascii ESC */) {

    // manage errors and debounce
		if (ch < 0) { 

      // deal with stdin error
      if (ferror(stdin)) { /* there was an error... */ }

      // i want to test debounce here and yield the time if debounce state is false
      // same code as if you hit another key...
      if (keydown == 1) {
        gettimeofday(&t2, NULL);

        // this is a 2.5 second debounce timer on the error
        if (yield_time() > 2500) {
          keydown = 0; /* set keyup */
          keyup_event();
        }
      }

			clearerr(stdin);
			/* do other stuff */
    } 
    // manage 'f' down case
    else if (ch == 'f') {
      printf("%c\n", ch);
      if (keydown == 1) {
        gettimeofday(&t2, NULL);

        // give some debounce on the f key
        if (yield_time() > 20) {
          // the state is actually no longer keyup... we can get rid of this concept
          keydown = 0; /* set keyup */

          // this is our event
          keyup_event();

          // reset the f keydown event time
          gettimeofday(&t1, NULL);
          keydown = 1; /* set keyup */
        }
        else {
          printf("debounce event: f.\n");
        }
      }
      else {
        keydown = 1; /* set keydown */
        gettimeofday(&t1, NULL);
      } 
    }
    // what if there is a key that is not the f
    else {
			/* some key OTHER than ESC was hit, do something about it? */
      
      /*
      if (keydown == 1) {
        keydown = 0; // set keyup 
        gettimeofday(&t2, NULL);
        printf("%f ms.\n", elapsedTime);

        yield_time();
      }
      */

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



