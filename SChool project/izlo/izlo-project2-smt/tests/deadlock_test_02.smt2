(set-logic UFLIA)
(set-info :status sat)

(declare-fun is_process (Int) Bool)
(declare-fun is_resource (Int) Bool)

(define-fun owns ((p Int) (r Int)) Bool
  (and
    (= p 157)
    (or
      (= r 2)
      (= r 3)
      (= r 4)
    )
  )
)


(define-fun requests ((p Int) (r Int)) Bool
  (and
    (= p 157)
    (= r 3)
  )
)

(define-fun in_deadlock ((p Int)) Bool
  (= p 157)
)

; <script_input>

(assert
  (and
    (is_process 157)
    (is_resource 2)
    (is_resource 3)
    (is_resource 4)

    has_deadlock
  )
)

(check-sat)
