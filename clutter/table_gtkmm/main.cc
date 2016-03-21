/* Copyright (C) 2016 Andreas Österreicher
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
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
#include "mainwindow.h"
#include <gtkmm/application.h>
#include <clutter/clutter.h>
#include <iostream>

int main(int argc, char *argv[])
{
	if(CLUTTER_INIT_SUCCESS !=clutter_init (&argc, &argv))
		std::cout << "clutter init failed" << std::endl;

	auto app = Gtk::Application::create(argc, argv, "oesi.org.example.clutter.gtkmm");
	MainWindow window(app);

	return app->run(window);
}
