# IBM Developer's Workframe/2 Make File Creation run at 19:38:38 on 12/26/92

# Make File Creation run in directory:
#   D:\DEV\GENERIC;

.SUFFIXES:

.SUFFIXES: .c .rc .ipf

ALL: GENERIC.EXE \
     GENERIC.HLP \
     GENERIC.RES

GENERIC.EXE:  \
  GENERIC.OBJ \
  GENERIC.RES \
  GENERIC.DEF \
  GENERIC.MAK
   @REM @<<GENERIC.@0
     /CO /PM:PM +
     GENERIC.OBJ
     GENERIC.EXE
     
     
     GENERIC.DEF;
<<
   LINK386.EXE @GENERIC.@0
   RC GENERIC.RES GENERIC.EXE

{.}.rc.res:
   RC -r .\$*.RC

{.}.ipf.hlp:
   IPFC .\$*.IPF

{.}.c.obj:
   ICC.EXE /Sm /Ss /Fl /Ls /Lt"GENERIC - (C)opyright 1992, Gene Backlin" /Lu"A Generic PM Program" /Ti /Gm /C .\$*.c

!include GENERIC.DEP
