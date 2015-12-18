#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// #define SERIAL_PORT "/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AI025XIU-if00-port0"
#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE B9600

/* Prototypes */
void setup_stdin(void);
int init(void);
void this_to_that(int f1, int f2);

/* Set up the terminal mode */
void setup_stdin(void) {
  struct termios term_struct;
  
  tcgetattr(0, &term_struct);
  /* lflag meanings:
      - ISIG    - generate corresponding signal for INTR, QUIT
      - ICANON  - canonical mode
      - ECHO    - echo input characters
      - ECHOE   - ERASE character erases preceding input character, WERASE erases preceding word
      - VEOF    - End-of-file character; causes pending tty buffer to be sent to the waiting user program without waiting for end-of-line
      - VEOL    - Additional end-of-line character
      - VERASE  - Erase character; erases previous, not yet erased character, but doesn't erase past EOF or beginning of line
      - VINTR   - Interrupt character; send a SIGINT (need ISIG set)
      - VKILL   - kill character; erases input since last EOF or beginning of line
      - VQUIT   - quit character; send SIGQUIT (need ISIG set)
  */
  term_struct.c_lflag = ISIG | ICANON | ECHO | ECHOE | VEOF | VERASE | VINTR | VKILL | VQUIT;
  tcsetattr(0, TCSANOW, &term_struct);
}

/* Open up the serial port and set parameters */
int init() {
  int usb_file_desc;
  struct termios term_struct;                  // Terminal control structure

  /* Open up serial port */
  usb_file_desc = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
  // TODO: check for error
  
  tcgetattr(usb_file_desc, &term_struct);                // Get the current settings

  term_struct.c_iflag = IGNPAR;                // Ignore parity
  term_struct.c_oflag = 0;                     // Don't need anything here

  term_struct.c_cflag = CS8 | CREAD | CLOCAL;  // 8 bit characters, enable receiver, no modem/status lines
  term_struct.c_lflag = 0;                     // Don't need anything here

  /* Set baud rate */
  cfsetispeed(&term_struct, BAUD_RATE);
  cfsetospeed(&term_struct, BAUD_RATE);

  // TODO: bits per character
  tcsetattr(usb_file_desc, TCSANOW, &term_struct);       // Save out the new setting

  return usb_file_desc;
}

void this_to_that(int f1, int f2) {
  char c;

  while(1) {
    /* Read from f1 and write out to f2 */
    if (read(f1, &c, 1)) {
      write(f2, &c, 1);
    }
  }
}

int main() {
  int usb_file_desc;

  /* Set up the terminal settings */
  setup_stdin();
  
  /* Initialze serial communication */
  usb_file_desc = init();  // TODO: error check
  
  /* Fork for reading and writing via the terminal */
  if(fork()) {
    this_to_that(usb_file_desc, 1);              // Read from serial and write to terminal
  }
  else {
    this_to_that(0, usb_file_desc);              // Read from terminal and write to serial
  }

  // TODO: exit condition
  return 1;
 }
