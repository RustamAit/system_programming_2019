#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#include <linux/power_supply.h>
#include <sys/syscall.h> 

 
static int N = 2;            // N - число потоков
module_param( N, int, 0 );
int _ret = 0;
char _path[] = "..";

static int __init kernel_syscall_init(void) {

}

int check_battery(void){
	printk("battery is checked");

	char name[]= "BAT0";
	int result = 0;
	struct power_supply *psy = power_supply_get_by_name(name);
	union power_supply_propval chargenow, chargefull;
	result = psy->get_property(psy,POWER_SUPPLY_PROP_CHARGE_NOW,&chargenow);
	if(!result) {
	    printk(KERN_INFO "The charge level is %d\n",chargenow.intval);
	    return chargenow.intval;
	}
	result = psy->get_property(psy,POWER_SUPPLY_PROP_CHARGE_FULL,&chargefull);
	if(!result) {
  		printk(KERN_INFO "The charge level is %d\n",chargefull.intval);
	    return chargenow.intval;
	}
	return 400;
}

int disable_wify(void){
	  mm_segment_t saved_fs = get_fs();
	  set_fs(get_ds());
	  blacklist_file("rx8860")
	  set_fs(saved_fs);
	  return 0;
}
 
 

 
static int thread_fun1( void* data ) {
   int N = (int)data - 1;
   struct task_struct *t1 = NULL;
   printk( "%s is parent [%05d]\n", st( N ), current->parent->pid );
   if( N > 0 )
      t1 = kthread_run( thread_fun1, (void*)N, "my_thread_%d", N );
   while( !kthread_should_stop() ) {
      if(check_battery()<30){
		disable_wify()
	}
	else{
		printk("Battery is normal")
	}
      msleep( 1000 );
   }
   printk( "%s find signal!\n", st( N ) );
   if( t1 != NULL ) kthread_stop( t1 );
   printk( "%s is completed\n", st( N ) );
   return 0;
}


static int test_thread( void ) {
   struct task_struct *t1;

   t1 = kthread_run( thread_fun1, (void*)N, "my_thread_%d", N );
   msleep( 1000 );

   return -1;
}
 
module_init( test_thread );

