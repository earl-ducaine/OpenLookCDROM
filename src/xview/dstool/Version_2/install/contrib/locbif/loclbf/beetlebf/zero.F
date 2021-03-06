C  -------------------------------------------------------------------
C
C	Copyright (1990-1995) by Alexander Khibnik, Yuri Kuznetsov, and 
C	Eugene Nikolaev.
C
C The Locbif computation code in DsTool is distributed in the hope that it  
C will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty  
C of FITNESS FOR A PARTICULAR PURPOSE.  The software is provided as is without 
C any obligation on the part of Locbif authors, Cornell faculty, 
C Cornell staff or Cornell students to assist in its use, correction, 
C modification or enhancement. 
C
C  -------------------------------------------------------------------

      SUBROUTINE ZERO (ISTEP,NX,NDIMM,NG,NCAN,NUF,IFUNS,IPRZ,STX2,EPS,
     A                 FUN,DFDX,OUT,DFUN,ISTOP)
C     3.05.90�.
C     7.08.1991
C     13.11.92
C     15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA  ZERO  �PE�HA�HA�EHA ��� �TO�HEH�� KOOP��HAT H��E�     *
C*    �A�AHH�X ��HK���.                                                *
C*                                                                     *
C*    � A P A M E T P �:                                               *
C*    =================                                                *
C*                                                                     *
C*    ISTEP -��C�O �A�OB, �PO�E�AHH�X B�O�� KP�BO�.                    *
C*    NX    -PA�MEPHOCT� �POCTPAHCTBA �EPMEHH�X.                       *
C*    NDIM  -�A�B�EHHA� CTPO�HA� PA�MEPHOCT� MATP��� DFUN.             *
C*    NG    -HOMEP ��ABHO� �EPEMEHHO�.                                 *
C*    NCAN  -HOMEP KAHA�A ��� B�BO�A PE����TATOB.                      *
C*    IPRZ  -�APAMETP, O�PE�E������ �O�HOT� �POTOKO�A.                 *
C*    NUF   -��C�O ��HK���, �CC�E��EM�X HA KP�BO�.                     *
C*    IFUNS -MACC�B HOMEPOB �CC�E��EM�X ��HK���.                       *
C*    STX2  -KOOP��HAT� TEK��E� TO�K� HA KP�BO�.                       *
C*    EPS   -TO�HOCT� O�PE�E�EH�� TO�K� HA KP�BO� B METO�E H��TOHA.    *
C*    FUN   -�M� �O��PO�PAMM� B���C�EH�� O�PE�E�����X ��HK���.         *
C*    DFDX  -�M� �O��PO�PAMM� B���C�EH�� �KO��AHA O�PE�E�����X ��HK���.*
C*    DFUN  -MACC�B, CO�EP�A��� �KO��AH O�PE�E�����X ��HK���.          *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*    �P� ��MEHEH�� MAKC�MA��HO�O ��C�A �EPEMEHH�X  NDIM -HEO�XO��MO   *
C*    ��MEH�T� PA�MEPHOCT� MACC�BOB:                                   *
C*    STX1(*) STX2(*) X1(*) X2(*) XZERO(*) VMOVE(*)                    *
C* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . *
C*    �P� ��MEHEH�� MAKC�MA��HO�O ��C�A ��HK���, �CC�E��EM�X HA KP�BO�.*
C*    HEO�XO��MO ��MEH�T� PA�MEPHOCT� MACC�BOB:                        *
C*    FUNS1(*) IFUNS(*) IFUNSA(*) IERF(*) FUNS2(*).                    *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2(I-N)
      include 'beetlebf.dim'
      CHARACTER*56 TEXT
      DIMENSION STX1(NDIM), STX2(NDIM), X1(NDIM), X2(NDIM),
     A          XZERO(NDIM), FUNS1(IFMAX), IFUNS(IFMAX),
     B          DFUN(NDIM,NX), INDEX(NPTYP)
      EXTERNAL FUN, DFDX, OUT
C
      COMMON /ADD3B/ EPSZER
C
      COMMON /OUT1/ FUNS2(IFMAX)
      COMMON /OUT2/ INDZ, IERF(IFMAX)
      COMMON /OUT4/ EPSZ, EPSR, FZERO
C
      COMMON /ZERO2/ IFUNSA(IFMAX)
      COMMON /WRTO1/ NFIX1, NFIX
      COMMON /WRTO2/ DMOVE1, DMOVE
      COMMON /WRTO4/ EPSC, EPSN
      COMMON /WRTO6/ ITC,IT2
      COMMON /WRTO7/ VMOVE(NDIM),VMOVN(NDIM)
      SAVE STX1,FUNS1,/OUT1/,/OUT2/,/OUT4/
C
      ISTOP = 0
      IF (NUF .EQ. 0) RETURN
          DO 100 I=1,NPTYP
             INDEX(I) = 0
 100      CONTINUE
          EPSC  = EPS
C
C.... B���C�EH�E �HA�EH�� BCEX ��HK��� B TEK��E� TO�KE HA KP�BO�.
C
          DO 200 IFUN=1,NUF
            INDF        = IFUNSA (IFUN)
            IER         = 0
            FUNS2(INDF) = FUNALL (INDF,NX,STX2,FUN,DFDX,IER,ISTOP)
            IF (ISTOP .NE. 0) RETURN
            IF (IER   .EQ. 0) GOTO 170
C
C.... HE B���C�EHO �HA�EH�E ��HK��� B TEK��E� TO�KE HA KP�BO�.
C
                IF (ISTEP .EQ. 0) FUNS1(INDF) = -666.D+33
                                  FUNS2(INDF) = -666.D+33
                IERF(INDF) = IER
                GOTO 190
 170        CONTINUE
            IF (IERF(INDF) .EQ. 0) GOTO 180
C
C.... HE B���C�EH� �HA�EH�� ��HK��� B �PE������X TO�KAX HA KP�BO�;
C     B���C�EHHOE �HA�EH�E ��HK��� B TEK��E� TO�KE HE �POBEP�ETC�.
C
                FUNS1(INDF) = FUNS2(INDF)
                IERF(INDF)  = 0
 180        CONTINUE
 190        CONTINUE
 200      CONTINUE
C
C.... B�BO� �POTOKO�A PAC�ETA ��HK���.
C
          IPRTCL = 3
#if defined(hib)
          CALL WRTZER (IPRTCL,INDF,IWRT,ISTEP,IPRZ,IT,IDIV,NCAN,NX,
     A                 NDIM,ITER,NUF,IFUNS,EPSR,EPSZ,FUNS2,
     B                 X2,FX,DFUN)
#endif
C
C.... O�PA�OTKA �O���OBATE�EM TEK��E� TO�K� HA KP�BO� �
C     B���C�EHH�X �HA�EH�� ��HK���.
C
          INDEX(1) = 1
          CALL OUT (ISTEP,INDEX,NX,NDIM,STX2,VMOVE,DFUN,ISTOP)
          IF (ISTOP .NE. 0) RETURN
          INDEX(1) = 0
          IF (ISTEP .EQ. 0) GOTO 390
              ITMAX = 10
              EPSZ  = EPSZER
C
C.... ��K� �POBEPK� HA CMEH� �HAKA BCEX B���C�EHH�X �HA�EH��.
C
              DO 380 IFUN=1,NUF
                 INDZ  = 0
                 INDF = IFUNSA(IFUN)
C
C.... �POBEPKA �C�OB��:
C     1). IFUNS(IFUN) < 0 -H��� HE �TO�H�ETC�;
C     2). IERF (INDF) .NE. 0 -�HA�EH�E ��HK��� HE B���C�EHO;
C
                 IF (IFUNS(IFUN) .LT. 0) GOTO 370
                 IF (IERF (INDF) .NE. 0) GOTO 370
C
C.... �O��OTOBKA PA�O��X MACC�BOB.
C
                 DO 210 I=1,NX
                    X1(I) = STX1(I)
                    X2(I) = STX2(I)
 210             CONTINUE
C
C.... �POBEPKA �C�OB�� CMEH� �HAKA.
C
                 F1 = FUNS1(INDF)
                 F2 = FUNS2(INDF)
                 IF (F1*F2 .GT. 0.D0) GOTO 370
                 DF = DABS((F1-F2)/(X1(NFIX1)-X2(NFIX1)))
                 IF (DF.LT.0.1*EPSZ) GOTO 370
		 INDLOC = 0
C
C.... ��K� �TO�HEH�� KOOP��HAT H��� ��HK���.
C
                     DO 300 ITER =1,ITMAX
C
C.... �PO�HO� KOOP��HAT H��� ��HK���.
C
 215              CONTINUE
                  IF (INDLOC .EQ. 0) RX = F2/(F2-F1)
                  IF (INDLOC .EQ. 1) RX = 0.5D0
                  IF (INDLOC .EQ. 2) RX = 4.D0/9.D0
C
                        DO 220 I=1,NX
                           XZERO(I) = X2(I)-(X2(I)-X1(I))*RX
 220                    CONTINUE
                        IT  = ITC
                        IPR = 0
                        IER = 0
c          WRITE (TEXT,1001) ITER
c          WRITE (6,1001) ITER
c 1001     FORMAT(
c     *   '  ZERO: Iter=',I2,41X)
C         CALL Showte(TEXT)
                        CALL NEWTN (NX,NDIM,NG,NFIX1,NCAN,XZERO,EPS,
     A                            EPSN,IT,IPR,FUN,DFDX,DFUN,IER,ISTOP)
                        IF (ISTOP .NE. 0) RETURN
                        IF (IER   .EQ. 0) GOTO 230
                        INDLOC=INDLOC+1
                        IF (INDLOC .LE. 2) GOTO 215
C
C.... HE CO���C� H��TOHOBCK�E �TEPA���.
C
 225                    CONTINUE
                            IPRTCL = 2
                            IWRT   = 1 - IER
                            GOTO 320
 230                    CONTINUE
        	R1 = 0.D0
		R2 = 0.D0
		DO 235 I=1,NX
                   R1 = R1+(XZERO(I)-STX1(I))*(STX2(I)-STX1(I))
                   R2 = R2+(XZERO(I)-STX2(I))*(STX2(I)-STX1(I))
 235            CONTINUE
                IF (R1*R2 .GT. 0.) GOTO 225
                        IER   = 0
                        FZERO = FUNALL(INDF,NX,XZERO,FUN,DFDX,IER,ISTOP)
c          WRITE (TEXT,1002) FZERO
c          WRITE (6,1002) FZERO
c 1002     FORMAT(
c     *   '  ZERO: Fzero=',G9.2,33X)
C         CALL Showte(TEXT)
                        IF (ISTOP .NE. 0) RETURN
                        IF (IER   .EQ. 0) GOTO 240
                        INDLOC=INDLOC+1
                        IF (INDLOC .LE. 2) GOTO 215
C
C.... HE B���C�EH� �HA�EH�� ��HK��� HA KP�BO�.
C
                            IPRTCL = 2
                            IWRT   = 0
                            GOTO 320
 240                    CONTINUE
C
C.... B��OP �HTEPBA�A, CO�EP�A�E�O H��� ��HK���.
C
                            IF (F1*FZERO .LT. 0.D0) GOTO 260
                                F1 = FZERO
                                DO 250 I=1,NX
                                   X1(I) = XZERO(I)
 250                            CONTINUE
                                GOTO 280
 260                        CONTINUE
                                DO 270 I=1,NX
                                   X2(I) = XZERO(I)
 270                            CONTINUE
                                F2 = FZERO
 280                         CONTINUE
C
C.... KOHTPO�� �OCT��H�TO� TO�HOCT�.
C
                       EPSR = DABS(X2(NFIX1)-X1(NFIX1))
                       DF = DABS((F1-F2)/(X1(NFIX1)-X2(NFIX1)))
                       FN=    DABS (FZERO/DF)
                       IF (EPSR.LE.EPSZ .OR. FN.LE.0.1*EPSZ) GOTO 310
 300                 CONTINUE
                     ITER = ITMAX
                     INDZ = -1
 310                 CONTINUE
C
C.... B��A�A �POTOKO�A: HA��EHO �P�����EH�E K H��� ��HK���.
C
                     IPRTCL = 1
c                     CALL WRTZER (IPRTCL,INDF,IWRT,ISTEP,IPRZ,IT,IDIV,
c     A                            NCAN,NX,NDIM,ITER,NUF,IFUNS,
c     B                            EPSR,EPSZ,XZERO,X2,FZERO,DFUN)
                     INDEX(3) = INDF
                     CALL OUT (ISTEP,INDEX,NX,NDIM,XZERO,VMOVE,
     A                         DFUN,ISTOP)
                     IF (ISTOP .NE. 0) RETURN
                     GOTO 380
 320                 CONTINUE
C
C.... B��A�A �POTOKO�A: HE CO���C� H��TOHOBCK�E �TEPA��� ���
C                       HE B���C�EHO �HA�EH�E ��HK���.
C
                     EPSR = DABS(X2(NFIX1)-X1(NFIX1))
C
                     FZERO = F1
                     IF (DABS(F1) .LE. DABS(F2)) GOTO 340
                         FZERO = F2
                         DO 330 I=1,NX
                            X1(I) = X2(I)
 330                     CONTINUE
 340                 CONTINUE
                     DMOVER= DMOVE
                     DMOVE = DMOVE1
                     NFIXR = NFIX
                     NFIX  = NFIX1
#if defined(hib)
                     CALL WRTZER (IPRTCL,INDF,IWRT,ISTEP,IPRZ,IT,IDIV,
     A                            NCAN,NX,NDIM,ITER,NUF,IFUNS,
     B                            EPSR,EPSZ,X1,XZERO,FZERO,DFUN)
#endif
                     DMOVE = DMOVER
                     NFIX  = NFIXR
C
C.... �O�O�H�TE��HA� O�PA�OTKA B���C�EHH�X �HA�EH�� ��HK���.
C
                     INDZ     =-1
                     INDEX(3) = INDF
                     CALL OUT (ISTEP,INDEX,NX,NDIM,X1,VMOVE,
     A                         DFUN,ISTOP)
                     IF (ISTOP .NE. 0) RETURN
 370             CONTINUE
 380          CONTINUE
 390      CONTINUE
C
C.... �O��OTOBKA K O�EPE�HOM� �A�� B�O�� KP�BO�.
C
          DO 400 I=1,NX
            STX1(I) = STX2(I)
 400      CONTINUE
          DO 410 IFUN = 1,NUF
            INDF        =IFUNSA(IFUN)
            FUNS1(INDF) = FUNS2(INDF)
 410      CONTINUE
          RETURN
          END
