#include "add_conditions.h"
#include <stddef.h>
#include <stdio.h>
#include "cnf.h"
#include "parse.h"

//
// LOGIN: xkomorr00
//
    
/** Funkce demonstrující vytvoření nové (arbitrárně vybrané) klauzule
* ve tvaru "A_{0,1,2} || -B_{0, 1, 2}" do výrokové formule
* @param formula výroková formule, do níž bude klauzule přidána
*/
void conditions_example(CNF* formula) {
    assert(formula != NULL);

    // vytvoření nové klauzule
    Clause* cl = create_new_clause(formula);

    // přidání proměnné A_{0,1,2} do klauzule
    // proměnná říká, že ulice spojující rozcestí 0 a 1 bude opravována v první fázi v den s indexem 2
    // cl - klauzule, do níž přidáváme literál
    // true - značí, že přidaný literál je pozitivní proměnná
    // FITST_PHASE_FLAG - značí, že aktuální proměnná je pro první fázi opravy
    // 0 - značí výchozí rozcestí s indexem 0
    // 1 - značí cílové rozcestí s indexem 1
    // 2 - značí den s indexem 2
    add_literal_to_clause(cl, true, FIRST_PHASE_FLAG, 0, 1, 2);

    // přidání proměnné -B_{0,1,2} do klauzule
    // proměnná říká, že ulice spojující rozcestí 0 a 1 nebude opravována v druhé fázi v den s indexem 2
    // cl - klauzule, do níž přidáváme literál
    // false - značí, že přidaný literál je negativní proměnná
    // SECOND_PHASE_FLAG - značí, že aktuální proměnná je pro druhou fázi opravy
    // 0 - značí výchozí rozcestí s indexem 0
    // 1 - značí cílové rozcestí s indexem 1
    // 2 - značí den s indexem 2
    add_literal_to_clause(cl, false, SECOND_PHASE_FLAG, 0, 1, 2);
}

/** Funkce vytvářející klauzule ošetřující podmínku 1 ze zadání
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_days počet dní
* @param num_of_crossroads počet rozcestí
* @param num_of_streets počet ulic
* @param neighbours seznamy sousedů
* @param streets seznam ulic
*/
void all_streets_min_one_day_of_first_phase_roadwork(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets) {

    assert(formula != NULL);
    assert(num_of_crossroads >= 2);
    assert(num_of_days > 0);

    // Každá klauzule bude vyjadřovat podmínku, že daná silnice je opravována
    // v první fázi alespoň v jednom z dostupných dnů
    // Existuje-li například silnice (0, 1) mezi rozcestími 0 a 1, pak pro
    // n dní bude mít klauzule tvar:
    // A_{0, 1, 0} || A_{0, 1, 1} || A_{0, 1, 2} || ... || A_{0, 1, n-1},
    // což ve formátu DIMACS odpovídá klauzuli:
    // A_{0, 1, 0} A_{0, 1, 1} A_{0, 1, 2} ... A_{0, 1, n-1} 0
    // (přičemž každá z proměnných A_{0, 1, d} je zakódována jako jedno celé číslo)
    for(unsigned street_idx = 0; street_idx < num_of_streets; ++street_idx) {
        Street street = streets[street_idx];
        Clause *cl = create_new_clause(formula);

        // Pro každou ulici vyjádříme podmínku, že musí být opravena v první fázi v alespoň jednom dni

        for(unsigned day = 0; day < num_of_days; ++day) {
            add_literal_to_clause(cl, true, FIRST_PHASE_FLAG, street.source, street.destination, day);
        }
    }

    /* Alternativní varianta:

    // iterujeme přes veškerá výstupní rozcestí
    for(unsigned src = 0; src < num_of_crossroads; ++src) {

        // iterujeme přes veškerá vstupní rozcestí, přičemž dle zadání vždy platí dst > src
        for(unsigned dst = src + 1; dst < num_of_crossroads; ++dst) {

            // zajímají nás pouze ty dvojice rozcestí, mezi nimiž skutečně existuje silnice
            if(are_neighbours(neighbours, src, dst)) {

                // dále budeme iterovat přes dny, tudíž zde vytvoříme čerstvou klauzuli
                Clause *cl = create_new_clause(formula);

                // iterujeme přes dny, abychom mohli přidávat odpovídající proměnné
                // do nově vytvořené klauzule
                for(unsigned day = 0; day < num_of_days; ++day) {
                    // přidání proměnné x_{src, dst, day} do klauzule cl
                    // příznaky vyjadřují, že jde o proměnnou v pozitivním tvaru a že odpovídá první fázi oprav
                    add_literal_to_clause(cl, true, FIRST_PHASE_FLAG, src, dst, day);
                }
            }
        }
    }*/
}

/** Funkce vytvářející klauzule ošetřující podmínku 2 ze zadání
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_days počet dní
* @param num_of_crossroads počet rozcestí
* @param num_of_streets počet ulic
* @param neighbours seznamy sousedů
* @param streets seznam ulic
*/
void all_streets_max_one_day_of_first_phase_roadwork(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets) {

    // Místo pro řešení úlohy

    assert(formula != NULL);
    assert(num_of_crossroads >= 2);
    assert(num_of_days > 0);



    for(unsigned street_idx = 0; street_idx < num_of_streets; ++street_idx) {
        Street street = streets[street_idx];

        for(unsigned day = 0; day < num_of_days-1; ++day) {
            for(unsigned day2 = day+1;day2< num_of_days;day2++)
            {
                Clause *cl = create_new_clause(formula);
                add_literal_to_clause(cl, false, FIRST_PHASE_FLAG, street.source, street.destination, day );
                add_literal_to_clause(cl, false, FIRST_PHASE_FLAG, street.source, street.destination, day2 );
            }
        }
    }

}

/** Funkce vytvářející klauzule ošetřující podmínku 3 ze zadání
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_days počet dní
* @param num_of_crossroads počet rozcestí
* @param num_of_streets počet ulic
* @param neighbours seznamy sousedů
* @param streets seznam ulic
*/
void second_phase_follows_first_immediately(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets) {

    assert(formula != NULL);
    assert(num_of_crossroads >= 2);
    assert(num_of_days > 0);


    for(unsigned street_idx = 0; street_idx < num_of_streets; ++street_idx) {
        Street street = streets[street_idx];
        //vytvarame clause cl3 ktory zakaze aby sa prve opravy diali v posledny den
        Clause *cl3 = create_new_clause(formula);
        add_literal_to_clause(cl3,false,FIRST_PHASE_FLAG,street.source,street.destination,num_of_days-1);
        //vytvarame clause cl4, ktory zakaze aby sa druhe opravy diali v prvy den
        Clause *cl4 = create_new_clause(formula);
        add_literal_to_clause(cl4,false,SECOND_PHASE_FLAG,street.source,street.destination,0);

        for(unsigned day = 0; day < num_of_days-1; ++day) {
            // mame ekvivalenicu medzi Aijd, Bijd+1 , z toho vzniknu dve implikacie, cize vytvaram dve klauzuly jednu pre kazdu stranu
            Clause *cl1 = create_new_clause(formula);
            add_literal_to_clause(cl1, false, FIRST_PHASE_FLAG, street.source, street.destination, day );
            add_literal_to_clause(cl1, true, SECOND_PHASE_FLAG, street.source, street.destination, day+1 );
            Clause *cl2 = create_new_clause(formula);
            add_literal_to_clause(cl2, false, SECOND_PHASE_FLAG, street.source, street.destination, day+1 );
            add_literal_to_clause(cl2, true, FIRST_PHASE_FLAG, street.source, street.destination, day );
        }
    }

}

/** Funkce vytvářející klauzule ošetřující podmínku 4 ze zadání
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_days počet dní
* @param num_of_crossroads počet rozcestí
* @param num_of_streets počet ulic
* @param neighbours seznamy sousedů
* @param streets seznam ulic
*/
void neighbour_streets_not_being_repaired_simultaneously(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets) {

    assert(formula != NULL);
    assert(num_of_crossroads >= 2);
    assert(num_of_days > 0);

    //prejdeme cez vsetky dvojice ulic
    for(unsigned street_idx1 = 0; street_idx1 < num_of_streets-1; ++street_idx1)  {
        for(unsigned street_idx2 = street_idx1+1; street_idx2 < num_of_streets; ++street_idx2){
            Street street1 = streets[street_idx1];
            Street street2 = streets[street_idx2];
            //  kontrolujeme ci maju ulice spolocny nejaky uzol teda ci su susedia
            if(street1.destination == street2.destination || street1.destination == street2.source || street1.source == street2.destination || street1.source == street2.source)
            {


                for(unsigned day = 0; day < num_of_days; ++day) {
                    // vytvoril som si implikaciu ze Xijd => NOT(Yijd), po rozpisani tejto implikacie vzniknu 4 disjunkcie, kvoli dvom fazam opprav

                    // NOT XAijd v NOT YAijd
                    Clause *cl1 = create_new_clause(formula);
                    add_literal_to_clause(cl1,false,FIRST_PHASE_FLAG,street1.source,street1.destination,day);
                    add_literal_to_clause(cl1,false,FIRST_PHASE_FLAG,street2.source,street2.destination,day);
                    //NOT XAijd v NOT YBijd
                    Clause *cl2 = create_new_clause(formula);
                    add_literal_to_clause(cl2,false,FIRST_PHASE_FLAG,street1.source,street1.destination,day);
                    add_literal_to_clause(cl2,false,SECOND_PHASE_FLAG,street2.source,street2.destination,day);
                    //NOT XBijd v NOT YAijd
                    Clause *cl3 = create_new_clause(formula);
                    add_literal_to_clause(cl3,false,SECOND_PHASE_FLAG,street1.source,street1.destination,day);
                    add_literal_to_clause(cl3,false,FIRST_PHASE_FLAG,street2.source,street2.destination,day);
                    //NOT XBijd v NOT YBijd  
                    Clause *cl4 = create_new_clause(formula);
                    add_literal_to_clause(cl4,false,SECOND_PHASE_FLAG,street1.source,street1.destination,day);
                    add_literal_to_clause(cl4,false,SECOND_PHASE_FLAG,street2.source,street2.destination,day);
                }
            }
        }
    }
}

/** Funkce vytvářející klauzule ošetřující podmínku 5 ze zadání
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_days počet dní
* @param num_of_crossroads počet rozcestí
* @param num_of_streets počet ulic
* @param neighbours seznamy sousedů
* @param streets seznam ulic
*/
void each_day_at_least_one_street_being_repaired(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets) {

    assert(formula != NULL);
    assert(num_of_crossroads >= 2);
    assert(num_of_days > 0);

    for(unsigned day = 0; day < num_of_days; ++day){

        Clause *cl = create_new_clause(formula);

        for(unsigned street_idx = 0; street_idx < num_of_streets; ++street_idx) {
            Street street = streets[street_idx];
            add_literal_to_clause(cl, true, FIRST_PHASE_FLAG, street.source, street.destination, day);
            add_literal_to_clause(cl, true, SECOND_PHASE_FLAG, street.source, street.destination, day);
        }
    }

}

/** Funkce vytvářející klauzule ošetřující podmínku 6 ze zadání
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_days počet dní
* @param num_of_crossroads počet rozcestí
* @param num_of_streets počet ulic
* @param neighbours seznamy sousedů
* @param streets seznam ulic
*/
void street_between_0_and_1_repaired_in_last_two_days(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets) {


    assert(formula != NULL);
    assert(num_of_crossroads >= 2);
    assert(num_of_days > 0);
    //zistujeme ci existuje ta ulica
    if(are_neighbours(neighbours,0,1))
    {
        
        for(unsigned day = 0;day<num_of_days;day++) {
            Clause *cl = create_new_clause(formula);
            // ak sme v predpredposlednom dni, robime opravu A
            if (day == num_of_days-2)
                add_literal_to_clause(cl, true, FIRST_PHASE_FLAG, 0, 1, day);
            // v predposlednom dni robime opravu B
            if(day == num_of_days-1)
                add_literal_to_clause(cl, true, SECOND_PHASE_FLAG, 0,1, day);
            // inac neopravujeme 
            if (num_of_days-2 > day)
            {
                add_literal_to_clause(cl,false,FIRST_PHASE_FLAG,0,1,day);
                add_literal_to_clause(cl,false,SECOND_PHASE_FLAG,0,1,day);
            }
        }
    }

}

/** Funkce vytvářející klauzule ošetřující podmínku 7 ze zadání
* @param formula výroková formule, do níž bude klauzule přidána
* @param num_of_days počet dní
* @param num_of_crossroads počet rozcestí
* @param num_of_streets počet ulic
* @param neighbours seznamy sousedů
* @param streets seznam ulic
*/
void no_street_to_0_repaired_during_weekend(CNF *formula, unsigned num_of_days, unsigned num_of_crossroads, unsigned num_of_streets, const NeighbourLists *neighbours, const Street *streets) {

    
    assert(formula != NULL);
    assert(num_of_crossroads >= 2);
    assert(num_of_days > 0);
    for(unsigned street_idx = 0; street_idx < num_of_streets; ++street_idx)  
    {
        Street street = streets[street_idx];
        // ak je spojene s letiskom ulica,
        if(are_neighbours(neighbours,0,street.destination))
        {


            for(unsigned days =0; days<num_of_days;days++)
            {   

                if(days%7 == 5 || days%7 == 6)
                {
                    // clause ktora zaistuje ze sa nedeje prva faza opravy pocas vikendu
                    Clause *cl1 = create_new_clause(formula);
                    // claue ktora zaistuje ze sa nedeje druha faza opravy
                    Clause *cl2 = create_new_clause(formula);
                    add_literal_to_clause(cl1,false,FIRST_PHASE_FLAG,street.source,street.destination,days);
                    add_literal_to_clause(cl2,false,SECOND_PHASE_FLAG,street.source,street.destination,days);
                }

            }

        }


    }
}
