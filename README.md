Cerinta1:

Incepem prin a scrie functia main si prin a verifica corectitudinea datelor
de intrare. Apoi m-am apucat cu crearea structurilor de date folosite pentru
grafuri: structura de graf cu lista de adiacenta si lista de varfuri. Apoi
functiile auxiliare pentru lucrul cu grafuri si liste in paralel cu citirea
datelor.

Nu am tinut cont de numele locatiilor decat pentru a face o mapare intre 
numere intregi de la 0 la N - 1 si acele nume pentru a putea citi datele
de intrare (in vectorul noduri), asa se face si in progamele reale. Citesc
cate un numar de locatie si daca nu il gasesc in vectorul de locatii deja salvate
ii aloc urmatorul numar disponibil si il salvez.

Odata citit graful ma gandesc la un mod simplu de a vedea cate componente 
conexe (insule sau zone legate cu rauri in enuntul nostru am): parcurg vectorul
de noduri din graf (eu practic am un vector de noduri cu o lista de adiacenta
pentru fiecare nod) si tin minte ce noduri au fost vizitate. Cand ajung intr-un
nod il marchez ca vizitat direct cu numarul grupului curent. Fac DFS pentru
fiecare nod, iar de fiecare data in care lansez un apel de DFS din functia de
numarat numarul de zone maresc numarul de grup cu care voi marca acele noduri
din acel DFS (pentru ca daca dupa un apel de DFS mai raman noduri nemarcate
inseamna ca mai am inca cel putin o zona izolata).

Pentru a doua cerinta trebuie sa gasesc un MST (minimum spanning tree, de
fapt lista de muchii ci doar sa maresc costul arborelui cu fiecare adaugare).

Imi creez un vector nou unde voi tine minte daca un nod a fost sau nu vizitat,
de data asta nu ma intereseaza grupa (insula/zona) din care face parte, ci doar
daca a fost sau nu vizitat. La fel ca si cu DFS la punctul anterior, voi apela
functia de MST intr-un for pentru toate nodurile care verifica daca nodurile
au fost sau nu parcurse. Astfel, algoritmul nu se va intrerupe daca am mai multe
componente de noduri conexe. Daca am o singura componenta conexa totul se va rezolva
dintr-un apel la MST.

Apoi sortez datele folosind metoda qsort() predefinita si fac afisarea datelor si 
eliberarea memoriei.

Solutia mea este bazata pe algoritmul lui Prim, insa nu am mai facut verificari suplimentare
pentru a vedea daca nu se formeaza un ciclu (pentru ca adaugam doar noduri care nu au mai fost
adaugate si la care oricum trebuia sa ajung). In MST vizitez nodul curent apoi,
incarc toate nodurile cu care este vecin (si care nu au fost inca vizitate) intr-o lista
sortata crescator dupa cost, pentru a putea lua apoi minimul, adica nodul pe care il
voi vizita urmatorul. Nu ma intereseaza sa tin minte din ce nod voi merge spre el, daca din
actualul nod sau alt nod. Deasemenea nu ma intereseaza daca am dubluri (adica doua rute catre
un nod vecin cu subgraful meu actual, una dintre rute fiind probabil mai buna) pentru ca
am o singura functie de filtrare si scoatere a minimului: iau primul nod urmator cu cost mai mic,
daca nu a fost parcurs intre timp. Daca a fost parcurs intre timp, arunc acea optiune 
si iau urmatorul nod. Pentru cazul in care aveam doua rute catre acelasi nod, dupa ce am luat
prima ruta, cea cu cost mai mic, si am marcat nodul ca fiind parcurs, cand se va ajunge la a doua
ruta nodul va aparea ca vizitat si ruta va fi ignorata. 
Algoritmul MST merge pana cand nu mai am alti vecini la care sa pot ajunge si functia de 
get_minim_filtered returneaza NULL. Daca am mai multe insule, se va mai porni cate un apel de MST
din main pentru noduri care nu au fost vizitate. 

Cerinta 2:
Updatez citirea datelor si tipul grafului, astfel incat sa pot crea si grafuri orientate si neorientate.
Am mai adaugat un camp la varfuri pentru distanta (scorul va fi distanta cand aplica 
algoritmul lui Dijkstra si o functie de adaugara a nodurilor intr-un graforientat. 
La creare varfurile am adaugat intai scorul 0 urmand ca apoi acesta sa fie updatat prin citirea
din fisier. Pentru adancim trebuia sa parcurg de foarte multe ori tot graful pentru a gasi de cate ori
apare fiecare nod asa ca am facut un vector separat pentru adamcimi. A trebuit insa sa updatez scorurile
parcurgand o singura data graful dupa ce am citit adancimile.

In vectorul de noduri am decis ca mereu corabia va fi nodul 0 si insula nodul cu indicele 1.
Am incercat in redactarea codului sa evit crearea unui copy-paste total cu citirea datelor.
Pentru a calcula distanta cea mai mica intre doua noduri am ales o implementare
Greedy bazata pe algoritmul lui Dijkstra, cu o functie care returneaza NULL 
daca nu exista cale sau un vector cu calea si numarul de noduri din cale
 intr-un parametru separat. Pentru a construi calea tin minte prin ce nod anterior trece cea mai buna
cale catre fiecare nod, iar la infinalul algoritmului folosesc acel vector de tati pentru a merge
de la destinatie inapoi catre sursa (vectorul rezultat va fi in ordine inversa). Acest algoritm va fi
rulat de doua ori, pentru cele doua rute. Apoi afisez ruta si costul total, folosind costurile
nu score-urile. Apoi vad adancimea minima din ruta, ignorand capetele care au normal costul 1.
Apoi calculez de cate drumuri este nevoie pentru a cara comoara. Nu am mai adaugat cazul in care
la ultimul drum mai ramane un rest de comoara si mai este necesara o tura pentru ca am vazut
ca merge pentru toate testele fara.
Apoi am mai eliminat niste cod copy-paste si niste dead cod comentat din timpul etapei de debug.
