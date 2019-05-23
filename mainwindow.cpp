#include "mainwindow.h"
#include "ui_mainwindow.h"

//подключаю файл с мтодом поиска базисных переменных
//и отрисовки модели в таблВью
#include "bazisAndGetModel.cpp"

//подключаю слот для пошагового выполнения
#include "stepSimplex.cpp"

//подключаю слот для загрузки таблицы из файла
#include "loadTabFromFileBut.cpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //создаём новый объект для модели симплекс таблицы
    simplexTabModel = new QStandardItemModel(0,0,this);

    //отображаем модель в tableView
    ui->tableView->setModel(simplexTabModel);

    //убираем заголовки в таблице
    //ui->tableView->verticalHeader()->hide();
    //ui->tableView->horizontalHeader()->hide();

    //Выделяем цветом заголовки
    QPalette palette = ui->tableView->horizontalHeader()->palette();
    palette.setColor( QPalette::Normal, QPalette::Window, Qt::red );
    ui->tableView->horizontalHeader()->setPalette( palette );

    palette = ui->tableView->verticalHeader()->palette();
    palette.setColor( QPalette::Normal, QPalette::Window, Qt::red );
    ui->tableView->verticalHeader()->setPalette( palette );

    //впишем таблицу полностью в таблвью
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    //устанавливаем счётчик количества симплекс преобразований
    globalSteps=0;
    d = true;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_roundFloat_clicked()
{
    //for(k=0; k<simplexCounts;k++){
        //qDebug()<<k;
        ///Восстанавливаем исходные данные
        simplexTable = simplexTableConst;
        colB = colBConst;
        r = rConst;
        bazisSimplex = bazisSimplexConst;
        permCol = permColConst;
        permRow = permRowConst;
        permItem = permItemConst;



        nanoSec = 0;
        //этот цикл пока не дойдёт до оптимального решения
        while(d==true){
            //стартуем счётчик времени
            mainTimer.start();

            //метод для симплес-преобразований
            simplexTransformationNoOutput();

            //вызываем метод поиска базисных переменных и отрисовки таблицы
            onlyBazisNoOutput();

            //выполняем метод проверки на оптимальность
            checkOfOptimalNoOutput();

            if(optimal == true){
                /*ui->status->append("Найдено оптимальное решение!");
                msgBox.setText("Найдено оптимальное решение!");
                msgBox.exec();*/
                break;
            }

            //выполняем метод выбора разрешающего столбца
            permittedCol();

            //выполняем метод выбора разрешающей строки
            permittedRowNoOutput();
            //установим разрешающий элемент
            permItem = simplexTable[permRow][permCol];

            //увеличиваем счётчик симплекс преобразований
            globalSteps++;

            nanoSec = nanoSec + mainTimer.nsecsElapsed();
        }
        measuresNsecs.append(nanoSec);


   // }

    qDebug()<<measuresNsecs.size()<<":"<<nanoSec;
/*
    //ЗАписываем результаты в файл
    QFile outputFile("outputFloat.csv");
    outputFile.open(QFile::WriteOnly | QFile::Text);

    //обработка ошибок открытия файла
    if(!outputFile.isOpen()){
        //если есть ошибки, то выводим в консоль
        ui->status->append("Ошибка открытия файла!");
    }
    else{
        //если файл открылся, то начинаем его читать построчно
        //открываем поток для чтения из файла

        ui->status->append("Выходной файл FLOAT открыт для  записи!");

        QTextStream outStream (&outputFile);

        //установливаем кодировку...
        outStream.setCodec("UTF-8");

        //переписываем массив в файл
        foreach(nanoSec, measuresNsecs){
            outStream<<nanoSec<<",";
        }

        outputFile.flush();
        outputFile.close();
        ui->status->append("Запись окончена!");

    }

*/

}





void MainWindow::on_roundINT_clicked()
{
    //for(k=0; k<simplexCounts;k++){
        //qDebug()<<k;
        ///Восстанавливаем исходные данные
        iSimplexTable = iSimplexTableConst;
        iColB = iColBConst;
        iR = iRConst;
        bazisSimplex = bazisSimplexConst;
        permCol = permColConst;
        permRow = permRowConst;
        iPermItem = iPermItemConst;



        nanoSec = 0;
        //этот цикл пока не дойдёт до оптимального решения
        while(d==true){
            //стартуем счётчик времени
            mainTimer.start();

            //метод для симплес-преобразований
            isimplexTransformationNoOutput();

            //вызываем метод поиска базисных переменных и отрисовки таблицы
            ionlyBazisNoOutput();

            //выполняем метод проверки на оптимальность
            icheckOfOptimalNoOutput();

            if(optimal == true){
                /*ui->status->append("Найдено оптимальное решение!");
                msgBox.setText("Найдено оптимальное решение!");
                msgBox.exec();*/
                break;
            }

            //выполняем метод выбора разрешающего столбца
            ipermittedCol();

            //выполняем метод выбора разрешающей строки
            ipermittedRowNoOutput();
            //установим разрешающий элемент
            iPermItem = iSimplexTable[permRow][permCol];

            //увеличиваем счётчик симплекс преобразований
            globalSteps++;

            nanoSec = nanoSec + mainTimer.nsecsElapsed();
        }
        measuresNsecs.append(nanoSec);


    //}
qDebug()<<measuresNsecs.size()<<":"<<nanoSec;

/*
*/


}

void MainWindow::on_toFile_clicked()
{
    //ЗАписываем результаты в файл
    QFile outputFileInt("output.csv");
    outputFileInt.open(QFile::WriteOnly | QFile::Text);

    //обработка ошибок открытия файла
    if(!outputFileInt.isOpen()){
        //если есть ошибки, то выводим в консоль
        ui->status->append("Ошибка открытия файла для записи");
    }
    else{
        //если файл открылся, то начинаем его читать построчно
        //открываем поток для чтения из файла

        ui->status->append("Выходной файл открыт для  записи!");

        QTextStream outStreamInt (&outputFileInt);

        //установливаем кодировку...
        outStreamInt.setCodec("UTF-8");

        //переписываем массив в файл
        foreach(nanoSec, measuresNsecs){
            outStreamInt<<nanoSec<<",";
        }

        outputFileInt.flush();
        outputFileInt.close();
        ui->status->append("Запись окончена!");

    }
}
