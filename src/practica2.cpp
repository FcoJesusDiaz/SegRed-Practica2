/*Libraries used*/
#include <iostream>
#include <stdlib.h>
#include <string>
#include <thread>
#include <math.h>
#include <vector>
#include <chrono>
#include <functional>
#include <algorithm>
#include <atomic>
#include <fstream>
#include <signal.h>
#include <string.h>

/*Custom library used*/
#include "colours.h"

/*Declaration of functions*/
void check_args(int argc, char** argv);
void create_threads(unsigned int n_threads, std::string passwords, std::string file_path);
void find_pass(unsigned int n_proc, unsigned int start_pos, unsigned int end_pos, std::string wordlist, std::string file_path);
void check_pass(std::string file_path, std::string pass);
int count_lines(std::string pathname);
void install_signal_handler();
void signal_handler(int signal_n);
void get_execution_time(int seconds);


/* Global Variables */
std::vector<std::thread> threads_vector;
std::atomic<bool> stop_threads (false);
std::atomic<int> attempts (0);


/* MAIN FUNCTION */
int main(int argc, char *argv[]){
    
    auto begin = std::chrono::high_resolution_clock::now();
    unsigned int n_threads;
    std::string file_path;

    check_args(argc, argv);
    install_signal_handler();

    n_threads = std::thread::hardware_concurrency();
    create_threads(n_threads, argv[1], argv[2]);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - begin);

    get_execution_time(elapsed.count());

    return EXIT_SUCCESS;
}

/*Gets execution time in %hours, %minutes and %seconds format*/
void get_execution_time(int seconds){
    int minutes, hours, copy_s = seconds;
    minutes = seconds / 60;
    seconds %= 60;
    hours = minutes / 60;
    minutes %= 60;
    std::cout << BOLDGREEN << "Tiempo de ejecución: " << hours << " horas, " 
        << minutes <<" minutos" << " y "<< seconds << " segundos. ("<< copy_s <<" segundos)." << RESET << std::endl;
}

/*Check correct input from user*/
void check_args(int argc, char *argv[]){
    
    if(argc != 3) {
        std::cerr << BOLDRED << "Numero de argumentos incorrecto. Uso: practica2 <diccionario> <archivo cifrado>."
                  << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    if(!strstr(argv[2], ".gpg")){
        std::cerr << BOLDRED << "El archivo " << argv[2] << " no esta cifrado\n"
                  << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream file;

    file.open(argv[2], std::ios::binary);

    if (!file.is_open()){
        std::cout << BOLDRED << "No se pudo abrir el siguiente archivo:'" << argv[2] << "'" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    file.close();
}


/*Create threads with the corresponding words that have to be checked out of the dictionary provided by the user*/
void create_threads(unsigned int n_threads, std::string wordlist, std::string file_path)
{
    int test_size, work_per_thread;
    unsigned int start_pos, end_pos;

    test_size = count_lines(wordlist);
    work_per_thread = test_size / n_threads;
    start_pos = 0;
    end_pos = work_per_thread;
    

    for (unsigned int i = 0; i < n_threads; i++){   
        threads_vector.push_back(std::thread(find_pass, i, start_pos, end_pos, wordlist, file_path));
        start_pos = end_pos + 1;
        (i == n_threads - 1) ? end_pos = test_size - 1 : end_pos += work_per_thread;
    }
    
    std::for_each(threads_vector.begin(), threads_vector.end(), std::mem_fn(&std::thread::join));
}

/*Reads line by line to check each password*/
void find_pass(unsigned int n_proc, unsigned int start_pos, unsigned int end_pos, std::string wordlist, std::string file_path)
{
    std::string pass;
    std::fstream file;

    file.open(wordlist, std::ios::in);

    for (unsigned int i = 0; getline(file, pass) && i <= end_pos && !stop_threads; i++){
        if(i >= start_pos){
            attempts += 1;
            std::cout << BOLDYELLOW <<"[HILO " << (n_proc + 1) << "]: Intento numero " << attempts << ". Probando con la siguiente clave: " << pass << RESET << std::endl;
            check_pass(file_path, pass);
        }
    }
    
    file.close();
}

/*Makes a system call to gpg with the gpg file and a password for decryption*/
void check_pass(std::string file_path, std::string pass)
{
    std::string gpg_cmd;
    int status;

    gpg_cmd = "gpg --batch --passphrase " + pass + " --yes -d " + file_path;

    status = system(gpg_cmd.c_str());
    
    if (status == EXIT_SUCCESS){
        stop_threads = true;
        std::cout << BOLDGREEN << "\nIntento " << attempts << ".Clave encontrada: " << pass << RESET << std::endl;
    }
}


/*Count the number of lines of a file*/
int count_lines(std::string pathname)
{
    int lines;
    std::ifstream file;
    std::string line = "";

    file.open(pathname, std::ios::binary);

    if (!file.is_open()){
        std::cout << BOLDRED << "No se pudo abrir el siguiente archivo:'" << pathname << "'" << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    for (lines = 0; !file.eof(); lines++) std::getline(file, line);

    file.close();
    return lines;
}


/* Main thread's signal handler for Ctrl + C interruptions  */
void signal_handler(int signal_n)
{
    stop_threads = true;
    std::cout << "Saliendo del programa..."
              << std::endl;
    exit(EXIT_SUCCESS);
}

/* Install main thread's signal handler */
void install_signal_handler()
{
    if (signal(SIGINT, signal_handler) == SIG_ERR){
        std::cerr << BOLDRED << "Error al instalar el manejador de señal." << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
}


