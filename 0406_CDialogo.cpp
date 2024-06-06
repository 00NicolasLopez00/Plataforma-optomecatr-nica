/*------------------------------------------
 ABOUT1.C -- About Box Demo Program No. 1
 (c) Charles Petzold, 1998
 ------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "0406_Resource.h"
#include "0402_CSerial.cpp"
#include "0406_CDialogo.h"

#define EXITO  0

CDialogo CDialogo::Caja;


/* Obtiene el nombre del puerto serial y establece comunicacion */
int OpenPort( CSerial& puerto ) {

  TCHAR arduino[] = L"SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_1A86&PID_7523";
  UINT  length    = sizeof( arduino ) / sizeof( TCHAR );
  UINT errorCode = 0;

   if( puerto.OpenRegisteredPort( arduino ) == 0xFFFFFFFF ) {

     // Lectura del encabezado
     errorCode  = 1;
     if( puerto.ReadLine( arduino, length ) == EXITO )
       return 1;
   }

   MessageBox(
     GetDesktopWindow(),
     L"El arduino no ha sido conectado.",
     L"Error de comunicación", MB_OK );
   return 0;
}


// *******************************************************************
// DEFINICION DE LA CLASE CDialogo
// *******************************************************************

/* Mueve el motor */
int  CDialogo::Mover() {

  // Formato del comando
  char  orden[64] = "1,800\r\n";
  float Conv = 80.0/9.0;
  UINT leidos;

  Destino = round(Grados*Conv);
  if(Destino > PasosV){
    Pasos = Destino - PasosV;
    PasosV = Destino;

    UINT  longitud  = sprintf( orden, "1,%d\r\n", Pasos ) + 1;

    // Despliega el mensaje
    HWND hWndChild = GetDlgItem( hDlg, IDC_EDIT1 );
    SendMessage(
      hWndChild, WM_SETTEXT,
      0, (LPARAM) L"Comando en proceso ..." );
    UpdateWindow( hWndChild );

   // Envia el mensaje
    if( Puerto->Send( orden, longitud ) )
      return 0;

    // Lectura del mensaje reflejado
    wchar_t  respuesta[128];
    UINT  leidos;
    if( Puerto->ReadLine( respuesta, leidos ) == EXITO ) {
      respuesta[ leidos - 2 ] = 0;
      SendMessage( hWndChild, WM_SETTEXT, 0, (LPARAM) respuesta );
    }

    return 1;
  } else {
    Pasos = PasosV - Destino;
    PasosV = Destino;
    
      // Envia el mensaje
    if( Puerto->Send( "3,0\r\n", 6 ) )
      return 0;

    // Lectura del mensaje reflejado
    HWND hWndChild = GetDlgItem( hDlg, IDC_EDIT1 );
    wchar_t  respuesta[128];
    if( Puerto->ReadLine( respuesta, leidos ) == EXITO ) {
      respuesta[ leidos - 2 ] = 0;
      SendMessage( hWndChild, WM_SETTEXT, 0, (LPARAM) respuesta );
    }

    UINT  longitud  = sprintf( orden, "1,%d\r\n", Pasos ) + 1;

    // Despliega el mensaje
    SendMessage(
      hWndChild, WM_SETTEXT,
      0, (LPARAM) L"Comando en proceso ..." );
    UpdateWindow( hWndChild );

    // Envia el mensaje
    if( Puerto->Send( orden, longitud ) )
      return 0;

    // Lectura del mensaje reflejado
    if( Puerto->ReadLine( respuesta, leidos ) == EXITO ) {
      respuesta[ leidos - 2 ] = 0;
      SendMessage( hWndChild, WM_SETTEXT, 0, (LPARAM) respuesta );
    }

    if( Puerto->Send( "3,0\r\n", 6 ) )
      return 0;

    // Lectura del mensaje reflejado
    if( Puerto->ReadLine( respuesta, leidos ) == EXITO ) {
      respuesta[ leidos - 2 ] = 0;
      SendMessage( hWndChild, WM_SETTEXT, 0, (LPARAM) respuesta );
    }

    return 0;
  }
}

int CDialogo::OnKillFocus(){
  
  // Lectura del mensaje
  wchar_t texto[128];
  SendDlgItemMessage(
    hDlg, IDC_EDIT1,
    WM_GETTEXT, 128, (LPARAM) texto );

  // Metodo de Horner
  wchar_t*  actual = texto;
  UINT      valor  = 0;
  UINT      digito;
  int       codigo = 1;

  while( *actual != 0 ) {

    digito = *actual++ - '0';
    if( digito <= 9 ) {
      valor = valor*10 + digito;
      continue;
    }
    
    EnableWindow( GetDlgItem( hDlg, IDB_MOVER ), FALSE );
    Grados = 0;
    return 0;
  }

  EnableWindow( GetDlgItem( hDlg, IDB_MOVER ), TRUE );
  Grados = valor;
  return 1;
}


int CDialogo::OnToggle() {

  // Envia el mensaje
  if( Puerto->Send( "3,0\r\n", 6 ) )
    return 0;

  // Lectura del mensaje reflejado
  HWND hWndChild = GetDlgItem( hDlg, IDC_EDIT1 );
  wchar_t  respuesta[128];
  UINT  leidos;
  if( Puerto->ReadLine( respuesta, leidos ) == EXITO ) {
    respuesta[ leidos - 2 ] = 0;
    SendMessage( hWndChild, WM_SETTEXT, 0, (LPARAM) respuesta );
  }

  return 0;
}


int  CDialogo::Home(){

  // Formato del comando
  char  orden[64] = "1,800\r\n";
  UINT  leidos;

  if(PasosV >= 1600){
    Pasos = 3200 - PasosV;
    PasosV = 0;

    UINT  longitud  = sprintf( orden, "1,%d\r\n", Pasos ) + 1;

    // Despliega el mensaje
    HWND hWndChild = GetDlgItem( hDlg, IDC_EDIT1 );
    SendMessage(
      hWndChild, WM_SETTEXT,
      0, (LPARAM) L"Comando en proceso ..." );
    UpdateWindow( hWndChild );

    // Envia el mensaje
    if( Puerto->Send( orden, longitud ) )
      return 0;

    // Lectura del mensaje reflejado
    wchar_t  respuesta[128];
    if( Puerto->ReadLine( respuesta, leidos ) == EXITO ) {
      respuesta[ leidos - 2 ] = 0;
      SendMessage( hWndChild, WM_SETTEXT, 0, (LPARAM) respuesta );
    }

      return 1;
    } else{

    Pasos = PasosV;
    PasosV = 0;

      // Envia el mensaje
    if( Puerto->Send( "3,0\r\n", 6 ) )
      return 0;

    // Lectura del mensaje reflejado
    HWND hWndChild = GetDlgItem( hDlg, IDC_EDIT1 );
    wchar_t  respuesta[128];
    if( Puerto->ReadLine( respuesta, leidos ) == EXITO ) {
      respuesta[ leidos - 2 ] = 0;
      SendMessage( hWndChild, WM_SETTEXT, 0, (LPARAM) respuesta );
    }

    UINT  longitud  = sprintf( orden, "1,%d\r\n", Pasos ) + 1;

    // Despliega el mensaje
    SendMessage(
      hWndChild, WM_SETTEXT,
      0, (LPARAM) L"Comando en proceso ..." );
    UpdateWindow( hWndChild );

    // Envia el mensaje
    if( Puerto->Send( orden, longitud ) )
      return 0;

    // Lectura del mensaje reflejado
    if( Puerto->ReadLine( respuesta, leidos ) == EXITO ) {
      respuesta[ leidos - 2 ] = 0;
      SendMessage( hWndChild, WM_SETTEXT, 0, (LPARAM) respuesta );
    }

    if( Puerto->Send( "3,0\r\n", 6 ) )
      return 0;

    // Lectura del mensaje reflejado
    if( Puerto->ReadLine( respuesta, leidos ) == EXITO ) {
      respuesta[ leidos - 2 ] = 0;
      SendMessage( hWndChild, WM_SETTEXT, 0, (LPARAM) respuesta );
    }

    return 0;
  }
}

// *******************************************************************
// PROCEDIMIENTO DE VENTANA
// *******************************************************************

INT_PTR CDialogo::AboutDlgProc
( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam ) {

  switch( message ) {

  case WM_INITDIALOG:
    Caja.Puerto = (CSerial*) lParam;
    Caja.hDlg   = hDlg;
    return TRUE;
 
  case WM_COMMAND :
      
    switch( wParam ) {
      
    case IDB_MOVER:
      return Caja.Mover();

    case IDB_REGRESA:
      return Caja.OnToggle();

    case IDB_HOME:
      return Caja.Home();

    case IDC_EDIT1 + ( EN_KILLFOCUS << 16 ):
      return Caja.OnKillFocus();

    case IDOK:
    case IDCANCEL:
      EndDialog( hDlg, 0 );
      return TRUE;
    }
  }

  return FALSE;
}




// ***********************************************************************
// PUNTO DE ENTRADA AL PROGRAMA
// ***********************************************************************

int WINAPI WinMain( 
  HINSTANCE  hInstance, HINSTANCE  hPrevInstance,
  PSTR       szCmdLine, int        iCmdShow ) {

  // Instancia de la clase CSerial
  CSerial motor; 
  if( OpenPort( motor ) == 0 )
    return 0;

  DialogBoxParam( 
    hInstance,
    MAKEINTRESOURCE( IDD_DIALOG1 ),
    GetDesktopWindow(),
    CDialogo::AboutDlgProc,
    (LPARAM) &motor ); //CAST  

  return 0;  
}