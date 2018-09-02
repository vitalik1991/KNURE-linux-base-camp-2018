#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/timer.h>


#define CLASS_NAME   "my_leds" 
#define LABLE_NAME   "GPIO_15" 
#define GPIO_NUM 15

static struct class * sys_class;

static struct device * led_dev;


static struct timer_list blink_timer;

static bool mode_blink = false;
static bool state =true;




void blink_timer_callback(unsigned long data)
{
   static int count = 1;

   if (count !=5)
   {
      count++;
      state=!state;
      gpio_set_value(GPIO_NUM, (int)(state));
      mod_timer(&blink_timer, jiffies + msecs_to_jiffies(200));     

   }
   else
      count=1;

}




static ssize_t red_value_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
   printk( "write red_value");
   
   
   return 1;
}



static ssize_t red_mode_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
   if (buf[0]=='1')
   {
      mode_blink =true;
      
      mod_timer(&blink_timer, jiffies + msecs_to_jiffies(200));
   }

   if (buf[0]=='0')
      mode_blink =false;
   
   return count;
}


static ssize_t green_value_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
   printk( "write green_value");
   
   
   return 1;
}


static ssize_t green_mode_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
   printk( "write green_mode");
   
   
   return 1;
}


CLASS_ATTR_WO( green_mode);
CLASS_ATTR_WO( green_value);
CLASS_ATTR_WO( red_mode);
CLASS_ATTR_WO( red_value);





static void  setup_led_sysfs (void) {

int res;
sys_class = class_create(THIS_MODULE, CLASS_NAME);

if (IS_ERR(sys_class)) 

   printk( "bad class create\n" );

else{
         res = class_create_file(sys_class, &class_attr_red_value);
         res = class_create_file(sys_class, &class_attr_red_mode);
 
}
}

static int  remove_led_sysfs (void){

   class_remove_file(sys_class, &class_attr_red_value);
   class_remove_file(sys_class, &class_attr_red_mode);

   class_destroy(sys_class);
     printk( "remove_led_sysfs\n" );
     return 0;

}

static int led_probe(struct platform_device *dev)
{
led_dev= &dev->dev;
 
int ret = 0;
ret = gpio_request(GPIO_NUM, "mygpio");
if (ret) {
   
   dev_info(led_dev, "err gpio_request 15 driver\n");
   return 1;
}
gpio_direction_output(GPIO_NUM, (int)state);

setup_timer( &blink_timer, blink_timer_callback, 1);


dev_info(led_dev, "init GPIO driver\n");


 return 0;
}


static int led_remove(struct platform_device *device)
{
   gpio_set_value(GPIO_NUM, 0);
   gpio_free(GPIO_NUM);
   int ret = del_timer( &blink_timer );
    if (ret)
        printk(KERN_ALERT "The timer is still in use...\n");

    dev_info(led_dev, "remove GPIO driver\n");
    return 0;
}






struct platform_driver led_driver = {
    .probe = led_probe,
    .remove = led_remove,
    .driver = { .name = CLASS_NAME }
};





 






int __init led_init(void) {

printk( "led_init\n" );
setup_led_sysfs ();
int ret = 0;
ret = platform_driver_register(&led_driver);
if (ret) {
   pr_err("%s: unable to platform_driver_register\n", __func__);
}
return ret;
}

void led_cleanup(void) {
   platform_driver_unregister(&led_driver);
   remove_led_sysfs();
   return;
}

module_init(led_init);
module_exit(led_cleanup);

MODULE_AUTHOR("Martovytskyi Vitalii <Martovytskyi@gmail.com>");
MODULE_DESCRIPTION("Led driver");
MODULE_LICENSE("GPL");