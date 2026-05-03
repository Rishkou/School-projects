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
    (is_process 0)
    (is_process 1)
    (is_resource 200)
    (is_resource 201)
    (owns 0 200)
    (owns 1 201)
    exclusive_resources
  )
)

(check-sat)
