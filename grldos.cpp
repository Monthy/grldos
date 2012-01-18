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

int callback(DIALOG *d, int button_index)
{
	switch (button_index)
	{
		case 1: // OK button
			// Return 1 to indicate that button number 1 was clicked, and add a
			// negative sign to tell adime_dialogf() to save the results.
			return -1;
		case 2: // Cancel button
			// Return 2 to indicate that button number 2 was clicked, and don't
			// add a negative sign to tell adime_dialogf() to ignore the results.
			return 2;
		default:// NOTREACHED
			ASSERT(0);
			return 0;
	}
}

GRlDos::GRlDos()
{
	grl_load_config();
	grl_init();

	do {
		grl_mouse_info();
		if ( !isDialog )
		{
			if ( key[KEY_ESC] )
			{
			// Salimos del Programa
				remove("ejecutar.bat");
				isPlay = false;
				isExit = true;
			}
			else if ( key[KEY_A] )
			{
			// Muestra Acerca de... del Programa
				grl_about_help(false);
			}
			else if ( key[KEY_F1] )
			{
			// Muestra la ayuda del programa
				grl_about_help(true);
			}
			else if ( key[KEY_UP] )
			{
			// Sube un item seleccionado
				grl_item_key_up();
			}
			else if ( key[KEY_DOWN] )
			{
			// Baja un item seleccionado
				grl_item_key_down();	
			}
			else if ( key[KEY_DEL] )
			{
			// Eliminamos el Juego Seleccionado
				grl_delete_game();
			}
			else if ( key[KEY_LEFT] )
			{
			// Editamos el Juego Seleccionado
				if ( id_game > -1 && num_items > -1 && !info[id_game].getEliminado() )
					grl_add_edit_game(true);
			}
			else if ( key[KEY_RIGHT] )
			{
			// Añadimos un nuevo Juego
				if ( num_items < MAX_JUEGOS )
					grl_add_edit_game(false);
			}
			else if ( key[KEY_O] )
			{
			// Opciones del Programa
				grl_options();
			}
			else if ( key[KEY_E] )
			{
			// Descomprime un archivo
				grl_decompress_file();
			}
			else if ( key[KEY_ENTER] || key[KEY_SPACE] )
			{
			// Ejecuta el juego
				grl_play_game();
			}
			else if ( key[KEY_BACKSPACE] )
			{
			// Ejecuta el setup del juego
				grl_setup_game();
			}
		}
		clear_keybuf();
		vsync();
	} while ( !isExit );
}

GRlDos::~GRlDos()
{
// destroy the BITMAP
	destroy_bitmap(img_bg);
	destroy_bitmap(img_item_list_s);
	destroy_bitmap(img_item_list_u);
	destroy_bitmap(img_item_list_del_s);
	destroy_bitmap(img_item_list_del_u);
	destroy_bitmap(img_buf);

	clear_keybuf();
	grl_save_config();

	alfont_destroy_font(grl_alfont);
	alfont_exit();
	adime_exit();

	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	allegro_exit();
}

// Carga la configuración
void GRlDos::grl_load_config()
{
	defect_msg_about =  "\nGRlDOs " GRLDOS_VER " - Copyright (c) 2011\n\n"
						"Web: http://www.gr-lida.org/grldos/\n\n\n"
						"{lb_designed_and_programmed}\n\n"
						" Pedro A. Garcia Rosado (Monthy)\n\n\n"
						"{lb_thanks}\n\n"
						" David Colmenero (D_Skywalk) -- libReadINI\n\n"
						" {lb_developers} ---------- Allegro\n\n"
						" Sven Sandberg ---------------- Adime GUI Allegro\n\n"
						" Angelo Mottola --------------- JPGalleg\n\n"
						" Peter Wang ------------------- loadPNG\n\n"
						" Elias Pschernig -------------- Allegro GIF Addon\n\n"
						" Dustin Dettmer --------------- XML xmlparser DUI";

	defect_msg_help =   "Tecla ------ Acción\n"
						" F1 -------- Muestra la ayuda del programa\n"
						" A --------- Muestra acerca del programa\n"
						" ESC ------- Salimos del programa\n"
						" O --------- Muestra las opciones del programa\n"
						" E --------- Muestra dialogo para descomprimir un archivo\n\n"
						" ENTER ----- Ejecuta el juego seleccionado\n"
						" SPACE ----- Ejecuta el juego seleccionado\n"
						" BACKSPACE - Ejecuta el setup de juego seleccionado\n\n"
						" UP -------- Mueve un item arriba\n"
						" DOWN ------ Mueve un item abajo\n"
						" DEL ------- Marca como eliminado un item\n"
						" LEFT ------ Muestra dialogo para editar el Juego Seleccionado\n"
						" RIGHT ----- Muestra dialogo para añadir un nuevo Juego";

	use_trans_sprite = false;

	isALFont      = false;
	isBtnMove     = false;
	isBtnMoveUp   = false;
	isBtnMoveDown = false;
	isBtnSelect   = false;
	id_btn_select = -1;

	isPlay   = false;
	isExit   = false;
	isDialog = false;
	isExec   = false;
	isSetup  = false;

	total_games = 0;
	num_items   = -1;

	color_dep  = get_config_int("grl_graphics", "color_dep" ,  32);
	width_grl  = get_config_int("grl_graphics", "width_grl" , 640);
	height_grl = get_config_int("grl_graphics", "height_grl", 480);

	cmd_new_linea = get_config_string("grl_general", "cmd_new_linea", "#");
	list_exe_ext  = get_config_string("grl_general", "list_exe_ext" , "exe;com;bat");
	list_zip_ext  = get_config_string("grl_general", "list_zip_ext" , "zip;7z;rar");

	id_game = get_config_int("grl_general", "id_game", 0);
	ptr     = get_config_int("grl_general", "ptr"    , 0);
	top     = get_config_int("grl_general", "top"    , 0);
	new_ptr = ptr;
}

// Guarda la configuración
void GRlDos::grl_save_config()
{
	set_config_int("grl_general", "id_game", id_game);
	set_config_int("grl_general", "ptr"    ,     ptr);
	set_config_int("grl_general", "top"    ,     top);
}

// Indicamos la profundidad de color
void GRlDos::grl_set_correct_color_depth()
{
	string dir_skin = "skins\\";
	dir_skin.append(get_config_string("grl_general", "skin", "grldos.ini"));
	if ( fileExists(dir_skin) )
	{
		cfgOpen(dir_skin.c_str(), MS_STYLE);
		cfgSelectSection("grl_skin_general");
		color_dep_recomend = cfgReadInt("color_dep_recomend", 16);

		cfgClose();
	} else {
		color_dep_recomend = 16;
	}

	if ( color_dep > color_dep_recomend )
		color_dep = color_dep_recomend;

	if ( color_dep < 16 )
		color_dep = 16;
	else if ( color_dep > 16 && color_dep < 32 )
		color_dep = 16;
	else if ( color_dep > 32 )
		color_dep = 32;
}

// Inicia
void GRlDos::grl_init()
{
	install_timer();
	install_mouse();
	install_keyboard();

	alfont_init();
	adime_init();

	grl_set_correct_color_depth();
	set_color_depth(color_dep);
	set_gfx_mode(GFX_AUTODETECT, width_grl, height_grl, 0, 0);

	img_buf	= openImage("", NULL, false, width_grl, height_grl, color_dep);

	grl_load_language(get_config_string("grl_general", "language", "lng_es.ini"));
	grl_load_skin(get_config_string("grl_general", "skin", "grldos.ini"));

	set_mouse_sprite(openImage(f_img_cursor));
	set_mouse_sprite_focus(x_cursor_focus, y_cursor_focus);
	show_mouse(screen);

	grl_display_background();
	grl_load_list_games();
	grl_display_screen();
}

// Dibuja la imagen de fondo
void GRlDos::grl_display_background()
{
	acquire_screen();
	clear_to_color(img_buf, makecol(235, 235, 235));
	blit(img_bg, img_buf, 0, 0, 0, 0, img_bg->w, img_bg->h);

	if ( color_dep > 16 && use_trans_sprite)
		set_alpha_blender();

//	textprintf_ex(img_buf, grl_font, 296, 48, makecol(0,0,0), -1, "%s ("%s)", GRLDOS_VER, GRLDOS_DATE);

	stretch_sprite(img_buf, img_scroll_rail, btn[btn_up].getPosX(), btn[btn_up].getPosY()+btn[btn_up].getIconH(), btn[btn_up].getIconW(), 
					btn[btn_down].getPosY()-btn[btn_down].getIconH()-btn[btn_up].getPosY());

	for (int id_btn = 1; id_btn < MAX_BOTTONS; id_btn++)
	{
		if ( btn[id_btn].getSowBtn() )
		{
			if ( color_dep > 16 && use_trans_sprite)
			{
				if ( (id_btn == btn_up && id_btn_select == btn_up) || (id_btn == btn_down && id_btn_select == btn_down) || (id_btn == btn_move && id_btn_select == btn_move) )
					draw_trans_sprite(img_buf, btn[id_btn].getIconS(), btn[id_btn].getPosX(), btn[id_btn].getPosY() );
				else
					draw_trans_sprite(img_buf, btn[id_btn].getIconU(), btn[id_btn].getPosX(), btn[id_btn].getPosY() );
			} else {
				if ( (id_btn == btn_up && id_btn_select == btn_up) || (id_btn == btn_down && id_btn_select == btn_down) || (id_btn == btn_move && id_btn_select == btn_move) )
					draw_sprite(img_buf, btn[id_btn].getIconS(), btn[id_btn].getPosX(), btn[id_btn].getPosY() );
				else
					draw_sprite(img_buf, btn[id_btn].getIconU(), btn[id_btn].getPosX(), btn[id_btn].getPosY() );
			}
			if ( id_btn != btn_up || id_btn != btn_down || id_btn != btn_move )
			{
				int f_x = btn[id_btn].getPosX()+(btn[id_btn].getIconW()/2);
				int f_y = btn[id_btn].getPosY()+(btn[id_btn].getIconH()/2)-(text_height(grl_font)/2);
				int f_c = makecol(color_btn_txt_unselect.r, color_btn_txt_unselect.g, color_btn_txt_unselect.b);
				if ( isALFont )
				{
					if ( isFontAa )
						alfont_textout_centre_aa_ex(img_buf, grl_alfont, btn[id_btn].getTitulo().c_str(), f_x, f_y, f_c, -1);
					else
						alfont_textout_centre_ex(img_buf, grl_alfont, btn[id_btn].getTitulo().c_str(), f_x, f_y, f_c, -1);
				} else
					textout_centre_ex(img_buf, grl_font, btn[id_btn].getTitulo().c_str(), f_x, f_y, f_c, -1);
			}
		}
	}
}

// Dibuja la imagen final en pantalla
void GRlDos::grl_display_screen()
{
	scare_mouse();		// Esconde el cursor del ratón
	draw_sprite(screen, img_buf, 0, 0);
	unscare_mouse();	// Muestra de nuevo el cursor del ratón
	release_screen();
}

// Espera a que se pulse otra tecla o un click de ratón
void GRlDos::grl_read_key_or_click()
{
	clear_keybuf();
	do {
		if ( (mouse_b & 1) || (mouse_b & 2) )
			simulate_keypress(key[KEY_V]);
		grl_mouse_info();
	} while ( !keypressed() );
}

// Muesta el botón activado/desactivado
void GRlDos::grl_bottons_state(bool select, int id_btn, bool clear_btns)
{
	int f_x, f_y, f_c;
	if ( select && id_btn != id_btn_select  && !isBtnSelect)
	{
		isBtnSelect = true;
		scare_mouse();	// Esconde el cursor del ratón

		if ( id_btn == btn_move )
			stretch_sprite(screen, img_scroll_rail, btn[btn_up].getPosX(), btn[btn_up].getPosY()+btn[btn_up].getIconH(), btn[btn_up].getIconW(), 
				btn[btn_down].getPosY()-btn[btn_down].getIconH()-btn[btn_up].getPosY());
		else
			blit(img_bg, screen, btn[id_btn].getPosX(), btn[id_btn].getPosY(), btn[id_btn].getPosX(), btn[id_btn].getPosY(), btn[id_btn].getIconW(), btn[id_btn].getIconH());

		if ( color_dep > 16 && use_trans_sprite)
			draw_trans_sprite(screen, btn[id_btn].getIconS(), btn[id_btn].getPosX(), btn[id_btn].getPosY() );
		else
			draw_sprite(screen, btn[id_btn].getIconS(), btn[id_btn].getPosX(), btn[id_btn].getPosY() );

		if ( id_btn != btn_up || id_btn != btn_down || id_btn != btn_move )
		{
			f_x = btn[id_btn].getPosX()+(btn[id_btn].getIconW()/2);
			f_y = btn[id_btn].getPosY()+(btn[id_btn].getIconH()/2)-(text_height(grl_font)/2);
			f_c = makecol(color_btn_txt_select.r, color_btn_txt_select.g, color_btn_txt_select.b);

			if ( isALFont )
			{
				if ( isFontAa )
					alfont_textout_centre_aa_ex(screen, grl_alfont, btn[id_btn].getTitulo().c_str(), f_x, f_y, f_c, -1);
				else
					alfont_textout_centre_ex(screen, grl_alfont, btn[id_btn].getTitulo().c_str(), f_x, f_y, f_c, -1);
			} else
				textout_centre_ex(screen, grl_font, btn[id_btn].getTitulo().c_str(), f_x, f_y, f_c, -1);	
		}

		unscare_mouse();	// Muestra de nuevo el cursor del ratón
		id_btn_select = id_btn;
	} else {
		if ( clear_btns && id_btn == id_btn_select  && isBtnSelect)
		{
			isBtnSelect = false;
			scare_mouse();	// Esconde el cursor del ratón

			if ( id_btn == btn_move )
				stretch_sprite(screen, img_scroll_rail, btn[btn_up].getPosX(), btn[btn_up].getPosY()+btn[btn_up].getIconH(), btn[btn_up].getIconW(), 
					btn[btn_down].getPosY()-btn[btn_down].getIconH()-btn[btn_up].getPosY());
			else
				blit(img_bg, screen, btn[id_btn_select].getPosX(), btn[id_btn_select].getPosY(), btn[id_btn_select].getPosX(), btn[id_btn_select].getPosY(), btn[id_btn_select].getIconW(), btn[id_btn_select].getIconH());

			if ( color_dep > 16 && use_trans_sprite)
				draw_trans_sprite(screen, btn[id_btn_select].getIconU(), btn[id_btn_select].getPosX(),  btn[id_btn_select].getPosY() );
			else
				draw_sprite(screen, btn[id_btn_select].getIconU(), btn[id_btn_select].getPosX(),  btn[id_btn_select].getPosY() );

			if ( id_btn_select != btn_up || id_btn_select != btn_down || id_btn_select != btn_move )
			{
				f_x = btn[id_btn_select].getPosX()+(btn[id_btn_select].getIconW()/2);
				f_y = btn[id_btn_select].getPosY()+(btn[id_btn_select].getIconH()/2)-(text_height(grl_font)/2);
				f_c = makecol(color_btn_txt_unselect.r, color_btn_txt_unselect.g, color_btn_txt_unselect.b);

				if ( isALFont )
				{
					if ( isFontAa )
						alfont_textout_centre_aa_ex(screen, grl_alfont, btn[id_btn].getTitulo().c_str(), f_x, f_y, f_c, -1);
					else
						alfont_textout_centre_ex(screen, grl_alfont, btn[id_btn].getTitulo().c_str(), f_x, f_y, f_c, -1);
				} else
					textout_centre_ex(screen, grl_font, btn[id_btn_select].getTitulo().c_str(), f_x, f_y, f_c, -1);	
			}

			unscare_mouse();	// Muestra de nuevo el cursor del ratón
			id_btn_select = btn_none;
		}
	}
}

// Informacion del raton: posicion cursor/clicks/zonas de clicks
void GRlDos::grl_mouse_info()
{
	int max_num_sow_item;
	get_mouse_mickeys(&mickeyx, &mickeyy);

	if ( num_items < num_sow_item )
		max_num_sow_item = num_items;
	else
		max_num_sow_item = num_sow_item;

	for (int id_btn = 1; id_btn < MAX_BOTTONS; id_btn++)
	{
		if ( btn[id_btn].getSowBtn() && mouse_y >= btn[id_btn].getPosY() && mouse_x >= btn[id_btn].getPosX() && mouse_y <= btn[id_btn].getPosY()+btn[id_btn].getIconH() && mouse_x <= btn[id_btn].getPosX()+btn[id_btn].getIconW() )
		{
			grl_bottons_state(true, id_btn);
			if ( mouse_b & 1 )
			{
				switch (id_btn)
				{
					case btn_play:	// Jugar
						grl_play_game();
					break;
					case btn_conf:	// Conf. Juego
						grl_setup_game();
					break;
					case btn_new:	// Nuevo
						if ( num_items < MAX_JUEGOS )
							grl_add_edit_game(false);
					break;
					case btn_edit:	// Editar
						if ( id_game > -1 && num_items > -1 && !info[id_game].getEliminado() )
							grl_add_edit_game(true);
					break;
					case btn_del:	// Eliminar
						grl_delete_game();
					break;
					case btn_zip:	// Descomprimir
						grl_decompress_file();
					break;
					case btn_opt:	// Opciones
						grl_options();
					break;
					case btn_help:	// Ayuda
						grl_about_help(true);
					break;
					case btn_acde:	// Acerca de
						grl_about_help(false);
					break;
					case btn_exit:	// Salir
						remove("ejecutar.bat");
						isPlay = false;
						isExit = true;
					break;
					case btn_up:	// Subir
						grl_item_key_up();
					break;
					case btn_down:	// Bajar
						grl_item_key_down();
					break;
					case btn_move: // Mueve la barra de scroll
						isBtnMove = true;
						if ( mickeyy > 2 )
						{
							isBtnMoveUp   = false;
							isBtnMoveDown = true;
						}
						else if ( mickeyy < -2 )
						{
							isBtnMoveUp   = true;
							isBtnMoveDown = false;
						}
					break;
					default:
						isBtnMove = false;
					break;
				}
			} else {
				isBtnMove     = false;
				isBtnMoveUp   = false;
				isBtnMoveDown = false;
			}
		} else {
			isBtnMove = false;
			grl_bottons_state(false, id_btn, true);
		}
	}

	if( isBtnMove )
	{
		if ( isBtnMoveUp )
		{
			position_mouse(btn[btn_move].getPosX()+(btn[btn_move].getIconW()/2), btn[btn_move].getPosY()+(btn[btn_move].getIconH()/2));
			grl_item_key_up(true);
		}
		else if ( isBtnMoveDown )
		{
			position_mouse(btn[btn_move].getPosX()+(btn[btn_move].getIconW()/2), btn[btn_move].getPosY()+(btn[btn_move].getIconH()/2));
			grl_item_key_down(true);
		}
	} else {
		if ( center_scroll != pos_scroll )
			btn[btn_move].setPosY(center_scroll);
	}

	if ( mouse_y >= y_item && mouse_x >= x_item && mouse_y <= y_item+(max_num_sow_item*(h_item+s_item))-s_item && mouse_x <= x_item+w_item )
	{
		if ( mouse_b & 1 )
		{
			if ( (mouse_y - y_item) > 0 )
				ptr = (mouse_y - y_item) / (h_item + s_item);
			else
				ptr = 0;

			if ( new_ptr != ptr )
			{
				id_game = top + ptr;
				grl_display_background();
				grl_display_list_games(top);
				grl_display_screen();
				new_ptr = ptr;
			}
		}
		if ( (mouse_b & 2) )
		{
			grl_play_game();
		}
	}
}

// Carga la lista de juegos
void GRlDos::grl_load_list_games()
{
	if ( !fileExists("lista.xml") )
	{
		ofstream myfile;
		myfile.open("lista.xml");
		if ( myfile.is_open() )
		{
			myfile << "" << endl;
			myfile.flush();
			myfile.close();
		}
	}

	XmlNodes xml_list_games;
	XmlNodes::iterator itr;
	xml_list_games = loadXmlNodes("lista.xml");
	total_games = xml_list_games.size();

	if ( total_games > 0 )
	{
		int porcentaje = 0;
		int f_x = width_grl/2;
		int f_y = height_grl/2;
		int f_c = makecol(255,255,255);
		if ( id_game > (total_games-1) )
		{
			id_game = 0;
			ptr     = 0;
			new_ptr = ptr;
			top     = 0;
		}
		num_items = 0;
		info[num_items].setEliminado(true);
		for ( itr = xml_list_games.begin(); itr != xml_list_games.end(); ++itr )
		{
			if ( num_items < MAX_JUEGOS && StrCompare(itr->tagName, "game") )
			{
				info[num_items].setEliminado();
				info[num_items].setTitulo    ( itr->getAttrStr("title"  , lng.list_game_no_title) );
				info[num_items].setAnno      ( itr->getAttrStr("year"   , "") );
				info[num_items].setCompania  ( itr->getAttrStr("company", "") );
				info[num_items].setGenero    ( itr->getAttrStr("genere" , "") );
				info[num_items].setCover     ( StrReplace( itr->getAttrStr("cover", ""), "/", "\\") );
		//	if ( sow_item_icon )
		//		info[num_items].setIcon      ( StrReplace( itr->getAttrStr("icon" , ""), "/", "\\"), w_item_icon, h_item_icon, color_dep);
				info[num_items].setIconStr   ( StrReplace( itr->getAttrStr("icon" , ""), "/", "\\") );
				info[num_items].setPath      ( StrReplace( itr->getAttrStr("path" , ""), "/", "\\") );
				info[num_items].setParam     ( itr->getAttrStr("param", "") );
				info[num_items].setPathSetup ( StrReplace( itr->getAttrStr("path_setup", ""), "/", "\\") );
				info[num_items].setParamSetup( itr->getAttrStr("param_setup", "") );
				info[num_items].setCmdBefore ( StrReplace( itr->getAttrStr("cmd_before", ""), "/", "\\") );
				info[num_items].setCmdAfter  ( StrReplace( itr->getAttrStr("cmd_after" , ""), "/", "\\") );

				if ( num_items < num_sow_item && top < 1 )
					grl_display_game_item(num_items, x_item, y_item+(num_items*(h_item+s_item)), num_items==id_game?true:false);
				num_items++;
				porcentaje = num_items*100/total_games;

				if ( isALFont )
				{
					if ( isFontAa )
						alfont_textprintf_centre_aa_ex(screen, grl_alfont, f_x, f_y, f_c, 0, "  %s %i de %i - %i%%  ", lng.list_game_load.c_str(), num_items, total_games, porcentaje);
					else
						alfont_textprintf_centre_ex(screen, grl_alfont, f_x, f_y, f_c, 0, "  %s %i de %i - %i%%  ", lng.list_game_load.c_str(), num_items, total_games, porcentaje);
				} else
					textprintf_centre_ex(screen, grl_font, f_x, f_y, f_c, 0, "  %s %i de %i - %i%%  ", lng.list_game_load.c_str(), num_items, total_games, porcentaje);
			}
		}
	}

	if ( top > 0 )
		grl_display_list_games(top);
	else
		grl_display_count_item();
}

// Guarda la lista de juegos
void GRlDos::grl_save_list_games()
{
	ofstream myfile;
	myfile.open("lista.xml");
	string m_cover, m_icon;
	if ( myfile.is_open() )
	{
		for ( int i = 0; i < num_items; i++ )
		{
			if ( !info[i].getEliminado() )
			{
				myfile << "<game ";
				myfile << "title=\"" << info[i].getTitulo() << "\" ";
				myfile << "year=\"" << info[i].getAnno() << "\" ";
				myfile << "company=\"" << info[i].getCompania() << "\" ";
				myfile << "genere=\"" << info[i].getGenero() << "\" ";
				myfile << "cover=\"" << StrReplace(info[i].getCover(), "\\", "/") << "\" ";
				myfile << "icon=\"" << StrReplace(info[i].getIconStr(), "\\", "/") << "\" ";
				myfile << "path=\"" << StrReplace(info[i].getPath() , "\\", "/") << "\" ";
				myfile << "param=\"" << info[i].getParam() << "\" ";
				myfile << "path_setup=\"" << StrReplace(info[i].getPathSetup(), "\\", "/") << "\" ";
				myfile << "param_setup=\"" << info[i].getParamSetup() << "\" ";
				myfile << "cmd_before=\"" << StrReplace(info[i].getCmdBefore(), "\\", "/") << "\" ";
				myfile << "cmd_after=\"" << StrReplace(info[i].getCmdAfter(), "\\", "/") << "\" ";
				myfile << "/>" << endl;
			}
		}
		myfile.flush();
		myfile.close();
	}
}

// Obtiene la información del ejecutable juego
void GRlDos::grl_game_exe_info(int id)
{
	isExec  = false;
	f_exe   = "";
	f_ext   = "";
	f_path  = "";
	f_param = "";

	isSetup       = false;
	f_exe_setup   = "";
	f_ext_setup   = "";
	f_path_setup  = "";
	f_param_setup = "";

	if ( id > -1 && !info[id].getEliminado() )
	{
		if ( sow_cover )
			stretch_sprite(img_buf, openImage(info[id].getCover()), x_cover, y_cover, w_cover, h_cover);

		if ( fileExists( info[id].getPath() ) )
		{
			isExec  = true;
			f_exe   = fileName( info[id].getPath() );
			f_ext   = fileExt( f_exe );
			f_path  = filePath( info[id].getPath() ).erase(0, 3);
			f_param = info[id].getParam();
		}

		if ( fileExists( info[id].getPathSetup() ) )
		{
			isSetup       = true;
			f_exe_setup   = fileName( info[id].getPathSetup() );
			f_ext_setup   = fileExt( f_exe_setup );
			f_path_setup  = filePath( info[id].getPathSetup() ).erase(0, 3);
			f_param_setup = info[id].getParamSetup();
		}
	}
}

// Muestra el numero de items y el seleccionado
void GRlDos::grl_display_count_item(bool isScreen)
{
	if ( sow_num_items )
	{
		char buffer[20];
		sprintf(buffer, "%04d - %04d", total_games>0?id_game+1:0, total_games>0?num_items:0);
		blit(img_bg, isScreen?screen:img_buf, pos_x_num_items, pos_y_num_items, pos_x_num_items, pos_y_num_items, text_length(grl_font, buffer), text_height(grl_font));
		if ( isALFont )
		{
			if ( isFontAa )
				alfont_textout_aa_ex(isScreen?screen:img_buf, grl_alfont, buffer, pos_x_num_items, pos_y_num_items, makecol(color_num_items.r, color_num_items.g, color_num_items.b), -1);
			else
				alfont_textout_ex(isScreen?screen:img_buf, grl_alfont, buffer, pos_x_num_items, pos_y_num_items, makecol(color_num_items.r, color_num_items.g, color_num_items.b), -1);
		} else
			textout_ex(isScreen?screen:img_buf, grl_font, buffer, pos_x_num_items, pos_y_num_items, makecol(color_num_items.r, color_num_items.g, color_num_items.b), -1);
	}
}

// Dibuja un item del juego
void GRlDos::grl_display_game_item(int id, int x, int y, bool select, bool isScreen)
{
	int color_select;
	if ( info[id].getEliminado() )
	{
		if ( select )
		{
			if ( color_dep > 16 && use_trans_sprite )
				draw_trans_sprite(isScreen?screen:img_buf, img_item_list_del_s, x, y);
			else
				draw_sprite(isScreen?screen:img_buf, img_item_list_del_s, x, y);
			color_select = makecol(color_item_txt_delete_s.r, color_item_txt_delete_s.g, color_item_txt_delete_s.b);
		} else {
			if ( color_dep > 16 && use_trans_sprite )
				draw_trans_sprite(isScreen?screen:img_buf, img_item_list_del_u, x, y);
			else
				draw_sprite(isScreen?screen:img_buf, img_item_list_del_u, x, y);
			color_select = makecol(color_item_txt_delete_u.r, color_item_txt_delete_u.g, color_item_txt_delete_u.b);
		}
	} else {
		if ( select )
		{
			grl_game_exe_info(id);
			if ( color_dep > 16 && use_trans_sprite )
				draw_trans_sprite(isScreen?screen:img_buf, img_item_list_s, x, y);
			else
				draw_sprite(isScreen?screen:img_buf, img_item_list_s, x, y);
			color_select = makecol(color_item_txt_select.r, color_item_txt_select.g, color_item_txt_select.b);
		} else {
			if ( color_dep > 16 && use_trans_sprite )
				draw_trans_sprite(isScreen?screen:img_buf, img_item_list_u, x, y);
			else
				draw_sprite(isScreen?screen:img_buf, img_item_list_u, x, y);
			color_select = makecol(color_item_txt_unselect.r, color_item_txt_unselect.g, color_item_txt_unselect.b);
		}
	}

	if ( isALFont )
	{
		if ( isFontAa )
		{
			alfont_textprintf_aa_ex(isScreen?screen:img_buf, grl_alfont, x+x_item_txt_title, y+y_item_txt_title, color_select, -1, "%s", StrLimit(info[id].getTitulo(), max_caracter_title).c_str() );

			if ( sow_year )
				alfont_textprintf_aa_ex(isScreen?screen:img_buf, grl_alfont, x+x_item_txt_year, y+y_item_txt_year, color_select, -1, "%s", info[id].getAnno().c_str() );

			if ( sow_company )
				alfont_textprintf_aa_ex(isScreen?screen:img_buf, grl_alfont, x+x_item_txt_company, y+y_item_txt_company, color_select, -1, "%s", StrLimit(info[id].getCompania(), max_caracter_company).c_str() );

			if ( sow_separate )
				alfont_textprintf_aa_ex(isScreen?screen:img_buf, grl_alfont, x+x_item_txt_separate, y+y_item_txt_separate, color_select, -1, "%s", item_txt_separate.c_str() );
		} else {
			alfont_textprintf_ex(isScreen?screen:img_buf, grl_alfont, x+x_item_txt_title, y+y_item_txt_title, color_select, -1, "%s", StrLimit(info[id].getTitulo(), max_caracter_title).c_str() );

			if ( sow_year )
				alfont_textprintf_ex(isScreen?screen:img_buf, grl_alfont, x+x_item_txt_year, y+y_item_txt_year, color_select, -1, "%s", info[id].getAnno().c_str() );

			if ( sow_company )
				alfont_textprintf_ex(isScreen?screen:img_buf, grl_alfont, x+x_item_txt_company, y+y_item_txt_company, color_select, -1, "%s", StrLimit(info[id].getCompania(), max_caracter_company).c_str() );

			if ( sow_separate )
				alfont_textprintf_ex(isScreen?screen:img_buf, grl_alfont, x+x_item_txt_separate, y+y_item_txt_separate, color_select, -1, "%s", item_txt_separate.c_str() );
		}
	} else {
		textprintf_ex(isScreen?screen:img_buf, grl_font, x+x_item_txt_title, y+y_item_txt_title, color_select, -1, "%s", StrLimit(info[id].getTitulo(), max_caracter_title).c_str() );

		if ( sow_year )
			textprintf_ex(isScreen?screen:img_buf, grl_font, x+x_item_txt_year, y+y_item_txt_year, color_select, -1, "%s", info[id].getAnno().c_str() );
		if ( sow_company )
			textprintf_ex(isScreen?screen:img_buf, grl_font, x+x_item_txt_company, y+y_item_txt_company, color_select, -1, "%s", StrLimit(info[id].getCompania(), max_caracter_company).c_str() );
		if ( sow_separate )
			textprintf_ex(isScreen?screen:img_buf, grl_font, x+x_item_txt_separate, y+y_item_txt_separate, color_select, -1, "%s", item_txt_separate.c_str() );
	}

	if ( sow_item_icon )
	{
		img_icon = openImage(info[id].getIconStr(), NULL, true, 32, 32, color_dep);

		if ( color_dep > 16 && use_trans_sprite )
			draw_trans_sprite(isScreen?screen:img_buf, img_icon, x+x_item_icon, y+y_item_icon);
		else
			draw_sprite(isScreen?screen:img_buf, img_icon, x+x_item_icon, y+y_item_icon);

		destroy_bitmap(img_icon);
	}
}

// Dibuja la lista de juegos
void GRlDos::grl_display_list_games(int top_)
{
	grl_display_count_item();

	int pos = 0;
	for ( int id = top_; id < (top_ + num_sow_item); id++ )
	{
		if ( id < num_items )
		{
			if ( id == id_game )
				grl_display_game_item(id, x_item, y_item+(pos*(h_item+s_item)), true);
			else
				grl_display_game_item(id, x_item, y_item+(pos*(h_item+s_item)), false);
		}
		pos++;
	}
}

// Sube un item seleccionado
void GRlDos::grl_item_key_up(bool isNext)
{
	pos_scroll = top_scroll;
	btn[btn_move].setPosY(pos_scroll);
	if ( id_game > -1 && num_items > -1 )
	{
		grl_display_background();
		if ( isNext )
		{
			ptr = 0;
			if ( top > 0 )
				top = top - num_sow_item;
		} else {
			if ( ptr > 0 )
				ptr--;
			else {
				if ( top > 0 )
					top--;
			}
		}
		if ( top < 1 )
			top = 0;

		new_ptr = ptr;
		id_game = top + ptr;
		grl_display_list_games(top);
		grl_display_screen();
	}
}

// Baja un item seleccionado
void GRlDos::grl_item_key_down(bool isNext)
{
	pos_scroll = bot_scroll;
	btn[btn_move].setPosY(pos_scroll);
	if ( num_items > -1 )
	{
		grl_display_background();
		if ( isNext )
		{
			ptr = 0;
			if ( (top + num_sow_item) < total_games )
				top = top + num_sow_item;
		} else {
			if ( ptr < (num_sow_item-1) && ptr < ((total_games-1) - top) )
			{
				ptr++;
				new_ptr = ptr;
			} else {
				if ( (top + num_sow_item) < total_games )
					top++;
			}
		}
		if ( total_games > num_sow_item && (top + num_sow_item) > total_games  )
			top = total_games-num_sow_item;

		new_ptr = ptr;
		id_game = top + ptr;
		grl_display_list_games(top);
		grl_display_screen();
	}
}

// Ejecuta el juego
void GRlDos::grl_play_game()
{
	if ( id_game > -1 && num_items > -1 && isExec )
	{
		ofstream myfile;
		myfile.open("ejecutar.bat");
		if ( myfile.is_open() )
		{
			myfile << StrReplace(info[id_game].getCmdBefore(), cmd_new_linea, "\n") << endl;

			myfile << "cd \\" << endl;
			myfile << "cd " << f_path << endl;
			if ( f_ext == "bat" )
				myfile << "call " << f_exe << " " << f_param << endl;
			else
				myfile << f_exe << " " << f_param  << endl;

			myfile << StrReplace(info[id_game].getCmdAfter(), cmd_new_linea, "\n") << endl;

			myfile << "cd \\" << endl;
			myfile << "cd grldos" << endl;
			myfile.flush();
			myfile.close();
		}
		isPlay = true;
		isExit = true;
	} else
		remove("ejecutar.bat");
}

// Ejecuta el setup del juego
void GRlDos::grl_setup_game()
{
	if ( id_game > -1 && num_items > -1 && isSetup )
	{
		ofstream myfile;
		myfile.open("ejecutar.bat");
		if ( myfile.is_open() )
		{
			myfile << StrReplace(info[id_game].getCmdBefore(), cmd_new_linea, "\n") << endl;

			myfile << "cd \\" << endl;
			myfile << "cd " << f_path_setup << endl;
			if ( f_ext_setup == "bat" )
				myfile << "call " << f_exe_setup << " " << f_param_setup << endl;
			else
				myfile << f_exe_setup << " " << f_param_setup  << endl;

			myfile << StrReplace(info[id_game].getCmdAfter(), cmd_new_linea, "\n") << endl;

			myfile << "cd \\" << endl;
			myfile << "cd grldos" << endl;
			myfile.flush();
			myfile.close();
		}
		isPlay = true;
		isExit = true;
	} else
		remove("ejecutar.bat");
}

// Carga el Idioma del Programa
void GRlDos::grl_load_language(string filename)
{
	string dir_file = "lng\\";
	dir_file.append(filename);

	if ( fileExists(dir_file) )
	{
		cfgOpen(dir_file.c_str(), MS_STYLE);
	// Dialogo comun
		cfgSelectSection("dlg_main");
		lng.dlg_main_btn_ok     = cfgReadText("btn_ok"    , "Aceptar");
		lng.dlg_main_btn_cancel = cfgReadText("btn_cancel", "Cancelar");

	// Dialogo Eliminar
		cfgSelectSection("dlg_delete");
		lng.dlg_delete_title   = cfgReadText("title"  , "¿Eliminar?");
		lng.dlg_delete_mensage = cfgReadText("mensage", "¿Deseas eliminar de la lista?");

	// Dialogo Añadir/Editar
		cfgSelectSection("dlg_add_edit");
		lng.dlg_add_edit_title_add         = cfgReadText("title_add"        , "Añadiendo Nuevo Juego");
		lng.dlg_add_edit_title_edit        = cfgReadText("title_edit"       , "Editando:");
		lng.dlg_add_edit_lb_title          = cfgReadText("lb_title"         , "Titulo:");
		lng.dlg_add_edit_lb_year           = cfgReadText("lb_year"          , "Año:");
		lng.dlg_add_edit_lb_company        = cfgReadText("lb_company"       , "Compañia:");
		lng.dlg_add_edit_lb_genere         = cfgReadText("lb_genere"        , "Genero:");
		lng.dlg_add_edit_lb_sep_image      = cfgReadText("lb_sep_image"     , "Imágenes");
		lng.dlg_add_edit_open_file_image   = cfgReadText("open_file_image"  , "Seleciona una imágen");
		lng.dlg_add_edit_lb_cover          = cfgReadText("lb_cover"         , "Cover:");
		lng.dlg_add_edit_lb_icon           = cfgReadText("lb_icon"          , "Icono:");
		lng.dlg_add_edit_lb_sep_path       = cfgReadText("lb_sep_path"      , "Ejecutable (EXE, COM, BAT)");
		lng.dlg_add_edit_lb_sep_path_setup = cfgReadText("lb_sep_path_setup", "Setup (EXE, COM, BAT)");
		lng.dlg_add_edit_open_file_exe     = cfgReadText("open_file_exe"    , "Seleciona el ejecutable");
		lng.dlg_add_edit_lb_path           = cfgReadText("lb_path"          , "Dirección:");
		lng.dlg_add_edit_lb_param_exe      = cfgReadText("lb_param_exe"     , "Parametros:");
		lng.dlg_add_edit_lb_command_lines  = cfgReadText("lb_command_lines" , "Lineas de comandos");
		lng.dlg_add_edit_lb_before_after   = cfgReadText("lb_before_after"  , "Antes y Despues de ejecutar el juego");
		lng.dlg_add_edit_lb_before         = cfgReadText("lb_before"        , "Antes:");
		lng.dlg_add_edit_lb_after          = cfgReadText("lb_after"         , "Despues:");
		lng.dlg_add_edit_lb_newline        = cmd_new_linea;
		lng.dlg_add_edit_lb_newline.append( " " );
		lng.dlg_add_edit_lb_newline.append( cfgReadText("lb_newline", "Para el salto de linea usar") );

	// Dialogo Opciones
		cfgSelectSection("dlg_options");
		lng.dlg_option_title           = cfgReadText("title"          , "Opciones del GRlDOs");
		lng.dlg_option_lb_language     = cfgReadText("lb_language"    , "Idioma:");
		lng.dlg_option_lb_skins        = cfgReadText("lb_skins"       , "Parametros:");
		lng.dlg_option_lb_info_ext_zip = cfgReadText("lb_info_ext_zip", "Tienen que estar separados por ;");

	// Dialogo Descomprimir
		cfgSelectSection("dlg_extract");
		lng.dlg_extract_title                 = cfgReadText("title"                , "Descomprimir archivo");
		lng.dlg_extract_lb_exe                = cfgReadText("lb_exe"               , "Programa:");
		lng.dlg_extract_lb_source             = cfgReadText("lb_source"            , "Origen:");
		lng.dlg_extract_open_file_source      = cfgReadText("open_file_source"     , "Selecciona el archivo de origen");
		lng.dlg_extract_lb_destination        = cfgReadText("lb_destination"       , "Destino:");
		lng.dlg_extract_open_file_destination = cfgReadText("open_file_destination", "Selecciona el destino");

	// Dialogo Acerca de..
		cfgSelectSection("dlg_about");
		lng.dlg_about_title                  = cfgReadText("title"                 , "Acerca de GRlDOs");
		lng.dlg_about_lb_designed_programmed = cfgReadText("lb_designed_programmed", "Diseñado y Programado por:");
		lng.dlg_about_lb_thanks              = cfgReadText("lb_thanks"             , "Agradecimiento:");
		lng.dlg_about_lb_developers          = cfgReadText("lb_developers"         , "Los desarrolladores");

	// Dialogo Ayuda
		cfgSelectSection("dlg_help");
		lng.dlg_help_title   = cfgReadText("title"  , "Ayuda de GRlDOs");
		lng.dlg_help_mensage = cfgReadText("mensage", defect_msg_help.c_str() );

	// Otros
		cfgSelectSection("list_game");
		lng.list_game_no_title = cfgReadText("no_title", "Juego sin Titulo");
		lng.list_game_load     = cfgReadText("load"    , "Cargando lista:" );
	
	// Etiquetas para los botones
		cfgSelectSection("grl_skin_bottons");
		string str_title_btn;
		for (int id_btn = 1; id_btn < MAX_BOTTONS; id_btn++)
		{
			str_title_btn = StrAppend( "lb_title_btn_", IntToStr(id_btn) );
			lb_btn[id_btn].titulo = cfgReadText(str_title_btn.c_str(), "");
		}

		cfgClose();
	} else {
	// Dialogo comun
		lng.dlg_main_btn_ok     = "Aceptar";
		lng.dlg_main_btn_cancel = "Cancelar";

	// Dialogo Eliminar
		lng.dlg_delete_title   = "¿Eliminar?";
		lng.dlg_delete_mensage = "¿Deseas eliminar de la lista?";

	// Dialogo Añadir/Editar
		lng.dlg_add_edit_title_add         = "Añadiendo Nuevo Juego";
		lng.dlg_add_edit_title_edit        = "Editando:";
		lng.dlg_add_edit_lb_title          = "Titulo:";
		lng.dlg_add_edit_lb_year           = "Año:";
		lng.dlg_add_edit_lb_company        = "Compañia:";
		lng.dlg_add_edit_lb_genere         =  "Genero:";
		lng.dlg_add_edit_lb_sep_image      = "Imágenes";
		lng.dlg_add_edit_open_file_image   = "Selecciona una imágen";
		lng.dlg_add_edit_lb_cover          = "Cover:";
		lng.dlg_add_edit_lb_icon           = "Icono:";
		lng.dlg_add_edit_lb_sep_path       = "Ejecutable (EXE, COM, BAT)";
		lng.dlg_add_edit_lb_sep_path_setup = "Setup (EXE, COM, BAT)";
		lng.dlg_add_edit_open_file_exe     = "Selecciona el ejecutable";
		lng.dlg_add_edit_lb_path           = "Dirección:";
		lng.dlg_add_edit_lb_param_exe      = "Parametros:";
		lng.dlg_add_edit_lb_command_lines  = "Lineas de comandos";
		lng.dlg_add_edit_lb_before_after   = "Antes y Despues de ejecutar el juego";
		lng.dlg_add_edit_lb_before         = "Antes:";
		lng.dlg_add_edit_lb_after          = "Despues:";
		lng.dlg_add_edit_lb_newline        = "# Para escribir en una nueva linea.";

	// Dialogo Opciones
		lng.dlg_option_title           = "Opciones del GRlDOs";
		lng.dlg_option_lb_language     = "Idioma:";
		lng.dlg_option_lb_skins        = "Parametros:";
		lng.dlg_option_lb_info_ext_zip = "Tienen que estar separados por ;";

	// Dialogo Descomprimir
		lng.dlg_extract_title                 = "Descomprimir archivo";
		lng.dlg_extract_lb_exe                = "Programa:";
		lng.dlg_extract_lb_source             = "Origen:";
		lng.dlg_extract_open_file_source      = "Selecciona el archivo de origen";
		lng.dlg_extract_lb_destination        = "Destino:";
		lng.dlg_extract_open_file_destination = "Selecciona el destino";

	// Dialogo Acerca de..
		lng.dlg_about_title                  = "Acerca de GRlDOs";
		lng.dlg_about_lb_designed_programmed = "Diseñado y Programado por:";
		lng.dlg_about_lb_thanks              = "Agradecimiento:";
		lng.dlg_about_lb_developers          = "Los desarrolladores";

	// Dialogo Ayuda
		lng.dlg_help_title   = "Ayuda de GRlDOs";
		lng.dlg_help_mensage = defect_msg_help;

	// Otros
		lng.list_game_no_title = "Juego sin Titulo";
		lng.list_game_load     = "Cargando lista:";
	}
}

// Carga un tipo de Letra o fuente en PCX o TTF
void GRlDos::grl_open_font(string f_str, int f_size, bool f_aa)
{
	isALFont   = false;
	isFontAa   =  f_aa;
	grl_font   =  font;
	grl_alfont =  NULL;

	if ( !StrCompare( f_str, "") )
	{
		f_str = StrPreAppend(f_str, "skins\\fonts\\");
		if (fileExists(f_str))
		{
			string f_ext = fileExt(f_str);
			if ( f_ext == "pcx" )
				grl_font = load_font(f_str.c_str(), pal_font, NULL);
			else if ( f_ext == "ttf")
			{
				isALFont   = true;
				grl_alfont = alfont_load_font( f_str.c_str() );
				alfont_set_font_size(grl_alfont, f_size);
			}
		}
	}
}

// Carga la apariencia del Programa
void GRlDos::grl_load_skin(string filename)
{
	string dir_skin = "skins\\";
	dir_skin.append(filename);

	if ( fileExists(dir_skin) )
	{
		cfgOpen(dir_skin.c_str(), MS_STYLE);
		cfgSelectSection("grl_skin_general");
		color_dep_recomend   = cfgReadInt("color_dep_recomend"  ,   16);
		f_img_bg             = StrPreAppend(cfgReadText("img_bg", "grldos\\fondo.png"), "skins\\");
		num_sow_item         = cfgReadInt("num_sow_item"        ,    9);
		max_caracter_title   = cfgReadInt("max_caracter_title"  ,   40);
		max_caracter_company = cfgReadInt("max_caracter_company",   34);
		sow_company          = cfgReadBool("sow_company"        , true);
		sow_year             = cfgReadBool("sow_year"           , true);
		sow_separate         = cfgReadBool("sow_separate"       , true);
		item_txt_separate    = cfgReadText("item_txt_separate"  ,  "-");
		sow_num_items        = cfgReadBool("sow_num_items"      , true);
		pos_x_num_items      = cfgReadInt("pos_x_num_items"     ,  520);
		pos_y_num_items      = cfgReadInt("pos_y_num_items"     ,   23);
		color_num_items      = convertToRGB( cfgReadText("color_num_items", "226, 226, 218") );

		cfgSelectSection("grl_skin_font");
		grl_open_font(cfgReadText("grl_font", ""), cfgReadInt("font_size", 12), cfgReadBool("font_antialiasing", true));

		cfgSelectSection("grl_skin_mouse");
		f_img_cursor   = StrPreAppend(cfgReadText("img_cursor", "grldos\\cursor.gif"), "skins\\");
		x_cursor_focus = cfgReadInt("x_cursor_focus" ,  16);
		y_cursor_focus = cfgReadInt("y_cursor_focus" ,  16);

		cfgSelectSection("grl_skin_bottons");
		color_btn_txt_select   = convertToRGB( cfgReadText("color_btn_txt_select"  , "41, 41, 41") );
		color_btn_txt_unselect = convertToRGB( cfgReadText("color_btn_txt_unselect", "128, 128, 128") );
		f_img_scroll_rail      = StrPreAppend(cfgReadText("img_scroll_rail", "grldos\\sc_rail.png"), "skins\\");

		string str_img_btn_s, str_img_btn_u, str_x_btn, str_y_btn, str_w_btn, str_h_btn, str_sow_btn;

		btn[btn_none].setTitulo("");
		btn[btn_none].setIconSize(1, 1);
		btn[btn_none].setIconS("", color_dep);
		btn[btn_none].setIconU("", color_dep);
		btn[btn_none].setPos(0, 0);
		btn[btn_none].setSowBtn(false);

		for (int id_btn = 1; id_btn < MAX_BOTTONS; id_btn++)
		{
			str_img_btn_s = StrAppend( "img_btn_s_", IntToStr(id_btn) );
			str_img_btn_u = StrAppend( "img_btn_u_", IntToStr(id_btn) );
			str_x_btn     = StrAppend( "x_btn_"    , IntToStr(id_btn) );
			str_y_btn     = StrAppend( "y_btn_"    , IntToStr(id_btn) );
			str_w_btn     = StrAppend( "w_btn_"    , IntToStr(id_btn) );
			str_h_btn     = StrAppend( "h_btn_"    , IntToStr(id_btn) );
			str_sow_btn   = StrAppend( "sow_btn_"  , IntToStr(id_btn) );

			if ( id_btn == btn_up )
			{
				btn[btn_up].setTitulo("");
				btn[btn_up].setIconSize(cfgReadInt(str_w_btn.c_str(), 16), cfgReadInt(str_h_btn.c_str(), 20));
				btn[btn_up].setIconS(StrPreAppend(cfgReadText(str_img_btn_s.c_str(), "grldos\\b_up_s.png"), "skins\\"), color_dep);
				btn[btn_up].setIconU(StrPreAppend(cfgReadText(str_img_btn_u.c_str(), "grldos\\b_up_u.png"), "skins\\"), color_dep);
				btn[btn_up].setPos(cfgReadInt(str_x_btn.c_str(), 450), cfgReadInt(str_y_btn.c_str(), 63));
			}
			else if ( id_btn == btn_down )
			{
				btn[btn_down].setTitulo("");
				btn[btn_down].setIconSize(cfgReadInt(str_w_btn.c_str(), 16), cfgReadInt(str_h_btn.c_str(), 20));
				btn[btn_down].setIconS(StrPreAppend(cfgReadText(str_img_btn_s.c_str(), "grldos\\b_down_s.png"), "skins\\"), color_dep);
				btn[btn_down].setIconU(StrPreAppend(cfgReadText(str_img_btn_u.c_str(), "grldos\\b_down_u.png"), "skins\\"), color_dep);
				btn[btn_down].setPos(cfgReadInt(str_x_btn.c_str(), 450), cfgReadInt(str_y_btn.c_str(), 435));
			}
			else if ( id_btn == btn_move )
			{
				btn[btn_move].setTitulo("");
				btn[btn_move].setIconSize(cfgReadInt(str_w_btn.c_str(), 16), cfgReadInt(str_h_btn.c_str(), 196));
				btn[btn_move].setIconS(StrPreAppend(cfgReadText(str_img_btn_s.c_str(), "grldos\\sc_mov_s.png"), "skins\\"), color_dep);
				btn[btn_move].setIconU(StrPreAppend(cfgReadText(str_img_btn_u.c_str(), "grldos\\sc_mov_u.png"), "skins\\"), color_dep);
				btn[btn_move].setPos(cfgReadInt(str_x_btn.c_str(), 450), cfgReadInt(str_y_btn.c_str(), 83));
			} else {
				btn[id_btn].setTitulo( lb_btn[id_btn].titulo );
				btn[id_btn].setIconSize(cfgReadInt(str_w_btn.c_str(), 110), cfgReadInt(str_h_btn.c_str(), 24));
				btn[id_btn].setIconS(StrPreAppend(cfgReadText(str_img_btn_s.c_str(), "grldos\\btn_s.png"), "skins\\"), color_dep);
				btn[id_btn].setIconU(StrPreAppend(cfgReadText(str_img_btn_u.c_str(), "grldos\\btn_u.png"), "skins\\"), color_dep);
				btn[id_btn].setPos(cfgReadInt(str_x_btn.c_str(), 510), cfgReadInt(str_y_btn.c_str(), 66+(id_btn*26)));
			}
			btn[id_btn].setSowBtn(cfgReadBool(str_sow_btn.c_str(), true));
		}

		cfgSelectSection("grl_skin_item");
		f_img_item_list_s     = StrPreAppend(cfgReadText("img_item_list_s"    , "grldos\\item_s.png" ), "skins\\");
		f_img_item_list_u     = StrPreAppend(cfgReadText("img_item_list_u"    , "grldos\\item_u.png" ), "skins\\");
		f_img_item_list_del_s = StrPreAppend(cfgReadText("img_item_list_del_s", "grldos\\i_del_s.png"), "skins\\");
		f_img_item_list_del_u = StrPreAppend(cfgReadText("img_item_list_del_u", "grldos\\i_del_u.png"), "skins\\");

		color_item_txt_select   = convertToRGB( cfgReadText("color_item_txt_select"  , "41, 41, 41"   ) );
		color_item_txt_unselect = convertToRGB( cfgReadText("color_item_txt_unselect", "128, 128, 128") );
		color_item_txt_delete_s = convertToRGB( cfgReadText("color_item_txt_delete_s", "135, 78, 78"  ) );
		color_item_txt_delete_u = convertToRGB( cfgReadText("color_item_txt_delete_u", "222, 108, 108") );

		x_item_txt_title    = cfgReadInt("x_item_txt_title"   , 44);
		y_item_txt_title    = cfgReadInt("y_item_txt_title"   ,  8);
		x_item_txt_year     = cfgReadInt("x_item_txt_year"    , 44);
		y_item_txt_year     = cfgReadInt("y_item_txt_year"    , 25);
		x_item_txt_company  = cfgReadInt("x_item_txt_company" , 92);
		y_item_txt_company  = cfgReadInt("y_item_txt_company" , 25);
		x_item_txt_separate = cfgReadInt("x_item_txt_separate", 80);
		y_item_txt_separate = cfgReadInt("y_item_txt_separate", 25);

		sow_item_center = cfgReadBool("sow_item_center", false);
		x_item          = cfgReadInt("x_item",  44);
		y_item          = cfgReadInt("y_item",  63);
		w_item          = cfgReadInt("w_item", 396);
		h_item          = cfgReadInt("h_item",  40);
		s_item          = cfgReadInt("s_item",   4);

		sow_item_icon = cfgReadBool("sow_item_icon", true);
		x_item_icon   = cfgReadInt("x_item_icon",  6);
		y_item_icon   = cfgReadInt("y_item_icon",  4);
		w_item_icon   = cfgReadInt("w_item_icon", 32);
		h_item_icon   = cfgReadInt("h_item_icon", 32);

		cfgSelectSection("grl_skin_cover");
		sow_cover = cfgReadBool("sow_cover", false);
		x_cover   = cfgReadInt("x_cover", 448);
		y_cover   = cfgReadInt("y_cover",  63);
		w_cover   = cfgReadInt("w_cover", 145);
		h_cover   = cfgReadInt("h_cover", 186);

		cfgSelectSection("grl_skin_dlg");
		if ( cfgReadBool("adime_custom_color", false) )
		{
			adime_text_rgb             = convertToRGB( cfgReadText("adime_text"            , "0, 0, 0"      ) );
			adime_disabled_text_rgb    = convertToRGB( cfgReadText("adime_disabled_text"   , "128, 128, 128") );
			adime_error_text_rgb       = convertToRGB( cfgReadText("adime_error_text"      , "255, 0, 0"    ) );
			adime_border_rgb           = convertToRGB( cfgReadText("adime_border"          , "0, 0, 0"      ) );
			adime_background_rgb       = convertToRGB( cfgReadText("adime_background"      , "192, 192, 192") );
			adime_edit_field_rgb       = convertToRGB( cfgReadText("adime_edit_field"      , "255, 255, 255") );
			adime_button_rgb           = convertToRGB( cfgReadText("adime_button"          , "192, 192, 192") );
			adime_dark_shadow_rgb      = convertToRGB( cfgReadText("adime_dark_shadow"     , "0, 0, 0"      ) );
			adime_shadow_rgb           = convertToRGB( cfgReadText("adime_shadow"          , "128, 128, 128") );
			adime_highlight_rgb        = convertToRGB( cfgReadText("adime_highlight"       , "192, 192, 192") );
			adime_light_highlight_rgb  = convertToRGB( cfgReadText("adime_light_highlight" , "255, 255, 255") );
			adime_title_text_rgb       = convertToRGB( cfgReadText("adime_title_text"      , "255, 255, 255") );
			adime_title_background_rgb = convertToRGB( cfgReadText("adime_title_background", "0, 0, 128"    ) );
			adime_title_shadow_rgb     = convertToRGB( cfgReadText("adime_title_shadow"    , "0, 0, 0"      ) );
			adime_title_highlight_rgb  = convertToRGB( cfgReadText("adime_title_highlight" , "255, 255, 255") );
		}

		cfgClose();
	} else {
	// grl_skin_general
		color_dep_recomend   = 16;
		f_img_bg             = "skins\\grldos\\fondo.png";
		num_sow_item         =  9;
		max_caracter_title   = 40;
		max_caracter_company = 34;
		sow_company          = true;
		sow_year             = true;
		sow_separate         = true;
		item_txt_separate    = "-";
		sow_num_items        = true;
		pos_x_num_items      = 520;
		pos_y_num_items      =  23;
		color_num_items      = convertToRGB("226, 226, 218");

	// grl_skin_font
		grl_open_font("", 12, true);

	// grl_skin_mouse
		f_img_cursor   = "skins\\grldos\\cursor.gif";
		x_cursor_focus =  16;
		y_cursor_focus =  16;

	// grl_skin_bottons
		color_btn_txt_select   = convertToRGB(41, 41, 41);
		color_btn_txt_unselect = convertToRGB(128, 128, 128);
		f_img_scroll_rail      = "skins\\grldos\\sc_rail.png";

		btn[btn_none].setTitulo("");
		btn[btn_none].setIconSize(1, 1);
		btn[btn_none].setIconS("", color_dep);
		btn[btn_none].setIconU("", color_dep);
		btn[btn_none].setPos(0, 0);
		btn[btn_none].setSowBtn(false);

		for (int id_btn = 1; id_btn < MAX_BOTTONS; id_btn++)
		{
			if ( id_btn == btn_up )
			{
				btn[btn_up].setTitulo("");
				btn[btn_up].setIconSize(16, 20);
				btn[btn_up].setIconS("skins\\grldos\\b_up_s.png", color_dep);
				btn[btn_up].setIconU("skins\\grldos\\b_up_u.png", color_dep);
				btn[btn_up].setPos(450, 63);
				btn[btn_up].setSowBtn(true);
			}
			else if ( id_btn == btn_down )
			{
				btn[btn_down].setTitulo("");
				btn[btn_down].setIconSize(16, 20);
				btn[btn_down].setIconS("skins\\grldos\\b_down_s.png", color_dep);
				btn[btn_down].setIconU("skins\\grldos\\b_down_u.png", color_dep);
				btn[btn_down].setPos(450, 435);
				btn[btn_down].setSowBtn(true);
			}
			else if ( id_btn == btn_move )
			{
				btn[btn_move].setTitulo("");
				btn[btn_move].setIconSize(16, 196);
				btn[btn_move].setIconS("skins\\grldos\\sc_mov_s.png", color_dep);
				btn[btn_move].setIconU("skins\\grldos\\sc_mov_u.png", color_dep);
				btn[btn_move].setPos(450, 83);
				btn[btn_move].setSowBtn(false);
			} else {
				btn[id_btn].setTitulo(lb_btn[id_btn].titulo);
				btn[id_btn].setIconSize(110, 24);
				btn[id_btn].setIconS( "skins\\grldos\\btn_s.png", color_dep);
				btn[id_btn].setIconU( "skins\\grldos\\btn_u.png", color_dep);
				btn[id_btn].setPos(510, 66+(id_btn*26));
				btn[id_btn].setSowBtn(true);
			}
		}

	// grl_skin_item
		f_img_item_list_s     = "skins\\grldos\\item_s.png";
		f_img_item_list_u     = "skins\\grldos\\item_u.png";
		f_img_item_list_del_s = "skins\\grldos\\i_del_s.png";
		f_img_item_list_del_u = "skins\\grldos\\i_del_u.png";

		color_item_txt_select   = convertToRGB( 41,  41,  41);
		color_item_txt_unselect = convertToRGB(128, 128, 128);
		color_item_txt_delete_s = convertToRGB(135,  78,  78);
		color_item_txt_delete_u = convertToRGB(222, 108, 108);

		x_item_txt_title    = 44;
		y_item_txt_title    =  8;
		x_item_txt_year     = 44;
		y_item_txt_year     = 25;
		x_item_txt_company  = 92;
		y_item_txt_company  = 25;
		x_item_txt_separate = 80;
		y_item_txt_separate = 25;

		sow_item_center = false;
		x_item          =  44;
		y_item          =  63;
		w_item          = 396;
		h_item          =  40;
		s_item          =   4;

		sow_item_icon = true;
		x_item_icon   =  6;
		y_item_icon   =  4;
		w_item_icon   = 32;
		h_item_icon   = 32;

	// grl_skin_cover
		sow_cover = false;
		x_cover   = 448;
		y_cover   =  63;
		w_cover   = 145;
		h_cover   = 186;
	}

	img_bg              = openImage( f_img_bg             , NULL, false, width_grl, height_grl, color_dep );
	img_item_list_s     = openImage( f_img_item_list_s    , NULL, false, w_item, h_item, color_dep );	
	img_item_list_u     = openImage( f_img_item_list_u    , NULL, false, w_item, h_item, color_dep );
	img_item_list_del_s = openImage( f_img_item_list_del_s, NULL, false, w_item, h_item, color_dep );
	img_item_list_del_u = openImage( f_img_item_list_del_u, NULL, false, w_item, h_item, color_dep );
	img_scroll_rail     = openImage( f_img_scroll_rail    , NULL, false, btn[btn_move].getIconW(), btn[btn_move].getIconH(), color_dep );

	mickeyx       = 0;
	mickeyy       = 0;
	h_scroll      = btn[btn_down].getPosY() - btn[btn_up].getPosY() - btn[btn_up].getIconH();
	top_scroll    = btn[btn_up].getPosY()+btn[btn_up].getIconH();
	center_scroll = btn[btn_up].getPosY()+btn[btn_up].getIconH()+(h_scroll/2)-(btn[btn_move].getIconH()/2);
	bot_scroll    = btn[btn_down].getPosY()-btn[btn_move].getIconH();
	pos_scroll    = center_scroll;

	btn[btn_move].setPosY(center_scroll);

	if ( sow_item_center )
		x_item  = (SCREEN_W/2) - (img_item_list_s->w/2);
}
		
// Añade o Edita un Juego de la Lista
void GRlDos::grl_add_edit_game(bool editando)
{
	isDialog = true;
	clear_keybuf();

// Cargamos la lista de Compañias
	vector<string> xml_list_compania = cargarDatosInfo("companys.xml", "company");
	int total_list_compania = xml_list_compania.size();
	const char *list_Compania[total_list_compania];

	for ( int i=0; i < total_list_compania; i++ )
		list_Compania[i] = xml_list_compania[i].c_str();
// Fin carga lista Compañias

// Cargamos la lista de Generos
	vector<string> xml_list_generos = cargarDatosInfo("generes.xml", "genere");
	int total_list_genero = xml_list_generos.size();
	const char *list_Genero[total_list_genero];

	for ( int i=0; i < total_list_genero; i++ )
		list_Genero[i] = xml_list_generos[i].c_str();
// Fin carga lista Generos

	string titulo_dlg      = "";
	char c_Titulo[256]     = "";
	char c_Anno[6]         = "";
	int index_Compania     =  0;
	int index_Genero       =  0;
	char c_Cover[1024]     = "";
	char c_IconStr[1024]   = "";
	char c_Path[1024]      = "";
	char c_Param[256]      = "";
	char c_PathSetup[1024] = "";
	char c_ParamSetup[256] = "";
	char c_CmdBefore[1024] = "";
	char c_CmdAfter[1024]  = "";

	if ( editando )
	{
		titulo_dlg = lng.dlg_add_edit_title_edit;
		titulo_dlg.append(" ");
		titulo_dlg.append(StrLimit(info[id_game].getTitulo(), max_caracter_title, true));

		strcpy( c_Titulo    , info[id_game].getTitulo().c_str()     );
		strcpy( c_Anno      , info[id_game].getAnno().c_str()       );
	//	strcpy( c_Cover     , StrReplace( StrUpper(info[id_game].getCover()  ), StrReplace("{C}:\\GRLDOS\\COVERS", "{C}", fileLetter(info[id_game].getCover())), "COVERS").c_str() );
		strcpy( c_Cover     , info[id_game].getCover().c_str()      );
	//	strcpy( c_IconStr   , StrReplace( StrUpper(info[id_game].getIconStr()), StrReplace("{C}:\\GRLDOS\\ICONS" , "{C}", fileLetter(info[id_game].getCover())), "ICONS" ).c_str()	);
		strcpy( c_IconStr   , info[id_game].getIconStr().c_str()	);
		strcpy( c_Path      , info[id_game].getPath().c_str()       );
		strcpy( c_Param     , info[id_game].getParam().c_str()      );
		strcpy( c_PathSetup , info[id_game].getPathSetup().c_str()  );
		strcpy( c_ParamSetup, info[id_game].getParamSetup().c_str() );
		strcpy( c_CmdBefore , info[id_game].getCmdBefore().c_str()  );
		strcpy( c_CmdAfter  , info[id_game].getCmdAfter().c_str()   );

		for ( int i = 0; i < total_list_compania; i++ )
		{
			if ( StrCompare(info[id_game].getCompania(), list_Compania[i]) )
			{
				index_Compania = i;
				break;
			}
		}

		for ( int i = 0; i < total_list_genero; i++ )
		{
			if ( StrCompare(info[id_game].getGenero(), list_Genero[i]) )
			{
				index_Genero = i;
				break;
			}
		}		
	} else
		titulo_dlg = lng.dlg_add_edit_title_add;

	string conf_dlg = lng.dlg_add_edit_lb_title;
	conf_dlg.append("%string[256]");
	conf_dlg.append(lng.dlg_add_edit_lb_year);
	conf_dlg.append("%string[6]");
	conf_dlg.append(lng.dlg_add_edit_lb_company);
	conf_dlg.append("%vlist[4]");
	conf_dlg.append(lng.dlg_add_edit_lb_genere);
	conf_dlg.append("%vlist[4]");
	conf_dlg.append(lng.dlg_add_edit_lb_sep_image);
	conf_dlg.append("%line[]");
	conf_dlg.append(lng.dlg_add_edit_lb_cover);
	conf_dlg.append("%filename[1024,bmp;lbm;pcx;tga;gif;jpg;png,");
	conf_dlg.append(lng.dlg_add_edit_open_file_image);
	conf_dlg.append("]");
	conf_dlg.append(lng.dlg_add_edit_lb_icon);
	conf_dlg.append("%filename[1024,bmp;lbm;pcx;tga;gif;jpg;png,");
	conf_dlg.append(lng.dlg_add_edit_open_file_image);
	conf_dlg.append("]");
	conf_dlg.append(lng.dlg_add_edit_lb_sep_path);
	conf_dlg.append("%line[]");
	conf_dlg.append(lng.dlg_add_edit_lb_path);
	conf_dlg.append("%filename[1024,");
	conf_dlg.append(list_exe_ext);
	conf_dlg.append(",");
	conf_dlg.append(lng.dlg_add_edit_open_file_image);
	conf_dlg.append("]");
	conf_dlg.append(lng.dlg_add_edit_lb_param_exe);
	conf_dlg.append("%string[256]");
	conf_dlg.append(lng.dlg_add_edit_lb_sep_path_setup);
	conf_dlg.append("%line[]");
	conf_dlg.append(lng.dlg_add_edit_lb_path);
	conf_dlg.append("%filename[1024,");
	conf_dlg.append(list_exe_ext);
	conf_dlg.append(",");
	conf_dlg.append(lng.dlg_add_edit_open_file_image);
	conf_dlg.append("]");
	conf_dlg.append(lng.dlg_add_edit_lb_param_exe);
	conf_dlg.append("%string[256]");
	conf_dlg.append(lng.dlg_add_edit_lb_command_lines);
	conf_dlg.append("%line[]");
	conf_dlg.append(lng.dlg_add_edit_lb_before_after);
	conf_dlg.append("%nothing[]");
	conf_dlg.append(lng.dlg_add_edit_lb_before);
	conf_dlg.append("%string[1024]");
	conf_dlg.append(lng.dlg_add_edit_lb_after);
	conf_dlg.append("%string[1024]");
	conf_dlg.append(lng.dlg_add_edit_lb_newline);

	conf_dlg.append("\n%nothing[]%line[]%buttonrow[ ");
	conf_dlg.append( lng.dlg_main_btn_ok );
	conf_dlg.append(" ;CTRL+O;ENTER, ");
	conf_dlg.append( lng.dlg_main_btn_cancel );
	conf_dlg.append(" ;ESC]");

	int dlg_ = adime_lowlevel_dialogf( titulo_dlg.c_str(), 
			ADIME_ALIGN_CENTRE, ADIME_ALIGN_CENTRE, 300, conf_dlg.c_str(),
				c_Titulo,
				c_Anno,
			// inicio lista compania
				&index_Compania,
				list_Compania,
				total_list_compania,
			// fin lista compania
			// inicio lista genero
				&index_Genero,
				list_Genero,
				total_list_genero,
			// fin lista genero
				c_Cover,
				c_IconStr,
				c_Path,
				c_Param,
				c_PathSetup,
				c_ParamSetup,
			// Lineas de comandos
				c_CmdBefore,
                c_CmdAfter,
			callback);

	if ( dlg_ == 1)
	{
		int new_game = 0;
		if ( editando )
			new_game = id_game;
		else {
			if ( num_items < 0 )
				num_items = 0;
			new_game = num_items;
		}

		info[new_game].setEliminado();
		info[new_game].setTitulo    ( c_Titulo     );
		info[new_game].setAnno      ( c_Anno       );
		info[new_game].setCompania  ( list_Compania[index_Compania] );
		info[new_game].setGenero    ( list_Genero[index_Genero]     );
	//	info[new_game].setCover     ( StrReplace( StrUpper(c_Cover  ), StrReplace("{C}:\\GRLDOS\\COVERS", "{C}", fileLetter(c_Cover)) ,"COVERS") );
		info[new_game].setCover     ( c_Cover      );
	//	info[new_game].setIcon      ( c_IconStr, w_item_icon, h_item_icon, color_dep);
	//	info[new_game].setIconStr   ( StrReplace( StrUpper(c_IconStr), StrReplace("{C}:\\GRLDOS\\ICONS", "{C}", fileLetter(c_IconStr)), "ICONS") );
		info[new_game].setIconStr   ( c_IconStr    );
		info[new_game].setPath      ( c_Path       );
		info[new_game].setParam     ( c_Param      );
		info[new_game].setPathSetup ( c_PathSetup  );
		info[new_game].setParamSetup( c_ParamSetup );
		info[new_game].setCmdBefore ( c_CmdBefore  );
		info[new_game].setCmdAfter  ( c_CmdAfter   );

		if ( editando )
			grl_display_game_item(new_game, x_item, y_item+(ptr*(h_item+s_item)), true, true);
		else {
			if ( num_items < num_sow_item && top < 1 )
			
				grl_display_game_item(new_game, x_item, y_item+(new_game*(h_item+s_item)), num_items==0?true:false, true);
			num_items++;
			total_games = num_items;
			grl_display_count_item(true);
		}

		grl_save_list_games();
	}

	grl_read_key_or_click();	// Espera a que se pulse otra tecla o un click de ratón
	isDialog = false;
}

// Elimina un Juego de la Lista
void GRlDos::grl_delete_game()
{
	if ( id_game > -1 && num_items > -1 && !info[id_game].getEliminado() )
	{
		isDialog = true;
		clear_keybuf();

		string conf_dlg = lng.dlg_delete_mensage;
		conf_dlg.append("\n\n");
		conf_dlg.append( info[id_game].getTitulo() );

		conf_dlg.append("\n%nothing[]%line[]%buttonrow[ ");
		conf_dlg.append( lng.dlg_main_btn_ok );
		conf_dlg.append(" ;CTRL+O;ENTER, ");
		conf_dlg.append( lng.dlg_main_btn_cancel );
		conf_dlg.append(" ;ESC]");

		int dlg_ = adime_lowlevel_dialogf( lng.dlg_delete_title.c_str(), 
				ADIME_ALIGN_CENTRE, ADIME_ALIGN_CENTRE, 300, conf_dlg.c_str(), callback);

		if ( dlg_ == 1)
		{
			info[id_game].setEliminado(true);

			grl_display_background();
			grl_display_list_games(top);
			grl_display_screen();

			grl_save_list_games();
		}

		grl_read_key_or_click();	// Espera a que se pulse otra tecla o un click de ratón
		isDialog = false;
	}
}

// Descomprime un archivo
void GRlDos::grl_decompress_file()
{
	isDialog = true;
	clear_keybuf();

	int index_zipes = 0;
	string lista_zipes = "";

	XmlNodes xml_zipes = loadXmlNodes("zipes.xml");
	int total_zipes = xml_zipes.size();
	t_zipes zipes[total_zipes];

	int i = 0;
	XmlNodes::iterator itr;
	for ( itr = xml_zipes.begin(); itr != xml_zipes.end(); ++itr )
	{
		if ( i < total_zipes && StrCompare(itr->tagName, "zipes") )
		{
			zipes[i].z_titulo = itr->getAttrStr("title", "-");
			zipes[i].z_exe    = itr->getAttrStr("exe"  , "");
			zipes[i].z_param  = itr->getAttrStr("param", "");
			if ( i > 0 )
				lista_zipes.append( ";" );
			lista_zipes.append( zipes[i].z_titulo );
			i++;
		}
	}

	char c_Source[1024]      = "";
	char c_Destination[1024] = "";

	string conf_dlg = lng.dlg_extract_lb_exe;
	conf_dlg.append("%list[4,");
	conf_dlg.append(lista_zipes);
	conf_dlg.append("]");
	conf_dlg.append(lng.dlg_extract_lb_source);
	conf_dlg.append("%filename[1024,");
	conf_dlg.append( list_zip_ext );
	conf_dlg.append(",");
	conf_dlg.append(lng.dlg_extract_open_file_source);
	conf_dlg.append("]");
	conf_dlg.append(lng.dlg_extract_lb_destination);
	conf_dlg.append("%filename[1024,*,");
	conf_dlg.append(lng.dlg_extract_open_file_destination);
	conf_dlg.append("]");

	conf_dlg.append("\n%line[]%buttonrow[ ");
	conf_dlg.append( lng.dlg_main_btn_ok );
	conf_dlg.append(" ;CTRL+O;ENTER, ");
	conf_dlg.append( lng.dlg_main_btn_cancel );
	conf_dlg.append(" ;ESC]");

	int dlg_ = adime_lowlevel_dialogf( lng.dlg_extract_title.c_str(), 
			ADIME_ALIGN_CENTRE, ADIME_ALIGN_CENTRE, 300, conf_dlg.c_str(),
				&index_zipes,
				c_Source,
				c_Destination,
			callback);

	if ( dlg_ == 1)
	{
		ofstream myfile;
		myfile.open("ejecutar.bat");
		if ( myfile.is_open() )
		{
			string str_zip = StrReplace(zipes[index_zipes].z_param, "{source}", c_Source);
			str_zip = StrReplace(str_zip, "{destination}", filePath(c_Destination).c_str() );

			myfile << zipes[index_zipes].z_exe << " " << str_zip << endl;

			myfile.flush();
			myfile.close();
		}
		isPlay = true;
		isExit = true;
	} else
		grl_read_key_or_click();	// Espera a que se pulse otra tecla o un click de ratón

	isDialog = false;
}

// Opciones del Programa
void GRlDos::grl_options()
{
	isDialog = true;
	clear_keybuf();

	string select_language, select_skin;
	select_language = get_config_string("grl_general", "language"    , "lng_es.ini");
	select_skin     = get_config_string("grl_general", "skin"        , "grldos.ini");
	list_zip_ext    = get_config_string("grl_general", "list_zip_ext", "zip;7z;rar");
	char c_ListZip[256] = "";

	strcpy( c_ListZip, list_zip_ext.c_str() );

// Carga la lista de Idiomas disponibles
	vector<string> v_lista_language = listarDir("lng", "ini");
	int total_list_language = v_lista_language.size();
	const char *list_Language[total_list_language];
	int index_Language = 0;

	for ( int i = 0; i < total_list_language; i++ )
	{
		list_Language[i] = v_lista_language[i].c_str();
		if ( StrCompare(select_language, list_Language[i]) )
			index_Language = i;
	}
// FIN lista de Idiomas

// Carga la lista de Skins disponibles
	vector<string> v_lista_skin = listarDir("skins", "ini");
	int total_list_skin = v_lista_skin.size();
	const char *list_Skin[total_list_skin];
	int index_Skin = 0;

	for ( int i = 0; i < total_list_skin; i++ )
	{
		list_Skin[i] = v_lista_skin[i].c_str();
		if ( StrCompare(select_skin, list_Skin[i]) )
			index_Skin = i;
	}
// FIN lista de Skins

	string conf_dlg = lng.dlg_option_lb_language;
	conf_dlg.append("%vlist[4]");
	conf_dlg.append(lng.dlg_option_lb_skins);
	conf_dlg.append("%vlist[4]");
	conf_dlg.append("Zip Ext:%line[]%string[256]");
	conf_dlg.append( lng.dlg_option_lb_info_ext_zip );

	conf_dlg.append("\n%nothing[]%line[]%buttonrow[ ");
	conf_dlg.append( lng.dlg_main_btn_ok );
	conf_dlg.append(" ;CTRL+O;ENTER, ");
	conf_dlg.append( lng.dlg_main_btn_cancel );
	conf_dlg.append(" ;ESC]");

	int dlg_ = adime_lowlevel_dialogf( lng.dlg_option_title.c_str(), 
			ADIME_ALIGN_CENTRE, ADIME_ALIGN_CENTRE, 300, conf_dlg.c_str(),
			// inicio lista language
				&index_Language,
				list_Language,
				total_list_language,
			// fin lista language
			// inicio lista skin
				&index_Skin,
				list_Skin,
				total_list_skin,
			// fin lista skin
				c_ListZip,
			callback);

	if ( dlg_ == 1)
	{
		set_config_string("grl_general", "language"     , list_Language[index_Language] );
		set_config_string("grl_general", "skin"         , list_Skin[index_Skin]         );
		set_config_string("grl_general", "list_zip_ext" , c_ListZip                     );
		list_zip_ext = c_ListZip;
	}

	grl_read_key_or_click();	// Espera a que se pulse otra tecla o un click de ratón
	isDialog = false;
}

// Muestra Acerca de... del Programa
void GRlDos::grl_about_help(bool isHelp)
{
	isDialog = true;
	clear_keybuf();

	string titulo_dlg = "";
	string conf_dlg   = "";

	if ( isHelp )
	{
		titulo_dlg = lng.dlg_help_title;
		conf_dlg.append( StrReplace(lng.dlg_help_mensage, "<br>", "\n") );
	} else {
		titulo_dlg = lng.dlg_about_title;

		string msg_about = defect_msg_about;
		msg_about = StrReplace(msg_about, "{lb_designed_and_programmed}", lng.dlg_about_lb_designed_programmed);
		msg_about = StrReplace(msg_about, "{lb_thanks}"                 , lng.dlg_about_lb_thanks);
		msg_about = StrReplace(msg_about, "{lb_developers}"             , lng.dlg_about_lb_developers);

		conf_dlg.append( msg_about );
	}

	conf_dlg.append("\n%nothing[]%line[]%buttonrow[ ");
	conf_dlg.append( lng.dlg_main_btn_ok );
	conf_dlg.append(" ;CTRL+O;ENTER");
	conf_dlg.append("]");

	adime_lowlevel_dialogf( titulo_dlg.c_str(), 
			ADIME_ALIGN_CENTRE, ADIME_ALIGN_CENTRE, 408, conf_dlg.c_str(), callback);

	grl_read_key_or_click();	// Espera a que se pulse otra tecla o un click de ratón
	isDialog = false;
}
