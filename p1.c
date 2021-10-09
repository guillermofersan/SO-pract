/*
 * SO LabAssignment1
 *
 * AUTHOR 1: Guillermo Fernández Sánchez | login: guillermo.fernandezs
 * AUTHOR 2: Javier Fernández Rozas      | login: j.frozas
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


#include "list.h"

#define MAXLINEA 1024

tList list;
bool iscmd;

void ProcesarEntrada(char *tr[]);
int trocearcadena(char * cadena, char * trozos[]);

void cmd_autores(char **);
void cmd_carpeta(char **);
void cmd_pid(char **);
void cmd_fin(char **);
void cmd_fecha(char **);
void cmd_hist(char **);
void cmd_comando(char **);
void cmd_infosis(char **);
void cmd_ayuda(char **);
void cmd_crear(char **);
void cmd_borrar(char **);
void cmd_borrarrec(char **);
void cmd_listfich(char **);
void cmd_listdir(char **);




struct CMD{
    char * cmdname;
    void (*func)(char **);
};

struct CMD C[]={
        {"fin", cmd_fin},
        {"bye", cmd_fin},
        {"salir", cmd_fin},
        {"carpeta", cmd_carpeta},
        {"autores", cmd_autores},
        {"pid", cmd_pid},
        {"fecha", cmd_fecha},
        {"hist", cmd_hist},
        {"comando", cmd_comando},
        {"infosis", cmd_infosis},
        {"ayuda", cmd_ayuda},
        {"crear", cmd_crear},
        {"borrar", cmd_borrar},
        {"borrarrec", cmd_borrarrec},
        {"listfich", cmd_listfich},
        {"listdir", cmd_listdir},
        {NULL ,NULL}
};





void cmd_autores (char *tr[]){
    /*prints the authors logins and/or names depending on the command's argument*/
    if (tr[0]==NULL){
        printf("The authors are:\nGuillermo Fernandez: guillermo.fernandezs\nJavier Fernandez: j.frozas\n");
    } else if(!strcmp(tr[0],"-l")){
        printf("The authors are:\nguillermo.fernandezs\nj.frozas\n");
    } else if(!strcmp(tr[0],"-n")){
        printf("The authors are:\nGuillermo Fernandez\nJavier Fernandez\n");
    } else printf("Command not found\n");

}

void cmd_pid(char *tr[]){
/*prints the process id of the current shell process or the id of the process' parent*/
    if (tr[0]== NULL) {
        printf("Shell process pid: %d\n",getpid());
    } else if(!strcmp(tr[0],"-p")){
        printf("Shell parent process pid: %d\n",getppid());
    }	else{
        printf("Command not found\n");
    }
}


void cmd_carpeta(char *tr[]){
/*prints the current directory of the shell*/
    char dir[MAXLINEA];

    if(tr[0]== NULL)
        printf("%s\n", getcwd(dir, MAXLINEA));
    else
    if(chdir(tr[0]) == -1){
        printf("Cannot change dir %s:\n", tr[0]);
        perror("");


    } else printf("New directory: %s\n",getcwd(dir, MAXLINEA));

}

void cmd_fecha(char *tr[]){
/*prints the system time/date*/

    time_t t;
    t = time(NULL);
    struct tm tm;
    tm = *localtime(&t);

    if(tr[0]==NULL){
        printf("Date and time: %02d/%02d/%d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    } else if(!strcmp(tr[0],"-d")){
        printf("Date: %02d/%02d/%02d\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    } else if(!strcmp(tr[0],"-h")){
        printf("Time: %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
}



void cmd_infosis(char *tr[]){
/*prints the system information*/

    struct utsname infosis;
    uname(&infosis);

    printf("%15s %s\n","system name:  ", infosis.sysname);
    printf("%15s %s\n","node name:  ", infosis.nodename);
    printf("%15s %s\n","release:  ", infosis.release);
    printf("%15s %s\n","version:  ", infosis.version);
    printf("%15s %s\n","machine:  ", infosis.machine);

}

void cmd_ayuda(char *tr[]){

    if(tr[0]==NULL) printf("Available commands:\n->autores\n->pid\n->carpeta\n->fecha\n->hist\n->comando\n->infosis\n->fin\n->salir\n->bye\n");
    else if(!strcmp(tr[0],"autores")) printf("autores [-l] [-n]: Prints the names and logins of the program authors.\n\n\t-l\tprints only the logins of the authors.\n\t-n\tprints only the names of the authors.\n\n");
    else if(!strcmp(tr[0],"pid")) printf("pid [-p]: Prints the pid of the process executing the shell.\n\n\t-p\tprints the pid of the shell’s parent process.\n\n");
    else if(!strcmp(tr[0],"carpeta")) printf("carpeta [direct]: Changes the current working directory of the shell to direct. When invoked without auguments it prints the current working directory.\n");
    else if(!strcmp(tr[0],"fecha")) printf("fecha [-d] [-h]: Prints both the current date and the current time.\n\n\t-d\tprints the current date in the format DD/MM/YYYY.\n\t-h\tprints the current time in the format hh:mm:ss.\n\n");
    else if(!strcmp(tr[0],"hist")) printf("hist [-c][-N]: Shows the historic of commands executed by this shell in order.\n\n\t-c\tclears the historic.\n\t-N\tprints the first N comands\n\n");
    else if(!strcmp(tr[0],"comando")) printf("comando N: Repeats command number N\n");
    else if(!strcmp(tr[0],"infosis")) printf("infosis: Prints information on the machine running the shell\n");
    else if(!strcmp(tr[0],"ayuda")) printf("ayuda [cmd]: ayuda displays a list of available commands.\n\n\t[cmd]\tgives a brief help on the usage of comand cmd\n\n");
    else if(!strcmp(tr[0],"fin")) printf("fin: Ends the shell\n");
    else if(!strcmp(tr[0],"bye")) printf("bye: Ends the shell\n");
    else if(!strcmp(tr[0],"salir")) printf("salir: Ends the shell\n");
    else if(!strcmp(tr[0],"crear")) printf("crear [-f] [name]: Creates a directory or file in the file system.\n\n\tcrear\t\t shows the current directory\n\tcrear [name]\t creates an empty directory with name [name]\n\tcrear -f [name]  creates an empty file with name [name]\n\n");
    else printf("command not found\n");

}

void cmd_fin(char *tr[]){
/*exits the shell*/
    exit(0);
}



void cmd_hist(char *tr[]){
/*prints the list of the previous commands*/

    LPos pos;
    tItemL item;

    if(tr[0]!=NULL && !strcmp(tr[0],"-c")){
        clearList(&list);
        iscmd=false;
    } else{

        int i=1, n;
        if(tr[0]==NULL){
            n=-MAXLINEA;
        } else n= strtol(tr[0], NULL, 10);

        if(!isEmptyList(list) && n<0){

            pos= first(list);
            while (pos!=LNULL && n<=-i){
                item  = getItem(pos, list);
                printf("%d->", i);
                puts(item.cmdline);
                pos = next(pos, list);
                i++;
            }
        } else printf("The historic is empty or number of commands to print is 0\n");
    }
}




void cmd_comando(char *nchar[]){
/*Repeats the command indicated*/

    int i, N = strtol(nchar[0], NULL, 10);
    if((N==0) | isEmptyList(list)){
        printf("command number not valid\n");
    } else{

        char linea[MAXLINEA];
        char *tr[MAXLINEA / 2];

        tItemL item;
        LPos pos;

        pos= first(list);

        for(i=1; i<N;i++){

            if(next(pos, list)!=NULL){
                pos= next(pos, list);
            } else {
                printf("command number not valid\n");
                pos=NULL;
                break;
            }


        }
        if(pos!=NULL){
            item = getItem(pos, list);
            printf("Repeating command %d:",i);
            puts(item.cmdline);

            strcpy(linea, item.cmdline);

            trocearcadena(linea, tr);
            ProcesarEntrada(tr);

        }

    }
    iscmd=false;
}


void cmd_crear(char *tr[]){
/*  */

    if(tr[0]==NULL) cmd_carpeta(tr);
    else if(!strcmp(tr[0],"-f")){
        if (tr[1]==NULL) cmd_carpeta(tr+1);
        else{

            FILE *f;
            f  = fopen (tr[1],"w");

            if (f==NULL) perror("Error");
        }
    }
    else {
        if(mkdir(tr[0],S_IRWXU)==-1){
            perror("Imposible crear");

        }
    }
}

void cmd_borrar(char *tr[]){

    int i=0;
    FILE *f;
    DIR *d;

    while (tr[i]!=NULL){

            if (remove(tr[i])==-1) perror("Imposible eliminar");

        i++;
    }
}

bool isDir (const char *path) {
    DIR *d;

    if ((d = opendir(path))) {
        closedir(d);
        return true;
    } else return false;
}

void deleteDir(const char *path){
    /*precondition: path belongs to a real directory*/
    int i=0;
    DIR *d;
    struct dirent *dirStruct;
    struct stat *fileStruct;
    char dir[MAXLINEA];
    char Curdir[MAXLINEA];
    strcpy(Curdir,getcwd(dir, MAXLINEA));

    d = opendir(path);

    chdir(path);
        if (d) {
            while ((dirStruct = readdir(d)) != NULL) {

                if (strcmp(dirStruct->d_name,".")!=0 && strcmp(dirStruct->d_name,"..")!=0){
                    printf("removing: %d->%s\n", i,dirStruct->d_name);
                    i++;
                    if (isDir(dirStruct->d_name)){
                        deleteDir(dirStruct->d_name);
                    }else{
                        if (remove(dirStruct->d_name)==-1) perror("");
                    }
                }
             }
                closedir(d);
        }

    chdir(Curdir);
    remove(path);

}

void cmd_borrarrec(char *tr[]){

    int i=0;
    DIR *d;

    while (tr[i]!=NULL){

        if((d= opendir(tr[i]))){
            closedir(d);
            if (remove(tr[i])==-1) deleteDir(tr[i]);
        } else if (remove(tr[i])==-1) perror("Imposible eliminar");

        i++;
    }




}

void printFile(bool longListing, bool link, bool acc, char* name){

    printf("%s\n", name);

}



void cmd_listfich(char *tr[]){

    /* (use stat)
     * none: size and name of each file
     * longListing: print out the date of last modification (in format YYYY/MM/DD-HH:mm), number of links,
     *              owner, group, mode (drwx format), size and name of the file.
     *              date number of links (inode number) owner group mode size name
     * link: only for long listing: if the file is a symbolic link the name of the file it points is printed as well
     *       date number of links (inode number) owner group mode size name->file the link points to
     *
     * acc: last access time will be used instead of last modification time
     * */

    bool longListing=false, link=false, acc=false;
    int i=0,j=0,k=0;


    char *names[MAXLINEA / 2];

    while(tr[i]!=NULL){
        if(!strcmp(tr[i],"-long")){
            longListing=true;
        } else if (!strcmp(tr[i],"-link")){
            link=true;
        } else if (!strcmp(tr[i],"-acc")){
            acc=true;
        } else{
            names[j]=tr[i];
            j++;
        }
        i++;
    }
    names[j]=NULL;
    if(j==0) {
        cmd_carpeta(tr+i);
    } else{
        while (names[k]!=NULL){
            printFile(longListing, link, acc, names[k]);
            k++;
        }
    }

}


void cmd_listdir(char *tr[]){

    /*
     * If file, use printFile
     * if no name
     *
     * none: size and name of each file
     * longListing: print out the date of last modification (in format YYYY/MM/DD-HH:mm), number of links,
     *              owner, group, mode (drwx format), size and name of the file.
     *              date number of links (inode number) owner group mode size name
     * link: only for long listing: if the file is a symbolic link the name of the file it points is printed as well
     *       date number of links (inode number) owner group mode size name->file the link points to
     *
     * acc: last access time will be used instead of last modification time
     *
     * hid: hidden files and directories are also listed
     *
     *reca:  when listing a directory’s contents, subdirectories will be listed recursively AFTER all the files in the directory.
     *       (if the -hid option is also given, hidden subdirectories will also get listed, except . and .. to avoid infinite recursion).
     *
     *recb: same as reca but contents are listed before the files in the directory
     *
     * */

    bool longListing=false, link=false, acc=false, hid=false;
    int i=0,j=0,k=0,rec=0;
    /*rec=0 if no recursion
     *rec=1 if reca
     *rec=2 if reb
     */


    char *names[MAXLINEA / 2];

    while(tr[i]!=NULL){
        if(!strcmp(tr[i],"-long")){
            longListing=true;
        } else if (!strcmp(tr[i],"-link")){
            link=true;
        } else if (!strcmp(tr[i],"-acc")){
            acc=true;
        } else if (!strcmp(tr[i],"-hid")){
            hid=true;
        } else if (!strcmp(tr[i],"-reca")){
            rec=1;
        } else if (!strcmp(tr[i],"-recb")){
            rec=2;
        } else{
            names[j]=tr[i];
            j++;
        }
        i++;
    }
    names[j]=NULL;

    /*
     * Loop to display each file with printfile nose
     * */



}





int trocearcadena(char * cadena, char * trozos[]){
    int i=1;

    if((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]= strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void ProcesarEntrada(char *tr[]){

    iscmd=true;
    bool found=false;

    int i;
    if(tr[0]==NULL) return;
    for (i = 0; C[i].cmdname != NULL; i++) {

        if (!strcmp(tr[0],C[i].cmdname)){
           (*C[i].func)(tr+1);
           found=true;
        }

    }

    if(!found) printf("command not found\n");



}

int main (int argc, char*argv[]) {
    char linea[MAXLINEA];
    char aux[MAXLINEA];
    char *tr[MAXLINEA / 2];
    
	
    createList(&list);

    while(1){
        printf("*)");
        fgets(linea, MAXLINEA, stdin);
        tItemL item;

	/*an aux array is created to store linea, then the content of aux is copied into the list*/
        strcpy(aux, linea);
        aux[strlen(aux) - 1] = '\0';

        if (trocearcadena(linea, tr) == 0)
            continue;
        ProcesarEntrada(tr);

        if(iscmd){

            strcpy(item.cmdline, aux);
            insertItem(item, &list);

        }
    }
}
