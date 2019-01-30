#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer ConstantBuffer
// {
//
//   float4x4 world;                    // Offset:    0 Size:    64 [unused]
//   float4x4 view;                     // Offset:   64 Size:    64
//   float4x4 projection;               // Offset:  128 Size:    64
//   float4 lightDir[3];                // Offset:  192 Size:    48 [unused]
//   float4 lightCol[3];                // Offset:  240 Size:    48 [unused]
//   float4 outputCol;                  // Offset:  288 Size:    16 [unused]
//   float3 pad;                        // Offset:  304 Size:    12 [unused]
//   float pointRad;                    // Offset:  316 Size:     4 [unused]
//   float3 pad2;                       // Offset:  320 Size:    12 [unused]
//   float coneRatio;                   // Offset:  332 Size:     4 [unused]
//   float4 coneDir;                    // Offset:  336 Size:    16 [unused]
//   float4x4 TreeInstPositions[16];    // Offset:  352 Size:  1024
//   float4 camPos;                     // Offset: 1376 Size:    16 [unused]
//   float2 pad3;                       // Offset: 1392 Size:     8 [unused]
//   float2 PowInt;                     // Offset: 1400 Size:     8 [unused]
//   float4 fairyPos[8];                // Offset: 1408 Size:   128 [unused]
//   float deltaT;                      // Offset: 1536 Size:     4 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// ConstantBuffer                    cbuffer      NA          NA            cb0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyzw        0     NONE   float   xyzw
// COLOR                    0   xyzw        1     NONE   float   xyzw
// TEXCOORD                 0   xy          2     NONE   float   xy  
// NORMAL                   0   xyzw        3     NONE   float   xyz 
// SV_InstanceID            0   x           4   INSTID    uint   x   
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// COLOR                    0   xyzw        1     NONE   float   xyzw
// TEXCOORD                 1   xy          2     NONE   float   xy  
// NORMAL                   0   xyzw        3     NONE   float   xyzw
// TEXCOORD                 2   xyzw        4     NONE   float   xyzw
// TEXCOORD                 3   xyz         5     NONE   float   xyz 
//
vs_4_0
dcl_constantbuffer CB0[86], dynamicIndexed
dcl_input v0.xyzw
dcl_input v1.xyzw
dcl_input v2.xy
dcl_input v3.xyz
dcl_input_sgv v4.x, instance_id
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_output o2.xy
dcl_output o3.xyzw
dcl_output o4.xyzw
dcl_output o5.xyz
dcl_temps 3
ishl r0.x, v4.x, l(2)
mul r1.xyzw, v0.yyyy, cb0[r0.x + 23].xyzw
mad r1.xyzw, cb0[r0.x + 22].xyzw, v0.xxxx, r1.xyzw
mad r1.xyzw, cb0[r0.x + 24].xyzw, v0.zzzz, r1.xyzw
mad r1.xyzw, cb0[r0.x + 25].xyzw, v0.wwww, r1.xyzw
dp4 r2.x, r1.xyzw, cb0[4].xyzw
dp4 r2.y, r1.xyzw, cb0[5].xyzw
dp4 r2.z, r1.xyzw, cb0[6].xyzw
dp4 r2.w, r1.xyzw, cb0[7].xyzw
mov o4.xyzw, r1.xyzw
dp4 o0.x, r2.xyzw, cb0[8].xyzw
dp4 o0.y, r2.xyzw, cb0[9].xyzw
dp4 o0.z, r2.xyzw, cb0[10].xyzw
dp4 o0.w, r2.xyzw, cb0[11].xyzw
mov o1.xyzw, v1.xyzw
mov o2.xy, v2.xyxx
mul r1.xyzw, v3.yyyy, cb0[r0.x + 23].xyzw
mad r1.xyzw, cb0[r0.x + 22].xyzw, v3.xxxx, r1.xyzw
mad o3.xyzw, cb0[r0.x + 24].xyzw, v3.zzzz, r1.xyzw
mov o5.xyz, l(0,0,0,0)
ret 
// Approximately 21 instruction slots used
#endif

const BYTE VSFairy[] =
{
     68,  88,  66,  67, 179, 247, 
    243,  76, 123,  94, 147, 200, 
      6, 154,  58, 105, 232,  92, 
    223, 230,   1,   0,   0,   0, 
    244,   8,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    140,   3,   0,   0,  68,   4, 
      0,   0,   8,   5,   0,   0, 
    120,   8,   0,   0,  82,  68, 
     69,  70,  80,   3,   0,   0, 
      1,   0,   0,   0,  76,   0, 
      0,   0,   1,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
    254, 255,   0,   1,   0,   0, 
     39,   3,   0,   0,  60,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     67, 111, 110, 115, 116,  97, 
    110, 116,  66, 117, 102, 102, 
    101, 114,   0, 171,  60,   0, 
      0,   0,  17,   0,   0,   0, 
    100,   0,   0,   0,  16,   6, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 252,   1, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   0,   0, 
      0,   0,   4,   2,   0,   0, 
      0,   0,   0,   0,  20,   2, 
      0,   0,  64,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,   4,   2,   0,   0, 
      0,   0,   0,   0,  25,   2, 
      0,   0, 128,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,   4,   2,   0,   0, 
      0,   0,   0,   0,  36,   2, 
      0,   0, 192,   0,   0,   0, 
     48,   0,   0,   0,   0,   0, 
      0,   0,  48,   2,   0,   0, 
      0,   0,   0,   0,  64,   2, 
      0,   0, 240,   0,   0,   0, 
     48,   0,   0,   0,   0,   0, 
      0,   0,  76,   2,   0,   0, 
      0,   0,   0,   0,  92,   2, 
      0,   0,  32,   1,   0,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0, 104,   2,   0,   0, 
      0,   0,   0,   0, 120,   2, 
      0,   0,  48,   1,   0,   0, 
     12,   0,   0,   0,   0,   0, 
      0,   0, 124,   2,   0,   0, 
      0,   0,   0,   0, 140,   2, 
      0,   0,  60,   1,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 152,   2,   0,   0, 
      0,   0,   0,   0, 168,   2, 
      0,   0,  64,   1,   0,   0, 
     12,   0,   0,   0,   0,   0, 
      0,   0, 124,   2,   0,   0, 
      0,   0,   0,   0, 173,   2, 
      0,   0,  76,   1,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 152,   2,   0,   0, 
      0,   0,   0,   0, 183,   2, 
      0,   0,  80,   1,   0,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0, 104,   2,   0,   0, 
      0,   0,   0,   0, 191,   2, 
      0,   0,  96,   1,   0,   0, 
      0,   4,   0,   0,   2,   0, 
      0,   0, 212,   2,   0,   0, 
      0,   0,   0,   0, 228,   2, 
      0,   0,  96,   5,   0,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0, 104,   2,   0,   0, 
      0,   0,   0,   0, 235,   2, 
      0,   0, 112,   5,   0,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   0, 240,   2,   0,   0, 
      0,   0,   0,   0,   0,   3, 
      0,   0, 120,   5,   0,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   0, 240,   2,   0,   0, 
      0,   0,   0,   0,   7,   3, 
      0,   0, 128,   5,   0,   0, 
    128,   0,   0,   0,   0,   0, 
      0,   0,  16,   3,   0,   0, 
      0,   0,   0,   0,  32,   3, 
      0,   0,   0,   6,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 152,   2,   0,   0, 
      0,   0,   0,   0, 119, 111, 
    114, 108, 100,   0, 171, 171, 
      3,   0,   3,   0,   4,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 118, 105, 
    101, 119,   0, 112, 114, 111, 
    106, 101,  99, 116, 105, 111, 
    110,   0, 108, 105, 103, 104, 
    116,  68, 105, 114,   0, 171, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   4,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
    108, 105, 103, 104, 116,  67, 
    111, 108,   0, 171, 171, 171, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   3,   0,   0,   0, 
      0,   0,   0,   0, 111, 117, 
    116, 112, 117, 116,  67, 111, 
    108,   0, 171, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 112,  97, 100,   0, 
      1,   0,   3,   0,   1,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 112, 111, 
    105, 110, 116,  82,  97, 100, 
      0, 171, 171, 171,   0,   0, 
      3,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 112,  97, 100,  50, 
      0,  99, 111, 110, 101,  82, 
     97, 116, 105, 111,   0,  99, 
    111, 110, 101,  68, 105, 114, 
      0,  84, 114, 101, 101,  73, 
    110, 115, 116,  80, 111, 115, 
    105, 116, 105, 111, 110, 115, 
      0, 171, 171, 171,   3,   0, 
      3,   0,   4,   0,   4,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0,  99,  97, 109,  80, 
    111, 115,   0, 112,  97, 100, 
     51,   0,   1,   0,   3,   0, 
      1,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     80, 111, 119,  73, 110, 116, 
      0, 102,  97, 105, 114, 121, 
     80, 111, 115,   0,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   0, 100, 101, 108, 116, 
     97,  84,   0,  77, 105,  99, 
    114, 111, 115, 111, 102, 116, 
     32,  40,  82,  41,  32,  72, 
     76,  83,  76,  32,  83, 104, 
     97, 100, 101, 114,  32,  67, 
    111, 109, 112, 105, 108, 101, 
    114,  32,  49,  48,  46,  49, 
      0, 171,  73,  83,  71,  78, 
    176,   0,   0,   0,   5,   0, 
      0,   0,   8,   0,   0,   0, 
    128,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,  15,   0,   0, 
    137,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  15,  15,   0,   0, 
    143,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   3,   3,   0,   0, 
    152,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  15,   7,   0,   0, 
    159,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,   1,   1,   0,   0, 
     80,  79,  83,  73,  84,  73, 
     79,  78,   0,  67,  79,  76, 
     79,  82,   0,  84,  69,  88, 
     67,  79,  79,  82,  68,   0, 
     78,  79,  82,  77,  65,  76, 
      0,  83,  86,  95,  73, 110, 
    115, 116,  97, 110,  99, 101, 
     73,  68,   0, 171, 171, 171, 
     79,  83,  71,  78, 188,   0, 
      0,   0,   6,   0,   0,   0, 
      8,   0,   0,   0, 152,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0, 164,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     15,   0,   0,   0, 170,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      3,  12,   0,   0, 179,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     15,   0,   0,   0, 170,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
     15,   0,   0,   0, 170,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   5,   0,   0,   0, 
      7,   8,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  67,  79, 
     76,  79,  82,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0,  78,  79,  82,  77,  65, 
     76,   0, 171, 171,  83,  72, 
     68,  82, 104,   3,   0,   0, 
     64,   0,   1,   0, 218,   0, 
      0,   0,  89,   8,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,  86,   0,   0,   0, 
     95,   0,   0,   3, 242,  16, 
     16,   0,   0,   0,   0,   0, 
     95,   0,   0,   3, 242,  16, 
     16,   0,   1,   0,   0,   0, 
     95,   0,   0,   3,  50,  16, 
     16,   0,   2,   0,   0,   0, 
     95,   0,   0,   3, 114,  16, 
     16,   0,   3,   0,   0,   0, 
     96,   0,   0,   4,  18,  16, 
     16,   0,   4,   0,   0,   0, 
      8,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
     50,  32,  16,   0,   2,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   3,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   4,   0, 
      0,   0, 101,   0,   0,   3, 
    114,  32,  16,   0,   5,   0, 
      0,   0, 104,   0,   0,   2, 
      3,   0,   0,   0,  41,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,  16, 
     16,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   2,   0, 
      0,   0,  56,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86,  21,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   6,   0,   0,   0,   0, 
     23,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  12, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   6,   0,   0, 
      0,   0,  22,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   6,  16,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  12, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   6,   0,   0, 
      0,   0,  24,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  12, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   6,   0,   0, 
      0,   0,  25,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0, 246,  31,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     17,   0,   0,   8,  18,   0, 
     16,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,  17,   0,   0,   8, 
     34,   0,  16,   0,   2,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  17,   0, 
      0,   8,  66,   0,  16,   0, 
      2,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
     17,   0,   0,   8, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   7,   0, 
      0,   0,  54,   0,   0,   5, 
    242,  32,  16,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  17,   0, 
      0,   8,  18,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     17,   0,   0,   8,  34,  32, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   9,   0, 
      0,   0,  17,   0,   0,   8, 
     66,  32,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,  17,   0, 
      0,   8, 130,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,  11,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   1,   0,   0,   0, 
     70,  30,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
     50,  32,  16,   0,   2,   0, 
      0,   0,  70,  16,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,  21, 
     16,   0,   3,   0,   0,   0, 
     70, 142,  32,   6,   0,   0, 
      0,   0,  23,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  12, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   6, 
      0,   0,   0,   0,  22,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   6,  16, 
     16,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  12, 
    242,  32,  16,   0,   3,   0, 
      0,   0,  70, 142,  32,   6, 
      0,   0,   0,   0,  24,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0, 166,  26, 
     16,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   8, 
    114,  32,  16,   0,   5,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    116,   0,   0,   0,  21,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  11,   0, 
      0,   0,  15,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};
