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

      SUBROUTINE NEWTN (NX,NDIMM,NG,NFIX,NCAN,X0,EPS,EPSN,IT,IPR,FUN,
     A                  DFDX,DFUN,IER,ISTOP)
C     3.05.90�.
c Rev.29.07.92 
C     18.11.92
C     15.06.94
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*                                                                     *
C*    �PO�PAMMA NEWTN �PE�HA�HA�EHA ���  �TO�HEH�� PE�EH�� C�CTEM�     *
C*    HE��HE�H�X YPABHEH��  F(X) = 0  METO�OM H��TOHA, �P� ��KC�POBAH- *
C*    HOM �HA�EH�� �EPEMEHHO�  X(NFIX) = X0(NFIX).                     *
C*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*
C*    �APAMETP�:                                                       *
C*    =========                                                        *
C*    NX   - ��C�O �EPEMEHH�X;                                         *
C*    NDIM - �A�B�EHHA� CTPO�HA� PA�MEPHOCT� MATP��� DFUN;             *
C*    NG   - HOMEP B��E�EHHO� �EPEMEHHO�;                              *
C*    NFIX - HOMEP ��KC�POBAHHO� �EPEMEHHO�;                           *
C*    X0   - HA BXO�E �PO�PAMM� - HA�A��HOE �P�����EH�E K PE�EH��;     *
C*           HA B�XO�E - �TO�HEHHOE �P�����EH�E K PE�EH�� C �A�AHHO�   *
C*           TO�HOCT��.                                                *
C*    EPS  - TO�HOCT� METO�A;                                          *
C*    EPSN - HA B�XO�E PEA��HO �OCT��H�TA� TO�HOCT�.                   *
C*    IT   - HA BXO�E �PO�PAMM� - MAKC�MA��HO �O��CT�MOE ��C�O �TEPA���;
C*           HA B�XO�E - ��C�O C�E�AHH�X �TEPA���;                     *
C*    IPR  - �APAMETP ��PAB�EH�� �E�AT�� COO��EH��-�PO�HO� �O H��TOH�  *
C*           B��E� �A �PAH���;                                         *
C*    FUN  - �M� �O��PO�PAMM� B���C�EH�� �PAB�X �ACTE�;                *
C*    DFDX - �M� �PO�PAMM� B���C�EH�� MATP��� �ACTH�X �PO��BO�H�X;     *
C*    DFUN - �B�MEPH�� MACC�B PA�MEPHOCT� NDIM*NX, CO�EP�A��� MATP���  *
C*           �EPB�X �ACTH�X �PO��BO�H�X.                               *
C*    IER  - �APAMETP O���K�:                                          *
C*             0-�TEPA��� �C�E�HO CO���C� C TO�HOCT�� EPS;             *
C*            -1-TPE��EMA� TO�HOCT� HE �OCT��H�TA �A ITC �TEPA���.     *
C*            -2-HA O�EPE�HO� �TEPA��� O�PE�E��TE�� ��HE�HO� C�CTEM� =0;
C*            -3-HE B���C�EH� �HA�EH�� ��HK��� B H��TOHOBCKOM �PO�HO�E;*
C*            -4-HE B���C�EHA MATP��A �ACTH�X �PO��BO�H�X;             *
C*. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .*
C*           BMAS - BC�OMO�ATE��H�� PA�O��� MACC�B.                    *
C*                                                                     *
C*           B���BAEM�E �O��PO�PAMM�:                                  *
C*              DERN, FUN, JACOB.                                      *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C*    �P� ��MEHEH�� MAKC�MA��HO�O ��C�A (NDIM) �EPEMEHH�X HEO�XO��MO   *
C*    ��MEH�T� PA�MEPHOCT� MACC�BOB:   A(NDIM,NDIM), BMAS(NDIM),       *
C*                                     ARR1(NDIM,NDIM)                 *
C* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*2 (I-N)
      include 'beetlebf.dim'
      EXTERNAL FUN, DFDX
      DIMENSION X0(1), DFUN(NDIMM,NX), BMAS(NDIM),DFNORM(NDIM)
C
      COMMON /CNWT1/ IPNT1
C
      COMMON /ADD6A/ XGMN, XGMX
      COMMON /ADD7A/ EPSNWT
      COMMON /ADD7B/ ITNWT
C
      COMMON /ALL3/  IBND
C
      COMMON /WORKBE/ ARR1(NDIM,NDIM), A(NDIM,NDIM)
C
      COMMON /MESSG/ MESSAG
C
      ISTOP = 0
      ITMAX = IT
      IT=0
      NFUN  = NX - 1
      EPSN  = 100.D0*EPSNWT
      DO 99 J=1,NX
         DFUN(NX,J) = 0.D0
 99   CONTINUE
      DFUN(NX,NFIX) = 1.D0
      XREM          = X0(NFIX)

C.... �O��PAB�M ��KC�POBAHH�� �EPMEHH��.
C
c         X0(NFIX) = XREM
C
C.... B���C��EM �HA�EH�� ��HK��� FUN B TO�KE X0;
C     B���C�EHH�E �HA�EH�� PA�ME�AEM B MACC�BE BMAS.
C
        IER = 0
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1021
 1021 FORMAT(/'    Determining system evaluation starting...')
      ENDIF
#endif
C***E

        CALL FUN (X0,BMAS,IER,ISTOP)

C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1022,IER,ISTOP
 1022 FORMAT(/'    Determining system evaluation completed with',
     *           ' IER=',I3,'  ISTOP=',I3)
      PRINT 1023,(BMAS(I),I=1,NX-1)
 1023 FORMAT('     Function values (BMAS)=',
     *     /('         ',3G23.15)) 
      ENDIF
#endif
C***E
        IF (ISTOP .NE. 0) RETURN
        IF (IER .NE. 0) THEN
            IER = -3
            RETURN
        ENDIF

      ANORMF=0.D0
      DO I=1,NX-1
        IF (ANORMF.LT.DABS(BMAS(I))) ANORMF=DABS(BMAS(I))
      ENDDO

        CALL SHOWIT(IT,EPSN,ANORMF)

      IF (IPNT1.EQ.0 .AND. ANORMF.GT.100.D0) THEN
            IER= -7
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1025
 1025 FORMAT(/'    Function values too large: IER=-7')
      ENDIF
#endif
C***E
            RETURN
      ENDIF


C
C.... OP�AH���EM ��K� �O MAKC�MA��HOM� ��C�� �O��CT�M�X �TEPA���.
C
      DO 440 IT = 1,ITMAX
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1011,IT
 1011 FORMAT(//'    Corrector iteration ',I3)
      PRINT 1012,(X0(I),I=1,NX)
 1012 FORMAT(  '    Starting point (X0)=',
     *     /(  '         ',3G23.15)) 
      ENDIF
#endif
C***E
C
      IF (IPNT1 .EQ. 1)                      GOTO 101
      IF (IT.GT.ITNWT) GOTO 105
 101  CONTINUE
C
C.... B���C��EM MATP��� �PO��BO�H�X DFUN.
C
      IER = 0
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1031
 1031 FORMAT(/'    Jacobian evaluation starting...')
      ENDIF
#endif
C***E

      CALL JACOB (NX,NDIMM,X0,FUN,DFDX,DFUN,BMAS,IER,ISTOP)

C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1032,IER,ISTOP
 1032 FORMAT(/'    Jacobian evaluation completed with',
     *           ' IER=',I3,'  ISTOP=',I3)
      PRINT 1033, ((DFUN(I,J),J=1,NX),I=1,NX-1)
 1033 FORMAT('     Jacobian (DFUN)=',
     *     /('         ',3G23.15)) 
      ENDIF
#endif
C***E
      IF (ISTOP .NE. 0) RETURN
      IF (IER   .EQ. 0) GOTO 105
          IER = -4
          RETURN
 105  CONTINUE
C
C.... �OPM�P�EM BEKTOP-CTO��E� (BMAS) � MATP��� (A).
C
         BMAS(NX)  = 0.D0
         DO 120 I=1,NX
            BMAS(I) =-BMAS(I)
	    DFNORM(I)=0.D0
            DO 110 J=1,NX
               A(I,J)=DFUN(I,J)
	       DFNORM(I)=DFNORM(I)+DABS(DFUN(I,J))
 110        CONTINUE
 120     CONTINUE
C
C.... PE�AEM C�CTEM� ��HE�H�X �PABHEH��  A*Y = BMAS;
C     PE�EH�E Y PA�ME�AETC� B MACC�BE BMAS.
C
        IER = 0
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1041
 1041 FORMAT(/'    Solving linear system...')
      ENDIF
#endif
C***E
        CALL LEQV (A,BMAS,NDIMM,NX,IER)
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1042,IER
 1042 FORMAT(/'    Solving completed with IER=',I3)
      PRINT 1043,(BMAS(I),I=1,NX)
 1043 FORMAT('     Solution vector=',
     *     /('         ',3G23.15)) 
      ENDIF
#endif
C***E
        IF (IER .EQ. 0) GOTO 260
C
C.... MATP��A A - B�PO��EHA.
C
           IER =-2
           RETURN
 260    CONTINUE
C
C.... HAXO��M O�EPE�HOE �P�����EH�E.
C
        EPSN = 0.D0
        DO 320 I = 1,NX
           RR=X0(I)
           X0(I) = RR + BMAS(I)
           RR1   = DABS(BMAS(I))/(1.D0+DABS(RR))
	   IF (DABS(X0(I)).LT.1.D-5*RR1) X0(I)=0.D0
           IF(EPSN.LT.RR1) EPSN = RR1
 320    CONTINUE
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1051
 1051 FORMAT(/'    Test of convergence:')
      PRINT 1052,(X0(I),I=1,NX)
 1052 FORMAT('     New point (X0)=',
     *     /('         ',3G23.15)) 
      PRINT 1053,EPSN
 1053 FORMAT('     Accuracy achieved (EPSN)=',G11.4)
      ENDIF
#endif
C***E
        IF (IBND .EQ. 0) GOTO 400
C
C.... COO��EH�E:�PO�HO� �O H��TOH� B��E� �A �O��CT�M�E �PAH���.
C
        IWRT = 11
        IF (X0(NG) .LT. XGMN .OR. X0(NG) .GT. XGMX)
     A      CALL WRTOUT (IWRT,ISTEP,IPR,IT,IDIV,NCAN,NX,NDIMM,X0,DFUN)
C
C.... �CE�EH�E H��TOHOBCKO�O �PO�HO�A �O ���EP��OCKOCT�
C     X(NG) = XGMN (��� X(NG) = XGMX)
C
        IF (X0(NG) .LT. XGMN) X0(NG) = XGMN
        IF (X0(NG) .GT. XGMX) X0(NG) = XGMX
C
C.... �POBEP�EM O�EPE�HOE �P�����EH�E HA TO�HOCT�.
C
 400  CONTINUE
C
C.... �O��PAB�M ��KC�POBAHH�� �EPMEHH��.
C
         X0(NFIX) = XREM
C
C.... B���C��EM �HA�EH�� ��HK��� FUN B TO�KE X0;
C     B���C�EHH�E �HA�EH�� PA�ME�AEM B MACC�BE BMAS.
C
        IER = 0
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1021
c 1021 FORMAT(/'    Determining system evaluation starting...')
      ENDIF
#endif
C***E

        CALL FUN (X0,BMAS,IER,ISTOP)

C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1022,IER,ISTOP
c 1022 FORMAT(/'    Determining system evaluation completed with',
c     *           ' IER=',I3,'  ISTOP=',I3)
      PRINT 1023,(BMAS(I),I=1,NX-1)
c 1023 FORMAT('     Function values (BMAS)=',
c     *     /('         ',3G23.15)) 
      ENDIF
#endif
C***E
        IF (ISTOP .NE. 0) RETURN
        IF (IER .NE. 0) THEN
            IER = -3
            RETURN
        ENDIF

      ANORMF=0.D0
      ANRMF1=0.D0
      DO I=1,NX-1
         R1=DABS(BMAS(I)/(1.D0+DFNORM(I)))
         IF (ANORMF.LT.DABS(BMAS(I))) ANORMF=DABS(BMAS(I))
         IF (ANRMF1.LT.R1) ANRMF1=R1
      ENDDO
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1081,ANORMF,ANRMF1
 1081 FORMAT(/'    Function norms: ANORMF=',G11.4,'  ANRMF1=',G11.4)
      ENDIF
#endif
C***E

        CALL SHOWIT(IT,EPSN,ANORMF)

      IF (IPNT1.EQ.0 .AND. ANORMF.GT.100.D0) THEN
            IER= -7
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1025
c 1025 FORMAT(/'    Function values too large: IER=-7')
      ENDIF
#endif
C***E
            RETURN
      ENDIF



C
        IF (EPSN .GT. EPS .OR. ANORMF.GT.EPS) GOTO 420
            IER = 0
            RETURN
 420    CONTINUE
        IF (IPNT1.EQ.0 .AND. EPSN.GT.10.D0) THEN
            IER= -5
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1061
 1061 FORMAT(/'    Correction too large: IER=-5')
      ENDIF
#endif
C***E
            RETURN
        ENDIF
        IF (IT.GT.2 .AND. EPSN.GT. 10.0D0*EPSNOL) THEN
            IER= -6
C***B
#if defined(hib)
      IF (MESSAG.LE.-6) THEN
      PRINT 1071
 1071 FORMAT(/'    No progress in convergence: IER=-6')
      ENDIF
#endif
C***E
            RETURN
        ENDIF
        EPSNOL=EPSN
 440  CONTINUE
      IT  = ITMAX
      IER =-1
      RETURN
      END
