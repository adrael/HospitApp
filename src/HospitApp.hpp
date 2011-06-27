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

#ifndef HOSPIT_APP
#define HOSPIT_APP

/** --------------------------------------------------------------------------------------------------------------- **/

#include <QDebug>

#include <QSignalMapper>
#include <QApplication>
#include <QPrintDialog>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QPrinter>
#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QFile>

#include <qwt_plot.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_renderer.h>

/** --------------------------------------------------------------------------------------------------------------- **/

class HospitApp : public QMainWindow
{
        Q_OBJECT

public:
        HospitApp(QWidget * parent = 0);
        ~HospitApp();

public slots:
        void expand(const int);
        void updateScan(const int inc = 1);
        void updateScan2();
        void change();
        void selection(QString);
        void getPath();
        void print();

        QWidget * makeRadio(QWidget *);
        QWidget * makeScan(QWidget *);
        QWidget * make3D(QWidget *);
        QWidget * makeEcg(QWidget *);

private:

        // SCAN WIDGETS
        QLabel * scan;
        QTimer * upScan;
        QPushButton * playScan, * pauseScan, * nextScan, * previousScan;
        int noScan;

        // ECG WIDGETS
        QVector<double> x;
        QVector<double> y;
        QwtPlot * myPlot;
        QwtPlotZoomer * zoom;


private slots:

};

/** --------------------------------------------------------------------------------------------------------------- **/

#endif // HOSPIT_APP
