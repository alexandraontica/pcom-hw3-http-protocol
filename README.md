Username: alexandra.ontica
Password: 1f6d38aca39c
id movie 1132 1147 1135

functie auxiliara la add movie to collection pt ca vreau sa o apelez cand creez colectie noua fara sa imi afiseze mesajul de la add movie to collection

--------------------------------------------------------------

*Ontica Alexandra-Elena - 321CB*

# Tema 3 - PCom

Am ales sa scriu in C deoarece la inceputul temei nu am preconizat ca voi avea nevoie de vreo structura de date din STL. Drept urmare, pentru parsarea de JSONi am folosit Parson.

Am pornit cu rezolvarea de la laboratoul 9. Am inclus in tema fisierele `helpers.c`, `helpers.h`, `buffer.c`, `buffer.h`, luate cu copy-paste (nu am modificat nimic). M-am folosit si de fisierul `requests.c` (si automat si `requests.h`) din lab, adaugand in plus la implementarile mele pentru `compute_get_request` si `compute_post_request` de la laborator si posibilitatea de a agauga un header cu token in mesaj si eliminand parametrul `body_data_count` deoarece acum ii trimit deja JSONul format. Am adaugat, de asemenea, functiile `compute_delete_request` si `compute_put_request` (m-am inspirat din implementarea celorlaltor 2 functii, de ex la PUT request se modifica doar headerul, in rest e identic cu POST).

Am ales sa adaug pe alocuri comentarii in engleza ca sa pastrez stilul din laboratoare, unde comentariile si indicatiile erau tot in engleza.

In client citesc cate o comanda pe rand (o iteratie din while = o comanda) si, in functie de comanda, citesc parametrii necesari si apelez functia corespunzatoare.
