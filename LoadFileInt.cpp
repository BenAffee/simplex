#include "mainwindow.h"
#include "ui_mainwindow.h"
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//---------------------------ЗАГРУЗКА СИМПЛЕКС-ТАБЛИЦЫ В РЕЖИМЕ INT--------------------------------------------
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
void MainWindow::on_loadTabFromFileButInt_clicked()
{
    //-----------------------------------------
    //здесь загружаем таблицу из файла в модель
    //-----------------------------------------

    //очищаем модель если уже нажмали на кнопку
    iSimplexTable.clear();

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
                if(!tmpString.isEmpty()) iTmp = tmpString.toInt();
                else iTmp = 0;

                //записываем значение во временный вектор,
                //который потом добавим в симплекс-таблицу
                iTmpVector.append(iTmp);
            }

            //получившийся вектор запишем в список векторов,
            //который и будет нашей исходной симплекс-таблицей
            iSimplexTable.append(iTmpVector);

            //очищаем временный вектор
            iTmpVector.clear();

        }

        //сохраним количество столбцов в исходной таблице
        maxColStart = maxCol;

        ui->status->append("В исходной таблице " + QString::number(maxColStart) + " столбцов");

        //формируем вектор r
        iR = iSimplexTable.last();
        iR.pop_back();
        tmpString.clear();
        foreach(i,iR)
            tmpString.append(QString::number(i) + ",");
        ui->status->append("Вектор r на старте: " + tmpString);

        //заполним вектор b
        iColB.fill(0,iSimplexTable.size()-1);

        //определим, есть ли базисные переменные в исходном плане
        //для этого переберём значения в симплекс-таблице по столбцам и заполним вектор базисов
        //если там будут только "минус единицы", то добавим квадратную матрицу, иначе прямоугольную
        bazisSimplex.fill(-1,iSimplexTable.size()-1);
        colBazis = 0;
        for(i=0; i < maxCol-1; i++){
            notNull = 0;
            for(j=0; j < iSimplexTable.size()-1; j++){
                if(abs(iSimplexTable[j][i]) > 0){
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
            for(i=0; i<iSimplexTable.size()-1; i++){
                for(j=maxCol-1; j<((maxCol-1)+(iSimplexTable.size()-1)); j++){
                    //если номер текущей позиции равен номеру в которой нужно добавить единицу,
                    //то добавляем единицу, иначе 0
                    if(currentNumOfOne == j) iSimplexTable[i].insert(j,1);
                    else iSimplexTable[i].insert(j,0);
                }
                currentNumOfOne++;
            }

            //в последнюю строку (целевую функцию) добавляем необходимое количество нулей
            for(j=maxCol-1; j<((maxCol-1)+(iSimplexTable.size()-1)); j++){
                iSimplexTable[iSimplexTable.size()-1].insert(j,0);
            }

            //не забываем определить "ширину" таблицы и модели
            maxCol = maxCol + iSimplexTable.size();
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
            for(i=0; i < iSimplexTable.size(); i++){
                for(j=maxCol-1; j<(maxCol-1+colBazis); j++){
                    iSimplexTable[i].insert(j,0);
                }
            }

            //поставим в нужных местах 1
            //стартовую позицию в которую поставим 1 установим в конец матрицы
            currentNumOfOne = maxCol-1;
            for(i=0; i < bazisSimplex.size(); i++){
                if (bazisSimplex[i] == -1) {
                    iSimplexTable[i][currentNumOfOne] = 1;
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
        simplexTabModel->setRowCount(iSimplexTable.size());
        simplexTabModel->setColumnCount(maxCol);

        ibazisAndGetModel();//вызываем метод поиска базисных переменных и отрисовки таблицы

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

        //подсветим разрешающий элемент
        modIndex = simplexTabModel->index(permRow,permCol);
        simplexTabModel->setData(modIndex,QVariant(QBrush(Qt::red)), Qt::BackgroundRole);

        //сохраняем исходные данные
        iSimplexTableConst = iSimplexTable;
        iColBConst = iColB;//столбец значений b
        iRConst = iR;//вектор коэффициентов при свободных неизвестных целевой функции
        bazisSimplexConst = bazisSimplex;//базисные переменные
        permColConst = permCol;
        permRowConst = permRow;
        iPermItemConst = iPermItem;

        inputFile.flush();
        inputFile.close();

    }
}
