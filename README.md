*Ontica Alexandra-Elena - 321CB*

# Tema 3 - PCom

Am ales sa scriu in C deoarece la inceputul temei nu am preconizat ca voi avea nevoie de vreo structura de date din STL. Drept urmare, pentru parsarea de JSONi am folosit Parson.

Am pornit cu rezolvarea de la laboratoul 9. Am inclus in tema fisierele `helpers.c`, `helpers.h`, `buffer.c`, `buffer.h`, luate cu copy-paste (nu am modificat nimic). M-am folosit si de fisierul `requests.c` (si automat si `requests.h`) din lab, adaugand in plus la implementarile mele pentru `compute_get_request` si `compute_post_request` de la laborator si posibilitatea de a agauga un header cu token in mesaj si eliminand parametrul `body_data_count` deoarece acum ii trimit deja JSONul format. Am adaugat, de asemenea, functiile `compute_delete_request` si `compute_put_request` (m-am inspirat din implementarea celorlaltor 2 functii, de ex la PUT request se modifica doar headerul, in rest e identic cu POST).

Am ales sa adaug pe alocuri comentarii in engleza ca sa pastrez stilul din laboratoare, unde comentariile si indicatiile erau tot in engleza.

In client citesc cate o comanda pe rand (o iteratie din while = o comanda) si, in functie de comanda, citesc parametrii necesari, verific daca sunt valizi (username/parola fara spatii, ca sunt numere naturale/reale acolo unde e cazul) si apelez functia corespunzatoare.

Functiile cu comenzi se afla in fisierul `commands.c`, cu semnaturile + alte define-uri in `commands.h`.

Pentru stocarea cookie-urilor si a tokenului am ales sa folosesc variabile globale in care sa le salvez.

### Fiecare comanda urmeaza in mare aceeasi pasi:
- (oprional) construiesc JSONul pe care trebuie sa il trimit catre server
- creez un vector de cookies care va fi adaugat la mesaj
- construiesc URLul catre care trimit request
- construiesc mesajul pe care il voi trimite cu functiile de compute
- deschid o conexiune cu serverul
- trimit mesajul catre server
- astept raspunsul serverului
- extrag statusul raspunsului (200, 403 etc.)
- verific statusul:
    - in caz de succes: 
        - afisez mesaj de succes
        - (optional) extrag datele utile din JSONul de raspuns si le salvez (ex: token, id-ul unei colectii nou create, etc.)
        - (optional) afisez alte informatii cerute de task (de ex. informatii despre film etc.)
        - (optional) daca am de afisat o lista, o sortez mai intai crescator dupa `id` (**vezi sectiunea "Probleme"**)
    - in caz de eroare:
        - extrag eroarea din JSONul primit
        - afisez eroarea
- (optional) adaug cookie-urile primite in variabilele globale pentru cookie-uri
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

#### add_collection - edge cases
In enunt nu este specificat ce se intampla daca, la adaugarea unei colectii, id-ul unui film e gresit (nu exista/nu sunt owner). Am preferat sa pastrez colectia in acest caz de eroare (nu o sterg). Totusi nu printez mesaj de succes, printez eroarea.

Puteam sa sterg colectia, dar cum cerinta nu cere acest lucru explicit am decis sa las asa.

De asemenea, presupun ca userul stie ca la add_collection trebuie sa dea id-uri de filme diferite (pare ceva de bun simt). Daca da acelasi id de mai multe ori va primi eroare, colectia va fi totusi creata si filmul va fi adaugat doar o data (din nou decid sa nu sterg colectia pentru ca acest caz nu e prevazut in cerinta).

#### Observatie
Unele id-uri le-am dat ca parametru direct ca string, nu ca int, deoarece am nevoie de ele doar ca sa le adaug la URL si imi e mai usor asa ca sa fac direct strcat fara conversii.

### Probleme 
O problema observata este ca la **get_movies**, **get_collections** si **get_collection**, listele de filme/colectii returnate nu ajung mereu in ordine crescatoare dupa id. O sesiune ar merge asa:

```
add_movie
title=film1
(nu conteaza restul campurilor)
(raspuns cu succes)

> add_movie
title=film2
(...)

> get_movies
(mesaj succes)
#1873 film2
#1872 film1
```

Acum checkerul va face **get_movie** cu primul id gasit in lista de filme (adica 1873) si va astepta sa primeasca primul film adaugat, ceea ce in exemplul dat nu se intampla => eroare

Acest lucru nu se intampla mereu, uneori trec testele fara probleme, alteori nu.
Ca sa fiu sigura ca imi trec mereu testele, sortez array-urile pe care le primesc crescator dupa id inainte sa le afisez.