CC = gcc
CFLAGS = -c -Wall -g
OBJS = main.o pam.o silhouette.o bss_tss.o

.PHONY: run clean zip

run: main
	./main 51 5 choixpeauMagique.csv
	
main: $(OBJS)
	$(CC) -o $@ $(OBJS) -g -lm

main.o: main.c *.h
	$(CC) $(CFLAGS) $< 

%.o: %.c %.h
	$(CC) $(CFLAGS) $< 

clean:
	rm -f $(OBJS)
	rm -f main
	rm -f *.txt
	ls -l

DOSSIER = AMMAD_Massina-BENAYED_Khadija-DEMANGE_Noe-VILLA_Romain_IN511

zip:
	rm -rf $(DOSSIER)
	rm -f $(DOSSIER).zip
	mkdir $(DOSSIER)
	cp *.c *.h Makefile choixpeauMagique.csv kmeans.R $(DOSSIER)
	zip -r $(DOSSIER).zip $(DOSSIER)
	rm -rf $(DOSSIER)
	ls -l
