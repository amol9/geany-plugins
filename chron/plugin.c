#include <geanyplugin.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


GeanyPlugin *geany_plugin;
GeanyData *geany_data;


PLUGIN_VERSION_CHECK(GEANY_API_VERSION)
PLUGIN_SET_INFO(_("Chron"), _("Inserts current date/time."), _("1.0.0"), _("Amol Umrale"))


enum
{
	KB_INSERT_DATE,
	KB_INSERT_TIME,
	KB_INSERT_DATE_TIME,
	KB_COUNT
};

gchar* current_dt(const gchar* fmt) {
	GDateTime* dt = g_date_time_new_now_local();
	return g_date_time_format(dt, fmt);
}

void insert_at_cursor(const gchar* text) {
	GeanyDocument *doc = document_get_current();
	gint pos = sci_get_current_position(doc->editor->sci);

	//gchar* final = g_strconcat(text);
	sci_insert_text(doc->editor->sci, pos, text);
	sci_set_current_position(doc->editor->sci, pos + strlen(text), TRUE);
	//g_free(final);
}

static void insert_date(G_GNUC_UNUSED GtkMenuItem * menuitem) {
	gchar* date = current_dt("%d %b %Y\n");
	insert_at_cursor(date);
	g_free(date);
}

static void insert_time(G_GNUC_UNUSED GtkMenuItem * menuitem) {
	gchar* time = current_dt("%H:%M\n");
	insert_at_cursor(time);
	g_free(time);
}

static void insert_date_time(G_GNUC_UNUSED GtkMenuItem * menuitem) {
	gchar* dt = current_dt("%d %b %Y, %H:%M\n");
	insert_at_cursor(dt);
	g_free(dt);
}

GtkWidget* add_main_menu_item() {
	GtkWidget* main_menu = gtk_menu_item_new_with_mnemonic(_("_Date/Time"));
	gtk_widget_show_all(main_menu);
	gtk_container_add(GTK_CONTAINER(geany->main_widgets->tools_menu), main_menu);

	ui_add_document_sensitive(main_menu);

	GtkWidget* main_menu_submenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(main_menu), main_menu_submenu);

	return main_menu_submenu;
}

GtkWidget* add_sub_menu_item(GtkWidget* main_menu_submenu, const gchar* label, GCallback cb) {
	GtkWidget* submenu = gtk_menu_item_new_with_mnemonic(label);
	g_signal_connect((gpointer) submenu, "activate", G_CALLBACK(cb), NULL);
	gtk_container_add(GTK_CONTAINER(main_menu_submenu), submenu);

	return submenu;
}

GtkWidget* main_menu = NULL;

GtkWidget** setup_menu() {
	main_menu = add_main_menu_item();

	GtkWidget** sub_menus = (GtkWidget**) malloc(sizeof(GtkWidget*) * 3);

	sub_menus[0] = add_sub_menu_item(main_menu, _("Insert _Date"), G_CALLBACK(insert_date));
	sub_menus[1] = add_sub_menu_item(main_menu, _("Insert _Time"), G_CALLBACK(insert_time));
	sub_menus[2] = add_sub_menu_item(main_menu, _("Insert D_ateTime"), G_CALLBACK(insert_date_time));

	for (int i = 0; i < 3; i++)
		gtk_widget_show(sub_menus[i]);

	return sub_menus;
}

void setup_keybindings(GeanyPlugin *plugin, GtkWidget** sub_menus) {
	GeanyKeyGroup* key_group = plugin_set_key_group(plugin, "html_chars", KB_COUNT, NULL);
	keybindings_set_item(key_group, KB_INSERT_DATE, insert_date, 0, 0, "insert_date", _("Insert date"), sub_menus[0]);
	keybindings_set_item(key_group, KB_INSERT_TIME, insert_time, 0, 0, "insert_time", _("Insert time"), sub_menus[1]);
	keybindings_set_item(key_group, KB_INSERT_DATE_TIME, insert_date_time, 0, 0, "insert_date_time",
			_("Insert date and time"), sub_menus[2]);

}

void plugin_init(GeanyData *data)
{
    	GtkWidget** sub_menus = setup_menu();
    	setup_keybindings(geany_plugin, sub_menus);

    	return TRUE;
}

void plugin_cleanup(void)
{
	gtk_widget_destroy(main_menu);
}

