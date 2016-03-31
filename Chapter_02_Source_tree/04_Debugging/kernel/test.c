#include <stdio.h>
#include <kernel/test.h>

static int x __attribute__((section (".varx")));

void ispisi_x() {
	printf("Adresa varijable x: %x\r\n", &x);
    printf("Vrijednost varijable x: %x\r\n", x);
    printf("Adresa funkcije ispisi_x: %x\r\n", ispisi_x);
}
