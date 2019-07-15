#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/vfs.h>

static char *str = "./rec/";

int mkdirs(const char *path, mode_t mode);
void mkname(char *dirname);
int check();
void scandirs(char *path);
void *thr_mkdir(void *data);
void *thr_dir_remove(void *data);
void target(char *target);
float hardCheck(void);
void *thr_cam(void *data);
void camAction();

int main()
{
	pthread_t p_thread[3];
	int thr_id;
	int status;
	int a=1;
	int b=2;
	int c=3;

	thr_id=pthread_create(&p_thread[0], NULL, thr_mkdir, (void *)&a);
	if(thr_id<0)
	{
		perror("thread1 error :");
		exit(0);
	}
	
	thr_id=pthread_create(&p_thread[1], NULL, thr_dir_remove, (void *)&b);
	if(thr_id<0)
	{
		perror("thread2 error :");
		exit(0);
	}

	thr_id=pthread_create(&p_thread[2], NULL, thr_cam, (void *)&c);
	if(thr_id<0)
	{
		perror("thread3 error :");
		exit(0);
	}

//	camAction();
/*	thr_id=pthread_create(&p_thread[2], NULL, thr_rec, (void *)&c);
	if(thr_id<0)
	{
		perror("thread3 error :");
		exit(0);
	}
	*/



	pthread_join(p_thread[0], (void**)&status);
	pthread_join(p_thread[1], (void**)&status);
	pthread_join(p_thread[2], (void**)&status);

	return 0;
}


int mkdirs(const char *path, mode_t mode)
{
	char tmp_path[2048];
	const char *tmp = path;
	int len = 0;
	int ret;

	if(path == NULL || strlen(path)>=2048){
		printf("error\n");
		return -1;
	}

	while((tmp=strchr(tmp, '/'))!=NULL){
		len = tmp - path;
		tmp++;
		if(len == 0){
			continue;
		}
		strncpy(tmp_path, path, len);
		tmp_path[len] = 0x00;
		if((ret=mkdir(tmp_path, mode))==-1){
			if(errno != EEXIST){
				return -1;
			}
		}
	}
	return mkdir(path, mode);
}



void mkname(char *dirname)
{
	char s[100];
	int rc;
	time_t temp;
	struct tm *timeptr;

	temp=time(NULL);
	timeptr = localtime(&temp);
	rc=strftime(s, sizeof(s), "%Y%m%d_%H", timeptr);
	strcpy( dirname, s);
}

int check()
{
	char *s=(char*)malloc(sizeof(char)*100);
	int rc;
	time_t temp;
	struct tm *timeptr;
	int result;

	temp=time(NULL);
	timeptr=localtime(&temp);
	rc=strftime(s, sizeof(s), "%M", timeptr);
	result=atoi(s);
	return result;
}

void target(char *target)
{
	struct dirent **namelist;
	int count;
	int idx;

	if((count=scandir(str, &namelist, NULL, alphasort))==-1)
	{
		fprintf(stderr, "%s target scan failed %s\n", str, strerror(errno));
	}
	strcpy(target, namelist[2]->d_name);
}

void scandirs(char *path)
{
	struct dirent **namelist;
	int count;
	int idx;

	if((count=scandir(path, &namelist, NULL, alphasort))==-1)
	{
		fprintf(stderr, "%s scan error:%s\n", path, strerror(errno));
		remove(path);
	}

	if(count==2)
	{
		remove(path);
	}
	else
	{
		for(idx=2; idx<count; idx++)
		{
			char *test=(char*)malloc(sizeof(char)*100);
			sprintf(test, "%s%s",path,namelist[idx]->d_name);
			if(remove(test)==-1)
			{
				scandirs(test);
			}
			remove(path);
			free(test);
		}
	}
}



void *thr_dir_remove(void *data)
{

	while(1)
	{
		float *ck =(float *)malloc(sizeof(float)*1);
		*ck=0.25;
		if(hardCheck()<*ck)
		{
			char *removetarget = (char*)malloc(sizeof(char)*100);
			char *removetarget1 = (char*)malloc(sizeof(char)*100);
			target(removetarget);
			sprintf(removetarget1, "%s%s/", str, removetarget);
			scandirs(removetarget1);
			free(removetarget);
			free(removetarget1);
			printf("remove\n");
		}
		free(ck);
	}
}
void *thr_mkdir(void *data)
{
	while(1)
	{	
		char *dirname1=(char*)malloc(sizeof(char)*10);
		char *mkn=(char*)malloc(sizeof(char)*100);
		mkname(dirname1);


		sprintf(mkn, "%s%s",str, dirname1);
		mkdirs(mkn, 0776);
		free(mkn);
		free(dirname1);
	}
}

float hardCheck(void)
{
	struct statfs fs;
	statfs("/", &fs);
	float r=(float)fs.f_bavail/fs.f_blocks;
	return r;
}
void *thr_cam(void *data)
{
	camAction();
}

void camAction()
{
//	system("python cam.py");

	if(0!=system("python cam.py")){
		printf("python error\n");
		camAction();
	}

}
