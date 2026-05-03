(set-logic UFLIA)
(set-info :status sat)

(declare-fun requests (Int Int) Bool)
(declare-fun in_deadlock (Int) Bool)

(define-fun is_process ((p Int)) Bool
  (= (mod p 2) 0)
)

(define-fun is_resource ((r Int)) Bool
  (= (mod r 2) 1)
)

(define-fun owns ((p Int) (r Int)) Bool
  (= (+ p 1) r)
)

; <script_input>

(assert exclusive_resources)

(check-sat)
