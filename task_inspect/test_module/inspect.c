#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#define __NR_inspect 312

extern void *sys_call_table[];

asmlinkage long *original_inspect;

asmlinkage long inspect_call(void) {
  struct task_struct* cur_task;
  pid_t pid;
  cur_task = get_current();
  pid = cur_task->pid;

  printk(KERN_INFO "SHIT. It worked. PID: %d\n", pid);
  return 0;
}

static int init(void) {
  pte_t *pte;
  unsigned int look;

  printk(KERN_INFO "Test inspect module loaded.\n");
  printk(KERN_INFO "sys_call_table %p\n", sys_call_table);

  pte = lookup_address(sys_call_table, &look);
  pte->pte |= _PAGE_RW;

  original_inspect = sys_call_table[__NR_inspect];
  sys_call_table[__NR_inspect]=inspect_call;

  return 0;
}

static void cleanup(void) {
  sys_call_table[__NR_inspect] = original_inspect;
  printk(KERN_INFO "Test inspect module un-loaded.\n");
}

module_init(init);
module_exit(cleanup);

MODULE_AUTHOR("HA! It's me. That's great.\n");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("None to speak of.");
