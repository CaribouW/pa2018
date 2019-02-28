#include "common.h"
#include "string.h"

extern uint8_t read_byte(uint32_t);
extern void write_byte(uint32_t, uint8_t);

typedef struct
{
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

typedef struct
{
	bool used;		//state
	uint32_t index; //read/write index
	uint32_t offset;
} Fstate;

enum
{
	SEEK_SET,
	SEEK_CUR,
	SEEK_END
};

//file name : file size : file location
static const file_info file_table[] = {
	{"1.rpg", 188864, 1048576},
	{"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304},
	{"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032},
	{"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460},
	{"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582},
	{"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673},
	{"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367},
	{"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267},
	{"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941},
	{"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713},
	{"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989},
	{"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037},
	{"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
	{"credits_bgm.wav", 10067800, 1048576},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

Fstate files[NR_FILES + 3]; //k_th file relate to the (k+3)th

// void ide_read(uint8_t *buf, uint32_t offset, uint32_t len) {
// 	uint32_t i;
// 	for (i = 0; i < len; i ++) {
// 		buf[i] = read_byte(offset + i);
// 	}
// }
void ide_read(uint8_t *buf, uint32_t offset, uint32_t len);  //read from disk , to the buf
void ide_write(uint8_t *buf, uint32_t offset, uint32_t len); //write buf to the disk

int fs_open(const char *pathname, int flags);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);

/* Please implement a simple file system here */

//ret the file index in the table
int fs_open(const char *pathname, int flags)
{
	int index = 0;
	// panic("Please implement fs_open at fs.c");

	for (; index < NR_FILES; index++)
	{
		file_info fileInfo = file_table[index];
		char *origin_str = fileInfo.name;
		int i = 0;
		for (; origin_str[i] != '\0' && pathname[i] != '\0'; i++)
		{
			if (origin_str[i] != pathname[i]) //we have one not match
				break;
		}
		if (origin_str[i] == '\0' && pathname[i] == '\0')
		{
			// printf("index = %d\n",index);
			Log("index = %d\n", index);
			//open
			files[index + 3].used = true;
			files[index + 3].index = 0;
			return index;
		}
	}

	assert(0);
	return -1;
	// return -1;
	// return syscall(SYS_open, pathname, flags);
}

size_t fs_read(int fd, void *buf, size_t len)
{
	assert(fd > 2);
	if (len == 0)
		return 0;
	file_info fileInfo = file_table[fd];
	uint32_t file_size = fileInfo.size;
	uint32_t disk_offset = fileInfo.disk_offset;
	uint32_t index = files[fd + 3].index;
	if (file_size < len + index) //The file is small
		len = file_size - index;

	ide_read(buf, disk_offset + index, len);
	Log("len = %d\nfilesize = %x\n", len, file_size);
	fs_close(fd);
	// Log("buf0 = %x\n",(uint8_t*)buf[0]);
	//------------------------------------------------
	// int offset = disk_offset + index;
	// uint8_t *ptr = (uint8_t *)buf;
	// uint32_t i;
	// Log("read\n");
	// for (i = 0; i < len; i++)
	// {
	// 	ptr[i] = read_byte(offset + i);
	// 	Log("ptr = %x\n", ptr[i]);
	// 	assert(ptr[i] != '\0');
	// }
	//------------------------------------------------
	files[fd + 3].index += len; //reset the index
	return len;
}

size_t fs_write(int fd, void *buf, size_t len)
{
	assert(fd <= 2);
#ifdef HAS_DEVICE_SERIAL
	int i;
	extern void serial_printc(char);
	for (i = 0; i < len; i++)
	{
		// Log("write\n");
		serial_printc(((char *)buf)[i]);
	}
#else
	asm volatile(".byte 0x82"
				 : "=a"(len)
				 : "a"(4), "b"(fd), "c"(buf), "d"(len));
#endif

	return len;
}

off_t fs_lseek(int fd, off_t offset, int whence)
{
	panic("Please implement fs_lseek at fs.c");
	return -1;
}

int fs_close(int fd)
{
	panic("Please implement fs_close at fs.c");
	files[fd + 3].used = false;
	files[fd + 3].index = 0;
	return 0;
}
