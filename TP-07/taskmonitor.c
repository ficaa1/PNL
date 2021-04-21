#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/pid.h>
#include<linux/slab.h>
#include<linux/kthread.h>
#include<linux/sched/task.h>
#include<linux/sched/cputime.h>

#define SECONDS 5

MODULE_DESCRIPTION("Module pour surveiller un processus");
MODULE_AUTHOR("Filip BILJIC");
MODULE_LICENSE("GPL");

static unsigned frequency = 1;
static unsigned int target = 1;
module_param(target, int, 0);
MODULE_PARM_DESC(target, "PID du processus à surveiller");
struct task_sample {
	struct list_head list;
	u64 utime;
	u64 stime;
};
struct task_monitor {
	struct task_sample samples;
	struct pid *pid;
	struct mutex mutex;
	int samplecount;
};



static struct task_monitor task_monitor;
static struct task_struct *monitor_thread;
static struct task_sample task_sample;


static bool get_sample(struct task_monitor *tm, struct task_sample *sample){

	struct task_struct *task;
	bool alive = false;

	task = get_pid_task(tm->pid, PIDTYPE_PID);
	if (!task) {
		pr_err("can't find task for pid %u\n", pid_nr(tm->pid));
		return alive;
	}

	task_lock(task);
	alive = pid_alive(task);
	if (alive)
		task_cputime(task, &sample->utime, &sample->stime);
	task_unlock(task);
	put_task_struct(task);
	return alive;
}

static void save_sample(void) {

	struct task_sample *tt;
	
	tt = kmalloc(sizeof(struct task_sample), GFP_KERNEL);
	if(!tt){
		pr_err("allocation error");
	}
	mutex_lock(&task_monitor.mutex);
	get_sample(&task_monitor, tt);
	list_add_tail(&tt->list, &task_monitor.samples.list);
	task_monitor.samplecount++;
	mutex_unlock(&task_monitor.mutex);
	kfree(tt);

}


static int monitor_pid(pid_t npid){
	struct pid *pid;
	if(task_monitor.pid != NULL)
		put_pid(task_monitor.pid);
	pid = find_get_pid(npid);
	if (pid == NULL){
		return -1;
	}
	task_monitor.pid = pid;
	pr_info("pid_t: %d, pid->level: %d\n", npid, pid->level);
	return 0;

}

static int monitor_fn(void *unused)
{
	struct task_struct *task;

	while (!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);		
		if (schedule_timeout(max(HZ/frequency/20, 1U/20)))
			return -EINTR;
		save_sample();
	}
	monitor_thread=NULL;
	return 0;
}


static int cmd_start(void)
{
	if (monitor_thread) {
		pr_info("Thread already running !\n");
		return 1;
	}
	monitor_pid(target);
	if (task_monitor.pid) {
		monitor_thread = kthread_run(monitor_fn, NULL, "monitor fn");
		pr_info("Thread started, pid: %d\n", monitor_thread->pid);
		return 0;
	}
	pr_info("%d does not exist!\n", target);
	return 1;
}

static int cmd_stop(void)
{
	if (monitor_thread) {
		kthread_stop(monitor_thread);
		pr_info("Thread stopped\n");
		return 0;
	}
	pr_info("No thread running\n");
	return 1;
}

static ssize_t task_monitor_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	return snprintf(buf, 30, "pid %d usr %lld sys %lld\n", target,
			task_sample.utime / 1000000,
			task_sample.stime / 1000000);
}

static ssize_t task_monitor_store(struct kobject *kobj,
				 struct kobj_attribute *attr, const char *buf,
				 size_t count)
{

	if(strcmp(buf, "start") == 0){
		pr_info("START \n");
		cmd_start();
	}
	else if (strcmp(buf, "stop") == 0){
		pr_info("STOP \n");
		cmd_stop;
	}
	return count;
}

static struct kobj_attribute hello_kobj = __ATTR_RW(task_monitor);

static int task_monitor_init(void)
{
	int error;
	mutex_init(&task_monitor.mutex);
	task_monitor.samplecount = 0;
	INIT_LIST_HEAD(&task_monitor.samples.list);
	if(error = sysfs_create_file(kernel_kobj, &hello_kobj.attr))
		return error;
	cmd_start();
	pr_info("task_monitor module loaded\n");
	return 0;
}

static void task_monitor_exit(void)
{
	cmd_stop();
	sysfs_remove_file(kernel_kobj, &hello_kobj.attr);
	pr_info("task_monitor module unloaded\n");

	if(task_monitor.pid != NULL)
		put_pid(task_monitor.pid);
}

module_init(task_monitor_init);
module_exit(task_monitor_exit);
