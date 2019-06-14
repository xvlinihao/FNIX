#include"myfs.h"
/*void writeonedir(struct Dir *dir,char clu[],int offset){//新设计,考虑不要这个函数
	//printf("sizeofcluux:%d\n",sizeof(char));	
//	char *clubuf = clu+offset;
//	int n=32;
//	printf("\nxxx%s\n",dir->dir_name);
	memcpy(dir, clu+offset, 32);
//	printf("\nxxx%s\n",dir->dir_name);
//	printf("\nxxx%s\n",dir->dir_name);
//	for(int i=0;i<32;i++){
//		printf("%c",clubuf[i]);
//	}
//	struct Dir tb;
//	memcpy( clubuf, &tb,32);
	
//	printf("\nyyy%s\n",tb.dir_name);	
	
};

void writealldir(int n,struct Dir dir[],char clu[]){//考虑删掉第三个参数//新设计,考虑不要这个函数 
	//检查n是否超限度.32*n+16<=512  

	//
	if(n>15){
		printf("wrong!too much dir\n");
		return 0; 
	}
//	int nbuf=n;
//	printf("debug1 %d\n",n);
	memcpy(&n,clu,sizeof(int));
//	printf("debug1 %d\n",n);
		
	//把数字写入第一格 
	int offset=sizeof(int);
	int i;
	for(i=0;i<n;i++){
//		printf("debug2\n");
				
		writeonedir(&dir[i],clu,offset);	
		offset +=32;
	}
	//第2块
	for(i=0;i<100;i++){
	printf("%c",clu[i]);
	}
	pio_write_sector(clu,2);
};

void readalldir(int* nptr,struct Dir dir[]){//新设计,考虑不要这个函数
	char buf[512];
	pio_read_sector(buf,2);
	int offset= sizeof(int);
	memcpy(buf,nptr,sizeof(int));
	int i;
	for( i=0;i<*nptr;i++){
		memcpy(buf+offset,dir++,32);	
		offset+=32;
	}
};
*/

void initdirclu(struct dirclu *dc){
	pio_read_sector(dc,2);
	for(int i=0;i<dc->n;i++){
		dc->dir[i].state = CLOSED;
	}
}

void writedirclu(struct dirclu *dc){
	pio_write_sector(dc,2);
	//pio_read_sector(dc,2);
}

//void initfs(){};

void writetime(char buf[]){
	cmos_rtc_t rtc = cmos_read_rtc();
	buf[0] = rtc.month;
	buf[1] = rtc.day;
	buf[2] = rtc.hours;
	buf[3] = rtc.minutes;	
}

void readtime(char buf[]){

	printf("[%2d/%2d %2d:%2d] ", buf[0],buf[1],buf[2],buf[3]);
	
}



void testwrite(){
	
	struct dirclu dclu={2,
		
		"testdir1",
		'0',
		0,
		0,//type
		0,//1st_c
		0,//
		"0",//cre_t
		//34,//cre_d
		"0",//wri_t
		//55,//wri_d
		0,//offset
		0//size
		,

		"testdir3",
		'0',
		0,
		0,//type
		0,//1st_c
		0,//
		"0",//cre_t
		//34,//cre_d
		"0",//wri_t
		//55,//wri_d
		0,//offset
		0//size
	};

	writetime(dclu.dir[0].Creation_Time);	
	writetime(dclu.dir[1].Creation_Time);
	//char buff[13]="testdir1";
	
	//memcpy(dt[0].dir_name,buff,13);
	//printf("\n%s\n",dt[0].dir_name);
	//printf("%s\n",dt[1].dir_name);
	//char bufclu[512];	
	//writealldir(2,dt,bufclu);	
	writedirclu(&dclu);
};

void testread(){
	//struct Dir dt[2];
	//int n;
/*	char clu[512];
	pio_read_sector(clu,2);		
	int offset = sizeof(int);	
	memcpy(clu+offset,dt,32);
	offset+=32;
	memcpy(clu+offset,dt+1,32);*/
	struct dirclu dclu; 
	//readalldir(&n,dt);
	initdirclu(&dclu);
	//printf("%d\n",dclu.n);
	//printf("%s\n",dclu.dir[0].dir_name);
	//printf("%s\n",dclu.dir[1].dir_name);
	readtime(dclu.dir[1].Creation_Time);
	readtime(dclu.dir[0].Creation_Time);
};

void seemap(struct bit_map *b){
	for(int i=0;i<508;i++){
		printf("%c",b->map[i]);
		//if(i%20==0)printf("\n");
	}
}
void seebmap(){//转命令。显示bmap.
	struct bit_map *b;
	b=(struct bit_map*)malloc(sizeof(struct bit_map));
	initmap(b);
	for(int i=0;i<508;i++){
		printf("%c",b->map[i]);
		//if(i%20==0)printf("\n");
	}
}


void mapclear(struct bit_map *b){
	for(int i=0;i<3;i++)b->map[i]='1';
	for(int i=3;i<508;i++)b->map[i]='0';
};

int getfreeclu(struct bit_map *b){//不妨从5号块开始作为可用//获取的块会标为已用
	if(b->nextfreeclu!=-1){
		computefreeclu(b);
		int buf=b->nextfreeclu;
		//printf("debug4:%d\n",buf);
		//printf("\nnext:%d\n",b->nextfreeclu);	
		
		b->map[buf]='1';			
		//printf("debug5\n");
		//seemap(b);

		if(computefreeclu(b)==-1){
			printf("now map is full!!!\n");
		}			
		return buf;
	}
	else return -1; 
	
}

int computefreeclu(struct bit_map *b){
	int i;		
	for(i=5;i<508;i++)if(b->map[i]=='0'){ b->nextfreeclu = i;return(i);}
	
	printf("map is full\n");
	return -1;
		
}
void freeabit(struct bit_map *b,int bit){
	b->map[bit]='0';	

};


void initmap(struct bit_map *b){
	pio_read_sector(b,1);
};


void savemap(struct bit_map *b){
	pio_write_sector(b,1);
	computefreeclu(b);
};


int openfile(struct my_file *f, struct Dir *d, int oflag){
	
	memcpy(d->dir_name,f->dir_name,NAME_LENGTH);
	if(oflag==WRITE_ONLY||oflag==READ_ONLY||oflag==READ_WRITE){
		
		
	}
	else{
		printf("wrong oflag when open a file!");
		return -1;
	}
	
	d->state = OPENED;
	f->dir=d;
	f->attr = oflag;

	f->filesize = d->FileSize;
	
	if(f->filesize==0){f->nclu=0;return 0;}
	
	int nclu = f->filesize/BYTEACLU +1;//
	f->nclu=nclu;
	int noffset = f->filesize%BYTEACLU;
		
	f->content = (char*)malloc(f->filesize);
	f->cluno = (int*)malloc(nclu*sizeof(int));
	int c = d->First_Cluster;//clu编号 
	int _offset=0;
	f->cluno[0]=c;
	//printf("xxxxxxxxx1st:%d\n",c);
	//printf("debug2:%d\n",nclutowrite);
	//printf("xxxxxxxxxxxnoffset:%d\n",noffset);
	for(int i=0;i<nclu;i++){
	//	char buf[512];
		struct cluster clu;
		pio_read_sector(&clu,c);
		//for(int j=0;j<29;j++){
			//printf("%c",clu.content[j]);
		//}
		if(i!=nclu-1){
			memcpy(clu.content,f->content+_offset,BYTEACLU);							
			c = clu.nextfclu;
			f->cluno[i+1]=c;
			_offset+=	BYTEACLU;				
		}
		else{
			memcpy(clu.content,f->content+_offset,noffset);
		}
		
	}
	
	f->offset = d->offset;
	return 0;
};

int readfile(struct my_file *f,int start, int end){//字节号
	if(f->attr == WRITE_ONLY){
		printf("wrong ,WRITE ONLY");
		return -1;
	}
	if(start>end || end>f->filesize || start<0 ){
		printf("wrong para");
		return -1;
	}
	char *buf = f->content;
	buf += start;
	for(int i=start;i<=end;i++){
		printf("%c",buf[i]);
		
	}
	return 0;
}

int closefile(struct my_file *f){//可以考虑把更新dir的放这里
	/*****把malloc的东西清掉,把f删掉.把dir状态改掉***/
	if(f->dir->state==CLOSED){
		printf("wrong state");
		return -1;
	}
	else{
		f->dir->state=CLOSED;
	}
	//free(f->content);
	//free(f->cluno);
	//free(f);
	/******把malloc的东西清掉***/

	return 0;

}

int freeclu(struct my_file *f,int start ,struct bit_map *b){//在bmap中标记为空,实际内容尚存 
	int nclu = f->nclu;
	
	if(start > nclu || start<0 ){
		printf("wrong para");
		return -1;
	}
	for(int i=start;i<=nclu;i++){
		b->map[f->cluno[i]]='0';
	}
	computefreeclu(b);
	return 0;	
}

int writefile(struct my_file *f,struct bit_map *b,int start ){//此处clu单位是clu数 //此时f中的内容与硬盘不同,即f已经写过了 ,且filesize也已经改变了,但nclu未变 
	if(f->attr == READ_ONLY){
		printf("wrong ,READ ONLY");
		return -1;
	}

//printf("debug-1\n");
	
	
	int nclu = f->nclu;
	if(start > nclu || start<0 ){
		printf("wrong para");
		return -1;
	}
	//freeclu(f,start,b);
	int offset = start * BYTEACLU;
	int c = getfreeclu(b);
	
//printf("debug0c=%d\n",c);
		
		
	
	struct cluster clu;
	if(start!=0){//需要调整上一个块 
		pio_read_sector(&clu,f->cluno[start-1]); 
		clu.nextfclu = c;
		pio_write_sector(&clu,f->cluno[start-1]); 		
	}
	else {//若为首块,调整dir 
		f->dir->First_Cluster=c;
	}
	
	
	f->dir->FileSize = offset + f->filesize;//调整目录块。
	f->nclu = f->filesize/BYTEACLU+1;
	int noffset = f->filesize%BYTEACLU;
	int nclutowrite = f->nclu - start;
	//printf("debug1noffset:%d\n",noffset);
	//printf("debug2:%d\n",nclutowrite);
//printf("offset:%d\n",offset);
	
	for(int i=0;i<nclutowrite;i++){
		if(i==nclutowrite-1){
			//printf("debug4\n");					
			memcpy(f->content+offset,clu.content,noffset);//填写clu 
			clu.nextfclu = 0;

			//for(int j=0;j<29;j++){
				//printf("%c",clu.content[j]);
			//}
			pio_write_sector(&clu,c);		
			break;
		}

		memcpy(f->content+offset,clu.content,BYTEACLU);//填写clu
		
		int nextc = getfreeclu(b);//if
		//for(int j=0;j<29;j++){
			//printf("%c",clu.content[j]);
		//}
		
		clu.nextfclu = nextc;
		pio_write_sector(&clu,c);		
		c=nextc;
		offset+=BYTEACLU;
		//printf("debug3\n");
			
	}
/*********

修改dir的write时间 

*********/
	writetime(f->dir->Write_Time);

	return 0;	
}

void testwrite1(){//可以改命令。往最后一个dir的文件里面写wx.//注意：当最后一个目录项为空时调用
	struct bit_map *b;
	b=(struct bit_map*)malloc(sizeof(struct bit_map));
	
	//mapclear(b);
	//savemap(b);
	initmap(b);
	//seemap(b);

	//printf("\nnext:%d\n",b->nextfreeclu);	
	
	struct dirclu dclu; 
	//readalldir(&n,dt);
	initdirclu(&dclu);
	int j= dclu.n-1;
	//printf("%d\n",dclu.n);
	//printf("%s\n",dclu.dir[0].dir_name);
	//printf("%s\n",dclu.dir[1].dir_name);
	struct my_file f;
	openfile(&f,&(dclu.dir[j]),READ_WRITE);
	f.filesize=30;
	f.content = (char*)malloc(30);
	char buf[30]="this is a test sentence";	
	//memcpy(buf,f.content,1);
	//printf("xxxxx\n");
	for(int i=0;i<14;i++)f.content[i]='w';
	for(int i=14;i<28;i++)f.content[i]='x';
	f.content[29]='\0';	
	//printf("%s\n",f.content);
	//printf("fnclu:%d\n",f.nclu);
	//printf("ffilesize:%d\n",f.filesize);
	writefile(&f,b,0);
	//readfile(&f,0,29);
	//printf("fnclu:%d\n",f.nclu);
	//printf("firesize:%d\n",dclu.dir[j].FileSize);
	//printf("ffilesize:%d\n",f.filesize);
	//printf("firstclu:%d\n",dclu.dir[j].First_Cluster);
	//readtime(dclu.dir[j].Write_Time);
	closefile(&f);
//	seemap(b);
	savemap(b);
	//struct my_file f2;
	//openfile(&f2,dclu.dir,READ_WRITE);	
	//readfile(&f2,0,29);
	
	writedirclu(&dclu);
}

void testread1(){//可改命令。读取第一（0）个文件信息。
	struct dirclu *dclu;
	dclu =(struct dirclu*)malloc(sizeof(struct dirclu)); 
	initdirclu(dclu);
	printf("firesize:%d\n",dclu->dir[0].FileSize);
	printf("firstclu:%d\n",dclu->dir[0].First_Cluster);
	struct my_file f;
	openfile(&f,dclu->dir,READ_WRITE);
	readfile(&f,0,29);
	//readtime(dclu.dir[0].Write_Time);

	//struct bit_map b ;
//mapclear(&b);
//initmap(&b);
//printf("bmap%d\n",b.nextfreeclu);

//seemap(&b);
//savemap(&b);
}

void testwrite2(){//可改命令。往最后一个文件写2个块
		
	struct bit_map *b;
	b=(struct bit_map*)malloc(sizeof(struct bit_map));
	
	//mapclear(b);
	//savemap(b);
	initmap(b);
	//seemap(b);

	//printf("\nnext:%d\n",b->nextfreeclu);	
	
	struct dirclu dclu; 
	//readalldir(&n,dt);
	initdirclu(&dclu);
	int j =dclu.n-1;
	//printf("%d\n",dclu.n);
	//printf("%s\n",dclu.dir[0].dir_name);
	//printf("%s\n",dclu.dir[1].dir_name);
	struct my_file f;
	openfile(&f,&(dclu.dir[j]),READ_WRITE);
	f.filesize=600;
	f.content = (char*)malloc(600);
	//char buf[300]="this is a test sentence";	
	//memcpy(buf,f.content,1);
	//printf("xxxxx\n");
	for(int i=0;i<100;i++)f.content[i]='w';
	for(int i=101;i<200;i++)f.content[i]='x';
	for(int i=201;i<300;i++)f.content[i]='z';
	for(int i=301;i<400;i++)f.content[i]='a';
	for(int i=401;i<500;i++)f.content[i]='b';
	for(int i=501;i<599;i++)f.content[i]='c';
	f.content[599]='\0';
	//printf("%s\n",f.content);
	//printf("fnclu:%d\n",f.nclu);
	//printf("ffilesize:%d\n",f.filesize);
	writefile(&f,b,0);
	//readfile(&f,0,29);
	//printf("fnclu:%d\n",f.nclu);
	//printf("firesize:%d\n",dclu.dir[1].FileSize);
	//printf("ffilesize:%d\n",f.filesize);
	//printf("firstclu:%d\n",dclu.dir[1].First_Cluster);
	//readtime(dclu.dir[j].Write_Time);
	
	writedirclu(&dclu);
		
	closefile(&f);
	//seemap(b);
	savemap(b);
	

	//struct my_file f2;
	//openfile(&f2,dclu.dir,READ_WRITE);	
	//readfile(&f2,0,29);
	

}

void testread2(){//可以改命令。读第一（2）个文件信息。
	struct dirclu *dclu;
	dclu =(struct dirclu*)malloc(sizeof(struct dirclu)); 
	initdirclu(dclu);
	printf("firesize:%d\n",dclu->dir[1].FileSize);
	printf("firstclu:%d\n",dclu->dir[1].First_Cluster);
	struct my_file f;
	openfile(&f,&(dclu->dir[1]),READ_WRITE);
	readfile(&f,0,599);
	//readtime(dclu.dir[1].Write_Time);

//	struct bit_map b ;
//mapclear(&b);
//initmap(&b);
//printf("bmap%d\n",b.nextfreeclu);

//seemap(&b);
//savemap(&b);
}

void printdir(struct Dir dir){
	//dir.dir_name[12]='\0';
	//printf("%s,creattime:",dir.dir_name);
	readtime(dir.Creation_Time);
	printf("writetime:");	
	readtime(dir.Write_Time);
	
}


void ls(){//列出当前所有文件。可改命令
	struct dirclu *d; 
	d=(struct dirclu*)malloc(sizeof(struct dirclu));
	initdirclu(d);
	int n=d->n;
	printf("n:%d\n",n);
	for(int i=0;i<n;i++){
		printf("%s ",d->dir[i].dir_name);
		printf("%d ",d->dir[i].FileSize);		
		readtime(d->dir[i].Creation_Time);		
		if(d->dir[i].FileSize!=0)readtime(d->dir[i].Write_Time);
		printf("\n");			
	//printdir(d->dir[i]);
	}
	
//	printf("firesize:%d\n",d->dir[1].FileSize);
//	printf("firstclu:%d\n",d->dir[1].First_Cluster);
//	struct my_file f;
//	openfile(&f,&(dclu.dir[1]),READ_WRITE);
//	readfile(&f,0,599);
	//readtime(d->dir[1].Write_Time);
/*
	struct bit_map b ;
mapclear(&b);
initmap(&b);
printf("bmap%d\n",b.nextfreeclu);

seemap(&b);
*/	
}
void testcreate(){//建立空文件。可改命令。
	struct dirclu *d; 
	d=(struct dirclu*)malloc(sizeof(struct dirclu));
	initdirclu(d);
	int n=d->n;
	struct Dir dr={
		"testfiledira",
		'0',
		0,
		0,//type

		0,//1st_c
		0,//
		"0",//cre_t
		//34,//cre_d
		"0",//wri_t
		//55,//wri_d
		0,//offset
		0//size
	};
	dr.dir_name[11]+=n;
	writetime(dr.Creation_Time);	
	memcpy(&dr,&(d->dir[n]),32);
	d->n++;
	writedirclu(d);	
	
}


void deletedir(int n){//删除第n个文件。可改命令。
	struct dirclu *d; 
	d=(struct dirclu*)malloc(sizeof(struct dirclu));
	initdirclu(d);
	int ndir=d->n;
	struct bit_map *b;
	b=(struct bit_map*)malloc(sizeof(struct bit_map));
	initmap(b);
	printf("debug7\n");
//处理bitmap
	if(d->dir[n].FileSize!=0){
		if(d->dir[n].FileSize > BYTEACLU){
			struct my_file *f;
			f=(struct my_file*)malloc(sizeof(struct my_file));

			openfile(f, &(d->dir[n]), READ_ONLY);
			for(int i=0;i<f->nclu;i++){
				freeabit(b,f->cluno[i]);
			}
			closefile(f);
			
		}
		else{
			freeabit(b,d->dir[n].First_Cluster);
		}
		savemap(b);	
	}
	
//处理dirclu.
	if(n == ndir-1){
		//d->n-=1;
	}
	else{//将最后一个dir覆盖一下
		printf("debug8\n");
		memcpy(&(d->dir[ndir-1]),&(d->dir[n]),32);
	}
	d->n-=1;
	writedirclu(d);		

}

