#include <stdio.h>
#include <stdlib.h>

struct Virus{ //структура переменных с данными о вирусе
    char name[100];
    int move;
    char sign[6];
    char signfile[6];
    char MZ[2];
    char MZT[2];
}vir;
int CloseFile(FILE *f){ //закрытие файла
    int resoult;
    resoult=fclose(f);
    if (resoult!=0) return 1;
    return 0;
}
int DataRead(void){ //чтение в переменные Б\Д
    FILE *f;
    int resoult;
    f=fopen("Database.txt","r");
    if (f==NULL) return 1;

    resoult=fscanf(f,"%s %d %hhx %hhx %hhx %hhx %hhx %hhx",&vir.name,&vir.move,&vir.sign[0],&vir.sign[1],&vir.sign[2],&vir.sign[3],&vir.sign[4],&vir.sign[5]);
    if (resoult!=8) return 2;

    resoult=fclose(f);
    if (resoult!=0) return 3;

    return 0;
}
int SeekZero(FILE *f){ //идём в начало файла
    int resoult;
    resoult=fseek(f,0,SEEK_SET);
    if(resoult!=0) return 1;
    return 0;
}
int SeekEnd(FILE *f){ //идём в конец файла
    int resoult;
    resoult=fseek(f,0,SEEK_END);
    if(resoult!=0) return 1;
    return 0;
}
int GoSign(FILE *f){ //идём к сигнатуре
    int resoult;
    resoult=fseek(f,vir.move,SEEK_SET);
    if(resoult!=0) return 1;
    return 0;
}
int MZScan(FILE *f){
    int resoult;
    vir.MZT[0]='M';
    vir.MZT[1]='Z';
    for (int i=0;i<2;i++){
        resoult=fread(&vir.MZ[i],sizeof(char),1,f);
        if (resoult!=1) return 1;
    }
    return 0;
}

int main()
{
    FILE *f;
    char FileName[100];
    int resoult,reslen;
    int check=0;

    //привествие
    resoult=printf("---------------------------------------------------\n| You are welcomed by the MALWARE SEARCH PROGRAM! |\n---------------------------------------------------\nTo check the file, enter the full path to it below.\n");
    if(resoult<0){
        printf("Error printf!");
        return 8;
    }

    //открытие базы данных и запись значений
    resoult=DataRead();
    switch (resoult){
        case 1:
            printf("Error open file!\n");
            return 1;
        case 2:
            printf("Error of read database");
            return 2;
        case 3:
            printf("Error close file!");
            return 3;
    }

    //ввод имени\пути файла для проверки
    resoult=printf("Enter the path to the file: ");//можно в функцию
    if(resoult<0){
        printf("Error printf!");
        return 8;
    }
    resoult=scanf("%s",&FileName);
    if(resoult!=1){
        printf("Error scanf!");
        return 9;
    }

    //открытие .ехе файла
    f=fopen(FileName,"rb");
    if (f==NULL){
        printf("Error open file!");//можно в функцию
        return 10;
    }

    //поиск MZ в начале
    //переходим в начало файла на всякий случай
    resoult=SeekZero(f);
    if(resoult==1){
        printf("Error moving to the beginning of the file!");
        return 5;
    }

    //достаю MZ из начала файла
    resoult=MZScan(f);
    if(resoult==1){
        printf("Error of fread!");
        return 11;
    }

    //сравнение MZ
    if (vir.MZ[0]!=vir.MZT[0]&&vir.MZ[1]!=vir.MZT[1]){
        resoult=printf("\nThis is a GOOD program! Have a nice day! Bye!\n");
        if(resoult<0){
            printf("Error printf!");
            return 8;
        }
        resoult=CloseFile(f);
        if (resoult==1){
            printf("Error close file!");
            return 4;
        }
        return 0;
    }

    //ищем размер файла
    resoult=SeekEnd(f);
    if(resoult==1){
        printf("Error when moving to the end of the file!");
        return 6;
    }
    reslen=ftell(f);//узнаем текущее смещение в конце файла
    if (reslen==-1){
        printf("Error of ftell!");
        return 12;
    }
    if(reslen<vir.move){ //сравниваем текущее смещение со смещением нашей сигнатуры
        resoult=printf("\nThis is a GOOD program! Have a nice day! Bye!\n"); //если размер меньше, то это не наш файл
        if(resoult<0){
            printf("Error printf!");
            return 8;
        }
        resoult=CloseFile(f);
        if (resoult==1){
            printf("Error close file!");
            return 4;
        }
        return 0;
    }

    //идём к началу сигнатуры
    resoult=GoSign(f);
    if(resoult==1){
        printf("Error when moving to the beginning of the signature!");
        return 7;
    }

    //сравниваем сигнатуры
    check=0;
    for(int i=0;i<6;i++){
        resoult=fread(&vir.signfile[i],sizeof(vir.signfile[0]),1,f);
        if (resoult!=1){
            printf("Error of fread!");
            return 13;
        }
        if (vir.signfile[i]==vir.sign[i]){ //само сравнение сигнатур и счётчик
            check+=1;
        }
    }

    //выводы по сигнатуре
    if (check==6){
        resoult=printf("\nThis is a BAD program! \nVirus name: %s \nFile name: %s\n",vir.name,FileName); //если сигнатуры совпали - это ВИРУС
        if(resoult<0){
            printf("Error printf!");
            return 8;
        }
        resoult=CloseFile(f);
        if (resoult==1){
            printf("Error close file!");
            return 4;
        }
        return 0;
    }
    else{
        resoult=printf("\nThis is a GOOD program! Have a nice day! Bye!\n"); //если сигнатуры не совпали - это /НЕ\ ВИРУС
        if(resoult<0){
            printf("Error printf!");
            return 8;
        }
        resoult=CloseFile(f);
        if (resoult==1){
            printf("Error close file!");
            return 4;
        }
        return 0;
    }
    return 0;
}
