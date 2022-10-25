DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRFILES := files/

FLAGS := -I$(DIRHEA) -c -Wall -std=c++11
LDLIBS := -pthread -std=c++11
CC := g++

all : dirs compile

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) 

compile: $(DIROBJ)practica2.o
	$(CC) $^ -o $(DIREXE)practica2 $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(FLAGS) $^ -o $@

test1:
	./$(DIREXE)practica2 $(DIRFILES)wordlist.txt $(DIRFILES)aes256.txt.gpg

test2:
	./$(DIREXE)practica2 $(DIRFILES)wordlist.txt $(DIRFILES)archivo.pdf.gpg

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~ 
