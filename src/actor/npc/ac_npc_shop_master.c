#include "ac_npc_shop_master.h"

#include "m_shop.h"
#include "m_common_data.h"
#include "m_font.h"
#include "m_msg.h"
#include "m_private.h"
#include "m_actor_type.h"
#include "m_player_lib.h"
#include "m_actor.h"
#include "m_actor_type.h"
#include "ac_uki.h"

int aNSC_check_buy_item_single(NPC_SHOP_MASTER_ACTOR *nook, Submenu *menu) {

    u32 res = aNSC_RES_0;

    mActor_name_t item = menu->item_p[0].item;

    u32 quant = 0;

    u32 cond = Now_Private->inventory.item_conditions;

    if ((cond >> (menu->item_p->slot_no * 2) & 2) != 0) {
        res = aNSC_RES_REFUSE_QUEST_COND;
    }
    else if (ITEM_NAME_GET_TYPE(item) == NAME_TYPE_ITEM1 && ITEM_NAME_GET_CAT(item) == ITEM1_CAT_KABU) {
        if (item == ITM_KABU_SPOILED) {
            res = aNSC_RES_TAKE_OFF_HANDS;
            quant = 1;
        }
        else if (Common_Get(time).rtc_time.weekday == lbRTC_SUNDAY) {
            res = aNSC_RES_NO_SUNDAY_TURNIPS;
        }
        else {
            res = aNSC_check_buy_item_sub(&quant, item);
        }
    } 
    else {
        if (mSP_ItemNo2ItemPrice(item) >> 2 == 0) {
            res = aNSC_RES_TAKE_OFF_HANDS;
            quant = 1;
        }
        else if (ITEM_IS_PAPER(item)) {
            res = aNSC_check_buy_paper(&quant, item);
        } else {
            res = aNSC_check_buy_item_sub(&quant, item);
        }
    }
    
    nook->counter = quant;
    return res;
}

int aNSC_check_buy_item_plural(NPC_SHOP_MASTER_ACTOR *nook, Submenu *menu) {
    Submenu_Item_c *selected = menu->item_p;

    int res = aNSC_RES_7;

    u32 num_selected = menu->selected_item_num;
    nook->counter = num_selected;

    while (num_selected) {
        if (selected->item >> 0xc == 2) {
            if ((selected->item >> 8 & 0xf) == 0xf) {
                res = aNSC_RES_6;
                break;
            }
        }
        if (mSP_ItemNo2ItemPrice(selected->item) >> 2 != 0) {
            res = aNSC_RES_6;
            break;
        }

        selected += 1;
        num_selected -= 1;
    }

    return res;
}

int aNSC_check_buy_item(NPC_SHOP_MASTER_ACTOR *sm, Submenu *menu) {
    if (menu->selected_item_num == 1) {
        return aNSC_check_buy_item_single(sm, menu);
    } else {
        return aNSC_check_buy_item_plural(sm, menu);
    }
}

int aNSC_check_item_with_ticket(mActor_name_t item) {
    int type;
    int cat;
    int res;

    type = ITEM_NAME_GET_TYPE(item);
    res = 0;

    if (type != NAME_TYPE_ITEM1) {
        if (type < NAME_TYPE_ITEM1) {
            if (type >= NAME_TYPE_FTR0) {
                goto DONE1;
            }
        }
        else if (type < NAME_TYPE_WARP) {
            DONE1: return TRUE;
        }
    } else {
        if (ITEM_NAME_GET_CAT(item) != ITEM1_CAT_CLOTH) {
            if (ITEM_NAME_GET_CAT(item) < ITEM1_CAT_CLOTH) {
                if (ITEM_NAME_GET_CAT(item) == ITEM1_CAT_TOOL) {
                    goto UMB;
                }
                goto DONE;
            }
            else if (ITEM_NAME_GET_CAT(item) >= ITEM1_CAT_FRUIT || ITEM_NAME_GET_CAT(item) < ITEM1_CAT_CARPET) {
                goto DONE;
            }
        }
        return TRUE;
UMB:
        if (item >= ITM_UMBRELLA_START && item <= ITM_UMBRELLA_END - 1) {
            res = TRUE;
        }
    }

DONE:
    return res;
}

void aNSC_setup_ticket_remain() {
    s32 tickets = Now_Private->inventory.lotto_ticket_mail_storage;
    u8 month = Common_Get(time).rtc_time.month;
    if (month != Now_Private->inventory.lotto_ticket_expiry_month) {
        tickets = 0;
        Now_Private->inventory.lotto_ticket_expiry_month = month;
    }
    tickets += 1;
    if (tickets > aNSC_MAX_TICKETS) {
        tickets = aNSC_MAX_TICKETS;
    }
    Now_Private->inventory.lotto_ticket_mail_storage = tickets;
}

int aNSC_check_same_month_ticket(mActor_name_t ticket) {
    int res = FALSE;

    mActor_name_t new_ticket = ticket;
    int idx = mPlib_Get_space_putin_item_forTICKET(&new_ticket);
    if (idx != -1) {
        mPr_SetPossessionItem(Now_Private, idx, new_ticket, mPr_ITEM_COND_NORMAL);
        res = TRUE;
    }
    return res;
}

int aNSC_check_money_overflow(int p1, int p2) {
    u32 r3;
    u32 bags;
    u32 bells_remaining;
    int empty_spaces;
    int res;

    r3 = 0;
    empty_spaces = mPr_GetPossessionItemSum(Now_Private, EMPTY_NO);
    bells_remaining = 99999 - (p1 + 30000);
    bags = bells_remaining / 30000;
    if (p1 >= 99999) {
        do {
            r3 += 1;
        } while (--bags);
    }

    res = FALSE;
    if (r3 <= empty_spaces + p2) {
        res = TRUE;
    }
    return res;
}

int aNSC_check_buy_item_sub(u32 *p1, mActor_name_t itm_name) {
    int i;
    int res = 0;
    mActor_name_t *item = Now_Private->inventory.pockets;
    int shift = 0;

    for (i = 0; i < 15; i++) {
        if (*item == itm_name) {
            u32 cond = Now_Private->inventory.item_conditions >> shift;
            if ((cond & mPr_ITEM_COND_QUEST) == 0 && (cond & mPr_ITEM_COND_PRESENT) == 0) {
                *p1 += 1;
            }
        }
        item += 1;
        shift += 2;
    }
    if (*p1 > 1) {
        aNSC_set_buy_sum_str(itm_name, *p1);
        res = 5;
    }
    return res;
}

int aNSC_check_buy_paper(u32 *quant, mActor_name_t itm_name) {
    int i;
    int res = 0; 
    mActor_name_t *item = Now_Private->inventory.pockets;
    int shift = 0;

    for (i = 0; i < 15; i++) {
        if (ITEM_IS_PAPER(*item)) {
            if (PAPER2TYPE(*item - ITM_PAPER_START) == PAPER2TYPE(itm_name - ITM_PAPER_START)) {
                u32 cond = Now_Private->inventory.item_conditions >> shift;
                if ((cond & mPr_ITEM_COND_QUEST) == 0 && (cond & mPr_ITEM_COND_PRESENT) == 0) {
                    *quant += PAPER2STACK(*item - ITM_PAPER_START) + 1;
                }
            }
        }
        item += 1;
        shift += 2;
    }
    if (*quant > 1) {
        aNSC_set_buy_sum_str(itm_name, *quant);
        res = 5;
    }
    return res;
}

int aNSC_get_msg_no(int p1) {
    int res = 0x82a;
    if (p1 < 0) {
        return res;
    }
    else if (p1 < 100) {
        res = p1 + 0x107b;
    }
    else if (p1 < 200) {
        res = p1 + 0x2baf;
    }
    else if (p1 < 300) {
        res = p1 + 0x2de6;
    } else {
        res = p1 + 0x3cd2;
    }
    return res;
}

void aNSC_set_pockets_name_str(int quant, int p2) {
    u8 name_str[16];
    int art;

    mIN_copy_name_str(name_str, quant);
    art = mIN_get_item_article(quant);

    mMsg_Set_item_str_art(mMsg_Get_base_window_p(), p2, name_str, 0x10, art);
}

void aNSC_set_value_str(int p1, int p2) {
    u8 value_str[16];

    mFont_UnintToString(value_str, sizeof(value_str), p1, 0x10, TRUE, FALSE, TRUE);
    mMsg_Set_free_str(mMsg_Get_base_window_p(), p2, value_str, 0x10);
}

int aNSC_money_check(u32 amount) {
    return mSP_money_check(amount);
}

void aNSC_get_sell_price(u32 amount) {
    mSP_get_sell_price(amount);
}

int aNSC_buy_item_single(int *p1, mActor_name_t item, u16 p2) {
    static mActor_name_t aNSC_exchange_itemNo[] = {ITM_MONEY_30000, EMPTY_NO};
    u32 *bells;
    int idx;
    int replace_with_bag;
    u32 bells_before;
    Private_c *priv = Now_Private;
    int res = 1;

    for (; p2 != 0; p2--) {
        bells_before = *p1;
        if (bells_before >= 99999) {
            *p1 -= 30000;
            replace_with_bag = 0;
            res = 0;
        } else {
            replace_with_bag = 1;
        }
        idx = mPr_GetPossessionItemIdxWithCond(priv, item, mPr_ITEM_COND_NORMAL);

        mPr_SetPossessionItem(Now_Private, idx, aNSC_exchange_itemNo[replace_with_bag], mPr_ITEM_COND_NORMAL);
    }


    while (*bells >= 99999) {
        *bells -= 30000;
        mPr_SetFreePossessionItem(Now_Private, ITM_MONEY_30000, mPr_ITEM_COND_NORMAL);
    }

    return res;
}
