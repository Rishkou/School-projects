(set-logic UFLIA)

(set-info :status sat)

(declare-fun is_process (Int) Bool)
(declare-fun is_resource (Int) Bool)

(define-fun owns ((p Int) (r Int)) Bool
  (or
    (and (= p 1) (= r 42))
    (and (= p 2) (= r 999))
  )
)

(define-fun requests ((p Int) (r Int)) Bool
  (or
    (and (= p 1) (= r 999))
    (and (= p 2) (= r 42))
  )
)

(define-fun in_deadlock ((p Int)) Bool
  (or
    (= p 1)
    (= p 2)
  )
)

;; START OF SOLUTION
;; ==================

; XLOGIN: xkomorr00

;; Formule 1
(define-fun exclusive_resources () Bool
  ; Zde nahraďte vaším řešením
  ;zobereme vsetky i - resources, a b su porcesy, -> pre kazde i, ktore je resource, ak a je pridelene a b je pridelene => a = b
  (forall ((i Int) (a Int) (b Int))
    (=>
      (and
          (is_resource i)
          (is_process a)
          (is_process b)
          (owns a i)
          (owns b i)
      )
      (= a b)
    )
  )
)

;; Formule 2
(define-fun finitely_many_processes () Bool
  ; Zde nahraďte vaším řešením
  ; existuje x,y take ze x > identifikator procesu a zaroven y < identifikator procesu
  (exists ((x Int)(y Int))
    (forall ((i Int))
      (=>
        (is_process i)
      
        (and
            (> x i)
            (> i y)
        )
      )
    )
  )
)

;; Formule 3
(define-fun has_deadlock () Bool
  ; Zde nahraďte vaším řešením
  ; pre kazde i, ktore je v mnozine dedlok  plati ze 

  ;1) mnozina obsahuje aspon 1 proces p
  ;2) pre kazdy jej proces plati ze caka na pridelenie zdroju, ktory je vlastneny -> forall proces p, exists zdroj r taky ze (requests p r)

  (and
    ;mnozina obsahuje aspon jeden proces p
    (exists ((p Int))
      (and
        (in_deadlock p)
        (is_process p)
      )
    )
    ; pre kazdy proces p v dedlocku, existuje resource r requests(p r ) a zaroven r vlastni nejaky p2
     (forall ((p Int))
      (=>
        (and
          (in_deadlock p)
          (is_process p)
        )
        (exists ((r Int)(p2 Int))
          (and
            (is_resource r)
            (in_deadlock p2)
            (is_process p2)
            (requests p r)
            (owns p2 r)
          )
        )
      )
    )
  )
)
;; ==================
;; END OF SOLUTION

(assert
  (and
    (is_process 1)
    (is_process 2)
    (is_resource 42)
    (is_resource 999)

    has_deadlock
  )
)

(check-sat)
