#include <termios.h>
#include <unistd.h>

void enableRawMode(void)
{
  struct termios raw;
  // get settings from terminal and saves it in "raw"
  tcgetattr(STDIN_FILENO, &raw);
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
