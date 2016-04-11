/*
 * Copyright 2016 Vyacheslav Verkhovin
 *
 * This file is part of MixCalc.
 *
 * MixCalc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RxCalc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RxCalc. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "mygraphicview.h"
#include "myrect.h"

struct freqStruct {
    bool operator<(const freqStruct& other) const {
        return fmin < other.fmin; // sort by fmin
    }
    bool operator==(const freqStruct& other) const {
        return (fmin == other.fmin) && (fmin == other.fmin);
    }
    bool operator!=(const freqStruct& other) const {
        return (fmin != other.fmin) || (fmin != other.fmin);
    }
    float fmin;
    float fmax;
};

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_comboBox_5_currentIndexChanged(const QString &arg1);
    void on_comboBox_3_currentIndexChanged(const QString &arg1);
    void validate(QString text);
    bool calc();
   // void updRange();

    void on_horizontalSlider_valueChanged(int value);

    //bool namefileLessThan(const fr &d1, const fr &d2);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    MyGraphicView *graphicView;
};

#endif // MAINWINDOW_H
