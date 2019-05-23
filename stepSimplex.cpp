#include "mainwindow.h"
#include "ui_mainwindow.h"
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//----------------СЛОТ ДЛЯ ПОШАГОВОГО ВЫПОЛНЕНИЯ---------------------------------------------------------------
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
void MainWindow::on_stepSimplexBut_clicked()
{

    modIndex = simplexTabModel->index(permRow,permCol);
    simplexTabModel->setData(modIndex,QVariant(QBrush(Qt::transparent)), Qt::BackgroundRole);

    //стартуем счётчик времени
    mainTimer.start();


    //метод для симплес-преобразований
    simplexTransformation();

    //вызываем метод поиска базисных переменных и отрисовки таблицы
    bazisAndGetModel();

    //выполняем метод проверки на оптимальность
    checkOfOptimal();

    //выполняем метод выбора разрешающего столбца
    permittedCol();

    //выполняем метод выбора разрешающей строки
    permittedRow();
    //установим разрешающий элемент
    permItem = simplexTable[permRow][permCol];

    ui->status->append("Разрешающий столбец: " + QString::number(permCol));
    ui->status->append("Разрешающая строка: " + QString::number(permRow));
    ui->status->append("Разрешающий элемент: " + QString::number(permItem));

    //увеличиваем счётчик симплекс преобразований
    globalSteps++;

    nanoSec = mainTimer.nsecsElapsed();

    modIndex = simplexTabModel->index(permRow,permCol);
    simplexTabModel->setData(modIndex,QVariant(QBrush(Qt::red)), Qt::BackgroundRole);

    ui->status->append("Прошло (наносекунд):" +QString::number(nanoSec));
    //qDebug()<< "Прошло (наносекунд):" << nanoSec;
}

void MainWindow::on_stepSimplexButINT_clicked()
{

    modIndex = simplexTabModel->index(permRow,permCol);
    simplexTabModel->setData(modIndex,QVariant(QBrush(Qt::transparent)), Qt::BackgroundRole);

    //стартуем счётчик времени
    mainTimer.start();


    //метод для симплес-преобразований
    isimplexTransformation();

    //вызываем метод поиска базисных переменных и отрисовки таблицы
    ibazisAndGetModel();

    //выполняем метод проверки на оптимальность
    icheckOfOptimal();

    //выполняем метод выбора разрешающего столбца
    ipermittedCol();

    //выполняем метод выбора разрешающей строки
    ipermittedRow();
    //установим разрешающий элемент
    iPermItem = iSimplexTable[permRow][permCol];

    ui->status->append("Разрешающий столбец: " + QString::number(permCol));
    ui->status->append("Разрешающая строка: " + QString::number(permRow));
    ui->status->append("Разрешающий элемент: " + QString::number(iPermItem));

    //увеличиваем счётчик симплекс преобразований
    globalSteps++;

    nanoSec = mainTimer.nsecsElapsed();

    modIndex = simplexTabModel->index(permRow,permCol);
    simplexTabModel->setData(modIndex,QVariant(QBrush(Qt::red)), Qt::BackgroundRole);

    ui->status->append("Прошло (наносекунд):" +QString::number(nanoSec));
    //qDebug()<< "Прошло (наносекунд):" << nanoSec;
}

