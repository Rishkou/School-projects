(set-logic UFLIA)
(set-info :status sat)

(declare-fun is_resource (Int) Bool)
(declare-fun requests (Int Int) Bool)
(declare-fun owns (Int Int) Bool)
(declare-fun in_deadlock (Int) Bool)

(define-fun is_process ((p Int)) Bool
  (or
    (= p 100)
    (= p 200)
    (= p 999)
    (= p 1001)
  )
)

; <script_input>

(assert finitely_many_processes)

(check-sat)
