#ifndef _MYFS_H_
#define _MYFS_H_

#include<string.h>
#include"atapio.h"
#include"myfs.h"
#include<stdio.h>
#include <core/cmos.h>//����ʱ��ĸ�ʽ

#define READ_WRITE 2
#define READ_ONLY 3
#define WRITE_ONLY 4

#define NAME_LENGTH 13 
#define BYTEACLU 508
//#define MAXCLUAFILE 128

#define CLOSED 0
#define OPENED 1


//д�ļ�:�ɰ�atend��atbegin 2�ַ�ʽ.

 
/*
typedef struct FATFile
{
	char Name[256];
	
	uint8_t Type;
	uint32_t First_Cluster;
	
	uint32_t Creation_Time;//In tenth seconds
	uint32_t Creation_Date;
	uint32_t Write_Time;
	uint32_t Write_Date;
	uint32_t Access_Date;
	uint32_t FileSize;
}FATFile_Type;
*/
//Ŀǰ�ļƻ�:����Ŀ¼.
//��1���� bitmap.��clu�Ƿ���е�ָʾ 
//��2����:��һ��32�ֽ�,ָʾdir����.֮��ÿ32���ֽ�,Ϊһ��dir.
//
//https://blog.csdn.net/judyge/article/details/52373751
typedef struct Dir
{

	char dir_name[NAME_LENGTH];
	char state;//�Ƿ��Ѵ� 
	uint8_t spare;//������	
	uint8_t Type;//1
	uint16_t First_Cluster;//2
	uint16_t Last_Cluster;//2

	char Creation_Time[4];//4,ÿλ�ֱ�������ʱ��
	char Write_Time[4];//4

	uint16_t offset;//2
	uint16_t FileSize;//2
		
}Dir_Entrance;

typedef struct __attribute__((packed)) dirclu 
{
	uint32_t n;//��ֻ�����ļ���С�ڵ���15 
	struct Dir dir[15]; 
}dirsec;


typedef struct __attribute__((packed)) bit_map 
{
	int nextfreeclu;
	char map[508]; 
}bmap;

typedef struct __attribute__((packed)) cluster 
{
//	char occupied;	
	uint16_t nextfclu;
	char spare[2];//������ 
	char content[508]; 
} clutr;


typedef struct my_file
{

	char dir_name[NAME_LENGTH];
	char attr;//����,Ȩ�� 
	char *content;
	int *cluno;
	int filesize;
	int offset;
	int nclu;//clu����
	struct Dir *dir; 
	
/*	uint8_t Type;//1
	uint16_t First_Cluster;//2
	uint16_t Creation_Time;//2
	uint16_t Creation_Date;//2
	uint16_t Write_Time;//2
	uint16_t Write_Date;//2
	uint32_t offset;//4
	uint32_t FileSize;//4
*/
	
}file;

void seebmap(void);
void testwrite1(void);
void testread1(void);
void testwrite2(void);
void testread2(void);
void ls(void);
void testcreate(void);
void deletedir(int n);



#endif
