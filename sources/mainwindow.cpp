#include "mainwindow.h"
#include "ui_mainwindow.h"

// FIXME 330
// FIXME положение курсора
// TODO вычислять зеркала

#include <QDebug>
#include <QtMath>
#include <QEvent>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    graphicView = new MyGraphicView(scene);
    ui->formLayout->addWidget(graphicView);

    connect(ui->rfMin, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
    connect(ui->rfMax, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
    connect(ui->ifMin, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
    connect(ui->ifMax, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
    connect(ui->loMin, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
    connect(ui->loMax, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
    connect(ui->max, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
    connect(ui->min, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
    //connect(ui->parasitFreq, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));
    connect(ui->spectrumWidth, SIGNAL(textChanged(QString)), this, SLOT(validate(QString)));

    connect(ui->spectrum, SIGNAL(toggled(bool)), ui->spectrumWidth, SLOT(setEnabled(bool)));
   // connect(ui->parasit, SIGNAL(toggled(bool)), ui->parasitFreq, SLOT(setEnabled(bool)));

   // connect(ui->max, SIGNAL(textChanged(QString)), this, SLOT(updRange()));
   // connect(ui->min, SIGNAL(textChanged(QString)), this, SLOT(updRange()));


    ui->ifMax->setVisible(false);
    calc();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::updRange()
//{
//    ui->horizontalSlider->setRange(floor(ui->min->text().toFloat())
//                                   ,ceil(ui->max->text().toFloat()));
//}

void MainWindow::on_pushButton_clicked()
{
    scene->clear();

    if (!calc())
    {
        return;
    }

    ui->horizontalSlider->setRange(floor(ui->min->text().toFloat())
                                   ,ceil(ui->max->text().toFloat()));
    //ui->horizontalSlider->setValue(ui->); // TODO add

    QList<freqStruct> freqList;
    freqStruct freq;

    float maxM = 0;
    float maxN = 0;
    float ff1,ff2;

    float fLoMin = ui->loMin->text().toFloat();
    float fLoMax = ui->loMax->text().toFloat();
    float fRfMin = ui->rfMin->text().toFloat();
    float fRfMax = ui->rfMax->text().toFloat();

    if (ui->spectrum->isChecked())
    {
        fRfMin -= 0.5 * ui->spectrumWidth->text().toFloat();
        fRfMax += 0.5 * ui->spectrumWidth->text().toFloat();
    }

    for (int n = -(ui->n->value()); n <= ui->n->value(); n++)
    {
        for (int m = -(ui->m->value()); m <= ui->m->value(); m++)
        {
            if ((n == 0) && (m == 0))
            {
                continue;
            }

            // вычисляем
            ff1 = (fRfMin * m + fLoMin * n);
            ff2 = (fRfMax * m + fLoMax * n);
            freq.fmin = fmin(ff1,ff2);
            freq.fmax = fmax(ff1,ff2);

            // Добавляем паразиты
            if (ui->parasit->isChecked())
            {
                ff1 = (fRfMin * m + fLoMax * n);
                ff2 = (fRfMax * m + fLoMin * n);
                freq.fmin = fmin(fmin(ff1,ff2),freq.fmin);
                freq.fmax = fmax(fmax(ff1,ff2),freq.fmax);
            }

            // удаляем лишние
            if ((freq.fmax < ui->min->text().toFloat()) || (freq.fmin > ui->max->text().toFloat()))
            {
                continue;
            }
            // рисуем
            DiagramItem *rect = new DiagramItem(DiagramItem::SpurFreq, freq.fmin, freq.fmax, m, n); //TODO утечка памяти
            scene->addItem(rect);

            if ((abs(m)!=1) || (abs(n)!=1)) //TODO подсвечивать и только нужную (на основе формулы)
            {
                //добавляем в массив
                freqList.append(freq);
            }

            if (abs(m) > maxM)
            {
                maxM = m;
            }
            if (abs(n) > maxN)
            {
                maxN = n;
            }
        }
    }



//    // Добавляем паразиты
//    if (ui->parasit->isChecked())
//    {
//        for (int i = 0; i < 10; i++)
//        {
//            freq.fmin = (float)i * ui->parasitFreq->text().toFloat();
//            freq.fmax = freq.fmin;
//            //добавляем в массив
//            freqList.append(freq);
//            // рисуем
//            DiagramItem *rect = new DiagramItem(DiagramItem::SpurFreq, freq.fmin, freq.fmax, i, i); //TODO утечка памяти
//            scene->addItem(rect);
//        }
//    }


    if (!freqList.isEmpty())
    {
    qSort(freqList.begin(), freqList.end());

//    for (int i = 0; i < (freqList.size() ); i++)
//    {
//    qDebug() << ((freqStruct)freqList.at(i)).fmin;
//    }

    //for (int i = 0; freqList.at(i) != freqList.last() ; i++)
    for (int i = 0;  i<(freqList.size()-1) ; i++)
    //while (true)
    {
        while (((freqStruct)freqList.at(i+1)).fmax < ((freqStruct)freqList.at(i)).fmax)
        {
           // qDebug() << "del" << ((freqStruct)freqList.at(i+1)).fmin;
            freqList.removeAt(i+1);



            if (freqList.at(i) == freqList.last())
                break;
        }

        if (freqList.at(i) == freqList.last())
            break;
    }



//    for (int i = 0; i < (freqList.size() ); i++)
//    {
//    qDebug() << ((freqStruct)freqList.at(i)).fmin;
//    }

    float f0, f1;//, f2;

    for (int i = 0; i < (freqList.size() - 1); i++)
    {
        //f0 = ((freqStruct)freqList.at(i - 1)).fmax;
        f0 = ((freqStruct)freqList.at(i)).fmax;
        f1 = ((freqStruct)freqList.at(i + 1)).fmin;
        if ((f0 < f1) ) //Есть свободное место!
        {
            DiagramItem *rect = new DiagramItem(DiagramItem::FreeFreq, f0, f1, maxM, maxN); //TODO утечка памяти
            scene->addItem(rect);
        }
    }

//    for (int i = 1; i < (freqList.size() - 1); i++)
//    {
//        f0 = ((freqStruct)freqList.at(i - 1)).fmax;
//        f1 = ((freqStruct)freqList.at(i)).fmax;
//        f2 = ((freqStruct)freqList.at(i + 1)).fmin;
//        if ((f0 < f1) && (f1 < f2)) //Есть свободное место!
//        {
//            DiagramItem *rect = new DiagramItem(DiagramItem::FreeFreq, f1, f2, maxM, maxN); //TODO утечка памяти
//            scene->addItem(rect);
//        }
//    }
    // проверяем граничные
    f0 = ui->min->text().toFloat();
    f1 = ((freqStruct)freqList.first()).fmin;
    if (f0 < f1)
    {
        DiagramItem *rect = new DiagramItem(DiagramItem::FreeFreq, f0, f1, maxM, maxN); //TODO утечка памяти
        scene->addItem(rect);
    }
    f0 = ((freqStruct)freqList.last()).fmax;
    f1 = ui->max->text().toFloat();
    if (f0 < f1)
    {
        DiagramItem *rect = new DiagramItem(DiagramItem::FreeFreq, f0, f1, maxM, maxN); //TODO утечка памяти
        scene->addItem(rect);
    }
}

    DiagramItem *rect1 = new DiagramItem(DiagramItem::InputFreq, ui->rfMin->text().toFloat(), ui->rfMax->text().toFloat()); //TODO утечка памяти
    scene->addItem(rect1);
    DiagramItem *rect2 = new DiagramItem(DiagramItem::InputFreq, ui->loMin->text().toFloat(), ui->loMax->text().toFloat()); //TODO утечка памяти
    scene->addItem(rect2);
    DiagramItem *rect3 = new DiagramItem(DiagramItem::InputFreq, ui->ifMin->text().toFloat(), ui->ifMax->text().toFloat()); //TODO утечка памяти
    scene->addItem(rect3);

    float fIfMin = ui->ifMin->text().toFloat();
    float fIfMax = ui->ifMax->text().toFloat();
    float fMirrorMin;
    float fMirrorMax;
    if (ui->comboBox_3->currentText() == "IF=LO-RF")
    {
        fMirrorMin = fLoMin+fIfMin;
        fMirrorMax = fLoMax+fIfMax;
    }
    else if (ui->comboBox_3->currentText() == "IF=RF-LO")
    {
        fMirrorMin = fLoMin-fIfMin;
        fMirrorMax = fLoMax-fIfMax;
    }
    else if (ui->comboBox_3->currentText() == "IF=RF+LO")
    {
            exit(0);//FIXME
    }
    DiagramItem *rect4 = new DiagramItem(DiagramItem::InputFreq, fMirrorMin, fMirrorMax); //TODO утечка памяти
    scene->addItem(rect4);

    graphicView->run(ui->min->text().toFloat(), ui->max->text().toFloat());
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->ifMax->setVisible(true);
    ui->loMax->setVisible(true);
    ui->rfMax->setVisible(true);
    ui->ifLabelMax->setText("Max");
    ui->loLabelMax->setText("Max");
    ui->rfLabelMax->setText("Max");
    ui->ifLabelMin->setText("Min");
    ui->rfLabelMin->setText("Min");
    ui->loLabelMin->setText("Min");

    if (arg1 == "IF")
    {
        ui->ifMax->setVisible(false);
        ui->ifLabelMin->setText("Fix");
        ui->ifLabelMax->setText("");
        ui->statusBar->showMessage("Reciver mode", 1000);
    }
    else if (arg1 == "RF")
    {
        ui->rfMax->setVisible(false);
        ui->rfLabelMin->setText("Fix");
        ui->rfLabelMax->setText("");
        ui->statusBar->showMessage("Transmitter mode", 1000);
    }
    else if (arg1 == "LO")
    {
        ui->loMax->setVisible(false);
        ui->loLabelMin->setText("Fix");
        ui->loLabelMax->setText("");
        ui->statusBar->showMessage("Converter mode", 1000);
    }
    else if (arg1 == "All")
    {
        ui->loMax->setVisible(false);
        ui->ifMax->setVisible(false);
        ui->rfMax->setVisible(false);
        ui->loLabelMin->setText("Fix");
        ui->ifLabelMin->setText("Fix");
        ui->rfLabelMin->setText("Fix");
        ui->loLabelMax->setText("");
        ui->ifLabelMax->setText("");
        ui->rfLabelMax->setText("");
        ui->statusBar->showMessage("Simple mode", 1000);
    }
}

void MainWindow::on_comboBox_5_currentIndexChanged(const QString &arg1)
{
    ui->rfMax->setEnabled(true);
    ui->rfMin->setEnabled(true);
    ui->loMax->setEnabled(true);
    ui->loMin->setEnabled(true);
    ui->ifMax->setEnabled(true);
    ui->ifMin->setEnabled(true);

    if (arg1 == "IF")
    {
        ui->ifMax->setStyleSheet("");
        ui->ifMin->setStyleSheet("");
        ui->ifMax->setEnabled(false);
        ui->ifMin->setEnabled(false);
    }
    else if (arg1 == "RF")
    {
        ui->rfMax->setStyleSheet("");
        ui->rfMin->setStyleSheet("");
        ui->rfMax->setEnabled(false);
        ui->rfMin->setEnabled(false);
    }
    else if (arg1 == "LO")
    {
        ui->loMax->setStyleSheet("");
        ui->loMin->setStyleSheet("");
        ui->loMax->setEnabled(false);
        ui->loMin->setEnabled(false);
    }
}

void MainWindow::on_comboBox_3_currentIndexChanged(const QString &arg1)
{
    calc();
}

void MainWindow::validate(QString text)
{
    QLineEdit *sender = (QLineEdit *)QObject::sender();
    sender->setStyleSheet(""); //clear

    bool valid;
    float value = text.toFloat(&valid);

    if (!valid)
    {
        sender->setStyleSheet("border: 1px solid red;");

        ui->statusBar->showMessage("Invalid data", 1000);
        return;
    }

    if (value < 0)
    {
        sender->setStyleSheet("border: 1px solid red;");
        ui->statusBar->showMessage("Invalid freq", 1000);
        return;
    }
}


bool MainWindow::calc()
{
    disconnect(ui->rfMin, SIGNAL(textChanged(QString)), this, SLOT(calc()));
    disconnect(ui->rfMax, SIGNAL(textChanged(QString)), this, SLOT(calc()));
    disconnect(ui->ifMin, SIGNAL(textChanged(QString)), this, SLOT(calc()));
    disconnect(ui->ifMax, SIGNAL(textChanged(QString)), this, SLOT(calc()));
    disconnect(ui->loMin, SIGNAL(textChanged(QString)), this, SLOT(calc()));
    disconnect(ui->loMax, SIGNAL(textChanged(QString)), this, SLOT(calc()));

    if (ui->comboBox->currentText() == "RF")
    {
        ui->rfMax->setText(ui->rfMin->text());
    }
    else if (ui->comboBox->currentText() == "LO")
    {
        ui->loMax->setText(ui->loMin->text());
    }
    else if (ui->comboBox->currentText() == "IF")
    {
        ui->ifMax->setText(ui->ifMin->text());
    }
    else if (ui->comboBox->currentText() == "All")
    {
        ui->rfMax->setText(ui->rfMin->text());
        ui->loMax->setText(ui->loMin->text());
        ui->ifMax->setText(ui->ifMin->text());
    }

    float fMax, fMin;
    float rfMax = ui->rfMax->text().toFloat();
    float rfMin = ui->rfMin->text().toFloat();
    float loMax = ui->loMax->text().toFloat();
    float loMin = ui->loMin->text().toFloat();
    float ifMax = ui->ifMax->text().toFloat();
    float ifMin = ui->ifMin->text().toFloat();

    if (ui->comboBox_5->currentText() == "RF")
    {
        rfMax = 0;
        rfMin = 0;
    }
    else if (ui->comboBox_5->currentText() == "LO")
    {
        loMax = 0;
        loMin = 0;
    }
    else if (ui->comboBox_5->currentText() == "IF")
    {
        ifMax = 0;
        ifMin = 0;
    }

    if (ui->comboBox_3->currentText() == "IF=LO-RF")
    {
        fMax = loMax - rfMax - ifMax;
        fMin = loMin - rfMin - ifMin;
        if (ui->comboBox_5->currentText() == "LO")
        {
            fMax = -(fMax);
            fMin = -(fMin);
        }
    }
    else if (ui->comboBox_3->currentText() == "IF=RF-LO")
    {
        fMax = rfMax - loMax - ifMax;
        fMin = rfMin - loMin - ifMin;
        if (ui->comboBox_5->currentText() == "RF")
        {
            fMax = -(fMax);
            fMin = -(fMin);
        }
    }
    else if (ui->comboBox_3->currentText() == "IF=RF+LO")
    {
        fMax = rfMax + loMax - ifMax;
        fMin = rfMin + loMin - ifMin;
        if ((ui->comboBox_5->currentText() == "LO") ||
                (ui->comboBox_5->currentText() == "RF"))
        {
            fMax = -(fMax);
            fMin = -(fMin);
        }
    }

    if (ui->comboBox_5->currentText() == "RF")
    {
        ui->rfMax->setText(QString::number(fMax));
        ui->rfMin->setText(QString::number(fMin));
    }
    else if (ui->comboBox_5->currentText() == "LO")
    {
        ui->loMax->setText(QString::number(fMax));
        ui->loMin->setText(QString::number(fMin));
    }
    else if (ui->comboBox_5->currentText() == "IF")
    {
        ui->ifMax->setText(QString::number(fMax));
        ui->ifMin->setText(QString::number(fMin));
    }

    connect(ui->rfMin, SIGNAL(textChanged(QString)), this, SLOT(calc()));
    connect(ui->rfMax, SIGNAL(textChanged(QString)), this, SLOT(calc()));
    connect(ui->ifMin, SIGNAL(textChanged(QString)), this, SLOT(calc()));
    connect(ui->ifMax, SIGNAL(textChanged(QString)), this, SLOT(calc()));
    connect(ui->loMin, SIGNAL(textChanged(QString)), this, SLOT(calc()));
    connect(ui->loMax, SIGNAL(textChanged(QString)), this, SLOT(calc()));

    return true; // TODO
}

void MainWindow::on_horizontalSlider_valueChanged(int value) // TODO провекрить
{
    if (ui->comboBox->currentText() == "RF")
    {
        ui->rfMin->setText(QString::number(value));
    }
    else if (ui->comboBox->currentText() == "LO")
    {
        ui->loMin->setText(QString::number(value));
    }
    else if (ui->comboBox->currentText() == "IF")
    {
        ui->ifMin->setText(QString::number(value));
    }
    else if (ui->comboBox->currentText() == "All")
    {
        //exit(0);// TODO исправить
    }

    on_pushButton_clicked();

    //calc();
}
