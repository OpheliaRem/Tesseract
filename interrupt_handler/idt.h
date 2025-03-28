#ifndef IDT_C_FILE
#define IDT_C_FILE

#define IDT_SIZE 256

extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long* idt_ptr);

typedef struct {
    unsigned short offset_lowerbits;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attribute;
    unsigned short offset_higherbits;
} IdtEntry;

IdtEntry idt[IDT_SIZE];

void idt_init(void) {
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    keyboard_address = (unsigned long)keyboard_handler;

    idt[0x21].offset_lowerbits = keyboard_address & 0xffff;
    idt[0x21].selector = 0x08;
    idt[0x21].zero = 0;
    idt[0x21].type_attribute = 0x8e;
    idt[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    write_port(0x20, 0x11);
    write_port(0xA0, 0x11);

    write_port(0x21, 0x20);
    write_port(0xA1, 0x28);

    write_port(0x21, 0x00);
    write_port(0xA1, 0x00);

    write_port(0x21, 0x01);
    write_port(0xA1, 0x01);

    write_port(0x21, 0xff);
    write_port(0xA1, 0xff);

    idt_address = (unsigned long)idt;
    idt_ptr[0] = (sizeof(IdtEntry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    load_idt(idt_ptr);  
}

void keyboard_init(void) {
    write_port(0x21, 0xFD);
}

#endif