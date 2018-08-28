/*
 * yl_sys.h
 *
 *  Created on: 2008/12/10
 *      Author: ww
 */

#ifndef yl_sys_H_
#define yl_sys_H_

#include <stdlib.h>
#include <string.h>
#include "yl_oswrap.h"

/******************************************************************************
 * System functions
 * ============================================================================
 *
 * 2008-0918 wsw created
 * 		added some YL platform provided system calls
 * 		moved time marco here
 *
 * 2008-0915 added time comparison macros using uItron get_tim
 * 2008-0918 added systick
 *
 *****************************************************************************/

#define __LUSIMUS__
//#define OS_MEM_MGR		// use OS memory management

// ==== time comparison macros ===========================
// OSTICK --> uItron internal tick, mili-second, 32bit width only
// gets overflowed quickly
#define OSTICK get_tim()

#define typecheck(type,x) ({ \
	type __dummy; \
	typeof(x) __dummy2; \
	(void)((type*)&__dummy == (type*)&__dummy2); \
	1; \
})

#define time_after(a,b) ( \
    typecheck(unsigned long, a) && \
     typecheck(unsigned long, b) && \
     ( ((long)(b) - (long)(a)) < 0) \
)

#define time_before(a,b)        time_after(b,a)

// SYSTICK --> System maintained tick, 50 mili-second resolution, 64bit width
// can't use time_after & time_before, these are 32-bit
QWORD tq_timerGetTimerTick(void);
#define SYSTICK tq_timerGetTimerTick()

#if __GNUC__ > 4

#ifndef strcmpi
#define strcmpi strcasecmp
#endif
#ifndef stricmp
#define stricmp strcasecmp
#endif
#ifndef strncmpi
#define strncmpi strncasecmp
#endif
#ifndef strnicmp
#define strnicmp strncasecmp
#endif

#endif

// ==== utils/ucon.c =================================
typedef void (*CMDPROC)( int, char ** );

typedef struct _UCON_ARG_CMD {
	CMDPROC func;			// Pointer to func
	const char*   cmd;		// Pointer to command string
	const char*   desc;		// Pointer to description
} UCON_ARG_CMD;

void uconLineReset(void);
int uconInputCheck(void);
char* uconGetCmdBuf(void);
int uconExec(void);
char* uconAsk(void);


// ==== task lpi
/**@brief �Ǧ^ LPI �һݰt�m�Ŷ�
 *
 * �C�� Task ���|�����ۨ�����e���|�A��Task's local path infomation �N�� task creation �ɰt�m�A�̷ӷf�t���ɮרt�Τ��P�A�t�λݴ��ѩһݪŶ��H�ѧ@�~�t�ΰt�m�C
 * @ref cre_tsk ���ѿﶵ�O�_�n�t�m LPI
 *
 * @return �һݪŶ� (Bytes)
 */
inline int lpiSize(void);


/**@brief ���o Task ��e�u�@���|
 *
 * @return File system ��e�u�@���|�A���A���M��ϥΤ� File system�C
 */
inline void* lpiGetCWD();


/**@brief ���o Task ��e�ϥΤ��� File system handle
 *
 * File system handle �����A�άO�_�ݭn�� handle ���M��ҨϥΤ��ɮרt�ά[�c�C
 *
 * @return File system handle ����
 */
inline void* lpiGetFSHandle();


/**@brief �]�w Task ��e�ϥΤ��� File system handle
 *
 * File system handle �����A�άO�_�ݭn�� handle ���M��ҨϥΤ��ɮרt�ά[�c�C
 *
 * @param ptr system handle ����
 */
inline void lpiSetFSHandle(void* ptr);

/**@} GROUP_SYS_SERVICE *********************************************************************/



// ==== utils/string-util.c =================================
char *yl_strdup(const char *str);

ucs2_t* yl_wconcat(ucs2_t* absPath,const ucs2_t* relPath);
ucs2_t* yl_wstrcpy(ucs2_t* dst,const ucs2_t* src);
ucs2_t *yl_wstrdup(const ucs2_t *str);
int yl_wstrlen(const ucs2_t* uni);
int yl_strtoaddr(char* str,DWORD *value);
int yl_strtol(char* str,int *value);
int yl_strtoul(char* str,DWORD *value);
double yl_strtod(const char *str, char **endptr);

#define strtod  yl_strtod
#define atof(x) yl_strtod(x,0)

#define	strdup	yl_strdup

#define wcscat	yl_wconcat
#define	wcslen	yl_wstrlen
#define wcscpy	yl_wstrcpy
#define wcsdup	yl_wstrdup
// ==== utils/getopt.c =================================
int yl_getopt(int argc, char **argv, char *optionS);
extern char *yl_optarg;
extern int yl_optind;

#define getopt yl_getopt
#define optarg yl_optarg
#define optind yl_optind


// ==== utils/printf-stdarg.c =================================
int yl_uartPrintf(const char *szFmt, ...);
int yl_sprintf(char *out, const char *format, ...);
int tq_uartPutChar(DWORD c);
int tq_uartGetChar(void);

#ifdef DBGOUT_ON
#define dbgout				yl_uartPrintf
#else
#define dbgout(...)
#endif

#define printf				yl_uartPrintf
#define sprintf				yl_sprintf
#define putch(c)			({int x=c; if (x=='\n') tq_uartPutChar('\r');tq_uartPutChar(x);}) // ww 2014-0123 added variable x.2014-0121 putch is modified to interpret \n as \r\n
#define getch				tq_uartGetChar

// ==== utils/malloc.c ======================================
#ifdef OS_MEM_MGR
void* tx_os_malloc(DWORD size);
void* tx_os_calloc(DWORD num, DWORD size);
void* tx_os_realloc(void *mptr, DWORD size);
void tx_os_free(void* memptr);
DWORD tx_os_mleft(void);

#define malloc				tx_os_malloc
#define realloc				tx_os_realloc
#define calloc				tx_os_calloc
#define free				tx_os_free
#define mleft				tx_os_mleft
#endif

// ==== stdlib.h ===========================================
void srand(unsigned __seed);
int rand(void);
unsigned long strtoul(const char *__n, char **__end_PTR, int __base);


// ==== util/memmgr.c =======================================
#ifndef OS_MEM_MGR
/** @brief �O���骬�A���c
 *
 * @sa memGetStatus
 */
typedef struct {
	DWORD used;		///< �w�ϥΤ��O����(byte)
	DWORD segs;		///< �D�s�����O����϶��ƶq(���q)
	DWORD largest;	///< �̤j���ۥѰO����϶�
	DWORD mcbs;		///< �O���鱱��϶�(Memory Control Block)���ƶq
} MEMORY_STATUS_T;


/**@brief ��l�ưO����\��
 *
 * �O����N�|�ѰO����޲z�{���t�d�t�m�ϥΡA�{���Y�мg���O����Ŷ��A�N�l���t�m�����϶��A�y���O����޲z�{������B�@
 *
 * @param base �ťժ��O����Ŷ�
 * @param size ���O����Ŷ����j�p
 */
void memInit(DWORD base, DWORD size);


/**@fn void* malloc(DWORD size_requested);
 * @brief �n�D�t�m�O����Ŷ�
 *
 * @param size_requested �ҭn�D���O����Ŷ��j�p
 *
 * @return �t�m�n���O����Ŷ���}
 * @cond INTERNAL_USE
 */
void* memAlloc(DWORD size_requested);
#define malloc				memAlloc
///@endcond


/**@fn void* realloc(void* memory, DWORD size_requested);
 * @brief ���s�t�m�ª��O����Ŷ�
 *
 * @param memory ���O����Ŷ����Ҧb��}
 * @param size_requested �n�D���s�t�m���O����Ŷ��j�p
 *
 * @return �t�m�n���O����Ŷ���}
 * @cond INTERNAL_USE
 */
void* memRealloc(void* memory, DWORD size_requested);
#define realloc				memRealloc
///@endcond


/**@fn void free(void* memory);
 * @brief ����ҫ��w���O����Ŷ�
 *
 * @param memory �����񤧰O����Ŷ����Ҧb��}
 * @cond INTERNAL_USE
 */
void memFree(void* memory);
#define free				memFree
///@endcond


/**@fn void *malloca(DWORD size, int alignbits);
 * @brief �V�t�έn�D�t�m�����}���O����Ŷ�
 *
 * @param size ���t�m���O����j�p
 * @param alignbits ����O�����}�����n����balignbits�W(0~31 bit)
 *
 * @return �t�m�n���O����Ŷ���}
 * @cond INTERNAL_USE
 */
void *memAlignedAlloc(DWORD size_requested, int alignbits);
#define malloca				memAlignedAlloc
///@endcond


/**@fn void *mallocr(DWORD size, int alignbits);
 * @brief �V�t�έn�D�t�m�O����Ŷ��A�åB�Ѩ����j size_requested ���̤p�ۥѪŶ������ݰt�m
 *
 * @param size_requested ���t�m���O����j�p
 *
 * @return �t�m�n���O����Ŷ���}
 * @cond INTERNAL_USE
 */
void* memRevAlloc(DWORD size_requested);
#define mallocr				memRevAlloc
///@endcond


/**@brief ���o�ثe�O���餧���A
 *
 * @param pms �I�s�̴��� @ref MEMORY_STATUS_T ���A�����c���СA�� null ���ϥΡC
 *
 * @retval pms memGetStatus �N�|��J�O���������T�C
 *
 * @return �Ѿl���ϥΤ��O����Ŷ��C
 */
DWORD memGetStatus(MEMORY_STATUS_T* pms);


/**@fn void* calloc(DWORD num, DWORD size);
 * @brief �ʺA�t�m�O����Ŷ�, �ñN�t�m�᪺�Ŷ���ƥ����]��0
 *
 * @param num ���t�m���϶��Ӽ�
 * @param size ���t�m�϶��Ӽƪ��j�p
 *
 * @return �t�m�������O�����}
 *
 * @sa malloc, realloc
 * @cond INTERNAL_USE
 */
#define calloc(num,size)			({ \
	register void* p; \
	register unsigned long t=num*size; \
	p=malloc(t); \
	if (p) memset(p,0,t); \
	p; \
})
///@endcond


#endif

/**@} GROUP_SYS_LIBC *********************************************************************/


// ==== Driver Task message structure =========================
typedef struct drvmsg_struct {
	void* tskid;			///< The task id of caller
	void *mptr;				///< The media pointer of file system to be processed
} DRVMSG, *DRVMSG_PTR;


// ==== Central Resource Depot Management =======================
typedef DWORD HRES;
#define CRD_SEEKSET			0
#define CRD_SEEKCUR			1
#define CRD_SEEKEND			2


int crdRegisterFile(char* file);
int crdRegisterMem(const BYTE* addr);
int crdRegisterNAND(int startBlock);
int crdInit(void);
HRES crdOpen(DWORD resid);
int crdRead(HRES hRes, BYTE* buf, DWORD size);
int crdSeek(HRES hRes, int off, DWORD where);
int crdTell(HRES hRes);
int crdEOF(HRES hRes);
void crdClose(HRES hRes);
void* crdLock(HRES hRes);
DWORD crdSize(HRES hRes);


// ==== mem trace ====
void istracemem(void* p);
void untracemem(void* p);
void tracemem(void* p);


// ============================================================================================================================
// ============================================================================================================================
// ============================================================================================================================
#define ASSERT(s,a)	{if (!(a)) {int yl_uartPrintf(const char *format, ...);YL_PSR_SAVE_AREA;XDISABLE;yl_uartPrintf("**ASSERT**\n%s\n",s);while(1);XRESTORE;}}


#endif /* yl_sys_H_ */
