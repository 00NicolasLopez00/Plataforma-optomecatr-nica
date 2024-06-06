class CDialogo {

  static   CDialogo Caja;
  CSerial*  Puerto;
  HWND      hDlg;
  UINT      Grados;
  UINT      Pasos;
  UINT      PasosV;
  UINT      Destino;

  int  Mover();
  int  OnKillFocus();
  int  OnToggle();
  int  Home();

public:
  static INT_PTR AboutDlgProc(
    HWND   hDlg,   UINT   message,
    WPARAM wParam, LPARAM lParam );
};
