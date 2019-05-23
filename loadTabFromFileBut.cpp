#include "mainwindow.h"
#include "ui_mainwindow.h"
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//----------------СЛОТ ДЛЯ ЗАГРУЗКИ ТАБЛИЦЫ ИЗ ФАЙЛА-----------------------------------------------------------
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************

void MainWindow::on_loadTabFromFileBut_clicked()
{
    //-----------------------------------------
    //здесь загружаем таблицу из файла в модель
    //-----------------------------------------

    //очищаем модель если уже нажмали на кнопку
    simplexTable.clear();

    //открываю файл для чтения
    QFile inputFile("simplex.csv");
    inputFile.open(QFile::ReadOnly | QFile::Text);

    //обработка ошибок открытия файла
    if(!inputFile.isOpen()){
        //если есть ошибки, то выводим в консоль
        ui->status->append("Ошибка открытия файла!");
    }
    else{
        //если файл открылся, то начинаем его читать построчно
        //открываем поток для чтения из файла

        ui->status->append("Входной файл открыт для  чтения!");

        QTextStream inStream (&inputFile);

        //установливаем кодировку...
        inStream.setCodec("UTF-8");

        maxCol=0;


        //читаем построчно
        while(!inStream.atEnd()){
            //читаем из потока очередную строку во временную переменную
            tmpString = inStream.readLine();

            //распрасим строку по разделителю (знак ",") и запишем её в список строк
            tmpStringList = tmpString.split(",");

            //определяем количество элементов в строке (это будет количество колонок в модели)
            if(tmpStringList.size() > maxCol) maxCol = tmpStringList.size();


            //переберём все элементы из списка строк и запишем их в вектор чисел
            foreach(tmpString, tmpStringList){

                //если ячейка в таблице была пустая, то считаем её нулём
                if(!tmpString.isEmpty()) tmpFloat = tmpString.toFloat();
                else tmpFloat = 0.0;

                //записываем значение во временный вектор,
                //который потом добавим в симплекс-таблицу
                tmpVector.append(tmpFloat);
            }

            //получившийся вектор запишем в список векторов,
            //который и будет нашей исходной симплекс-таблицей
            simplexTable.append(tmpVector);

            //очищаем временный вектор
            tmpVector.clear();

        }

        //сохраним количество столбцов в исходной таблице
        maxColStart = maxCol;

        ui->status->append("В исходной таблице " + QString::number(maxColStart) + " столбцов");

        //формируем вектор r
        r = simplexTable.last();
        r.pop_back();
        tmpString.clear();
        foreach(i,r)
            tmpString.append(QString::number(i) + ",");
        ui->status->append("Вектор r на старте: " + tmpString);

        //заполним вектор b
        colB.fill(0,simplexTable.size()-1);

        //определим, есть ли базисные переменные в исходном плане
        //для этого переберём значения в симплекс-таблице по столбцам и заполним вектор базисов
        //если там будут только "минус единицы", то добавим квадратную матрицу, иначе прямоугольную
        bazisSimplex.fill(-1,simplexTable.size()-1);
        colBazis = 0;
        for(i=0; i < maxCol-1; i++){
            notNull = 0;
            for(j=0; j < simplexTable.size()-1; j++){
                if(fabs(simplexTable[j][i]) > 0){
                    numNull=j;
                    notNull++;
                }
            }
            if(notNull == 1) {
                bazisSimplex[numNull] = i;
                colBazis++;
            }
        }

        //выводим строку в статус-бокс
        tmpString.clear();
        foreach(i,bazisSimplex)
            tmpString.append(QString::number(i) + ",");

        ui->status->append("Базис на старте: " + tmpString);

        //добавляем базисные переменные, если на предыдущем шаге не было найдено базисных переменных,
        //то добавляем квадратную нулевую матрицу
        //с единицами по диагонали. Количсетво переменных равно количеству строк в исходной
        //таблице без последней.
        if(colBazis == 0){
            ui->status->append("В исходном плане НЕ найдены базисные переменные");

            //добавляем в каждую строку исходной симплекс таблицы полученный вектор
            currentNumOfOne = maxCol-1;//это текущая позиция, в которой будет стоять единица
                                        //на каждой итерации она будет инкрементироваться
            for(i=0; i<simplexTable.size()-1; i++){
                for(j=maxCol-1; j<((maxCol-1)+(simplexTable.size()-1)); j++){
                    //если номер текущей позиции равен номеру в которой нужно добавить единицу,
                    //то добавляем единицу, иначе 0
                    if(currentNumOfOne == j) simplexTable[i].insert(j,1);
                    else simplexTable[i].insert(j,0);
                }
                currentNumOfOne++;
            }

            //в последнюю строку (целевую функцию) добавляем необходимое количество нулей
            for(j=maxCol-1; j<((maxCol-1)+(simplexTable.size()-1)); j++){
                simplexTable[simplexTable.size()-1].insert(j,0);
            }

            //не забываем определить "ширину" таблицы и модели
            maxCol = maxCol + simplexTable.size();
        }

        else{
            ui->status->append("В исходном плане найдены базисные переменные");
            //qDebug()<<"в исходном плане найдены базисные переменные";
            //определяем сколько нам нужно добавить переменных
            colBazis = - colBazis + bazisSimplex.size();

            //ответим в статус-бокс
            ui->status->append("Нужно добавить " + QString::number(colBazis) + " переменных");

            //просто допишем в каждое уравнение необходимое количество нулей
            //на следующем этапе в нужных местах поставим 1
            for(i=0; i < simplexTable.size(); i++){
                for(j=maxCol-1; j<(maxCol-1+colBazis); j++){
                    simplexTable[i].insert(j,0);
                }
            }

            //поставим в нужных местах 1
            //стартовую позицию в которую поставим 1 установим в конец матрицы
            currentNumOfOne = maxCol-1;
            for(i=0; i < bazisSimplex.size(); i++){
                if (bazisSimplex[i] == -1) {
                    simplexTable[i][currentNumOfOne] = 1;
                    currentNumOfOne++;
                }
            }

            //не забываем определить "ширину" таблицы и модели
            maxCol = maxCol + colBazis+1;
        }

        //заполняем горизонтальный заголовок
        tmpStringList.clear();
        for(i=0; i < maxCol-2; i++){
            tmpStringList.append("x" + QString::number(i));
        }
        tmpStringList.append("H");
        tmpStringList.append("b");
        simplexTabModel->setHorizontalHeaderLabels(tmpStringList);

        //устанавливаем размер модели по количеству строк и столбцов в симплекс-таблице
        simplexTabModel->setRowCount(simplexTable.size());
        simplexTabModel->setColumnCount(maxCol);

        bazisAndGetModel();//вызываем метод поиска базисных переменных и отрисовки таблицы

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

        //подсветим разрешающий элемент
        modIndex = simplexTabModel->index(permRow,permCol);
        simplexTabModel->setData(modIndex,QVariant(QBrush(Qt::red)), Qt::BackgroundRole);

        //сохраняем исходные данные
        simplexTableConst = simplexTable;
        colBConst = colB;//столбец значений b
        rConst = r;//вектор коэффициентов при свободных неизвестных целевой функции
        bazisSimplexConst = bazisSimplex;//базисные переменные
        permColConst = permCol;
        permRowConst = permRow;
        permItemConst = permItem;

        inputFile.flush();
        inputFile.close();
    }
}

