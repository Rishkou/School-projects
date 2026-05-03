(set-logic UFLIA)

(set-info :status unsat)

(declare-fun is_process (Int) Bool)
(declare-fun is_resource (Int) Bool)

(define-fun owns ((p Int) (r Int)) Bool
  (or
    (and (= p 1) (= r 42))
    (and (= p 2) (= r 999))
  )
)

(define-fun requests ((p Int) (r Int)) Bool
  false
)

(define-fun in_deadlock ((p Int)) Bool
  (or
    (= p 1)
    (= p 2)
  )
)

; <script_input>

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
