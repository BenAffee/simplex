#include "mainwindow.h"
#include "ui_mainwindow.h"
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//----------------Методы для действий симплекс метода С ЦЕЛОЧИСЛЕННЫМИ КОЭФФИЦИЕНТАМИ-----------------------------------------------------------
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************

//*************************************************************************************************************
//*************************************************************************************************************
//----------------Проверка на оптимальность--------------------------------------------------------------------
//*************************************************************************************************************
//*************************************************************************************************************

void MainWindow::icheckOfOptimal(){
    //проверяем план на оптимальность
    //если среди коэффициентов целевой функции нет отрицательных элементов, то план оптимален

    //будем перебирать коэффициенты вектора r до тех пор,
    //пока не найдём отрицательный элемент. Если он найден,
    //то признак оптимальности установим в false


    //устанавливаем признак оптимальности в истину
    optimal = true;

    //tmpVector = simplexTable.last();
    for(i=0; i < iR.size(); i++)
        if (iR[i] < 0) optimal = false;

    if(optimal == true){
        ui->status->append("Найдено оптимальное решение!");
        msgBox.setText("Найдено оптимальное решение!");
        msgBox.exec();
        return;
    }

    ui->status->append("================================================");
    ui->status->append("Решение НЕ оптимально, продолжаем...");
    ui->status->append("Симплекс шаг: " + QString::number(globalSteps));
}

void MainWindow::ipermittedCol(){
    //выбираем разрешающий столбец
    //это максимальный по модулю отрицательный компонент вектора r
    iminNum = -1;
    iMin = 0;
    for(i=0; i < iR.size(); i++){
        if ((iR[i] < 0) && (abs(iR[i]) > iMin)){
            iMin = iR[i];
            iminNum = i;
        }
    }
    if(iminNum == -1){
        ui->status->append("Ошибка! Не определён максимальный по модулю отрицательный компонент вектора r");
        return;
    }

    //устанавливаем разрешающий столбец
    permCol = iminNum;
}

void MainWindow::ipermittedRow(){
    //рассчитаем столбец значений b=h/(элементы разрешающего столбца)
    for(i=0; i < iSimplexTable.size()-1; i++){
        iTmpVector = iSimplexTable[i];
        if(iTmpVector[permCol] > 0)
            iColB[i] = iTmpVector.last() / iTmpVector[permCol];
        else
            iColB[i] = -1;

        //сразу запишем в модель...
        modIndex = simplexTabModel->index(i, maxCol-1);
        simplexTabModel->setData(modIndex,iColB[i]);
    }
    //найдём минимальный положительный элемент в столбце b
    //для этого сначала найдём первый положительный элемент в столбце,
    //а затем сравним его с остальными
    i=0;
    while(iColB[i]<0){
        i++;
    }
    iMin=iColB[i];//сюда запишется первый положительный или нулевой элемент
    iminNum = i;//...а сюда его номер
    for(i=iminNum; i < iColB.size(); i++){
        if ((iColB[i] >= 0) && (iColB[i] < iMin)){
            iMin = iColB[i];
            iminNum = i;
        }
    }

    //установим разрешающую строку
    permRow = iminNum;
}

void MainWindow::isimplexTransformation(){
    //начинаем симплекс-преобразования
    //реплицируем симплекс-таблицу
    //(из одной будем брать исходные данные, а во вторую подставлять результаты)
    iSimplexTableRepl = iSimplexTable;

    //элементы не разрешающего столбца и строки пересчитаем по правилу прямоугольника
    for(i=0; i < iSimplexTable.size(); i++){
        if (i != permRow){
            for(j=0; j < maxCol-1; j++){
                //если текущий элемент не относится к разрешающей строке или столбцу,
                //то пересчитываем, иначе просто пропускаем
                if(j != permCol)
                    iSimplexTableRepl[i][j] = iSimplexTable[i][j] - (iSimplexTable[permRow][j] * iSimplexTable[i][permCol]);
                    //iSimplexTableRepl[i][j] = iSimplexTable[i][j] - (iSimplexTable[permRow][j] * iSimplexTable[i][permCol] / iPermItem);
            }
        }

    }

    //разрешающий элемент превращаем в 1
    //в целочисленном варианте маожно не делать
    //т.к. разрешающий элемент у нас всегда 1
    //iSimplexTableRepl[permRow][permCol] = 1;

    //элементы разрешающей строки делим на разрешающий элемент
    //в целочисленном варианте маожно не делать
    //т.к. разрешающий элемент у нас всегда 1
    //for(i=0; i < maxCol-1; i++)
        //if (i != permCol) iSimplexTableRepl[permRow][i] = iSimplexTable[permRow][i] / iPermItem;

    //элементы разрешающего столбца превращаем в 0
    for(i=0; i < iSimplexTableRepl.size(); i++)
        if (i != permRow) iSimplexTableRepl[i][permCol] = 0;

    //переписываем реплику симплекс таблицы в исходную
    iSimplexTable = iSimplexTableRepl;

    //записываем вектор r
    iTmpVector = iSimplexTable.last();
    iR.clear();
    for(i=0; i<maxColStart; i++)
        iR.append(iTmpVector[i]);

    //отражаем вектор r в статус-бокс
    tmpString.clear();
    foreach(i,iR)
        tmpString.append(QString::number(i) + ",");
    ui->status->append("Вектор r: " + tmpString);
}

//симплекс-преобразования без ввода вывода

void MainWindow::isimplexTransformationNoOutput(){
    //начинаем симплекс-преобразования
    //реплицируем симплекс-таблицу
    //(из одной будем брать исходные данные, а во вторую подставлять результаты)
    iSimplexTableRepl = iSimplexTable;

    //элементы не разрешающего столбца и строки пересчитаем по правилу прямоугольника
    for(i=0; i < iSimplexTable.size(); i++){
        if (i != permRow){
            for(j=0; j < maxCol-1; j++){
                //если текущий элемент не относится к разрешающей строке или столбцу,
                //то пересчитываем, иначе просто пропускаем
                if(j != permCol){
                    if(iSimplexTable[permRow][j] == 0 || iSimplexTable[i][permCol] == 0) continue;
                    iSimplexTableRepl[i][j] = iSimplexTable[i][j] - (iSimplexTable[permRow][j] * iSimplexTable[i][permCol]);
                    //iSimplexTableRepl[i][j] = iSimplexTable[i][j] - (iSimplexTable[permRow][j] * iSimplexTable[i][permCol] / iPermItem);
               }
            }

        }

    }

    //разрешающий элемент превращаем в 1
    //iSimplexTableRepl[permRow][permCol] = 1;

    //элементы разрешающей строки делим на разрешающий элемент
    //в целочисленном варианте маожно не делать
    //т.к. разрешающий элемент у нас всегда 1
    //for(i=0; i < maxCol-1; i++)
        //if (i != permCol) iSimplexTableRepl[permRow][i] = iSimplexTable[permRow][i] / iPermItem;

    //элементы разрешающего столбца превращаем в 0
    for(i=0; i < iSimplexTableRepl.size(); i++)
        if (i != permRow) iSimplexTableRepl[i][permCol] = 0;

    //переписываем реплику симплекс таблицы в исходную
    iSimplexTable = iSimplexTableRepl;

    //записываем вектор r
    iTmpVector = iSimplexTable.last();
    iR.clear();
    for(i=0; i<maxColStart; i++)
        iR.append(iTmpVector[i]);

}

void MainWindow::icheckOfOptimalNoOutput(){
    //проверяем план на оптимальность
    //если среди коэффициентов целевой функции нет отрицательных элементов, то план оптимален

    //будем перебирать коэффициенты вектора r до тех пор,
    //пока не найдём отрицательный элемент. Если он найден,
    //то признак оптимальности установим в false


    //устанавливаем признак оптимальности в истину
    optimal = true;

    //tmpVector = simplexTable.last();
    for(i=0; i < iR.size(); i++)
        if (iR[i] < 0) optimal = false;



}

void MainWindow::ipermittedRowNoOutput(){
    //рассчитаем столбец значений b=h/(элементы разрешающего столбца)
    for(i=0; i < iSimplexTable.size()-1; i++){
        iTmpVector = iSimplexTable[i];
        if(iTmpVector[permCol] > 0)
            iColB[i] = iTmpVector.last() / iTmpVector[permCol];
        else
            iColB[i] = -1;
    }

    //найдём минимальный положительный элемент в столбце b
    //для этого сначала найдём первый положительный элемент в столбце,
    //а затем сравним его с остальными
    i=0;
    while(iColB[i]<0){
        i++;
    }
    iMin=iColB[i];//сюда запишется первый положительный или нулевой элемент
    iminNum = i;//...а сюда его номер
    for(i=iminNum; i < iColB.size(); i++){
        if ((iColB[i] >= 0) && (iColB[i] < iMin)){
            iMin = iColB[i];
            iminNum = i;
        }
    }

    //установим разрешающую строку
    permRow = iminNum;
}
