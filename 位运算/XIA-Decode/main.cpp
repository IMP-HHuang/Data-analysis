
#include<stdio.h>
#include<iostream>
#include <stdio.h>
#include <unistd.h>  //read
#include <fcntl.h>  //open

int main(int argv, char* argc[])
{
	int fd = open("data_R3032_M00.bin",O_RDONLY);
	if(fd < 0)	
		std::cout << " Cannot open data_R3032_M00.bin" << std::endl;
	unsigned int buff[32768]; // buffer to read 4 buytes from file(32bit)
	int Count = 0;
	while(1)
	{
		if(Count > 10)	break;
		size_t n = read(fd,&buff,16);
		if(n <= 0) std::cout << " false" << std::endl;
		std::cout << buff << std::endl;
		// & 与运算 >>　右移 
		// 0X 十六进制转化为二进制的前缀表示方法
		// & 0x0000000f 提取前四位数
		// >> 0x0 右移0位ch存在前四位
//		std::cout << "channel " << ((buff[0] & 0x0000000f)) << std::endl;
		std::cout << "channel " << ((buff[0] & 0x0000000f) >> 0x0) << std::endl;
//		std::cout << "SlotID " << ((buff[0] & 0x000000f0)) << std::endl;
		std::cout << "SlotID " << ((buff[0] & 0x000000f0) >> 4) << std::endl;
//		std::cout << "CrateID " << ((buff[0] & 0x00000f00)) << std::endl;
		std::cout << "CrateID " << ((buff[0] & 0x00000f00) >> 8) << std::endl;
		std::cout << "Header Length " << ((buff[0] & 0x0001f000) >> 12) << std::endl;
		std::cout << "Event Length " << ((buff[0] & 0x7fff0000) >> 17) << std::endl;
		std::cout << "Pile up " << ((buff[0] & 0x80000000) >> 31) << std::endl;
		std::cout << "ts " << ((buff[1] & 0xffffffff) >> 0) << std::endl;
		// buff[2] :CFD 注意：不同采样率格式不一样
		std::cout << "Energy " << ((buff[3] & 0x0000ffff) >> 0) << std::endl;
		std::cout << "Trace Length " << ((buff[3] & 0x7fff0000) >> 16) << std::endl;
		std::cout << "Trace out of range flag " << ((buff[3] & 0x80000000) >> 31) << std::endl;
		//波形读取
		if(((buff[3] & 0x7fff0000) >> 16))
		{
			n = read(fd, &buff, ((buff[3] & 0x7fff0000) >> 16)/2*4);
		}
		Count++;
	}
}
