/****************************************************************************
*  Copyright (C) 2011 Raphaël MARQUES <adrael_boy@live.fr>
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

/** --------------------------------------------------------------------------------------------------------------- **/

#include "HospitApp.hpp"

/** --------------------------------------------------------------------------------------------------------------- **/

int main(int argc, char ** argv)
{
        Q_INIT_RESOURCE(resources);
        QApplication app(argc, argv);
        app.setApplicationName("HospitApp");
        app.setWindowIcon(QIcon(":/icons/logo.png"));
        app.setQuitOnLastWindowClosed(true);
        QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

        HospitApp HA;
        HA.setWindowTitle("HospitApp");
        //HA.setFixedSize(1024, 768);
        HA.show();

        return app.exec();
}

/** --------------------------------------------------------------------------------------------------------------- **/
