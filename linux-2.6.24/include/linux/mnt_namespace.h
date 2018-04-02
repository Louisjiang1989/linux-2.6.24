#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_
#ifdef __KERNEL__

#include <linux/mount.h>
#include <linux/sched.h>
#include <linux/nsproxy.h>

struct mnt_namespace {
	atomic_t		count; /* is the usage number of the processes using the namespace */
	struct vfsmount *	root;  /* point to the vfsmount instance of root directory */ 
	struct list_head	list; /* the start of a doubly linked list that holds all vfsmount instance linked by their mnt_list elements.*/
	wait_queue_head_t poll;
	int event;
};

extern struct mnt_namespace *copy_mnt_ns(unsigned long, struct mnt_namespace *,
		struct fs_struct *);
extern void __put_mnt_ns(struct mnt_namespace *ns);

static inline void put_mnt_ns(struct mnt_namespace *ns)
{
	if (atomic_dec_and_lock(&ns->count, &vfsmount_lock))
		/* releases vfsmount_lock */
		__put_mnt_ns(ns);
}

static inline void exit_mnt_ns(struct task_struct *p)
{
	struct mnt_namespace *ns = p->nsproxy->mnt_ns;
	if (ns)
		put_mnt_ns(ns);
}

static inline void get_mnt_ns(struct mnt_namespace *ns)
{
	atomic_inc(&ns->count);
}

#endif
#endif
