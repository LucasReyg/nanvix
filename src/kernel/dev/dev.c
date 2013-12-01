/*
 * Copyright(C) 2011-2013 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *
 * dev/dev.c - Uniform device interface.
 */

#include <dev/tty.h>
#include <nanvix/const.h>
#include <nanvix/dev.h>
#include <errno.h>

/*============================================================================*
 *                            Character Devices                               *
 *============================================================================*/

/* Number of character devices. */
#define NR_CHRDEV 2

/*
 * Character devices table.
 */
PRIVATE const struct cdev *cdevsw[NR_CHRDEV] = {
	NULL, /* /dev/null */
	NULL  /* /dev/tty  */
};

/*
 * Registers a character device.
 */
PUBLIC int cdev_register(unsigned major, const struct cdev *dev)
{
	/* Invalid major number? */
	if (major >= NR_CHRDEV)
		return (-EINVAL);
	
	/* Device already registered? */
	if ((major == NULL_MAJOR) || (cdevsw[major] != NULL))
		return (-EBUSY);
	
	/* Register character device. */
	cdevsw[major] = dev;
	
	return (0);
}

/*
 * Writes to a character device.
 */
PUBLIC ssize_t cdev_write(dev_t dev, const void *buf, size_t n)
{	
	/* Null device. */
	if (MAJOR(dev) == NULL_MAJOR)
		return ((ssize_t)n);
	
	/* Invalid device. */
	if (cdevsw[MAJOR(dev)] == NULL)
		return (-EINVAL);
	
	/* Operation not supported. */
	if (cdevsw[MAJOR(dev)]->write == NULL)
		return (-ENOTSUP);
	
	return (cdevsw[MAJOR(dev)]->write(MINOR(dev), buf, n));
}

/*
 * Reads from a character device.
 */
PUBLIC ssize_t cdev_read(dev_t dev, void *buf, size_t n)
{
	/* Null device. */
	if (MAJOR(dev) == NULL_MAJOR)
		return (0);
	
	/* Invalid device. */
	if (cdevsw[MAJOR(dev)] == NULL)
		return (-EINVAL);
	
	/* Operation not supported. */
	if (cdevsw[MAJOR(dev)]->read == NULL)
		return (-ENOTSUP);
	
	return (cdevsw[MAJOR(dev)]->read(MINOR(dev), buf, n));
}

/*============================================================================*
 *                              Block Devices                                 *
 *============================================================================*/

/* Number of block devices. */
#define NR_BLKDEV 1

/*
 * Block devices table.
 */
PRIVATE const struct bdev *bdevsw[NR_BLKDEV] = {
	NULL /* /dev/ramdisk */
};

/*
 * Registers a block device.
 */
PUBLIC int bdev_register(unsigned major, const struct bdev *dev)
{
	/* Invalid major number? */
	if (major >= NR_BLKDEV)
		return (-EINVAL);
	
	/* Device already registered? */
	if (bdevsw[major] != NULL)
		return (-EBUSY);
	
	/* Register block device. */
	bdevsw[major] = dev;
	
	return (0);
}

/*
 * Writes to a block device.
 */
PUBLIC ssize_t bdev_write(dev_t dev, const char *buf, size_t n, off_t off)
{
	/* Invalid device. */
	if (bdevsw[MAJOR(dev)] == NULL)
		return (-EINVAL);
	
	/* Operation not supported. */
	if (bdevsw[MAJOR(dev)]->write == NULL)
		return (-ENOTSUP);
	
	return (bdevsw[MAJOR(dev)]->write(MINOR(dev), buf, n, off));
}

/*
 * Reads from a block device.
 */
PUBLIC ssize_t bdev_read(dev_t dev, char *buf, size_t n, off_t off)
{
	/* Invalid device. */
	if (bdevsw[MAJOR(dev)] == NULL)
		return (-EINVAL);
		
	/* Operation not supported. */
	if (bdevsw[MAJOR(dev)]->read == NULL)
		return (-ENOTSUP);
	
	return (bdevsw[MAJOR(dev)]->read(MINOR(dev), buf, n, off));
}

/*============================================================================*
 *                                 Devices                                    *
 *============================================================================*/

/*
 * Initializes character device drivers.
 */
PUBLIC void dev_init()
{
	/* Initializes character devices. */
	tty_init();
}
