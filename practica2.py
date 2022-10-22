#!/usr/bin/env python3

from time import sleep
import gnupg
import multiprocessing


def worker_thread(id, inicio, final):
    with open('wordlist.txt', 'rb') as f:
        print(f"thread {id}: inicio {passwords[inicio]}: final {passwords[final]}")
        sleep(0.01)
        for _ in range (0, inicio):
            f.readline()
        while inicio <= final:
            passw = f.readline()
            print(passw)
            check_pass(passw.strip().decode('utf-8'))
            inicio+=1


def check_pass(passw):
    with open('aes256.txt.gpg', 'rb') as fenc:
        status = gpg.decrypt_file("aes256.txt.gpg",always_trust=True, passphrase=passw, output='dec.txt')
        if status.ok == True:
            print(f"Credenciales correctas: {passw}")


if __name__ == "__main__":
    
    gpg = gnupg.GPG()
    num_threads = 8

    passwords = []

    with open('wordlist.txt', 'rb') as f:
        passwords = f.readlines()
    f.close()

    work_size = int(passwords.__len__() / num_threads)
    
    inicio = 0
    final = work_size

    threads = []

    for id in range(0,num_threads):
        threads.append(multiprocessing.Process(target=worker_thread, args=(id, inicio, final)))
        threads[-1].start()
        inicio = final +1
        if id == num_threads-2:
            final = passwords.__len__()-1
        else:
            final = final + work_size

    for thread in threads:
        thread.join()
