#include <stdio.h>
#include <stdlib.h>

struct Virus{ //��������� �\�
char name[100];
char sign[6];
char signfile[6];
int move;
}vir;
int CloseFile(FILE *f){ //�������� �����
    int resoult;
    resoult=fclose(f);
    if (resoult!=0) return 1;
    return 0;
}
int DataRead(void){ //������ � ���������� �\�
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
int SeekZero(FILE *f){ //��� � ������ �����
    int resoult;
    resoult=fseek(f,0,SEEK_SET);
    if(resoult!=0) return 1;
    return 0;
}
int SeekEnd(FILE *f){ //��� � ����� �����
    int resoult;
    resoult=fseek(f,0,SEEK_END);
    if(resoult!=0) return 1;
    return 0;
}
int GoSign(FILE *f){ //��� � ���������
    int resoult;
    resoult=fseek(f,vir.move,SEEK_SET);
    if(resoult!=0) return 1;
    return 0;
}



int main()
{
    FILE *f;
    char FileName[100];
    int resoult,reslen;
    int MZ[2];
    int MZT[2]={'M','Z'};
    int check=0;

    //����������
    resoult=printf("---------------------------------------------------\n| You are welcomed by the MALWARE SEARCH PROGRAM! |\n---------------------------------------------------\nTo check the file, enter the full path to it below.\n");
    if(resoult<0){
        printf("Error printf!");
        return 8;
    }

    //�������� ���� ������ � ������ ��������
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

    //���� �����\���� ����� ��� ��������
    resoult=printf("Enter the path to the file: ");//����� � �������
    if(resoult<0){
        printf("Error printf!");
        return 8;
    }
    resoult=scanf("%s",&FileName);
    if(resoult!=1){
        printf("Error scanf!");
        return 9;
    }

    //�������� .��� �����
    f=fopen(FileName,"rb");
    if (f==NULL){
        printf("Error open file!");//����� � �������
        return 10;
    }

    //����� MZ � ������
    resoult=SeekZero(f);
    if(resoult==1){
        printf("Error moving to the beginning of the file!");
        return 5;
    }
    for (int i=0;i<2;i++){
        resoult=fread(&MZ[i],sizeof(char),1,f);
        if (resoult!=1){
            printf("Error of fread!");
            return 11;
        }
    }

    //��������� MZ
    if (MZ[0]!=MZT[0]&&MZ[1]!=MZT[1]){
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

    //���� ������ �����
    resoult=SeekEnd(f);
    if(resoult==1){
        printf("Error when moving to the end of the file!");
        return 6;
    }
    reslen=ftell(f);//������ ������� �������� � ����� �����
    if (reslen==-1){
        printf("Error of ftell!");
        return 12;
    }
    if(reslen<vir.move){ //���������� ������� �������� �� ��������� ����� ���������
        resoult=printf("\nThis is a GOOD program! Have a nice day! Bye!\n"); //���� ������ ������, �� ��� �� ��� ����
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

    //��� � ������ ���������
    resoult=GoSign(f);
    if(resoult==1){
        printf("Error when moving to the beginning of the signature!");
        return 7;
    }

    //���������� ���������
    check=0;
    for(int i=0;i<6;i++){
        resoult=fread(&vir.signfile[i],sizeof(vir.signfile[0]),1,f);
        if (resoult!=1){
            printf("Error of fread!");
            return 13;
        }
        if (vir.signfile[i]==vir.sign[i]){ //���� ��������� �������� � �������
            check+=1;
        }
    }

    //������ �� ���������
    if (check==6){
        resoult=printf("\nThis is a BAD program! \nVirus name: %s \nFile name: %s\n",vir.name,FileName); //���� ��������� ������� - ��� �����
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
        resoult=printf("\nThis is a GOOD program! Have a nice day! Bye!\n"); //���� ��������� �� ������� - ��� /��\ �����
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
