#ifndef AMBA_CALIB_DEF_H
#define AMBA_CALIB_DEF_H
#ifndef __ASM__

typedef signed char         INT8;           /* 8 bits, [-128, 127] */
typedef short               INT16;          /* 16 bits */
typedef int                 INT32;          /* 32 bits */
typedef long long           INT64;          /* 64 bits */

typedef unsigned char       UINT8;          /* 8 bits, [0, 255] */
typedef unsigned short      UINT16;         /* 16 bits */
typedef unsigned int        UINT32;         /* 32 bits */
typedef unsigned long long  UINT64;         /* 64 bits */

typedef unsigned short      WCHAR;          /* 16 bits */
typedef long unsigned int   SIZE_t;

typedef float               FLOAT;
typedef double              DOUBLE;
typedef long                LONG;
typedef unsigned long       ULONG;

/* GNU Compiler Setting */
/* Watch out: there is no space between __attribute__ and (x) */

#endif  /* !__ASM__ */

#define BLEND_ALPHA_ITSELF (255U)
#define CALIB_MAX_RAW_WIDTH (4000U)
#define CALIB_MAX_RAW_HEIGHT (3000U)
#define CALIB_MAX_RAW_SIZE (CALIB_MAX_RAW_WIDTH  * CALIB_MAX_RAW_HEIGHT)

typedef enum {
    AMBA_CAL_TBL_VALID = 0,
    AMBA_CAL_TBL_INVALID,
} AMBA_CAL_TBL_STATUS_e;

typedef enum {
    AMBA_CAL_ROTATE_0 = 0,
    AMBA_CAL_ROTATE_90,
    AMBA_CAL_ROTATE_180,
    AMBA_CAL_ROTATE_270,
} AMBA_CAL_ROTATION_e;

typedef struct {
    DOUBLE X;
    DOUBLE Y;
} AMBA_CAL_POINT_DB_2D_s;

typedef struct {
    INT32 X;
    INT32 Y;
} AMBA_CAL_POINT_INT_2D_s;

typedef struct {
    DOUBLE X;
    DOUBLE Y;
    DOUBLE Z;
} AMBA_CAL_POINT_DB_3D_s;

typedef struct {
    INT32 X;
    INT32 Y;
    INT32 Z;
} AMBA_CAL_POINT_INT_3D_s;

typedef struct {
    UINT32 StartX;
    UINT32 StartY;
    UINT32 Width;
    UINT32 Height;
} AMBA_CAL_ROI_s;

typedef struct {
    UINT32 Width;
    UINT32 Height;
} AMBA_CAL_SIZE_s;

typedef struct {
    AMBA_CAL_POINT_DB_3D_s WorldPos;
    AMBA_CAL_POINT_DB_2D_s RawPos;
} AMBA_CAL_WORLD_RAW_POINT_s;
#ifdef __cplusplus
extern "C" {
#endif
UINT32 SvcAVMScenario_GetRaw2World(UINT32 FovIdx, UINT32 PointNum, AMBA_CAL_POINT_DB_2D_s *Raw, AMBA_CAL_POINT_DB_3D_s *pWorld);
UINT32 SvcAVMScenario_Get2DWorld2Vout(AMBA_CAL_POINT_DB_3D_s *pWorld, AMBA_CAL_POINT_DB_3D_s *pVout);
UINT32 SvcAVMScenario_GetWorld2Raw(UINT32 FovIdx, AMBA_CAL_POINT_DB_3D_s *pWorld, AMBA_CAL_POINT_DB_2D_s *pRaw);
UINT32 SvcAVMScenario_Get2DYuv2Vout(UINT32 FovIdx, const AMBA_CAL_POINT_DB_3D_s *pYuv, AMBA_CAL_POINT_DB_3D_s *pVout);
UINT32 SvcAVMScenario_Get2DVout2Yuv(UINT32 FovIdx, const AMBA_CAL_POINT_DB_3D_s *pVout, AMBA_CAL_POINT_DB_3D_s *pYuv);

void SvcAVMScenario_Init(void);
#ifdef __cplusplus
};
#endif

#endif
