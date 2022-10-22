#!/usr/bin/env python3

from time import sleep
import multiprocessing
import os
import gnupg

NUM_processes = 8

def worker_thread(proc_id, start, end):
    with open('wordlist.txt', 'rb') as f:
        #print(f"thread {id}: start {passwords[start]}: end {passwords[end]}")
        #sleep(0.01)
        for _ in range(0, start):
            f.readline()
        while start <= end:
            passw = f.readline()
            print(passw)
            check_pass(passw.strip().decode('utf-8'))
            start += 1


def check_pass(passw):
    gpg = gnupg.GPG()
    print(f"[{os.getpid()}]: Reading file...")
    status = gpg.decrypt_file("aes256.txt.gpg", always_trust=True, passphrase=passw, output='dec.txt')
    print(f"[{os.getpid()}]: Finished")
    if status.ok:
        print(f"Credenciales correctas: {passw}")


def main():
    passwords = []
    with open('wordlist.txt', 'rb') as file:
        passwords = file.readlines()
    file.close()

    work_size = int(len(passwords) / NUM_processes)
    
    start = 0
    end = work_size

    processes = []

    for proc_id in range(0, NUM_processes):
        new_proc = multiprocessing.Process(target=worker_thread, args=(proc_id, start, end))
        new_proc.start()
        processes.append(new_proc)
        start = end +1
        if proc_id == NUM_processes - 2:
            end = len(passwords) - 1
        else:
            end = end + work_size

    for process in processes:
        process.join()


if __name__ == "__main__":
    main()
