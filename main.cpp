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

#include "grldos.h"

int main(int argc, char *argv[])
{
	set_uformat(U_UTF8);	// U_UNICODE, U_ASCII, U_UTF8
	allegro_init();
	push_config_state();
	set_config_file("grldos.ini");

	loadpng_init();			// Para poder mostrar imágenes en PNG
	jpgalleg_init();		// Para poder mostrar imágenes en JPG
	algif_init();			// Para poder mostrar imágenes en GIF

	GRlDos *dlg = new GRlDos();
	bool isPlay = dlg->grl_isPlay();
	delete dlg;

	if ( !isPlay )
		cout << "GRlDOs " << GRLDOS_VER << endl;

	return 0;
}

END_OF_MAIN()
