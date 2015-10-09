/*
** main.c for Kernel in /home/mestag_a/tests/Kernel
** 
** Made by alexis mestag
** Login   <mestag_a@epitech.net>
** 
** Started on  Fri Oct  9 13:40:50 2015 alexis mestag
** Last update Fri Oct  9 17:12:15 2015 alexis mestag
*/

typedef	unsigned char	uint8_t;
typedef	unsigned short	uint16_t;

struct vga_text_char {
  uint16_t ascii_code		: 8;
  uint16_t fg_color_rgb		: 3;
  uint16_t fg_color_intensity	: 1;
  uint16_t bg_color_rgb		: 3;
  uint16_t bg_color_intensity	: 1;
} __attribute__((packed));

void	outw(unsigned short val, unsigned short port) {
  asm volatile("outb %w0, %w1\n\t" :  : "a" (val), "d" (port));
}

uint8_t	inb(unsigned short port) {
  uint8_t	res;

  asm volatile("inb %w1, %0\n\t" : "=a" (res) : "Nd" (port));
  return (res);
}

void	enable_cursor() {
  outb(0x3d4, 0xa);
  char curstart = inb(0x3d5) & 0x1F;

  outb(0x3d4, 0xa);
  outb(0x3d5, curstart | 0x20);
}

uint16_t	get_cursor() {
  uint16_t	low = 0, high = 0, ret = 0;

  outb(0x3D4, 0x0F);
  high = inb(0x3D5);
  outb(0x3D4, 0x0E);
  low = inb(0x3D5);

  ret = (high << 8) | low;
  return (ret); 
}

void	update_cursor(int row, int col) {
  uint16_t	pos = row * 80 + col;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t) (pos & 0xFF));

  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void	printk(const char *msg) {
  struct vga_text_char	*vga = (void *) 0xb8000;

  /* get cursor pos */
  uint16_t		offset = get_cursor();

  struct vga_text_char	*vga_begin = vga + offset;
  for(int i = 0; msg[i]; ++i) {
    vga_begin[i].ascii_code = msg[i];
    offset++;
  }

  /* set cursor pos */
  uint16_t	row = offset / 80, col = offset % 80;
  update_cursor(row, col);
  /* offset = get_cursor(); */
}

int	main() {
  enable_cursor();
  /* outb(0x3D4, 0x0A); */
  /* outb(0x3D5, 0x00); */
  printk("Yolo");
  printk("Bitch");
  return (0);
}
