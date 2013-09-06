Informacion que debe mostrar el programa.

Parte B
Escriba una versión inicial del ksamp programa que inspeccione las variables del kernel a través del /proc e informe por stdout: 

Tipo y modelo de CPU. 
Versión de Kernel.
Cantidad de tiempo transcurrido desde que se inició el sistema operativo, con el formato ddD:hh:mm:ss. 


Parte C
Escriba una segunda versión del programa que imprima la misma información que la versión por defecto, pero en caso de invocarse con la opción ­s, agrega la siguiente información: 

Cantidad de tiempo de CPU utilizado para usuarios, sistema y proceso idle. 
Cantidad de cambios de contexto. 
Fecha y hora cuando el sistema fue iniciado. 
Número de procesos creados desde el inicio del sistema. 


Parte D
La tercera parte involucra imprimir todo lo de las versiones anteriores, pero cuando se invoca con la opción ­l interval duration imprime además:

Número de peticiones a disco realizadas. 
Cantidad de memoria configurada en el hardware. 
Cantidad de memoria disponible. 
Lista de los promedios de carga de 1 minuto. 
