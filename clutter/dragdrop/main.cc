/* Copyright (C) 2016 Andreas Österreicher
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <gtk/gtk.h>
#include <iostream>
#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>

static void activate (GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *m_Box;
    GtkWidget *clutter0;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Clutter Example With GTK+ Drag&Drop");
    gtk_window_set_default_size(GTK_WINDOW (window), 800, 600);

    m_Box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    gtk_container_add(GTK_CONTAINER (window), m_Box);

    clutter0 = gtk_clutter_embed_new();
    gtk_container_add(GTK_CONTAINER(m_Box), clutter0);

    ClutterActor *stage = NULL;
    stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (clutter0));

    ClutterColor stage_color = { 255, 255, 255, 255 };
    clutter_actor_set_size (stage, 512, 512);
    clutter_actor_set_background_color (stage, &stage_color);

    ClutterActor *box = clutter_actor_new ();
    ClutterColor box_color = { 56, 147, 254, 255 };
    clutter_actor_set_background_color (box, &box_color);
    clutter_actor_set_position (box, 100, 100);
    clutter_actor_set_size (box, 100, 100);
    clutter_actor_set_reactive(box, true);
    clutter_actor_add_child(stage, box);

    clutter_actor_add_action(box, clutter_drag_action_new());

    gtk_widget_show_all(window);
}

int main(int argc, char *argv[])
{
	if(CLUTTER_INIT_SUCCESS !=clutter_init (&argc, &argv))
		std::cout << "clutter init failed" << std::endl;
	GtkApplication *app;
	int status;

	app = gtk_application_new ("oesi.org.example.clutter.gtk.dragdrop", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
