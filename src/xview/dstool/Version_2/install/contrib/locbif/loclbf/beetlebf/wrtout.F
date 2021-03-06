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

      SUBROUTINE WRTOUT (IWRT,ISTEP,IPR,IT,IDIV,NCAN,NX,NDIMM,X,DFUN)
C     3.05.90�.
C     13.11.92
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA WRTOUT �PE�HA�HA�EHA ��� �E�AT� CTAH�APTHO�O �POTOKO�A *
C*    PA�OT� �PO�PAMM�  BEETLE.                                        *
C*                                                                     *
C*    � A P A M E T P �:                                               *
C*    ===================                                              *
C*    IWRT  -�P��HAK �E�AT� TPE��EMO�O COO��EH��:                      *
C*                                                                     *
C*           1 -CTAH�APTHA� �E�AT� KOOP��HAT O�EPE�HO� TO�K� HA KP�BO�;*
C*              (�E�AT� HEKOTOP�X KOHTPO��H�X XAPAKTEP�CT�K KP�BO�).   *
C*                                                                     *
C*           2 -COO��EH�E: HE CO���C� H��TOHOBCK�E �TEPA���,           *
C*              �A�AHHA� TO�HOCT� HE �OCT��H�TA.                       *
C*                                                                     *
C*           3 -COO��EH�E: HE CO���C� H��TOHOBCK�E �TEPA���,           *
C*              B�PO��EHA MATP��A �ACTH�X �PO��BO�H�X                  *
C*              (HA �E�AT� B��AETC� MATP��A �KO��).                    *
C*                                                                     *
C*           4 -COO��EH�E: HE CO���C� H��TOHOBCK�E �TEPA���,           *
C*              HE B���C�EH� �HA�EH�� ��HK��� B O�EPE�HO� TO�KE        *
C*              H��TOHOBCKO�O �PO�HO�A.                                *
C*                                                                     *
C*           5 -COO��EH�E: HE CO���C� H��TOHOBCK�E �TEPA���,           *
C*              HE B���C�EHA MATP��A �ACTH�X �PO��BO�H�X B             *
C*              O�EPE�HO� TO�KE H��TOHOBCKO�O �PO�HO�A.                *
C*                                                                     *
C*           6 -COO��EH�E: HE HA��EH KACATE��H�� BEKTOP, B�PO��EHA     *
C*              MATP��A �ACTH�X �PO��BO�H�X (HA �E�AT� B��AETC�        *
C*              MATP��A �ACTH�X �PO��BO�H�X).                          *
C*                                                                     *
C*           7 -COO��EH�E: HE HA��EH KACATE��H�� BEKTOP, HE B���C�EH�  *
C*              �HA�EH�� ��HK��� B TO�KE HA KP�BO�.                    *
C*                                                                     *
C*           8 -COO��EH�E: HE HA��EH KACATE��H�� BEKTOP, HE B���C�EHA  *
C*              MATP��A �ACTH�X �PO��BO�H�X B TO�KE HA KP�BO�.         *
C*                                                                     *
C*           9 -COO��EH�E: �TO�HEHHA� TO�KA HE ����KA K TO�KE-�PO�HO��.*
C*                                                                     *
C*          10 -COO��EH�E: �PO�HO� �O KACATE��HO� B��E� �A �PAH���.    *
C*                                                                     *
C*          11 -COO��EH�E: �PO�HO� �O H��TOH� B��E� �A �PAH���.        *
C*                                                                     *
C*   (12,13,14)-COO��EH�E: �TO�H�ETC� TO�KA �AM�KAH��.                 *
C*                                                                     *
C*          15 -COO��EH�E: �O���OBATE�EM �P�H�TO PE�EH�E �EPEC��TAT�   *
C*                         �TO�HEHH�� TO�K� C �O�OB�HH�M �A�OM.        *
C*                                                                     *
C*    ISTEP -��C�O �PO�E�AHH�X �A�OB B�O�� KP�BO�.                     *
C*    IPR   -�P��HAK �E�AT�:                                           *
C*           (-2;-1)-B��A�A �POTOKO�A OTC�CTB�ET;                      *
C*                0 -HA �E�AT� B��A�TC� KOOP��HAT� TO�EK HA KP�BO�;    *
C*                1 -HA �E�AT� B��A�TC� KOOP��HAT� TO�EK HA KP�BO� �   *
C*                   HEKOTOPA� KOHTPO��HA� �H�OPMA���;                 *
C*                2 -HA �E�AT� B��A�TC� BCE �P���H� �E�EH�� �A�A �O�O�AM
C*                   � HEKOTOPA� �O�O�H�TE��HA� �H�OPMA���.            *
C*    IT    -��C�O H��TOHOBCK�X �TEPA��� �PO�E�AHH�X �P� �TO�HEH��     *
C*           TEK��E� TO�K� HA KP�BO�.                                  *
C*    IDIV  -��C�O �E�EH�� �A�A �O�O�AM.                               *
C*    NCAN  -HOMEP KAHA�A ��� B�BO�A PE����TATOB.                      *
C*    NX    -PA�MEPHOCT� �POCTAHCTBA �EPEMEHH�X.                       *
C*    NFUN  -PA�MEPHOCT� BEKTOP-��HK���.                               *
C*    X     -KOOP��HAT� TO�K� (HA KP�BO� ��� HEKOTOPO� �POME��TO�HO�). *
C*    DFUN  -MACC�B �ACTH�X �PO��BO�H�X.                               *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*    �P� ��MEHEH�� MAKC�MA��HO�O ��C�A �EPEMEHH�X- NDIM,              *
C*    HEO�XO��MO ��MEH�T� PA�MEPHOCT� MACC�BA  VMOVE(NDIM)             *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2(I-N)
      include 'beetlebf.dim'
      LOGICAL MAS1(8), MAS2(8)
      DIMENSION X(1), DFUN(NDIM,NX)
      COMMON /ALL1/  XGMN, XGMX
C
      COMMON /LEQV1/ DET1, COND1
      COMMON /CORR1/ DET,  COND, SGNDET
C
      COMMON /WRTO1/ NFIX1, NFIX
      COMMON /WRTO2/ DMOVE1, DMOVE
      COMMON /WRTO3/ COSI1, COSI2, COSI0,COSI3
      COMMON /WRTO4/ EPSC, EPSN
      COMMON /WRTO6/ ITC,IT2
C!!!  COMMON /WRTO7/ VMOVE(25)
      COMMON /WRTO7/ VMOVE(NDIM),VMOVN(NDIM)
C
      DATA MAS1 /' ',' ',' ',' ',' ',' ',' ',' '/,
     A     MAS2 /'+','+','+','+','+','+','+','+'/
C
#if defined(hib)
      IF (IPR .LE. -2) RETURN
          GOTO (100,110,120,130,140,150,160,170,180,190,
     A          200,210,220,230,240), IWRT
 100  CONTINUE
C
C.... IWRT = 1;
C     �E�AT� KOOP��HAT O�EPE�HO� TO�K� HA KP�BO�.
C
      IF (IPR .LT. 0) RETURN
          IF (IPR .LT. 2 .OR. IDIV .EQ. 0) GOTO 102
C
C.... �E�AT� ��C�A �E�EH�� �CXO�HO�O �A�A �O�O�AM.
C
              WRITE (NCAN,4060) IDIV
              WRITE (NCAN,2010)
              WRITE (NCAN,2000)
 102      CONTINUE
          NT = ISTEP + 1
          WRITE (NCAN,1005) NT, (X(I),I=1,NX)
      IF (IPR .LT. 1) GOTO 11
C
C.... �E�AT� �O�O�H�TE��HO� �H�OPMA���.
C
          WRITE (NCAN,1010)
          WRITE (NCAN,1020)
          IF (DABS(DMOVE1) .NE. DABS(DMOVE)) WRITE (NCAN,1030)  DMOVE1,
     A                                        (MAS2(I),I=1,8),  DMOVE
          IF (DABS(DMOVE1) .EQ. DABS(DMOVE)) WRITE (NCAN,1030)  DMOVE1,
     A                                        (MAS1(I),I=1,8),  DMOVE
          IF (NFIX1 .NE. NFIX)               WRITE (NCAN,1040)  NFIX1,
     A                                        (MAS2(I),I=1,8),  NFIX
          IF (NFIX1 .EQ. NFIX)               WRITE (NCAN,1040)  NFIX1,
     A                                        (MAS1(I),I=1,8),  NFIX
          WRITE (NCAN,1050) IT
          WRITE (NCAN,4090)
          WRITE (NCAN,1052) EPSN
          WRITE (NCAN,1053) EPSC
          WRITE (NCAN,4090)
                               WRITE (NCAN,5000) DET1
          IF (NFIX .NE. NFIX1) WRITE (NCAN,5020) DET
                               WRITE (NCAN,5010) COND1
          IF (NFIX .NE. NFIX1) WRITE (NCAN,5030) COND
          WRITE (NCAN,4090)
          IF (IPR .LT. 2) GOTO 10
              WRITE (NCAN,1060)
              WRITE (NCAN,1070) COSI1
              WRITE (NCAN,1080) COSI0
              WRITE (NCAN,1090) COSI2
              WRITE (NCAN,1095) (VMOVE(I),I=1,NX)
 10       CONTINUE
      WRITE (NCAN,2000)
 11   CONTINUE
      RETURN
 110  CONTINUE
C
C.... IWRT = 2;
C     COO��EH�E: HE CO���C� H��TOHOBCK�E �TEPA���
C                (TPE��EMA� TO�HOCT� HE �OCT��H�TA).
C
      IF (IPR .LT. 2)   RETURN
          WRITE (NCAN,2010)
          WRITE (NCAN,2020) ITC
          WRITE (NCAN,4090)
          WRITE (NCAN,1052) EPSN
          WRITE (NCAN,1053) EPSC
          WRITE (NCAN,4090)
          WRITE (NCAN,3080) NFIX
          WRITE (NCAN,5000) DET1
          WRITE (NCAN,5010) COND1
          WRITE (NCAN,4090)
          WRITE (NCAN,2030) DMOVE
          WRITE (NCAN,4090)
          WRITE (NCAN,5040)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          RETURN
 120  CONTINUE
C
C.... IWRT = 3;
C     COO��EH�E: HE CO���C� H��TOHOBCK�E �TEPA���
C                (B�PO��EHA MATP��A �ACTH�X �PO��BO�H�X).
C
      IF (IPR .LT. 2)   RETURN
          IT = IT - 1
          WRITE (NCAN,2010)
          WRITE (NCAN,3070)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          WRITE (NCAN,1050) IT
          WRITE (NCAN,4090)
          WRITE (NCAN,5000) DET1
          WRITE (NCAN,5010) COND1
          WRITE (NCAN,4090)
          WRITE (NCAN,2030) DMOVE
          WRITE (NCAN,3080) NFIX
          WRITE (NCAN,3089)
          DO 125 I=1,NX
             WRITE (NCAN,3090)  (DFUN(I,J), J=1,NX)
 125      CONTINUE
          RETURN
 130  CONTINUE
C
C.... IWRT = 4;
C     COO��EH�E: HE CO���C� H��TOHOBCK�E �TEPA���
C                (HE B���C�EH� �HA�EH�� ��HK��� B O�EPE�HO�
C                 TO�KE H��TOHOBCKO�O �PO�HO�A).
C
      IF (IPR .LT. 2)   RETURN
          IT = IT - 1
          WRITE (NCAN,2010)
          WRITE (NCAN,4000)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          WRITE (NCAN,1050) IT
          WRITE (NCAN,2030) DMOVE
          RETURN
 140  CONTINUE
C
C.... IWRT = 5;
C     COO��EH�E: HE CO���C� H��TOHOBCK�E �TEPA���
C                (HE B���C�EH� �HA�EH�� �ACTH�X �PO��BO�H�X B O�EPE�HO�
C                 TO�KE H��TOHOBCKO�O �PO�HO�A).
C
      IF (IPR .LT. 2)   RETURN
          IT = IT - 1
          WRITE (NCAN,2010)
          WRITE (NCAN,4020)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          WRITE (NCAN,1050) IT
          WRITE (NCAN,2030) DMOVE
          RETURN
 150  CONTINUE
C
C.... IWRT = 6;
C     COO��EH�E: HE HA��EH KACATE��H�� BEKTOP
C                (B�PO��EHA MATP��A �ACTH�X �PO��BO�H�X).
C
      IF (IPR .LT. 2)   RETURN
          WRITE (NCAN,2010)
          WRITE (NCAN,4030)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          WRITE (NCAN,4090)
          WRITE (NCAN,5000) DET1
          WRITE (NCAN,5010) COND1
          WRITE (NCAN,4090)
          WRITE (NCAN,2030) DMOVE
          WRITE (NCAN,3080) NFIX
          WRITE (NCAN,3089)
          DO 155 I=1,NX
             WRITE (NCAN,3090)  (DFUN(I,J), J=1,NX)
 155      CONTINUE
          RETURN
 160  CONTINUE
C
C.... IWRT = 7;
C     COO��EH�E: HE HA��EH KACATE��H�� BEKTOP
C                (HE B���C�EH� ��HK��� B TO�KE HA KP�BO�).
C
      IF (IPR .LT. 2)   RETURN
          WRITE (NCAN,2010)
          WRITE (NCAN,4040)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          WRITE (NCAN,2030) DMOVE
          RETURN
 170  CONTINUE
C
C.... IWRT = 8;
C     COO��EH�E: HE HA��EH KACATE��H�� BEKTOP
C                (HE B���C�EH� �HA�EH�� �ACTH�X �PO��BO�H�X
C                 B TO�KE HA KP�BO�).
C
      IF (IPR .LT. 2)   RETURN
          WRITE (NCAN,2010)
          WRITE (NCAN,4050)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          WRITE (NCAN,2030) DMOVE
          RETURN
 180  CONTINUE
C
C.... IWRT = 9;
C     COO��EH�E:�TO�HEHHA� TO�KA HE ����KA K TO�KE-�PO�HO��
C               (�A� �E��TC� �O�O�AM).
C
      IF (IPR .LT. 2) RETURN
          WRITE (NCAN,2010)
          WRITE (NCAN,2060)
          WRITE (NCAN,2070) COSI0
          WRITE (NCAN,2080) COSI1
          WRITE (NCAN,1050) IT
          WRITE (NCAN,2030) DMOVE
          WRITE (NCAN,3050)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          RETURN
 190  CONTINUE
C
C.... IWRT = 10;
C     COO��EH�E: �PO�HO� �O KACATE��HO� B��E� �A �PAH���.
C
      IF (IPR .LT. 2) RETURN
          WRITE (NCAN,2010)
          WRITE (NCAN,2090)
          WRITE (NCAN,3000)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          WRITE (NCAN,2030) DMOVE1
          WRITE (NCAN,2040) DMOVE
          WRITE (NCAN,2010)
          WRITE (NCAN,2000)
          RETURN
 200  CONTINUE
C
C.... IWRT = 11;
C     COO��EH�E: �PO�HO� �O H��TOH� B��E� �A �PAH���.
C
      IF (IPR .LT. 2) RETURN
          WRITE (NCAN,2010)
          WRITE (NCAN,3020)
          WRITE (NCAN,3021)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          WRITE (NCAN,2010)
          WRITE (NCAN,2000)
          RETURN
 210  CONTINUE
C
C.... IWRT = 12
C     COO��EH�E: �TO�H�ETC� TO�KA �AM�KAH��.
C
      IF (IPR .LT. 2) RETURN
          WRITE (NCAN,2010)
          WRITE (NCAN,3030)
          WRITE (NCAN,3040)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          RETURN
C
C.... IWRT = 13
C
 220  CONTINUE
      IF (IPR .LT. 2) RETURN
          WRITE (NCAN,3050)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          RETURN
C
C.... IWRT = 14
C
 230  CONTINUE
      IF (IPR .LT. 2) RETURN
          WRITE (NCAN,3060)
          WRITE (NCAN,3010) (X(I),I=1,NX)
          WRITE (NCAN,2010)
          WRITE (NCAN,2000)
          RETURN
 240  CONTINUE
C
C.... IWRT = 15;
C     COO��EH�E:�O���OBATE�EM �P�H�TO PE�EH�E �EPEC��TAT� TO�K�
C               (�A� �E��TC� �O�O�AM).
C
      IF (IPR .LT. 2) RETURN
          WRITE (NCAN,2010)
          WRITE (NCAN,5050)
          WRITE (NCAN,3050)
          WRITE (NCAN,3010) (X(I),I=1,NX)
#endif
          RETURN
C
C.... C��COK �OPMATOB.
C
#if defined(hib)
 1005 FORMAT
     A('   NT=',I6,' X=',7(G14.5,1X)/10(15X,7(G14.5,1X)/))
C
 1010 FORMAT
     A(/40X,'�HA�EH�� KOHTPO��H�X �APAMETPOB:')
C
 1020 FORMAT
     A(30X,'TEK���E �HA�EH��',15X,'�PO�HO��P�EM�E �HA�EH��'/)
C
 1030 FORMAT
     A('   �A� B�O�� KP�BO�:            ',G14.5,8X,8A1,6X,G14.5)
C
 1040 FORMAT
     A('   HOMEP ��KC�POBAHHO� �EPEMEHHO�:  ',I6,12X,8A1,6X,I6)
C
 1050 FORMAT
     A('   �PO�E�AHO H��TOHOBCK�X �TEPA���: ',I6)
C
 1052 FORMAT
     A('   �OCT��H�TA� TO�HOCT�:            ',G15.6)
C
 1053 FORMAT
     A('   TPE��EMA�   TO�HOCT�:            ',G15.6)
C
 1060 FORMAT
     A('   �HA�EH�� KOC�H�CA ���A ME��� KACATE��HO� � XOP�O�:')
C
 1070 FORMAT
     A('   M�H�MA��HOE KOHTPO��HOE �HA�EH�E :',G14.5)
C
 1080 FORMAT
     A('   TEK��EE �HA�EH�E KOC�H�CA ���A   :',G14.5)
C
 1090 FORMAT
     A('   MAKC�MA��HOE KOHTPO��HOE �HA�EH�E:',G14.5)
C
 1095 FORMAT
     A(/'   KOOP��HAT� KACATE��HO�O BEKTOPA:'/
     B 10(2X,7(G14.5,1X)/))
C
 2000 FORMAT
     A(1X,120('.'))
C
 2010 FORMAT
     A(1X,120('-'))
C
 2020 FORMAT
     A(/'   �PO�E�AHO MAKC�MA��HOE ��C�O H��TOHOBCK�X �TEPA���:',I6/
     B  '   ---------------------------------------------------')
C
 2030 FORMAT
     A('   BE����HA �PE�BAP�TE��HO�O �A�A:    ',G14.5)
C
 2040 FORMAT
     A('   BE����HA �TO�HEHHO�O �A�A :        ',G14.5)
C
 2050 FORMAT
     A('   ��C�O �E�EH�� �PE�BAP�TE��HO�O �A�A �O�O�AM:',I6)
C
 2060 FORMAT
     A(/'   �TO�HEHHA� TO�KA HE ����KA K TO�KE-�PO�HO��.'/
     B  '   --------------------------------------------')
C
 2070 FORMAT
     A('   TEK��EE     �HA�EH�E KOC�H�CA ���A:',G14.5)
C
 2080 FORMAT
     A('   KOHTPO��HOE �HA�EH�E KOC�H�CA ���A:',G14.5)
C
 2090 FORMAT
     A(/'    �PO�HO� �O KACATE��HO� B��E� �A �PAH���.'/
     B  '    ----------------------------------------')
C
 3000 FORMAT
     A('    KOOP��HAT� �PO�HO�A �PAH��HO� TO�K� :')
C
 3010 FORMAT
     A(/10(2X,7(G14.5,1X)/))
C
 3020 FORMAT
     A(/'   �PO�HO� �O H��TOH� B��E� �A �PAH���.'/
     B  '   ------------------------------------')
C
 3021 FORMAT
     A ('   KOOP��HAT� TO�K� �PO�HO�A:')
C
 3030 FORMAT
     A(/'   �TO�H�ETC� TO�KA �AM�KAH��.'/
     B  '   ---------------------------')
C
 3040 FORMAT
     A(/'   KOOP��HAT� �PO�HO�A TO�K� �AM�KAH��:')
C
 3050 FORMAT
     A(/'   KOOP��HAT� �TO�HEHHO� TO�K�:        ')
C
 3060 FORMAT
     A(/'   KOOP��HAT� �EPBO� TO�K� HA KP�BO�:  ')
C
 3070 FORMAT
     A(/'   HE CO���C� H��TOHOBCK�E �TEPA���- B�PO��EHA MATP��A'
     B /'   �ACTH�X �PO��BO�H�X B TO�KE C KOOP��HATAM�:')
C
 3080 FORMAT
     A('   HOMEP ��KC�POBAHHO� �EPEMEHHO�: ',I6)
C
 3089 FORMAT
     A(/'   MATP��A �ACTH�X �PO��BO�H�X  DF(I)/DX(J):')
C
 3090 FORMAT
     A (/10(2X,7(G14.5,1X)/))
C
 4000 FORMAT
     A(/'   HE CO���C� H��TOHOBCK�E �TEPA���- HE B���C�EH� �HA�EH��'
     B /'   ��HK��� B TO�KE H��TOHOBCKO�O �PO�HO�A C KOOP��HATAM�:')
C
 4020 FORMAT
     A(/'   HE CO���C� H��TOHOBCK�E �TEPA���- HE B���C�EH� �HA�EH��'
     B /'   �ACTH�X �PO��BO�H�X B TO�KE H��TOHOBCKO�O �PO�HO�A:   ')
 4030 FORMAT
     A(/'   HE HA��EH KACATE��H�� BEKTOP- MATP��A �ACTH�X �PO��BO�H�X'
     B /'   B�PO��EHA B TO�KE HA KP�BO� C KOOP��HATAM�:')
C
 4040 FORMAT
     A(/'   HE HA��EH KACATE��H�� BEKTOP- HE B���C�EH� ��HK���'
     B /'   B TO�KE HA KP�BO� C KOOP��HATAM�:')
C
 4050 FORMAT
     A(/'   HE B���C�EH KACATE��H�� BEKTOP- HE B���C�EH� �HA�EH��'
     B /'   �ACTH�X �PO��BO�H�X B TO�KE HA KP�BO�:')
C
 4060 FORMAT
     A(/   '   C�MMAPHOE ��C�O �E�EH�� �A�A �O�O�AM: ',I6/
     B     '   ------------------------------------  ')
C
 4090 FORMAT
     A(1X)
C
 5000 FORMAT
     A('   TEK��EE �HA�EH�E O�PE�E��TE��:   ',G15.6)
C
 5010 FORMAT
     A('   O�EHKA O��C�OB�EHHOCT�:          ',G15.6)
C
 5020 FORMAT
     A('   (HOBOE �HA�EH�E O�PE�E��TE��:    ',G15.6,')')
C
 5030 FORMAT
     A('   (HOBA� O�EHKA O��C�OB�EHHOCT�:   ',G15.6,')')
C
 5040 FORMAT
     A('   KOOP��HAT� �OC�E�HE� TO�K�:      ')
C
 5050 FORMAT
     A(/'   �O���OBATE�EM �P�H�TO PE�EH�E �EPEC��TAT� TO�K�'
     B /'            (�A� �E��TC� �O�O�AM)'
     C /'   -----------------------------------------------')
#endif
      END
