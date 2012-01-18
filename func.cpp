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

#include "func.h"

// Número a Texto
string IntToStr(int number)
{
	stringstream ss;
	ss << number;
	return ss.str();
}

// Texto a Número
int StrToInt(string t_str)
{
	int number;
	stringstream ss(t_str);
	return ss >> number ? number : 0;
}

// Conviete el Texto a Mayúsculas
string StrUpper(string str)
{
	for (size_t i = 0; i < str.length(); ++i)
		str[i] = toupper(str[i]);
	return str;
}

// Conviete el Texto a Minúsculas
string StrLower(string str)
{
	for (size_t i = 0; i < str.length(); ++i)
		str[i] = tolower(str[i]);
	return str;
}

// Limita el Texto a la cantidad indicada
string StrLimit(string t_str, int limit, bool puntos)
{
	int max_c = t_str.length();
	if ( max_c > limit)
	{
		t_str.erase(limit);
		if (puntos)
			t_str.append("...");
	}
	return t_str;
}

// Reemplaza el Texto indicado
string StrReplace(string str, string str_old, string str_new)
{
	string::size_type uPos = 0;
	string::size_type str_old_len = str_old.length();
	string::size_type str_new_len = str_new.length();
	while ( (uPos = str.find( str_old, uPos )) != string::npos )
	{
		str.replace( uPos, str_old_len, str_new );
//		uPos++;
		uPos += str_new_len;
	}
	return str;
}

// Añade Texto al final
string StrAppend(string str, string str_add)
{
	return StrPreAppend(str, str_add, true);
}

// Añade Texto al principio
string StrPreAppend(string str, string str_add, bool final)
{
	string str_tmp = "";
	if ( final )
	{
		str_tmp = str;
		str_tmp.append(str_add);
	} else {
		str_tmp = str_add;
		str_tmp.append(str);
	}
	return str_tmp;
}

// Compara dos Textos
bool StrCompare(string str_a, string str_b)
{
	if ( str_a.compare(str_b) == 0 )
		return true;
	else
		return false;
}

vector<string> StrSplit(const string str, const string delimiters)
{
	vector<string> tokens;
// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos)
	{          
	// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
	// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
	// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
	return tokens;
}

// Carga una Imágen
BITMAP *openImage(string f_str, RGB *palette, bool isIcon, int ico_w, int ico_h, int col_dep)
{
	BITMAP *bmp;
	if (fileExists(f_str))
	{
		string f_ext = fileExt(f_str);
		if ( f_ext == "png" )
			bmp = load_png(f_str.c_str(), palette);
		else if ( f_ext == "jpg" )
			bmp = load_jpg(f_str.c_str(), palette);
		else if ( f_ext == "gif" )
			bmp = load_gif(f_str.c_str(), palette);
		else if ( f_ext == "bmp" || f_ext == "lbm" || f_ext == "pcx" || f_ext == "tga" )
			bmp = load_bitmap(f_str.c_str(), palette);
		else {
			bmp = create_bitmap_ex(col_dep, ico_w, ico_h);
			clear_to_color(bmp, makeacol_depth(col_dep, 235, 235, 235, 255) );
		}

		if ( isIcon )
		{
			if ( bmp->w > ico_w || bmp->h > ico_h )
			{
				BITMAP *m_icon = create_bitmap_ex(col_dep, ico_w, ico_h);
				clear_to_color(m_icon, makeacol_depth(col_dep, 255, 0, 255, 255) );
			//	clear_to_color(m_icon, makecol(255, 0, 255) );
				stretch_sprite(m_icon, bmp, 0, 0, ico_w, ico_h);
				destroy_bitmap(bmp);
				return m_icon;
			}
		}

		return bmp;
	} else {
		bmp = create_bitmap_ex(col_dep, ico_w, ico_h);
		clear_to_color(bmp, makeacol_depth(col_dep, 255, 0, 255, 0));
	//	clear_to_color(bmp, makecol(255, 0, 255) );
		return bmp;
	}
}

RGB convertToRGB(int r, int g, int b)
{
	RGB m_color;
	if ( r > -1 && r < 256 ) m_color.r = r; else m_color.r = 0;
	if ( g > -1 && g < 256 ) m_color.g = g; else m_color.g = 0;
	if ( b > -1 && b < 256 ) m_color.b = b; else m_color.b = 0;
	return m_color;
}

RGB convertToRGB(string str_rgb)
{
	int r, g, b;
	vector<string> split_rgb = StrSplit(str_rgb, ",");
	r = StrToInt(split_rgb[0]);
	g = StrToInt(split_rgb[1]);
	b = StrToInt(split_rgb[2]);
	return convertToRGB(r, g, b);
}

// Obtiene el listado para la info
vector<string> cargarDatosInfo(string filename, string tag_name)
{
	vector<string> v_list;
	XmlNodes xml_list = loadXmlNodes( filename );
	XmlNodes::iterator itr;
	for(itr = xml_list.begin(); itr != xml_list.end(); ++itr)
	{
		if ( StrCompare(itr->tagName, tag_name) )
			v_list.push_back( itr->getAttrStr("title", "-") );
	}
	return v_list;
}

// Obtiene una lista de archivos del directorio indicado
vector<string> listarDir(string dirname, string ext)
{
	vector<string> v_dir;
	DIR *pDir;
	struct dirent *entry;
	pDir = opendir( dirname.c_str() );
	if ( pDir )
	{
		while ( (entry = readdir(pDir)) )
		{
			if ( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
			{
				if ( fileExt(entry->d_name) == ext )
					v_dir.push_back( entry->d_name );
			}
		}
		closedir(pDir);
	}
	return v_dir;
}
 
// Comprobamos si existe la carpeta
bool pathExists(string path_name)
{
	DIR *pDir;
	pDir = opendir(path_name.c_str());
	bool path_exists = false;
	if ( pDir != NULL )
		path_exists = true;    
	closedir(pDir);
	return path_exists;
}

// Comprobamos si existe el archivo
bool fileExists(string filename)
{
	ifstream fin;
	fin.open(filename.c_str());
	bool file_exists = false;
	if ( fin.fail() )
		file_exists = false;
	else
		file_exists =  true;
	fin.close();
	return file_exists;
}

// Obtiene el Nombre de un archivo
string fileName(string f_str)
{
	size_t found;
	found = f_str.find_last_of("/\\");
	return f_str.substr(found+1);
}

// Obtiene el Directorio
string filePath(string f_str)
{
	size_t found;
	found = f_str.find_last_of("/\\");
	return f_str.substr(0,found);
}

// Obtiene la Extensión de un archivo
string fileExt(string f_str)
{
	size_t found;
	found = f_str.find_last_of(".");
	return StrLower( f_str.substr(found+1) );
}

// Obtiene la Letra de Unidad de la ruta de un archivo
string fileLetter(string f_str)
{
	size_t found;
	found = f_str.find_last_of(":");
	return StrUpper( f_str.substr(0,found) );
}
