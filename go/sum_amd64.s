#include "textflag.h"

// func sumAVX2(ptr unsafe.Pointer, len int) uint64
TEXT ·sumAVX2(SB), NOSPLIT, $0-24
    MOVQ ptr+0(FP), SI
    MOVQ len+8(FP), CX

    VPXOR Y0, Y0, Y0   // zero
    VPXOR Y1, Y1, Y1   // acc0
    VPXOR Y2, Y2, Y2   // acc1
    VPXOR Y3, Y3, Y3   // acc2
    VPXOR Y4, Y4, Y4   // acc3

    MOVQ CX, DX
    SHRQ $7, DX        // chunks of 128 bytes
    TESTQ DX, DX
    JZ remainder32

loop128:
    VMOVDQU 0(SI), Y5
    VMOVDQU 32(SI), Y6
    VMOVDQU 64(SI), Y7
    VMOVDQU 96(SI), Y8

    VPSADBW Y0, Y5, Y5
    VPSADBW Y0, Y6, Y6
    VPSADBW Y0, Y7, Y7
    VPSADBW Y0, Y8, Y8

    VPADDQ Y5, Y1, Y1
    VPADDQ Y6, Y2, Y2
    VPADDQ Y7, Y3, Y3
    VPADDQ Y8, Y4, Y4

    ADDQ $128, SI
    DECQ DX
    JNZ loop128

remainder32:
    VPADDQ Y2, Y1, Y1
    VPADDQ Y4, Y3, Y3
    VPADDQ Y3, Y1, Y1

    MOVQ CX, DX
    ANDQ $127, DX
    SHRQ $5, DX        // chunks of 32 bytes
    TESTQ DX, DX
    JZ horizontal_sum

loop32:
    VMOVDQU 0(SI), Y5
    VPSADBW Y0, Y5, Y5
    VPADDQ Y5, Y1, Y1
    ADDQ $32, SI
    DECQ DX
    JNZ loop32

horizontal_sum:
    VEXTRACTI128 $1, Y1, X2
    VPADDQ X2, X1, X1
    VMOVDQU X1, X2
    VPSRLDQ $8, X1, X1
    VPADDQ X1, X2, X2
    VMOVQ X2, AX

    // Remaining bytes (< 32)
    MOVQ CX, DX
    ANDQ $31, DX
    TESTQ DX, DX
    JZ done

loop_rem:
    MOVBQZX (SI), BX
    ADDQ BX, AX
    INCQ SI
    DECQ DX
    JNZ loop_rem

done:
    VZEROUPPER
    MOVQ AX, ret+16(FP)
    RET
