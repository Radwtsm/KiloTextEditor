#include <stdio.h>
#include <ctype.h>
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
  // get settings from terminal and saves it in "raw"
  tcgetattr(STDIN_FILENO, &orig_termios);

  // automatically call the function when the main() has finished or exit() is called
  atexit(disableRawMode);

  // create a copy to make the changes
  struct termios raw = orig_termios;

  // avoid showing user input in terminal
  // c_lflag stands for local flags , when flags start with "I" it mean they are input flags, event tho ICANON and ISIG are not input flags.
  // bitwise AND with the NOT(ECHO), we switch the ECHO attribute
  // With ICANON we can now read input byte by byte instead of lines.
  // With ISIG we disable the SIGINT (Ctrl-c) and the SIGTSTP(Ctrl-z)
  // With IEXTEN we disable Ctrl-V (i.e if you type Ctrl-C Ctrl-V we could input 3 bytes)
  // By default, Ctrl-S and Ctrl-Q are used for software flow control. Ctrl-S stops data from being transmitted to the terminal until you press Ctrl-Q.
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  // IXON turns off XON (Ctrl-S to pause transmission) and XOFF (Ctrl-Q to resume) events.
  // ICRNL transaltes any carriage returns into newlines.
  // BRKINT sends will sedn a SIGINT signal when a break condition occurs
  // INPCK enables parity checking, which doesn’t seem to apply to modern terminal emulators.
  // ISTRIP causes the 8th bit of each input byte to be stripped, meaning it will set it to 0. This is probably already turned off.
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  // OPOST outpuf flag, disables all output processing
  // by default every "\n" is converted to "\r\n"
  raw.c_oflag &= ~(OPOST);

  // CS8 is not a flag, it is a bit mask with multiple bits, which we set using the bitwise-OR (|) operator unlike all the flags we are turning off. It sets the character size (CS) to 8 bits per byte. On my system, it’s already set that way.
  raw.c_cflag |= (CS8);

  // VMIN sets the minimum number of bytes to read before the read() function returns.
  // We set it to zero , so the read() function returns as soon as there is an input to read
  raw.c_cc[VMIN] = 0;
  // VTIME sets the time that the read() function is going to wait for an input before returning 0 (it is correct as it usually returns the number of bytes received)
  // we set it to 1/10 of a second , so 100 ms
  raw.c_cc[VTIME] = 1;

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
  {
    // checks for control characters (characters that are not printable i.e Escape)
    if (iscntrl(c))
    {
      // we manually add \r to move the cursor left (we disabled OPOST flag that prepends a \r to \n)
      printf("%d \r\n", c);
    }
    else
    {
      // we show the char as a decimal number (%d) and as a byte directly (%c)
      printf("%d %c \r\n", c, c);
    }
  }

  return 0;
}
