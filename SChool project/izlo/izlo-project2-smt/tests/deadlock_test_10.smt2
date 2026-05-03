(set-logic UFLIA)
(set-info :status unsat)

(define-fun is_process ((p Int)) Bool true)
(define-fun is_resource ((r Int)) Bool false)
(define-fun owns ((p Int) (r Int)) Bool true)
(define-fun requests ((p Int) (r Int)) Bool true)
(define-fun in_deadlock ((p Int)) Bool
  (or
    (= p 1)
    (= p 2)
  )
)

; <script_input>

(assert has_deadlock)

(check-sat)
