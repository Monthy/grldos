/*
 *   _______    ________           _______    ________          
 *  |  _____|  |  ____  |  ___    |  ____ \  |  ____  |  ______ 
 *  | |        | |    | | |_  |   | |    | | | |    | | |  ____|
 *  | |  ____  | |____| |   | |   | |    | | | |    | | | |____ 
 *  | | |__  | |  __   _|   | |   | |    | | | |    | | |____  |
 *  | |____| | | |  \  \   _| |_  | |____| | | |____| |  ____| |
 *  |________| |_|   \__| |_____| |_______/  |________| |______|
 *
 * GRlDOs by Monthy                                Version 0.1.1
 *
 * Copyright (c) 2011 Pedro A. Garcia Rosado Aka Monthy
 *
 * This file is part of GRlDOs games launcher for DOS and DOSBox
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 *
**/

#define GRLDOS_VER "v0.1.1"
#define GRLDOS_DATE "May 21 2011"
#define MAX_JUEGOS 4000
#define MAX_BOTTONS 14

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <direct.h>

#include <allegro.h>
#include <loadpng.h>
#include <jpgalleg.h>
#include <algif.h>
#include <alfont.h>
#include <adime.h>

#include "LibReadINI.h"
#include "xml/xmlparser.h"
using namespace std;

string IntToStr(int number);										// Número a Texto
int StrToInt(string t_str);											// Texto a Número
string StrUpper(string str);										// Conviete el Texto a Mayúsculas
string StrLower(string str);										// Conviete el Texto a Minúsculas
string StrLimit(string t_str, int limit = 35, bool puntos = true);	// Limita el Texto a la cantidad indicada
string StrReplace(string str, string str_old, string str_new);		// Reemplaza el Texto indicado
string StrAppend(string str, string str_add);						// Añade Texto al final
string StrPreAppend(string str, string str_add, bool final = false);// Añade Texto al principio
bool StrCompare(string str_a = "", string str_b = "");				// Compara dos Textos
vector<string> StrSplit(const string str, const string delimiters);	// Separa el texto 

BITMAP *openImage(string f_str, RGB *palette = NULL, bool isIcon = false, int ico_w = 32, int ico_h = 32, int col_dep = 32);// Carga una Imágen
RGB convertToRGB(int r=0, int g=0, int b=0);			// Convierte valores numericos 0-255 de color a RGB
RGB convertToRGB(string str_rgb);						// Convierte valores en texto 0, 0, 0 de color a RGB

vector<string> cargarDatosInfo(string filename, string tag_name);	// Obtiene el listado para la info

vector<string> listarDir(string dirname, string ext);	// Obtiene una lista de archivos del directorio indicado
bool pathExists(string path_name);						// Comprobamos si existe la carpeta
bool fileExists(string filename);						// Comprobamos si existe el archivo
string fileName(string f_str);							// Obtiene el Nombre de un archivo
string filePath(string f_str);							// Obtiene el Directorio
string fileExt(string f_str);							// Obtiene la Extensión de un archivo
string fileLetter(string f_str);						// Obtiene la Letra de Unidad de la ruta de un archivo
