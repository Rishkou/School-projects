(set-logic UFLIA)
(set-info :status sat)

(declare-fun in_deadlock (Int) Bool)

(define-fun is_process ((p Int)) Bool true)
(define-fun is_resource ((r Int)) Bool false)
(define-fun owns ((p Int) (r Int)) Bool true)
(define-fun requests ((p Int) (r Int)) Bool true)

; <script_input>

(assert exclusive_resources)

(check-sat)
