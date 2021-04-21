#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/pid.h>
#include<linux/slab.h>
#include<linux/kthread.h>
#include<linux/sched/task.h>

#define SECONDS 5

MODULE_DESCRIPTION("Module pour surveiller un processus");
MODULE_AUTHOR("Filip BILJIC");
MODULE_LICENSE("GPL");


static int target;
module_param(target, int, 0);
MODULE_PARM_DESC(target, "PID du processus à surveiller");

struct task_monitor {
	struct list_head list;
	struct pid *pid;
	struct mutex mutex;
	int samplecount;
};

struct task_times {
	u64 utime;
	u64 stime;
};

static struct task_monitor task_monitor;
static struct task_struct *monitor_thread;
static struct task_times task_times;


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
		if (task_monitor.pid == NULL)
			continue;
		task = get_pid_task(task_monitor.pid, PIDTYPE_PID);
		if (pid_alive(task)) {
			task_times.utime = task->utime;
			task_times.stime = task->stime;
		} else {
			pr_info("Tache morte, fin du thread...");
			break;
		}
		put_task_struct(task);
		set_current_state(TASK_UNINTERRUPTIBLE);
		schedule_timeout(SECONDS * HZ);
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

static ssize_t taskmonitor_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	return snprintf(buf, 30, "pid %d usr %lld sys %lld\n", target,
			task_times.utime / 1000000,
			task_times.stime / 1000000);
}

static ssize_t taskmonitor_store(struct kobject *kobj,
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

static struct kobj_attribute hello_kobj = __ATTR_RW(taskmonitor);

static int taskmonitor_init(void)
{
	int error;
	mutex_init(&task_monitor.mutex);
	task_monitor.samplecount = 0;
	INIT_LIST_HEAD(&task_monitor.list);
	if(error = sysfs_create_file(kernel_kobj, &hello_kobj.attr))
		return error;
	cmd_start();
	pr_info("taskmonitor module loaded\n");
	return 0;
}

static void taskmonitor_exit(void)
{
	cmd_stop();
	sysfs_remove_file(kernel_kobj, &hello_kobj.attr);
	pr_info("taskmonitor module unloaded\n");

	if(task_monitor.pid != NULL)
		put_pid(task_monitor.pid);
}

module_init(taskmonitor_init);
module_exit(taskmonitor_exit);
