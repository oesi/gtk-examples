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
#ifndef DBA_MAINWINDOW_H
#define DBA_MAINWINDOW_H

#include <gtkmm.h>
#include <iostream>
#include <clutter/clutter.h>

struct ColumnData
{
	std::string name;
	std::string datatype;
	bool primarykey;
	bool foreignkey;
	bool notnull;
	ColumnData(std::string n, std::string dt, bool pk, bool fk, bool nn){name = n;datatype = dt, primarykey = pk;foreignkey = fk; notnull = nn;};
	~ColumnData(){};
};

struct TableData
{
	std::string name;
	std::vector<ColumnData> columns;
	float xposition;
	float yposition;
	TableData(std::string n, std::vector<ColumnData> c, float x, float y){name = n;columns = c;xposition=x;yposition=y;};
	~TableData(){};
};

class MainWindow : public Gtk::Window
{
public:
	MainWindow(const Glib::RefPtr<Gtk::Application>& app);
	virtual ~MainWindow();
	void addTableRow(ClutterActor *parent, std::vector<ColumnData> columns);
	void drawTable(ClutterActor *stage, TableData tableobject);
	void drawLine(ClutterActor *parent, ClutterActor *actor1, ClutterActor *actor2);
	void drawStage(ClutterActor *stage);

private:
	Gtk::Box m_VBox, m_Box;
	GtkWidget *clutter0;

};

#endif
