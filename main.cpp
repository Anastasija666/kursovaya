//Подключение необходимых библиотек
#include <iostream>
#include <fstream>
#include <locale>
#include <cmath>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Rus");
    float Ninfo, Ninfo2, R, Refftx, rxSensitivity, N_sh_symb, Noh, v, Spectral_efficiency;
    int MCS, TBS, n, C, Nre, Nre2, Ndmrs, n_PRB, Qm, i, table;

    //Выбор таблицы кодирование:
    do{
      cout <<"Выберите желаемую таблицу кодирования:\n";
      cout <<"нажмите 1 - если хотите выбрать Table 5.1.3.1-1 MCS index table 1 for PDSCH\n";
      cout <<"нажмите 2 - если хотите выбрать Table 5.1.3.1-2 MCS index table 2 for PDSCH\n";
      cout <<"нажмите 3 - если хотите выбрать Table 5.1.3.1-3 MCS index table 3 for PDSCH\n";
      cin >>table;
    }while (table<1 || table>3);

    //Ввод дополнительных данных:
    do{
    cout <<"Введите максимальное количество слоев: ";
    cin >>v;
    }while (v<0);

    do{
    cout <<"Введите общее количество выделенных для пользовательского устройства физических ресурсных блоков: ";
    cin >>n_PRB;
    }while (n_PRB<0);

    do{
    cout <<"Введите количество запланированных OFDM символов: ";
    cin >>N_sh_symb;
    }while (N_sh_symb<0);

    do{
    cout <<"Введите количество ресурсных элементов, выделенных\nдля опорных сигналов демoдуляции на один физический ресурсный блок: ";
    cin >>Ndmrs;
    }while(Ndmrs<0);

    do{
    cout <<"Введите накладные расходы, сконфигурированные более высоким уровнем: ";
    cin >>Noh;
    }while(Noh<0);

    for (MCS=0;MCS<32;MCS++)
    {

    //Ввод индекса MCS и считывание с файла порядка модуляци и кодовой скорости:
    if (table==1)
    {
        ifstream file ("Table 5.1.3.1-1 MCS index table 1 for PDSCH.txt");
        for (i=0;i<=MCS;i++)
          file >>n>>Qm>>R>>Spectral_efficiency;
        file.close();
    }
    if (table==2)
    {
        ifstream file ("Table 5.1.3.1-2 MCS index table 2 for PDSCH.txt");
        for (i=0;i<=MCS;i++)
          file >>n>>Qm>>R>>Spectral_efficiency;
        file.close();
    }
    if (table==3)
    {
        ifstream file ("Table 5.1.3.1-3 MCS index table 3 for PDSCH.txt");
        for (i=0;i<=MCS;i++)
          file >>n>>Qm>>R>>Spectral_efficiency;
        file.close();
    }
    R=R/1024;

    //Вычисление Ninfo:
    Nre2=12*N_sh_symb-Ndmrs-Noh;
    if (Nre2>156) Nre=156*n_PRB;
     else Nre=Nre2*n_PRB;
    Ninfo=Nre*R*Qm*v;


    //Вычисление размера транспортных блоков:
    if (Ninfo<=3824)
    {
        n=floor(log2(Ninfo))-6;
        if (n<3) n=3;

        Ninfo2=pow(2, n)*floor(Ninfo/pow(2, n));
        if (Ninfo2<24) Ninfo2=24;
    }
    else
    {
        n=floor(log2(Ninfo-24))-5;
        Ninfo2=pow(2, n)*round((Ninfo-24)/pow(2, n));
    }

    if (R<=0.25)
    {
        C=ceil((Ninfo2+24)/3816);
        TBS=(8*C*ceil((Ninfo2+24)/(8*C)))-24;
    }
    else
        if (Ninfo2>=8424)
        {
            C=ceil((Ninfo2+24)/8424);
            TBS=(8*C*ceil((Ninfo2+24)/(8*C)))-24;
        }
        else TBS=(8*ceil((Ninfo2+24)/8))-24;

    if (Ninfo<=3824)
    {
        ifstream fl ("TBS_for_Ninfo_less_then_3824.txt");
        do   fl >>n;    while (TBS>n);
        TBS=n;
        fl.close();
    }

    //Вычисление эффективной частоты передачи:
    Refftx=TBS*0.9/1000;
    //Вычисление границы чувствительности приемника:
    rxSensitivity=10*log10(pow(2, Refftx/4)-1)-81.3;

    //Вывод полученных результатов в консоль:
    cout <<"MCS = "<<MCS<<endl;
    cout <<"Порядок модуляции: "<<Qm<<"\nСкорость кодирования [*1024]: "<<R*1024<<endl;
    if (R==0) cout <<"5G NR TBS (размер транспортного блока): reserved"<<endl<<endl;
    else
    {
        cout <<"TBS = "<<TBS<<endl;
        cout <<"Эффективная частота передачи = "<<Refftx<<endl;
        cout <<"Граница чувствительности приемника = "<<rxSensitivity<<endl<<endl;
    }

    //Вывод полученных данных в файл:
    if (MCS==0)
    {

        ofstream fl ("output.txt");
        fl <<"MCS = "<<MCS<<endl;
        if (R==0) fl <<"5G NR TBS (размер транспортного блока): reserved"<<endl<<endl;
        else
         {
            fl <<"Порядок модуляции: "<<Qm<<"\nСкорость кодирования [*1024]: "<<R*1024<<endl;
            fl <<"TBS = "<<TBS<<endl;
            fl <<"Эффективная частота передачи = "<<Refftx<<endl;
            fl <<"Граница чувствительности приемника = "<<rxSensitivity<<endl<<endl;
         }
            fl.close();

    ofstream flp ("rxSensitivity.xls");
    flp <<"Индекс MCS\tПорядок модуляции\tCкорость кодирования [*1024]\tГраница чувствительности приемника"<<endl;
    flp <<MCS<<"\t"<<Qm<<"\t"<<R*1024<<"\t"<<rxSensitivity<<endl;
    flp.close();
    ofstream flp1 ("outputQmp.txt");
    flp1 <<Qm<<endl;
    flp1.close();
    ofstream flp2 ("outputRp.txt");
    flp2 <<R*1024<<endl;
    flp2.close();
    ofstream flp3 ("outputrxp.txt");
    flp3 <<rxSensitivity<<endl;
    flp3.close();
    }
    else
    {
        ofstream fl ("output.txt", ios::app);
        fl <<"MCS = "<<MCS<<endl;
        if (R==0)   fl <<"5G NR TBS (размер транспортного блока): reserved"<<endl<<endl;
        else
            {
                fl <<"Порядок модуляции: "<<Qm<<"\nСкорость кодирования [*1024]: "<<R*1024<<endl;
                fl <<"TBS = "<<TBS<<endl;
                fl <<"Эффективная частота передачи = "<<Refftx<<endl;
                fl <<"Граница чувствительности приемника = "<<rxSensitivity<<endl<<endl;
            }
        fl.close();

        ofstream flp ("rxSensitivity.xls", ios::app);
        if (R==0)   flp <<MCS<<"\t"<<"5G NR TBS (размер транспортного блока): reserved"<<endl;
        else flp <<MCS<<"\t"<<Qm<<"\t"<<R*1024<<"\t"<<rxSensitivity<<endl;
        flp.close();
        if (R!=0)
        {
          ofstream flp1 ("outputQmp.txt", ios::app);
          flp1 <<Qm<<endl;
          flp1.close();
          ofstream flp2 ("outputRp.txt", ios::app);
          flp2 <<R*1024<<endl;
          flp2.close();
          ofstream flp3 ("outputrxp.txt", ios::app);
          flp3 <<rxSensitivity<<endl;
          flp3.close();
        }
    }}
    return 0;
}
