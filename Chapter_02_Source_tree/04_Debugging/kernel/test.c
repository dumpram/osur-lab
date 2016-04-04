#include <kernel/kprint.h>
#include <kernel/test.h>

static int x __attribute__((section (".varx"))) = 0xA5A5A5A5;

void ispisi_x() {
    kprintf("\n # ISPIS ADRESA # \n");
	kprintf("Adresa varijable x: %x\n", &x);
    kprintf("Vrijednost varijable x: %x\n", x);
    kprintf("Adresa funkcije ispisi_x: %x\n\n", ispisi_x);
}
