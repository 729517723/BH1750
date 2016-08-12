#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<termios.h>
#include<fcntl.h>
#include<errno.h>

#define I2C_DEV "/dev/i2c-1"
#define I2C_SLAVE 0x0703
#define I2C_TENBIT 0x0704
#define I2C_ADDR 0x46
#define DATA_LEN 8

#define BH1750_Addr 0x46
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07

int main()
{
	int len=0,i2c_fd,temp=0;
	char temp1[1],temp2[1],temp3[1];

	temp1[0]=0x01;
	temp2[0]=0x07;
	temp3[0]=0x10;

	char tx_buf[DATA_LEN+1],rx_buf[DATA_LEN];

	i2c_fd=open(I2C_DEV,O_RDWR);
	if(i2c_fd<0)
	{
		printf("open i2c device error\n");
		return 0;
	}

	if(ioctl(i2c_fd,I2C_SLAVE,I2C_ADDR>>1)<0)
	{
		printf("ioctl i2c device error\n");
		return 0;
	}

	tx_buf[0]=BH1750_ON;
	tx_buf[1]=BH1750_RSET;
	tx_buf[2]=BH1750_CON;

	//len=write(i2c_fd,&tx_buf[0],1);
	if(len<0)
	{
		printf("write i2c device error 0\n");
		return 0;
	}

	//len=write(i2c_fd,&tx_buf[1],1);
	if(len<0)
	{
		printf("write i2c device error 1\n");
		return 0;
	}

	len=write(i2c_fd,&tx_buf[2],1);
	if(len<0)
	{
		printf("write i2c device error 2\n");
		return 0;
	}

	while(1)
	{
	sleep(1);

	len=read(i2c_fd,rx_buf,2);
	if(len<=0)
	{
		printf("read i2c device error\n");
		return 0;
	}

	temp = rx_buf[0];
	temp <<= 8;
	temp |= rx_buf[1];

	printf("Light:%lf lx\n\n",temp/1.2);
	}
	return 0;
}
