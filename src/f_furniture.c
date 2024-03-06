#include "f_furniture.h"

#include "dolphin/os/OSCache.h"
#include "m_rcp.h"
#include "m_play.h"
#include "m_common_data.h"
#include "audio.h"
#include "m_player_lib.h"
#include "ac_my_room.h"
#include "sys_matrix.h"
#include "m_malloc.h"
#include "m_debug.h"

static u16 fFTR_myhome_off_pal_table[][16] = {
    // clang-format off
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xFD68, 0xF907, 0xECC6, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xFE6B, 0xFA09, 0xF5C8, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xFF6C, 0xFF2A, 0xFEE8, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xCF68, 0xBF04, 0xAEC3, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0x9EC7, 0x8E65, 0x8A05, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xB2DF, 0xA67E, 0x9E1D, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xA9FF, 0xA19E, 0x9D5B, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xD59F, 0xC95E, 0xBD1C, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xFE54, 0xFA11, 0xF5CF, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xA508, 0x9CC6, 0x9484, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xFFFF, 0xFBDC, 0xF39A, 0x8000, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xCD26, 0xC0C5, 0xB4A5, 0x8000, 0x0622 },
    // clang-format on
};

static u16 fFTR_myhome_on_pal_table[][16] = {
    // clang-format off
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xFD68, 0xF907, 0xECC6, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xFE6B, 0xFA09, 0xF5C8, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xFF6C, 0xFF2A, 0xFEE8, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xCF68, 0xBF04, 0xAEC3, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0x9EC7, 0x8E65, 0x8A05, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xB2DF, 0xA67E, 0x9E1D, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xA9FF, 0xA19E, 0x9D5B, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xD59F, 0xC95E, 0xBD1C, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xFE54, 0xFA11, 0xF5CF, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xA508, 0x9CC6, 0x9484, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xFFFF, 0xFBDC, 0xF39A, 0xFFF2, 0x0622 },
    { 0x8EC9, 0x8EAE, 0x81E9, 0xFFFC, 0xEF79, 0xD274, 0xB58F, 0xFE8C, 0xE988, 0xC8E6, 0xB085, 0xCD26, 0xC0C5, 0xB4A5, 0xFFF2, 0x0622 },
    // clang-format on
};

static void fFTR_MorphHousePalette(u16* dst_pal_p, u16* off_pal_p, u16* on_pal_p, FTR_ACTOR* ftr_actor) {
    f32 target;
    int i;

    if (ftr_actor->switch_bit == TRUE) {
        target = 1.0f;
    } else {
        target = 0.0f;
    }

    if (ftr_actor->dynamic_work_f[0] > target) {
        ftr_actor->dynamic_work_f[0] -= 0.1f;

        if (ftr_actor->dynamic_work_f[0] < target) {
            ftr_actor->dynamic_work_f[0] = target;
        }
    } else if (ftr_actor->dynamic_work_f[0] < target) {
        ftr_actor->dynamic_work_f[0] += 0.1f;

        if (ftr_actor->dynamic_work_f[0] > target) {
            ftr_actor->dynamic_work_f[0] = target;
        }
    }

    for (i = 0; i < 16; i++) {
        f32 work;

        // We assume that the palette will be RGB5A3 with no alpha component so:
        // [1][rrrrr][ggggg][bbbbb]
        dst_pal_p[i] = 0;

        // Red
        work = (f32)((off_pal_p[i] >> 10) & 0x1F);
        dst_pal_p[i] |=
            ((u16)(work + ftr_actor->dynamic_work_f[0] * ((f32)((off_pal_p[i] >> 10) & 0x1F) - work)) & 0x1F) << 10;

        // Green
        work = (f32)((off_pal_p[i] >> 5) & 0x1F);
        dst_pal_p[i] |= ((u16)(work + ftr_actor->dynamic_work_f[0] * ((f32)((off_pal_p[i] >> 5) & 0x1F) - work)) & 0x1F)
                        << 5;

        // Blue
        work = (f32)((off_pal_p[i] >> 0) & 0x1F);
        dst_pal_p[i] |= ((u16)(work + ftr_actor->dynamic_work_f[0] * ((f32)((off_pal_p[i] >> 0) & 0x1F) - work)) & 0x1F)
                        << 0;

        // Alpha-bit (not sure why they bother even masking this out)
        dst_pal_p[i] |= (u16)(off_pal_p[i] & 0x8000);
    }

    DCStoreRangeNoSync(dst_pal_p, 16 * sizeof(u16));
}

static void fFTR_MorphHousepaletteCt(u16* dst_pal_p, u16* off_pal_p, u16* on_pal_p, FTR_ACTOR* ftr_actor) {
    if (ftr_actor->switch_bit == TRUE) {
        ftr_actor->dynamic_work_f[0] = 1.0f;
    } else {
        ftr_actor->dynamic_work_f[0] = 0.0f;
    }

    fFTR_MorphHousePalette(dst_pal_p, off_pal_p, on_pal_p, ftr_actor);
}

static Gfx* fFTR_GetTwoTileGfx(int width0, int height0, int scroll_x0, int scroll_y0, int width1, int height1,
                               int scroll_x1, int scroll_y1, GAME_PLAY* play, FTR_ACTOR* ftr_actor, int scroll_ofs) {
    u32 ctr_ofs;
    int ofs;

    if (ftr_actor->ctr_type) {
        ctr_ofs = play->game_frame;
    } else {
        ctr_ofs = play->game.frame_counter;
    }

    ofs = scroll_ofs + ctr_ofs;
    return two_tex_scroll_dolphin(play->game.graph, 0, -(scroll_x0 * ofs), -(scroll_y0 * ofs), width0, height0, 1,
                                  -(scroll_x1 * ofs), -(scroll_y1 * ofs), width1, height1);
}

#define aFTR_CAN_PLAY_SE(ftr_actor)                                                \
    (ftr_actor->state != aFTR_STATE_BIRTH && ftr_actor->state != aFTR_STATE_BYE && \
     ftr_actor->state != aFTR_STATE_DEATH && ftr_actor->state != aFTR_STATE_BIRTH_WAIT)

/**
 * How to add a completed furniture actor:
 *   1. ensure the furniture actor TU matches on decomp.me
 *   2. uncomment the #include
 */

#include "../src/ftr/ac_ari_isu01.c"
#include "../src/ftr/ac_ari_kitchen01.c"
#include "../src/ftr/ac_ari_reizou01.c"
#include "../src/ftr/ac_ari_table01.c"
#include "../src/ftr/ac_din_amber.c"
#include "../src/ftr/ac_din_ammonite.c"
#include "../src/ftr/ac_din_bront_body.c"
#include "../src/ftr/ac_din_bront_dummy.c"
#include "../src/ftr/ac_din_bront_head.c"
#include "../src/ftr/ac_din_bront_tail.c"
#include "../src/ftr/ac_din_dummy.c"
#include "../src/ftr/ac_din_egg.c"
#include "../src/ftr/ac_din_hutaba_body.c"
#include "../src/ftr/ac_din_hutaba_dummy.c"
#include "../src/ftr/ac_din_hutaba_head.c"
#include "../src/ftr/ac_din_hutaba_neck.c"
#include "../src/ftr/ac_din_mammoth_body.c"
#include "../src/ftr/ac_din_mammoth_dummy.c"
#include "../src/ftr/ac_din_mammoth_head.c"
#include "../src/ftr/ac_din_ptera_dummy.c"
#include "../src/ftr/ac_din_ptera_Lwing.c"
#include "../src/ftr/ac_din_ptera_Rwing.c"
#include "../src/ftr/ac_din_ptera_head.c"
#include "../src/ftr/ac_din_stego_body.c"
#include "../src/ftr/ac_din_stego_dummyA.c"
#include "../src/ftr/ac_din_stego_dummyB.c"
#include "../src/ftr/ac_din_stego_head.c"
#include "../src/ftr/ac_din_stego_tail.c"
#include "../src/ftr/ac_din_stump.c"
#include "../src/ftr/ac_din_trex_body.c"
#include "../src/ftr/ac_din_trex_dummy.c"
#include "../src/ftr/ac_din_trex_head.c"
#include "../src/ftr/ac_din_trex_tail.c"
#include "../src/ftr/ac_din_trikera_body.c"
#include "../src/ftr/ac_din_trikera_dummy.c"
#include "../src/ftr/ac_din_trikera_head.c"
#include "../src/ftr/ac_din_trikera_tail.c"
#include "../src/ftr/ac_din_trilobite.c"
#include "../src/ftr/ac_famicom_common.c"
#include "../src/ftr/ac_fmanekin.c"
#include "../src/ftr/ac_fumbrella.c"
#include "../src/ftr/ac_hnw_common.c"
#include "../src/ftr/ac_ike_art_ang.c"
#include "../src/ftr/ac_ike_art_fel.c"
#include "../src/ftr/ac_ike_island_hako01.c"
#include "../src/ftr/ac_ike_island_sensui01.c"
#include "../src/ftr/ac_ike_island_uku01.c"
#include "../src/ftr/ac_ike_jny_afmen01.c"
#include "../src/ftr/ac_ike_jny_botle01.c"
#include "../src/ftr/ac_ike_jny_gate01.c"
#include "../src/ftr/ac_ike_jny_gojyu01.c"
#include "../src/ftr/ac_ike_jny_hariko01.c"
#include "../src/ftr/ac_ike_jny_houi01.c"
#include "../src/ftr/ac_ike_jny_kibori01.c"
#include "../src/ftr/ac_ike_jny_makada01.c"
#include "../src/ftr/ac_ike_jny_moai01.c"
#include "../src/ftr/ac_ike_jny_ningyo01.c"
#include "../src/ftr/ac_ike_jny_pisa01.c"
#include "../src/ftr/ac_ike_jny_rosia01.c"
#include "../src/ftr/ac_ike_jny_sirser01.c"
#include "../src/ftr/ac_ike_jny_sirser201.c"
#include "../src/ftr/ac_ike_jny_syon01.c"
#include "../src/ftr/ac_ike_jny_tower01.c"
#include "../src/ftr/ac_ike_jny_truth01.c"
#include "../src/ftr/ac_ike_jpn_tansu01.c"
// #include "../src/ftr/ac_ike_kama_danro01.c"
// #include "../src/ftr/ac_ike_k_count01.c"
#include "../src/ftr/ac_ike_k_iveboy01.c"
#include "../src/ftr/ac_ike_k_kid01.c"
#include "../src/ftr/ac_ike_k_kid02.c"
#include "../src/ftr/ac_ike_k_mame01.c"
#include "../src/ftr/ac_ike_k_sum01.c"
#include "../src/ftr/ac_ike_k_otome01.c"
#include "../src/ftr/ac_ike_k_sinnen01.c"
// #include "../src/ftr/ac_ike_k_tanabata01.c"
#include "../src/ftr/ac_ike_k_turis01.c"
#include "../src/ftr/ac_ike_nikki_fan1.c"
#include "../src/ftr/ac_ike_nikki_fan2.c"
#include "../src/ftr/ac_ike_nikki_fan3.c"
#include "../src/ftr/ac_ike_nikki_fan4.c"
#include "../src/ftr/ac_ike_nikki_fan5.c"
#include "../src/ftr/ac_ike_nikki_wafu1.c"
// #include "../src/ftr/ac_ike_pst_pig01.c"
#include "../src/ftr/ac_ike_pst_tesyu01.c"
// #include "../src/ftr/ac_kob_disksystem8.c"
#include "../src/ftr/ac_kob_getabako1.c"
#include "../src/ftr/ac_kob_getabako2.c"
#include "../src/ftr/ac_kob_jimudesk.c"
#include "../src/ftr/ac_kob_jimuisu.c"
// #include "../src/ftr/ac_kob_locker1.c"
// #include "../src/ftr/ac_kob_mastersword.c"
// #include "../src/ftr/ac_kob_ncube.c"
#include "../src/ftr/ac_kob_pipeisu.c"
#include "../src/ftr/ac_kob_rika_desk.c"
#include "../src/ftr/ac_kob_tobibako.c"
// #include "../src/ftr/ac_kon_ameclock.c"
// #include "../src/ftr/ac_kon_atqclock.c"
// #include "../src/ftr/ac_kon_blueclock.c"
// #include "../src/ftr/ac_kon_cracker.c"
#include "../src/ftr/ac_kon_gomi03.c"
// #include "../src/ftr/ac_kon_gomi04.c"
// #include "../src/ftr/ac_kon_grclock.c"
#include "../src/ftr/ac_kon_isi01.c"
#include "../src/ftr/ac_kon_isi02.c"
#include "../src/ftr/ac_kon_isi03.c"
#include "../src/ftr/ac_kon_isi04.c"
#include "../src/ftr/ac_kon_isi05.c"
#include "../src/ftr/ac_kon_isi06.c"
// #include "../src/ftr/ac_kon_jihanki02.c"
#include "../src/ftr/ac_kon_jihanki03.c"
// #include "../src/ftr/ac_kon_mimiclock.c"
// #include "../src/ftr/ac_kon_musya.c"
#include "../src/ftr/ac_kon_pound.c"
// #include "../src/ftr/ac_kon_redclock.c"
// #include "../src/ftr/ac_kon_sisiodosi.c"
#include "../src/ftr/ac_kon_snowbed.c"
// #include "../src/ftr/ac_kon_snowbox.c"
// #include "../src/ftr/ac_kon_snowclock.c"
#include "../src/ftr/ac_kon_snowfreezer.c"
#include "../src/ftr/ac_kon_snowsofa.c"
#include "../src/ftr/ac_kon_snowtable.c"
// #include "../src/ftr/ac_kon_snowtansu.c"
// #include "../src/ftr/ac_kon_snowtv.c"
#include "../src/ftr/ac_kon_taiju.c"
#include "../src/ftr/ac_kon_tubo.c"
#include "../src/ftr/ac_kon_tubo2.c"
#include "../src/ftr/ac_kon_tubo3.c"
#include "../src/ftr/ac_kon_tukue.c"
// #include "../src/ftr/ac_kon_waclock.c"
#include "../src/ftr/ac_kon_xtree02.c"
// #include "../src/ftr/ac_myfmanekin.c"
// #include "../src/ftr/ac_myfumbrella.c"
#include "../src/ftr/ac_nog_amenbo.c"
#include "../src/ftr/ac_nog_ari.c"
// #include "../src/ftr/ac_nog_balloon_common.c"
#include "../src/ftr/ac_nog_beachbed.c"
// #include "../src/ftr/ac_nog_beachtable.c"
#include "../src/ftr/ac_nog_bishopB.c"
#include "../src/ftr/ac_nog_bishopW.c"
#include "../src/ftr/ac_nog_dango.c"
#include "../src/ftr/ac_nog_collegenote.c"
#include "../src/ftr/ac_nog_dump.c"
#include "../src/ftr/ac_nog_earth.c"
// #include "../src/ftr/ac_nog_fan01.c"
#include "../src/ftr/ac_nog_flat.c"
#include "../src/ftr/ac_nog_harddiary.c"
#include "../src/ftr/ac_nog_isidai.c"
// #include "../src/ftr/ac_nog_ka.c"
// #include "../src/ftr/ac_nog_kaeru.c"
// #include "../src/ftr/ac_nog_kamakura.c"
// #include "../src/ftr/ac_nog_kera.c"
#include "../src/ftr/ac_nog_kingB.c"
#include "../src/ftr/ac_nog_kingW.c"
#include "../src/ftr/ac_nog_knightB.c"
#include "../src/ftr/ac_nog_knightW.c"
// #include "../src/ftr/ac_nog_koban.c"
#include "../src/ftr/ac_nog_kumo.c"
#include "../src/ftr/ac_nog_kurage.c"
#include "../src/ftr/ac_nog_maimai.c"
#include "../src/ftr/ac_nog_mino.c"
#include "../src/ftr/ac_nog_mikanbox.c"
#include "../src/ftr/ac_nog_medaka.c"
// #include "../src/ftr/ac_nog_museum.c"
// #include "../src/ftr/ac_nog_myhome2.c"
// #include "../src/ftr/ac_nog_myhome4.c"
// #include "../src/ftr/ac_nog_nabe.c"
// #include "../src/ftr/ac_nog_pawnB.c"
// #include "../src/ftr/ac_nog_pawnW.c"
#include "../src/ftr/ac_nog_piraluku.c"
#include "../src/ftr/ac_nog_queenB.c"
#include "../src/ftr/ac_nog_queenW.c"
#include "../src/ftr/ac_nog_rail.c"
#include "../src/ftr/ac_nog_rookB.c"
#include "../src/ftr/ac_nog_rookW.c"
#include "../src/ftr/ac_nog_schoolnote.c"
// #include "../src/ftr/ac_nog_shrine.c"
#include "../src/ftr/ac_nog_snowman.c"
// #include "../src/ftr/ac_nog_station.c"
#include "../src/ftr/ac_nog_systemnote.c"
#include "../src/ftr/ac_nog_s_tree.c"
#include "../src/ftr/ac_nog_f_tree.c"
#include "../src/ftr/ac_nog_suzuki.c"
// #include "../src/ftr/ac_nog_shop1.c"
#include "../src/ftr/ac_nog_tai.c"
// #include "../src/ftr/ac_nog_tri_audio01.c"
#include "../src/ftr/ac_nog_tri_bed01.c"
#include "../src/ftr/ac_nog_tri_chair01.c"
// #include "../src/ftr/ac_nog_tri_chest01.c"
// #include "../src/ftr/ac_nog_tri_chest02.c"
// #include "../src/ftr/ac_nog_tri_chest03.c"
// #include "../src/ftr/ac_nog_tri_clock01.c"
#include "../src/ftr/ac_nog_tri_rack01.c"
#include "../src/ftr/ac_nog_tri_sofa01.c"
#include "../src/ftr/ac_nog_tri_table01.c"
#include "../src/ftr/ac_nog_tudurinote.c"
#include "../src/ftr/ac_nog_xtree.c"
// #include "../src/ftr/ac_nog_uranai.c"
// #include "../src/ftr/ac_nog_yamishop.c"
// #include "../src/ftr/ac_nog_yubin.c"
#include "../src/ftr/ac_nog_zarigani.c"
#include "../src/ftr/ac_nog_zassou.c"
#include "../src/ftr/ac_nog_w_tree.c"
// #include "../src/ftr/ac_sum_abura.c"
#include "../src/ftr/ac_sum_akiakane.c"
#include "../src/ftr/ac_sum_angel.c"
#include "../src/ftr/ac_sum_aroana.c"
#include "../src/ftr/ac_sum_art01.c"
#include "../src/ftr/ac_sum_art04.c"
#include "../src/ftr/ac_sum_art05.c"
#include "../src/ftr/ac_sum_art06.c"
#include "../src/ftr/ac_ike_art_sya.c"
#include "../src/ftr/ac_sum_art08.c"
#include "../src/ftr/ac_sum_art09.c"
#include "../src/ftr/ac_sum_art10.c"
#include "../src/ftr/ac_sum_art11.c"
#include "../src/ftr/ac_sum_art12.c"
#include "../src/ftr/ac_sum_art13.c"
#include "../src/ftr/ac_sum_art14.c"
#include "../src/ftr/ac_sum_art15.c"
#include "../src/ftr/ac_sum_asi_bed01.c"
#include "../src/ftr/ac_sum_asi_chair01.c"
#include "../src/ftr/ac_sum_asi_chair02.c"
#include "../src/ftr/ac_sum_asi_chest01.c"
// #include "../src/ftr/ac_sum_asi_chest02.c"
// #include "../src/ftr/ac_sum_asi_chest03.c"
#include "../src/ftr/ac_sum_asi_lanp01.c"
#include "../src/ftr/ac_sum_asi_screen01.c"
#include "../src/ftr/ac_sum_asi_table01.c"
#include "../src/ftr/ac_sum_asi_table02.c"
// #include "../src/ftr/ac_sum_asi_taiko.c"
#include "../src/ftr/ac_sum_ayu.c"
#include "../src/ftr/ac_sum_baketu.c"
#include "../src/ftr/ac_sum_bass.c"
// #include "../src/ftr/ac_sum_bass01.c"
#include "../src/ftr/ac_sum_bassl.c"
#include "../src/ftr/ac_sum_bassm.c"
// #include "../src/ftr/ac_sum_bdcake01.c"
#include "../src/ftr/ac_sum_billiads.c"
// #include "../src/ftr/ac_sum_biwa01.c"
#include "../src/ftr/ac_sum_bla_bed01.c"
#include "../src/ftr/ac_sum_bla_chair02.c"
// #include "../src/ftr/ac_sum_bla_chest01.c"
#include "../src/ftr/ac_sum_bla_chest02.c"
// #include "../src/ftr/ac_sum_bla_chest03.c"
#include "../src/ftr/ac_sum_bla_desk01.c"
// #include "../src/ftr/ac_sum_bla_lanp.c"
#include "../src/ftr/ac_sum_bla_sofa02.c"
#include "../src/ftr/ac_sum_bla_table01.c"
#include "../src/ftr/ac_sum_bla_table02.c"
#include "../src/ftr/ac_sum_blue_bed01.c"
#include "../src/ftr/ac_sum_blue_bench01.c"
// #include "../src/ftr/ac_sum_blue_bureau01.c"
// #include "../src/ftr/ac_sum_blue_cab01.c"
#include "../src/ftr/ac_sum_blue_chair01.c"
#include "../src/ftr/ac_sum_blue_chest01.c"
#include "../src/ftr/ac_sum_blue_chest02.c"
// #include "../src/ftr/ac_sum_blue_clk.c"
// #include "../src/ftr/ac_sum_blue_lowchest01.c"
#include "../src/ftr/ac_sum_blue_table01.c"
#include "../src/ftr/ac_sum_bon_boke.c"
#include "../src/ftr/ac_sum_bon_matu01.c"
#include "../src/ftr/ac_sum_bon_matu02.c"
#include "../src/ftr/ac_sum_bon_matu03.c"
#include "../src/ftr/ac_sum_bon_momiji.c"
#include "../src/ftr/ac_sum_bon_pira.c"
#include "../src/ftr/ac_sum_bon_sanshu.c"
#include "../src/ftr/ac_sum_bon_satuki.c"
#include "../src/ftr/ac_sum_bon_turu.c"
#include "../src/ftr/ac_sum_bon_ume.c"
#include "../src/ftr/ac_sum_bookcht01.c"
// #include "../src/ftr/ac_sum_casse01.c"
// #include "../src/ftr/ac_sum_cello01.c"
#include "../src/ftr/ac_sum_chair01.c"
// #include "../src/ftr/ac_sum_chikuon01.c"
// #include "../src/ftr/ac_sum_chikuon02.c"
#include "../src/ftr/ac_sum_classiccabinet01.c"
#include "../src/ftr/ac_sum_classicchair01.c"
// #include "../src/ftr/ac_sum_classicchest01.c"
#include "../src/ftr/ac_sum_classicchest02.c"
#include "../src/ftr/ac_sum_classictable01.c"
#include "../src/ftr/ac_sum_classicwardrope01.c"
#include "../src/ftr/ac_sum_clbed02.c"
#include "../src/ftr/ac_sum_clchair02.c"
// #include "../src/ftr/ac_sum_clchest03.c"
#include "../src/ftr/ac_sum_col_chair01.c"
#include "../src/ftr/ac_sum_col_chair02.c"
#include "../src/ftr/ac_sum_col_chair03.c"
#include "../src/ftr/ac_sum_comp01.c"
// #include "../src/ftr/ac_sum_conga01.c"
// #include "../src/ftr/ac_sum_conpo01.c"
// #include "../src/ftr/ac_sum_conpo02.c"
#include "../src/ftr/ac_sum_cont_bed01.c"
#include "../src/ftr/ac_sum_cont_cab01.c"
#include "../src/ftr/ac_sum_cont_chair01.c"
#include "../src/ftr/ac_sum_cont_chest01.c"
// #include "../src/ftr/ac_sum_cont_chest02.c"
// #include "../src/ftr/ac_sum_cont_chest03.c"
#include "../src/ftr/ac_sum_cont_sofa01.c"
#include "../src/ftr/ac_sum_cont_sofa02.c"
#include "../src/ftr/ac_sum_cont_table01.c"
#include "../src/ftr/ac_sum_cont_table02.c"
#include "../src/ftr/ac_sum_cupboard01.c"
#include "../src/ftr/ac_sum_danna.c"
#include "../src/ftr/ac_sum_demekin.c"
#include "../src/ftr/ac_sum_desk01.c"
#include "../src/ftr/ac_sum_dojyo.c"
// #include "../src/ftr/ac_sum_doll01.c"
#include "../src/ftr/ac_sum_doll02.c"
#include "../src/ftr/ac_sum_doll03.c"
#include "../src/ftr/ac_sum_doll04.c"
#include "../src/ftr/ac_sum_doll05.c"
#include "../src/ftr/ac_sum_doll06.c"
#include "../src/ftr/ac_sum_doll07.c"
#include "../src/ftr/ac_sum_doll08.c"
#include "../src/ftr/ac_sum_doll09.c"
#include "../src/ftr/ac_sum_doll10.c"
#include "../src/ftr/ac_sum_doll11.c"
#include "../src/ftr/ac_sum_donko.c"
#include "../src/ftr/ac_sum_fruitbed01.c"
#include "../src/ftr/ac_sum_fruitchair01.c"
#include "../src/ftr/ac_sum_fruitchair02.c"
#include "../src/ftr/ac_sum_fruitchair03.c"
#include "../src/ftr/ac_sum_fruitchair04.c"
// #include "../src/ftr/ac_sum_fruitchest01.c"
// #include "../src/ftr/ac_sum_fruitchest03.c"
// #include "../src/ftr/ac_sum_fruitclk.c"
#include "../src/ftr/ac_sum_fruittable01.c"
#include "../src/ftr/ac_sum_fruittable02.c"
#include "../src/ftr/ac_sum_fruittable03.c"
// #include "../src/ftr/ac_sum_fruittv01.c"
#include "../src/ftr/ac_sum_funa.c"
// #include "../src/ftr/ac_sum_genji.c"
#include "../src/ftr/ac_sum_gill.c"
#include "../src/ftr/ac_sum_ginyanma.c"
#include "../src/ftr/ac_sum_globe01.c"
#include "../src/ftr/ac_sum_golfbag01.c"
#include "../src/ftr/ac_sum_golfbag02.c"
#include "../src/ftr/ac_sum_golfbag03.c"
#include "../src/ftr/ac_sum_gomadara.c"
#include "../src/ftr/ac_sum_gre_bed01.c"
#include "../src/ftr/ac_sum_gre_chair01.c"
#include "../src/ftr/ac_sum_gre_chair02.c"
#include "../src/ftr/ac_sum_gre_chest01.c"
// #include "../src/ftr/ac_sum_gre_chest02.c"
// #include "../src/ftr/ac_sum_gre_chest03.c"
#include "../src/ftr/ac_sum_gre_counter01.c"
#include "../src/ftr/ac_sum_gre_desk01.c"
// #include "../src/ftr/ac_sum_gre_lanp01.c"
#include "../src/ftr/ac_sum_gre_table01.c"
// #include "../src/ftr/ac_sum_guitar01.c"
// #include "../src/ftr/ac_sum_guitar02.c"
// #include "../src/ftr/ac_sum_guitar03.c"
#include "../src/ftr/ac_sum_gupi.c"
#include "../src/ftr/ac_sum_hachi.c"
#include "../src/ftr/ac_sum_hal_bed01.c"
// #include "../src/ftr/ac_sum_hal_box01.c"
#include "../src/ftr/ac_sum_hal_chair01.c"
#include "../src/ftr/ac_sum_hal_chest01.c"
// #include "../src/ftr/ac_sum_hal_chest02.c"
// #include "../src/ftr/ac_sum_hal_chest03.c"
// #include "../src/ftr/ac_sum_hal_clk01.c"
// #include "../src/ftr/ac_sum_hal_lanp01.c"
// #include "../src/ftr/ac_sum_hal_mirror01.c"
#include "../src/ftr/ac_sum_hal_pkin.c"
#include "../src/ftr/ac_sum_hal_sofa01.c"
#include "../src/ftr/ac_sum_hal_table01.c"
// #include "../src/ftr/ac_sum_harp.c"
#include "../src/ftr/ac_sum_hera.c"
// #include "../src/ftr/ac_sum_higurashi.c"
#include "../src/ftr/ac_sum_hirata.c"
#include "../src/ftr/ac_sum_ito.c"
#include "../src/ftr/ac_sum_iwana.c"
// #include "../src/ftr/ac_sum_jukebox.c"
#include "../src/ftr/ac_sum_kabuto.c"
#include "../src/ftr/ac_sum_kamakiri.c"
#include "../src/ftr/ac_sum_kanabun.c"
#include "../src/ftr/ac_sum_kaseki.c"
#include "../src/ftr/ac_sum_kiageha.c"
#include "../src/ftr/ac_sum_kingyo.c"
// #include "../src/ftr/ac_sum_kirigirisu.c"
// #include "../src/ftr/ac_sum_kisha.c"
#include "../src/ftr/ac_sum_kitchair01.c"
#include "../src/ftr/ac_sum_kittable01.c"
#include "../src/ftr/ac_sum_koi.c"
#include "../src/ftr/ac_sum_kokuban.c"
// #include "../src/ftr/ac_sum_koorogi.c"
#include "../src/ftr/ac_sum_liccabed.c"
#include "../src/ftr/ac_sum_liccachair.c"
// #include "../src/ftr/ac_sum_liccachest.c"
#include "../src/ftr/ac_sum_liccakitchen.c"
#include "../src/ftr/ac_sum_liccalanp.c"
// #include "../src/ftr/ac_sum_liccalowchest.c"
#include "../src/ftr/ac_sum_liccalowtable.c"
// #include "../src/ftr/ac_sum_liccamirror.c"
// #include "../src/ftr/ac_sum_liccapiano.c"
#include "../src/ftr/ac_sum_liccasofa.c"
#include "../src/ftr/ac_sum_liccatable.c"
#include "../src/ftr/ac_sum_log_bed01.c"
#include "../src/ftr/ac_sum_log_chair01.c"
#include "../src/ftr/ac_sum_log_chair02.c"
#include "../src/ftr/ac_sum_log_chair03.c"
// #include "../src/ftr/ac_sum_log_chest01.c"
// #include "../src/ftr/ac_sum_log_chest02.c"
#include "../src/ftr/ac_sum_log_chest03.c"
// #include "../src/ftr/ac_sum_log_hatoclk.c"
#include "../src/ftr/ac_sum_log_table01.c"
#include "../src/ftr/ac_sum_log_table02.c"
// #include "../src/ftr/ac_sum_lv_stereo.c"
// #include "../src/ftr/ac_sum_matumushi.c"
// #include "../src/ftr/ac_sum_md01.c"
// #include "../src/ftr/ac_sum_mezaclock.c"
// #include "../src/ftr/ac_sum_minmin.c"
#include "../src/ftr/ac_sum_misin01.c"
#include "../src/ftr/ac_sum_miyama.c"
// #include "../src/ftr/ac_sum_mizunomi.c"
#include "../src/ftr/ac_sum_monki.c"
#include "../src/ftr/ac_sum_monshiro.c"
#include "../src/ftr/ac_sum_namazu.c"
#include "../src/ftr/ac_sum_nanahoshi.c"
#include "../src/ftr/ac_sum_nigoi.c"
#include "../src/ftr/ac_sum_niji.c"
#include "../src/ftr/ac_sum_nisiki.c"
#include "../src/ftr/ac_sum_nokogiri.c"
#include "../src/ftr/ac_sum_ohmurasaki.c"
#include "../src/ftr/ac_sum_oikawa.c"
// #include "../src/ftr/ac_sum_okiagari01.c"
#include "../src/ftr/ac_sum_okuwa.c"
// #include "../src/ftr/ac_sum_oldclk01.c"
#include "../src/ftr/ac_sum_oldsofa01.c"
#include "../src/ftr/ac_sum_oniyanma.c"
#include "../src/ftr/ac_sum_oonamazu.c"
// #include "../src/ftr/ac_sum_pet01.c"
// #include "../src/ftr/ac_sum_piano01.c"
#include "../src/ftr/ac_sum_pirania.c"
#include "../src/ftr/ac_sum_pl_aloe01.c"
#include "../src/ftr/ac_sum_pl_ananas.c"
#include "../src/ftr/ac_sum_pl_benjyami.c"
#include "../src/ftr/ac_sum_pl_caladium01.c"
#include "../src/ftr/ac_sum_pl_cocos.c"
#include "../src/ftr/ac_sum_pl_compacta.c"
#include "../src/ftr/ac_sum_pl_draca02.c"
#include "../src/ftr/ac_sum_pl_dracaena.c"
#include "../src/ftr/ac_sum_pl_gomunoki.c"
#include "../src/ftr/ac_sum_pl_hirasabo.c"
#include "../src/ftr/ac_sum_pl_kuroton.c"
#include "../src/ftr/ac_sum_pl_pakila.c"
#include "../src/ftr/ac_sum_pl_pothos.c"
#include "../src/ftr/ac_sum_pl_shuroci.c"
#include "../src/ftr/ac_sum_pl_yamayasi.c"
#include "../src/ftr/ac_sum_popchair01.c"
#include "../src/ftr/ac_sum_popchair02.c"
#include "../src/ftr/ac_sum_popchair03.c"
#include "../src/ftr/ac_sum_poptable01.c"
#include "../src/ftr/ac_sum_poptable02.c"
#include "../src/ftr/ac_sum_poptable03.c"
// #include "../src/ftr/ac_sum_radio01.c"
// #include "../src/ftr/ac_sum_radio02.c"
#include "../src/ftr/ac_sum_raigyo.c"
#include "../src/ftr/ac_sum_ratan_bed01.c"
// #include "../src/ftr/ac_sum_ratan_chest01.c"
// #include "../src/ftr/ac_sum_ratan_chest02.c"
#include "../src/ftr/ac_sum_ratan_chest03.c"
#include "../src/ftr/ac_sum_ratan_isu01.c"
#include "../src/ftr/ac_sum_ratan_isu02.c"
#include "../src/ftr/ac_sum_ratan_lanp.c"
// #include "../src/ftr/ac_sum_ratan_mirror.c"
#include "../src/ftr/ac_sum_ratan_screen.c"
#include "../src/ftr/ac_sum_ratan_table01.c"
// #include "../src/ftr/ac_sum_reco01.c"
// #include "../src/ftr/ac_sum_roboclk.c"
// #include "../src/ftr/ac_sum_roboconpo.c"
#include "../src/ftr/ac_sum_sabo01.c"
#include "../src/ftr/ac_sum_sabo02.c"
// #include "../src/ftr/ac_sum_saiconpo.c"
#include "../src/ftr/ac_sum_sake.c"
#include "../src/ftr/ac_sum_shiokara.c"
#include "../src/ftr/ac_sum_shoukaki.c"
// #include "../src/ftr/ac_sum_slot.c"
#include "../src/ftr/ac_sum_sofe01.c"
#include "../src/ftr/ac_sum_sofe02.c"
#include "../src/ftr/ac_sum_sofe03.c"
// #include "../src/ftr/ac_sum_stereo01.c"
// #include "../src/ftr/ac_sum_stereo02.c"
// #include "../src/ftr/ac_sum_stove01.c"
#include "../src/ftr/ac_sum_suberi01.c"
// #include "../src/ftr/ac_sum_suzumushi.c"
// #include "../src/ftr/ac_sum_syouryou.c"
// #include "../src/ftr/ac_sum_taiko01.c"
#include "../src/ftr/ac_sum_takkyu.c"
#include "../src/ftr/ac_sum_tamamushi.c"
#include "../src/ftr/ac_sum_tanago.c"
#include "../src/ftr/ac_sum_taru01.c"
#include "../src/ftr/ac_sum_taru02.c"
// #include "../src/ftr/ac_sum_tekin01.c"
#include "../src/ftr/ac_sum_tentou.c"
// #include "../src/ftr/ac_sum_timpani01.c"
// #include "../src/ftr/ac_sum_tonosama.c"
#include "../src/ftr/ac_sum_totemp01.c"
#include "../src/ftr/ac_sum_totemp02.c"
#include "../src/ftr/ac_sum_totemp03.c"
#include "../src/ftr/ac_sum_totemp04.c"
#include "../src/ftr/ac_sum_touro01.c"
#include "../src/ftr/ac_sum_touro02.c"
#include "../src/ftr/ac_sum_touro03.c"
#include "../src/ftr/ac_sum_touro04.c"
// #include "../src/ftr/ac_sum_tukutuku.c"
// #include "../src/ftr/ac_sum_tv01.c"
// #include "../src/ftr/ac_sum_tv02.c"
#include "../src/ftr/ac_sum_ugui.c"
#include "../src/ftr/ac_sum_unagi.c"
#include "../src/ftr/ac_sum_uwa_cup01.c"
#include "../src/ftr/ac_sum_uwa_poto01.c"
#include "../src/ftr/ac_sum_uwa_vase01.c"
#include "../src/ftr/ac_sum_uwa_vase02.c"
#include "../src/ftr/ac_sum_uwa_vase03.c"
// #include "../src/ftr/ac_sum_viola01.c"
#include "../src/ftr/ac_sum_wakasagi.c"
#include "../src/ftr/ac_sum_wc01.c"
#include "../src/ftr/ac_sum_wc02.c"
#include "../src/ftr/ac_sum_whi_bed01.c"
// #include "../src/ftr/ac_sum_whi_chest01.c"
// #include "../src/ftr/ac_sum_whi_chest02.c"
// #include "../src/ftr/ac_sum_whi_lanp.c"
// #include "../src/ftr/ac_sum_whi_mirror.c"
#include "../src/ftr/ac_sum_whi_sofa01.c"
#include "../src/ftr/ac_sum_x_bed01.c"
#include "../src/ftr/ac_sum_x_chair01.c"
// #include "../src/ftr/ac_sum_x_chest01.c"
// #include "../src/ftr/ac_sum_x_chest02.c"
#include "../src/ftr/ac_sum_x_chest03.c"
// #include "../src/ftr/ac_sum_x_clk.c"
#include "../src/ftr/ac_sum_x_lanp.c"
// #include "../src/ftr/ac_sum_x_piano.c"
#include "../src/ftr/ac_sum_x_sofa01.c"
#include "../src/ftr/ac_sum_x_table01.c"
#include "../src/ftr/ac_sum_yamame.c"
#include "../src/ftr/ac_tak_apollo1.c"
#include "../src/ftr/ac_tak_arwing.c"
#include "../src/ftr/ac_tak_asteroid1.c"
#include "../src/ftr/ac_tak_astro.c"
#include "../src/ftr/ac_tak_barber.c"
#include "../src/ftr/ac_tak_cone01.c"
#include "../src/ftr/ac_tak_cone03.c"
#include "../src/ftr/ac_tak_cube.c"
#include "../src/ftr/ac_tak_drum02.c"
#include "../src/ftr/ac_tak_eisei.c"
#include "../src/ftr/ac_tak_hole01.c"
// #include "../src/ftr/ac_tak_ice.c"
#include "../src/ftr/ac_tak_isu03.c"
// #include "../src/ftr/ac_tak_lion.c"
// #include "../src/ftr/ac_tak_megami.c"
// #include "../src/ftr/ac_tak_metro.c"
// #include "../src/ftr/ac_tak_money.c"
// #include "../src/ftr/ac_tak_moon.c"
#include "../src/ftr/ac_tak_mooncar.c"
#include "../src/ftr/ac_tak_moti.c"
#include "../src/ftr/ac_tak_neko.c"
#include "../src/ftr/ac_tak_nikki01.c"
#include "../src/ftr/ac_tak_rocket1.c"
#include "../src/ftr/ac_tak_sori01.c"
#include "../src/ftr/ac_tak_syogi.c"
#include "../src/ftr/ac_tak_shuttle.c"
#include "../src/ftr/ac_tak_snowisu.c"
#include "../src/ftr/ac_tak_snowlamp.c"
#include "../src/ftr/ac_tak_station.c"
#include "../src/ftr/ac_tak_table02.c"
// #include "../src/ftr/ac_tak_tailor.c"
#include "../src/ftr/ac_tak_tekkin.c"
#include "../src/ftr/ac_tak_tetra.c"
// #include "../src/ftr/ac_tak_toudai.c"
#include "../src/ftr/ac_tak_ufo.c"
#include "../src/ftr/ac_tak_yaji.c"
#include "../src/ftr/ac_tak_yoroi.c"
#include "../src/ftr/ac_iid_benti.c"
#include "../src/ftr/ac_iid_diary.c"
#include "../src/ftr/ac_iid_funediary.c"
#include "../src/ftr/ac_iid_mdiary.c"
#include "../src/ftr/ac_iid_newdiary.c"
#include "../src/ftr/ac_iid_ningyou.c"
#include "../src/ftr/ac_iid_snow.c"
#include "../src/ftr/ac_iid_surf.c"
#include "../src/ftr/ac_iid_yuki.c"
// #include "../src/ftr/ac_hayakawa_famicom.c"
// #include "../src/ftr/ac_radio_test.c"
// #include "../src/ftr/ac_gold_item.c"
// #include "../src/ftr/ac_utiwa.c"
// #include "../src/ftr/ac_kazaguruma.c"
// #include "../src/ftr/ac_tool.c"
#include "../src/ftr/ac_hos_deskL.c"
#include "../src/ftr/ac_hos_deskR.c"
#include "../src/ftr/ac_hos_flip.c"
#include "../src/ftr/ac_hos_mario_hata.c"
#include "../src/ftr/ac_hos_mario_kinoko.c"
#include "../src/ftr/ac_hos_piknic.c"
#include "../src/ftr/ac_hos_Tdesk.c"
#include "../src/ftr/ac_iku_bugzapper.c"
#include "../src/ftr/ac_hos_turkey_sofa.c"
#include "../src/ftr/ac_iku_busstop.c"
#include "../src/ftr/ac_iku_candy.c"
#include "../src/ftr/ac_iku_cement.c"
#include "../src/ftr/ac_iku_chair.c"
#include "../src/ftr/ac_iku_cocoa.c"
#include "../src/ftr/ac_iku_cow.c"
#include "../src/ftr/ac_iku_denko.c"
#include "../src/ftr/ac_iku_flagman.c"
#include "../src/ftr/ac_iku_flip_top.c"
#include "../src/ftr/ac_iku_gold_green.c"
#include "../src/ftr/ac_iku_gold_red.c"
#include "../src/ftr/ac_iku_ham.c"
#include "../src/ftr/ac_iku_hazardous_top.c"
#include "../src/ftr/ac_iku_ido.c"
#include "../src/ftr/ac_iku_jack.c"
#include "../src/ftr/ac_iku_jersey.c"
#include "../src/ftr/ac_iku_mario_coin.c"
#include "../src/ftr/ac_iku_mario_dokan.c"
#include "../src/ftr/ac_iku_mario_hatena.c"
#include "../src/ftr/ac_iku_mario_koura.c"
#include "../src/ftr/ac_iku_mario_renga.c"
#include "../src/ftr/ac_iku_mario_taihou.c"
#include "../src/ftr/ac_iku_mario_star.c"
#include "../src/ftr/ac_iku_orange.c"
#include "../src/ftr/ac_iku_reducespeed.c"
#include "../src/ftr/ac_iku_roller.c"
#include "../src/ftr/ac_iku_saku_a.c"
#include "../src/ftr/ac_iku_saku_b.c"
#include "../src/ftr/ac_iku_sawhousev.c"
#include "../src/ftr/ac_iku_slip.c"
#include "../src/ftr/ac_iku_tumble.c"
#include "../src/ftr/ac_iku_turkey_bed.c"
#include "../src/ftr/ac_iku_turkey_lamp.c"
#include "../src/ftr/ac_iku_turkey_table.c"
#include "../src/ftr/ac_iku_turkey_TV.c"
#include "../src/ftr/ac_iku_ukai.c"
#include "../src/ftr/ac_iku_work.c"
#include "../src/ftr/ac_sugi_alchair.c"
#include "../src/ftr/ac_sugi_barbecue.c"
#include "../src/ftr/ac_sugi_chesstable.c"
#include "../src/ftr/ac_sugi_kpool.c"
#include "../src/ftr/ac_sugi_radiatorl.c"
#include "../src/ftr/ac_sugi_torch.c"
#include "../src/ftr/ac_yaz_b_bath.c"
#include "../src/ftr/ac_yaz_b_house.c"
#include "../src/ftr/ac_yaz_candle.c"
#include "../src/ftr/ac_yaz_fish_trophy.c"
#include "../src/ftr/ac_yaz_mario_flower.c"
#include "../src/ftr/ac_yaz_telescope.c"
#include "../src/ftr/ac_yaz_tub.c"
#include "../src/ftr/ac_yaz_turkey_chair.c"
#include "../src/ftr/ac_yaz_turkey_chest.c"
#include "../src/ftr/ac_yaz_turkey_closet.c"
#include "../src/ftr/ac_yaz_rocket.c"
#include "../src/ftr/ac_yaz_wagon.c"
#include "../src/ftr/ac_yos_b_feeder.c"
#include "../src/ftr/ac_yos_cactus.c"
#include "../src/ftr/ac_yos_flamingo.c"
#include "../src/ftr/ac_yos_flamingo2.c"
#include "../src/ftr/ac_yos_gnome.c"
#include "../src/ftr/ac_yos_kflag.c"
#include "../src/ftr/ac_yos_pbstove.c"
#include "../src/ftr/ac_yos_mailbox.c"
#include "../src/ftr/ac_yos_mario.c"
#include "../src/ftr/ac_yos_luigi.c"
#include "../src/ftr/ac_yos_terrace.c"
#include "../src/ftr/ac_yos_turkey_mirror.c"
#include "../src/ftr/ac_yos_turkey_watch.c"
#include "../src/ftr/ac_yos_wheel.c"
#include "../src/ftr/ac_tak_ham1.c"
#include "../src/ftr/ac_tak_nes01.c"
#include "../src/ftr/ac_tak_noise.c"
#include "../src/ftr/ac_tak_stew.c"
#include "../src/ftr/ac_tak_tent.c"
#include "../src/ftr/ac_tak_tent_box.c"
#include "../src/ftr/ac_tak_tent_lamp.c"
#include "../src/ftr/ac_ike_fish_tro2.c"
#include "../src/ftr/ac_ike_prores_bench01.c"
#include "../src/ftr/ac_ike_prores_fense01.c"
#include "../src/ftr/ac_ike_prores_ling01.c"
#include "../src/ftr/ac_ike_prores_ling02.c"
#include "../src/ftr/ac_ike_prores_ling03.c"
#include "../src/ftr/ac_ike_prores_mat01.c"
#include "../src/ftr/ac_ike_prores_punch01.c"
#include "../src/ftr/ac_ike_prores_sandbag01.c"
#include "../src/ftr/ac_ike_prores_table01.c"
#include "../src/ftr/ac_ike_tent_bike01.c"
#include "../src/ftr/ac_ike_tent_fire01.c"
#include "../src/ftr/ac_ike_tent_fire02.c"
#include "../src/ftr/ac_ike_tent_kayak01.c"
#include "../src/ftr/ac_ike_tent_knap01.c"
#include "../src/ftr/ac_ike_tent_sleepbag01.c"
#include "../src/ftr/ac_nog_burner.c"
#include "../src/ftr/ac_nog_cornucopia.c"
#include "../src/ftr/ac_nog_flower.c"
#include "../src/ftr/ac_nog_gong.c"
#include "../src/ftr/ac_nog_lawnmower.c"
#include "../src/ftr/ac_nog_sprinkler.c"
#include "../src/ftr/ac_dummy.c"
