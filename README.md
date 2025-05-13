*Ontica Alexandra-Elena - 321CB*

# Tema 3 - PCom

Am ales sa scriu in C deoarece la inceputul temei nu am preconizat ca voi avea nevoie de vreo structura de date din STL. Drept urmare, pentru parsarea de JSONi am folosit Parson.

Am pornit cu rezolvarea de la laboratoul 9. Am inclus in tema fisierele `helpers.c`, `helpers.h`, `buffer.c`, `buffer.h`, luate cu copy-paste (nu am modificat nimic). M-am folosit si de fisierul `requests.c` (si automat si `requests.h`) din lab, adaugand in plus la implementarile mele pentru `compute_get_request` si `compute_post_request` de la laborator si posibilitatea de a agauga un header cu token in mesaj si eliminand parametrul `body_data_count` deoarece acum ii trimit deja JSONul format. Am adaugat, de asemenea, functiile `compute_delete_request` si `compute_put_request` (m-am inspirat din implementarea celorlaltor 2 functii, de ex la PUT request se modifica doar headerul, in rest e identic cu POST).

Am ales sa adaug pe alocuri comentarii in engleza ca sa pastrez stilul din laboratoare, unde comentariile si indicatiile erau tot in engleza.

In client citesc cate o comanda pe rand (o iteratie din while = o comanda) si, in functie de comanda, citesc parametrii necesari si apelez functia corespunzatoare.

Functiile cu comenzi se afla in fisierul `commands.c`, cu semnaturile + alte define-uri in `commands.h`.

Pentru stocarea cookie-urilor si a tokenului am ales sa folosesc variabile globale in care sa le salvez.

### Fiecare comanda urmeaza in mare aceeasi pasi:
- (oprional) construiesc JSONul pe care trebuie sa il trimit catre server
- construiesc URLul catre care trimit request
- construiesc mesajul pe care il voi trimite cu functiile de compute
- deschid o conexiune cu serverul
- trimit mesajul catre server
- astept raspunsul serverului
- extrag statusul raspunsului (200, 403 etc.)
- in functie de status, afisez fie "success" urmat de alte date necesare (in principal la GET am nevoie sa afisez si alte informatii), fie "error", urmat de eroarea primita de la server (extrasa din JSONul primit ca raspuns)
- (optinal) in caz de succes, extrag datele utile din JSONul de raspuns si le salvez/afisez (ex: token, id-ul unei colectii nou create, toti userii etc.)
- (optional) adaug cookie-urile primite in vectorul de cookies
- clean up: eliberez memoria alocata dinamic si inchid conexiunea

**!!** La toate comenzile am incercat sa pastrez formatul pentru output prezentat in cerinta (pe pagina unde sunt descrise functionalitatile clientului), am luat cu copy-paste mesajele de succes de pe site.

#### Mentiuni verificare response status:
- cand verific daca statusul indica succes, verific ca primele 2 cifre sa fie 20 deoarece succesul poate fi indicat de coduri precum 200, 201 etc.
- la fel si la eroare, verific ca primele doua cifre din status sa fie 40 ca sa imi prinda si coduri de genul 401, 403 etc.
- am si un branch in if care prinde si alte statusuri, a fost de folos cand serverul arunca 500 ca sa stiu ca problema nu e de la mine

#### Mici exceptii de la acest tipar:
- la logout si logout_admin sterg si cookie-urile si tokenul de acces daca statusul este unul de succes
- la **add_collection** am avut nevoie de mai multe requesturi ca sa rezolv cerinta:
    - am o functie care respecta pasii descrisi mai sus care imi creeaza o colectie goala
    - am o functie care adauga un film intr-o colectie pe care o apelez pentru fiecare film in parte
    - ambele functii imi returneaza statusul intors ca sa il pot verifica in functia care le apeleaza
- la **add_movie_to_collection** folosesc functia auxiliara pe care am folosit-o si la **add_collection**

### Probleme 
Pe langa toate probleme semnalate pe forum legate de server (caruia ii dadeam toti DoS la cate requesturi trimiteam si normal ca nu mergea mereu), am avut probleme si cu timeout-ul din checker. Acesta era cam mic, 1s, tinand cont ca reply-urile la requesturi veneau foarte greu. De cele mai multe ori checkul trimitea requesturi noi fara sa primeasca reply la requestul vechi si din cauza asta considera testele picate desi replyul venea corect ulterior. Daca cresateam acest timeout la 3-5s totul era ok.

Problema aceasta se vedea cel mai clar la colectii (am semnalat si pe forum problema). Din acest motiv am incetat la **add_collection** sa mai afisez si detaliile despre colectia adaugata (s-a scris pe forum ca este ok asa) si afisez doar mesaj de succes. Sper ca nu se depunteaza. La timeout mai mare de o secunda merge mereu si cu toate detaliile afisate, la timeout mic am erori.

Alte detalii de implementare pe care am considerat ca trebuie sa le explic le-am scris in comentarii.