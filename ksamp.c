#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#define BUFFSIZE 256

void validsOpt();
void defaultInfo();
void showCpuinfo();
void showVersion();
int showUptime();
void showStats();
void showMeminfo();
void infoEstadistica();
void parser(char txt[BUFFSIZE], char x);
void uptimeSystemDate();
void showDiskstats();
void findStr(char* filename, char* matched, char* matchStr);
void showLoadavg(char **argv, char *argumento, int puntero);

int main(int argc, char *argv[]) {
	int opcion;

	printf("KSAMP SOFTWARE 1.0 \n==================================================\n");

    if (argc == 1){
		defaultInfo();
	}

	 while ((opcion = getopt (argc, argv, "sl:")) != -1){
		switch (opcion) {
			case 's':
				if(argc == 2){
					defaultInfo();
					showStats();
					uptimeSystemDate();
				}else{
					validsOpt();
				}
				break;

			case 'l':
				if(argc == 4){
                    defaultInfo();
					showStats();
					uptimeSystemDate();
                    showMeminfo();
                    showDiskstats();
                    showLoadavg(argv,optarg,optind);
				}else{
					validsOpt();
				}
				break;

			default:
				validsOpt();
				return 0;
      		}
	}
	printf("\n");
    return 0;
}
/**
* Funcion para ser llamada en cas de ingreso erroneo de opciones. Muestra en pantalla las opciones validas.
*/
void validsOpt(){
	printf("Wrong choice!\n");
	printf("Valid Options: -s , -l [interval] [time]");
	printf("\n\n");
}

/**
*Muestra informacion referente a la parte B de la informacion.
*/
void defaultInfo(){
    showCpuinfo();
    showVersion();
    showUptime();
}

/**
* /proc/cpuinfo
* This  is  a collection of CPU and system architecture dependent items, for each supported architecture a different list.
* Two common entries are processor which gives CPU number and bogomips; a system constant that is calculated during kernel
* initialization.  SMP  machines  have  information  for each CPU.
*/
void showCpuinfo(){
    FILE *fd;
	char cpuinfo1[BUFFSIZE+1];
	char cpuinfo2[BUFFSIZE+1];
	char cpuinfo3[BUFFSIZE+1];
	char str1 [20];
	char str2 [20];
	char str3 [20];
	char str4 [20];
	char str5 [20];
	char str6 [20];

	fd = fopen("/proc/cpuinfo","r");
	fgets(cpuinfo1, BUFFSIZE+1, fd);
	fgets(cpuinfo1, BUFFSIZE+1, fd);
	fgets(cpuinfo2, BUFFSIZE+1, fd);
	fgets(cpuinfo2, BUFFSIZE+1, fd);
	fgets(cpuinfo2, BUFFSIZE+1, fd);
	fclose(fd);
	fflush (fd);

    //Filtra Type y lo imprime
    sscanf(cpuinfo1,"%*s %*s %s ", str1);
    printf("Type: %s \n",str1);

    //Filtra el modelo, lo copia a str2 y la imprime
    sscanf(cpuinfo2,"%*s %*s %*s %s %s %s %s %s %s", str1, str2, str3, str4, str5, str6);
    printf("Model: %s %s %s\n",str1, str2, str3, str5, str6);
}

/**
* /proc/version
* This string identifies the kernel version that is currently running.
* It includes the contents of /proc/sys/kernel/ostype, /proc/sys/kernel/osrelease
* and /proc/sys/kernel/version.  For example:
* Linux version 1.0.9 (quinlan@phaze) #1 Sat May 14 01:51:54 EDT 1994
*/
void showVersion(){
    FILE *fd;
    char version[BUFFSIZE+1];
    char str1 [20];

    fd = fopen("/proc/version","r");
	fgets(version, BUFFSIZE+1, fd);
	fclose(fd);
	fflush(fd);

    //Filtra Version y la imprime
	sscanf(version,"%*s %*s %s ", str1);
	printf("Kernel Version: %s \n",str1);
}

/**
* /proc/uptime
* This file contains two numbers: the uptime of the system (seconds),
* and the amount of time spent in idle process (seconds).
*/
int showUptime(){
    FILE *fd;
	int time=0, days=0, hours=0, mins=0, secs=0;

	fd = fopen("/proc/uptime", "r");
	fscanf(fd,"%d",&time);

	days = time / 86400;
	time = time % 86400;
	hours  = time / 3600;
	time = time % 3600;
	mins = time / 60;
	secs = time % 60;

	fclose(fd);
	fflush(fd);

	//Filtra uptime y lo imprime
	printf("Uptime: %dd:%dh:%dm:%ds\n",days,hours,mins,secs);
	return time;
}

/**
* /proc/stat
* kernel/system statistics.  Varies with architecture.
* Ver https://www.kernel.org/doc/Documentation/filesystems/proc.txt (1.8)
*/
void showStats(){
    FILE *fd;
    char stat1[BUFFSIZE+1];
	char stat2[BUFFSIZE+1];
	char str1 [20];
	char str2 [20];
	char str3 [20];
	char str4 [20];
	char str5 [20];
	char str6 [20];
    int flag1 = 1, flag2 = 1;

    fd = fopen("/proc/stat","r");
	fgets(stat1, BUFFSIZE+1, fd);

	//La cantidad de cpu puede variar, por ende no es seguro hacer fgets un numero fijo de veces
	//Por eso el while ejecuta fgets() hasta encontrar la primer linea que empiece con 'p',
	//que es la linea que contiene el valor buscado.
	while(!feof(fd)){
        fgets(stat2, BUFFSIZE+1, fd);

        if(strstr(stat2, "ctxt ") && flag1){
            sscanf(stat2,"%*s %s", str6);
            flag1 = 0;
        }
        if(strstr(stat2, "processes ") && flag2){
            sscanf(stat2,"%*s %s", str5);
            flag2 = 0;
        }
	}
	fclose(fd);
	fflush(fd);

    //Filtra los datos de uso del CPU y los imprime
    sscanf(stat1,"%*s %s %*s %s %s", str1, str2, str3);
    printf("UserTime: %s 1/100ths of a second\n",str1);
    printf("SysTime: %s 1/100ths of a second\n",str2);
    printf("IdleTime: %s 1/100ths of a second\n",str3);

    //Filtra el numero de procesos creados y los imprime
    printf("Processes created: %s\n", str5);
    printf("Context Changes: %s\n", str6);
}
/**
* /proc/meminfo
* This file reports statistics about memory usage on the system.  It is used by free(1) to report the amount of free
* and used memory (both physical and swap) on the system as well as the shared memory and buffers used by the kernel.
* Each line of the file consists of a parameter name, followed  by  a colon, the value of the parameter, and an option
* unit of measurement (e.g., "kB").  The list below describes the parameter names and the format specifier required
* to read the field value.  Except as noted below, all of the fields have been present since at least Linux 2.6.0.
* Some fileds are only displayed if the kernel was configured with various options; those dependencies are noted in
* the list.
*/
void showMeminfo(){
    FILE *fd;
	char meminfo1[BUFFSIZE+1];
	char meminfo2[BUFFSIZE+1];
	char str1 [20];

	fd = fopen("/proc/meminfo","r");
	fgets(meminfo1, BUFFSIZE+1, fd);
	fgets(meminfo2, BUFFSIZE+1, fd);
	fclose(fd);
	fflush(fd);

	//Filtra memoria total y la imprime
    sscanf(meminfo1,"%*s %s ", str1);
    printf("TotalMem: %s Kb\n",str1);

    //Filtra memoria libre y la imprime
    sscanf(meminfo2,"%*s %s ", str1);
    printf("FreeMem: %s Kb\n",str1);
}

/**
* Muestra la fecha y hora de inicio del sistema.
*/
void uptimeSystemDate(){
    FILE *in;
    extern FILE *popen();
    char buff[BUFFSIZE];

    in = popen("date -d \"`cut -f1 -d. /proc/uptime` seconds ago\"", "r");
    printf("Uptime system date: ");
    while(fgets(buff, sizeof(buff), in)!=NULL){
        printf("%s", buff);
    }
pclose(in);
}

/**
 * Muestra peticiones totales a disco
 */
void showDiskstats(){
	char buff[256];
	unsigned int reads, writes, requests;

	findStr("/proc/diskstats", buff, "sda");
	sscanf(buff, "sda %u", &reads);
	sscanf(buff, "sda %*u %*u %*u %*u %u", &writes);
	requests = writes + reads;
	printf("Disk loads: %u\n", requests);
}

/**
 * En matched coloca un puntero a la linea que comienza con la cadena especificada en matchStr.
 * @param filename Archivo donde se buscara la linea
 * @param matched Almacena la linea que comienza con la cadena matchStr.
 * @param matchStr Cadena a buscar.
 */
void findStr(char* filename, char* matched, char* matchStr){
	FILE* fd;
	char* findStr = NULL;
	char buffer[500];
	fd = fopen(filename,"r");

	while(feof(fd)==0){
		fgets(buffer, 500, fd);
		findStr = strstr(buffer, matchStr);
		if(findStr!=NULL)
			break;
	}
	fclose(fd);
	strcpy(matched,findStr);
}

/**
 * Obtiene informacion sobre el uso del disco del archivo /proc/loadavg
 * de manera secuencial con parametros de tiempo definidos por el usuario.
 */
void showLoadavg(char **argv, char *arg, int pointer){
	FILE *fd;
	int interval, time;
	char aux[BUFFSIZE];
	float avg;

	time=atoi(*(argv+pointer));
    printf("\nDisk load average for last minute: ");
	while(time>0){
		fd=fopen("/proc/loadavg","r");
		fscanf(fd, "%s", aux);
		avg = atof(aux);
		printf("\n--> %.2f",avg);
		interval=atoi(arg);
		sleep(interval);
		fclose(fd);
		time=time-interval;
	}
}

