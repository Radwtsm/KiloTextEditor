#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// this is where we store the original terminal attributes
struct termios orig_termios;

void disableRawMode(void)
{
  // revert to original stdin settings
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode(void)
{
  struct termios raw;
  // get settings from terminal and saves it in "raw"
  tcgetattr(STDIN_FILENO, &orig_termios);

  // automatically call the function when the main() has finished or exit() is called
  atexit(disableRawMode);

  // create a copy to make the changes
  struct termios raw = orig_termios;

  // avoid showing user input in terminal
  // bitwise AND with the NOT(ECHO), we switch the ECHO attribute
  raw.c_lflag &= ~(ECHO);

  // we use our struct for setting the attributes in the terminal
  //  TCSAFLUSH, the change shall occur after all output written to fildes is transmitted, and all input so far received but not read shall be discarded before the change is made.
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void)
{
  enableRawMode();

  // variable holding the character taken from user input
  char c;

  // we use read() to read a 1 byte from STDIN_FILENO (or standard input)
  // q keypress is excludes, so we can use the "q" to quit
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
    ;

  return 0;
}
