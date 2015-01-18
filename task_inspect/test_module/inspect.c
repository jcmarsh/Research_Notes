#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>

#define __NR_inspect 312

extern void *sys_call_table[];

asmlinkage long *original_inspect;

int count_list(struct list_head* lh) {
  struct list_head* cursor = NULL;
  int count = 0;

  list_for_each(cursor, lh) {
    count++;
  }
  return count;
}

int count_hlist(struct hlist_head* hlh) {
  struct hlist_node* cursor = NULL;
  int count = 0;

  hlist_for_each(cursor, hlh) {
    count++;
  }
  return count;
}

int count_plist(struct plist_head* plh) {
  struct plist_head* cursor = NULL;
  int count = 0;

  plist_for_each(cursor, plh) {
    count++;
  }
  return count;
}

asmlinkage long inspect_call(void) {
  struct task_struct* cur_task;
  pid_t pid;
  cur_task = get_current();
  pid = cur_task->pid;

  printk(KERN_INFO "SHIT. It worked. PID: %d\n", pid);
  printk(KERN_INFO "\tanon_vm_chain (mm):\t%d\n", count_list(&(cur_task->mm->mmap->anon_vma_chain)));
  printk(KERN_INFO "\tanon_vm_chain (active_mm):\t%d\n", count_list(&(cur_task->active_mm->mmap->anon_vma_chain)));
  printk(KERN_INFO "\tpreempt_notifiers:\t%d\n", count_hlist(&(cur_task->preempt_notifiers)));
  //printk(KERN_INFO "\trcu_node_entry:\t%d\n", count_list(&(cur_task->rcu_node_entry)));
  printk(KERN_INFO "\ttasks:\t%d\n", count_list(&(cur_task->tasks)));
  printk(KERN_INFO "\tchildren:\t%d\n", count_list(&(cur_task->children)));
  printk(KERN_INFO "\tsibling:\t%d\n", count_list(&(cur_task->sibling)));
  printk(KERN_INFO "\tptraced:\t%d\n", count_list(&(cur_task->ptraced)));
  printk(KERN_INFO "\tptrace_entry:\t%d\n", count_list(&(cur_task->ptrace_entry)));
  printk(KERN_INFO "\tthread_group:\t%d\n", count_list(&(cur_task->thread_group)));
  //printk(KERN_INFO "\tcpu_timers:\t%d\n", count_list(&(cur_task->cpu_timers)));
  printk(KERN_INFO "\tpi_waiters:\t%d\n", count_plist(&(cur_task->pi_waiters)));
  printk(KERN_INFO "\tcg_list:\t%d\n", count_list(&(cur_task->cg_list)));
  printk(KERN_INFO "\tpi_state_list:\t%d\n", count_list(&(cur_task->pi_state_list)));
  printk(KERN_INFO "\tperf_event_list:\t%d\n", count_list(&(cur_task->perf_event_list)));
  printk(KERN_INFO "END OF TASK_STRUCT\n");
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
