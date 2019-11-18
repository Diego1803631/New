#include <fstream>
#include <Windows.h>
#include "resource.h"
using namespace std;
BOOL CALLBACK VentUsua(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK vengeren(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK ven_Platillos(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venListaPlatillos(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venplati(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venEditarPlatillo(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK ven_Personal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venpersonal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venListaPersonal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venEditarPersonal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK vencventa(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venVerVenta(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venccoman(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venEditarComan(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venListPagos(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venpago(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK venmese(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK vencoman(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK vencerrar(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);

struct meseros {
	char primna[16];
	char segna[16];
	char appat[16];
	char apmat[16];
	char contra[21];
	char usuar[25];
	char fot[MAX_PATH];
	int n = 1;
	meseros *izq;
	meseros *der;
	meseros *padre;
};
class plati {
public:
	char nombre[26];
	char imagen[MAX_PATH];
	char precio[8];
	int cate;
	int id = 1;
	plati *prev = NULL;
	plati *next = NULL;
};
class comandas {
public:
	bool cerrada = false;
	bool pagada = false;
	char plati[26][26];
	char Mesa[4];
	char mese[16];
	char propi[6];
	int Dia;
	int Mes;
	int Anio;
	int Hora;
	int Min;
	int tam;
	int id = 1;
	int formaP;
	float totalPagar;
	comandas *prev = NULL;
	comandas *next = NULL;
};
struct ventas {
	char plati[26][26];
	char Mesa[4];
	char mese[16];
	char propi[6];
	int Dia;
	int Mes;
	int Anio;
	int fecha;
	int Hora;
	int Min;
	int tam;
	int formaP;
	int id;
	float totalPagar;
	ventas *prev = NULL;
	ventas *next = NULL;
};
meseros *raiz = 0, *auxm = 0;
plati *prim = 0, *auxp = 0;
comandas *cab = 0, *auxc = 0;
ventas *ini = 0, *auxv = 0;

HWND ghDlg = 0;
HINSTANCE ghInst;
int gShow, tam;
static HBITMAP bmp1;
char dir[MAX_PATH] = { "\0" }, folder[MAX_PATH], folder2[MAX_PATH], folder3[MAX_PATH], folder4[MAX_PATH], texto[40], texto2[40], fecha[10];
bool usuar = 0;
SYSTEMTIME time;

void ventana(HWND, int);
void Cargarima(HWND);
meseros *buscarUsuario(meseros *, char[]);
bool validarPersonal(meseros *, HWND, bool);
void altaMeseros(meseros *&, meseros *);
bool validarPlatillo(plati *, HWND, bool);
bool validarComanda(comandas *, HWND, bool);
void rellenarPlatillo(HWND);
void LlenarCombo(meseros *, HWND);
meseros *buscarArbol(meseros *, char []);
void eliminar(meseros *);
meseros *izquierda(meseros *);
void reemplazar(meseros *, meseros*);
void rellenarMesero(HWND);
void llenarLista(HWND);
void rellenarComan(HWND);
void quickSort(ventas *, ventas *);
ventas* medio(ventas*, ventas*);
void swap(ventas*, ventas*, ventas*);
ventas* busqueda(ventas*, int);
void insertionSort(ventas **);
void sortedInsert(ventas**, ventas*);
void guardarMesero(meseros *);
void guardarPlatillo();
void guardarComandas();
void guardarVenta();
void borrarMemoria(meseros *);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmd, int show)
{
	ghInst = hInst;
	gShow = show;
	ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenUsua), 0, VentUsua);
	ShowWindow(ghDlg, show);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (ghDlg == 0 || !IsDialogMessage(ghDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

BOOL CALLBACK VentUsua(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam)
{
	char usua[25];
	char contra[21] = { '\0' };
	switch (Mensaje) {
	case WM_INITDIALOG: {
		if (prim == NULL) {
			GetCurrentDirectory(MAX_PATH, folder); strcpy_s(folder2, folder); strcpy_s(folder3, folder); strcpy_s(folder4, folder);
			strcat_s(folder, "\\meseros.bin"); strcat_s(folder2, "\\platillos.bin"); strcat_s(folder3, "\\comandas.bin"); strcat_s(folder4, "\\ventas.bin");
			fstream archivo(folder, ios::out | ios::app);
			archivo.close();
			archivo.open(folder, ios::in | ios::binary);
			if (archivo.is_open()) {
				auxm = new meseros;
				archivo.read((char*)auxm, sizeof(meseros));
				while (!archivo.eof()) {
					altaMeseros(raiz, NULL);
					auxm = new meseros;
					archivo.read((char*)auxm, sizeof(meseros));
				}
				archivo.close();
			}
			fstream archiv(folder2, ios::out | ios::app);
			archiv.close();
			archiv.open(folder2, ios::in | ios::binary);
			if (archiv.is_open()) {
				plati *nuev = new plati;
				archiv.read((char*)nuev, sizeof(plati));
				while (!archiv.eof()) {
					nuev->next = NULL;
					plati *ultim = prim;
					while (ultim != NULL && ultim->next != NULL) {
						ultim = ultim->next;
					}
					if (ultim != NULL) {
						ultim->next = nuev;
						nuev->prev = ultim;
					}
					else
						prim = nuev;
					nuev = new plati;
					archiv.read((char*)nuev, sizeof(plati));
				}
				archiv.close();
			}
			fstream archi(folder3, ios::out | ios::app);
			archi.close();
			archi.open(folder3, ios::in | ios::binary);
			if (archi.is_open()) {
				comandas *nuev = new comandas;
				archi.read((char*)nuev, sizeof(comandas));
				while (!archi.eof()) {
					nuev->next = NULL;
					comandas *ultim = cab;
					while (ultim != NULL && ultim->next != NULL) {
						ultim = ultim->next;
					}
					if (ultim != NULL) {
						ultim->next = nuev;
						nuev->prev = ultim;
					}
					else
						cab = nuev;
					nuev = new comandas;
					archi.read((char*)nuev, sizeof(comandas));
				}
				archi.close();
			}
			fstream arch(folder4, ios::out | ios::app);
			arch.close();
			arch.open(folder4, ios::in | ios::binary);
			if (arch.is_open()) {
				ventas *nuev = new ventas;
				arch.read((char*)nuev, sizeof(ventas));
				while (!arch.eof()) {
					nuev->next = NULL;
					ventas *ultim = ini;
					while (ultim != NULL && ultim->next != NULL) {
						ultim = ultim->next;
					}
					if (ultim != NULL) {
						ultim->next = nuev;
						nuev->prev = ultim;
					}
					else
						ini = nuev;
					nuev = new ventas;
					arch.read((char*)nuev, sizeof(ventas));
				}
				arch.close();
			}
		}
		SendDlgItemMessage(Dlg, TipUsua, CB_ADDSTRING, 0, (LPARAM)"Gerente");
		SendDlgItemMessage(Dlg, TipUsua, CB_ADDSTRING, 0, (LPARAM)"Mesero");
		SendDlgItemMessage(Dlg, TipUsua, CB_SETCURSEL, 0, 0);
		return true; }
	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
		case Entrar:
			SendDlgItemMessage(Dlg, Usuario, WM_GETTEXT, 11, (LPARAM)usua);
			if (SendDlgItemMessage(Dlg, TipUsua, CB_GETCURSEL, 0, 0) == 1) {
				auxm = NULL; auxm = buscarUsuario(raiz, usua);
				if (auxm != NULL) {
					SendDlgItemMessage(Dlg, Contra, WM_GETTEXT, 11, (LPARAM)contra);
					if (strcmp(contra, auxm->contra) == 0) {
						ventana(Dlg, 7); usuar = 1;
					}
					else
						MessageBox(Dlg, "La contraseña no coincide", "Error en la contraseña", MB_OK | MB_ICONERROR);
				}
				else
					MessageBox(Dlg, "El usuario no existe", "Error en usuario", MB_OK | MB_ICONERROR);				
			}
			else {
				if (strcmp(usua, "Dykeo") == 0) {
					SendDlgItemMessage(Dlg, Contra, WM_GETTEXT, 11, (LPARAM)contra);
					if (strcmp(contra, "ewe") == 0) {
						ventana(Dlg, 1); usuar = 0;
					}
					else
						MessageBox(Dlg, "La contraseña no coincide", "Error en la contraseña", MB_OK | MB_ICONERROR);
				}
				else
					MessageBox(Dlg, "El usuario no existe", "Error en usuario", MB_OK | MB_ICONERROR);
			}
			return true;
		}
		return true; }
	case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK vengeren(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case Platillos: ventana(Dlg, 10); return true;
				case Personal: ventana(Dlg, 13); return true;
				case CVentas: ventana(Dlg, 4); return true;
				case CComan: ventana(Dlg, 5); return true;
				case Cuentas: ventana(Dlg, 17); return true;
				case Salir: ventana(Dlg, 0); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK ven_Platillos(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case IngresarPlatillo:	ventana(Dlg, 2);	return true;
				case ListaPlatillos: ventana(Dlg, 11);	return true;
				case Salir: ventana(Dlg, 1);
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venListaPlatillos(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_INITDIALOG: {
			auxp = prim;
			while (auxp != NULL) {
				SendDlgItemMessage(Dlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)auxp->nombre);
				auxp = auxp->next;
			}
			return true; }
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case IDC_COMBO1:
					if (SendDlgItemMessage(Dlg, IDC_COMBO1, CB_GETCURSEL, 0, 0) != CB_ERR) {
						EnableWindow(GetDlgItem(Dlg, Eliminar), TRUE); EnableWindow(GetDlgItem(Dlg, Editar), TRUE);
						auxp = prim;
						for (int i = 0; i < SendDlgItemMessage(Dlg, IDC_COMBO1, CB_GETCURSEL, 0, 0); i++)
							auxp = auxp->next;
						rellenarPlatillo(Dlg);
					}
					return true;
				case Editar: ventana(Dlg, 12); return true;
				case Eliminar: if (MessageBox(Dlg, "¿Seguro que quiere eliminar este platillo?", "Advertencia", MB_OKCANCEL) == IDOK) {
					if (auxp->prev == NULL)
						prim = auxp->next;
					else
						auxp->prev->next = auxp->next;
					if (auxp->next != NULL)
						auxp->next->prev = auxp->prev;
					delete auxp;	guardarPlatillo();
					MessageBox(0, "Platillo eliminado con éxito", "Eliminación", MB_OK); ventana(Dlg, 11);
				} return true;
				case Regresar: ventana(Dlg, 10); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venplati(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_INITDIALOG: {
			SendDlgItemMessage(Dlg, Cate, CB_ADDSTRING, 0, (LPARAM)"Desayuno");
			SendDlgItemMessage(Dlg, Cate, CB_ADDSTRING, 0, (LPARAM)"Comida");
			SendDlgItemMessage(Dlg, Cate, CB_ADDSTRING, 0, (LPARAM)"Cena");
			SendDlgItemMessage(Dlg, Cate, CB_ADDSTRING, 0, (LPARAM)"Bebidas");
			SendDlgItemMessage(Dlg, Cate, CB_ADDSTRING, 0, (LPARAM)"Postres");
			SendDlgItemMessage(Dlg, Cate, CB_SETCURSEL, 0, 0);
			return true; }
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case CarImagen: Cargarima(Dlg); return true;
				case Guardar: {
					auxp = new plati;
					if (validarPlatillo(auxp, Dlg, true) == true) {
						guardarPlatillo();
						MessageBox(0, "Platillo agregado con éxito", "Alta de platillo", MB_OK); ventana(Dlg, 2);
					}
					else
						delete auxp;
					return true; }
				case Regresar: ventana(Dlg, 10);  return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venEditarPlatillo(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_INITDIALOG:	rellenarPlatillo(Dlg);	return true; 
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case CarImagen: Cargarima(Dlg); return true;
				case Guardar: 
					if (strcmp(dir, "") == 0)
						strcpy_s(dir, auxp->imagen);
					if (validarPlatillo(auxp, Dlg, false) == true) {
						MessageBox(0, "Platillo editado con éxito", "Edición", MB_OK);
						guardarPlatillo(); ventana(Dlg, 11);
					}
					return true;
				case Regresar: ventana(Dlg, 11); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK ven_Personal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case IngresarPersonal: ventana(Dlg, 3); return true;
				case ListaPersonal: ventana(Dlg, 14);  return true;
				case Salir: ventana(Dlg, 1); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venpersonal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case foto: Cargarima(Dlg); return true;
				case Guardar: {
						auxm = new meseros;
						if (validarPersonal(auxm, Dlg, true) == true) {
							MessageBox(0, "Mesero agregado con éxito", "Alta mesero", MB_OK);
							ofstream archi;
							archi.open(folder, ios::binary | ios::trunc);
							archi.close(); 
							guardarMesero(raiz); ventana(Dlg, 3);
						}
						else
							delete auxm;
					return true; }
				case Cancelar: ventana(Dlg, 13); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venListaPersonal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	int i;
	switch (Mensaje) {
		case WM_INITDIALOG: LlenarCombo(raiz, Dlg); return true;
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case name: {
					if (SendDlgItemMessage(Dlg, name, CB_GETCURSEL, 0, 0) != CB_ERR) {
						EnableWindow(GetDlgItem(Dlg, Eliminar), TRUE); EnableWindow(GetDlgItem(Dlg, Editar), TRUE);
						i = SendDlgItemMessage(Dlg, name, CB_GETCURSEL, 0, 0);
						char nombre[16]; 
						SendDlgItemMessage(Dlg, name, CB_GETLBTEXT, i, (LPARAM)nombre);
						auxm = buscarArbol(raiz, nombre);
						rellenarMesero(Dlg);
					} return true; }
				case Editar: ventana(Dlg, 15); return true;
				case Eliminar: { 
					if (MessageBox(Dlg, "¿Está seguro de eliminar al mesero?", "Aviso", MB_OKCANCEL) == IDOK) {
						eliminar(auxm); ofstream archi;
						archi.open(folder, ios::binary | ios::trunc);
						archi.close(); guardarMesero(raiz);
						MessageBox(0, "Mesero eliminado con éxito", "Eliminación", MB_OK); ventana(Dlg, 14);
					} return true; }
				case Cancelar: ventana(Dlg, 13); return true;
				}
				return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venEditarPersonal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_INITDIALOG: rellenarMesero(Dlg); return true; 
		case WM_COMMAND: {
				switch (LOWORD(wParam))
				{
					case foto: Cargarima(Dlg);  return true;
					case Guardar: 
						if (strcmp(dir, "") == 0)
							strcpy_s(dir, auxm->fot);
						if (validarPersonal(auxm, Dlg, false) == true) {
							MessageBox(0, "Mesero editado con éxito", "Edición", MB_OK);
							ofstream archi;
							archi.open(folder, ios::binary | ios::trunc);
							archi.close();
							guardarMesero(raiz); ventana(Dlg, 14);
						}
						return true;
					case Cancelar: ventana(Dlg, 14); return true;
				}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK vencventa(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	int categ;
	switch (Mensaje) {
		case WM_INITDIALOG: {
			SendDlgItemMessage(Dlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Desayuno");
			SendDlgItemMessage(Dlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Comida");
			SendDlgItemMessage(Dlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Cena");
			SendDlgItemMessage(Dlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Bebidas");
			SendDlgItemMessage(Dlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Postres");
			LlenarCombo(raiz, Dlg);
			return true; }
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case Buscar: { SendDlgItemMessage(Dlg, lista, LB_RESETCONTENT, 0, 0); bool band;
					categ = SendDlgItemMessage(Dlg, IDC_COMBO1, CB_GETCURSEL, 0, 0);
					if (SendDlgItemMessage(Dlg, IDC_COMBO1, CB_GETCURSEL, 0, 0) != CB_ERR) {
						auxc = cab;
						while (auxc != NULL) {
							if (auxc->pagada == true) {
								band = false;
								auxp = prim;
								while (auxp != NULL) {
									for (int i = 0; i < auxc->tam; i++) {
										if (strcmp(auxc->plati[i], auxp->nombre) == 0) {
											if (categ == auxp->cate) {
												llenarLista(Dlg); band = true;
												break;
											}
										}
									}
									if (band == true) {
										band = false; break;
									}
									auxp = auxp->next;
								}
							}
							auxc = auxc->next;
						}
					}
					else {
						if (SendDlgItemMessage(Dlg, name, CB_GETCURSEL, 0, 0) != CB_ERR) {
							SendDlgItemMessage(Dlg, name, WM_GETTEXT, 16, (LPARAM)texto2);
							auxv = ini;
							while (auxv->next != NULL)
								auxv = auxv->next;
							quickSort(ini, auxv);
							auxv = ini;
							while (auxv != NULL) {
								if (strcmp(auxv->mese, texto2) == 0) {
									strcpy_s(texto, "ID ");  _itoa_s(auxv->id, fecha, 10); strcat_s(texto, fecha); strcat_s(texto, "- Mesa ");
									strcat_s(texto, auxv->Mesa); strcat_s(texto, "- Dia "); _itoa_s(auxv->Dia, fecha, 10);
									strcat_s(texto, fecha); strcat_s(texto, "- Mes "); _itoa_s(auxv->Mes, fecha, 10);
									strcat_s(texto, fecha); strcat_s(texto, "- Año "); _itoa_s(auxv->Anio, fecha, 10);
									strcat_s(texto, fecha);	SendDlgItemMessage(Dlg, lista, LB_ADDSTRING, 0, (LPARAM)texto);
								}
								auxv = auxv->next;
							}
						}
						else {
							int di1 = 0, me1 = 0, ani1 = 0, di2 = 0, me2 = 0, ani2 = 0;
							SendDlgItemMessage(Dlg, Dia1, WM_GETTEXT, 3, (LPARAM)fecha); di1 = atoi(fecha);
							SendDlgItemMessage(Dlg, Mes1, WM_GETTEXT, 3, (LPARAM)fecha); me1 = atoi(fecha);
							SendDlgItemMessage(Dlg, Anio1, WM_GETTEXT, 5, (LPARAM)fecha); ani1 = atoi(fecha);
							SendDlgItemMessage(Dlg, Dia2, WM_GETTEXT, 3, (LPARAM)fecha); di2 = atoi(fecha);
							SendDlgItemMessage(Dlg, Mes2, WM_GETTEXT, 3, (LPARAM)fecha); me2 = atoi(fecha);
							SendDlgItemMessage(Dlg, Anio2, WM_GETTEXT, 5, (LPARAM)fecha); ani2 = atoi(fecha);
							if (di1 == 0 && me1 == 0 && ani1 == 0 && di2 == 0 && me2 == 0 && ani2 == 0) {
								return true;
							}
							else {
								insertionSort(&ini);
								di1 =di1 + (me1 * 10000) + (ani1 * 1000000);
								di2 =di2 + (me2 * 10000) + (ani2 * 1000000);
								if (di1 != 0 && me1 != 0 && ani1 != 0 && di2 != 0 && me2 != 0 && ani2 != 0) {
									auxv = ini;
									while (auxv != NULL) {
										if (auxv->fecha >= di1 && auxv->fecha <= di2) {
											strcpy_s(texto, "ID ");  _itoa_s(auxv->id, fecha, 10); strcat_s(texto, fecha); strcat_s(texto, "- Mesa ");
											strcat_s(texto, auxv->Mesa); strcat_s(texto, "- Dia "); _itoa_s(auxv->Dia, fecha, 10);
											strcat_s(texto, fecha); strcat_s(texto, "- Mes "); _itoa_s(auxv->Mes, fecha, 10);
											strcat_s(texto, fecha); strcat_s(texto, "- Año "); _itoa_s(auxv->Anio, fecha, 10);
											strcat_s(texto, fecha);	SendDlgItemMessage(Dlg, lista, LB_ADDSTRING, 0, (LPARAM)texto);
										}
										auxv = auxv->next;
									}
								}
								else {
									MessageBox(0, "Llene todas las fechas, por favor", "Error", MB_OK);
								}
							}
						}
					}
					return true; }
				case lista: EnableWindow(GetDlgItem(Dlg, Ver), TRUE); return true;
				case Ver: {
					if (SendDlgItemMessage(Dlg, lista, LB_GETCURSEL, 0, 0) != LB_ERR) {
						SendDlgItemMessage(Dlg, lista, LB_GETTEXT, (WPARAM)SendDlgItemMessage(Dlg, lista, LB_GETCURSEL, 0, 0), (LPARAM)texto2);
						auxc = cab;
						while (auxc != NULL) {
							strcpy_s(texto, "ID ");  _itoa_s(auxc->id, fecha, 10); strcat_s(texto, fecha); strcat_s(texto, "- Mesa ");
							strcat_s(texto, auxc->Mesa); strcat_s(texto, "- Dia "); _itoa_s(auxc->Dia, fecha, 10);
							strcat_s(texto, fecha); strcat_s(texto, "- Mes "); _itoa_s(auxc->Mes, fecha, 10);
							strcat_s(texto, fecha); strcat_s(texto, "- Año "); _itoa_s(auxc->Anio, fecha, 10);
							strcat_s(texto, fecha);
							if (strcmp(texto, texto2) == 0) {
								ventana(Dlg, 18); return true;
							}
							auxc = auxc->next;
						}
					}return true;
				}
				case Cancelar: usuar ? ventana(Dlg, 7) : ventana(Dlg, 1); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venVerVenta(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_INITDIALOG: {
			for (int i = 0; i < auxc->tam; i++) {
				auxp = prim;
				while (auxp != NULL) {
					if (strcmp(auxc->plati[i], auxp->nombre) == 0) {
						strcpy_s(texto, auxp->nombre); strcat_s(texto, "      "); strcat_s(texto, auxp->precio);
						SendDlgItemMessage(Dlg, listilla, LB_ADDSTRING, 0, (LPARAM)texto);
						break;
					}
					auxp = auxp->next;
				}
			}
			SendDlgItemMessage(Dlg, mesa, WM_SETTEXT, 0, (LPARAM)auxc->Mesa); SendDlgItemMessage(Dlg, nombreMesero, WM_SETTEXT, 0, (LPARAM)auxc->mese);
			SendDlgItemMessage(Dlg, propina, WM_SETTEXT, 0, (LPARAM)auxc->propi); _itoa_s(auxc->Dia, texto2, 10); SendDlgItemMessage(Dlg, Ddia, WM_SETTEXT, 0, (LPARAM)texto2);
			_itoa_s(auxc->Mes, texto2, 10); SendDlgItemMessage(Dlg, Mmes, WM_SETTEXT, 0, (LPARAM)texto2); _itoa_s(auxc->Anio, texto2, 10); SendDlgItemMessage(Dlg, Aanio, WM_SETTEXT, 0, (LPARAM)texto2);
			_itoa_s(auxc->Hora, texto2, 10); SendDlgItemMessage(Dlg, Hhora, WM_SETTEXT, 0, (LPARAM)texto2); _itoa_s(auxc->Min, texto2, 10); SendDlgItemMessage(Dlg, Mminutos, WM_SETTEXT, 0, (LPARAM)texto2);
			int pago; pago = auxc->totalPagar;
			char pagot[10]; _itoa_s(pago, pagot, 10);
			SendDlgItemMessage(Dlg, total, WM_SETTEXT, 0, (LPARAM)pagot);
			return true; }
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case Regresar: ventana(Dlg, 4); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venccoman(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case Buscar:{
					SendDlgItemMessage(Dlg, lista, LB_RESETCONTENT, 0, 0);
					char buscar1[5], buscar2[5], buscar3[5], buscar4[5];
					tam = SendDlgItemMessage(Dlg, idc_mesa, WM_GETTEXT, 6, (LPARAM)buscar1);
					if (tam == 0)
						buscar1[0] = NULL;
					else {
						auxc = cab;
						while (auxc != NULL) {
							if (strcmp(auxc->Mesa, buscar1) == 0)
								llenarLista(Dlg);
							auxc = auxc->next;
						}
					}


					tam = SendDlgItemMessage(Dlg, idc_dia, WM_GETTEXT, 6, (LPARAM)buscar2);
					if (tam != 0) {
						tam = SendDlgItemMessage(Dlg, idc_mes, WM_GETTEXT, 6, (LPARAM)buscar3);
						if (tam == 0) {
							MessageBox(0, "Ingrese un mes", "ERROR", MB_OK);
							return true;
						}
						tam = SendDlgItemMessage(Dlg, idc_anio, WM_GETTEXT, 6, (LPARAM)buscar4);
						if (tam == 0) {
							MessageBox(0, "Ingrese un año", "ERROR", MB_OK);
							return true;
						} SendDlgItemMessage(Dlg, lista, LB_RESETCONTENT, 0, 0);


						int dia, mes, anio;
						dia = atoi(buscar2); mes = atoi(buscar3); anio = atoi(buscar4);
						dia = dia + (mes * 10000) + (anio * 1000000);
						auxv = NULL;
						auxv = busqueda(ini, dia);
						if (auxv != NULL) {
							strcpy_s(texto, "ID ");
							_itoa_s(auxv->id, fecha, 10);
							strcat_s(texto, fecha); strcat_s(texto, "- Mesa ");
							strcat_s(texto, auxv->Mesa); strcat_s(texto, "- Dia "); _itoa_s(auxv->Dia, fecha, 10);
							strcat_s(texto, fecha); strcat_s(texto, "- Mes "); _itoa_s(auxv->Mes, fecha, 10);
							strcat_s(texto, fecha); strcat_s(texto, "- Año "); _itoa_s(auxv->Anio, fecha, 10);
							strcat_s(texto, fecha);	SendDlgItemMessage(Dlg, lista, LB_ADDSTRING, 0, (LPARAM)texto);
						}


						auxv = ini;
						if (buscar1[0] == '\0') {
							while (auxv != NULL) {
								if (auxv->fecha >= dia && auxv->fecha <= dia) {
									strcpy_s(texto, "ID ");  _itoa_s(auxv->id, fecha, 10); strcat_s(texto, fecha); strcat_s(texto, "- Mesa ");
									strcat_s(texto, auxv->Mesa); strcat_s(texto, "- Dia "); _itoa_s(auxv->Dia, fecha, 10);
									strcat_s(texto, fecha); strcat_s(texto, "- Mes "); _itoa_s(auxv->Mes, fecha, 10);
									strcat_s(texto, fecha); strcat_s(texto, "- Año "); _itoa_s(auxv->Anio, fecha, 10);
									strcat_s(texto, fecha);	SendDlgItemMessage(Dlg, lista, LB_ADDSTRING, 0, (LPARAM)texto);
								}
								auxv = auxv->next;
							}
						}
						else {
							while (auxv != NULL) {
								if (auxv->fecha >= dia && auxv->fecha <= dia) {
									strcpy_s(texto, "ID ");  _itoa_s(auxv->id, fecha, 10); strcat_s(texto, fecha); strcat_s(texto, "- Mesa ");
									strcat_s(texto, auxv->Mesa); strcat_s(texto, "- Dia "); _itoa_s(auxv->Dia, fecha, 10);
									strcat_s(texto, fecha); strcat_s(texto, "- Mes "); _itoa_s(auxv->Mes, fecha, 10);
									strcat_s(texto, fecha); strcat_s(texto, "- Año "); _itoa_s(auxv->Anio, fecha, 10);
									strcat_s(texto, fecha);	SendDlgItemMessage(Dlg, lista, LB_ADDSTRING, 0, (LPARAM)texto);
								}
								auxv = auxv->next;
							}
						}
					} return true;
				}
				case lista: EnableWindow(GetDlgItem(Dlg, Info), TRUE); return true;
				case Info: if (SendDlgItemMessage(Dlg, lista, LB_GETCURSEL, 0, 0) != LB_ERR) {
					SendDlgItemMessage(Dlg, lista, LB_GETTEXT, (WPARAM)SendDlgItemMessage(Dlg, lista, LB_GETCURSEL, 0, 0), (LPARAM)texto2);
					auxc = cab;
					while (auxc != NULL) {
						strcpy_s(texto, "ID ");  _itoa_s(auxc->id, fecha, 10); strcat_s(texto, fecha); strcat_s(texto, "- Mesa ");
						strcat_s(texto, auxc->Mesa); strcat_s(texto, "- Dia "); _itoa_s(auxc->Dia, fecha, 10);
						strcat_s(texto, fecha); strcat_s(texto, "- Mes "); _itoa_s(auxc->Mes, fecha, 10);
						strcat_s(texto, fecha); strcat_s(texto, "- Año "); _itoa_s(auxc->Anio, fecha, 10);
						strcat_s(texto, fecha);
						if (strcmp(texto, texto2) == 0) {
							ventana(Dlg, 16); return true;
						}
						auxc = auxc->next;
					}
					}return true;
				case Cancelar: usuar ? ventana(Dlg, 7) : ventana(Dlg, 1); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venEditarComan(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_INITDIALOG: rellenarComan(Dlg); return true;
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case Guardar: 
					if (validarComanda(auxc, Dlg, false) == true) {
						MessageBox(0, "Comanda editada con éxito", "EDICIÓN", MB_OK);
						ventana(Dlg, 16);
					} return true;
				case Regresar: ventana(Dlg, 5); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venListPagos(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_INITDIALOG: {
			auxc = cab;
			while (auxc != NULL) {
				if (auxc->cerrada == true && auxc->pagada == false)
					llenarLista(Dlg);
				auxc = auxc->next;
			} return true; }
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case lista: if (SendDlgItemMessage(Dlg, lista, CB_GETCURSEL, 0, 0) != LB_ERR)
					EnableWindow(GetDlgItem(Dlg, Pagar), TRUE); return true;
				case Pagar: if (SendDlgItemMessage(Dlg, lista, LB_GETCURSEL, 0, 0) != LB_ERR) {
					SendDlgItemMessage(Dlg, lista, LB_GETTEXT, (WPARAM)SendDlgItemMessage(Dlg, lista, LB_GETCURSEL, 0, 0), (LPARAM)texto2);
					auxc = cab;
					while (auxc != NULL) {
						strcpy_s(texto, "ID ");  _itoa_s(auxc->id, fecha, 10); strcat_s(texto, fecha); strcat_s(texto, "- Mesa ");
						strcat_s(texto, auxc->Mesa); strcat_s(texto, "- Dia "); _itoa_s(auxc->Dia, fecha, 10);
						strcat_s(texto, fecha); strcat_s(texto, "- Mes "); _itoa_s(auxc->Mes, fecha, 10);
						strcat_s(texto, fecha); strcat_s(texto, "- Año "); _itoa_s(auxc->Anio, fecha, 10);
						strcat_s(texto, fecha);
						if (strcmp(texto, texto2) == 0) {
							ventana(Dlg, 6); return true;
						}
						auxc = auxc->next;
					}
				}return true;
				case  Regresar: ventana(Dlg, 1); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venpago(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_INITDIALOG: {
			for (int i = 0; i < auxc->tam; i++)
				SendDlgItemMessage(Dlg, lista, LB_ADDSTRING, 0, (LPARAM)auxc->plati[i]);
			int pago; pago = auxc->totalPagar;
			char pagot[10]; _itoa_s(pago, pagot, 10);
			SendDlgItemMessage(Dlg, total, WM_SETTEXT, 0, (LPARAM)pagot);
			SendDlgItemMessage(Dlg, mesero, WM_SETTEXT, 0, (LPARAM)auxc->mese);
			SendDlgItemMessage(Dlg, Forma, CB_ADDSTRING, 0, (LPARAM)"Efectivo");
			SendDlgItemMessage(Dlg, Forma, CB_ADDSTRING, 0, (LPARAM)"Tarjeta de crédito");
			SendDlgItemMessage(Dlg, Forma, CB_ADDSTRING, 0, (LPARAM)"Tarjeta de débito");
			SendDlgItemMessage(Dlg, Forma, CB_SETCURSEL, 0, 0);
			return true; }
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case Aceptar:	SendDlgItemMessage(Dlg, propina, WM_GETTEXT, 6, (LPARAM)auxc->propi);
					auxc->formaP = SendDlgItemMessage(Dlg, Forma, CB_GETCURSEL, 0, 0);
					auxc->pagada = true; 
					auxv = new ventas;
					for (int i = 0; i < auxc->tam; i++)
						strcpy_s(auxv->plati[i], auxc->plati[i]);
					strcpy_s(auxv->Mesa, auxc->Mesa); strcpy_s(auxv->mese, auxc->mese); strcpy_s(auxv->propi, auxc->propi);
					auxv->Dia = auxc->Dia; auxv->Mes = auxc->Mes; auxv->Anio = auxc->Anio; auxv->Hora = auxc->Hora; auxv->Min = auxc->Min;
					auxv->tam = auxc->tam; auxv->formaP = auxc->formaP; auxv->totalPagar = auxc->totalPagar;
					auxv->fecha = auxv->Dia + (auxv->Mes * 10000) + (auxv->Anio * 1000000); auxv->id = auxc->id;
					if (ini == 0) {
						ini = auxv;
						ini->next = ini->prev = NULL;
					}
					else {
						ventas *aux = ini;
						while (aux->next != NULL)
							aux = aux->next;
						aux->next = auxv;
						auxv->prev = aux;
						auxv->next = NULL;
					} guardarVenta(); guardarComandas();
					MessageBox(Dlg, "Cuenta pagada", "Pago", MB_OK); ventana(Dlg, 17); return true;
				case  Cancelar: ventana(Dlg, 17); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK venmese(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case Comandas: ventana(Dlg, 8); return true;
				case Cerrar: ventana(Dlg, 9); return true;
				case CVentas: ventana(Dlg, 4); return true;
				case CComan: ventana(Dlg, 5); return true;
				case Salir: ventana(Dlg, 0); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK vencoman(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	char Platillo[26];
	switch (Mensaje) {
		case WM_INITDIALOG: {
			auxp = prim;
			while (auxp != NULL) {
				SendDlgItemMessage(Dlg, plat, CB_ADDSTRING, 0, (LPARAM)auxp->nombre);
				auxp = auxp->next;
			}
			SendDlgItemMessage(Dlg, plat, CB_SETCURSEL, 0, 0);
			SendDlgItemMessage(Dlg, name, CB_ADDSTRING, 0, (LPARAM)auxm->appat);
			SendDlgItemMessage(Dlg, name, CB_SETCURSEL, 0, 0);
			return true; }
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case Aniadir:
					SendDlgItemMessage(Dlg, plat, CB_GETLBTEXT, (WPARAM)SendDlgItemMessage(Dlg, plat, CB_GETCURSEL, 0, 0), (LPARAM)Platillo);
					SendDlgItemMessage(Dlg, lista, LB_ADDSTRING, 0, (LPARAM)Platillo);
					return true;
				case Guardar:
					if (SendDlgItemMessage(Dlg, lista, LB_GETCOUNT, 0, 0) == 0)
						MessageBox(0, "No se ha añadido ningún platillo a la lista", "Error en la lista", MB_OK);
					else {
						auxc = new comandas;
						if (validarComanda(auxc, Dlg, true) == false)
							delete auxc;
						else {
							guardarComandas();
							MessageBox(0, "Comanda agregada con éxito", "Alta comanda", MB_OK);
							ventana(Dlg, 8);
						}
					}
					return true;
				case Regresar: ventana(Dlg, 7); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

BOOL CALLBACK vencerrar(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	switch (Mensaje) {
		case WM_INITDIALOG: {
			auxc = cab;
			while (auxc != NULL) {
				if (auxc->cerrada == false)
					llenarLista(Dlg);
				auxc = auxc->next;
			}
			return true; }
		case WM_COMMAND: {
			switch (LOWORD(wParam))
			{
				case Cerrar:
					if (SendDlgItemMessage(Dlg, lista, LB_GETCURSEL, 0, 0) != LB_ERR) {
						SendDlgItemMessage(Dlg, lista, LB_GETTEXT, (WPARAM)SendDlgItemMessage(Dlg, lista, LB_GETCURSEL, 0, 0), (LPARAM)texto2);
						auxc = cab;
						while (auxc != NULL) {
							strcpy_s(texto, "ID ");  _itoa_s(auxc->id, fecha, 10); strcat_s(texto, fecha); strcat_s(texto, "- Mesa ");
							strcat_s(texto, auxc->Mesa); strcat_s(texto, "- Dia "); _itoa_s(auxc->Dia, fecha, 10);
							strcat_s(texto, fecha); strcat_s(texto, "- Mes "); _itoa_s(auxc->Mes, fecha, 10);
							strcat_s(texto, fecha); strcat_s(texto, "- Año "); _itoa_s(auxc->Anio, fecha, 10);
							strcat_s(texto, fecha);
							if (strcmp(texto, texto2) == 0) {
								auxc->cerrada = true;
								MessageBox(0, "Cuenta cerrada con éxito", "CERRAR", MB_OK); guardarComandas();
								ventana(Dlg, 9); return true;
							}
							auxc = auxc->next;
						}
					} return true;
				case Salir: ventana(Dlg, 7); return true;
			}
			return true; }
		case WM_CLOSE: borrarMemoria(raiz); PostQuitMessage(0); return true;
	} return false;
}

meseros *buscarUsuario(meseros *raiz, char usuario[]) {
	if (raiz == NULL)
		return NULL;
	if (strcmp(raiz->usuar, usuario) == 0)
		return raiz;
	else {
		if (buscarUsuario(raiz->izq, usuario) == NULL)
			buscarUsuario(raiz->der, usuario);
	}
}

void ventana(HWND Dlg, int op) {
	switch (op) {
		case 0: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenUsua), 0, VentUsua); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 1: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenGeren), 0, vengeren); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 2: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenPlat), 0, venplati); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 3: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenPersonal), 0, venpersonal); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 4: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenCVentas), 0, vencventa); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 5: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenCComandas), 0, venccoman); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 6: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenPagos), 0, venpago); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 7: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenMese), 0, venmese); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 8: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenComan), 0, vencoman); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 9: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenCerrar), 0, vencerrar); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 10: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(Ven_Platillos), 0, ven_Platillos); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 11: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenListaPlatillo), 0, venListaPlatillos); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 12: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenEditarPlatillo), 0, venEditarPlatillo); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 13: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(Ven_Personal), 0, ven_Personal); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 14: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenListaPersonal), 0, venListaPersonal); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 15: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenEditarPersonal), 0, venEditarPersonal); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 16: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenEditarComan), 0, venEditarComan); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 17: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenListPagos), 0, venListPagos); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
		case 18: ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(VenVerVenta), 0, venVerVenta); ShowWindow(ghDlg, gShow);
			DestroyWindow(Dlg);	break;
	}
}

void Cargarima(HWND Dlg) {
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = ghDlg;
	ofn.lpstrFile = dir;
	ofn.nMaxFile = sizeof(dir);
	ofn.lpstrFilter = "Bmp\0*.BMP\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = 0;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetSaveFileName(&ofn)) {
		bmp1 = (HBITMAP)LoadImage(NULL, dir, IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
		SendDlgItemMessage(Dlg, Imagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp1);
		ShowWindow(GetDlgItem(Dlg, Imagen), SW_SHOW);
	}
}

bool validarPersonal(meseros *aux, HWND Dlg, bool band) {
	tam = SendDlgItemMessage(Dlg, primn, WM_GETTEXT, 17, (LPARAM)aux->primna);
	if (tam == 0) {
		MessageBox(Dlg, "Ingrese el primer nombre", "Error en Primer nombre", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else
		if (tam == 16) {
			MessageBox(Dlg, "Nombre máximo de 15 carácteres", "Error en Primer nombre", MB_OK | MB_ICONEXCLAMATION);
			SendDlgItemMessage(Dlg, primn, WM_SETTEXT, 0, (LPARAM)"");
			return false;
		}
	tam = SendDlgItemMessage(Dlg, segn, WM_GETTEXT, 17, (LPARAM)aux->segna);
	if (tam == 16) {
		MessageBox(Dlg, "Nombre máximo de 15 carácteres", "Error en Segundo nombre", MB_OK | MB_ICONEXCLAMATION);
		SendDlgItemMessage(Dlg, segn, WM_SETTEXT, 0, (LPARAM)"");
		return false;
	}
	tam = SendDlgItemMessage(Dlg, App, WM_GETTEXT, 17, (LPARAM)aux->appat);
	if (tam == 0) {
		MessageBox(Dlg, "Ingrese el apellido paterno", "Error en Apellido paterno", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else
		if (tam == 16) {
			MessageBox(Dlg, "Apellido máximo de 15 carácteres", "Error en Apellido paterno", MB_OK | MB_ICONEXCLAMATION);
			SendDlgItemMessage(Dlg, App, WM_SETTEXT, 0, (LPARAM)"");
			return false;
		}
	tam = SendDlgItemMessage(Dlg, Apm, WM_GETTEXT, 17, (LPARAM)aux->apmat);
	if (tam == 0) {
		MessageBox(Dlg, "Ingrese el apellido materno", "Error en Apellido materno", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else
		if (tam == 16) {
			MessageBox(Dlg, "Apellido máximo de 15 carácteres", "Error en Apellido materno", MB_OK | MB_ICONEXCLAMATION);
			SendDlgItemMessage(Dlg, Apm, WM_SETTEXT, 0, (LPARAM)"");
			return false;
		}
	tam = SendDlgItemMessage(Dlg, Usuario, WM_GETTEXT, 17, (LPARAM)aux->usuar);
	if (tam == 0) {
		MessageBox(Dlg, "Ingrese el usuario", "Error en Usuario", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else
		if (tam == 16) {
			MessageBox(Dlg, "Usuario máximo de 15 carácteres", "Error en Usuario", MB_OK | MB_ICONEXCLAMATION);
			SendDlgItemMessage(Dlg, Usuario, WM_SETTEXT, 0, (LPARAM)"");
			return false;
		}
	tam = SendDlgItemMessage(Dlg, Contra, WM_GETTEXT, 22, (LPARAM)aux->contra);
	if (tam == 0) {
		MessageBox(Dlg, "Ingrese la contraseña", "Error en Contraseña", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else
		if (tam == 21) {
			MessageBox(Dlg, "Contraseña máxima de 20 carácteres", "Error en Contraseña", MB_OK | MB_ICONEXCLAMATION);
			SendDlgItemMessage(Dlg, Contra, WM_SETTEXT, 0, (LPARAM)"");
			return false;
		}
	if (strcmp(dir, "") != 0)
		strcpy_s(auxm->fot, dir);
	else {
		MessageBox(Dlg, "Ingrese una imagen", "Error en imagen", MB_OK | MB_ICONEXCLAMATION);
		return false;
	} dir[0] = '\0';
	if (band == true)
		altaMeseros(raiz, NULL);
	return true;
}

void altaMeseros(meseros *&raiz, meseros *padre) {
	if (raiz == NULL) {
		raiz = auxm;
		raiz->izq = raiz->der = NULL;
		raiz->padre = padre;
	}
	else {
		if (strcmp(raiz->appat, auxm->appat) > 0)
			altaMeseros(raiz->izq, raiz);
		else
			altaMeseros(raiz->der, raiz);
	}
}

bool validarPlatillo(plati *aux, HWND Dlg, bool band) {
	tam = SendDlgItemMessage(Dlg, Nombre, WM_GETTEXT, 27, (LPARAM)aux->nombre);
	if (tam == 0) {
		MessageBox(Dlg, "Ingrese el nombre del platillo", "Error en Nombre del platillo", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	else
		if (tam == 26) {
			MessageBox(Dlg, "Nombre máximo de 25 carácteres", "Error en Nombre del platillo", MB_OK | MB_ICONEXCLAMATION);
			SendDlgItemMessage(Dlg, Nombre, WM_SETTEXT, 0, (LPARAM)"");
			return false;
		}
	aux->cate = SendDlgItemMessage(Dlg, Cate, CB_GETCURSEL, 0, 0);
	tam = SendDlgItemMessage(Dlg, Precio, WM_GETTEXT, 27, (LPARAM)aux->precio);
	if (tam == 0) {
		MessageBox(Dlg, "Ingrese el precio del platillo", "Error en Precio", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	if (strcmp(dir, "") != 0)
		strcpy_s(auxp->imagen, dir);
	else {
		MessageBox(Dlg, "Ingrese una imagen", "Error en imagen", MB_OK | MB_ICONEXCLAMATION);
		return false;
	} dir[0] = '\0';
	if (band == true) {
		if (prim == 0) {
			prim = aux;
			prim->next = prim->prev = 0;
		}
		else {
			auxp = prim;
			while (auxp->next != NULL)
				auxp = auxp->next;
			auxp->next = aux;
			aux->id = auxp->id + 1;
			aux->prev = auxp; aux->next = NULL;
		}
	}
	return true;
}

bool validarComanda(comandas *aux, HWND Dlg, bool band) {
	tam = SendDlgItemMessage(Dlg, mesa, WM_GETTEXT, 4, (LPARAM)aux->Mesa);
	if (tam == 0) {
		MessageBox(Dlg, "Ingrese el número de mesa", "Error en Mesa", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	SendDlgItemMessage(Dlg, name, WM_GETTEXT, 17, (LPARAM)aux->mese);
	GetLocalTime(&time);
	aux->Dia = time.wDay;	aux->Mes = time.wMonth;	aux->Anio = time.wYear;
	aux->Hora = time.wHour;	aux->Min = time.wMinute;
	auxc = cab;
	while (auxc != NULL) {
		if (strcmp(auxc->Mesa, aux->Mesa) == 0 && auxc->Dia == aux->Dia && auxc->Mes == aux->Mes && auxc->Anio == aux->Anio && strcmp(auxc->mese, aux->mese) != 0) {
			MessageBox(0, "Ya hay mesero encargado de esa mesa ese día", "Advertencia", MB_OK); return false;
		}
		auxc = auxc->next;
	}
	aux->tam = SendDlgItemMessage(Dlg, lista, LB_GETCOUNT, 0, 0); aux->totalPagar = 0;
	for (int i = 0; i < aux->tam; i++) {
		SendDlgItemMessage(Dlg, lista, LB_GETTEXT, (WPARAM)i, (LPARAM)aux->plati[i]);
		auxp = prim;
		while (strcmp(auxp->nombre, aux->plati[i]) != 0)
			auxp = auxp->next;
		aux->totalPagar += atof(auxp->precio);
	}
	if (band == true) {
		if (cab == NULL) {
			cab = aux;
			cab->next = cab->prev = NULL;
		}
		else {
			auxc = cab;
			while (auxc->next != NULL)
				auxc = auxc->next;
			auxc->next = aux;
			aux->id = auxc->id + 1;
			aux->prev = auxc; aux->next = NULL;
		}
	} return true;
}

void rellenarPlatillo(HWND Dlg) {
	SetWindowText(GetDlgItem(Dlg, Nombre), auxp->nombre); SetWindowText(GetDlgItem(Dlg, Precio), auxp->precio); 
	SendDlgItemMessage(Dlg, Cate, CB_ADDSTRING, 0, (LPARAM)"Desayuno");	SendDlgItemMessage(Dlg, Cate, CB_ADDSTRING, 0, (LPARAM)"Comida");
	SendDlgItemMessage(Dlg, Cate, CB_ADDSTRING, 0, (LPARAM)"Cena");	SendDlgItemMessage(Dlg, Cate, CB_ADDSTRING, 0, (LPARAM)"Bebidas");
	SendDlgItemMessage(Dlg, Cate, CB_ADDSTRING, 0, (LPARAM)"Postres");	SendDlgItemMessage(Dlg, Cate, CB_SETCURSEL, auxp->cate, 0);
	bmp1 = (HBITMAP)LoadImage(NULL, auxp->imagen, IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
	SendDlgItemMessage(Dlg, Imagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp1);
}

void LlenarCombo(meseros *raiz, HWND Dlg) {
	if (raiz == NULL)
		return;
	else {
		LlenarCombo(raiz->izq, Dlg);
		SendDlgItemMessage(Dlg, name, CB_ADDSTRING, 0, (LPARAM)raiz->appat);
		LlenarCombo(raiz->der, Dlg);
	}
}

meseros *buscarArbol(meseros *raiz, char buscar[]) {
	if (strcmp(raiz->appat, buscar) == 0)
		return raiz;
	else {
		if (strcmp(raiz->appat, buscar) > 0)
			return buscarArbol(raiz->izq, buscar);
		else
			return buscarArbol(raiz->der, buscar);
	}
}

void eliminar(meseros *aux) {
	if (aux->izq && aux->der) { //tiene ambos hijos
		meseros *izqui = izquierda(aux->der);
		strcpy_s(aux->primna, izqui->primna); strcpy_s(aux->segna, izqui->segna); strcpy_s(aux->appat, izqui->appat);
		strcpy_s(aux->apmat, izqui->apmat); strcpy_s(aux->contra, izqui->contra); strcpy_s(aux->usuar, izqui->usuar);
		strcpy_s(aux->fot, izqui->fot); aux->n = izqui->n;
		eliminar(izqui);
	}
	else {
		if (aux->izq) { //Sólo tiene el izquierdo
			reemplazar(aux, aux->izq);
			aux->izq = aux->der = NULL;
			delete aux;
		}
		else {
			if (aux->der) { //Sólo tiene derecho
				reemplazar(aux, aux->der);
				aux->izq = aux->der = NULL;
				delete aux;
			}
			else { //No tiene hijos
				reemplazar(aux, NULL);
				aux->izq = aux->der = NULL;
				delete aux;
			}
		}
	}
}

meseros *izquierda(meseros *raiz) {
	if (raiz->izq)
		return izquierda(raiz->izq);
	else
		return raiz;
}

void reemplazar(meseros *raiz, meseros *nodo) {
	if (raiz->padre) {
		if (raiz->padre->izq == raiz)
			raiz->padre->izq = nodo;
		else
			raiz->padre->der = nodo;
	}
	else
		if (nodo)
			nodo->padre = NULL;
	if (nodo)
		nodo->padre = raiz->padre;
	else {
		if (raiz->padre->izq == raiz)
			raiz->padre->izq = NULL;
		else
			raiz->padre->der = NULL;
	}
}

void rellenarMesero(HWND Dlg) {
	SetWindowText(GetDlgItem(Dlg, primn), auxm->primna);  SetWindowText(GetDlgItem(Dlg, segn), auxm->segna);
	SetWindowText(GetDlgItem(Dlg, App), auxm->appat); SetWindowText(GetDlgItem(Dlg, Apm), auxm->apmat);
	SetWindowText(GetDlgItem(Dlg, Usuario), auxm->usuar); SetWindowText(GetDlgItem(Dlg, Contra), auxm->contra);
	bmp1 = (HBITMAP)LoadImage(NULL, auxm->fot, IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
	SendDlgItemMessage(Dlg, Imagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp1);
}

void llenarLista(HWND Dlg) {
	strcpy_s(texto, "ID ");  _itoa_s(auxc->id, fecha, 10); strcat_s(texto, fecha); strcat_s(texto, "- Mesa ");
	strcat_s(texto, auxc->Mesa); strcat_s(texto, "- Dia "); _itoa_s(auxc->Dia, fecha, 10);
	strcat_s(texto, fecha); strcat_s(texto, "- Mes "); _itoa_s(auxc->Mes, fecha, 10);
	strcat_s(texto, fecha); strcat_s(texto, "- Año "); _itoa_s(auxc->Anio, fecha, 10);
	strcat_s(texto, fecha);	SendDlgItemMessage(Dlg, lista, LB_ADDSTRING, 0, (LPARAM)texto);
}

void rellenarComan(HWND Dlg) {
	auxp = prim;
	while (auxp != NULL) {
		SendDlgItemMessage(Dlg, plat, CB_ADDSTRING, 0, (LPARAM)auxp->nombre);
		auxp = auxp->next;
	}
	for (int i = 0; i < auxc->tam; i++)
		SendDlgItemMessage(Dlg, lista, LB_ADDSTRING, 0, (LPARAM)auxc->plati[i]);
	SendDlgItemMessage(Dlg, mesa, WM_SETTEXT, 0, (LPARAM)auxc->Mesa);
	LlenarCombo(raiz, Dlg);
	SendDlgItemMessage(Dlg, name, CB_SETCURSEL, SendDlgItemMessage(Dlg, name, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)auxc->mese), 0);
}

void quickSort(ventas *izq, ventas *der) {
	ventas *i = izq, *j = der;
	ventas *tmp;
	ventas* med = medio(izq, NULL);
	while (strcmp(i->mese,j->mese) <= 0) {
		while (strcmp(i->mese, med->mese) < 0)
			i = i->next;
		while (strcmp(j->mese, med->mese) > 0)
			j = j->prev;
		if (strcmp(i->mese, j->mese) <= 0) {
			swap(i, i, j);
			i = i->next;
			j = j->prev;
		}
	}
	if (strcmp(izq->mese, j->mese) < 0)
		quickSort(izq, j); 
	if (strcmp(i->mese, der->mese) < 0)
		quickSort(i, der);
}
ventas* medio(ventas* prime, ventas* ulti)
{
	if (prime == NULL)
		return NULL;
	ventas* slow = prime;
	ventas* fast = prime->next;
	while (fast != ulti)
	{
		fast = fast->next;
		if (fast != ulti)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	return slow;
}
void swap(ventas *tmp, ventas *i, ventas *j) {
	for (int k = 0; k < i->tam; k++)
		strcpy_s(i->plati[k], j->plati[k]);
	strcpy_s(i->Mesa, j->Mesa); strcpy_s(i->mese, j->mese); strcpy_s(i->propi, j->propi);
	i->Dia = j->Dia; i->Mes = j->Mes; i->Anio = j->Anio; i->Hora = j->Hora; i->Min = j->Min;
	i->tam = j->tam; i->formaP = j->formaP; i->totalPagar = j->totalPagar;
	i->fecha = j->fecha;

	for (int k = 0; k < j->tam; k++)
		strcpy_s(j->plati[k], tmp->plati[k]);
	strcpy_s(j->Mesa, tmp->Mesa); strcpy_s(j->mese, tmp->mese); strcpy_s(j->propi, tmp->propi);
	j->Dia = tmp->Dia; j->Mes = tmp->Mes; j->Anio = tmp->Anio; j->Hora = tmp->Hora; j->Min = tmp->Min;
	j->tam = tmp->tam; j->formaP = tmp->formaP; j->totalPagar = tmp->totalPagar;
	j->fecha = tmp->fecha;
}

ventas* busqueda(ventas* head, int fecha)
{
	ventas* start = head;
	ventas* last = NULL;
	do
	{
		ventas* mid = medio(start, last);
		if (mid == NULL)
			return NULL;
		if (mid->fecha == fecha)
			return mid;
		else if (mid->fecha < fecha)
			start = mid->next;
		else
			last = mid;
	} while (last == NULL || last != start);
	return NULL;
}

void insertionSort(ventas ** head_ref) {
		ventas* sorted = NULL;
		ventas* current = *head_ref;
		while (current != NULL) {
			ventas* next = current->next;
			current->prev = current->next = NULL; 
			sortedInsert(&sorted, current);
			current = next;
		}
		*head_ref = sorted;
}
void sortedInsert(ventas** head_ref, ventas* newNode)
{
	ventas* current;
	if (*head_ref == NULL)
		*head_ref = newNode;
	else if ((*head_ref)->fecha >= newNode->fecha) {
		newNode->next = *head_ref;
		newNode->next->prev = newNode;
		*head_ref = newNode;
	}
	else {
		current = *head_ref;
		while (current->next != NULL && current->next->fecha < newNode->fecha)
			current = current->next;
		newNode->next = current->next;
		if (current->next != NULL)
			newNode->next->prev = newNode;
		current->next = newNode;
		newNode->prev = current;
	}
}

void guardarMesero(meseros *raiz) {
	ofstream archi(folder, ios::app | ios::out | ios::binary);
	if (raiz == NULL)
	{
		archi.close();
		return;
	}
	else
	{
		archi.write((char*)raiz, sizeof(meseros));
		archi.close();
		guardarMesero(raiz->izq);
		guardarMesero(raiz->der);
	}
}

void guardarPlatillo() {
	ofstream archi;
	archi.open(folder2, ios::binary | ios::trunc);
	auxp = prim;
	while (auxp != 0) {
		archi.write((char *)auxp, sizeof(plati));
		auxp = auxp->next;
	}
	archi.close();
}

void guardarComandas() {
	ofstream archi;
	archi.open(folder3, ios::binary | ios::trunc);
	auxc = cab;
	while (auxc != 0) {
		archi.write((char *)auxc, sizeof(comandas));
		auxc = auxc->next;
	}
	archi.close();
}

void guardarVenta() {
	ofstream archi;
	archi.open(folder4, ios::binary | ios::trunc);
	auxv = ini;
	while (auxv != 0) {
		archi.write((char *)auxv, sizeof(ventas));
		auxv = auxv->next;
	}
	archi.close();
}

void borrarMemoria(meseros *raiz) {
	if (raiz == NULL)
		return;
	else {
		borrarMemoria(raiz->izq);
		borrarMemoria(raiz->der);
		delete raiz;
	}
	auxp = prim;
	while (prim != NULL) {
		prim = prim->next;
		delete auxp;
		auxp = prim;
	}
	auxc = cab;
	while (cab != NULL) {
		cab = cab->next;
		delete auxc;
		auxc = cab;
	}
}