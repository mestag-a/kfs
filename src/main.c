/*
** main.c for Kernel in /home/mestag_a/tests/Kernel
** 
** Made by alexis mestag
** Login   <mestag_a@epitech.net>
** 
** Started on  Fri Oct  9 13:40:50 2015 alexis mestag
** Last update Sat Oct 10 17:05:58 2015 alexis mestag
*/

typedef	unsigned char	uint8_t;
typedef	unsigned short	uint16_t;

/*
** UART Line control structure
** For assignment : 8N1: 0b00000011
*/
struct		lc_t {
  uint8_t	word_length	: 2; /* 0b11 for 8 bits word length */
  uint8_t	stop_bit	: 1; /* 0b0 for 1 stop bit */
  uint8_t	parity		: 3; /* 0b0 for no parity check */
  uint8_t	break_signal	: 1;
  uint8_t	dlab		: 1;
} __attribute__((packed));

struct		vga_text_char {
  uint16_t	ascii_code		: 8;
  uint16_t	fg_color_rgb		: 3;
  uint16_t	fg_color_intensity	: 1;
  uint16_t	bg_color_rgb		: 3;
  uint16_t	bg_color_intensity	: 1;
} __attribute__((packed));

struct		uint16_s {
  uint8_t	low;
  uint8_t	high;
} __attribute__((packed));

/*
** Union to avoid bitwise operations
*/
union		cursor {
  uint16_t		offset;
  struct uint16_s	bytes;
};

void	outb(uint8_t value, uint16_t port) {
  asm volatile("outb %0, %1\n\t" :  : "a" (value), "d" (port));
}

uint8_t	inb(uint16_t port) {
  uint8_t	value;

  asm volatile("inb %1, %0\n\t" : "=a" (value) : "d" (port));
  return (value);
}

uint16_t	get_cursor() {
  union cursor	c;

  outb(0x0f, 0x3d4);
  c.bytes.low = inb(0x3d5);
  outb(0x0e, 0x3d4);
  c.bytes.high = inb(0x3d5);
  return (c.offset);
}

void	update_cursor(uint16_t row, uint16_t col) {
  union cursor	c = { .offset = row * 80 + col };

  outb(0x0f, 0x3d4);
  outb(c.bytes.low, 0x3d5);
  outb(0x0e, 0x3d4);
  outb(c.bytes.high, 0x3d5);
}

void	printk(const char *msg) {
  struct vga_text_char	*vga = (void *) 0xb8000;
  uint16_t		offset = get_cursor();
  uint16_t		row = offset / 80, col = offset % 80;

  for(int i = 0; msg[i]; ++i) {
    switch (msg[i]) {
    case '\b':
      if (col == 0) {
	col = 79; row--;
      } else {
	col--;
      }
      break;
    case '\t':
      while (++col % 8 != 0);
      break;
    case '\n': row++; col = 0;
      break;
    case '\v':
      break;
    case '\f':
      break;
    case '\r': col = 0;
      break;
    default:
      vga[offset].ascii_code = msg[i];
      if (col == 79) {
	col = 0; row++;
      } else {
	col++;
      }
      break;
    }    
    offset = row * 80 + col;
  }
  update_cursor(row, col);
}

int		main() {
  printk("Hello World!\n");
  printk("\bYolo\tooo\rYala\n");
  printk("0123456789\n");
  printk("01234567\t89\n");
  printk("0123456\t789\n");
  return (0);
}
