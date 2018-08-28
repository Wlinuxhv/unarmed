/******************************************************************************
 * YL_FSIO.h -- YL File system I/O routines
 * ============================================================================
 *
 *
 * 2008-xxxx Chris
 *
 *
 *(\~~/)(\~~/)
 *('.' )(^.^ )
 * (_(__ (_(__)~*
 *****************************************************************************/
#ifndef YL_FSIO_H_
#define YL_FSIO_H_

#define MAX_PATH_LENGTH 256		//FX_MAX_LONG_NAME_LEN

#ifndef BUFSIZ
# define BUFSIZ 4096
#endif

typedef void* FILE;
#define __FILE_defined // ww 2018-0604 added, to avoid conflicting types with new gcc/dev

#include <platform/YL_Type.h>

#define	O_RDONLY	0		/* +1 == FREAD */
#define	O_WRONLY	1		/* +1 == FWRITE */
#define	O_RDWR		2		/* +1 == FREAD|FWRITE */
#define	O_APPEND	0x0008
#define	O_CREAT		0x0200
#define	O_TRUNC		0x0400

struct	stat {
  mode_t	st_mode;
  off_t		st_size;
  struct tm	st_atime;
  struct tm	st_mtime;
  struct tm	st_ctime;
};

/* Directory part */
struct dirent {
	unsigned int	d_off;
	unsigned short	d_reclen;
	unsigned char	d_type;
	char			d_name[256];
};

typedef void* DIR;

/* File types to use for d_type */
#define DT_UNKNOWN	 0
#define DT_DIR		 4
#define DT_REG		 8
#define DT_RDONLY	0x01		//FX_READ_ONLY
#define DT_HIDDEN	0x02		//FX_HIDDEN
#define DT_ARCHV	0x20		//FX_ARCHIVE
//#define DT_VOL		0x08		//FX_VOLUME
//#define DT_SYS		0x04		//FX_SYSTEM
//#define DT_DIR		0x10		//FX_DIRECTORY


// make stat -> st_mode equal to attribute in FILEX
#define S_ISDIR(m)	(((m)&DT_DIR) == DT_DIR)
#define S_ISREG(m)	(((m)&DT_REG) == DT_REG)
#define S_ISHID(m)	(((m)&DT_HIDDEN) == DT_HIDDEN)
#define S_ISARC(m)	(((m)&DT_ARCHV) == DT_ARCHV)

#define SEEK_SET	0		//FX_SEEK_BEGIN
#define SEEK_END	1		//FX_SEEK_END
#define SEEK_CUR 	2		//FX_SEEK_FORWARD

extern FILE *_fopen(const char *, const char *);
extern int _fclose(FILE * __f);
extern int _fseek(FILE * __f, off_t __o, int __w);
extern off_t _ftell(FILE * __f);
extern int _stat(const char *, struct stat *);
extern int _truncate(const char *, off_t);
extern int _fputs(const char *, FILE *);
extern int _feof(FILE *fp);
extern int _fputc(int, FILE *);
extern int _fgetc(FILE *);
extern char *_fgets(char *, int, FILE *);
extern size_t _fread(void *, size_t, FILE *);
extern size_t _fwrite(const void *, size_t, FILE *);
extern int _rename(const char *, const char *);
extern int _unlink(const char *);
extern int _fflush(void);
extern FILE* _tmpfile(void);
int _ferror(FILE* stream);

extern DIR *_opendir(const char *);
extern struct dirent *_readdir(DIR *);
extern void _rewinddir(DIR *);
extern off_t _telldir(DIR *);
extern int _chdir(const char *);
extern int _mkdir(const char *);		/* mode permission no longer needed */
extern int _rmdir(const char *);
extern char *_getcwd(char *, unsigned long);
extern int _closedir(DIR *);
/* chris: the following is for unicode pathname manipulation */
extern int chdirW(const ucs2_t *path_name);
extern int mkdirW(const ucs2_t *path_name);
extern int rmdirW(const ucs2_t *path_name);

extern int wildcmp(const char *wild, const char *string);

/* chris: the following is for unicode pathname manipulation */
extern FILE *fopenW(const ucs2_t *file, const char *mode);
extern int unlinkW(const ucs2_t * file_name);
extern int statW(const ucs2_t *path, struct stat *buf);
ucs2_t* getcwdW(ucs2_t* absPath,unsigned int size);


// ==== makepath.c ======================================
void  yl_splitpath(const char* inpath, char *out);
void yl_makepath(char * path, const char * drive, const char *dir, const char * filename, const char * ext);
char* yl_fullpath(char* ppart);
void specialcombine(char* pathbuf);
void beautifypath(char* start);
void pathcat(char* s1, char* s2);

void  yl_splitpathW(const ucs2_t* inpath, ucs2_t *out);
void yl_makepathW(ucs2_t *path, const ucs2_t *drive, const ucs2_t *dir,const ucs2_t *filename, const ucs2_t *ext);
ucs2_t* yl_fullpathW(ucs2_t* ppart);
void specialcombineW(ucs2_t* pathbuf);
void beautifypathW(ucs2_t* start);
void pathcatW(ucs2_t* s1, ucs2_t* s2);


// ==== filefiles.c ==============================================================
#define	FNM_NOMATCH	1	/* Match failed. */
#define	FNM_MATCH	0	/* Match ok. */

#define SP_DRV(b)		(((char**)b)[0])
#define SP_DIR(b)		(((char**)b)[1])
#define SP_NAME(b)		(((char**)b)[2])
#define SP_EXT(b)		(((char**)b)[3])
#define SP_EXTRA_SPACE	0x20
#define SP_MAX_PATH		(MAX_PATH_LENGTH+SP_EXTRA_SPACE)

#define WSP_DRV(b)		(((ucs2_t**)b)[0])
#define WSP_DIR(b)		(((ucs2_t**)b)[1])
#define WSP_NAME(b)		(((ucs2_t**)b)[2])
#define WSP_EXT(b)		(((ucs2_t**)b)[3])
#define WSP_MAX_PATH	((MAX_PATH_LENGTH+SP_EXTRA_SPACE)*2)

#define FF_TIME_AFTER		0x04000000
#define FF_TIME_BEFORE		0x08000000
#define FF_SIZE_BIGGER		0x10000000
#define FF_SIZE_SMALLER		0x20000000
#define FF_ATTR_AND			0x40000000
#define FF_ATTR_OR			0x80000000

#define FA_RDONLY			0x01		//FX_READ_ONLY
#define FA_HIDDEN			0x02		//FX_HIDDEN
#define FA_SYSTEM			0x04		//FX_SYSTEM
#define FA_LABEL			0x08		//FX_VOLUME
#define FA_DIR				0x10		//FX_DIRECTORY
#define FA_ARCH 			0x20		//FX_ARCHIVE

typedef struct _ffblk {
	DWORD	size;				/* if size > 0, compare size */
	struct tm	time;				/* convet to time_t before use it */
	DWORD	attr;
	char	*search_path;
	char	*rule_set;

	void *pfx_path;	// caller must prepare the sturcture
	void *pfx_media;		// media pointer will be valid after calling find_first
	DWORD	entry_offset;
	DWORD	cwd_num_entries;

	DWORD magic;

	int		ff_hasuname;			// the file has unicode name
	ucs2_t* ff_longname;
	char* 	ff_name;
	DWORD	ff_attr;
	DWORD	ff_size;
	struct tm	ff_time;

} ffblk;

ffblk* find_init(void);
int	find_first(ffblk *ff);
int	find_next(ffblk *ff);	/* return NULL if no more */

// ==== charconv.c ==============================================================
int fx_big5_name_get(void* media_ptr, char *src_short_name, BYTE *dest_big5_name, int dest_size);

// ==============================================================================


#define fopen 	_fopen
#define fclose	_fclose
#define fseek	_fseek
#define ftell	_ftell
#define	stat(p,b)	_stat((p),(b))
#define truncate	_truncate
#define fputs	_fputs
#define fputc	_fputc
#define putc(c,f)  _fputc((c),(f))
#define fgetc	_fgetc
#define fgets	_fgets
#define getc(f) _fgetc(f)
#define feof 	_feof
#define	fread	_fread
#define fwrite	_fwrite
#define rename	_rename
#define unlink	_unlink
#define fflush	_fflush
#define tmpfile _tmpfile
#define ferror _ferror

#define opendir	_opendir
#define readdir	_readdir
#define rewinddir 	_rewinddir
#define telldir	_telldir
#define chdir	_chdir
#define mkdir	_mkdir
#define rmdir	_rmdir
#define getcwd	_getcwd
#define closedir	_closedir

#define splitpath			yl_splitpath
#define makepath			yl_makepath
#define fullpath			yl_fullpath

#define splitpathW			yl_splitpathW
#define makepathW			yl_makepathW
#define fullpathW			yl_fullpathW



/**********************************************************************************************//**
 * @defgroup GROUP_SYS_DRVMAN Drive Manager
 * @{
 *************************************************************************************************/

// ==== Drive Manager APIs =========================
/** @brief �ϺвպA, Disk Geometry
 *
 * @sa IOCTRL_DRVIVE_GET_GEO
 */
typedef struct {
	int cylinders;	///< number of tracks (�ϬW��)
	int heads;		///< heads (���Y)
	int sectors;	///< Sectors per track (�ϰ�), �q�`��512 bytes
	int sec_size;	///< Sector size in bytes, �ϰϤj�p
	int cluster;	///< sectors per cluster (���L; �ϰ��O��), �Ϻо����̤p���s�����, �Ѧh��sectors�զ�
} DRIVE_GEO_T;


/**@brief �˸m�X�ʵ{�����Ƶ{���ŧi
 *
 * @param media_ptr �C�鱱�����
 *
 * @note ���禡���A���M��ҨϥΤ� File system�C
 *
 * @sa REGISTER_DRIVE_T, yl_dmRegister
 */
typedef void (*DRIVERFUNC)(void *media_ptr);


/**@brief �˸m����R�O�B�z�{�����Ƶ{���ŧi
 *
 * @param arg0		�q�`���˸m�X�ʵ{���һݤ� extra infomation
 * @param arg1		�q�`�� IO ����X
 * @param arg2		�q�`������X�Ѽ�
 *
 * @return �ӱ���X���Ǧ^��
 *
 * @note ���禡�\����M��˸m�X�ʵ{���������]�p
 *
 * @sa REGISTER_DRIVE_T, yl_dmRegister, yl_dmIOCtrl
 */
typedef int (*IOCTL)(DWORD arg0, DWORD arg1, DWORD arg2);


/**@brief ���U�˸m�Ѽ�
 *
 * @sa yl_dmRegister
 */
typedef struct {
	char* name;			///< �˸m�W�١A�i�� null
	DRIVERFUNC driver;	///< �˸m�X�ʵ{��
	void* ext;			///< �˸m���B�~��T, �N�|�e�{�b  FX_MEDIA_PTR.fx_media_driver_info, @note ���e���M��ҨϥΤ� File system�C
	char* addr;			///< �˸mBuffer���_�l�O�����m�C �Y��m�Ȭ�0, �h�t�~�ϥΰt�m���O����C
	int bufsize;		///< �u�@�w�İϤj�p, �H�x�s�C��̤p�x�s��쬰�D; ex:FAT�ϥΪ��̤p��쬰512 bytes
	IOCTL ioctl;		///< �˸m������R�O�B�z�{��
} REGISTER_DRIVE_T;


/**@brief �榡�ưѼ�
 *
 * @sa yl_dmFormat
 */
typedef struct FORMAT_PARAM_S {
	char* vol;			///< �ɮרt�μ���(volume label)
	int nFATs;			///< FAT���ƶq
	int nRootDirEnt;	///< �ڥؿ����ؿ��ƶq
	int nHiddenSec;		///< ���úϰϤ��ƶq
} FORMAT_PARAM_T;

/**********************************************************************************************//**
 * @defgroup GROUP_SYS_CORRECT_OPT Check & Correcting options
 * @{
 * @sa yl_dmMediaCheckRepair
 *************************************************************************************************/
#define	FAT_CHAIN_ERROR		0x01		//FX_FAT_CHAIN_ERROR		///< FAT ����~
#define DIRECTORY_ERROR		0x02		//FX_DIRECTORY_ERROR		///< �ؿ����c�l��
#define LOST_CLUSTER_ERROR	0x04		//FX_LOST_CLUSTER_ERROR	///< ���Τ��s�򪺺��L��

/**<
 * �b�Ϻо��ϥΤ@�q�ɶ�����, �i��]���ɮת����ʦ��ƼW�[, �y���F�i�κ��L(free cluster)�ä��|�s��;
 * ���ɭY�@���ɮצs��b�Ϻо��W, �i��L�k���γs�򪺺��L, �S�@�~�t�Φb�R���έק��ɮ׮�, ���ε{���i��]��
 * �Y�ؽt�G���_�q�ӳy�����, �άO�����`�a�����{��. �����ɮת��s���ʧ@�Y������, �i��|�y���Y�Ǻ��L���M�w�g
 * ���ϥΤF, ���O�ɮװt�m���Хܥ����ϥΪ��A, �y�����Ϊ����L��. ���ت��p�N�٬�lost cluster.
 */

#define	FILE_SIZE_ERROR		0x08		//FX_FILE_SIZE_ERROR		///< �ɮפj�p���~
/**<
 * �P�ҵ��U���ɮפj�p�P�ɮ��쪺�j�p���P�A�Ȭ������ﶵ�A�D�״_�ﶵ���@�C
 */
/**@} GROUP_SYS_CORRECT_OPT *********************************************************************/



/**@brief �V�t�ε��U�x�s�˸m
 *
 * @param drvnum �����U���Ϻмѽs��(0~3;A~D)
 * @param rdrv ���U����T
 *
 * @return
 * 0: OK
 * - @ref YLERR_ARG
 * - @ref YLERR_DRIVE, �Ϻмѽs�����X�k
 *
 * @note �N�˸m���U��b�t�ΤW�٥���ϥ�, ���g�L������~��ϥθ˸m
 *
 * @sa yl_dmMount
 */
int yl_dmRegister(int drvnum, REGISTER_DRIVE_T *rdrv);


/**@brief �Ѧw��(���U)
 *
 * @param drvnum ���Ѱ����Ϻмѽs��(0~3;A~D)
 *
 * @return no return
 */
int yl_dmUnregister(int drvnum);


/**@brief �o���ϺмѬO�_����
 *
 * @param drvnum ���o���O�_�������Ϻмѽs��(0~3;A~D)
 *
 * @return
 * - 0: �L����
 * - 1: ������
 */
int yl_dmIsMounted(int drvnum);


/**@brief �N�Ϻмѱ�����t�ΤW
 *
 * @param drvnum ���������Ϻмѽs��(0~3;A~D)
 *
 * @return
 * - @ref YLERR_OK
 * - @ref YLERR_FAILURE, �������~
 * - @ref YLERR_ARG
 * - @ref YLERR_FS
 * - @ref YLERR_FAT
 * - @ref YLERR_DRIVERIO
 * - @ref YLERR_DRIVE ,�Ϻмѽs�����X�k
 */
int yl_dmMount(int drvnum);


/**@brief �N�Ϻмѱq�t�ΤW�����U��
 *
 * @param drvnum ���������Ϻмѽs��(0~3;A~D)
 *
 * @return
 * - @ref YLERR_OK
 * - @ref YLERR_FAILURE, �������~
 * - @ref YLERR_DRIVERIO
 * - @ref YLERR_DRIVE ,�Ϻмѽs�����X�k
 */
int yl_dmUnmount(int drvnum);


/**@brief �N�ϺмѮ榡��
 *
 * @param drvnum ���榡�Ƥ��Ϻмѽs��(0~3;A~D)
 * @param param �榡�ưѼ�, ��0�h�۰ʨϥιw�]�Ѽ�
 * �w�]�榡�Ƥ��ѼƬ�:
 * - vol : NULL
 * - nFATs : 2
 * - nRootDirEnt : 224
 * - nHiddenSec : 0
 *
 * @return
 * - @ref YLERR_OK
 * - @ref YLERR_FAILURE, �������~
 * - @ref YLERR_DRIVERIO
 * - @ref YLERR_DRIVE ,�Ϻмѽs�����X�k
 * - @ref YLERR_NOTSUPP, ���ϺмѤ��䴩�榡��
 */
int yl_dmFormat(int drvnum, FORMAT_PARAM_T* param);


/**@brief ��ϺмѤU����R�O�ΰѼ�
 *
 * @param drvnum ������Ϻмѽs��(0~3;A~D)
 * @param arg1 ����R�O
 * @param arg2 ����Ѽ�; �I�s�̷̨ӱ���R�O���ѩһݭn���Ѽ�
 *
 * @return
 * - @ref YLERR_DRIVE ,�Ϻмѽs�����X�k
 * - @ref YLERR_NOTSUPP
 * - ��L: �ѷӦU����R�O���Ǧ^��
 *
 * @note arg2���@���Ъ�����, �I�s�̭n����; ���|�^�ǥt�@���Ц�}��Ҵ��Ѫ����Ф�
 *
 * @sa GROUP_SYS_DRVMAN_IOCTRL
 */
int yl_dmIOCtrl(int drvnum, DWORD arg1, DWORD arg2);


/**@brief ���|�ѪR
 *
 * �ѪR���|�A�N�۹�ε�����|�A�P Task ��e�u�@�ؿ��X�֡A�Ǧ^�������ɮרt�α�����ХH�Τ��t�ϺмѤ�������|�C
 * ��e�u�@�ؿ��A�Y���󴫹L�w�]�� "A:/"
 *
 * @param path ���ѪR�����|
 * @param out ���t�ϺмѤ����|
 * @param media �ӺϺмѪ�����϶�����
 *
 * @return
 * - !=0: �Ϻмѽs��
 * - @ref YLERR_NOMEM
 * - @ref YLERR_INVAL
 */
int yl_dmParsePath(const char* path, char **out, void **media);


/**@brief Unicode�����|�ѪR
 *
 * �ѪR���|�A�N�۹�ε�����|�A�P Task ��e�u�@�ؿ��X�֡A�Ǧ^�������ɮרt�α�����ХH�Τ��t�ϺмѤ�������|�C
 * ��e�u�@�ؿ��A�Y���󴫹L�w�]�� "A:/"
 *
 * @param path ���ѪR�����|(Unicode)
 * @param out_short_path ���t�ϺмѤ��u�ɦW���|
 * @param which_media �ӺϺмѪ�����϶�����
 *
 * @return
 * - !=0: �Ϻмѽs��
 * - @ref YLERR_NOMEM
 * - @ref YLERR_INVAL
 * - @ref YLERR_INVLPATH
 */
int yl_dmParsePathW(const ucs2_t *path, char **out_short_path, void **which_media);


/**@brief ���o�ϺмѤ�����϶�����
 *
 * @param drvnum �����o����϶����Ф��Ϻмѽs��(0~3;A~D)
 *
 * @return
 * - !=0 : �ӺϺмѤ��������
 * - 0 : ���Ϻмѽs�����s�b
 */
void* yl_dmGetFSHandle(int drvnum);


/**@brief ���o�ӱ���϶����Ф��Ϻмѽs��
 *
 * @param mp ���o���Ϻмѽs�����������
 *
 * @return
 * - @ref YLERR_FAILURE, �������~
 * - @ref YLERR_DRIVE, ��J������϶����Ь� 0 �ɵo��
 */
int yl_dmGetDrvIndex(void *mp);


/**@brief �C���ˬd/�״_
 *
 * @param mp ���ˬd/�״_���Ϻмѽs��������϶�����
 * @param correct_option ���״_������; 0�����״_, �״_���ج�:@ref GROUP_SYS_CORRECT_OPT
 * @param error_detected �^�ǵo�{�����~����
 *
 * @return
 * - @ref YLERR_OK
 * - @ref YLERR_FAILURE, �������~
 * - @ref YLERR_FAT
 * - @ref YLERR_DRIVERIO
 * - @ref YLERR_DRIVE
 * - @ref YLERR_NOMEM
 */
int yl_dmMediaCheckRepair(void *mp, DWORD correct_option, DWORD *error_detected);


/**@brief ���o�ϺмѤ��i�ϥΪŶ�(byte)
 *
 * @param mp ���o�����ϺмѪ�����϶�����
 * @param freespace �i�ϥΤ��Ŷ�
 *
 * @return
 * - @ref YLERR_OK
 * - @ref YLERR_DRIVE
 */
int yl_dmGetMediaFreeSpace(void *mp, DWORD *freespace);


/** @brief ���o�ϺмѤ��w�ϥΪŶ�(byte)
 *
 * @param mp ���o���w�ϥΪ��Ϻмѽs��������϶�����
 * @param usage �w�ϥΤ��Ŷ�
 *
 * @return
 * - @ref YLERR_OK
 * - @ref YLERR_DRIVE
 */
int yl_dmGetMediaUsage(void *mp, DWORD *usage);


/** @brief ���o�ϺмѤ��`�e�q(byte)
 *
 * @param mp ���o���Ϻмѽs��������϶�����
 * @param total �Ǧ^���Ϻм��`�e�q
 *
 * @return
 * - @ref YLERR_OK
 * - @ref YLERR_DRIVE
 * - @ref YLERR_NOTSUPP
 */
int yl_dmGetMediaTotalSpace(void *mp, DWORD *total);

/**********************************************************************************************//**
 * @defgroup GROUP_SYS_DRVMAN_IOCTRL IO Control Codes
 * @{
 * @sa yl_dmIOCtrl
 *************************************************************************************************/
/** @brief ���o�ϺмѸ�T
 *
 * @note �I�s�̶����Ѥ@@ref DRIVE_GEO_T���A���ܼ�, ��@ref yl_dmIOCtrl��arg2\n;
 * �I�s��������, ���ܼƴN��DRIVE_GEO_T��������T
 *
 * @sa yl_dmIOCtrl, DRIVE_GEO_T
 */
#define IOCTRL_DRVIVE_GET_GEO				0x0001

/** @brief ���s�榡��
 *
 * @sa yl_dmIOCtrl
 *
 * @note �byl_dmIOCtrl�𶷵�arg2�Ѽ�
 */
#define IOCTRL_NFTL_RAW_FORMAT				0x0002

/** @brief �C���榡��
 *
 * @sa yl_dmIOCtrl
 *
 * @note �byl_dmIOCtrl�𶷵�arg2�Ѽ�
 */
#define IOCTRL_NFTL_LOW_LEVEL_FORMAT		0x0003

/** @brief ��l��NFTL
 *
 * @note NFTL(NAND Flash Translation Layer), �D�n�\��O�NLogical Block Address������\n
 * ���骺flash memory address, �Ǧ��ӹF�����flash�ϥ�FAT���Φbblock device�o����file system
 *
 * @note �byl_dmIOCtrl�𶷵�arg2�Ѽ�
 *
 * @sa yl_dmIOCtrl
 */
#define IOCTRL_NFTL_INITIALIZE				0x0004

/** @brief ���o�޿�/�����ഫ��
 *
 * @note ������16�줸(WORD**)�s��V�q���}�C����, �|�^�ǦV�q������
 *
 * @sa yl_dmIOCtrl
 */
#define IOCTRL_NFTL_LP_TABLE				0x0005

/** @brief ���o�϶��쵲��
 *
 * @note ������16�줸(WORD**)�s��V�q���}�C����;\n
 * �ŧiargs[2], args[0]���V�q������; args[1]���V�q���}�Y��}(���)
 *
 * @sa yl_dmIOCtrl
 */
#define IOCTRL_NFTL_VEC_TABLE				0x0006

/** @brief ���o�϶��~�֪�
 *
 * @note �C�M���@��, �~�ִN�[1; ������16�줸(WORD**)�s��V�q���}�C����, �|�^�ǦV�q������
 *
 * @sa yl_dmIOCtrl
 */
#define IOCTRL_NFTL_AGE_TABLE				0x0007

/** @brief ���~�ˬd�����A
 *
 * @note @ref yl_dmIOCtrl���^�ǭȬ��ҭ״_������; arg2�n���Ѥ@����, �|�Ǧ^���i�״_������
 *
 * @sa yl_dmIOCtrl
 */
#define IOCTRL_NFTL_ECC_STATUS				0x0008

/** @brief ���NFTL��T
 *
 * @sa yl_dmIOCtrl
 *
 * @note NFTL(NAND Flash Translation Layer), �D�n�\��O�NLogical Block Address������\n
 * ���骺flash memory address, �Ǧ��ӹF�����flash�ϥ�FAT���Φbblock device�o����file system
 */
#define IOCTRL_NFTL_DISPINFO				0x0009

/** @brief ���mRAM DISK���Ŷ�, �N���M��0
 *
 * @note �byl_dmIOCtrl�𶷵�arg2�Ѽ�
 *
 * @sa yl_dmIOCtrl
 */
#define IOCTRL_RAMDRV_RESET_MEMORY			0x0010
/**@} GROUP_SYS_DRVMAN_IOCTRL *********************************************************************/

/**@} GROUP_SYS_DRVMAN *********************************************************************/


#endif /*YL_FSIO_H_*/
