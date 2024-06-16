#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>

MODULE_AUTHOR("MGE");
MODULE_DESCRIPTION("A sample driver using interrupts");
MODULE_LICENSE("GPL");

#define GPIO2_START_ADDR 0x481AC000
#define GPIO2_SIZE (0x481ADFFF - GPIO2_START_ADDR)

#define CM_PER_START_ADDR 0x44e00000
#define CM_PER_SIZE       0x400
#define CM_PER_GPIO2_CLKCTRL 0xb0

#define GPIO_IRQSTATUS_0 0x2C
#define GPIO_IRQSTATUS_1 0x30
#define GPIO_RISINGDETECT 0x148
#define GPIO_FALLINGDETECT 0x14C

#define PIN_A_GPIO 69
#define PIN_A_FLAGS GPIOF_IN
#define PIN_A_LABEL "HI_PIN_A"

static irqreturn_t irq_handler_pin_a (int irq, void *dev_id) {
  printk (KERN_INFO "Hello from irq_handler_pin_a...\n");

  return IRQ_HANDLED;
}

static int __init hello_interrupts_start (void) {
  int retval, irq, regval;
  void __iomem *mem;
  void __iomem *cm_per;
  printk (KERN_INFO "Loading hello_interrupts module...\n");

  retval = gpio_request_one(PIN_A_GPIO, PIN_A_FLAGS, PIN_A_LABEL);

  if (retval) {
    printk (KERN_ERR "HI: Error: Failed to request GPIO pin#%i for IRQ (error %i)\n", PIN_A_GPIO, retval);

  }

  irq = gpio_to_irq (PIN_A_GPIO);
  if (irq < 0) {
    printk (KERN_ERR "HI: ERROR: Failed to obtain IRQ number for GPIO #%i (error %i)\n", PIN_A_GPIO, irq);

  }

  retval = request_irq (irq, irq_handler_pin_a, 0, PIN_A_LABEL, NULL);
  irq_set_irq_type (irq, IRQ_TYPE_EDGE_BOTH);

  if (retval) {
    printk (KERN_ERR "HI: ERROR: The requested IRQ line#%i from GPIO#%i (error %i)\n", irq, PIN_A_GPIO, retval);

  }
  else {
    printk (KERN_INFO "HI: Initialized GPIO #%i to IRQ #%i\n", PIN_A_GPIO, irq);
  }

  cm_per = ioremap(CM_PER_START_ADDR, CM_PER_SIZE);
  if(!cm_per) {
    printk (KERN_ERR "HI: ERROR: Failed to remap memory for CM_PER.\n");
    return 0;
  }
  iowrite32(0x02, cm_per + CM_PER_GPIO2_CLKCTRL);
  iounmap(cm_per);

  mem = ioremap(GPIO2_START_ADDR, GPIO2_SIZE);
  if(!mem) {
    printk (KERN_ERR "HI: ERROR: Failed to remap memory for GPIO Bank 2 IRQ pin configuration.\n");
    return 0;
  }

  regval = ioread32 (mem + GPIO_IRQSTATUS_0);
  regval |= (1 << 5);
  iowrite32 (regval, mem + GPIO_IRQSTATUS_0);

  regval = ioread32 (mem + GPIO_IRQSTATUS_1);
  regval |= (1 << 5);
  iowrite32 (regval, mem + GPIO_IRQSTATUS_1);

  regval = ioread32 (mem + GPIO_RISINGDETECT);
  regval |= (1 << 5);
  iowrite32(regval, mem + GPIO_RISINGDETECT);

  regval = ioread32 (mem + GPIO_FALLINGDETECT);
  regval |= (1 << 5);
  iowrite32 (regval, mem + GPIO_FALLINGDETECT);

  iounmap (mem);

  return 0;
}

static void __exit hello_interrupts_end(void) {

  printk ("HI: Releasing IRQ resources...\n");

  free_irq (gpio_to_irq (PIN_A_GPIO), NULL);
  gpio_free (PIN_A_GPIO);

  printk (KERN_INFO "Goodbye hello_interrupts!\n");
}

module_init (hello_interrupts_start);
module_exit (hello_interrupts_end);
