(set-logic UFLIA)
(set-info :status sat)

(declare-fun is_process (Int) Bool)
(declare-fun is_resource (Int) Bool)
(declare-fun requests (Int Int) Bool)
(declare-fun owns (Int Int) Bool)
(declare-fun in_deadlock (Int) Bool)

; <script_input>

(assert
  (and
    (is_process 1000000)
    finitely_many_processes
  )
)

(check-sat)
