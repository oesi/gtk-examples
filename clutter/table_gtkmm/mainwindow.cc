/* Copyright (C) 2016 Andreas Österreicher
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include "mainwindow.h"
#include <iostream>
#include <string>
#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>

void helper_clutter_actor_get_center_position(ClutterActor *actor, float *x, float *y)
{
    float sizeX, sizeY;
    float posX, posY;

    clutter_actor_get_size(actor, &sizeX, &sizeY);
    clutter_actor_get_position(actor, &posX, &posY);

    *x = posX + (sizeX/2);
    *y = posY + (sizeY/2);
}


void get_line_position(ClutterActor *actor1, ClutterActor *actor2, float *x, float *y, float *len, float *angle)
{
	float actor1x, actor1y, actor2x, actor2y;
	float actorRightX, actorRightY, actorLeftX, actorLeftY;
	float lenX, lenY, anglecorrection;
    bool p1;

	helper_clutter_actor_get_center_position(actor1, &actor1x, &actor1y);
	helper_clutter_actor_get_center_position(actor2, &actor2x, &actor2y);

	if(actor1x>=actor2x)
	{
		// Actor1 is right
		actorLeftX = actor2x;
		actorLeftY = actor2y;
		actorRightX = actor1x;
		actorRightY = actor1y;

		if(actor1y<actor2y)
		{
			// Actor1 top right
            p1=false;
            anglecorrection=90;
		}
		else
		{
			// Actor1 is bottom right
            p1=true;
            anglecorrection=180;
		}
	}
	else
	{
		// Actor1 is left
		actorLeftX = actor1x;
		actorLeftY = actor1y;
		actorRightX = actor2x;
		actorRightY = actor2y;

		if(actor1y<actor2y)
		{
			// Actor1 is top left
            p1=true;
            anglecorrection=0;
		}
		else
		{
			// Actor1 is bottom left
            p1=false;
            anglecorrection=-90;
		}
	}

	lenX = (actorRightX-actorLeftX);
	if(actorRightY<actorLeftY)
		lenY = actorLeftY - actorRightY;
	else
		lenY = actorRightY - actorLeftY;

	*x = actor1x;
	*y = actor1y;
	*len = sqrt((lenX*lenX) + (lenY*lenY));
    if(p1)
    	*angle = (atan(lenY / lenX)/(2*M_PI)*360)+anglecorrection;
    else
    	*angle = (atan(lenX / lenY)/(2*M_PI)*360)+anglecorrection;
}

void redrawLine(ClutterActor *line, ClutterActor *actor1, ClutterActor *actor2)
{
	float x,y,len,angle;

	get_line_position(actor1, actor2, &x, &y, &len, &angle);

	clutter_actor_set_position(line, x, y);
	clutter_actor_set_size(line, len, 1);
	clutter_actor_set_rotation_angle (line, CLUTTER_Z_AXIS, angle);
}

void on_actor_dragmotion (ClutterDragAction *action,
               ClutterActor      *actor,
               gfloat             delta_x,
               gfloat             delta_y,
               gpointer           user_data)
{
	ClutterActor *parent;
	ClutterActor *line, *t1, *t2;
	parent = clutter_actor_get_parent(actor);
	line = clutter_container_find_child_by_name(CLUTTER_CONTAINER(parent), "line1");
	t1 = clutter_container_find_child_by_name(CLUTTER_CONTAINER(parent), "tbl_person");
	t2 = clutter_container_find_child_by_name(CLUTTER_CONTAINER(parent), "tbl_user");
	redrawLine(line,t1,t2);
}

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& app)
: 	m_VBox(Gtk::ORIENTATION_VERTICAL, 6)
{
	set_title("Clutter Example with Gtkmm");
	set_size_request(800, 600);

	clutter0 = gtk_clutter_embed_new();
	Gtk::Widget *mmclutter = Glib::wrap(clutter0);
	m_Box.pack_start(*mmclutter);

	add(m_Box);

	ClutterActor *stage = NULL;
	stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (clutter0));

	drawStage(stage);
	show_all_children();
}

void MainWindow::addTableRow(ClutterActor *parent, std::vector<ColumnData> columns)
{
	for(unsigned int i = 0; i < columns.size(); i++)
	{
		ClutterActor *rowText = clutter_text_new();
		std::string bezeichnung = columns[i].name + " " + columns[i].datatype;
		if(columns[i].primarykey)
			bezeichnung += " PK";
		if(columns[i].foreignkey)
			bezeichnung += " FK";
		if(columns[i].notnull)
			bezeichnung += " NN";

		clutter_text_set_text(CLUTTER_TEXT(rowText), bezeichnung.c_str());
		clutter_actor_set_x_expand(rowText, true);
		clutter_actor_add_child(parent, rowText);
	}
}

void MainWindow::drawTable(ClutterActor *stage, TableData tableobject)
{
	ClutterColor stage_color = { 255, 255, 255, 255 };
	clutter_actor_set_size (stage, 512, 512);
	clutter_actor_set_background_color (stage, &stage_color);

	ClutterLayoutManager *box_layout = clutter_box_layout_new ();
	clutter_box_layout_set_orientation (CLUTTER_BOX_LAYOUT (box_layout), CLUTTER_ORIENTATION_VERTICAL);
	clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (box_layout), 0);
	ClutterActor *box = clutter_actor_new ();
	clutter_actor_set_layout_manager (box, box_layout);
	ClutterColor box_color = { 56, 147, 254, 255 };
	clutter_actor_set_background_color (box, &box_color);
	clutter_actor_set_position (box, tableobject.xposition, tableobject.yposition);
	clutter_actor_set_reactive(box, true);
	clutter_actor_add_child(stage, box);


	ClutterActor *headerRect = clutter_actor_new ();
	clutter_actor_set_background_color(headerRect, &box_color);

	clutter_actor_set_x_expand(headerRect, true);
	clutter_actor_add_child(box, headerRect);
	clutter_actor_set_name(box,tableobject.name.c_str());

	ClutterActor *headerText = clutter_text_new();
	clutter_text_set_text(CLUTTER_TEXT(headerText), tableobject.name.c_str());
	clutter_actor_add_child(headerRect, headerText);

	ClutterLayoutManager *body_layout = clutter_box_layout_new ();
	clutter_box_layout_set_orientation (CLUTTER_BOX_LAYOUT (body_layout), CLUTTER_ORIENTATION_VERTICAL);
	clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (body_layout), 5);
	ClutterColor bodyColor = { 192, 214, 255, 255 };
	ClutterActor *bodyRect = clutter_actor_new ();
	clutter_actor_set_layout_manager (bodyRect, box_layout);
	clutter_actor_set_background_color(bodyRect, &bodyColor);

	ClutterMargin bodymargin = {2.f, 2.f, 2.f, 2.f};
	clutter_actor_set_margin(bodyRect, &bodymargin);

	clutter_actor_add_child(box, bodyRect);

	addTableRow(bodyRect,tableobject.columns);

	ClutterAction *dragaction;
	dragaction = clutter_drag_action_new();
	clutter_actor_add_action(box, dragaction);
	g_signal_connect (dragaction, "drag-motion", G_CALLBACK(on_actor_dragmotion), NULL);

}

void MainWindow::drawLine(ClutterActor *parent, ClutterActor *actor1, ClutterActor *actor2)
{
	float x,y,len,angle;

	get_line_position(actor1, actor2, &x, &y, &len, &angle);

	ClutterActor *myline = clutter_actor_new ();
	ClutterColor myLineColor = { 255, 0, 0, 255 };
	clutter_actor_set_background_color(myline, &myLineColor);
	clutter_actor_add_child(parent, myline);
	clutter_actor_set_position(myline, x, y);
	clutter_actor_set_size(myline, len, 1);
	clutter_actor_set_rotation_angle (myline, CLUTTER_Z_AXIS, angle);

	clutter_actor_set_name(myline, "line1");
}

void MainWindow::drawStage(ClutterActor *stage)
{
	ClutterActor *t1, *t2;
	// Draw Tables
	std::vector<ColumnData> cols;
	cols.push_back(ColumnData("person_id", "integer", true, false, true));
	cols.push_back(ColumnData("name", "varchar(255)", false, false, false));
	cols.push_back(ColumnData("birthdate", "date", false, false, true));
	cols.push_back(ColumnData("title", "varchar(50)", false, false, false));
	cols.push_back(ColumnData("nation", "varchar(50)", false, true, false));

	drawTable(stage, TableData("tbl_person",cols, 100, 100));

	t1 = clutter_actor_get_last_child (stage);

	cols.clear();

	cols.push_back(ColumnData("uid", "varchar(32)", true, false, true));
	cols.push_back(ColumnData("active", "boolean", false, false, true));
	cols.push_back(ColumnData("person_id", "integer", false, true, true));

	drawTable(stage, TableData("tbl_user",cols, 400, 200));

	t2 = clutter_actor_get_last_child (stage);

	drawLine(stage, t1, t2);
}

MainWindow::~MainWindow()
{
}
