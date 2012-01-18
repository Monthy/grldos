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

struct idioma {
// Dialogo comun
	string dlg_main_btn_ok;
	string dlg_main_btn_cancel;
// Dialogo Eliminar
	string dlg_delete_title;
	string dlg_delete_mensage;
// Dialogo Añadir/Editar
	string dlg_add_edit_title_add;
	string dlg_add_edit_title_edit;
	string dlg_add_edit_lb_title;
	string dlg_add_edit_lb_year;
	string dlg_add_edit_lb_company;
	string dlg_add_edit_lb_genere;
	string dlg_add_edit_lb_sep_image;
	string dlg_add_edit_open_file_image;
	string dlg_add_edit_lb_cover;
	string dlg_add_edit_lb_icon;
	string dlg_add_edit_lb_sep_path;
	string dlg_add_edit_lb_sep_path_setup;
	string dlg_add_edit_open_file_exe;
	string dlg_add_edit_lb_path;
	string dlg_add_edit_lb_param_exe;
	string dlg_add_edit_lb_command_lines;
	string dlg_add_edit_lb_before_after;
	string dlg_add_edit_lb_before;
	string dlg_add_edit_lb_after;
	string dlg_add_edit_lb_newline;
// Dialogo Opciones
	string dlg_option_title;
	string dlg_option_lb_language;
	string dlg_option_lb_skins;
	string dlg_option_lb_info_ext_zip;
// Dialogo Descomprimir
	string dlg_extract_title;
	string dlg_extract_lb_exe;
	string dlg_extract_lb_source;
	string dlg_extract_open_file_source;
	string dlg_extract_lb_destination;
	string dlg_extract_open_file_destination;
// Dialogo Acerca de..
	string dlg_about_title;
	string dlg_about_lb_designed_programmed;
	string dlg_about_lb_thanks;
	string dlg_about_lb_developers;
// Dialogo Ayuda
	string dlg_help_title;
	string dlg_help_mensage;
// Otros
	string list_game_load;
	string list_game_no_title;
};

struct t_zipes {
	string z_titulo;
	string z_exe;
	string z_param;
};

enum num_Btn {
	btn_none =  0,
	btn_play =  1,
	btn_conf =  2,
	btn_new  =  3,
	btn_edit =  4,
	btn_del  =  5,
	btn_zip  =  6,
	btn_opt  =  7,
	btn_help =  8,
	btn_acde =  9,
	btn_exit = 10,
	btn_up   = 11,
	btn_down = 12,
	btn_move = 13
};

// Struct y Class de Botones
struct lb_btn_idioma {
	string titulo;
};
class Bottons {
private:
	string m_titulo;
	BITMAP *m_icon_s;
	BITMAP *m_icon_u;
	int m_pos_x;
	int m_pos_y;
	int m_icon_w;
	int m_icon_h;
	bool m_sow_btn;

public:
	void setTitulo(string titulo) { m_titulo = titulo;}
	void setIconSize(int ico_w = 110, int ico_h = 24) { m_icon_w = ico_w; m_icon_h = ico_h;}
	void setIconS(string icon = "", int ico_col_dep = 32) { m_icon_s = openImage(icon, NULL, true, m_icon_w, m_icon_h, ico_col_dep);}
	void setIconU(string icon = "", int ico_col_dep = 32) { m_icon_u = openImage(icon, NULL, true, m_icon_w, m_icon_h, ico_col_dep);}
	void setPos(int pos_x, int pos_y) { m_pos_x = pos_x; m_pos_y = pos_y;}
	void setPosX(int pos_x) { m_pos_x = pos_x;}
	void setPosY(int pos_y) { m_pos_y = pos_y;}
	void setSowBtn(bool sow_btn = true) { m_sow_btn = sow_btn;}

	string getTitulo() { return m_titulo; }
	int getIconW() { return m_icon_w; }
	int getIconH() { return m_icon_h; }		
	BITMAP *getIconS() { return m_icon_s; }
	BITMAP *getIconU() { return m_icon_u; }
	int getPosX() { return m_pos_x; }
	int getPosY() { return m_pos_y; }
	bool getSowBtn() { return m_sow_btn;}
};

// Class de Juegos
class Juego {
private:
	bool m_eliminado;
	string m_titulo;
	string m_anno;
	string m_compania;
	string m_genero;
	string m_cover;
//	BITMAP *m_icon;
	string m_icon_str;
	string m_path;
	string m_param;
	string m_path_setup;
	string m_param_setup;
	string m_cmd_before;
	string m_cmd_after;

public:
	void setEliminado(bool eliminado = false) { m_eliminado = eliminado; }
	void setTitulo(string titulo) { m_titulo = titulo;}
	void setAnno(string anno) { m_anno = anno; }
	void setCompania(string compania = "") { m_compania = compania; }
	void setGenero(string genero = "") { m_genero = genero; }
	void setCover(string cover = "") { m_cover = cover; }
//	void setIcon(string icon = "", int ico_w = 32, int ico_h = 32, int ico_col_dep = 32) { m_icon = openImage(icon, NULL, true, ico_w, ico_h, ico_col_dep);}
	void setIconStr(string icon_str = "") { m_icon_str = icon_str; }
	void setPath(string path = "") { m_path = path; }
	void setParam(string param = "") { m_param = param; }
	void setPathSetup(string path_setup = "") { m_path_setup = path_setup; }
	void setParamSetup(string param_setup = "") { m_param_setup = param_setup; }
	void setCmdBefore(string cmd_before = "") { m_cmd_before = cmd_before; }
	void setCmdAfter(string cmd_after = "") { m_cmd_after = cmd_after; }

	bool getEliminado() { return m_eliminado; }
	string getTitulo() { return m_titulo; }
	string getAnno() { return m_anno; }
	string getCompania() { return m_compania; }
	string getGenero() { return m_genero; }
	string getCover() { return m_cover; }
	string getIconStr() { return m_icon_str; }
//	BITMAP *getIcon() { return m_icon; }
	string getPath() { return m_path; }
	string getParam() { return m_param; }
	string getPathSetup() { return m_path_setup; }
	string getParamSetup() { return m_param_setup; }
	string getCmdBefore() { return m_cmd_before; }
	string getCmdAfter() { return m_cmd_after; }
};

class GRlDos {
private:
// Variables -------------------------------------------------------
	FONT *grl_font;
	ALFONT_FONT *grl_alfont;
	PALETTE pal_font;
	bool isFontAa, isALFont;

	// Pantalla
	int color_dep, color_dep_recomend, width_grl, height_grl;

	Juego info[MAX_JUEGOS];
	Bottons btn[MAX_BOTTONS];
	lb_btn_idioma lb_btn[MAX_BOTTONS];
	idioma lng;

	// Fondo de pantalla e imágenes del Item
	BITMAP *img_buf, *img_bg, *img_icon;
	BITMAP *img_item_list_s, *img_item_list_u, *img_item_list_del_s, *img_item_list_del_u;

	string defect_msg_about, defect_msg_help;
	string f_exe, f_ext, f_path, f_param;
	string f_exe_setup, f_ext_setup, f_path_setup, f_param_setup;
	string f_img_bg, f_img_item_list_s, f_img_item_list_u, f_img_item_list_del_s, f_img_item_list_del_u;
	string item_txt_separate, list_exe_ext, list_zip_ext, cmd_new_linea;

	bool isPlay, isExit, isDialog, isExec, isSetup;
	bool sow_company, sow_year, sow_separate, sow_item_icon, sow_item_center, sow_cover, sow_num_items;
	bool use_trans_sprite;

	int total_games, id_game, ptr, top;
	int num_items, num_sow_item, pasos;
	int max_caracter_title, max_caracter_company;

	// Alto, Ancho, pos X, pos Y
	int x_item, y_item, w_item, h_item, s_item;
	int x_item_icon, y_item_icon, w_item_icon, h_item_icon;
	int x_item_txt_title, y_item_txt_title;
	int x_item_txt_year, y_item_txt_year;
	int x_item_txt_company, y_item_txt_company;
	int x_item_txt_separate, y_item_txt_separate;
	int x_cover, y_cover, w_cover, h_cover;
	int pos_x_num_items, pos_y_num_items;

	RGB color_num_items, color_item_txt_select, color_item_txt_unselect, 
		color_item_txt_delete_s, color_item_txt_delete_u, 
		color_btn_txt_select, color_btn_txt_unselect;

// Funciones Ratón -------------------------------------------------
	string f_img_cursor, f_img_scroll_rail;
	int new_ptr, x_cursor_focus, y_cursor_focus, id_btn_select;
	int top_scroll, center_scroll, bot_scroll, pos_scroll, h_scroll, mickeyx, mickeyy;
	bool isBtnSelect, isBtnMove, isBtnMoveUp, isBtnMoveDown;

	BITMAP *img_scroll_rail;

	void grl_read_key_or_click();			// Espera a que se pulse otra tecla o un click de ratón
	void grl_bottons_state(bool select, int id = 0, bool clear_btns = false);	// Muesta el botón activado/desactivado
	void grl_mouse_info();					// Informacion del raton: posicion cursor/clicks/zonas de clicks

// Funciones -------------------------------------------------------
	void grl_load_config();					// Carga la configuración
	void grl_save_config();					// Guarda la configuración

	void grl_set_correct_color_depth();		// Indicamos la profundidad de color
	void grl_init();						// Inicia
	void grl_display_background();			// Dibuja la imagen de fondo
	void grl_display_screen();				// Dibuja la imagen final en pantalla

	void grl_load_list_games();				// Carga la lista de juegos
	void grl_save_list_games();				// Guarda la lista de juegos

	void grl_game_exe_info(int id);						// Obtiene la información del ejecutable juego
	void grl_display_count_item(bool isScreen = false);	// Muestra el numero de items y el seleccionado
	void grl_display_game_item(int id, int x, int y, bool select, bool isScreen = false); // Dibuja un item del juego
	void grl_display_list_games(int top_);		// Dibuja la lista de juegos
	void grl_item_key_up(bool isNext = false);	// Sube un item seleccionado
	void grl_item_key_down(bool isNext = false);// Baja un item seleccionado
	void grl_play_game();						// Ejecuta el juego
	void grl_setup_game();						// Ejecuta el setup del juego

	void grl_load_language(string filename = "lng_es.ini");	// Carga el Idioma del Programa
	void grl_open_font(string f_str, int f_size = 12, bool f_aa = true);	// Carga un tipo de Letra o fuente en PCX o TTF
	void grl_load_skin(string filename = "defecto.ini");	// Carga la apariencia del Programa

	void grl_add_edit_game(bool editando);	// Añade o Edita un Juego de la Lista
	void grl_delete_game();					// Elimina un Juego de la Lista

	void grl_decompress_file();				// Descomprime un archivo
	void grl_options();						// Opciones del Programa
	void grl_about_help(bool isHelp);		// Muestra la ayuda o acerca de... del programa

public:
	GRlDos();	// Constructor
	~GRlDos();	// Destructor

	bool grl_isPlay(){return isPlay;} // Retorna si se va ha jugar
};
