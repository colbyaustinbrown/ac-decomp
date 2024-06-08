#ifndef SUB_SYS_H
#define SUB_SYS_H

#include "types.h"
#include "jaudio_NES/audiostruct.h"
#include "PR/mbi.h"

extern s32 CreateAudioTask(Acmd* cmds, s16* pSamples, u32 nSamples, s32 param_4);

extern void Nap_SetU16(u32 command, u16 value);
extern void Nap_SetU8(u32 command, u8 value);
extern void Nap_SetS8(u32 command, s8 value);
extern void Nap_SetF32(u32 command, f32 value);
extern void Nap_SetS32(u32 command, s32 value);

extern s8 Nap_ReadSubPort(s32, s32, s32);
extern s8 Nap_ReadGrpPort(s32, s32);

extern s32 Nap_SendStart(void);
extern void Nap_WaitVsync(void);

extern s32 Nap_GetRandom();

#endif