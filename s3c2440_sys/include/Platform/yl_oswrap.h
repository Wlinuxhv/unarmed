/**************************************************************************//**
 * @file YL_OSWrap.h
 * ============================================================================
 *
 *  Created on: 2010/6/25
 *      Author: Wesley Wang
 *
 *
 *(\~~/)(\~~/)
 *('.' )(^.^ )
 * (_(__ (_(__)~*
 *****************************************************************************/

#ifndef YL_OSWRAP_H_
#define YL_OSWRAP_H_

#include "YL_Type.h"

/**********************************************************************************************//**
 * @defgroup GROUP_OSWRAP OS Wrapper for UCOS II
 * @{
 * @brief OS Wrapper for UCOS II
 *
 *************************************************************************************************/

///@cond INTERNAL_USE
#define cre_tsk					yl_cre_tsk
#define del_tsk					yl_del_tsk
#define sta_tsk					yl_sta_tsk
#define ter_tsk					yl_ter_tsk
#define ext_tsk					yl_ext_tsk
#define exd_tsk					yl_exd_tsk
#define ref_tsk					yl_ref_tsk
#define rot_rdq					yl_rot_rdq
#define get_tid					yl_get_tid
#define wup_tsk					yl_wup_tsk
#define rel_wai					yl_rel_wai
#define dly_tsk					yl_dly_tsk
#define tslp_tsk				yl_tslp_tsk
#define tsk_var 				yl_tsk_var

#define cre_flg					yl_cre_flg
#define del_flg					yl_del_flg
#define set_flg					yl_set_flg
#define clr_flg					yl_clr_flg
#define twai_flg				yl_wai_flg

#define cre_sem					yl_cre_sem
#define del_sem					yl_del_sem
#define sig_sem					yl_sig_sem
#define twai_sem				yl_wai_sem

#define cre_mbx					yl_cre_mbx
#define del_mbx					yl_del_mbx
#define snd_msg					yl_snd_msg
#define trcv_msg				yl_rcv_msg

#define cre_mtx					yl_cre_mtx
#define del_mtx					yl_del_mtx
#define put_mtx					yl_put_mtx
#define tget_mtx				yl_get_mtx

#define def_cyc					yl_def_cyc
#define act_cyc					yl_act_cyc
#define del_cyc					yl_del_cyc
#define chg_cyc					yl_chg_cyc

#define cre_mpf					yl_cre_mpf
#define del_mpf					yl_del_mpf
#define tget_blf				yl_get_blf
#define rel_blf					yl_rel_blf

#define get_tim					yl_get_tim
///@endcond


/**********************************************************************************************//**
 * @defgroup GROUP_OSWRAP_ERRCODE Error Codes
 * @{
 * @brief Error Codes
 *
 *************************************************************************************************/
#define OSWRAP_PTR_ERROR			-1		///< ptr error
#define OSWRAP_WAIT_ERROR			-2		///< wait error
#define OSWRAP_OPTION_ERROR			-3		///< option error ; arugments type or value are error or not supported.
#define OSWRAP_PRIORITY_ERROR		-4		///< priority error
#define OSWRAP_CALLER_ERROR			-5		///< caller error ; call in ISR
#define OSWRAP_WAIT_ABORTED			-6		///< wait aborted
#define OSWRAP_NOT_AVAILABLE		-7		///< not available
#define OSWRAP_NOT_DONE				-8		///< not done
#define OSWRAP_NO_MEMORY			-9		///< no memory
#define OSWRAP_NOT_EXIST			-10		///< not exist
#define OSWRAP_TIME_OUT				-11		///< timeout
#define OSWRAP_NOT_HANDLED			-254	///< not handled error code
#define OSWRAP_FEATURE_NOT_ENABLED	-255	///< feature not enable
/**@} GROUP_OSWRAP_ERRCODE *********************************************************************/



/**********************************************************************************************//**
 * @defgroup GROUP_OSWRAP_TSK Task
 * @{
 * @brief Task
 *
 *************************************************************************************************/
 /**@brief Task �� Cyclic ���A
 *
 * Task �� Cyclic �����ŧi�� void <b>TSK_func_name</b>(DWORD);
 */
typedef void   (* FUNCENTRY)(DWORD);

/**********************************************************************************************//**
 * @defgroup GROUP_OSWrap_TSK_VAR Task Local Variables
 * @{
 * @sa tsk_var
 *************************************************************************************************/
enum {
	TSK_ERRNO=0,		///< Last error number
	TSK_STRTOK,			///< Record of tasks's last token found position of strtok
	TSK_LPI,			///< Local path infomatoin
	// 2018-0823 no virtual console here
#if 0
	TSK_CONSOLE,		///< VCON number the task currently being used. //Jon added. 2011-1228.
#endif
	TSK_PRIVATE,		///< Task's private space //2018-0822 ww added

///@cond INTERNAL_USE
	NUM_TSKVARS			///< number of task variable
///@endcond
};
/**@} GROUP_OSWrap_TSK_VAR *********************************************************************/


// task attribute
#define TSK_START			(1<<31)	///< Auto start task on cration
#define TSK_NEEDLPI			(1<<30)	///< Task needs local path info
#define TSK_REVMEM			(1<<29)	///< Allocate the task's memory from bottom of memory segment, to avoid generating memory fragment when deleteing the task


/**@brief Create Task
 *
 * A task has its own errno and strtok record, the LPI is optional by @ref TSK_NEEDLPI
 *
 * @param ppid ID of newly create object to be stored
 * @param task Task Start Address
 * @param stksz Stack Size (in bytes)
 * @param atr_pri Initial Task Priority
 * @param private Private space
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *  - OSWRAP_NO_MEMORY
 *  - OSWRAP_PRIORITY_ERROR priority is not allowed (or duplicated priority in ucos ii)
 *  - OSWRAP_NOT_AVAILABLE no more TCB (ucos ii)
 *  - OSWRAP_CALLER_ERROR called in ISR
 */
int cre_tsk(void** ppid, FUNCENTRY task, int stksz, DWORD atr_pri, void* private);


/**@brief Delete Task
 *
 * @param ptr ���R���� task ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *  - OSWRAP_PTR_ERROR invalid ppid
 *  - OSWRAP_NOT_EXIST task is not exist, memory of pid will be released.
 *  - OSWRAP_CALLER_ERROR called in ISR
 *
 * @sa cre_tsk
 */
int del_tsk(void* ptr);

/**@brief Start Task
 *
 * @param ptr ���Ұʪ�task ID
 * @param stacd Task Start Code
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *  - OSWRAP_PTR_ERROR invalid ppid
 *  - OSWRAP_PRIORITY_ERROR priority is not allowed (or duplicated priority in ucos ii)
 *  - OSWRAP_NOT_AVAILABLE no more TCB (ucos ii)
 *  - OSWRAP_CALLER_ERROR called in ISR
 *
 * @sa cre_tsk
 */
int sta_tsk(void* ptr, DWORD stacd);

/**@brief Terminate Other Task
 *
 * @param ptr ���פtask ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *  - OSWRAP_PTR_ERROR invalid ppid
 *  - OSWRAP_NOT_DONE try to kill self.
 *  - OSWRAP_NOT_AVAILABLE not started.
 *
 * @sa cre_tsk
 */
int ter_tsk(void* ptr);

/**@brief Exit and Delete Task
 */
void exd_tsk(void);

/**@brief Exit Task
 */
void ext_tsk(void);

/**@brief Get Task Identifier
 *
 * @return Required task ID
 */
void* get_tid(void);

/**@brief Get Task variables
 *
 * @param varid Variable ID @ref GROUP_OSWrap_TSK_VAR
 *
 * @return Address of the required task variable
 */
void* tsk_var(int varid);

/**@brief Rotate Tasks on the Ready Queue
 */
void rot_rdq(void);

/**@brief Release Wait of Other Task
 * �j������bWAIT���A�U(���]�tSUSPEND)��task
 *
 * @param ptr ���j������task ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *  - OSWRAP_PTR_ERROR invalid ppid
 *  - OSWRAP_NOT_AVAILABLE task is not waiting
 *
 * @sa wai_flg, pwai_sem, twai_sem
 */
int rel_wai(void* ptr);

/**@brief Delay Task
 * �N�b���椤��task�Ȯɰ���@�q�ɶ�
 *
 * @param wait �����𪺮ɶ�(ms)
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *  - NONE
 */
int dly_tsk(DWORD wait);

/**@brief Sleep Task with Timeout
 * �N�bRUN state����task�]�w��sleep, ����@a wai �ɶ��B@ref wup_tsk�Q�I�s��~����
 *
 * @param wai ��sleep���ɶ�(ms)
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *  - OSWRAP_PTR_ERROR invalid ppid
 *  - OSWRAP_WAIT_ABORTED the waiting has been release by rel_wai
 *  - OSWRAP_TIME_OUT time out
 *
 * @sa slp_tsk, wup_tsk
 */
int tslp_tsk(DWORD wai);

/**@brief Wakeup Other Task
 *
 * @param ptr �������task ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *  - OSWRAP_PTR_ERROR invalid ppid
 *  - OSWRAP_NOT_AVAILABLE task is not waiting
 *
 * @sa slp_tsk, tslp_tsk
 */
int wup_tsk(void* ptr);

/**@brief Sleep Task
 * �N�bRUN state����task�]�w��sleep, �B���L�����, �@����@ref wup_tsk�Q�I�s����
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *  - OSWRAP_PTR_ERROR invalid ppid
 *  - OSWRAP_WAIT_ABORTED the waiting has been release by rel_wai
 *
 *
 * @sa tslp_tsk, wup_tsk
 */
#define slp_tsk()				tslp_tsk((DWORD)0xFFFFFFFF)
/**@} GROUP_OSWRAP_TSK *********************************************************************/


/**********************************************************************************************//**
 * @defgroup GROUP_OSWRAP_FLG Flags
 * @{
 * @brief Event flags
 *
 *************************************************************************************************/
//*************************************************************************
//               EVENT FLAGS MODE CONSTANTS                               *
//*************************************************************************
/**********************************************************************************************//**
 * @defgroup GROUP_OSWrap_EVENTFLG_WAI Wait Mode
 * @{
 * @sa twai_flg, pwai_flg, wai_flg
 *************************************************************************************************/
#define  TWF_ORW                            0	///< OR wait
/**<Specifies that any event flag is satisfactory
 * @note TWF_ORW �P TWF_ANDW �ȯ�ϥΨ�@.
 */
#define  TWF_CLR                            1	///< Clear.
/**<Event flags that satisfy the request are cleared
 * @note �i�P TWF_ORW �P TWF_ANDW �@ or �B��@�P�ϥ�.
 */

#define  TWF_ANDW                           2	///< AND wait
/**<Specifies that all event flags must be present in the group
 * @note TWF_ORW �P TWF_ANDW �ȯ�ϥΨ�@.
 */


/**@} GROUP_OSWrap_EVENTFLG_WAI *********************************************************************/

/**@brief Create Eventflag
 *
 * @param ppid ID of newly create object to be stored
 * @param iflgptn Initial Event Flag Pattern
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 */
int cre_flg(void** ppid, DWORD iflgptn);

/**@brief Delete Eventflag
 *
 * @param ptr ���R����Event Flag ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_flg
 */
int del_flg(void* ptr);

/**@brief Set Eventflag
 *
 * @param ptr ���]�m��Event Flag ID
 * @param ptn ���]�m��Bit Pattern
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_flg
 */
int set_flg(void* ptr, DWORD ptn);

/**@brief Clear Eventflag
 *
 * @param ptr ���M����Event Flag ID
 * @param ptn ���M����Bit Pattern
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_flg
 */
int clr_flg(void* ptr, DWORD ptn);

/**@brief Wait for Eventflag with Timeout
 *
 * @param pflgptn ���Ǧ^��Bit Pattern���ܼƫ���
 * @param ptr �����ݪ�Event Flag ID
 * @param waiptn �����ݪ�Bit Pattern
 * @param wfmode Wait Event Flag Mode
 * @param wait ���ݮɶ�(ms)
 *
 * @retval pflgptn �Ǧ^��Bit Pattern
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa GROUP_OSWrap_EVENTFLG_WAI
 */
int twai_flg(DWORD *pflgptn, void* ptr, DWORD waiptn, DWORD wfmode, DWORD wait);	//time wait

/**@brief Wait for Eventflag (Polling)
 *
 * @param a ���Ǧ^��Bit Pattern���ܼƫ���
 * @param b �����ݪ�Event Flag ID
 * @param c �����ݪ�Bit Pattern
 * @param d Wait Event Flag Mode
 *
 * @retval a �Ǧ^��Bit Pattern
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa GROUP_OSWrap_EVENTFLG_WAI
 */
#define pwai_flg(a,b,c,d)		twai_flg(a,b,c,d,0)	//poll wait

/**@brief Wait for Eventflag
 *
 * @param a ���Ǧ^��Bit Pattern���ܼƫ���
 * @param b �����ݪ�Event Flag ID
 * @param c �����ݪ�Bit Pattern
 * @param d Wait Event Flag Mode
 *
 * @retval a �Ǧ^��Bit Pattern
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa GROUP_OSWrap_EVENTFLG_WAI
 */
#define wai_flg(a,b,c,d)		twai_flg(a,b,c,d,((DWORD) 0xFFFFFFFF)) //infinite wait
/**@} GROUP_OSWRAP_FLG *********************************************************************/


/**********************************************************************************************//**
 * @defgroup GROUP_OSWRAP_MBX Mailbox
 * @{
 * @brief Mailbox
 *
 *************************************************************************************************/
/**@brief Create Mailbox
 *
 * @param ppid ID of newly create object to be stored
 * @param bufcnt Buffer Message Count
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 */
int cre_mbx(void** ppid, int bufcnt);

/**@brief Delete Mailbox
 *
 * @param ptr ���R����Mailbox ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_mbx
 */
int del_mbx(void* ptr);

/**@brief Send Message to Mailbox
 *
 * @param ptr ���H�X��Mailbox ID
 * @param pk_msg Start Address of Message Packet
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_mbx
 */
int snd_msg(void* ptr, void* pk_msg);

/**@brief Receive Message from Mailbox with Timeout
 * wait for the limited time
 *
 * @param ptr ��������Mailbox ID
 * @param ppk_msg �������T�����Ŷ����_�l��}
 * @param wait ���ݮɶ�(ms)
 *
 * @retval ppk_msg �����쪺�T��
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_mbx
 */
int trcv_msg(void* ptr, void** ppk_msg, DWORD wait);

/**@brief Poll and Receive Message from Mailbox
 * glance and no wait
 *
 * @param a ��������Mailbox ID
 * @param b �������T�����Ŷ����_�l��}
 *
 * @retval b �����쪺�T��
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_mbx
 */
#define prcv_msg(a,b)			trcv_msg(a,b,0)

/**@brief Receive Message from Mailbox
 * wait for infinite
 *
 * @param a ��������Mailbox ID
 * @param b �������T�����Ŷ����_�l��}
 *
 * @retval b �����쪺�T��
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_mbx
 */
#define rcv_msg(a,b)			trcv_msg(a,b,((DWORD) 0xFFFFFFFF))
/**@} GROUP_OSWRAP_MBX *********************************************************************/


/**********************************************************************************************//**
 * @defgroup GROUP_OSWRAP_SEM Semaphore
 * @{
 * @brief Semaphore
 * �h��task���ݦh��signal
 *************************************************************************************************/
/**@brief Create Semaphore
 *
 * @param ppid ID of newly create object to be stored
 * @param isemcnt Initial Semaphore Count
 * @param maxsem Maximum Semaphore Count
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 */
int cre_sem(void** ppid, int isemcnt, int maxsem);

/**@brief Delete Semaphore
 *
 * @param ptr ���R����Semaphore ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_sem
 */
int del_sem(void* ptr);

/**@brief Signal Semaphore
 *
 * @param ptr �����oSemaphore�귽��ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_sem
 */
int sig_sem(void* ptr);

/**@brief Wait on Semaphore with Timeout
 *
 * @param ptr �����ݪ�Semaphore ID
 * @param wait ���ݮɶ�(ms)
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_sem
 */
int twai_sem(void* ptr, DWORD wait);

/**@brief Poll and Request Semaphore
 *
 * @param a �����ݪ�Semaphore ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 */
#define pwai_sem(a)				twai_sem(a,0)

/**@brief Wait on Semaphore
 *
 * @param a �����ݪ�Semaphore ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 */
#define wai_sem(a)				twai_sem(a,((DWORD) 0xFFFFFFFF))
/**@} GROUP_OSWRAP_SEM *********************************************************************/


/**********************************************************************************************//**
 * @defgroup GROUP_OSWRAP_MTX Mutex
 * @{
 * @brief Mutex
 * �h��task����1��signal, like RS-232
 *************************************************************************************************/
/**@brief Create Mutex
 *
 * @param ppid ID of newly create object to be stored
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 */
int cre_mtx(void** ppid);

/**@brief Delete Mutex
 *
 * @param ptr ���R����Mutex ID
 *
 * @return
 * 	- 0: �o�Ϳ��~�A errno �������~�X @ref GROUP_OSWRAP_ERRCODE
 * 	- !=0  �s�Ыت� Mutex ID
 */
int del_mtx(void* ptr);

/**@brief Create Mutex
 *
 * @param ptr ������Mutex ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_mtx, tget_mtx, pget_mtx, get_mtx
 */
int put_mtx(void* ptr);	//get�Χ�����nput

/**@brief Get Mutex With Timeout
 *
 * @param ptr �����o�ϥ��v��Mutex ID
 * @param wait ���ݮɶ�(ms)
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_mtx, pget_mtx, get_mtx, put_mtx
 */
int tget_mtx(void* ptr, DWORD wait);

/**@brief Poll and Get Mutex
 *
 * @param a �����o�ϥ��v��Mutex ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_mtx, tget_mtx, get_mtx, put_mtx
 */
#define pget_mtx(a)				tget_mtx(a,0)

/**@brief Get Mutex
 *
 * @param a �����o�ϥ��v��Mutex ID
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa cre_mtx, tget_mtx, pget_mtx, put_mtx
 */
#define get_mtx(a)				tget_mtx(a,((DWORD)0xFFFFFFFF))
/**@} GROUP_OSWRAP_MTX *********************************************************************/


/**********************************************************************************************//**
 * @defgroup GROUP_OSWRAP_MPF Memory pool Fixed
 * @{
 * @brief Memory pool Fixed
 *************************************************************************************************/
/**@brief Create memory pool fixed
 *
 * @param ppid ID of newly create object to be stored
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 * - OSWRAP_PTR_ERROR
 * - OSWRAP_CALLER_ERROR
 */
int cre_mpf(void** ppid, int mpfcnt, int blfsz);

/**@brief Delete memory pool fixed
 *
 * @param pid ID of object to be deleted
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 * - OSWRAP_PTR_ERROR
 * - OSWRAP_CALLER_ERROR
 */
int del_mpf(void* pid);

/**@brief Get a block from pool object
 *
 * @param pid ID of object to be operate
 * @param pblf pointer of a pointer variable to receive the block address
 * @param wait ���ݮɶ�(ms)
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 * - OSWRAP_NOT_AVAILABLE
 * - OSWRAP_TIME_OUT
 * - OSWRAP_WAIT_ABORTED
 * - OSWRAP_PTR_ERROR
 * - OSWRAP_NOT_DONE
 */
int tget_blf(void* pid, void** pblf, DWORD wait);

/**@brief Release a block
 *
 * @param pblf ID of object to be operate
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 * - OSWRAP_PTR_ERROR
 */
int yl_rel_blf(void* pblf);

/**@brief Poll and get a block from pool object (no wait)
 *
 * @param pid ID of object to be operate
 * @param pblf pointer of a pointer variable to receive the block address
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 * - OSWRAP_NOT_AVAILABLE
 * - OSWRAP_TIME_OUT
 * - OSWRAP_WAIT_ABORTED
 * - OSWRAP_PTR_ERROR
 * - OSWRAP_NOT_DONE
 */
#define pget_blf(a,b)			tget_blf(a,b,0)

/**@brief Get a block from pool object (wait forever)
 *
 * @param pid ID of object to be operate
 * @param pblf pointer of a pointer variable to receive the block address
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 * - OSWRAP_NOT_AVAILABLE
 * - OSWRAP_WAIT_ABORTED
 * - OSWRAP_PTR_ERROR
 * - OSWRAP_NOT_DONE
 */
#define get_blf(a,b)			tget_blf(a,b,((DWORD)0xFFFFFFFF))


/**@} GROUP_OSWRAP_MPF *********************************************************************/



/**********************************************************************************************//**
 * @defgroup GROUP_OSWRAP_TIM Timer
 * @{
 * @brief Timer
 *
 *************************************************************************************************/
//*************************************************************************
//               CLOCK CONSTANTS                                          *
//*************************************************************************
/**********************************************************************************************//**
 * @defgroup GROUP_OSWRAP_TCY Clock Constants
 * @{
 * @brief Clock Constants
 * @sa act_cyc
 *************************************************************************************************/
#define  TCY_OFF                            0	///< ����cyclic handler�B���z�|�����p�ƾ����Ѽ�			// TX_NO_ACTIVATE
#define  TCY_ON                             1	///< �ҥ�cyclic handler�����z�|�����p�ƾ����Ѽ�			// TX_AUTO_ACTIVATE
#define  TCY_INI                            2	///< ��l��cyclic handler�������p�ƾ�			// reset count on activation
/**@} GROUP_OSWRAP_TCY *********************************************************************/
//cyclic clock
/**@brief Define Cyclic Handler
 *
 * cyclic handler�O�@�ӿW�ߪ�task handler, ���H�T�w���g�����_�`����
 *
 * @param ppid ID of newly create object to be stored
 * @param cychdr Cyclic Handler Address
 * @param cyctim Cycle Time
 * @param cycact Cyclic Handler Attributes
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 */
int def_cyc(void** ppid, FUNCENTRY cychdr, DWORD cyctim, DWORD cycact);	//time:ms

/**@brief Delete Cyclic Handler
 *
 * @param ptr ���R����Cyclic Handler Number
 *
 * @return
 * 	- 0: �o�Ϳ��~�A errno �������~�X @ref GROUP_OSWRAP_ERRCODE
 * 	- !=0  �s�Ыت� Cyclic ID
 */
int del_cyc(void* ptr);

/**@brief Activate Cyclic Handler
 *
 * @param ptr ���ҥΪ�Cyclic Handler Number
 * @param cycact Cyclic Handler Activation, defined as:@ref GROUP_OSWRAP_TCY
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa def_cyc
 */
int act_cyc(void *ptr, DWORD cycact);

/**@brief Change Cyclic Handler Time
 *
 * @param ptr ������cycle time��Cyclic Handler Number
 * @param cyctim �����ܤ�cycle time
 *
 * @return @ref GROUP_OSWRAP_ERRCODE
 *
 * @sa def_cyc
 */
int chg_cyc(void *ptr, DWORD cyctim);	//change cyc time

/**@brief Get System Clock
 *
 * @return System Clock Ticks
 */
DWORD get_tim(void);

/**@} GROUP_OSWRAP_TIM *********************************************************************/


///@cond INTERNAL_USE
#define yl_irqMaskAll(void) ({ \
register int oldpsr; \
	__asm__ __volatile__ ( \
		"mrs  %0, CPSR\n" \
		"orr  r0, %0, #0xc0\n" \
		"msr  CPSR_c, r0\n" \
	: "=r" (oldpsr) \
	: /* no inputs */ \
	: "r0"); \
	oldpsr; \
})


#define yl_irqRestoreAll(oldpsr)  \
	__asm__ __volatile__ ( \
		"msr CPSR_c, %0\n" \
	: /* no outputs */ \
	: "r" (oldpsr) \
	: "cc");	/* PSR's condition code is changed, so "cc" to notify compiler */
///@endcond



#define YL_PSR_SAVE_AREA	register int __yl_psr_save			///< �ŧi PSR �Ȧs�ܼơA�ϥ� XDISABLE �����ϥΦ��ŧi
#define XDISABLE			__yl_psr_save = yl_irqMaskAll()		///< ���� CPU ���_�\��A�N CPU ���_�B�n���A�s�� YL_PSR_SAVE_AREA
#define XRESTORE			yl_irqRestoreAll(__yl_psr_save)		///< ��_ CPU ���_�\��A�H YL_PSR_SAVE_AREA ���e��_ CPU ���_�B�n���A




/**@} GROUP_OSWRAP *********************************************************************/


#endif /* YL_OSWRAP_H_ */
