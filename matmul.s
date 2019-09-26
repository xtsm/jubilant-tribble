.global matmul
.global vecmul

matmul:
    vmovapd (%rsi), %ymm0
    vmovapd 32(%rsi), %ymm1
    vmovapd 64(%rsi), %ymm2
    vmovapd 96(%rsi), %ymm3

    vunpcklpd %ymm1, %ymm0, %ymm4
    vunpcklpd %ymm3, %ymm2, %ymm5
    vunpckhpd %ymm1, %ymm0, %ymm6
    vunpckhpd %ymm3, %ymm2, %ymm7
    vperm2f128 $48, %ymm5, %ymm4, %ymm0
    vperm2f128 $33, %ymm5, %ymm4, %ymm1
    vperm2f128 $48, %ymm7, %ymm6, %ymm2
    vperm2f128 $33, %ymm7, %ymm6, %ymm3

    vmovapd (%rdi), %ymm7
    vmulpd %ymm7, %ymm0, %ymm4
    vmulpd %ymm7, %ymm1, %ymm5
    vmulpd %ymm7, %ymm2, %ymm6
    vmulpd %ymm7, %ymm3, %ymm7
    vpermpd $78, %ymm5, %ymm5
    vpermpd $78, %ymm7, %ymm7
    vhaddpd %ymm5, %ymm4, %ymm4
    vhaddpd %ymm7, %ymm6, %ymm6
    vhaddpd %ymm6, %ymm4, %ymm4
    vmovapd %ymm4, (%rdi)

    vmovapd 32(%rdi), %ymm7
    vmulpd %ymm7, %ymm0, %ymm4
    vmulpd %ymm7, %ymm1, %ymm5
    vmulpd %ymm7, %ymm2, %ymm6
    vmulpd %ymm7, %ymm3, %ymm7
    vpermpd $78, %ymm5, %ymm5
    vpermpd $78, %ymm7, %ymm7
    vhaddpd %ymm5, %ymm4, %ymm4
    vhaddpd %ymm7, %ymm6, %ymm6
    vhaddpd %ymm6, %ymm4, %ymm4
    vmovapd %ymm4, 32(%rdi)

    vmovapd 64(%rdi), %ymm7
    vmulpd %ymm7, %ymm0, %ymm4
    vmulpd %ymm7, %ymm1, %ymm5
    vmulpd %ymm7, %ymm2, %ymm6
    vmulpd %ymm7, %ymm3, %ymm7
    vpermpd $78, %ymm5, %ymm5
    vpermpd $78, %ymm7, %ymm7
    vhaddpd %ymm5, %ymm4, %ymm4
    vhaddpd %ymm7, %ymm6, %ymm6
    vhaddpd %ymm6, %ymm4, %ymm4
    vmovapd %ymm4, 64(%rdi)

    vmovapd 96(%rdi), %ymm7
    vmulpd %ymm7, %ymm0, %ymm4
    vmulpd %ymm7, %ymm1, %ymm5
    vmulpd %ymm7, %ymm2, %ymm6
    vmulpd %ymm7, %ymm3, %ymm7
    vpermpd $78, %ymm5, %ymm5
    vpermpd $78, %ymm7, %ymm7
    vhaddpd %ymm5, %ymm4, %ymm4
    vhaddpd %ymm7, %ymm6, %ymm6
    vhaddpd %ymm6, %ymm4, %ymm4
    vmovapd %ymm4, 96(%rdi)
    ret

vecmul:
    vmovapd (%rsi), %ymm3
    vmulpd (%rdi), %ymm3, %ymm4
    vmulpd 32(%rdi), %ymm3, %ymm5
    vmulpd 64(%rdi), %ymm3, %ymm6
    vmulpd 96(%rdi), %ymm3, %ymm7
    vperm2f128 $48, %ymm6, %ymm4, %ymm0
    vperm2f128 $33, %ymm6, %ymm4, %ymm1
    vperm2f128 $48, %ymm7, %ymm5, %ymm2
    vperm2f128 $33, %ymm7, %ymm5, %ymm3
    vhaddpd %ymm1, %ymm0, %ymm0
    vhaddpd %ymm3, %ymm2, %ymm2
    vhaddpd %ymm2, %ymm0, %ymm0
    vmovapd %ymm0, (%rsi)
    ret
