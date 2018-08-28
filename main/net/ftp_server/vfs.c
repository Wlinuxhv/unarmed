/*
 * vfs.c
 *
 *  Created on: 2009/9/29
 *      Author: william
 */
#include <netif/loopif.h>
#include "vfs.h"
#include <platform/yl_fsio.h>

/*******************************************************
 * �ؿ��ާ@
 * vfs_opendir=opendir		�}�ҥؿ�
 * vfs_closedir=closedir	�����ؿ�
 * vfs_readdir=readdir		Ū�����w�ؿ�
 * vfs_chdir=chdir			�ܧ�ؿ�
 * vfs_stat=?
 * vfs_getcwd=getcwd		�ثe�ؿ�
 * vfs_mkdir=mkdir			�s�إؿ�
 * vfs_rmdir=rmdir			�����ؿ�
 * vfs_close=closedir 		(�쥻�u���w�q�@�ӨӳB�z�ɮ׻P�ؿ�)
 *******************************************************/

/*DIR *_opendir(const char *);*/
vfs_dir_t *vfs_opendir(vfs_t *vfs, char *file) {
	return 0;
}

/* int _closedir(DIR *); */
int vfs_closedir(vfs_dir_t *dir) {
return 0;
}

/* struct dirent *_readdir(DIR *); */
vfs_dirent_t *vfs_readdir(vfs_dir_t *dir) {
	DIR *dirp;
	struct dirent *dp;
	char *a;
	a = "a:/";
	dirp = opendir(a);
	while ((dp = readdir(dirp)) != NULL) {
		printf("file:%s\n", dp->d_name);
	}
	closedir(dirp);
	printf("done!");
	return 0;
}

/* int _chdir(const char *); */
int vfs_chdir(vfs_t *vfs, const char *arg) {
	return 0;
}

/* Get file parameters */
int vfs_stat(vfs_t *vfs, const char *arg, vfs_stat_t *st) {
	return 0;
}

/* char *_getcwd(char *, unsigned long); */
char *vfs_getcwd(vfs_t *vfs, in_plugin_t *inp, char *dir, int size) {
	return 0;
}

/* int _mkdir(const char *); */
int vfs_mkdir(vfs_t *vfs) {
	return 0;
}

/* int _rmdir(const char *); */
int vfs_rmdir(vfs_t *vfs, const char *arg) {
	return 0;
}

/* int _closedir(DIR *); */
int vfs_close(vfs_t *vfs) {
	return 0;
}

/*******************************************************
 * �ɮ׾ާ@
 * vfs_open=fopen			�}���ɮ�
 * vfs_closef=fclose		�����ɮ�
 * VFS_ISDIR=S_ISDIR(m)
 * VFS_ISREG=S_ISREG(m)
 * vfs_eof=feof				�ɮ׫��ЬO�_���
 * vfs_rename=rename		�ܧ��ɮצW��
 * vfs_remove=unlink		�R���ɮ�
 * vfs_read=fread			Ū���ɮ�
 * vfs_write=fwrite			�g�J�ɮ�
 *******************************************************/

/* FILE *_fopen(const char *, const char *); */
vfs_file_t *vfs_open(vfs_t *vfs, const char *arg, char *mode) {
	return 0;
}

/*int _fclose(FILE * __f);*/
int vfs_closef(vfs_file_t *vfs_file) {
	return 0;
}

/* #define S_ISDIR(m)	(((m)&DT_DIR) == DT_DIR) */
int VFS_ISDIR(char mode) {
	return 0;
}

/* #define S_ISREG(m)	(((m)&DT_REG) == DT_REG) */
int VFS_ISREG(char mode) { //�P�_���o��������ɮסA�ؿ��A�ɮסA�]���ɵ�
	return 0;
}

/* int _feof(FILE *fp); */
int vfs_eof(vfs_file_t *vfs_file) {
	return 0;
}

/*int _rename(const char *, const char *);*/
int vfs_rename(vfs_t *vfs, char *renamefrom, const char *arg) {
	return 0;
}

/* int _unlink(const char *); */
int vfs_remove(vfs_t *vfs) {
	return 0;
}

/* size_t _fread(void *, size_t, FILE *); */
int vfs_read(char *buffer, int len, vfs_file_t *vfs_file) {
	return 0;
}

/*size_t _fwrite(const void *, size_t, FILE *);*/
int vfs_write(char *payload, int start, int ln, vfs_file_t *vfs_file) {
	return 0;
}
