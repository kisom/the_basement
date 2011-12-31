; quicklisp setup

(defparameter *base-packages*
  '( "drakma" "cxml" "cl-ppcre" "cl-oauth" "cl-postgres" "cl-twitter"
     "cl-json" "cl-mongo" "logv" ))

(load "~/src/quicklisp/quicklisp.lisp")

(quicklisp-quickstart:install)
(ql:add-to-init-file)
(ql:quickload *base-packages*)



