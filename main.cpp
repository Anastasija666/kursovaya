//����������� ����������� ���������
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

    //����� ������� �����������:
    do{
      cout <<"�������� �������� ������� �����������:\n";
      cout <<"������� 1 - ���� ������ ������� Table 5.1.3.1-1 MCS index table 1 for PDSCH\n";
      cout <<"������� 2 - ���� ������ ������� Table 5.1.3.1-2 MCS index table 2 for PDSCH\n";
      cout <<"������� 3 - ���� ������ ������� Table 5.1.3.1-3 MCS index table 3 for PDSCH\n";
      cin >>table;
    }while (table<1 || table>3);

    //���� �������������� ������:
    do{
    cout <<"������� ������������ ���������� �����: ";
    cin >>v;
    }while (v<0);

    do{
    cout <<"������� ����� ���������� ���������� ��� ����������������� ���������� ���������� ��������� ������: ";
    cin >>n_PRB;
    }while (n_PRB<0);

    do{
    cout <<"������� ���������� ��������������� OFDM ��������: ";
    cin >>N_sh_symb;
    }while (N_sh_symb<0);

    do{
    cout <<"������� ���������� ��������� ���������, ����������\n��� ������� �������� ���o������� �� ���� ���������� ��������� ����: ";
    cin >>Ndmrs;
    }while(Ndmrs<0);

    do{
    cout <<"������� ��������� �������, ������������������ ����� ������� �������: ";
    cin >>Noh;
    }while(Noh<0);

    for (MCS=0;MCS<32;MCS++)
    {

    //���� ������� MCS � ���������� � ����� ������� �������� � ������� ��������:
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

    //���������� Ninfo:
    Nre2=12*N_sh_symb-Ndmrs-Noh;
    if (Nre2>156) Nre=156*n_PRB;
     else Nre=Nre2*n_PRB;
    Ninfo=Nre*R*Qm*v;


    //���������� ������� ������������ ������:
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

    //���������� ����������� ������� ��������:
    Refftx=TBS*0.9/1000;
    //���������� ������� ���������������� ���������:
    rxSensitivity=10*log10(pow(2, Refftx/4)-1)-81.3;

    //����� ���������� ����������� � �������:
    cout <<"MCS = "<<MCS<<endl;
    cout <<"������� ���������: "<<Qm<<"\n�������� ����������� [*1024]: "<<R*1024<<endl;
    if (R==0) cout <<"5G NR TBS (������ ������������� �����): reserved"<<endl<<endl;
    else
    {
        cout <<"TBS = "<<TBS<<endl;
        cout <<"����������� ������� �������� = "<<Refftx<<endl;
        cout <<"������� ���������������� ��������� = "<<rxSensitivity<<endl<<endl;
    }

    //����� ���������� ������ � ����:
    if (MCS==0)
    {

        ofstream fl ("output.txt");
        fl <<"MCS = "<<MCS<<endl;
        if (R==0) fl <<"5G NR TBS (������ ������������� �����): reserved"<<endl<<endl;
        else
         {
            fl <<"������� ���������: "<<Qm<<"\n�������� ����������� [*1024]: "<<R*1024<<endl;
            fl <<"TBS = "<<TBS<<endl;
            fl <<"����������� ������� �������� = "<<Refftx<<endl;
            fl <<"������� ���������������� ��������� = "<<rxSensitivity<<endl<<endl;
         }
            fl.close();

    ofstream flp ("rxSensitivity.xls");
    flp <<"������ MCS\t������� ���������\tC������� ����������� [*1024]\t������� ���������������� ���������"<<endl;
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
        if (R==0)   fl <<"5G NR TBS (������ ������������� �����): reserved"<<endl<<endl;
        else
            {
                fl <<"������� ���������: "<<Qm<<"\n�������� ����������� [*1024]: "<<R*1024<<endl;
                fl <<"TBS = "<<TBS<<endl;
                fl <<"����������� ������� �������� = "<<Refftx<<endl;
                fl <<"������� ���������������� ��������� = "<<rxSensitivity<<endl<<endl;
            }
        fl.close();

        ofstream flp ("rxSensitivity.xls", ios::app);
        if (R==0)   flp <<MCS<<"\t"<<"5G NR TBS (������ ������������� �����): reserved"<<endl;
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
