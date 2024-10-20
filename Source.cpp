#include <iostream>

#define NOMINMAX

#include <fstream>
#include <string>
#include <cmath>
#include <matplot/matplot.h>

using namespace matplot;
using namespace std;

#define ARCHIVO_DEF "DataConcatenada"

enum Prop_send {
	AREA,
	CODRANA,
	CONDICION,
	PERIMETRO,
	TIEMPO,
	VOLUMENRELATIVO,
	VOLUMEN
};

//sapos pepes
struct Huevos {
	int codrana = 0;
	char condicion[20];
	int area;
	float perimetro;
	int tiempo;
	float volumenrelativo;
	long float volumen;
};

struct Nodo {
	Huevos h;
	Nodo* next;
};

void Formatear(Nodo*&);
void CalcularInfo(Nodo*&);
void CargarMuestra(Nodo*&, int);
void ParsingAreaRelativa(Nodo*&, Nodo*);
void MostrarHuevo(Nodo*);
void GenerarBin(Nodo*);

template<typename T>
vector<T> Get(Nodo* optn, Prop_send ps) {
	vector<T> vec;

	while (optn->next != nullptr) {
		switch (ps) {
			case VOLUMENRELATIVO: {
				vec.push_back(optn->h.volumenrelativo);
				break;
			}
			case TIEMPO: {
				vec.push_back(optn->h.tiempo);
				break;
			}
		}

		optn = optn->next;
	}

	return vec;
}



Huevos Parse(string);

int main() {

	Nodo* n = new Nodo;
	Formatear(n);
	CalcularInfo(n);
	GenerarBin(n);
	n = nullptr;

	Nodo* optn = nullptr;

	char opt[50];
	while (strcmp(opt, "F")) {

		system("cls");

		if (optn == nullptr) {
			strcpy(opt, "\t1) CAGAR OVOCITO");
		}
		else {
			sprintf(opt, "\t<*> OVOCITO N: %i\n\tG) GRAFICAR", optn->h.codrana);
		}

		cout << "SELECCIONA UNA OPCION:" << endl
			<< opt << endl
			<< "\tF) SALIR" << endl
			<< endl;
		cin >> opt;

		try {
			CargarMuestra(optn, stoi(opt));

			if (optn != nullptr) {
				MostrarHuevo(optn);
			}
		}
		catch(exception e){
			if (!strcmp(opt, "G"))
			{
				MostrarHuevo(optn);

				cla();
				std::vector<double> tiempo = Get<double>(optn, Prop_send::TIEMPO);
				std::vector<double> volumenrelativo = Get<double>(optn, Prop_send::VOLUMENRELATIVO);

				plot(tiempo, volumenrelativo, "s")->marker_face_color({ .5, .5, .5 });

				xlabel("TIEMPO");
				ylabel("VOLUMEN RELATIVO");

				show();
				
			}
		}
	}
}

void MostrarHuevo(Nodo* optn) {
	if (optn != nullptr) {
		printf("| %-2s | %-7s | %-15s | %-5s | %-10s | %-14s | %-14s |\n", "N", "TIEMPO", "CONDICION", "AREA", "PERIMETRO", "VOLUMEN", "VOL. RELATIVO");

		while (optn->next != nullptr) {

			printf("| %-2i | %-7i | %-15s | %-5i | %-10f | %-14f | %-14f |\n", optn->h.codrana, optn->h.tiempo, optn->h.condicion, optn->h.area, optn->h.perimetro, optn->h.volumen, optn->h.volumenrelativo);

			optn = optn->next;
		}

		//printf("Presione ENTER para ir al menu principal...");

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	else {
		cout << "\t** Debe escribir el numero del experimento! **" << endl;

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

void CargarMuestra(Nodo*& optn, int i) {

	char strfile[50];
	sprintf(strfile, "%s.bin", ARCHIVO_DEF);
	FILE* datos = fopen(strfile, "rb");

	optn = new Nodo();

	while (!feof(datos)) {
		Huevos h;
		fread(&h, sizeof(Huevos), 1, datos);

		if (h.codrana == i) {
			Nodo* aux = new Nodo();

			aux->h = h;
			aux->next = optn;

			optn = aux;
		}
	}
}

void CalcularInfo(Nodo*& nodo) {
	cout << "void CalcularInfo(Nodo*)" << endl;

	Nodo* haux = nodo;
	Huevos h = nodo->h;

	Nodo* nhaux = new Nodo();

	while (haux->next != nullptr) {
		if (haux->h.codrana != 0) {

			long float volaux = pow(float(haux->h.area), 1.5);

			haux->h.volumen = volaux;

			if (haux->h.tiempo == 0) {
				ParsingAreaRelativa(haux, nodo);
			}

		}
		haux = haux->next;
	}
	cout << "void CalcularInfo(Nodo*) -- OK" << endl;
}

void ParsingAreaRelativa(Nodo*& haux, Nodo* nodo) {
	cout << "\tvoid ParsingAreaRelativa(Nodo*& haux)" << endl;

	while (nodo->next != nullptr) {
		if (nodo->h.codrana != 0) {
			if (nodo->h.codrana == haux->h.codrana) {
				float auxf = float(nodo->h.volumen / haux->h.volumen);
				nodo->h.volumenrelativo = auxf;
			}
		}
		nodo = nodo->next;
	}

	cout << "\tvoid ParsingAreaRelativa(Nodo*& haux) -- OK" << endl;
}

void GenerarBin(Nodo* nodo) {
	cout << "void GenerarBin(Nodo)" << endl;

	char strfile[50];
	sprintf(strfile, "%s.bin", ARCHIVO_DEF);
	FILE* datos = fopen(strfile, "wb");

	Nodo* aux = nodo;
	while (aux->next != nullptr) {

		fwrite(&aux->h, sizeof(Huevos), 1, datos);

		aux = aux->next;
	}

	fclose(datos);

	cout << "void GenerarBin(Nodo) -- OK" << endl;
}

void Formatear(Nodo* &nodo) {

	cout << "void Formatear(Nodo*)" << endl;

	char strfile[50];
	sprintf(strfile, "%s.csv", ARCHIVO_DEF);

	FILE* datos = fopen(strfile, "r");

	char linea[100];
	while (!feof(datos)){
		fgets(linea, 100, datos);
		string aux(linea);

		Huevos h;
		h = Parse(aux);

		Nodo *auxNodo = new Nodo();
		auxNodo->h = h;
		auxNodo->next = nodo;

		nodo = auxNodo;
	}

	fclose(datos);
	cout << "void Formatear(Nodo*) -- OK" << endl;
}

Huevos Parse(string s) {

	string aux(s);
	Huevos h;

	aux = aux.substr(aux.find(",\"") + 2);
	aux = aux.substr(0, aux.find(":"));
	try {
		h.codrana = stoi(aux);

		aux = s.substr(s.find(":")+1);
		aux = aux.substr(0, aux.find("\",\"")-3);

		strcpy(h.condicion, aux.data());

		aux = s.substr(s.find(":") + 1);
		aux = aux.substr(aux.find("\",\"") -2);
		aux = aux.substr(0, aux.find(",")-1);
		
		h.tiempo = stoi(aux);

		s = s.substr(s.find(":") + 1);
		s = s.substr(s.find(",") + 2);
		aux = s.substr(0, s.find(",") - 1);

		h.area = stoi(aux);

		s = s.substr(s.find(",") + 2);
		s = s.substr(0, s.find("\""));

		h.perimetro = stof(s);

		return h;
	}
	catch (exception e) {
		
		Huevos auxh;

		auxh.area = 0;
		auxh.codrana = 0;
		strcpy(auxh.condicion, "");
		auxh.perimetro = 0;
		auxh.tiempo = 0;
		auxh.volumen = 0.0;
		auxh.volumenrelativo = 0.0;

		return auxh;
	}
}