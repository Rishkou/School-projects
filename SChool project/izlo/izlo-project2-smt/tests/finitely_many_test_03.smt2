(set-logic UFLIA)
(set-info :status unsat)

(declare-fun is_resource (Int) Bool)
(declare-fun requests (Int Int) Bool)
(declare-fun owns (Int Int) Bool)
(declare-fun in_deadlock (Int) Bool)

(define-fun is_process ((p Int)) Bool
  (distinct p 0)
)

; <script_input>

(assert finitely_many_processes)

(check-sat)
