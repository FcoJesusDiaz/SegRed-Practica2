# SegRed-Practica2
Programa que averigua la clave de un archivo gpg. La clave solo contiene minúsculas.

# Autores del proyecto
- Francisco Jesús Díaz Pellejero: FcoJesus.Diaz@alu.uclm.es
- Miguel de las Heras Fuentes: Miguel.heras@alu.uclm.es

# Compilación de ejecutables
- Para compilar los ficheros primero crearemos los directorios con los archivos objeto (.o) y los ejecutables mediante el comando 'make dirs'.
- Una vez creados estos directorios pasaremos a la compilación del archivo fuente mediante el comando 'make compile'. No se podrá ejecutar esta
regla si no se han creado los directorios antes
- Para unificar los 2 comandos anteriores se podrá ejecutar el comando 'make all'.

# Ejecución del programa
- Una vez compilado el programa tendremos a nuestra disposicion 2 tests
- Se ejecutará el primero mediante el comando 'test1'. Este test tratará de desencriptar un archivo gpg cuya clave es 'bb' mediante un diccionario de palabras
que contiene todas las combinaciones posibles de 4 caracteres para las letras minúsculas. El tiempo de ejecución es de 3 minutos y medio para un ordenador
con 4 núcleos y hyperthreading
- Se ejecutará el segundo mediante el comando 'test2'. Ahora la contraseña es de 4 caracteres ('sgrd') y se utiliza el mismo diccionario de palabras
de antes. El tiempo de ejecución es de unas 30 horas para un ordenador con 4 núcleos y hyperthreading

# NOTA
Debido al uso de llamadas al sistema como system(). Los manejadores de señal para el programa no funcionan. Por lo que una vez ejecutado el programa, si
se quiere parar se tendrá que realizar mediante la línea de comandos con el comando 'kill -9 <pid del programa>'
