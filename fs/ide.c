/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;

	u_int zero = 0;
	u_int cur_offset = 0;
	// writef("ide_read in\n");
	while (offset_begin + offset < offset_end)
	{
		// Your code here
		// error occurred, then panic.
		cur_offset = offset_begin + offset;
		if (syscall_write_dev((u_int)&cur_offset, 0x13000000, 4) < 0)
			user_panic("ide_read panic");
		if (syscall_write_dev((u_int)&diskno, 0x13000010, 4) < 0)
			user_panic("ide_read panic");
		if (syscall_write_dev((u_int)&zero, 0x13000020, 4) < 0)
			user_panic("ide_read panic");
		u_int succ;
		if (syscall_read_dev((u_int)&succ, 0x13000030, 4) < 0)
			user_panic("ide_read panic");
		if (!succ)
			user_panic("ide_read panic");
		if (syscall_read_dev((u_int)(dst + offset), 0x13004000, 0x200) < 0)
			user_panic("ide_read panic");
		offset += 0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	// Your code here
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;

	u_int one = 1;
	u_int cur_offset = 0;

	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end)
	{
		// copy data from source array to disk buffer.
		// if error occur, then panic.
		cur_offset = offset_begin + offset;
		if (syscall_write_dev((u_int)(src + offset), 0x13004000, 0x200) < 0)
			user_panic("ide_write panic");
		if (syscall_write_dev((u_int)&diskno, 0x13000010, 4) < 0)
			user_panic("ide_write panic");
		if (syscall_write_dev((u_int)&cur_offset, 0x13000000, 4) < 0)
			user_panic("ide_write panic");
		if (syscall_write_dev((u_int)&one, 0x13000020, 4) < 0)
			user_panic("ide_write panic");
		u_int succ;
		if (syscall_read_dev((u_int)&succ, 0x13000030, 4) < 0)
			user_panic("ide_write panic");
		if(!succ)
			user_panic("ide_write panic");
		offset += 0x200;
	}
}
int raid4_valid(u_int diskno)
{
	int r;
	int succ;
	int zero=0;
	r=syscall_write_dev((u_int)&zero, 0x13000000, 4);
	if(r<0)return 0;
	r=syscall_write_dev((u_int)&diskno,0x13000010,4);
	if(r<0)return 0;
	r=syscall_write_dev((u_int)&zero, 0x13000020, 4);
	if(r<0)return 0;
	r=syscall_read_dev((u_int)&succ, 0x13000030, 4);
	if(r<0)return 0;
	if(succ==0)return 0;
	return 1;
}
int raid4_write(u_int blockno,void *src)
{
	char *srcp=(char*)src;
	char buf[4096];
	int number=5;
	int i;
	for(i=1;i<=5;i++){
		if(raid4_valid(i))
		number--;
	}
	for(i=0;i<BY2PG/8;i++){
		buf[i]=(srcp[i]^srcp[i+BY2PG/8]^srcp[i+BY2PG/8*2]^srcp[i+BY2PG/8*3]);
	}
	for(i=0;i<BY2PG/8;i++){
		buf[i+BY2PG/8]=(srcp[i+BY2PG/2]^srcp[i+BY2PG/8+BY2PG/2]^srcp[i+BY2PG/2+BY2PG/8*2]^srcp[i+BY2PG/2+BY2PG/8*3]);
	}
	ide_write(1,2*blockno,srcp,1);
	ide_write(2,blockno*2,srcp+BY2PG/8,1);
	ide_write(3,2*blockno,srcp+BY2PG/4,1);
	ide_write(4,blockno*2,srcp+BY2PG/8*3,1);

	ide_write(1,2*blockno+1,srcp+BY2PG/2,1);
	ide_write(2,blockno*2+1,srcp+BY2PG/8*5,1);
	ide_write(3,2*blockno+1,srcp+BY2PG/8*6,1);
	ide_write(4,blockno*2+1,srcp+BY2PG/8*7,1);
	ide_write(5,2*blockno,buf,1);
	ide_write(5,2*blockno+1,buf+BY2PG/8,1);
	return number;
}
int raid4_read(u_int blockno,void *dst)
{
	char * dstp=dst;
	char buf[8188];
	int number=5;
	int wrong;
	int i;
	for(i=1;i<=5;i++){
		if(raid4_valid(i)){
			number--;
		}else{
			wrong=i;
		}
	}
	if(number==0){
		// for(i=0;i<8;i++){
		// 	ide_read(i%4+1,blockno*2+(i>3?1:0),dstp+BY2PG/8*(i-1),1);
		// }
		ide_read(1,blockno*2,dstp+BY2PG/8*0,1);
		ide_read(2,blockno*2,dstp+BY2PG/8*1,1);
		ide_read(3,blockno*2,dstp+BY2PG/8*2,1);
		ide_read(4,blockno*2,dstp+BY2PG/8*3,1);
		ide_read(1,blockno*2+1,dstp+BY2PG/8*4,1);
		ide_read(2,blockno*2+1,dstp+BY2PG/8*5,1);
		ide_read(3,blockno*2+1,dstp+BY2PG/8*6,1);
		ide_read(4,blockno*2+1,dstp+BY2PG/8*7,1);

		ide_read(5,2*blockno,buf,1);
		ide_read(5,2*blockno+1,buf+BY2PG/8,1);
		for(i=0;i<BY2PG/8;i++){
			if(buf[i]!=
				(dstp[i]^
				dstp[i+BY2PG/8]^
				dstp[i+BY2PG/8*2]^
				dstp[i+BY2PG/8*3]) )
					return -1;
		}
		for(i=0;i<BY2PG/8;i++){
			if(buf[i+BY2PG/8]!=
				(dstp[i+BY2PG/2]^
				dstp[i+BY2PG/8+BY2PG/2]^
				dstp[i+BY2PG/8*2+BY2PG/2]^
				dstp[i+BY2PG/8*3+BY2PG/2]) )
					return -1;
		}
		return 0;
	}else if(number==1){
		if(wrong==5){
			ide_read(1,blockno*2,dstp+BY2PG/8*0,1);
			ide_read(2,blockno*2,dstp+BY2PG/8*1,1);
			ide_read(3,blockno*2,dstp+BY2PG/8*2,1);
			ide_read(4,blockno*2,dstp+BY2PG/8*3,1);
			ide_read(1,blockno*2+1,dstp+BY2PG/8*4,1);
			ide_read(2,blockno*2+1,dstp+BY2PG/8*5,1);
			ide_read(3,blockno*2+1,dstp+BY2PG/8*6,1);
			ide_read(4,blockno*2+1,dstp+BY2PG/8*7,1);
		}else{
			ide_read(1,blockno*2,buf+BY2PG/8*0,1);
			ide_read(2,blockno*2,buf+BY2PG/8*1,1);
			ide_read(3,blockno*2,buf+BY2PG/8*2,1);
			ide_read(4,blockno*2,buf+BY2PG/8*3,1);
			ide_read(1,blockno*2+1,buf+BY2PG/8*4,1);
			ide_read(2,blockno*2+1,buf+BY2PG/8*5,1);
			ide_read(3,blockno*2+1,buf+BY2PG/8*6,1);
			ide_read(4,blockno*2+1,buf+BY2PG/8*7,1);
			ide_read(5,blockno*2,buf+BY2PG,1);
			ide_read(5,blockno*2+1,buf+BY2PG/8*9,1);

			for(i=0;i<BY2PG/4;i++){
				char res=0;
				int j;
				for(j=0;j<5;j++){
					if(j+1==wrong)continue;
					res^=buf[j*(BY2PG/4)+i];
				}
				buf[(wrong-1)*(BY2PG/4)+i]=res;
			}
			user_bcopy(buf+BY2PG/4*(wrong-1),dst+(wrong-1)*(BY2PG/4),BY2PG/4);
		}
		return 1;
	}else{
		return number;
	}
}
int time_read()
{
	int temp=0;
	syscall_write_dev(&temp,0x15000000,4);
	syscall_read_dev(&temp,0x15000010,4);
	return temp;
}

void raid0_write(u_int secno,void *src,u_int nsecs)
{
	u_int i,nowno;
	for(i=0;i<nsecs;i++){
		nowno=i+secno;
		if(nowno%2==0){
			ide_write(1,nowno/2,src+i*0x200,1);
		}else{
			ide_write(2,nowno/2,src+i*0x200,1);
		}
	}
}

void raid0_read(u_int secno,void *dst,u_int nsecs)
{
	u_int i,nowno;
	for(i=0;i<nsecs;i++){
		nowno=i+secno;
		if(nowno%2==0){
			ide_read(1,nowno/2,dst+i*0x200,1);
		}else{
			ide_read(2,nowno/2,dst+i*0x200,1);
		}
	}
}