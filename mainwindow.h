#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <math.h>
#include <QElapsedTimer>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
   const int simplexCounts = 100;//количество циклов измерений
   QVector <qint64> measuresNsecs;//массив измерений

    //определяем переменные
    QStandardItemModel *simplexTabModel;//модель для хранения текущей симплекс таблицы
    //сущность для хранения симплекс-таблицы
    //(её будем отражать в модель, а модель в таблвью)

    //симплекс таблица с плавающей точкой
    QVector <QVector <float>> simplexTable;
    QVector <QVector <float>> simplexTableRepl;//реплика симплекс таблицы для расчётов
    QVector <float> colB;//столбец значений b
    QVector <float> r;//вектор коэффициентов при свободных неизвестных целевой функции
    QVector <int> bazisSimplex;//базисные переменные

    //симплекс таблица с целочисленными коэффициентами
    QVector <QVector <int>> iSimplexTable;
    QVector <QVector <int>> iSimplexTableRepl;//реплика симплекс таблицы для расчётов
    QVector <int> iColB;//столбец значений b
    QVector <int> iR;//вектор коэффициентов при свободных неизвестных целевой функции
    int iPermItem;

    //здесь храним исходные данные после загрузки симплекс-таблицы
    //симплекс таблица c целочисленными коэффициентами
    QVector <QVector <int>> iSimplexTableConst;
    QVector <int> iColBConst;//столбец значений b
    QVector <int> iRConst;//вектор коэффициентов при свободных неизвестных целевой функции
    int iPermItemConst;//разрешающий элемент
    int iminNum, iMin;

    //здесь храним исходные данные после загрузки симплекс-таблицы
    //симплекс таблица с плавающей точкой
    QVector <QVector <float>> simplexTableConst;
    QVector <float> colBConst;//столбец значений b
    QVector <float> rConst;//вектор коэффициентов при свободных неизвестных целевой функции
    QVector <int> bazisSimplexConst;//базисные переменные
    int permColConst;//разрешающий столбец
    int permRowConst;//разрешающая строка
    float permItemConst;//разрешающий элемент

    QMessageBox msgBox; //объект окошка с ошибками
    int i,j,globalSteps,k, //счётчики для циклов for
        maxCol,//номер последнего столбца в таблице с учётом введённых базисных переменных и столбца b
        maxColStart,//количество столбцов в исходной симплекс таблице
        notNull,//количество не нулевых значений в столбце при поиске базиса
        numNull,//номер не нулевого значения в столбце при поиске базиса
        colBazis,//количество базисных переменных
        permCol,//разрешающий столбец
        permRow,//разрешающая строка
        maxNum,//номер максимального по модулю элемента в целевой функции
        minNum,//номер минимального элемента в векторе b
        currentNumOfOne,//номер элемента в который добавляем единицу при добавлении базисных переменных
        row,//счётчик для строк в модели
        col;//счётчик для столбцов в модели
    QModelIndex modIndex;//индекс для модели
    float fMax,// просто максимальное значение
            permItem,//разрешающий элемент
            fMin;// просто минимальное значение
    bool optimal; // признак оптимальности плана

    QElapsedTimer mainTimer;//главный таймер для замера времени выполнения
    qint64 nanoSec;


    //ЗДЕСЬ ВРЕМЕННЫЕ ПЕРЕМЕННЫЕ
    QString tmpString;//временная строка

    QStringList tmpStringList; //временный список строк
    QVector <float> tmpVector;//временный вектор
    QVector <float> tmpVector1;//ещё временный вектор
    float tmpFloat;//временная с плавающей точкой

    QVector <int> iTmpVector;//временный вектор
    QVector <int> iTmpVector1;//ещё временный вектор
    int iTmp;//временная с плавающей точкой

    bool d;


    //это метод для определения базисных переменных и
    //заполнения модели для отрисовки таблицы
    void bazisAndGetModel();
    //тоже самое только без отрисовки таблицы и вывода в консоль
    void onlyBazisNoOutput();
    ///ЦЕЛОЧИСЛЕННЫЕ
    //это метод для определения базисных переменных и
    //заполнения модели для отрисовки таблицы
    void ibazisAndGetModel();
    //тоже самое только без отрисовки таблицы и вывода в консоль
    void ionlyBazisNoOutput();

    //метод для проверки на оптимальность
    void checkOfOptimal();
    //тоже самое без ввода-вывода
    void checkOfOptimalNoOutput();
    ///ЦЕЛОЧИСЛЕННЫЕ
    //метод для проверки на оптимальность
    void icheckOfOptimal();
    //тоже самое без ввода-вывода
    void icheckOfOptimalNoOutput();


    //метод выбора разрешающего столбца
    void permittedCol();
    ///ЦЕЛОЧИСЛЕННЫЕ
    //метод выбора разрешающего столбца
    void ipermittedCol();

    //метод выбора разрешающей строки
    void permittedRow();
    //тоже самое без ввода-вывода
    void permittedRowNoOutput();
    ///ЦЕЛОЧИСЛЕННЫЕ
    //метод выбора разрешающей строки
    void ipermittedRow();
    //тоже самое без ввода-вывода
    void ipermittedRowNoOutput();

    //метод симплекс-преобразований
    void simplexTransformation();
    //тоже самое без ввода-вывода
    void simplexTransformationNoOutput();
    ///ЦЕЛОЧИСЛЕННЫЕ
    //метод симплекс-преобразований
    void isimplexTransformation();
    //тоже самое без ввода-вывода
    void isimplexTransformationNoOutput();
    //метод поиска базиса без отрисовки в таблицу


private slots:
    void on_loadTabFromFileBut_clicked();

    void on_stepSimplexBut_clicked();

    void on_roundFloat_clicked();

    void on_loadTabFromFileButInt_clicked();

    void on_stepSimplexButINT_clicked();

    void on_roundINT_clicked();

    void on_toFile_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
