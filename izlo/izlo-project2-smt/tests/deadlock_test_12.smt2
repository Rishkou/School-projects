(set-logic UFLIA)

(set-info :status sat)

(define-fun is_process ((p Int)) Bool
  (and
    (>= p 0)
    (< p 21)
  )
)
(define-fun is_resource ((r Int)) Bool
  (>= r 100)
)

(define-fun owns ((p Int) (r Int)) Bool
  (and
    (is_process p)
    (= (+ p 100) r)
  )
)

(define-fun requests ((p Int) (r Int)) Bool
  (or
    (and (= p 20) (= r 100))
    (and
      (is_process p)
      (is_resource r)
      (= (+ p 101) r)
    )
  )
)

(define-fun in_deadlock ((p Int)) Bool
  (and
    (>= p 0)
    (< p 21)
  )
)

; <script_input>

(assert has_deadlock)

(check-sat)
