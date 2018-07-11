/****************************************************
**                                                 **
** Program: Generic.c                              **
** Author : Gene Backlin                           **
**                                                 **
** Address: CompuServe ID 70401,1574               **
**                                                 **
** Purpose: To give the User as well as the        **
**          Programmer new to OS/2's Presentation  **
**          Manager interface, a generic shell, to **
**          see how the pieces are fit together.   **
**                                                 **
** Written: 12/25/92                               **
** Revised: 12/25/92                               **
**    Version 1.00 - 12/25/92                      **
**       Original Version                          **
**                                                 **
****************************************************/
#define INCL_32

#define INCL_WIN
#define INCL_GPI

#define INCL_WINHELP                            // Include IPF Header File

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>

#include "generic.h"

/****************************************************
*                                                   *
*  Function Prototypes                              *
*                                                   *
****************************************************/
MRESULT EXPENTRY ClientWndProc   (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AboutDlgProc    (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY GenericDlgProc  (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY CenterDlg       (HWND hwnd);

/****************************************************
*                                                   *
*  Required IPF Structure Declarations              *
*                                                   *
****************************************************/
HELPINIT    hmiHelpData;                        // Help Initialization Structure
HWND        hwndHelpInstance;                   // Handle to the Help Window

/****************************************************
*                                                   *
*  Main Function                                    *
*                                                   *
****************************************************/
INT main(INT argc, CHAR *argv, CHAR *envp)
{
   HAB     hab;                                 // Handle to Application  
   HMQ     hmq;                                 // Hold the Application's Message Queue
   QMSG    qmsg;                                // The actual Queue Message             

   HWND    hwndFrame;                           // Presentation Manager 
   HWND    hwndClient;                          //    Window Handles
   HWND    hwndMenu;                            // Menu Handle
   HWND    hwndTemp;                            // Temporary Menu Handle

   SWCNTRL SwData;                              // Switch control data block
   HSWITCH hSwitch;                             // Switch entry handle

   ULONG flFrameFlags =                         // Set the Frame-window creation Flags
      FCF_MENU          |                       // Application Menu
      FCF_TITLEBAR      |                       // Application Title
      FCF_SIZEBORDER    |                       // Application Size Border
      FCF_MINMAX        |                       // Minimize and Maximum Buttons
      FCF_SYSMENU       |                       // Application System Menu
      FCF_SHELLPOSITION	|                       // System default size and position
      FCF_ICON          |                       // System default size and position
      FCF_TASKLIST;		                        // Add name to TaskList

   hab = WinInitialize(0);                      // Register the Application
   hmq = WinCreateMsgQueue(hab, 0);             // Create the Message Queue

   if (!WinRegisterClass(                       // Register the window class
      hab,                                      // Handle to application
      CLIENT_CLASS,	                           // Name of the Class
      ClientWndProc,	                           // Window procedure name
      CS_SIZEREDRAW,	                           // Window style
      0))                                       // Extra window words
         return (FALSE);                        // Terminate if Unsuccessful

/****************************************************
*                                                   *
*  IPF Initialization Structure                     *
*                                                   *
****************************************************/
   hmiHelpData.cb                      = sizeof(HELPINIT);	// size of initialization structure
   hmiHelpData.ulReturnCode            = 0;			// store HM return code from init
   hmiHelpData.pszTutorialName         = 0;			// no tutorial program
   hmiHelpData.phtHelpTable            = (PVOID)
                  (0xffff0000 | MAIN_HELPTABLE);   // help table defined in RC file
   hmiHelpData.hmodAccelActionBarModule= 0;
   hmiHelpData.idAccelTable            = 0;
   hmiHelpData.idActionBar             = 0;
   hmiHelpData.pszHelpWindowTitle      = TITLE;
   hmiHelpData.hmodHelpTableModule     = 0; 			// Help Table not in a DLL
   hmiHelpData.fShowPanelId            = 0;			// Help Panels ID is not displayed
   hmiHelpData.pszHelpLibraryName      = HELPFILE;	// Library with help panels

/****************************************************
*                                                   *
*  Create Instance of IPF                           *
*                                                   *
****************************************************/
   hwndHelpInstance =                            // Pass Anchor Block handle and address 
      WinCreateHelpInstance(hab, &hmiHelpData);  // of IPF initialization structure

   if(!hwndHelpInstance)
      {
      WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                   (PSZ) "Help Not Available",
                   (PSZ) "Help Creation Error",
                   1,
                   MB_OK | MB_APPLMODAL | MB_MOVEABLE);
      }
   else
      {
      if(hmiHelpData.ulReturnCode)
         {
         WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                      (PSZ) "Help NotAvailable",
                      (PSZ) "Help Creation Error",
                      1,
                      MB_OK | MB_APPLMODAL | MB_MOVEABLE);
         WinDestroyHelpInstance(hwndHelpInstance);
         }
      }

/****************************************************
*                                                   *
*  Create a Top-Level frame window with a client    *
*  window that belongs to the window class          *
*  CLIENT_CLASS. (see generic.h)                    *
*                                                   *
****************************************************/
   hwndFrame = WinCreateStdWindow(              // Create the Frame Window
                      HWND_DESKTOP,	            // Parent is the Desktop
                      0L,                       // Don't make frame window visible
                      &flFrameFlags,            // Frame Controls
                      CLIENT_CLASS,	            // Window class for client
                      TITLE,                    // Window Title
                      0L,                       // Don't make client window visible
                      0,                        // Resources in application model
                      ID_MENU_RESOURCE,         // Resource identifier
                      &hwndClient);	            // Pointer to client window handle
   if (!hwndFrame)
      return (FALSE);

   WinSetWindowPos(
      hwndFrame,                                // Shows and activates frame
      HWND_TOP,                                 // Put the window on top
      55,                                       // Positon x
      350,                                      // Positon y
      525,                                      // New width
      75,                                       // New height
      SWP_SIZE      |                           // Change the size
      SWP_MOVE      |                           // Move the window
      SWP_ACTIVATE  |                           // Make it the active window
      SWP_SHOW                                  // Make it visible
      );

   hwndMenu =
      WinWindowFromID( hwndFrame, FID_MENU);    // Get the Menu handle

   SwData.hwnd          = hwndFrame;            // Set frame Window handle
   SwData.hwndIcon      = 0;                    // Use the default Icon
   SwData.hprog;                                // Use default program handle
   SwData.idProcess     = 0;                    // Use current process id
   SwData.idSession     = 0;                    // Use current session id
   SwData.uchVisibility	= SWL_VISIBLE;          // Make Visible
   SwData.fbJump        = SWL_JUMPABLE;         // Make Jumpable via Alt+Esc
   SwData.szSwtitle[0]	= '\0';                 // Use default Title Test

   hSwitch = WinAddSwitchEntry(&SwData);        // Add task manager entry

/****************************************************
*                                                   *
*  Associate Instance of IPF                        *
*                                                   *
****************************************************/
   if(hwndHelpInstance)
      WinAssociateHelpInstance(hwndHelpInstance, hwndFrame);

/****************************************************
*                                                   *
*  Start the main message loop. Get Messages from   *
*  the queue and dispatch them to the appropriate   *
*  windows.                                         *
*                                                   *
****************************************************/
   while(WinGetMsg(hab, &qmsg, 0, 0, 0))        // Loop until WM_QUIT
      WinDispatchMsg(hab, &qmsg);

/****************************************************
*                                                   *
*  Main Loop has terminated. Destroy all windows and*
*  the message queue; then terminate the application*
*                                                   *
****************************************************/
   if(hwndHelpInstance)
      WinDestroyHelpInstance(hwndHelpInstance);	// Destroy Help Instance

   WinDestroyWindow(hwndFrame);                 // Destroy main window
   WinDestroyMsgQueue(hmq);                     // Destroy message queue
   WinTerminate(hab);                           // Deregister application

   return 0;
}

/****************************************************
*                                                   *
*  Client Window Procedure                          *
*                                                   *
****************************************************/
MRESULT EXPENTRY ClientWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   RECTL   rcl;                                 // Window rectangle
   HPS     hps;                                 // Handle to the Presentation space
   UCHAR   szMsg[40];                           // Standared Text Message
   UCHAR   szTitle[20];                         // Title...
   ULONG   rc;                                  // Status return code

  switch(msg)
   {
   case WM_CREATE:
      WinSendMsg(                               // is just the
         hwnd,                                  // About Box
         WM_COMMAND,                            // Credit
         (MPARAM) IDM_HEL_ABOUT,	               // Display.
         0L);
      break;

    case WM_ERASEBACKGROUND:
      return (MRESULT)(TRUE);

    case WM_CHAR:
      return (MRESULT)(TRUE);

    case WM_PAINT:
      hps = WinBeginPaint(hwnd, 0, &rcl);
      WinEndPaint(hps);
      break;

    case WM_COMMAND:
      switch (SHORT1FROMMP(mp1))
        {
        case IDM_DIS_GENERIC:
          rc = WinDlgBox(
                  HWND_DESKTOP,                 // Desktop is parent
                  hwnd,                         // Current window is owner
                  GenericDlgProc,               // Entry point of dialog proc.
                  0,                            // Resource is in EXE
                  IDD_DIS_GENERIC,              // Dialog resource identifier
                  (PVOID)NULL);                 // Pointer to initialization dat
          return 0;

        case IDM_HEL_ABOUT:                     // Program Credits
          rc = WinDlgBox(
                  HWND_DESKTOP,                 // Desktop is parent
                  hwnd,                         // Current window is owner
                  AboutDlgProc,                 // Entry point of dialog proc.
                  0,                            // Resource is in EXE
                  IDD_ABOUTBOX,                 // Dialog resource identifier
                  (PVOID)NULL);                 // Pointer to initialization dat
          return 0;

        case IDM_EXIT:
          WinSendMsg(hwnd, WM_CLOSE, 0, 0);     // Exit the Program
          return 0;

        case IDM_HELP_FOR_HELP:
          if(hwndHelpInstance)
            WinSendMsg(hwndHelpInstance, HM_DISPLAY_HELP, 0L, 0L);
          break;

        default:
          return WinDefWindowProc(hwnd, msg, mp1, mp2);
        }
      break;


    case HM_ERROR:
      if((hwndHelpInstance && (ULONG) mp1) == HMERR_NO_MEMORY)
        {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                     (PSZ) "Help Terminated Due to Error",
                     (PSZ) "Help Error",
                     1,
                     MB_OK | MB_APPLMODAL | MB_MOVEABLE);
          WinDestroyHelpInstance(hwndHelpInstance);
        }
      else
        {
          WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                     (PSZ) "Help Error Occurred",
                     (PSZ) "Help Error",
                     1,
                     MB_OK | MB_APPLMODAL | MB_MOVEABLE);
        }
      break;

    case WM_CLOSE:
      /*
       * This is the place to put your termination routines
       */

      WinPostMsg( hwnd, WM_QUIT, 0L, 0L );	// Cause termination
      break;

    default:
      return WinDefWindowProc(hwnd, msg, mp1, mp2); 
    }
  return (MRESULT)FALSE;
}

/****************************************************
*                                                   *
*  About Window Procedure                           *
*                                                   *
****************************************************/
MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch(msg)
    {
    case WM_INITDLG:
      CenterDlg(hwnd);
      return 0;

    case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )             // Extract the command value
        {
        case DID_OK:
          WinDismissDlg( hwnd, DID_OK );
        break;

        case DID_CANCEL:
          WinDismissDlg( hwnd, DID_CANCEL );
        break;
        }
		/* no matter what the command, close the dialog */
      WinDismissDlg(hwnd, TRUE);
      break;

    default:
      return(WinDefDlgProc(hwnd, msg, mp1, mp2));
      break;
    }
  return(MPVOID);
}   /* AboutDlgProc() */



/****************************************************
*                                                   *
*  Generic Window Procedure                         *
*                                                   *
****************************************************/
MRESULT EXPENTRY GenericDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch(msg)
    {
    case WM_INITDLG:
      CenterDlg(hwnd);
      return 0;

    case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )             // Extract the command value
        {
        case DID_OK:
           WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                     (PSZ) "Ok Button was Pressed",
                     (PSZ) "Generic Dlg Proc",
                     1,
                     MB_OK | MB_APPLMODAL | MB_MOVEABLE);
          WinDismissDlg( hwnd, DID_OK );
        break;

        case DID_CANCEL:
           WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                     (PSZ) "Cancel Button was Pressed",
                     (PSZ) "Generic Dlg Proc",
                     1,
                     MB_OK | MB_APPLMODAL | MB_MOVEABLE);
          WinDismissDlg( hwnd, DID_CANCEL );
        break;
        }

    default:
      return(WinDefDlgProc(hwnd, msg, mp1, mp2));
    }
  return(MPVOID);
}   /* GenericDlgProc() */

/****************************************************
*                                                   *
*  Center Dialog Box Procedure                      *
*                                                   *
****************************************************/
MRESULT EXPENTRY CenterDlg(HWND hwnd)
{
   RECTL rclScreen;
   RECTL rclDialog;
   LONG  sWidth, sHeight, sBLCx, sBLCy;
   
   WinQueryWindowRect(HWND_DESKTOP, &rclScreen);
   WinQueryWindowRect(hwnd, &rclDialog);
   
   sWidth  = (LONG) (rclDialog.xRight - rclDialog.xLeft);
   sHeight = (LONG) (rclDialog.yTop   - rclDialog.yBottom);
   sBLCx   = ((LONG) rclScreen.xRight - sWidth)  / 2;
   sBLCy   = ((LONG) rclScreen.yTop   - sHeight) / 2;
   
   WinSetWindowPos(
      hwnd,                                    // Activates frame
      HWND_TOP,                                // Put the window on top
      sBLCx,                                   // Positon x
      sBLCy,                                   // Positon y
      0,                                       // New width
      0,                                       // New height
      SWP_MOVE);                               // Move the window
                                               
   return 0;                                   
}                                              
