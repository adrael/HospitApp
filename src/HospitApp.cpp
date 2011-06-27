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

HospitApp::HospitApp(QWidget * parent) : QMainWindow(parent)
{

        QTabWidget * globalWidget = new QTabWidget(this);

        globalWidget -> addTab(this -> makeRadio(globalWidget), QIcon(":/radio/preview.jpg"), "Radio");
        globalWidget -> addTab(this -> makeScan(globalWidget), QIcon(":/scan/preview.jpg"),"Scan");
        globalWidget -> addTab(this -> makeEcg(globalWidget), QIcon(":/ecg/preview.png"),"ECG");
        globalWidget -> addTab(this -> make3D(globalWidget), QIcon(":/3d/preview.png"),"3D");

        this -> setCentralWidget(globalWidget);
}

/** --------------------------------------------------------------------------------------------------------------- **/

HospitApp::~HospitApp()
{
        Q_CLEANUP_RESOURCE(resources);
}

/** --------------------------------------------------------------------------------------------------------------- **/

QWidget * HospitApp::make3D(QWidget * parent)
{
        QWidget * tmp = new QWidget(parent);

        return tmp;
}

/** --------------------------------------------------------------------------------------------------------------- **/

QWidget * HospitApp::makeRadio(QWidget * parent)
{
        QWidget * tmp = new QWidget(parent);
        QSignalMapper * radioMapper = new QSignalMapper(tmp);

        QGridLayout * radioLayout = new QGridLayout(tmp);
        radioLayout -> setContentsMargins(0, 0, 0, 0);
        radioLayout -> setSpacing(0);

        QCursor cursor(QPixmap(":/icons/expand.png").scaled(22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        const int width(73), height(77);

        for(int i(0); i < 10; ++i) {
                for(int j(0); j < 10; ++j) {
                        QPushButton * button = new QPushButton(QIcon(QPixmap(":/radio/radio_" + QString::number(i) + "_" + QString::number(j) + ".jpg").scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation)), "", tmp);
                        button -> setFlat(true);
                        button -> setIconSize(QSize(width, height));
                        button -> setFixedSize(width, height);
                        button -> setCursor(cursor);

                        this -> connect(button, SIGNAL(clicked()), radioMapper, SLOT(map()));
                        radioMapper -> setMapping(button, (i * 10) + j);
                        radioLayout -> addWidget(button, i, j);
                }
        }

        this -> connect(radioMapper, SIGNAL(mapped(const int)), this, SLOT(expand(const int)));

        tmp -> setLayout(radioLayout);
        return tmp;
}

/** --------------------------------------------------------------------------------------------------------------- **/

QWidget * HospitApp::makeScan(QWidget * parent)
{
        QWidget * tmp = new QWidget(parent);
        QVBoxLayout * multiLayout = new QVBoxLayout(tmp);
        QHBoxLayout * controlLayout = new QHBoxLayout();

        this -> playScan = new QPushButton("       Play scanner", tmp);
        playScan -> setStyleSheet("QPushButton {color: white; border: none; image: url(:/icons/play.png); width: 128px; height: 32px;} QPushButton:hover {image: url(:/icons/play_h.png);} QPushButton:pressed {color: gray; image: url(:/icons/play_p.png);}");
        playScan -> setFixedSize(128, 32);
        playScan -> setCursor(Qt::PointingHandCursor);

        this -> pauseScan = new QPushButton("       Pause scanner", tmp);
        pauseScan -> setStyleSheet("QPushButton {color: white; border: none; image: url(:/icons/pause.png); width: 128px; height: 32px;} QPushButton:hover {image: url(:/icons/pause_h.png);} QPushButton:pressed {color: gray; image: url(:/icons/pause_p.png);}");
        pauseScan -> setFixedSize(128, 32);
        pauseScan -> setCursor(Qt::PointingHandCursor);

        this -> nextScan = new QPushButton("       Next scanner", tmp);
        nextScan -> setStyleSheet("QPushButton {color: white; border: none; image: url(:/icons/next.png); width: 128px; height: 32px;} QPushButton:hover {image: url(:/icons/next_h.png);} QPushButton:pressed, QPushButton:disabled {color: gray; image: url(:/icons/next_p.png);}");
        nextScan -> setFixedSize(128, 32);
        nextScan -> setCursor(Qt::PointingHandCursor);

        this -> previousScan = new QPushButton("       Previous scanner", tmp);
        previousScan -> setStyleSheet("QPushButton {color: white; border: none; image: url(:/icons/previous.png); width: 128px; height: 32px;} QPushButton:hover {image: url(:/icons/previous_h.png);} QPushButton:pressed, QPushButton:disabled {color: gray; image: url(:/icons/previous_p.png);}");
        previousScan -> setFixedSize(128, 32);
        previousScan -> setCursor(Qt::PointingHandCursor);

        this -> scan = new QLabel(tmp);
        this -> noScan = 0;
        this -> upScan = new QTimer(tmp);
        this -> upScan -> setInterval(90);
        this -> scan -> setPixmap(QPixmap(":/scan/scan_" + QString::number(noScan) + ".jpg"));
        this -> connect(upScan, SIGNAL(timeout()), this, SLOT(updateScan()));

        this -> pauseScan -> setVisible(false);

        this -> connect(this -> playScan, SIGNAL(clicked()), upScan, SLOT(start()));
        this -> connect(this -> pauseScan, SIGNAL(clicked()), upScan, SLOT(stop()));

        this -> connect(this -> playScan, SIGNAL(clicked()), this, SLOT(change()));
        this -> connect(this -> pauseScan, SIGNAL(clicked()), this, SLOT(change()));

        this -> connect(this -> nextScan, SIGNAL(clicked()), this, SLOT(updateScan()));
        this -> connect(this -> previousScan, SIGNAL(clicked()), this, SLOT(updateScan2()));

        controlLayout -> addWidget(this -> previousScan);
        controlLayout -> addWidget(this -> playScan);
        controlLayout -> addWidget(this -> pauseScan);
        controlLayout -> addWidget(this -> nextScan);

        multiLayout -> addWidget(scan);
        multiLayout -> addLayout(controlLayout);

        tmp -> setLayout(multiLayout);
        return tmp;
}

/** --------------------------------------------------------------------------------------------------------------- **/

QWidget *  HospitApp::makeEcg(QWidget * parent)
{
        QWidget * tmp = new QWidget(parent);
        QVBoxLayout * global = new QVBoxLayout(tmp);
        QHBoxLayout * control = new QHBoxLayout();

        QPushButton * selectEcg = new QPushButton("       Select an other ECG", tmp);
        selectEcg -> setStyleSheet("QPushButton {color: white; border: none; image: url(:/icons/open.png); width: 128px; height: 32px;} QPushButton:hover {image: url(:/icons/open_h.png);} QPushButton:pressed {color: gray; image: url(:/icons/open_p.png);}");
        selectEcg -> setFixedSize(128, 32);
        selectEcg -> setCursor(Qt::PointingHandCursor);
        this -> connect(selectEcg, SIGNAL(clicked()), this, SLOT(getPath()));

        QPushButton * printEcg = new QPushButton("       Print current ECG", tmp);
        printEcg -> setStyleSheet("QPushButton {color: white; border: none; image: url(:/icons/print.png); width: 128px; height: 32px;} QPushButton:hover {image: url(:/icons/print_h.png);} QPushButton:pressed {color: gray; image: url(:/icons/print_p.png);}");
        printEcg -> setFixedSize(128, 32);
        printEcg -> setCursor(Qt::PointingHandCursor);
        this -> connect(printEcg, SIGNAL(clicked()), this, SLOT(print()));

        this -> myPlot = new QwtPlot(parent);
        this -> myPlot -> setCanvasBackground(QColor(81, 145, 198));
        this -> myPlot -> setTitle("ECG");
        this -> myPlot -> setAxisTitle(QwtPlot::xBottom, QString("Second"));
        this -> myPlot -> setAxisTitle(QwtPlot::yLeft, QString("Beat"));
        this -> myPlot -> plotLayout() -> setAlignCanvasToScales(true);

        this -> zoom = new QwtPlotZoomer(myPlot -> canvas());
        this -> zoom -> setTrackerMode(QwtPicker::AlwaysOn);
        this -> zoom -> setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
        this -> zoom -> setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);

        QwtPlotPanner * panner = new QwtPlotPanner(myPlot -> canvas());
        panner -> setMouseButton(Qt::MidButton);


        this -> selection("../resources/ecg/ecg.txt");

        control -> addWidget(selectEcg);
        control -> addWidget(printEcg);

        global -> addWidget(this -> myPlot);
        global -> addLayout(control);

        tmp -> setLayout(global);

        return tmp;
}

/** --------------------------------------------------------------------------------------------------------------- **/

void HospitApp::print()
{
        QPrinter printer(QPrinter::HighResolution);

        QString docName = myPlot -> title().text();
        if (!docName.isEmpty()) {
                docName.replace(QRegExp(QString::fromLatin1("\n")), tr(" -- "));
                printer.setDocName(docName);
        }

        printer.setCreator("ECG");
        printer.setOrientation(QPrinter::Landscape);

        QPrintDialog dialog(&printer);
        if (dialog.exec()) {
                QwtPlotRenderer renderer;

                if (printer.colorMode() == QPrinter::GrayScale) {
                        renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground);
                        renderer.setLayoutFlag(QwtPlotRenderer::FrameWithScales);
                }

                renderer.renderTo(myPlot, printer);
        }
}

/** --------------------------------------------------------------------------------------------------------------- **/

void HospitApp::getPath()
{
        this -> selection(QFileDialog::getOpenFileName(this, trUtf8("Open existing ECG"), QDir::homePath(), trUtf8("All files (*)")));
}

/** --------------------------------------------------------------------------------------------------------------- **/

void HospitApp::selection(QString path)
{
        this -> myPlot -> detachItems();

        QwtPlotGrid  * myGrid = new QwtPlotGrid();

        QwtPlotCurve * myCurve = new QwtPlotCurve();
        myCurve -> setRenderHint(QwtPlotItem::RenderAntialiased);
        myCurve -> setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
        myCurve -> setPen(QPen(Qt::white));
        myCurve -> setStyle(QwtPlotCurve::Lines);
        myCurve -> setCurveAttribute(QwtPlotCurve::Fitted);

        QwtPlotMarker * mY = new QwtPlotMarker();
        mY -> setLabel(QString::fromLatin1("y = 0"));
        mY -> setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
        mY -> setLineStyle(QwtPlotMarker::HLine);
        mY -> setYValue(0.0);

        QFile fichier(path);
        bool ok;
        double cpt(0.000);
        QLocale c(QLocale::French);

        if (fichier.open(QIODevice::ReadOnly)) {
                QTextStream stream(&fichier);
                this -> y.clear();
                this -> x.clear();
                while (!stream.atEnd()) {
                        this -> y.append(c.toDouble(stream.readLine(), &ok));
                        this -> x.append(cpt);
                        cpt += 0.001;
                }
                fichier.close();
        }

        myCurve -> setRawSamples(this -> x.data(), this -> y.data(), this -> y.size());

        myCurve -> attach(this -> myPlot);
        myGrid -> attach(this -> myPlot);
        mY -> attach(this -> myPlot);

        this -> myPlot -> setAxisAutoScale(0);
        this -> myPlot -> setAxisAutoScale(2);

        this -> zoom -> setZoomBase();

        this -> myPlot -> replot();
}

/** --------------------------------------------------------------------------------------------------------------- **/

void HospitApp::change()
{
        bool v1(this -> playScan -> isVisible());
        this -> playScan -> setVisible(!v1);
        this -> pauseScan -> setVisible(!this -> pauseScan -> isVisible());
        this -> previousScan -> setEnabled(!v1);
        this -> nextScan -> setEnabled(!v1);
}

/** --------------------------------------------------------------------------------------------------------------- **/

void HospitApp::updateScan2()
{
        this -> updateScan(-1);
}

/** --------------------------------------------------------------------------------------------------------------- **/

void HospitApp::updateScan(const int inc)
{
        this -> noScan += inc;
        if(this -> noScan > 39) this -> noScan = 0;
        else if(this -> noScan < 0) this -> noScan = 39;
        this -> scan -> setPixmap(QPixmap(":/scan/scan_" + QString::number(noScan) + ".jpg"));
}

/** --------------------------------------------------------------------------------------------------------------- **/

void HospitApp::expand(const int tmp)
{
        const int i(tmp / 10), j(tmp - (10 * (tmp / 10))), width(166), height(175);
        QDialog * that = new QDialog(this, Qt::Popup);
        QGridLayout * zoom = new QGridLayout(that);
        zoom -> setContentsMargins(5, 5, 5, 5);
        zoom -> setSpacing(0);

        QCursor cursor(QPixmap(":/icons/shrink.png").scaled(22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        for(int ii((i > 0 ? (i - 1) : 0)); ii <= 9 && ii <= i + 1; ++ii) {
                for(int jj((j > 0 ? (j - 1) : 0)); jj <= 9 && jj <= j + 1; ++jj) {
                        QString ID(QString::number(ii) + "_" + QString::number(jj));
                        QPushButton * expanded = new QPushButton(QIcon(QPixmap(":/radio/radio_" + ID + ".jpg").scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation)), "", that);
                        expanded -> setFlat(true);
                        expanded -> setIconSize(QSize(width, height));
                        expanded -> setFixedSize(width, height);
                        expanded -> setCursor(cursor);

                        zoom -> addWidget(expanded, ii, jj);
                        this -> connect(expanded, SIGNAL(clicked()), that, SLOT(close()));
                }
        }

        that -> setLayout(zoom);
        that -> show();
}

/** --------------------------------------------------------------------------------------------------------------- **/
