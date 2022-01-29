#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <graphics.h>


void BSpline(const struct KordinatTutma Nokta1,const struct KordinatTutma Nokta2);
void KordinatSistemiCiz();
void NoktaCiz(struct KordinatTutma *kordinatlar,int n);
void CemberCiz(struct Cember *C);

struct KordinatTutma{
    float x;
    float y;
};
struct Cember{
    struct KordinatTutma C;
    float R;
};

float mesafe(const struct KordinatTutma& Nokta1,const struct KordinatTutma& Nokta2)
{
    return sqrt(pow(Nokta1.x - Nokta2.x, 2) + pow(Nokta1.y - Nokta2.y, 2));
}

bool icinde_mi(const struct Cember MerkezNokta, const struct KordinatTutma Noktalar)
{
    return mesafe(MerkezNokta.C, Noktalar) <= MerkezNokta.R;
}

struct KordinatTutma daire_merkezi(float bx, float by,float cx, float cy)
{
    struct KordinatTutma Q;
    float B = (bx * bx) + (by * by);
    float C = (cx * cx) + (cy * cy);
    float D = (bx * cy) - (by * cx);
    Q.x=(cy * B - by * C) / (2 * D);
    Q.y=(bx * C - cx * B) / (2 * D);
    return Q;
}


struct Cember gelen_daire2(const struct KordinatTutma &Nokta1,const struct KordinatTutma &Nokta2,const struct KordinatTutma &Nokta3){
    struct Cember  I;
    I.C=daire_merkezi(Nokta2.x-Nokta1.x,Nokta2.y-Nokta1.y,Nokta3.x-Nokta1.x,Nokta3.y-Nokta1.y);
    I.C.x += Nokta1.x;
    I.C.y += Nokta1.y;
    I.R = mesafe(I.C,Nokta1);
    return I;
};

struct Cember  gelen_daire(const struct KordinatTutma Nokta1,const struct KordinatTutma Nokta2)
{
    struct Cember  Merkez;
    Merkez.C.x =  (Nokta1.x + Nokta2.x) / 2.0;
    Merkez.C.y = (Nokta1.y + Nokta2.y) / 2.0 ;
    Merkez.R=mesafe(Nokta1,Nokta2)/ 2.0;
    return Merkez;
}

bool gecerli_mi(const struct Cember& c,const struct KordinatTutma &kordinatlar)
{
    if (!icinde_mi(c, kordinatlar))
            return false;
    return true;
}

struct Cember minimum_cevreleyen_cember(struct KordinatTutma *kordinatlar,int n){
    struct Cember mec={{0 , 0}, 0};
    struct Cember gecici;


    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){

            gecici = gelen_daire(kordinatlar[i],kordinatlar[j]);
            if(gecici.R<mec.R && gecerli_mi(gecici,*kordinatlar))
                mec=gecici;
        }
    }

    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            for(int k=j+1 ; k<n-1 ;k ++){

                gecici = gelen_daire2(kordinatlar[i],kordinatlar[j],kordinatlar[k]);

                if(gecici.R<mec.R && gecerli_mi(gecici,*kordinatlar))
                    mec=gecici;
                }
            }
        }

    mec.C.x=gecici.C.x;
    mec.C.y=gecici.C.y;
    mec.R=gecici.R;

    return mec;
};

int main()
{
    FILE *dosya = fopen("kordinatlar.txt", "r");
    if(dosya == NULL){
        printf("Dosya Acilamadi.");
        exit(1);
    }

    struct KordinatTutma kordinatlar[100];
    char *str;
    char arr[100];
    int i=0;
    int satir_sayisi = 0;

    while ( fgets(arr, 100, dosya) != NULL ) {
        satir_sayisi++;
        str = strtok(arr,",");
        sscanf(str,"%f", &kordinatlar[i].x);
        str = strtok(NULL,"");
        sscanf(str,"%f", &kordinatlar[i].y);
        i++;
    }
    struct Cember mec = minimum_cevreleyen_cember(kordinatlar,satir_sayisi);
    printf("\nMerkez = { %.1f, %.1f } Yaricap = %f \n",mec.C.x,mec.C.y,mec.R);

    fclose(dosya);

    initwindow(700, 700);
    KordinatSistemiCiz();
    NoktaCiz(kordinatlar,satir_sayisi);
    CemberCiz(&mec);
    for(int q=0;q<satir_sayisi-1;q++){
        BSpline(kordinatlar[q],kordinatlar[q+1]);
    }

    closegraph();
    return 0;
}
void KordinatSistemiCiz(){
    line(0,300,600,300);
    line(300,0,300,600);
    outtextxy(300,300,(char*)"0");
    int fx,fy,kx,ky;
    fy=300;
    ky=300;
    fx=300;
    kx=300;
    for(int i =0;i<=9;i++){

        char msg[128];
        char msg2[128];
        char msg3[128];
        char msg4[128];
        kx-=30;
        fx+=30;
        fy+=30;
        ky-=30;
        sprintf(msg4,"%d",(i+1));
        sprintf(msg3,"%d",- (i+1));
        sprintf(msg2,"%d",i+1);
        sprintf(msg,"%d",- (i+1));
        outtextxy(305,fy, msg);
        outtextxy(305,ky,msg2);
        outtextxy(kx,305,msg3);
        outtextxy(fx,305,msg4);

    }

    getch();
}
void NoktaCiz(struct KordinatTutma *kordinatlar,int n){
    for(int i=0;i<n;i++){
        float ekle = 30*(kordinatlar[i].x);
        float ekle2= 30*(kordinatlar[i].y);

        int x=300+ekle;
        int y=300-ekle2;

        fillellipse(x, y, 5, 5);
        getch();
    }
}
void CemberCiz(struct Cember *q){
    float ekle=30*(q->C.x);
    float ekle2=30*(q->C.y);
    float r=30*(q->R);
    int x=300+ekle;
    int y=300-ekle2;
    NoktaCiz(&q->C,1);
    circle(x,y,r);
    getch();
}

void BSpline(const struct KordinatTutma Nokta1,const struct KordinatTutma Nokta2)
{
    int x[4];
    int y[4];
    x[0]=300+(30*Nokta2.x);
    y[0]=300-(30*Nokta2.y);
    x[1]=270+(30*Nokta1.x);
    x[2]=360+(30*Nokta2.x);
    y[1]=270-(30*Nokta1.y);
    y[2]=360-(30*Nokta2.y);
    x[3]=300+(30*Nokta1.x);
    y[3]=300-(30*Nokta1.y);

    double xu = 0.0 , yu = 0.0 , u = 0.0 ;
    for(u = 0.0 ; u <= 1.0 ; u += 0.0001)
    {
        xu = pow(1-u,3)*x[0]+3*u*pow(1-u,2)*x[1]+3*pow(u,2)*(1-u)*x[2]
             +pow(u,3)*x[3];
        yu = pow(1-u,3)*y[0]+3*u*pow(1-u,2)*y[1]+3*pow(u,2)*(1-u)*y[2]
            +pow(u,3)*y[3];

        putpixel( xu , yu,13) ;
    }

    getch();
}
