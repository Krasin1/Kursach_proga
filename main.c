#include <stdio.h>
#include <math.h>
#include <float.h>
#define N 1000

void zastavka () {
    FILE *f = fopen("zast.txt","r");
    char ch;
    while (!feof(f)) {
        fscanf(f,"%c",&ch);
        printf("%c",ch);
    }
    fclose(f);
}

float parametr (int n, float * t, float * Uvx, float * Uvix, int count, float dt) {
    float max_Uvix = FLT_MIN, min_Uvix = FLT_MAX, max_Uvx = FLT_MIN;
    float dlit = 0;
    float vrem;					
    for (int i = 0; i < count; i++) {  //ищем нужные экстремумы функций
        if (max_Uvix < Uvix[i]) {
            max_Uvix = Uvix[i];
        }
        if (min_Uvix > Uvix[i]) {
            min_Uvix = Uvix[i];
        }
        if (max_Uvx < Uvx[i]) {
            max_Uvx = Uvx[i];
        }
    }
    switch(n){
        case 1:{
            float Uimp;
            Uimp = min_Uvix + 0.5 * (max_Uvix - min_Uvix);
            for (int i = 0; i < count; i++) {
                if (Uvix[i] >= Uimp) {
                    dlit += dt;
                }
            }
            return dlit;
        }

        case 2:{
            float u1, u2;
            u1 = min_Uvix + 0.1 * (max_Uvix - min_Uvix);
            u2 = min_Uvix + 0.9 * (max_Uvix - min_Uvix);
            for (int i = 0; i < count; i++) {
                if (Uvix[i] > u1 && Uvix[i] < u2 && Uvix[i + 1] > Uvix[i]) {
                    dlit += dt;
                }
            }
            return dlit;
        }

        case 3:{
            int k = 0;
            for (int i = 0; i < count; i++) {
                if (Uvx[i] > 80) {
                    k = i;
                    break;
                }
            }
            vrem = t[k];
            return vrem;
        }

        case 4:{
            for (int i = 0; i < count; i++){
                if (Uvx[i] == max_Uvx) {
                    vrem = t[i];
                    break;
                }
            }
            return vrem;
        }
    }

}

void pog(int n, float * t, float * Uvx, float * Uvix, float count, float dt, float eps) {
    float p = 1;
    float par = FLT_MAX, par1;
    while (p > eps) {
        par1 = parametr(n, t, Uvx, Uvix, count, dt);
        p = fabs(par - par1) / par1;
        printf("№ = %d  Параметр = %f  Погрешность = %f\n", n, par1, p);
        par = par1;
        n = 2 * n;
    }
}

int main() {
    float t[N], Uvx[N], Uvix[N];
    int n, choose;
    zastavka();
    while (1) {
        puts("\n      Меню");           //меню
        puts("1 - Контрольный расчет для n точек");
        puts("2 - Расчет параметра с заданной точностью");
        puts("3 - Запись данных в файл");
        puts("4 - Завершить работу");
        if(1 != scanf("%d", &choose) ) {
            puts("Error!");
            return 0;
        }
        switch(choose) {
            case 1:{
                puts("Введтие кол-во точек для контрольного расчета");
                if (1 != scanf("%d", &n) ) {
                    puts("Error!");
                    return 0;
                }
                if (n < 2 || n > N) {
                    puts("\nЧё-то тут не так");
                    return 0;
                }

                float tn = 1, tk = 10, dt;
                dt = (tk - tn) / (n - 1);
                for (int i = 0; i < n; i++) {       //массив t
                    t[i] = tn + i * dt;
                }

                float a0 = 2, a1 = 5, a2 = 0.3;
                for (int i = 0; i < n; i++) {       //массив Uvx
                    Uvx[i] = a0 + a1 * t[i] + a2 * t[i] * t[i];   
                }

                float Uvx1 = 10, a = 5, b = 0.05;
                for (int i = 0; i < n; i++) {       //массив Uvix
                    if (Uvx[i] <= Uvx1) { Uvix[i] = a; }
                    else { Uvix[i] = b * Uvx[i] * Uvx[i]; }
                }

                puts("    №       t      Uvx      Uvix  ");
                for (int i = 0; i < n; i++) {       //печать массивов
                    printf("  %3d   %6.3f   %6.3f   %6.3f\n",i,t[i],Uvx[i],Uvix[i]);
                }
                break;
            }

            case 2:{
                float tn = 1, tk = 10, dt, eps;
                dt = (tk - tn) / (n - 1);
                puts("\nЗадайте погрешность(0 <= x <= 1):");
                if (1 != scanf("%f", &eps) || eps < 0 || eps > 1){
                    puts("Error!");
                    return 0;
                }
                puts("\nВыбор параметра для контрольных точек");      //parametr
                puts("1 - Нахождение длительности импульса  сигнала");
                puts("2 - Нахождение длительности переднего фронта импульса сигнала");
                puts("3 - Нахождение момента времени, при котором Uvx достигает 80 В");
                puts("4 - Нахождение момента времени, при котором Uvx достигает максимума");
                if(1 != scanf("%d", &choose) || choose > 4 || choose < 1 ) {
                    puts("Error!");
                    return 0;
                }
                pog(choose, t, Uvx, Uvix, n, dt, eps);
                break;
            }

            case 3: {
                FILE *f1,*f2,*f3;                   //вывод массивов в файлы
                f1=fopen("massiv_t.txt","w");
                f2=fopen("massiv_Uvx.txt", "w");
                f3=fopen("massiv_Uvix.txt", "w");
                for (int i=0;i<n;i++) { 
                    fprintf(f1," %6.3f\n",t[i]);
                    fprintf(f2," %6.3f\n", Uvx[i]);
                    fprintf(f3," %6.3f\n",Uvix[i]);
                }
                fclose(f1);
                fclose(f2);
                fclose(f3);
                puts("\nДанные записаны в файлы");
                break;
            }
            case 4: {
                puts("Bye");
                return 0;
            }

            default: {
                puts("Error!");
                return 0;
            }
        }
    }
}
