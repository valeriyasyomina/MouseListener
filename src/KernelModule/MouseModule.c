#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/input.h>
#include <linux/platform_device.h>


#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/usb.h>
/* Представление устройства ввода */
/* Representation of an input device */
struct input_dev *vms_input_dev; 

/* Структура представляющая устройство*/
/* Device structure */
static struct platform_device *vms_dev; 
/*  Метод доступный через sysfs для получения 
симулируемых координат драйвером виртуальной
мыши */

/* Sysfs method to input simulated
coordinates to the virtual mouse driver */

static ssize_t write_vms(struct device *dev,
          struct device_attribute *attr,
          const char *buffer, size_t count)
{
  int x,y;
  sscanf(buffer, "%d%d", &x, &y);




  printk(KERN_INFO "EBAT' %s ;)))))\n",buffer);

 input_report_rel(vms_input_dev, REL_X, x);
 input_report_rel(vms_input_dev, REL_Y, y);
 input_sync(vms_input_dev);
 return count;
}

/* Привязка метода к файлы в sysfs */
/* Attach the sysfs write method */
DEVICE_ATTR(coordinates, 0644, NULL, write_vms);
/* Дексрипторы атрибутов */
/* Attribute Descriptor */
static struct attribute *vms_attrs[] = {
 &dev_attr_coordinates.attr,
 NULL
};

/* Группы атрибутов */
/* Attribute group */
static struct attribute_group vms_attr_group = {
 .attrs = vms_attrs,
};

/* Инициализация драйверов */
/* Driver Initialization */
static int __init display_init(void)
{
/* регистрация устройства*/
 /* Register a platform device */
   vms_dev = platform_device_register_simple("vms", -1, NULL, 0);
   if (IS_ERR(vms_dev)) {
     PTR_ERR(vms_dev);
     printk("vms_init: error\n");
   }
  /* Создание файла в sysfs для чтения симулируемых координат */
   /* Create a sysfs node to read simulated coordinates */
    sysfs_create_group(&vms_dev->dev.kobj, &vms_attr_group);
  /* Выделение памяти для структур устройства ввода */
    /* Allocate an input device data structure */
    vms_input_dev = input_allocate_device();
    if (!vms_input_dev) {
      printk("Bad input_alloc_device()\n");
    }
  /* Анонс того, что виртуальная мышь будет генерировать 
  координаты связанные с предыдущими*/
    /* Announce that the virtual mouse will generate
       relative coordinates */
   /* set_bit(EV_REL, vms_input_dev->evbit);
    set_bit(REL_X, vms_input_dev->relbit);
    set_bit(REL_Y, vms_input_dev->relbit);*/

    vms_input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
    vms_input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) |
      BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
    vms_input_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
    vms_input_dev->keybit[BIT_WORD(BTN_MOUSE)] |= BIT_MASK(BTN_SIDE) |
      BIT_MASK(BTN_EXTRA);
    vms_input_dev->relbit[0] |= BIT_MASK(REL_WHEEL);

  
  /* регистрация в подсистеме ввода */
    /* Register with the input subsystem */
    input_register_device(vms_input_dev);
    printk("Virtual Mouse Driver Initialized.\n");
    return 0;
}
/* Выход из драйвера */
/* Driver Exit */
static void vms_cleanup(void)
{
  /* Отмена регистрации в подсистеме ввода */
    /* Unregister from the input subsystem */
    input_unregister_device(vms_input_dev);
  /* Очистка файла в sysfs */
    /* Cleanup sysfs node */
    sysfs_remove_group(&vms_dev->dev.kobj, &vms_attr_group);
  /* Отмена регистрации драйвера */
    /* Unregister driver */
    platform_device_unregister(vms_dev);
    return;
}
module_init(display_init);
module_exit(vms_cleanup);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abakumkin Alexey");
MODULE_DESCRIPTION("USB HID Line Display Device Driver");