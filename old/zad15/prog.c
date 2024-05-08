#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IMIE I NAZWISKO");
MODULE_DESCRIPTION("Przykładowy moduł jądra Linux");

static int numer_indeksu = 42069;

static int __init init_modul(void)
{
    printk(KERN_INFO "Ładowanie modułu. Autor: IMIE I NAZWISKO\n");
    return 0;
}

static void __exit exit_modul(void)
{
    printk(KERN_INFO "Usunięcie modułu. Numer indeksu: %d\n", numer_indeksu);
}

module_init(init_modul);
module_exit(exit_modul);

// przykladowy Makefile

obj-m += prog.o
CC := gcc-12

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules CC=$(CC)
	@echo "Building..."

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	@echo "Cleaning..."
