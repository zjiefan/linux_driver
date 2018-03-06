/* **************** LFD331:4.2 s_16/lab1_putget.c **************** */
/*
 * The code herein is: Copyright the Linux Foundation, 2015
 *
 * This Copyright is retained for the purpose of protecting free
 * redistribution of source.
 *
 *     URL:    http://training.linuxfoundation.org
 *     email:  trainingquestions@linuxfoundation.org
 *
 * This code is distributed under Version 2 of the GNU General Public
 * License, which you should have received with the source.
 *
 */
/*
 * Using get_user() and put_user().
 *
 * Adapt your character driver to use get_user() and put_user().
 *
 @*/

#include <linux/module.h>

/* #include "lab_char.h" */
#include "lab_miscdev.h"

static inline ssize_t
mycdrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = 0, maxbytes, bytes_to_do;
	char *tmp = ramdisk + *ppos;
	maxbytes = ramdisk_size - *ppos;
	bytes_to_do = maxbytes > lbuf ? lbuf : maxbytes;
	if (bytes_to_do == 0)
		dev_warn(my_dev, "Reached end of the device on a read");
	while ((nbytes < bytes_to_do) && !put_user(*tmp, (buf + nbytes))) {
		nbytes++;
		tmp++;
	}
	*ppos += nbytes;
	dev_info(my_dev, "\n Leaving the   READ function, nbytes=%d, pos=%d\n",
		 nbytes, (int)*ppos);
	return nbytes;
}

static inline ssize_t
mycdrv_write(struct file *file, const char __user *buf, size_t lbuf,
	     loff_t *ppos)
{
	int nbytes = 0, maxbytes, bytes_to_do;
	char *tmp = ramdisk + *ppos;
	maxbytes = ramdisk_size - *ppos;
	bytes_to_do = maxbytes > lbuf ? lbuf : maxbytes;
	if (bytes_to_do == 0)
		dev_warn(my_dev, "Reached end of the device on a write");
	while ((nbytes < bytes_to_do) && !get_user(*tmp, (buf + nbytes))) {
		nbytes++;
		tmp++;
	}
	*ppos += nbytes;
	dev_info(my_dev, "\n Leaving the   WRITE function, nbytes=%d, pos=%d\n",
		 nbytes, (int)*ppos);
	return nbytes;
}

static const struct file_operations mycdrv_fops = {
	.owner = THIS_MODULE,
	.read = mycdrv_read,
	.write = mycdrv_write,
	.open = mycdrv_generic_open,
	.release = mycdrv_generic_release,
};

module_init(my_generic_init);
module_exit(my_generic_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LFD331:4.2 s_16/lab1_putget.c");
MODULE_LICENSE("GPL v2");
