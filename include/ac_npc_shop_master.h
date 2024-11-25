#ifndef AC_NPC_SHOP_MASTER_H
#define AC_NPC_SHOP_MASTER_H

#include "types.h"
#include "m_actor.h"
#include "ac_npc.h"

#define aNSC_MAX_TICKETS 255

typedef struct npc_shop_master_s NPC_SHOP_MASTER_ACTOR;

struct npc_shop_master_s {
    /* 0x000 */ NPC_ACTOR npc_class;
    /* 0x994 */ int _994;
    /* 0x998 */ int _998;
    /* 0x99C */ int _99C;
    /* 0x9A0 */ int _9A0;
    /* 0x9A4 */ int _9A4;
    /* 0x9A8 */ int _9A8;
    /* 0x9AC */ int _9AC;
    /* 0x9B0 */ int counter;
    /* 0x9B4 */ int daily_speak_flag;
};

enum buy_result {
    aNSC_RES_0,
    aNSC_RES_1,
    aNSC_RES_NO_SUNDAY_TURNIPS,
    aNSC_RES_REFUSE_QUEST_COND,
    aNSC_RES_TAKE_OFF_HANDS,
    aNSC_RES_5,
    aNSC_RES_6,
    aNSC_RES_7,

    aNSC_RES_NUM
};

#ifdef __cplusplus
extern "C" {
#endif

extern ACTOR_PROFILE Npc_Shop_Master_Profile;

#ifdef __cplusplus
}
#endif

extern void aNSC_set_buy_sum_str(mActor_name_t itm, u32 quantity);
int aNSC_check_buy_item_sub(u32 *p1, mActor_name_t itm_name);
int aNSC_check_buy_paper(u32 *p1, mActor_name_t itm_name);

#endif

