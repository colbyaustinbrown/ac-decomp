#ifndef M_MAILBOX_OVL_H
#define M_MAILBOX_OVL_H

#include "types.h"
#include "m_mailbox_ovl_h.h"
#include "m_submenu_ovl.h"
#include "m_submenu.h"

#ifdef __cplusplus
extern "C" {
#endif

#define mMB_MAIL_COUNT HOME_MAILBOX_SIZE

typedef int (*mMB_GET_LAST_MAIL_IDX_PROC)(void);

struct mailbox_ovl_s {
    u8 open_flag;
    u8 _01;
    u8 display_flag;
    u8 _03;
    int _04;
    u16 mark_bitfield;
    int mark_flag;
    mMB_GET_LAST_MAIL_IDX_PROC get_last_mail_idx_proc;
};

extern void mMB_mailbox_ovl_set_proc(Submenu* submenu);
extern int mMB_get_last_mail_idx();
extern void mMB_mailbox_ovl_move(Submenu* submenu);
extern void mMB_mailbox_ovl_draw(Submenu* submenu, GAME*);
extern void mMB_set_dl(Submenu* submenu, GAME* game, mSM_MenuInfo_c* menu_info);
extern void mMB_set_frame_dl(GRAPH* graph, mSM_MenuInfo_c* menu_info, f32 x, f32 y, mMB_Ovl_c* ovl);

#ifdef __cplusplus
}
#endif

#endif
