#include "m_mailbox_ovl.h"

#include "libultra/libultra.h"
#include "m_lib.h"
#include "m_submenu.h"
#include "m_submenu_ovl.h"
#include "m_home_h.h"
#include "m_common_data.h"
#include "m_hand_ovl.h"
#include "m_tag_ovl.h"

void mMB_mailbox_ovl_draw(Submenu* submenu, GAME* game) {
    mSM_MenuInfo_c* menu_info = &submenu->overlay->menu_info[mSM_OVL_MAILBOX];

    (*menu_info->pre_draw_func)(submenu, game);
    mMB_set_dl(submenu, game, menu_info);
}

void mMB_move_Play(Submenu* submenu, mSM_MenuInfo_c* menu_info) {

    int last_mail_idx;

    if (menu_info->open_flag == FALSE) {
        last_mail_idx = mMB_get_last_mail_idx();

        submenu->overlay->tag_ovl->chg_tag_func_proc(submenu, 5, 0, last_mail_idx, 0.0f, 1.0f);

        menu_info->open_flag = TRUE;
    }

    submenu->overlay->menu_control.tag_move_func(submenu, menu_info);
}

void mMB_mailbox_ovl_set_proc(Submenu* submenu) {
    Submenu_Overlay_c* overlay = submenu->overlay;
    
    overlay->menu_control.menu_move_func = mMB_mailbox_ovl_move;
    overlay->menu_control.menu_draw_func = mMB_mailbox_ovl_draw;

    if (overlay->hand_ovl != NULL && overlay->menu_info[mSM_OVL_MAILBOX].next_proc_status != mSM_OVL_PROC_END) {
        submenu->overlay->hand_ovl->set_hand_func(submenu);
    }
}

void mMB_move_Move(Submenu* submenu, mSM_MenuInfo_c* menu) {
    (*submenu->overlay->move_Move_proc)(submenu, menu);
}

void mMB_mailbox_ovl_data_init(Submenu* submenu) {
    mem_clear((u8 *)submenu->overlay->mailbox_ovl, sizeof(mMB_Ovl_c), 0);
}

void mMB_mailbox_ovl_init(Submenu* submenu) {
    Submenu_Overlay_c* overlay;
    mMB_Ovl_c* mailbox;
    
    overlay = submenu->overlay;
    mailbox = overlay->mailbox_ovl;

    overlay->menu_control.animation_flag = FALSE;

    mailbox->mark_bitfield = 0;
    mailbox->mark_flag = FALSE;

    overlay->menu_info[mSM_OVL_MAILBOX].proc_status = mSM_OVL_PROC_MOVE;
    overlay->menu_info[mSM_OVL_MAILBOX].move_drt = mSM_MOVE_IN_LEFT;
    overlay->menu_info[mSM_OVL_MAILBOX].next_proc_status = mSM_OVL_PROC_PLAY;

    overlay->mailbox_ovl->open_flag = TRUE;
}

int mMB_get_last_mail_idx() {
    Mail_c *mailbox;
    int idx;

    idx = HOME_MAILBOX_SIZE;
    mailbox = &Now_Home->mailbox[HOME_MAILBOX_SIZE];

    do {
        idx -= 1;
        mailbox -= 1;
        if (mMl_check_not_used_mail(mailbox) == 0) {
            break;
        }
    } while (idx != 0);
    return idx;
}

void mMB_mailbox_ovl_construct(Submenu* submenu) {
    Submenu_Overlay_c* overlay;

    overlay = submenu->overlay;
    if (overlay->mailbox_ovl == NULL) {
        mMB_mailbox_ovl_data_init(submenu);
        submenu->overlay->mailbox_ovl->get_last_mail_idx_proc = (void *)mMB_get_last_mail_idx;
    }
    mMB_mailbox_ovl_init(submenu);
    mMB_mailbox_ovl_set_proc(submenu);
}

void mMB_mailbox_ovl_destruct(Submenu* submenu) {
    submenu->overlay->mailbox_ovl = NULL; 
}
