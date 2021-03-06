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

        SUBROUTINE DERN (NFUN,NX,NDX,X0,FX0,DF,FUN,IER,ISTOP)
C       3.05.90�.
C       01.09.90
C       13.11.92
C       15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA DERN �PE�HA�HA�EHA ��� ��C�EHHO�O ����EPEH��POBAH��    *
C*    BEKTOP-��HK��� F(X) �O �A�AHHOM� AP��MEHT�.                      *
C*                                                                     *
C*    �APAMETP�:                                                       *
C*    =========                                                        *
C*    NFUN - ��C�O KOM�OHEHT BEKTOP-��HK��� F=(F(1),...,F(NFUN)).      *
C*    NX   - ��C�O AP��MEHTOB BEKTOP-��HK���:(X(1),...,X(NX)).         *
C*    NDX  - HOMEP AP��MEHTA, �O KOTOPOM� B���C���TC� �PO��BO�H�E.     *
C*    X0   - KOOP��HAT� TO�K�, B KOTOPO� B���C���TC� �PO��BO�H�E       *
C*           (X0: X0(1),...,X0(NX)).                                   *
C*    FX0  - BEKTOP �HA�EH�� KOM�OHEHT BEKTOP-��HK��� B TO�KE X0.      *
C*    DF   - BEKTOP-�PO��BO�HA� OT BEKTOP-��HK���.                     *
C*    FUN  - �M� �PO�PAMM� B���C�EH�� BEKTOP-��HK���.                  *
C* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . *
C*                                                                     *
C*    B���BAEM�E �PO�PAMM�: FUN.                                       *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*    �P� ��MEHEH�� MAKC�MA��HO�O ��C�A �EPEMEHH�X NDIM HEO�XO��MO     *
C*    ��MEH�T� PA�MEPHOCT� MACC�BA FX1(NDIM-1).                        *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
        IMPLICIT REAL*8 (A-H,O-Z)
        IMPLICIT INTEGER*2 (I-N)
	include 'beetlebf.dim'
        EXTERNAL FUN
        DIMENSION X0(1), FX0(1), DF(1), FX1(NDIM1)
        COMMON /DERN1/ DX0
C
        ISTOP   = 0
        IER     = 0
        R       = X0(NDX)
        DXSC    = DX0*(1.D0+DABS(X0(NDX)))
        X0(NDX) = X0(NDX) + DXSC
C
        CALL FUN (X0,FX1,IER,ISTOP)
        IF (ISTOP .NE. 0) RETURN
        IF (IER   .NE. 0) RETURN
C
        DO 100 I = 1,NFUN
          DF(I) = (FX1(I) - FX0(I))/DXSC
100     CONTINUE
        X0(NDX) = R
        RETURN
        END
