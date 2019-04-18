#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

class FA
{
private:
    int nr_stari,nr_simboluri,nr_stari_finale,stare_initiala,stari_finale[100],***tabel,stari[100];
    char simboluri[1000];
    static int indice_stare;
public:
    FA()
    {
        tabel=new int**[100];
        for(int i=0;i<100;i++)
        {
            tabel[i]=new int*[1000];
            for(int j=0;j<1000;j++)
                tabel[i][j]=new int[100];
        }
    }
    FA(char a)
    {
        tabel=new int**[100];
        for(int i=0;i<100;i++)
        {
            tabel[i]=new int*[1000];
            for(int j=0;j<1000;j++)
                tabel[i][j]=new int[100];
        }
        nr_stari=2;
        nr_simboluri=1;
        nr_stari_finale=1;
        simboluri[0]=a;
        stare_initiala=indice_stare++;
        stari_finale[0]=indice_stare++;
        tabel[stare_initiala][a][stari_finale[0]]=1;
        stari[0]=indice_stare-2;
        stari[1]=indice_stare-1;
    }
    FA& stelare();
    FA& operator=(FA& a);
    friend FA& concatenare(FA& a1, FA& a2);
    friend FA& operator+(FA& a1, FA& a2);
    friend ostream& operator<<(ostream& out, const FA& a3);
};
int FA::indice_stare=0;
FA& FA::operator=(FA& a)
{
    if(this!=&a)
    {
        nr_stari=a.nr_stari;
        nr_simboluri=a.nr_simboluri;
        nr_stari_finale=a.nr_stari_finale;
        stare_initiala=a.stare_initiala;
        for(int i=0;i<a.nr_simboluri;i++)
            simboluri[i]=a.simboluri[i];
        for(int i=0;i<a.nr_stari;i++)
            stari[i]=a.stari[i];
        for(int i=0;i<a.nr_stari_finale;i++)
            stari_finale[i]=a.stari_finale[i];
        for(int i=0;i<a.nr_stari;i++)
            {
                for(int j=0;j<a.nr_simboluri;j++)
                    for(int k=0;k<a.nr_stari;k++)
                        if(a.tabel[a.stari[i]][a.simboluri[j]][a.stari[k]]==1)
                            tabel[stari[i]][simboluri[j]][stari[k]]=1;
            for(int l=0;l<100;l++)
                if(a.tabel[a.stari[i]][999][l]==1)
                    tabel[stari[i]][999][l]=1;
            }
    }
    return (*this);
}
ostream& operator<<(ostream& out, const FA& a3)
{
    out<<"Automatul:"<<endl;
    out<<"Stari:";
    for(int i=0;i<a3.nr_stari;i++)
        out<<a3.stari[i]<<" ";
    out<<endl;
    out<<"Simboluri:";
    for(int i=0;i<a3.nr_simboluri;i++)
        out<<a3.simboluri[i]<<" ";
    out<<endl;
    out<<"Stare initiala:"<<a3.stare_initiala<<endl;
    out<<"Stari finale:";
    for(int i=0;i<a3.nr_stari_finale;i++)
        out<<a3.stari_finale[i]<<" ";
    out<<endl;
    out<<"Tranzitii:"<<endl;
    for(int i=0;i<a3.nr_stari;i++)
    {
        for(int j=0;j<a3.nr_simboluri;j++)
        {
            for(int k=0;k<a3.nr_stari;k++)
            {
                if(a3.tabel[a3.stari[i]][a3.simboluri[j]][a3.stari[k]]==1)
                    out<<a3.stari[i]<<" "<<a3.stari[k]<<" "<<a3.simboluri[j]<<endl;
            }
        }
        for(int l=0;l<100;l++)
            if(a3.tabel[a3.stari[i]][999][l]==1)
                out<<a3.stari[i]<<" "<<l<<" "<<"lambda"<<endl;
    }
    return out;
}
FA& concatenare(FA& a1, FA& a2)
{
    FA* a3=new FA;
    int p;
    (*a3).nr_stari=a1.nr_stari+a2.nr_stari;
    (*a3).stare_initiala=a1.stare_initiala;
    ///simboluri
    for(int i=0;i<a1.nr_simboluri;i++)
    {
        (*a3).simboluri[i]=a1.simboluri[i];
    }
    (*a3).nr_simboluri=a1.nr_simboluri;
    for(int i=0;i<a2.nr_simboluri;i++)
    {
        p=1;
        for(int j=0;j<a1.nr_simboluri;j++)
            if(a1.simboluri[j]==a2.simboluri[i])
                p=0;
        if(p==1)
        {
            (*a3).nr_simboluri++;
            (*a3).simboluri[(*a3).nr_simboluri-1]=a2.simboluri[i];
        }
    }
    ///tabel
    for(int i=0;i<100;i++)
        for(int j=0;j<1000;j++)
            for(int k=0;k<100;k++)
                if(a1.tabel[i][j][k]==1 || a2.tabel[i][j][k]==1)
                    (*a3).tabel[i][j][k]=1;
    for(int i=0;i<a1.nr_stari_finale;i++)
        (*a3).tabel[a1.stari_finale[i]][999][a2.stare_initiala]=1; ///999 e pt lambda
    ///stari finale
    (*a3).nr_stari_finale=a2.nr_stari_finale;
    for(int i=0;i<a2.nr_stari_finale;i++)
        (*a3).stari_finale[i]=a2.stari_finale[i];
    ///stari
    for(int i=0;i<a1.nr_stari;i++)
        (*a3).stari[i]=a1.stari[i];
    for(int i=0;i<a2.nr_stari;i++)
        (*a3).stari[a1.nr_stari+i]=a2.stari[i];
    return (*a3);
}

FA& operator+(FA& a1, FA& a2)
{
    FA* a3=new FA;
    int p;
    (*a3).nr_stari=a1.nr_stari+a2.nr_stari+1;
    (*a3).stare_initiala=(*a3).indice_stare++;
    ///simboluri
    for(int i=0;i<a1.nr_simboluri;i++)
    {
        (*a3).simboluri[i]=a1.simboluri[i];
    }
    (*a3).nr_simboluri=a1.nr_simboluri;
    for(int i=0;i<a2.nr_simboluri;i++)
    {
        p=1;
        for(int j=0;j<a1.nr_simboluri;j++)
            if(a1.simboluri[j]==a2.simboluri[i])
                p=0;
        if(p==1)
        {
            (*a3).nr_simboluri++;
            (*a3).simboluri[(*a3).nr_simboluri-1]=a2.simboluri[i];
        }
    }
    ///stari finale
    (*a3).nr_stari_finale=a1.nr_stari_finale+a2.nr_stari_finale;
    for(int i=0;i<a1.nr_stari_finale;i++)
        (*a3).stari_finale[i]=a1.stari_finale[i];
    for(int i=0;i<a2.nr_stari_finale;i++)
        (*a3).stari_finale[a1.nr_stari_finale+i]=a2.stari_finale[i];
    ///tabel
    for(int i=0;i<100;i++)
        for(int j=0;j<1000;j++)
            for(int k=0;k<100;k++)
                if(a1.tabel[i][j][k]==1 || a2.tabel[i][j][k]==1)
                    (*a3).tabel[i][j][k]=1;
    (*a3).tabel[(*a3).stare_initiala][999][a1.stare_initiala]=1;   ///999 e pt lambda
    (*a3).tabel[(*a3).stare_initiala][999][a2.stare_initiala]=1;
    ///stari
    for(int i=0;i<a1.nr_stari;i++)
        (*a3).stari[i]=a1.stari[i];
    for(int i=0;i<a2.nr_stari;i++)
        (*a3).stari[a1.nr_stari+i]=a2.stari[i];
    (*a3).stari[(*a3).nr_stari-1]=(*a3).stare_initiala;
    //cout<<"a3::::::::::::::::::"<<a3<<endl;
    //a1=a3;
    //cout<<"a1::::::::::::::::::"<<a1<<endl;
    return (*a3);
}

FA& FA:: stelare()
{
    for(int i=0;i<nr_stari_finale;i++)
        tabel[stari_finale[i]][999][indice_stare]=1;
    tabel[indice_stare][999][stare_initiala]=1;
    stare_initiala=indice_stare;
    indice_stare++;
    nr_stari_finale=1;
    stari_finale[0]=stare_initiala;
    nr_stari++;
    stari[nr_stari-1]=stare_initiala;
    return (*this);
}


int main()
{
    ifstream in("fisier.in");
    ofstream out("fisier.out");
    char expresie[1000],*p,*q,*r,*tool;
    in.get(expresie,999);
    in.get();
    FA *automate;
    automate=new FA[30];
    int nr_automate=0;
    int i,nr_ordine;
    for(i=0;i<strlen(expresie);i++)
        if(expresie[i]!='*' && expresie[i]!='(' && expresie[i]!=')' && expresie[i]!='|')
        {
            FA aux(expresie[i]);
            automate[nr_automate]=aux;
            nr_automate++;
        }
    while(strlen(expresie)>1)
    {
        while(strchr(expresie,')'))
        {
            p=strchr(expresie,')');
            q=p;
            //cout<<"p= "<<p<<endl<<"q= "<<q<<endl;
            while(q[0]!='(')
                    q--;
            strcpy(expresie+(q-expresie),expresie+(q-expresie)+1);  ///elimin (
            p--;
            strcpy(expresie+(p-expresie),expresie+(p-expresie)+1);  ///elimin )
            p--;
            //cout<<"1 "<<expresie<<endl;
            //cout<<"p= "<<p<<endl<<"q= "<<q<<endl;
            while(strchr(q,'*')!=NULL && p-strchr(q,'*')>=0)
            {
                r=strchr(expresie,'*');    ///gasesc *
                r--;
                tool=expresie;
                nr_ordine=-1;
                while(r-tool>=0)
                {
                    if(tool[0]!='|' && tool[0]!='(' && tool[0]!=')' && tool[0]!='*')
                        nr_ordine++;
                    tool++;
                }
                automate[nr_ordine]=automate[nr_ordine].stelare();
                strcpy(r+1,r+2);  ///elimin *
                p--;
            }
            r=q;
            //cout<<"2 "<<expresie<<endl;
            //cout<<"p= "<<p<<endl<<"q= "<<q<<endl;
            while(r!=p)
            {
                if(r[0]!='|' && r[1]!='|')  ///caut concatenari
                {
                    tool=expresie;
                    nr_ordine=-1;
                    while(r-tool>=0)
                    {
                        if(tool[0]!='|' && tool[0]!='(' && tool[0]!=')' && tool[0]!='*')
                            nr_ordine++;
                        tool++;
                    }
                    automate[nr_ordine]=concatenare(automate[nr_ordine],automate[nr_ordine+1]);
                    for(i=nr_ordine+1;i<nr_automate-1;i++)
                        automate[i]=automate[i+1];
                    nr_automate--;
                    strcpy(r+1,r+2);     ///contopesc concatenarile
                    p--;
                }
                else
                    r++;
            }
            //cout<<"3 "<<expresie<<endl;
            //cout<<"p= "<<p<<endl<<"q= "<<q<<endl;
            while(p!=q)
            {
                tool=expresie;
                nr_ordine=-1;
                while(q-tool>=0)
                {
                    if(tool[0]!='|' && tool[0]!='(' && tool[0]!=')' && tool[0]!='*')
                        nr_ordine++;
                    tool++;
                }
                automate[nr_ordine]=automate[nr_ordine]+automate[nr_ordine+1];
                //cout<<automate[nr_ordine];
                for(i=nr_ordine+1;i<nr_automate-1;i++)
                    automate[i]=automate[i+1];
                nr_automate--;
                strcpy(q,q+2);     ///contopesc reuniunile
                p=p-2;
            }
        }
        //cout<<"4 "<<expresie<<endl;
        q=expresie;
        p=expresie+strlen(expresie)-1;
        while(strchr(q,'*')!=NULL && p-strchr(q,'*')>=0)
            {
                r=strchr(expresie,'*');    ///gasesc *
                r--;
                tool=expresie;
                nr_ordine=-1;
                while(r-tool>=0)
                {
                    if(tool[0]!='|' && tool[0]!='(' && tool[0]!=')' && tool[0]!='*')
                        nr_ordine++;
                    tool++;
                }
                automate[nr_ordine]=automate[nr_ordine].stelare();
                strcpy(r+1,r+2);  ///elimin *
                p--;
            }
            r=q;
            //cout<<"5 "<<expresie<<endl;
            while(r!=p)
            {
                if(r[0]!='|' && r[1]!='|')  ///caut concatenari
                {
                    tool=expresie;
                    nr_ordine=-1;
                    while(r-tool>=0)
                    {
                        if(tool[0]!='|' && tool[0]!='(' && tool[0]!=')' && tool[0]!='*')
                            nr_ordine++;
                        tool++;
                    }
                    automate[nr_ordine]=concatenare(automate[nr_ordine],automate[nr_ordine+1]);
                    for(i=nr_ordine+1;i<nr_automate-1;i++)
                        automate[i]=automate[i+1];
                    nr_automate--;
                    strcpy(r+1,r+2);     ///contopesc concatenarile
                    p--;
                }
                else
                    r++;
            }
            //cout<<"6 "<<expresie<<endl;
            while(p!=q)
            {
                tool=expresie;
                nr_ordine=-1;
                while(q-tool>=0)
                {
                    if(tool[0]!='|' && tool[0]!='(' && tool[0]!=')' && tool[0]!='*')
                        nr_ordine++;
                    tool++;
                }
                automate[nr_ordine]=automate[nr_ordine]+automate[nr_ordine+1];
                for(i=nr_ordine+1;i<nr_automate-1;i++)
                    automate[i]=automate[i+1];
                nr_automate--;
                strcpy(q,q+2);     ///contopesc reuniunile
                p-=2;
            }
            //cout<<"7 "<<expresie<<endl;
    }
    out<<automate[0];
    /*FA a1('a'),a2('b');
    //FA a1('a'),a2('b');
    a1=a1+a2;
    a1=a1.stelare();
    FA a3('a');
    a1=concatenare(a1,a3);
    FA a4('b');
    a1=concatenare(a1,a4);
    FA a5('b');
    a1=concatenare(a1,a5);
    cout<<a1;
    //cout<<a1;*/
    /*a6=a6.stelare();
    a6=concatenare(a6,a3);
    a6=concatenare(a6,a4);
    a6=concatenare(a6,a5);
    cout<<a6;*/
    //cout<<automat;
    return 0;
}
