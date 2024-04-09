#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "AmbaCalib_Def.h"
#include "AmbaIPC_Rpc_Def.h"
#include "aipc_user.h"
#include "rtos_ipc.h"

typedef unsigned int UINT32;
typedef double DOUBLE;

struct InputPara {
  UINT32 Idx;
  char Data[0];
};

struct GetRaw2World {
  UINT32 FovIdx;
  UINT32 PointNum;
  AMBA_CAL_POINT_DB_2D_s Raw;
};

static CLIENT_ID_t clnt;

void SvcAVMScenario_Exit(void) { ambaipc_clnt_destroy(clnt); }

void SvcAVMScenario_Init(void) {
  clnt = ambaipc_clnt_create(1, 0x100FFFFF, 1);
  if (!clnt) {
    //printf("ambaipc_clnt_create err\n");
    exit(-2);
  }

  atexit(SvcAVMScenario_Exit);
}

// idx = 1
UINT32 SvcAVMScenario_GetRaw2World(UINT32 FovIdx, UINT32 PointNum,
                                   AMBA_CAL_POINT_DB_2D_s *Raw,
                                   AMBA_CAL_POINT_DB_3D_s *pWorld) {
  AMBA_IPC_REPLY_STATUS_e status;
  struct InputPara inputPara;
  char buf[100];
  struct GetRaw2World getRaw2World;
  AMBA_CAL_POINT_DB_3D_s world;

  getRaw2World.FovIdx = FovIdx;
  getRaw2World.PointNum = PointNum;
  getRaw2World.Raw = *Raw;
  inputPara.Idx = 1;
  memcpy(buf, (char *)&inputPara, sizeof(inputPara));
  memcpy(buf + sizeof(inputPara), (char *)&getRaw2World, sizeof(getRaw2World));

  status =
      ambaipc_clnt_call(clnt, 1, buf, sizeof(inputPara) + sizeof(getRaw2World),
                        &world, sizeof(AMBA_CAL_POINT_DB_3D_s), 100);

  *pWorld = world;
  //printf("%s %d:%f:%f\n", __FUNCTION__, status, pWorld->X, pWorld->Y);
  return status;
}

// Idx = 2
UINT32 SvcAVMScenario_Get2DWorld2Vout(AMBA_CAL_POINT_DB_3D_s *pWorld,
                                      AMBA_CAL_POINT_DB_3D_s *pVout) {
  AMBA_IPC_REPLY_STATUS_e status;
  struct InputPara inputPara;
  char buf[100];
  AMBA_CAL_POINT_DB_3D_s world;

  inputPara.Idx = 2;
  memcpy(buf, (char *)&inputPara, sizeof(inputPara));
  memcpy(buf + sizeof(inputPara), (char *)pWorld, sizeof(*pWorld));

  status = ambaipc_clnt_call(clnt, 1, buf, sizeof(inputPara) + sizeof(*pWorld),
                             &world, sizeof(AMBA_CAL_POINT_DB_2D_s), 100);
  *pVout = world;

  ////printf("%s %d:%f:%f\n", __FUNCTION__, status, world.X, world.Y);

  return status;
}
// Idx = 3
struct GetWorld2Raw {
  UINT32 FovIdx;
  AMBA_CAL_POINT_DB_3D_s World;
};
UINT32 SvcAVMScenario_GetWorld2Raw(UINT32 FovIdx,
                                   AMBA_CAL_POINT_DB_3D_s *pWorld,
                                   AMBA_CAL_POINT_DB_2D_s *pRaw) {
  struct InputPara inputPara;
  char buf[100];
  struct GetWorld2Raw getWorld2Raw;
  AMBA_CAL_POINT_DB_2D_s Raw;
  AMBA_IPC_REPLY_STATUS_e status;

  getWorld2Raw.FovIdx = FovIdx;
  getWorld2Raw.World = *pWorld;

  inputPara.Idx = 3;
  memcpy(buf, (char *)&inputPara, sizeof(inputPara));
  memcpy(buf + sizeof(inputPara), (char *)&getWorld2Raw, sizeof(getWorld2Raw));

  //printf("%f %f %f\n", pWorld->X, pWorld->Y, pWorld->Z);

  status =
      ambaipc_clnt_call(clnt, 1, buf, sizeof(inputPara) + sizeof(getWorld2Raw),
                        &Raw, sizeof(AMBA_CAL_POINT_DB_2D_s), 100);
  *pRaw = Raw;

  //printf("%s %d:%f:%f\n", __FUNCTION__, status, Raw.X, Raw.Y);

  return status;
}

// Idx = 4
struct Get2DYuv2Vou {
  UINT32 FovIdx;
  AMBA_CAL_POINT_DB_3D_s Yuv;
};
UINT32 SvcAVMScenario_Get2DYuv2Vout(UINT32 FovIdx,
                                    const AMBA_CAL_POINT_DB_3D_s *pYuv,
                                    AMBA_CAL_POINT_DB_3D_s *pVout) {
  struct InputPara inputPara;
  char buf[100];
  struct Get2DYuv2Vou get2DYuv2Vou;
  AMBA_CAL_POINT_DB_3D_s Vout;
  AMBA_IPC_REPLY_STATUS_e status;

  get2DYuv2Vou.FovIdx = FovIdx;
  get2DYuv2Vou.Yuv = *pYuv;

  inputPara.Idx = 4;
  memcpy(buf, (char *)&inputPara, sizeof(inputPara));
  memcpy(buf + sizeof(inputPara), (char *)&get2DYuv2Vou, sizeof(get2DYuv2Vou));

  status =
      ambaipc_clnt_call(clnt, 1, buf, sizeof(inputPara) + sizeof(get2DYuv2Vou),
                        &Vout, sizeof(AMBA_CAL_POINT_DB_2D_s), 100);
  *pVout = Vout;

  //printf("%s %d:%f:%f\n", __FUNCTION__, status, Vout.X, Vout.Y);

  return status;
}

// Idx = 5
struct Get2DVout2Yuv {
  UINT32 FovIdx;
  AMBA_CAL_POINT_DB_3D_s Vout;
};
UINT32 SvcAVMScenario_Get2DVout2Yuv(UINT32 FovIdx,
                                    const AMBA_CAL_POINT_DB_3D_s *pVout,
                                    AMBA_CAL_POINT_DB_3D_s *pYuv) {
  struct InputPara inputPara;
  char buf[100];
  struct Get2DVout2Yuv get2DVout2Yuv;
  AMBA_CAL_POINT_DB_3D_s Yuv;
  AMBA_IPC_REPLY_STATUS_e status;

  get2DVout2Yuv.FovIdx = FovIdx;
  get2DVout2Yuv.Vout = *pVout;

  inputPara.Idx = 5;
  memcpy(buf, (char *)&inputPara, sizeof(inputPara));
  memcpy(buf + sizeof(inputPara), (char *)&get2DVout2Yuv,
         sizeof(get2DVout2Yuv));

  status =
      ambaipc_clnt_call(clnt, 1, buf, sizeof(inputPara) + sizeof(get2DVout2Yuv),
                        &Yuv, sizeof(AMBA_CAL_POINT_DB_2D_s), 100);
  *pYuv = Yuv;

  //printf("%s %d:%f:%f\n", __FUNCTION__, status, Yuv.X, Yuv.Y);

  return status;
}
