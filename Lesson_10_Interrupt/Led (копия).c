#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/err.h>


MODULE_LICENSE( "GPL" );
MODULE_AUTHOR("Vitalii Martovytskyi <martovytskyi@gmail.com>");

#define CLASS_NAME   "gree_led" 

static struct class *sys_class;

static ssize_t red_value_show( struct class *class, struct class_attribute *attr, char *buf ) {
   
   printk(  "Read_red_value" );

   return strlen( buf );
   
}

static ssize_t red_value_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
   printk( "write red_value");
   
   
   return 0;
}


CLASS_ATTR_RW( red_value);



static ssize_t red_mode_show( struct class *class, struct class_attribute *attr, char *buf ) {
   
   printk(  "Read red_mode");

   return strlen( buf );
   
}

static ssize_t red_mode_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
   printk( "write red_mode");
   
   
   return 0;
}


CLASS_ATTR_RW( red_mode);



static ssize_t green_value_show( struct class *class, struct class_attribute *attr, char *buf ) {
   
   printk(  "Read green_value");
   
   return strlen( buf );
   
}

static ssize_t green_value_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
   printk( "write green_value");
   
   
   return 0;
}


CLASS_ATTR_RW( green_value);


static ssize_t green_mode_show( struct class *class, struct class_attribute *attr, char *buf ) {
   
   printk(  "Read green_mode" );

   return strlen( buf );
   
}

static ssize_t green_mode_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
   printk( "write green_mode");
   
   
   return 0;
}


CLASS_ATTR_RW( green_mode);


static void  setup_led_sysfs (void) {

int res;
sys_class = class_create(THIS_MODULE, CLASS_NAME);

if (IS_ERR(sys_class)) 

   printk( "bad class create\n" );

else{
         res = class_create_file(sys_class, &class_attr_red_value);
         res = class_create_file(sys_class, &class_attr_red_mode);
         res = class_create_file(sys_class, &class_attr_green_value);
         res = class_create_file(sys_class, &class_attr_green_mode);
}
}

static int  remove_led_sysfs (void){

   class_remove_file(sys_class, &class_attr_red_value);
   class_remove_file(sys_class, &class_attr_red_mode);
   class_remove_file(sys_class, &class_attr_green_value);
   class_remove_file(sys_class, &class_attr_green_mode);
   class_destroy(sys_class);
     printk( "remove_led_sysfs\n" );
     return 0;

}



int __init led_init(void) {

printk( "led_init\n" );
setup_led_sysfs ();
   return 0;
}

void led_cleanup(void) {
      printk( "led_cleanup\n" );

   remove_led_sysfs();
   return;
}

