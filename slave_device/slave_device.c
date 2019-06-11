#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/net.h>
#include <net/sock.h>
#include <asm/processor.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/debugfs.h>
#include <linux/mm.h>
#include <asm/page.h>


#ifndef VM_RESERVED
#define VM_RESERVED   (VM_DONTEXPAND | VM_DONTDUMP)
#endif

#define slave_IOCTL_CREATESOCK 0x12345677
#define slave_IOCTL_MMAP 0x12345678
#define slave_IOCTL_EXIT 0x12345679

#define BUF_SIZE 512
#define MMAP_SIZE PAGE_SIZE*100



struct dentry  *file1;//debug file

typedef struct socket * ksocket_t;

//functions about kscoket are exported,and thus we use extern here
extern ksocket_t ksocket(int domain, int type, int protocol);
extern int kconnect(ksocket_t socket, struct sockaddr *address, int address_len);
extern ssize_t krecv(ksocket_t socket, void *buffer, size_t length, int flags);
extern int kclose(ksocket_t socket);
extern unsigned int inet_addr(char* ip);
extern char *inet_ntoa(struct in_addr *in); //DO NOT forget to kfree the return pointer

static int __init slave_init(void);
static void __exit slave_exit(void);

void vm_open(struct vm_area_struct *vma);
void vm_close(struct vm_area_struct *vma);
static int vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf);

int slave_close(struct inode *inode, struct file *filp);
int slave_open(struct inode *inode, struct file *filp);
static long slave_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);
int receive_msg(struct file *filp, char *buf, size_t count, loff_t *offp );
int slave_mmap(struct file *filp, struct vm_area_struct *vm);

static mm_segment_t old_fs;
static ksocket_t sockfd_cli;//socket to the master server
static struct sockaddr_in addr_srv; //address of the master server

static char *userdata;
char socket_buf[BUF_SIZE];
int socket_bufSeek = 0;
int socket_bufRemain = 0;

//file operations
static struct file_operations slave_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = slave_ioctl,
	.open = slave_open,
	.read = receive_msg,
	.release = slave_close,
	.mmap = slave_mmap
};

struct vm_operations_struct vm_ops = {
	.open	= vm_open,
	.close	= vm_close,
	.fault	= vm_fault
};

//device info
static struct miscdevice slave_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "slave_device",
	.fops = &slave_fops
};

static int __init slave_init(void)
{
	int ret;
	file1 = debugfs_create_file("slave_debug", 0644, NULL, NULL, &slave_fops);

	//register the device
	if( (ret = misc_register(&slave_dev)) < 0){
		printk(KERN_ERR "misc_register failed!\n");
		return ret;
	}

	printk(KERN_INFO "slave has been registered!\n");

	return 0;
}

static void __exit slave_exit(void)
{
	misc_deregister(&slave_dev);
	printk(KERN_INFO "slave exited!\n");
	debugfs_remove(file1);
}

void vm_open(struct vm_area_struct *vma){
}
void vm_close(struct vm_area_struct *vma){
}
static int vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf){
	vmf->page = virt_to_page(vma->vm_pgoff<<PAGE_SHIFT);
	get_page(vmf->page);

	return 0;
}

int slave_close(struct inode *inode, struct file *filp)
{
	kfree(userdata);
	return 0;	
}

int slave_open(struct inode *inode, struct file *filp)
{
	userdata = kzalloc(MMAP_SIZE, GFP_KERNEL);
	return 0;
}
static long slave_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	long ret = -EINVAL;

	int addr_len ;
	unsigned int i;
	size_t len, data_size = 0;
	char *tmp, ip[20], buf[BUF_SIZE];
	struct page *p_print;
	unsigned char *px;

    pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
    pte_t *ptep, pte;
	old_fs = get_fs();
	set_fs(KERNEL_DS);

	int nowNum;
	int temp;
	switch(ioctl_num){
		case slave_IOCTL_CREATESOCK:// create socket and connect to master

			if(copy_from_user(ip, (char*)ioctl_param, sizeof(ip)))
				return -ENOMEM;

			sprintf(current->comm, "ksktcli");

			memset(&addr_srv, 0, sizeof(addr_srv));
			addr_srv.sin_family = AF_INET;
			addr_srv.sin_port = htons(2325);
			addr_srv.sin_addr.s_addr = inet_addr(ip);
			addr_len = sizeof(struct sockaddr_in);

			sockfd_cli = ksocket(AF_INET, SOCK_STREAM, 0);
			printk("sockfd_cli = 0x%p  socket is created\n", sockfd_cli);
			if (sockfd_cli == NULL)
			{
				printk("socket failed\n");
				return -1;
			}
			if (kconnect(sockfd_cli, (struct sockaddr*)&addr_srv, addr_len) < 0)
			{
				printk("connect failed\n");
				return -1;
			}
			tmp = inet_ntoa(&addr_srv.sin_addr);
			printk("connected to : %s %d\n", tmp, ntohs(addr_srv.sin_port));
			kfree(tmp);
			ret = 0;
			break;
		case slave_IOCTL_MMAP:
			memcpy(userdata, socket_buf+socket_bufSeek, socket_bufRemain);
			nowNum = socket_bufRemain;
			while(nowNum < MMAP_SIZE){
				temp = krecv(sockfd_cli, socket_buf, sizeof(socket_buf), 0);
				if(temp == 0)
					break;
				if(nowNum+temp < MMAP_SIZE){
					socket_bufRemain = 0;
					socket_bufSeek = 0;
					memcpy(userdata+nowNum, socket_buf, temp);
					nowNum += temp;
				}
				else{
					socket_bufRemain = nowNum + temp - MMAP_SIZE;
					socket_bufSeek = temp - socket_bufRemain;
					memcpy(userdata+nowNum, socket_buf, socket_bufSeek);	
					nowNum = MMAP_SIZE;
				}	
			}
			ret = nowNum;
			break;
		case slave_IOCTL_EXIT:
			if(kclose(sockfd_cli) == -1)
			{
				printk("kclose cli error\n");
				return -1;
			}
			set_fs(old_fs);
			ret = 0;
			break;
		default:
            pgd = pgd_offset(current->mm, ioctl_param);
			pud = pud_offset(pgd, ioctl_param);
			pmd = pmd_offset(pud, ioctl_param);
			ptep = pte_offset_kernel(pmd , ioctl_param);
			pte = *ptep;
			printk("slave: %lX\n", pte);
			ret = 0;
			break;
	}

	return ret;
}

int receive_msg(struct file *filp, char *buf, size_t count, loff_t *offp )
{
//call when user is reading from this device
	char msg[BUF_SIZE];
	size_t len;
	len = krecv(sockfd_cli, msg, sizeof(msg), 0);
	if(copy_to_user(buf, msg, len))
		return -ENOMEM;	
	return len;
}

int slave_mmap(struct file *filp, struct vm_area_struct *vm){
	//pgoff: page number of our buffer

	vm->vm_pgoff = (virt_to_phys(userdata)) >> PAGE_SHIFT;
	remap_pfn_range(vm, vm->vm_start, vm->vm_pgoff, vm->vm_end-vm->vm_start, vm->vm_page_prot);
	vm->vm_ops = &vm_ops;
	vm->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP;
	//poke the page, generate page fault
	vm_open(vm);

	return 0;
}



module_init(slave_init);
module_exit(slave_exit);
MODULE_LICENSE("GPL");
