#include <stdio.h>
#define BUFFSIZE 100

void validsOpt();
void showCpuinfo();
void showVersion();
void showUptime();
void showStats();
void showMeminfo();

int main(int argc, char *argv[]) {
	int opcion;

    if (argc == 1){
		printf("case argc1\n\n");
		showCpuinfo();
		showVersion();
		showUptime();
		showStats();
		showMeminfo();
	}

	 while ((opcion = getopt (argc, argv, "sl:")) != -1){
		switch (opcion) {
			case 's':
				if(argc == 2){
					printf("case S\n");
				}else{
					validsOpt();
				}
				break;

			case 'l':
				if(argc == 4){
                    printf("case L\n");
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
/*
* Funcion para ser llamada en cas de ingreso erroneo de opciones. Muestra en pantalla las opciones validas.
*/
void validsOpt(){
	printf("Las opciones ingresadas no son correctas\n");
	printf("Opciones validas: -s , -l [intervalo] [tiempo]");
	printf("\n\n");
}

/*
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
    sscanf(cpuinfo2,"%*s %*s %*s %s", str1);
    printf("Model: %s \n",str1);
}

/*
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
	char str2 [20];
	char str3 [20];
	char str4 [20];
	char str5 [20];

    fd = fopen("/proc/version","r");
	fgets(version, BUFFSIZE+1, fd);
	fclose(fd);
	fflush(fd);

    //Filtra Version y la imprime
	sscanf(version,"%*s %*s %s ", str1);
	printf("Kernel Version: %s \n",str1);
}

/*
* /proc/uptime
* This file contains two numbers: the uptime of the system (seconds),
* and the amount of time spent in idle process (seconds).
*/
void showUptime(){
    FILE *fd;
    char uptime[BUFFSIZE+1];
    char str1 [20];
	char str2 [20];
	char str3 [20];
	char str4 [20];
	char str5 [20];

	fd = fopen("/proc/uptime", "r");
	fgets(uptime, BUFFSIZE+1, fd);
	fclose(fd);
	fflush(fd);

	//Filtra uptime y lo imprime
	sscanf(uptime,"%s %s ", str1, str2);
	printf("Uptime: %s %s \n", str1, str2);
}

/*
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

    fd = fopen("/proc/stat","r");
	fgets(stat1, BUFFSIZE+1, fd);

	//La cantidad de cpu puede variar, por ende no es seguro hacer fgets un numero fijo de veces
	//Por eso el while ejecuta fgets() hasta encontrar la primer linea que empiece con 'p',
	//que es la linea que contiene el valor buscado.
	while(str4[0] != 'p'){
        fgets(stat2, BUFFSIZE+1, fd);
        sscanf(stat2,"%s %s", str4, str5);
	}
	fclose(fd);
	fflush(fd);

    //Filtra los datos de uso del CPU y los imprime
    sscanf(stat1,"%*s %s %*s %s %s", str1, str2, str3);
    printf("UserTime: %s 1/100ths of a second\n",str1);
    printf("SysTime: %s 1/100ths of a second\n",str2);
    printf("IdleTime: %s 1/100ths of a second\n",str3);

    //Filtra el numero de procesos creados y los imprime
    printf("Processes: %s\n", str5);

}
/*
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
	char str2 [20];
	char str3 [20];
	char str4 [20];
	char str5 [20];

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
